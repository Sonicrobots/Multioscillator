/*
 * NoteManager
 *
 * Written by Lennart Schierling (Lennart@bastl-instruments.com)
 *
 * Features:
 * 	* Map MIDI notes to multiChannelsOscillator's compare values
 * 	* lookup table generation using floating point calculation
 * 	* handle polyphony
 *
 */

#ifndef NOTEMANAGER_H_
#define NOTEMANAGER_H_

#include "settings.h"
#include <inttypes.h>



class NoteManger {

public:
	static void init();

public:
	// use those as callbacks for Arduino Midi Library
	static void HandleNoteOn(uint8_t midiChannel, uint8_t note, uint8_t velocity);
	static void HandleNoteOff(uint8_t midiChannel, uint8_t note, uint8_t velocity);

private:
	static void createTable();
	static uint16_t comparevalueTable[128];

	static const uint8_t numbChannels = NUMB_CHANNELS;
	static const uint8_t channelOFF = 255;
	static uint8_t channelNotes[numbChannels];
	static uint8_t channelOrder[numbChannels];
	static uint8_t numbActiveChannels ;

};








#endif /* NOTEMANAGER_H_ */
