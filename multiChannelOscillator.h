/*
 * multiChannelOscillator.h
 *
 *  Written by Lennart Schierling (Lennart@bastl-instruments.com)
 *
 *  Features: Up to 8 square wave oscillators with different frequencies on on port of ATMEGA328
 *  External settings that need to be provided by defines:
 *  	* PRESCALER: Defines tick size of square wave generation (avr-gcc 4.9 supports only 256)
 *  	* NUMB_CHANNELS: Number of oscillator channels (1 to 8)
 *
 *  */

#ifndef MULTICHANNELOSCILLATOR_H_
#define MULTICHANNELOSCILLATOR_H_

#include "settings.h"

#include <inttypes.h>
#include "FiFoBuffer.h"


// Tune lowest value possible that doesn't create underruns
#define EVENT_BUFFER_SIZE 10


struct toggleEvent {
	uint16_t time;
	uint8_t bits;
};



class MultiChannelOscillator {

public:

	// initialize the hardware and set the pins that are controlled by each channel
	// takes pointer to an array of length NUMB_CHANNELS with values from 0 to 7
	void init(uint8_t* pinIndices);

	// set frequency of individual channel by directly setting its divider in from F_CPU/PRESCALER
	void setComparevalue(uint8_t channel, uint16_t comparevalue);

	// fill up the event buffer
	// call this regularly to avoid an underrun
	void fillBuffer();

	static const uint8_t eventBufferSize = EVENT_BUFFER_SIZE;
	static const uint8_t numbChannels = NUMB_CHANNELS;
	static const uint16_t prescaler = PRESCALER;


public:
	// called in ISR. This checks if a toggle event is present for the current call tick
	void performToggle();


private:

	uint8_t channelMappings[numbChannels];
	int16_t compareValues[numbChannels];
	int16_t currentCompareValues[numbChannels];

	void queueNextToggle();
	FiFoBuffer<eventBufferSize,toggleEvent> buffer;

	// turn channel output on or off
	uint8_t channelEnabled;
	void setChannelActive(uint8_t channel, bool val);


};





#endif /* MULTICHANNELOSCILLATOR_H_ */
