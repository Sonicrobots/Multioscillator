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
	static void HandlePitchBend(uint8_t midiChannel, int value);

private:
	static void createTable();

	// lookup table from midi note to oscillator compare value
	static uint16_t comparevalueTable[128];

	// number of channels and number of ones that are playing
	static const uint8_t numbChannels = NUMB_CHANNELS;
	static uint8_t numbActiveChannels ;

	// MIDI notes being played on each channel
	static const uint8_t channelOFF = 255;
	static uint8_t channelNotes[numbChannels];

	// order of activation of channels
	static uint8_t channelOrder[numbChannels];




};








#endif /* NOTEMANAGER_H_ */
