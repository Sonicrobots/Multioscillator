#include "multiChannelOscillator.h"

extern MultiChannelOscillator oscil;

uint8_t pinIndices[8] = {2,3,4,5,6,7,0,1};
uint16_t frequencies[8] = {153,185,267,327,465,1023,587,845};

void setup() {
        TIMSK2 = 0; // disable millis() interrrupt from Arduino because it interferes with our interrupt
        oscil.init(pinIndices);
        oscil.setFrequencies(frequencies);
        oscil.start();
        oscil.fillBuffer();
}

void loop() {
        //... modify 'frequencies' as desired

        // then update the oscillator
        oscil.setFrequencies(frequencies);
}

