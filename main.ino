/*
 *
 * Multi Channel Oscillator
 *
 * Written by Lennart Schierling (Lennart@bastl-instruments.com)
 *
 * Multi Channel MIDI to Square Wave Oscillator
 *
 *
 */



// Includes for Arduino IDE
#ifdef EXTERNAL_IDE
#include <Arduino.h>
int main(void) {
  init();
  setup();
  while(true) {
    loop();
  }
}
#endif




#include "settings.h"
#include "portManipulations.h"

#include "multiChannelOscillator.h"
extern MultiChannelOscillator oscil;
#include "NoteManager.h"

#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE()


// Readout the coded switch on PIN 2,3,4,7 on Startup. To set the MIDI Channel (1-16)
uint8_t readMidiChannel()
{
	// set pins as inputs
	pinMode(3, INPUT_PULLUP);
	pinMode(7, INPUT_PULLUP);
	pinMode(2, INPUT_PULLUP);	
	pinMode(4, INPUT_PULLUP);	

	uint8_t i=0;
	bitWrite(i, 0, !digitalRead(3));
	bitWrite(i, 1, !digitalRead(7));
	bitWrite(i, 2, !digitalRead(2));
	bitWrite(i, 3, !digitalRead(4));

	// midi channel starts with one not zero
	return i+1; 
}




void setup() {

	// disable Timer 0 interrupt from arduino's init()
	// this will disable delay() and millis()
	TIMSK0 = 0;

	// Enable debug pin
	bit_dir_outp(PIN);

	// Initialize NoteManager
	NoteManger::init();

	// Initialize MIDI
	MIDI.begin(readMidiChannel());
	MIDI.setThruFilterMode(midi::Off);
	MIDI.setHandleNoteOn(NoteManger::HandleNoteOn);
	MIDI.setHandleNoteOff(NoteManger::HandleNoteOff);
	MIDI.setHandlePitchBend(NoteManger::HandlePitchBend);

	// Initialize Oscillator
	uint8_t pins[NUMB_CHANNELS] = {0,1,2,3};
	oscil.init(pins);

}

void loop() {

	oscil.fillBuffer();
	MIDI.read();
}



