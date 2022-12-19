#include <Arduino.h>
#include "TinyRF_RX.h"
#include "TinyRF.h"

/*
notes:
- if we had errors when increasing data rates it's because of noise and we should enable MAX values for
pulse periods to increase accuracy
- if we had errors when increasing gap between messages it's because of preable
- sometimes preamble can act as EOT, this should not be relied on and should be prevented
- we send zero bytes as preamble so EOT "HAS TO" be detected before next transmission begins
otherwise these zeroes will be considered part of the previous message
- it's possible that START pulse can act as EOT but this should not be used
*/

namespace tinyrf
{

	// is true when START is received but EOT hasn't happened yet
	volatile bool transmitOngoing = false;
	// set to true every time interrupt runs, used to determine when data hasn't come in for a long time
	volatile bool interruptRun = false;
	// efficient alternative to attachInterrupt/detachInterrupt
	volatile bool interruptDisabled = false;
	// buffer for received pulses(bits)
	volatile unsigned long rcvdPulses[8];
	// buffer for received bytes
	volatile byte rcvdBytesBuf[TRF_RX_BUFFER_SIZE];
	// index of rcvdBytesBuf to write the next byte in
	volatile uint8_t bufWriteIndex = 0;
	// index of rcvdBytesBuf to read the next byte from
	uint8_t bufReadIndex = 0;
	// used to detect when bufWriteIndex is about to overwire bufReadIndex
	volatile uint8_t bufsDiff = 0;
	// whether buffer is being overwritten from the end, used when reading buffer
	volatile boolean bufOverwriteOngoing = false;
	// the frame length as received in the first byte of the message
	volatile uint8_t rcvdFrameLen = 0;
	// the frame length as the number of actual bytes received since the transmission has begun
	volatile uint8_t frameLen = 0;
	// number of messages currently in buffer
	volatile uint8_t numMsgsInBuffer = 0;
	// beggining of the current message in buffer, the value of this will be the length of the message
	volatile uint8_t msgAddrInBuf = 0;
	// pin used for recption, should support external interrupts. Must be set in setupReceiver().
	uint8_t rxPin = 0xff;

}

void interrupt_routine();

void lim_inc(volatile uint8_t &num)
{
	// reset if it has reached max value
	if (num == TRF_RX_BUFFER_SIZE - 1)
	{
		num = 0;
	}
	else
	{
		num++;
	}
}

void lim_sum(volatile uint8_t &num, uint16_t c)
{
	c = c % TRF_RX_BUFFER_SIZE;
	// reset if it has reached max value
	int sum = num + c;
	if (sum >= TRF_RX_BUFFER_SIZE)
	{
		num = sum - TRF_RX_BUFFER_SIZE;
	}
	else
	{
		num += c;
	}
}

void lim_dec(volatile uint8_t &num)
{
	// reset if it has reached min value
	if (num == 0)
	{
		num = TRF_RX_BUFFER_SIZE - 1;
	}
	else
	{
		num--;
	}
}

void lim_sub(volatile uint8_t &num, uint16_t c)
{
	c = c % TRF_RX_BUFFER_SIZE;
	int sub = num - c;
	// reset if it has reached min value
	if (sub < 0)
	{
		num = TRF_RX_BUFFER_SIZE - c + num;
	}
	else
	{
		num -= c;
	}
}

void setupReceiver(uint8_t pin)
{
	using namespace tinyrf;
	rxPin = pin;
	pinMode(rxPin, INPUT);
	attachInterrupt(digitalPinToInterrupt(rxPin), interrupt_routine, FALLING);
}

void detachReceiver()
{
	using namespace tinyrf;
	if (rxPin == 0xff)
		return;
	detachInterrupt(digitalPinToInterrupt(rxPin));
	rxPin = 0xff;
}

inline void incBufWriteIndex()
{
	using namespace tinyrf;
	lim_inc(bufWriteIndex);
	lim_inc(bufsDiff);
	// if incrementing bufsDiff causes it to reset to zero it means it has reached the bufReadIndex
	// so we move bufReadIndex one frame forward
	if (bufsDiff == 0)
	{
		bufOverwriteOngoing = true;
		uint8_t emptiedBytes = rcvdBytesBuf[bufReadIndex] + 1;
		numMsgsInBuffer--;
		lim_sum(bufReadIndex, emptiedBytes);
		lim_sub(bufsDiff, emptiedBytes);
		bufOverwriteOngoing = false;
	}
}

/**
 * This function is called when end of transmission is detected either through interrupt or
 * getReceivedData()
 **/
inline void EOT()
{
	using namespace tinyrf;
	transmitOngoing = false;
	// the transmission has ended
	// put the message length at the beggining of the message data in buffer
	// increment numMsgsInBuffer
	// increment bufWriteIndex
	if (frameLen > 0)
	{
		rcvdBytesBuf[msgAddrInBuf] = frameLen - 1; // minus the 'len' byte
		numMsgsInBuffer++;
		// if a message's length is 0, then this block will not run and bufWriteIndex will stay
		// the same and next msg will be written over it
		incBufWriteIndex();
	}
}

