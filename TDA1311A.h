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
#ifndef __TDA1311A_H__
#define __TDA1311A_H__

#if (ARDUINO >= 100)
	#include <Arduino.h>
#else
	#include <WProgram.h>
	#include <pins_arduino.h>
#endif

// assume 'HIGH' is defined as 1...
#define TDA1311A_CHAN_LEFT	 HIGH
#define TDA1311A_CHAN_RIGHT  LOW


/* compiler optimization of digitalWriteFast only works 
 * when __builtin_constant_p passes, which is with constant values only.
 * 'const varname' doesn't seem to work for that, but #defines or template<...> will. Templates are more cumbersome though.
 * We define some defaults here for Teensy's'
 */
#ifndef TDA1311A_PIN_CLOCK
	#if defined(__AVR_ATmega32U4__)
		// teensy 2.0
		#define TDA1311A_PIN_CLOCK	PIN_F7
		#warning "Using default 'PIN_F7' for TDA1311A_PIN_CLOCK"
	#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
		// teensy++ 2.0
		#define TDA1311A_PIN_CLOCK	PIN_C0
		#warning "Using default 'PIN_C0' for TDA1311A_PIN_CLOCK"
	#else
		#warning "TDA1311A_PIN_CLOCK not defined"
	#endif
#endif


#ifndef TDA1311A_PIN_CHAN
	#if defined(__AVR_ATmega32U4__)
		// teensy 2.0
		#define TDA1311A_PIN_CHAN	PIN_F5
		#warning "Using default 'PIN_F5' for TDA1311A_PIN_CHAN"
	#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
		// teensy++ 2.0
		#define TDA1311A_PIN_CHAN	PIN_C1
		#warning "Using default 'PIN_C1' for TDA1311A_PIN_CHAN"
	#else
		#warning "TDA1311A_PIN_CHAN not defined"
	#endif
#endif


#ifndef TDA1311A_PIN_DATA
	#if defined(__AVR_ATmega32U4__)
		// teensy 2.0
		#define TDA1311A_PIN_DATA	PIN_F6
		#warning "Using default 'PIN_F6' for TDA1311A_PIN_DATA"
	#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
		// teensy++ 2.0
		#define TDA1311A_PIN_DATA	PIN_C2
		#warning "Using default 'PIN_C2' for TDA1311A_PIN_DATA"
	#else
		#warning "TDA1311A_PIN_DATA not defined"
	#endif
#endif


union ChannelData_t {
	uint16_t data;
	uint8_t  datab[2];
};


class TDA1311A {
	private:
		ChannelData_t _last_data[2]; /**< The data to send to the DAC */
#if !defined(TDA1311A_PIN_CLOCK) || !defined(TDA1311A_PIN_CHAN) || !defined(TDA1311A_PIN_DATA)
		const uint8_t TDA1311A_PIN_CLOCK;
		const uint8_t TDA1311A_PIN_CHAN;
		const uint8_t TDA1311A_PIN_DATA;
#endif

	public:
#if defined(TDA1311A_PIN_CLOCK) && defined(TDA1311A_PIN_CHAN) && defined(TDA1311A_PIN_DATA)
		TDA1311A();
#else
		TDA1311A(const uint8_t pin_clock, const uint8_t pin_chan, const uint8_t pin_data);
#endif
		~TDA1311A();
		
		/**
		 * \brief  Set the pin modes for the clock/channel/data pins as output
		 */
		void setup(); 
		
		/**
		 *	\brief  Set the low byte of channel output to data
		 *
		 *	\param[in] channel either TDA1311A_CHAN_LEFT or TDA1311A_CHAN_RIGHT
		 *  \param[in] data the data
		 */
		void setChannelOutputL(uint8_t channel, uint8_t data) {
			channel &= 0x01;
			_last_data[channel].data &= 0xFF00;
			_last_data[channel].data |= data;
		}
		
		
		/**
		 *	\brief  Set the high byte of channel output to data
		 *
		 *	\param[in] channel either TDA1311A_CHAN_LEFT or TDA1311A_CHAN_RIGHT
		 *  \param[in] data the data
		 */
		void setChannelOutputH(uint8_t channel, uint8_t data) {
			channel &= 0x01;
			_last_data[channel].data &= 0x00FF;
			_last_data[channel].data |= (data << 8);
		}
		
		
		/**
		 *	\brief  Set the data of channel output
		 *
		 *	\param[in] channel either TDA1311A_CHAN_LEFT or TDA1311A_CHAN_RIGHT
		 *  \param[in] data the data
		 */
		void setChannelOutput(uint8_t channel, uint16_t data) {
			channel &= 0x01;
			_last_data[channel].data = data;
		}
		
		
		/**
		 *	\brief  Set the data for both channels
		 *
		 *	\param[in] channel either TDA1311A_CHAN_LEFT or TDA1311A_CHAN_RIGHT
		 *  \param[in] data the data
		 */
		void setOutput(uint16_t left, uint16_t right) {
			_last_data[TDA1311A_CHAN_LEFT].data = left;
			_last_data[TDA1311A_CHAN_RIGHT].data = right;
		}

		/**
		 *	\brief  Send the stored data of both channels to the DAC
		 */
		void sendOutput();

		/**
		 *	\brief  Send the stored data for one channel to the DAC
		 *
		 *	\param[in] channel either TDA1311A_CHAN_LEFT or TDA1311A_CHAN_RIGHT
		 */
		void sendOutput(uint8_t channel);
};

#endif

