#include <Arduino.h>
#include "radio/TinyRF_TX.h"
#include "onewire/OneWire.h"
#include "sleep.h"

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
  // Read 1.1V reference against AVcc
  ADMUX = _BV(MUX3) | _BV(MUX2);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH 
  uint8_t high = ADCH; // unlocks both
  
  uint32_t val = (high<<8) | low;  
  val = (uint32_t)(1024) * 110 / val; // Calculate Vcc (times 100, ie 317 for 3.17V); 112640 = 1.1*1024*100
  // Empirical correction BARGH ~ 0.96
  // val = (uint32_t)(985) * 110 / val;
  
  // Write to TX data
  uint16_t uvcc = val;
  txData.vccLo = (uvcc & 0xff);
  txData.vccHi = (uvcc >> 8);
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
  pinMode(RADIO_PWR_PIN, OUTPUT);
  setupTransmitter(RADIO_TX_PIN);
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
  digitalWrite(RADIO_PWR_PIN, HIGH);
  send((byte*)&txData, sizeof(TXData));
  digitalWrite(RADIO_PWR_PIN, LOW);

  // Back to a long, deep sleep
  for (uint16_t n = 0; n < PERIOD_8SEC; ++n)
  {
    sleep(9, true);
  }
}
