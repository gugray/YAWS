#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <DOG7565R.h>
#include <WiFiClientSecureBearSSL.h>
#include "instrument.h"
#include "predictor.h"
#include "canvas.h"

#define LED_PIN LED_BUILTIN
#define RADIO_RX_PIN D6
#define PHOTO_RESISTOR_PIN A0
#define BACKLIGHT_PIN TX
#define DOG_CS_PIN D8
#define DOG_A0_PIN D3
#define DOG_RESET_PIN D0
#define BUTTON_PIN RX

#define BME_ADDR 0x76

#define BUTTON_MSEC_SERVER 200 // Start/stop web server if button's been pressed this long

#define EXDATA_MAX_WAIT_SEC 2000 // This is a bit over 6 x 320 sec (the outdoor unit's broadcast period)

// extern RH_ASK radio;
extern DOG7565R dog;

extern Instrument instrument;
extern Canvas canvas;
extern Predictor predictor;
extern int16_t buttonPressed; // Button has been pressed for this many msec. -1 if already handled.
extern bool bmeOk;
extern const size_t bufSize;
extern char *buf;

extern float currTemp;
extern float currHumi;
extern float currPres;
extern float currExTemp;
extern float currExBattery;
extern uint16_t secSinceExData; // Seconds since we last received data from outdoor unit

void flushCanvasToDisplay();

#endif

