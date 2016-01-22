/*
 * NoteManager.cpp
 *
 *  Created on: 22.01.2016
 *      Author: user
 */

#include "NoteManager.h"

#include "multiChannelOscillator.h"
extern MultiChannelOscillator oscil;


uint16_t NoteManger::comparevalueTable[128];
uint8_t NoteManger::channelNotes[numbChannels];
uint8_t NoteManger::channelOrder[numbChannels];
uint8_t NoteManger::numbActiveChannels;


void NoteManger::HandleNoteOn(uint8_t midiChannel, uint8_t note, uint8_t velocity) {

	// free channel available, find it and use it
	if (numbActiveChannels < numbChannels) {

		for (uint8_t i=0; i<numbChannels; i++) {
			if (channelNotes[i] == channelOFF) {
				channelNotes[i] = note;
				numbActiveChannels++;
				channelOrder[i] = numbActiveChannels;
				oscil.setComparevalue(i,comparevalueTable[note]);
				return;
			}
		}

	// no free channel; replace oldest one
	} else {
		for (uint8_t i=0; i<numbChannels; i++) {

			// find the oldest channel
			if (channelOrder[i] == 1) {
				channelNotes[i] = note;
				channelOrder[i] = numbActiveChannels; // set it as the newest
				oscil.setComparevalue(i,comparevalueTable[note]);
			} else {
				// all other channels get one step older
				channelOrder[i]--;
			}
		}
	}

}

void NoteManger::HandleNoteOff(uint8_t midiChannel, uint8_t note, uint8_t velocity) {

	for (uint8_t i=0; i<numbChannels; i++) {
		if (channelNotes[i] == note) {
			channelNotes[i] = channelOFF;
			numbActiveChannels--;
			oscil.setComparevalue(i,0);
		}
	}
}



void NoteManger::init() {
	createTable();

	for (uint8_t c=0; c<numbChannels; c++) {
		channelNotes[c] = channelOFF;
		channelOrder[c] = 0;
	}

	numbActiveChannels = 0;
}


void NoteManger::createTable() {
	const float noteInterval = 1.0594630944; // approximation of 12th root of 2
	const float c_zero = 8.176;
	const uint16_t prescaler = PRESCALER;

	float freq = c_zero;
	for (uint8_t i=0; i<128; i++) {
		comparevalueTable[i] = (uint16_t) (F_CPU / (2 * prescaler * freq));
		freq *= noteInterval;
	}

}
