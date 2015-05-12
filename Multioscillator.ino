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
  int velocitymap = map(velocity, 0, 127, 1, 20);
  
  switch (note) {
    case 48:
  	oscil.setFrequency(/*channel*/ 1, velocitymap); 
      break;
    case 49:
  	oscil.setFrequency(/*channel*/ 2, velocitymap); 
      break;
    case 50:
  	oscil.setFrequency(/*channel*/ 3, velocitymap); 
      break;
    case 51:
  	oscil.setFrequency(/*channel*/ 4, velocitymap); 
      break;
  }
  	digitalWrite(led, HIGH);
}

void HandleNoteOff(byte channel, byte note, byte velocity)
{       
  switch (note) {
    case 48:
  	oscil.setFrequency(/*channel*/ 1, 0); 
          digitalWrite(4, LOW);
      break;
    case 49:
  	oscil.setFrequency(/*channel*/ 2, 0); 
          digitalWrite(5, LOW);
      break;
    case 50:
  	oscil.setFrequency(/*channel*/ 3, 0); 
          digitalWrite(6, LOW);
      break;
    case 51:
  	oscil.setFrequency(/*channel*/ 4, 0); 
          digitalWrite(7, LOW);
      break;


}
	digitalWrite(led, LOW);
}