/**
 * This function is called from the interrupt routine when 8 bits of data has been received
 * It turns the bits into a byte and puts it in the buffer
 **/
inline void process_received_byte()
{

	using namespace tinyrf;
	byte rcvdByte = 0x00;

	for (uint8_t i = 0; i < 8; i++)
	{
		// if pulse is greater than START_PULSE_PERIOD then we will not be here
		if (
				rcvdPulses[i] > (ONE_PULSE_PERIOD - ONE_PULSE_TRIGG_ERROR) && rcvdPulses[i] < (ONE_PULSE_PERIOD + ONE_PULSE_TRIGG_ERROR))
		{
			rcvdByte |= (1 << (7 - i)); // tx sends bytes in reverse
		}
		else if (
				rcvdPulses[i] < (ZERO_PULSE_PERIOD - ZERO_PULSE_TRIGG_ERROR) || rcvdPulses[i] > (ZERO_PULSE_PERIOD + ZERO_PULSE_TRIGG_ERROR))
		{
			// this is noise => end of transmission
			// regardless of whether we have received 'rcvdFrameLen' byte of data we EOT here
			EOT();
			return;
		}
	}

	// TRF_PRINT((char)rcvdByte);

	// we have received one bytes of data
	// if this is the first byte of the frame then it's the message length
	// this is only for detecting EOT and isn't stored in buffer
	if (frameLen == 0)
	{
#if defined(TRF_ERROR_CHECKING_NONE)
		rcvdFrameLen = rcvdByte + 1;
#else
		rcvdFrameLen = rcvdByte + 2;
#endif
		// TRF_PRINTLN(rcvdFrameLen);
	}
	// add it to the buffer
	// increment bufWriteIndex
	// increment frameLen
	else
	{
		incBufWriteIndex();
		rcvdBytesBuf[bufWriteIndex] = rcvdByte;
	}

	frameLen++;

	if (frameLen == rcvdFrameLen)
	{
		// TRF_PRINT("rcvd ");TRF_PRINT(rcvdFrameLen);TRF_PRINTLN(" bytes");
		EOT();
	}
}

/**
 * Interrupt routine called on falling edges of pulses
 * We use a pulse period encoding to determine what a pulse means
 * As suggested here: http://www.romanblack.com/RF/cheapRFmodules.htm
 * This way we will have both HIGH and LOW periods in both 1 and 0 bits, eliminating
 * the need for manchester encoding and other workarounds for sending equal HIGH and LOW
 * Pulse periods are defined in the file TinyRF.h
 * This interrupt routine usually take 8us - sometimes goes up to 30us
 * With our 100+us pulse periods this shouldn't be a problem
 **/
