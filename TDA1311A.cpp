/*
 * Copyright 2017 Hedde Bosman
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "TDA1311A.h"

#if defined(TDA1311A_PIN_CLOCK) && defined(TDA1311A_PIN_CHAN) && defined(TDA1311A_PIN_DATA)
TDA1311A::TDA1311A() {
	setOutput(0,0);
}
#else
TDA1311A::TDA1311A(const uint8_t pin_clock, const uint8_t pin_chan, const uint8_t pin_data) : 
		TDA1311A_PIN_CLOCK(pin_clock),
		TDA1311A_PIN_CHAN(pin_chan),
		TDA1311A_PIN_DATA(pin_data)
{
	setOutput(0,0);
}
#endif

TDA1311A::~TDA1311A() {

}

void TDA1311A::setup() {
	pinMode(TDA1311A_PIN_CLOCK,		OUTPUT);
	pinMode(TDA1311A_PIN_CHAN,		OUTPUT);
	pinMode(TDA1311A_PIN_DATA,		OUTPUT);
}


void TDA1311A::sendOutput() {
	sendOutput(TDA1311A_CHAN_LEFT);
	sendOutput(TDA1311A_CHAN_RIGHT);
}


		
/* data: msb first
 * DAC samplerate is about 192 kHz, so it depends on the microcontroller how fast it can send
 * bit clock cycle time minimal 54 ns
 * bit clock pulse width minimal 15 ns
 */

#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
/****************************************************************************************/
/* 16 bit version: */
/****************************************************************************************/
void TDA1311A::sendOutput(uint8_t channel) {
	uint16_t mask;

	digitalWriteFast(TDA1311A_PIN_CHAN, (channel == TDA1311A_CHAN_LEFT ? TDA1311A_CHAN_LEFT : TDA1311A_CHAN_RIGHT));

	for (mask = 0x8000; mask; mask >>= 1) {
		digitalWriteFast(TDA1311A_PIN_CLOCK, LOW);
		digitalWriteFast(TDA1311A_PIN_DATA, (_last_data[channel].data & mask ? HIGH : LOW));
		// data is clocked in on rising edge
		digitalWriteFast(TDA1311A_PIN_CLOCK, HIGH);
	}
}
#else
/****************************************************************************************/
/* 8 bit version: */
/****************************************************************************************/
void TDA1311A::sendOutput(uint8_t channel) {
	uint8_t mask;

	digitalWriteFast(TDA1311A_PIN_CHAN, (channel == TDA1311A_CHAN_LEFT ? TDA1311A_CHAN_LEFT : TDA1311A_CHAN_RIGHT));

	for (mask = 0x80; mask; mask >>= 1) {
		digitalWriteFast(TDA1311A_PIN_CLOCK, LOW);
		digitalWriteFast(TDA1311A_PIN_DATA, (_last_data[channel].datab[1] & mask ? HIGH : LOW));
		// data is clocked in on rising edge
		digitalWriteFast(TDA1311A_PIN_CLOCK, HIGH);
	}
	for (mask = 0x80; mask; mask >>= 1) {
		digitalWriteFast(TDA1311A_PIN_CLOCK, LOW);
		digitalWriteFast(TDA1311A_PIN_DATA, (_last_data[channel].datab[0] & mask ? HIGH : LOW));
		// data is clocked in on rising edge
		digitalWriteFast(TDA1311A_PIN_CLOCK, HIGH);
	}
}
#endif


