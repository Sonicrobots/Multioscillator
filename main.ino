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




void setup() {

	// disable Timer 0 interrupt from arduino's init()
	// this will disable delay() and millis()
	TIMSK0 = 0;

	// Enable debug pin
	bit_dir_outp(PIN);

	// Initialize NoteManager
	NoteManger::init();

	// Initialize MIDI
	MIDI.begin();
	MIDI.setThruFilterMode(midi::Off);
	MIDI.setHandleNoteOn(NoteManger::HandleNoteOn);
	MIDI.setHandleNoteOff(NoteManger::HandleNoteOff);

	// Initialize Oscillator
	uint8_t pins[NUMB_CHANNELS] = {0,1,2,3,4};
	oscil.init(pins);

}

void loop() {

	oscil.fillBuffer();
	MIDI.read();
}
