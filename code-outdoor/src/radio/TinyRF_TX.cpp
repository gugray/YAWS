#include <Arduino.h>
#include "TinyRF_TX.h"
#include "TinyRF.h"

uint8_t txPin = 0xff;

void setupTransmitter(uint8_t pin)
{
	txPin = pin;
}

void transmitByte(byte _byte);

/**
 * Notes:
 * When the receiver is an Arduino:
 * Using digitalWrite() and delayMicroseconds() we will have frequent 8-12us timing errors
 * Using native AVR code (PORTx/_delay_us) will decrease the error rate to frequent 4us errors
 * Disabling interrupts during transmission + native AVR reduces errors to occasional 4us errors
 * With our 100+us timings even a 50us error is forgiveable so we'll just stick with digitalWrite()
 * Regarding Flash usage, it appears MicroCore's ATtiny13 optimizer already does the convertsion
 * and in my tests there was no difference between using digitalWrite() and native code
 **/
void send(byte *data, uint8_t len, bool incrementSeq)
{

// we calculate the crc here, because if we do it after the transmission has started
// it will create a delay during transmission which causes the receiver to lose accuracy
#ifndef TRF_ERROR_CHECKING_NONE
	byte errChck = TRF_ERR_CHK_FUNC(data, len);
#endif

	// preamble
	// based on our pulse period our zeroes have more HIGH time than our ones
	// so we send zero bytes as preamble
	for (uint8_t i = 0; i < NUM_PREAMBLE_BYTES; i++)
	{
		transmitByte(0x00);
	}

	// START pulse
	digitalWrite(txPin, LOW);
	delayMicroseconds(START_PULSE_PERIOD - PERIOD_HIGH_DURATION);
	digitalWrite(txPin, HIGH);
	delayMicroseconds(PERIOD_HIGH_DURATION);

	transmitByte(len);

// error checking byte
#ifndef TRF_ERROR_CHECKING_NONE
	transmitByte(errChck);
#endif

	// data
	// we send it like this because it uses less memory
	while (len--)
	{
		transmitByte(data[len]);
	}

	// reset the line to LOW so receiver detects last pulse
	// because receiver uses falling edges to detect pulses
	digitalWrite(txPin, LOW);

	// receiver relies on noise to detect end of transmission,
	// so we send it some artificial noise to "announce" end of transmission
	// be careful choosing this because when we're here receiver is expecting a byte not a start pulse
	// so it's more sensitive
	// we really need 8 to fill the rcvdPulses[] buffer, but we send 10 just to be sure
#if defined(TRF_EOT_IN_TX) && !defined(TRF_EOT_NONE)
	for (uint8_t i = 0; i < 10; i++)
	{
		delayMicroseconds(PERIOD_HIGH_DURATION / 2);
		digitalWrite(txPin, HIGH);
		delayMicroseconds(PERIOD_HIGH_DURATION / 2);
		digitalWrite(txPin, LOW);
	}
#else
	// delayMicroseconds(MIN_TX_INTERVAL_REAL*2);
#endif
}

// sends one byte
void transmitByte(byte _byte)
{
	// we do our loop like this because it uses less memory
	uint8_t i = 7;
	do
	{
		// send the LOW part of the pulse
		// the LOW duration determines wether a pulse is a 1 or a 0
		digitalWrite(txPin, LOW);
		// if 1
		if (_byte & (1 << i))
		{
			delayMicroseconds(ONE_PULSE_PERIOD - PERIOD_HIGH_DURATION);
		}
		else
		{
			delayMicroseconds(ZERO_PULSE_PERIOD - PERIOD_HIGH_DURATION);
		}
		// send the HIGH part of the pulse
		// all pulses have the same HIGH duration
		digitalWrite(txPin, HIGH);
		delayMicroseconds(PERIOD_HIGH_DURATION);

	} while (i--);
}