#ifdef TRF_MCU_ESP
IRAM_ATTR void interrupt_routine()
{
#else
void interrupt_routine()
{
#endif

	using namespace tinyrf;

	interruptRun = true;

	static uint8_t pulse_count = 0;
	static unsigned long lastTime = 0;

	unsigned long time = micros();
	unsigned long pulsePeriod = time - lastTime;
	lastTime = time;

	if (interruptDisabled)
	{
		return;
	}

	// TRF_PRINTLN(pulsePeriod);

	// start of transmission
	if (
			pulsePeriod > (START_PULSE_PERIOD - START_PULSE_TRIGG_ERROR) && pulsePeriod < (START_PULSE_PERIOD + START_PULSE_MAX_ERROR))
	{
		// if we receive a start while we are already processing an ongoing transmission
		// it means the previous transmission has ended
		if (transmitOngoing)
		{
			EOT();
		}
		transmitOngoing = true;
		pulse_count = 0;
		msgAddrInBuf = bufWriteIndex;
		frameLen = 0;
	}
	else if (transmitOngoing)
	{
		rcvdPulses[pulse_count] = pulsePeriod;
		pulse_count++;
		if (pulse_count == 8)
		{
			process_received_byte();
			pulse_count = 0;
		}
	}

	// TRF_PRINTLN(micros() - time);
}

// degug stuff
// #define showbuffer
// #define showseq
// #define showduplicates

uint8_t getReceivedData(byte buf[], uint8_t bufSize, uint8_t &numRcvdBytes)
{

	using namespace tinyrf;

	numRcvdBytes = 0;

	// we rely on noise to detect end of transmission
	// in the rare event that there was no noise(the interrupt did not trigger) for a long time
	// consider the transmission over and add received data to buffer
#if !defined(TRF_EOT_IN_TX) && !defined(TRF_EOT_NONE)

	// todo: there is no guarantee this will be called frequently enough so put it in a timer

	// we can't use lastTime which is set in the interrupt because it is a long (non-atomic)
	// and using it here will wreak havoc, so we calculate another one here
	// it does not matter that it takes a bit longer because this function is called from loop
	// not from interrupt
	static unsigned long lastInterruptRun = 0;
	unsigned long time = micros();

	if (interruptRun)
	{
		lastInterruptRun = time;
		interruptRun = false;
	}
	else if (transmitOngoing)
	{
		// actually instead of being > START_PULSE_PERIOD, it should be > ONE_PULSE_PERIOD
		// because after transmission has started we only expect 1 and 0, and if we haven't
		// received a byte for more than ONE_PULSE_PERIOD it means transmission is over
		// but for some reason using that value causes lost messages and I cannot for the life of
		// me find out why
		if ((time - lastInterruptRun) > (MIN_TX_INTERVAL_REAL))
		{
			// we don't want the interrupt to run while we're modifying these
			// it's unlikely that this will hurt the interrupt because if it hasn't run for a while
			// it means transmission has stopped. also this is quite short
			interruptDisabled = true;
			EOT();
			interruptDisabled = false;
		}
	}

#endif

	if (numMsgsInBuffer == 0)
	{
		return TRF_ERR_NO_DATA;
	}

#ifdef showbuffer
	TRF_PRINT("len addr: ");
	TRF_PRINT2(bufReadIndex, DEC);
	TRF_PRINT(" - #msgs in buf: ");
	TRF_PRINT(numMsgsInBuffer);
	TRF_PRINTLN("");
#endif

	/* manage buffer */
	// if buffer is being overwritten wait until it's done so we get the currect bufReadIndex
	// the time it takes the next few instructions to run is way less than the time it takes for a byte
	// to arrive, so we're pretty sure buffer overwrite will not happen during the next few instructions
	while (bufOverwriteOngoing)
	{
	};

	// this is how our buffer looks like:
	//[frm0 len|frm0 crc|frm0 seq#|frm0 byte0|frm0 byte1|...|frm1 len|frm1 crc|frm1 seq#|frm1 byte0|frm1 byte1|...]
	// frame length = data length + seq# + error checking byte

	// bufReadIndex points to the first byte of frame, i.e. the length
	uint8_t frameLen = rcvdBytesBuf[bufReadIndex];
	lim_inc(bufReadIndex);
	lim_dec(bufsDiff);

	uint8_t frameReadIndex = bufReadIndex;
	// move bufReadIndex 'length' bytes forward to point to the next frame
	lim_sum(bufReadIndex, frameLen);
	lim_sub(bufsDiff, frameLen);

	// we consider this message processed as of now
	numMsgsInBuffer--;

	// a buffer overwrite could happen from this point forward, writing to the buffer as we are
	// reading it. but it will only corrupt this one frame, unless we are sending very fast and
	// reading very slowly, in which case the buffer keeps being overwritten without us being able
	// to catch up with it

	// a frame's minimum length is 3 bytes: CRC + SEQ + 1 Byte Data (at least)
	if (frameLen == 0)
	{
		return TRF_ERR_NO_DATA;
	}
	else if (frameLen < 3)
	{
		return TRF_ERR_NOISE;
	}

	uint8_t dataLen = frameLen;

#ifndef TRF_ERROR_CHECKING_NONE
	dataLen--;
	byte errChckRcvd = rcvdBytesBuf[frameReadIndex];
	lim_inc(frameReadIndex);
#endif

	numRcvdBytes = dataLen;

	if (dataLen > bufSize)
	{
		TRF_PRINT("buffer overflow: ");
		TRF_PRINTLN(dataLen);
		return TRF_ERR_BUFFER_OVERFLOW;
	}

#ifdef showbuffer
	TRF_PRINT(" - read index: ");
	TRF_PRINT2(frameReadIndex, DEC);
	TRF_PRINT(" - len: ");
	TRF_PRINT(frameLen);
	TRF_PRINTLN("");
	for (int i = 0; i < 256; i++)
	{
		TRF_PRINT(i);
		TRF_PRINT("[");
		TRF_PRINT(rcvdBytesBuf[i]);
		TRF_PRINT("],");
	}
	TRF_PRINTLN("");
#endif

	// copy the data from 'frameReadIndex' until frameReadIndex+dataLen
	for (uint8_t i = 0; i < dataLen; i++)
	{
		buf[dataLen - 1 - i] = rcvdBytesBuf[frameReadIndex]; // tx sends data in reverse
		lim_inc(frameReadIndex);
	}

/*** error checking ***/
#ifndef TRF_ERROR_CHECKING_NONE
	byte errChckCalc = TRF_ERR_CHK_FUNC(buf, dataLen);
	if (errChckRcvd != errChckCalc)
	{
		return TRF_ERR_CORRUPTED;
	}
	// if all data is zeroes CRC will also be zero and CRC check will pass
	// we can eliminate this by using a non-zero crc init but that would increase crc function
	// size, so we do this here in the RX instead
	else if ((errChckRcvd | errChckCalc) == 0)
	{
		boolean allZeroes = true;
		for (uint8_t i = 0; i < dataLen; i++)
		{
			if (buf[i] != 0x00)
			{
				allZeroes = false;
			}
		}
		if (allZeroes)
		{
			return TRF_ERR_NOISE;
		}
	}
#endif

	return TRF_ERR_SUCCESS;
}
