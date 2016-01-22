/*
 * multiChannelOscillator.cpp
 *
 *  Created on: 23.10.2014
 *      Author: Lennart Schierling for bastl-instruments
 */


#include "multiChannelOscillator.h"

#include <Arduino.h>
#include <util/atomic.h>

#include "portManipulations.h"





// macros for accessing port register
// they take the port indication letter (A to D) as argument
// and turn them into the corresponding register (PIN, PORT or DIR)
#define REG_PIN(...) REG_PIN_(__VA_ARGS__)
#define REG_PIN_(L) PIN ## L

#define REG_PORT(...) REG_PORT_(__VA_ARGS__)
#define REG_PORT_(L) PORT ## L

#define REG_DIR(...) REG_DIR_(__VA_ARGS__)
#define REG_DIR_(L) DDR ## L


void inline error() {
	while(1) {
		bit_set(PIN);
		bit_clear(PIN);
	}
}



void MultiChannelOscillator::init(uint8_t* pinIndices) {


	// save pin Indices
	for (uint8_t index=0; index<numbChannels; index++) {
		this->channelMappings[index] = (1<<pinIndices[index]);
	}

	// Set up tick interrupt
	// Tick length is F_CPU/PRESCALER
	TCCR1A = 0;
	TCCR1B = (1<<WGM12); 	// CTC
	switch (prescaler) {
	case 64:
		TCCR1B |= (1<<CS10) | (1<<CS11);
		break;
	case 256:
		TCCR1B |= (1<<CS12);
		break;
	default:
		while (1); // error -> halt
	}


	// Initialize channels
	for (uint8_t channel=0; channel<numbChannels; channel++) {
		// Set up all pins as outputs
		REG_DIR(OSCIL_PORT) |= channelMappings[channel];

		// set dummy comparevalues to keep buffer alive
		setComparevalue(channel,20);

		// disable all channels
		channelEnabled = 0;
		setChannelActive(channel, false);
	}

	// fill up event buffer
	fillBuffer();

	// loader interrupt with first event from buffer
	toggleEvent event;
	buffer.get(event);
	OCR1A = event.time;

	// start up
	setHigh(TIMSK1,OCIE1A);
}


void MultiChannelOscillator::setComparevalue(uint8_t channel, uint16_t comparevalue) {

	if (comparevalue != 0) {
		compareValues[channel] = comparevalue;
		setChannelActive(channel, true);
	} else {
		setChannelActive(channel, false);
	}
}



void MultiChannelOscillator::setChannelActive(uint8_t channel, bool val) {

	if (val) {
		setHigh(channelEnabled,channel);
	} else {
		setLow(channelEnabled,channel);
		REG_PORT(OSCIL_PORT) &= ~(channelMappings[channel]);
	}
}



void MultiChannelOscillator::fillBuffer() {

	while(!buffer.isFull()) {
		queueNextToggle();
	}
}


inline void MultiChannelOscillator::queueNextToggle() {

	// the last step that as been calculated
	static uint16_t lastTime = 0;
	static uint8_t lastPins = 0;

	// add pin action that has been calculated in previous call
	toggleEvent event;
	event.bits = lastPins;


	// get new time and pins
	uint16_t distanceToNext = -1;


	// go through all pins and find the one that needs to be toggled next
	for (uint8_t index=0; index<numbChannels; index++) {

		// calculate situation when last toggle event will be processed
		// lastTime number of ticks will have passed
		currentCompareValues[index] -= lastTime;
		if (currentCompareValues[index] <= 0) currentCompareValues[index] = compareValues[index];

		// now find the channel that will be trigged next (smallest currentCompareValue)

		// if the current one is smaller that the currently smallest one, take that
		if (currentCompareValues[index] < distanceToNext) {
			lastPins = channelMappings[index];
			distanceToNext = currentCompareValues[index];
		}

		// if the current one is equally small, flip its bit also
		if (currentCompareValues[index] == distanceToNext) {
			lastPins |= channelMappings[index];
		}

	}

	// if no next event has been found, this is an error and chip will halt
	if (distanceToNext == -1) while(1);

	// correct time it takes to call ISR and function it calls
	//if (distanceToNext != 0) distanceToNext-=1;

	// store calculated time and keep it for next run
	event.time = distanceToNext;
	//if (event.time>2) event.time--;


	lastTime = distanceToNext;


	// add event to buffer
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
	buffer.add(event);
	}

}


inline void MultiChannelOscillator::performToggle() {

	toggleEvent event;
	if (buffer.get(event)) {

		// correct timing (jitter?)
		if (event.time>1) event.time-=1;
		OCR1A = event.time;

		// only flip active channels
		REG_PIN(OSCIL_PORT) = event.bits & channelEnabled;

	} else {
		error();
	}
}




MultiChannelOscillator oscil;

//max 64 cycles = 4us
ISR(TIMER1_COMPA_vect) {
	oscil.performToggle();
}



