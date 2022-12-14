#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "sleep.h"

uint8_t lastSleepLength = 255;

void setupWatchdog(uint8_t sleepLength)
{
  if (sleepLength == lastSleepLength) return;
  else lastSleepLength = sleepLength;

  uint8_t b = sleepLength & 7;
  if (sleepLength > 7)
    b |= (1 << 5);
  b |= (1 << WDCE);
  MCUSR &= ~(1 << WDRF);

  // On Attiny, this is WDTCR
  // On Atmega (Nano), it's WDTCSR
#ifdef ENV_ATTINY
  // start timed sequence
  WDTCR |= (1 << WDCE) | (1 << WDE);
  // set new watchdog timeout value
  WDTCR = b;
  WDTCR |= _BV(WDIE);
#endif
#ifdef ENV_NANO
  // start timed sequence
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  // set new watchdog timeout value
  WDTCSR = b;
  WDTCSR |= _BV(WDIE);
#endif
}

// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {}

void sleep(uint8_t sleepLength, bool disableADC)
{
  setupWatchdog(sleepLength);
  
  if (disableADC)
    ADCSRA = 0;

  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  // This *should* work, but it messes up the MCU
  // Investigate?
  // MCUCR = bit(BODS) | bit(BODSE);     // turn on brown-out enable select
  // MCUCR = bit(BODS);                  // this must be done within 4 clock cycles of above

  sleep_mode();
  sleep_disable(); // system continues execution here when watchdog timed out
}
