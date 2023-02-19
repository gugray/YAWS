#include <Arduino.h>
#include "radio/TinyRF_TX.h"
#include "onewire/OneWire.h"
#include "sleep.h"

// TODO: Verify at 1Mhz, or update VCC conversion code

/*
Transmitter: At 3.3V
TX data GND (via 10k): real 0
TX data float: 10-20 uA
TX data VCC: 9-12 mA (small coiled antenna)
TX data VCC: 10.3ma (17 cm antenna)

MCU: ATTony85V 10U
When it cannot start (voltage too low w/BOD 2.7V): 500uA
When it cannot start (voltage too low w/BOD 1.8V): 150uA
When it cannot start (voltage too low no BOD):      50uA
Sleep: 30uA at 5V, 25uA at 3.3V (BOD on)
Sleep: 9uA at 5V, 5uA at 3.3V (BOD off)

Active modes at 8MHz
Empty loop:     3.2mA     5.5mA
Measure VCC:    3.6mA     6.0mA
Measure temp:   3.6mA     6.0mA (+ca. 20uA)
Transmit:       7.4mA    13.1mA (+-1 mA, antenna)

Loop is 146 msec + 1sec sleep

Let's assume 0.3 sec loop at 15mA; resting current at 10uA
1 HR consumption:
Resting per hour: 10 * 1e-6 = 1e-5 Ah
12 x working per hour: 0.3 * 12 * 15 * 1e-3 / 3600 = 0.000015 Ah
Sum: 0.000025 Ah = 0.025 mAh
One day: 0.625 mAh
One week: 4.2 mAh
1 year: 218 mAh

*/

// Sleep this many times 8 sec between broadcasts
#define PERIOD_8SEC 40

#define RADIO_TX_PIN 2
#define RADIO_PWR_PIN 3
#define DS18B20_PIN 1

OneWire ds(DS18B20_PIN);

#pragma pack(push, 1)
struct TXData
{
  uint8_t tempHi = 0;
  uint8_t tempLo = 0;
  uint8_t vccHi = 0;
  uint8_t vccLo = 0;
};
#pragma pack(pop)

byte dsData[9];
TXData txData;

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void enableADC()
{
  ADCSRA = _BV(ADEN);
}

void measureVCC()
{
  // get_vcc_ext uses the internal reference 1.1V and measures voltage on a pin
  // Pin has a 1M resistor to battery voltage and 100k resistor to ground
  // Ie pin has 0.090909 times battery voltage, getting compared to 1.1V

  ADCSRA |= 0b110;  // Prescaler: 64 -> 15kHz @1MHz
  ADMUX = _BV(MUX1) | _BV(MUX0) | _BV(REFS1); // ADC3 on PB3, which is pin 3; against 1.1V internal reference

  // Conversion takes 25 cycles when first enabled (13 afterwards)
  // Even at low ADC clock of 15kHz, that can no way be longer than 2msec
  // Count every 0.01msec = 10 micros up to 255; then, abandon
  uint8_t waitCount = 0;
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC) && waitCount != 0xff)
  {
    delayMicroseconds(100);
    ++waitCount;
  }
  // Conversion didn't complete in time
  if (waitCount == 0xff)
  {
    uint16_t dummy_vcc = 999;
    txData.vccLo = (dummy_vcc & 0xff);
    txData.vccHi = (dummy_vcc >> 8);
    return;
  }

  // Read value
  uint8_t low = ADCL;
  uint32_t val = (ADCH << 8) | low;

  // we return voltage * 100
  uint32_t uvcc = val * 7559 / 16384;
  txData.vccLo = (uvcc & 0xff);
  txData.vccHi = (uvcc >> 8);


  // // Read 1.1V reference against AVcc
  // ADMUX = _BV(MUX3) | _BV(MUX2);
  // delay(2); // Wait for Vref to settle
  // ADCSRA |= _BV(ADSC); // Start conversion
  // while (bit_is_set(ADCSRA,ADSC)); // measuring
  // uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH 
  // uint8_t high = ADCH; // unlocks both
  
  // uint32_t val = (high<<8) | low;  
  // // val = (uint32_t)(1024) * 110 / val; // Calculate Vcc (times 100, ie 317 for 3.17V); 112640 = 1.1*1024*100
  // // Empirical correction BARGH ~ 0.96
  // val = (uint32_t)(985) * 110 / val;
  
  // // Write to TX data
  // uint16_t uvcc = val;
  // txData.vccLo = (uvcc & 0xff);
  // txData.vccHi = (uvcc >> 8);
}


void startTempConversion()
{
  ds.reset();
  ds.skip();
  ds.write(0x44); // start conversion
}

void readTemp()
{
  ds.reset();
  ds.skip();
  ds.write(0xBE); // Read Scratchpad
  for (uint8_t i = 0; i < 9; i++)
    dsData[i] = ds.read();

  // TODO:
  ds.depower();

  // Convert the data to actual temperature
  int16_t raw = (dsData[1] << 8) | dsData[0]; // default is 12 bit resolution
  uint8_t cfg = (dsData[4] & 0x60);
  // at lower res, the low bits are undefined, so let's zero them
  if (cfg == 0x00)
    raw = raw & ~7;  // 9 bit resolution
  else if (cfg == 0x20) 
    raw = raw & ~3; // 10 bit res
  else if (cfg == 0x40) 
    raw = raw & ~1; // 11 bit res

  int32_t temp = raw * 5 + 2400; // Celsius times 80 + 2400 -> this puts -30C at 0

  // Convert to unsigned (-30C is lowest we get)
  if (temp < 0) temp = 0;
  uint16_t utemp = temp;

  // Write to TX data
  txData.tempLo = (utemp & 0xff);
  txData.tempHi = (utemp >> 8);
}



void setup()
{
  setupTransmitter(RADIO_TX_PIN);
  pinMode(RADIO_TX_PIN, OUTPUT);
  pinMode(RADIO_PWR_PIN, OUTPUT);
  digitalWrite(RADIO_PWR_PIN, HIGH);
  digitalWrite(RADIO_TX_PIN, LOW);
}

void loop()
{
  // Turn on ADC; start temp conversion
  enableADC();
  startTempConversion();

  // Sleep 1 sec: ADC settles; temp gets converted
  sleep(6, false);

  // Get readings
  readTemp();
  measureVCC();

  // Transmit
  send((byte*)&txData, sizeof(TXData));
  digitalWrite(RADIO_TX_PIN, LOW);

  // Back to a long, deep sleep
  for (uint16_t n = 0; n < PERIOD_8SEC; ++n)
  {
    sleep(9, true);
  }
}
