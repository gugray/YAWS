#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <DOG7565R.h>
#include <WiFiClientSecureBearSSL.h>
#include <RH_ASK.h>
#include "instrument.h"
#include "predictor.h"
#include "canvas.h"

#define LED_PIN LED_BUILTIN
#define RADIO_RX_PIN D6
#define RADIO_TX_PIN D4 // Not used; we're only receiving
#define PHOTO_RESISTOR_PIN A0
#define BACKLIGHT_PIN TX
#define DOG_CS_PIN D8
#define DOG_A0_PIN D3
#define DOG_RESET_PIN D0
#define BUTTON_PIN RX

#define BME_ADDR 0x76

#define BUTTON_MSEC_START_SERVER 200 // Start web server if button's been pressed this long
#define BUTTON_MSEC_STOP_SERVER 1000 // Quit running web server if button's been pressed this long

extern RH_ASK radio;
extern DOG7565R dog;

extern Instrument instrument;
extern Canvas canvas;
extern Predictor predictor;
extern uint16_t buttonPressed; // Button has been pressed for this many msec
extern bool bmeOk;
extern const size_t bufSize;
extern char *buf;

extern float currTemp;
extern float currHumi;
extern float currPres;
extern int16_t currBrightness;
extern float currExTemp;
extern float currExBattery;

void flushCanvasToDisplay();

#endif

