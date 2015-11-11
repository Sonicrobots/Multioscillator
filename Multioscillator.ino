#include "portManipulations.h"
#include "multiChannelOscillator.h"
#include <util/delay.h>

#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
int led = 13;

extern MultiChannelOscillator oscil;

MIDI_CREATE_DEFAULT_INSTANCE()

uint16_t const frequency[128] PROGMEM = {8, 9, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 18, 19, 21, 22, 23, 24, 26, 28, 29, 31, 33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62, 65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 5920, 6645, 7040, 7459, 7902, 8372, 8870, 9397, 9956, 10548, 11175, 11840, 12544}; 
 
uint8_t pinIndices[8]  = {2,  3,  4,  5,  6,  7,  0,  1};
    //                    D3, D4, D5, 
uint16_t frequencies[8] = {0,0,1000,1000,1000,1000,0,1000};

void setup() {

	pinMode(led, OUTPUT);

	_delay_ms(1000);
  
	TIMSK2 = 0; // kill delay and millis because that interrupt messes with ours
	TIMSK0 = 0;

	oscil.init(pinIndices);
	oscil.setFrequencies(frequencies);
	oscil.start();
      
	MIDI.begin(MIDI_CHANNEL_OMNI);                     // listens on only channel XXX
	MIDI.setHandleNoteOn(HandleNoteOn); // connect the "pitchbend" command from the midi.h library with the "pitchbend()" function defined below!
	MIDI.setHandleNoteOff(HandleNoteOff); // connect the "pitchbend" command from the midi.h library with the "pitchbend()" function defined below!


}



void loop() {
       
	MIDI.read(); // If we have received a message
	oscil.fillBuffer();

}




void HandleNoteOn(byte channel, byte note, byte velocity)
{          
  	oscil.setFrequency(/*channel*/ 1, (unsigned int)pgm_read_word(&frequency[note])); 
  	digitalWrite(led, HIGH);
}

void HandleNoteOff(byte channel, byte note, byte velocity)
{       

  	oscil.setFrequency(/*channel*/ 1, 10); 
	digitalWrite(led, LOW);
}


