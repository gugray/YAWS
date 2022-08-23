#include <Arduino.h>
#include <Ticker.h>
#include "duty100.h"
#include "weather_loop.h"
#include "server_loop.h"
#include "globals.h"

// https://wolles-elektronikkiste.de/433-mhz-funk-mit-dem-arduino

// Definitions of shared objects declared in globals.h
RH_ASK radio(2000, RADIO_RX_PIN, RADIO_TX_PIN);
DOG7565R dog;

Settings stgs;
Instrument instrument;
Canvas canvas;
Predictor predictor;
uint16_t buttonPressed = 0;
bool bmeOk = false;
const size_t bufSize = 1024;
char *buf = new char[bufSize];

float currTemp;
float currHumi;
float currPres;
int16_t currBrightness;
float currExTemp;
float currExBattery;

// Used in main only
enum Loops
{
  eWeatherLoop,
  eWebServerLoop,
};

Ticker ticker;
Loops currLoop = eWeatherLoop;

void flushCanvasToDisplay()
{

  if (!canvas.dirty)
    return;
  canvas.dirty = false;
  uint16_t i = 0;
  for (uint8_t p = 0; p < N_ROWS / 8; ++p)
  {
    dog.position(0, p);
    for (uint8_t x = 0; x < N_COLS; ++x, ++i)
    {
      dog.data(canvas.pixels[i]);
    }
  }
}


void setup()
{
  // Light up on-board LED for boot sequence
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  delay(500);

  pinMode(PHOTO_RESISTOR_PIN, INPUT);

  // Set up PWM for brightness
  analogWriteRange(1023);
  analogWriteFreq(880);
  pinMode(BACKLIGHT_PIN, OUTPUT);
  analogWrite(BACKLIGHT_PIN, 0);

  dog.initialize(DOG_CS_PIN, DOG_A0_PIN, DOG_RESET_PIN, DOGM128);

  // This must come AFTER dog.initialize()
  // DOG uses SPI, which sets MISO pin as input
  // But we're hijacking it here for ourselves; no MISO needed by the dog
  radio.init();
  radio.setModeRx();

  // Initialize BME280 weather sensor
  bmeOk = instrument.begin(BME_ADDR);

  // Create ticker for 100-msec duty cycle that checks readings
  ticker.attach_ms(100, duty100);

  // We're done, turn off LED
  digitalWrite(LED_PIN, HIGH);
  delay(500);
}

void loop()
{
  if (currLoop == eWeatherLoop)
  {
    // If button is pressed, start web server
    if (buttonPressed >= BUTTON_MSEC_START_SERVER)
    {
      beginServer();
      currLoop = eWebServerLoop;
      return;
    }
    // Do our normal weather station frame
    auto msec = weatherLoop();
    delay(msec);
  }
  else if (currLoop == eWebServerLoop)
  {
    // Upon long press, quit web server
    if (buttonPressed >= BUTTON_MSEC_START_SERVER)
    {
      stopServer();
      currLoop = eWeatherLoop;
      return;
    }
    // Do a web server frame
    bool quitServer = serverLoop();
    if (quitServer)
      currLoop = eWeatherLoop;
  }
}
