#include <Arduino.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Ticker.h>
#include "duty100.h"
#include "globals.h"

// https://wolles-elektronikkiste.de/433-mhz-funk-mit-dem-arduino

const char *fnFirmware = "firmware.bin";
const char *wifiNetwork = "Cirrus";
const char *wifiPassword = "brouhaha";
const uint32_t wifiConnectTimeout = 10000;
const char *fileUrl = "https://zydeo.net/firmware.bin";

// Definitions of shared objects declared in globals.h
RH_ASK radio(2000, RADIO_RX_PIN, RADIO_TX_PIN);
DOG7565R dog;
BearSSL::WiFiClientSecure secureBearClient;

Settings stgs;
Instrument instrument;
Canvas canvas;
Predictor predictor;

float currTemp;
float currHumi;
float currPres;

// Used in main only
Ticker ticker;
const size_t bufSize = 1024;
char buf[bufSize];

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

size_t downloadFile(const char *url, const char *fn)
{
  if (LittleFS.exists(fn))
    LittleFS.remove(fn);
  File f = LittleFS.open(fn, "w");
  if (!f)
  {
    Serial.printf("Failed to create file\n");
    return 0;
  }

  HTTPClient http;
  if (!http.begin(secureBearClient, url))
  {
    Serial.printf("http.begin() failed\n");
    f.close();
    return 0;
  }

  int16_t httpCode = http.GET();
  Serial.printf("http.GET() returned: %d\n", httpCode);
  if (httpCode != HTTP_CODE_OK && httpCode != HTTP_CODE_MOVED_PERMANENTLY)
  {
    f.close();
    http.end();
    return 0;
  }

  // Get length of document (is -1 when Server sends no Content-Length header)
  int32_t len = http.getSize();
  WiFiClient *stream = http.getStreamPtr();
  // Read all data from server
  while (http.connected() && (len > 0 || len == -1))
  {
    yield();
    int32_t size = stream->available();
    if (size <= 0)
      continue;
    if (size > (int32_t)bufSize)
      size = bufSize;
    size_t nRead = stream->readBytes(buf, size);
    if (nRead == 0)
      break;
    if (len > 0)
      len -= nRead;
    f.write(buf, nRead);
  }

  size_t sz = f.size();
  Serial.printf("Downloaded %d bytes\n", sz);

  f.close();
  http.end();
  return sz;
}

void doWifiStuff()
{
  Serial.begin(9600);
  Serial.printf("Hello, world.\n");

  LittleFS.begin();

  WiFi.begin(wifiNetwork, wifiPassword);
  Serial.printf("Connecting to WiFi...\n");
  int8_t wifiStatus = WiFi.waitForConnectResult(wifiConnectTimeout);
  if (wifiStatus == WL_CONNECTED)
  {
    Serial.printf("Connected to WiFi\n");
    String ipStr = WiFi.localIP().toString();
    Serial.printf("Local IP: %s\n", ipStr.c_str());
    secureBearClient.setInsecure();
    size_t updateSize = downloadFile(fileUrl, fnFirmware);
    if (updateSize > 0)
    {
      Serial.printf("Updating from file\n");
      File f = LittleFS.open(fnFirmware, "r");
      Update.begin(updateSize);
      Update.writeStream(f);
      if (!Update.end())
      {
        Serial.printf("Error: %d\n", Update.getError());
      }
      else
      {
        Serial.printf("Update finished; restarting. See you on the other side!\n");
        ESP.restart();
      }
    }
  }
  else
    Serial.printf("Not connected to WiFi; status: %d\n", wifiStatus);

  // File f = LittleFS.open("test.txt", "r");
  // while (f.available())
  // {
  //   f.readBytesUntil('\n', buf, bufSize);
  //   Serial.printf(buf);
  // }
  // f.close();
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  delay(500);

  Serial.begin(115200);
  Serial.printf("The UNIT sez Hi.\n");

  pinMode(BRIGHTNESS_PIN, INPUT);

  // Set up PWM for brightness
  // analogWriteRange(1023);
  // analogWriteFreq(880);
  // pinMode(TX, OUTPUT);
  // analogWrite(TX, 512);

  dog.initialize(DOG_CS_PIN, DOG_A0_PIN, DOG_RESET_PIN, DOGM128);
  canvas.clear();
  canvas.line(5, 5, 122, 5);
  canvas.fwText(20, 7, "Hello, world!");
  flushCanvasToDisplay();

  // This must come AFTER dog.initialize()
  // DOG uses SPI, which sets MISO pin as input
  // But we're hijacking it here for ourselves; no MISO needed by the dog
  radio.init();
  radio.setModeRx();

  bool bmeOk = instrument.begin(BME_ADDR);
  if (!bmeOk)
  {
    canvas.fwText(20, 0, "NO BME!!");
    flushCanvasToDisplay();
  }

  ticker.attach_ms(100, duty100);

  digitalWrite(LED_PIN, HIGH);
  delay(500);
}

int16_t count = 0;
const uint8_t radioBufSize = 128;
uint8_t radioBuf[radioBufSize];

void loop()
{
  // Control brightness
  // float val = 1023.0F * (sin(cnt) + 1) / 2;
  // analogWrite(TX, (int16_t)val);
  // cnt += 0.01;
  // if (cnt > 2 * PI)
  //   cnt -= 2 * PI;
  // delay(10);

  // Receive radio
  // uint8_t len = radioBufSize;
  // if (radio.recv(radioBuf, &len))
  // {
  //   Serial.printf("Messge %d received: '", count);
  //   for (uint8_t i = 0; i < len; ++i)
  //     Serial.print(radioBuf[i]);
  //   Serial.printf("'\n");
  //   ++count;
  // }

  // digitalWrite(LED_PIN, HIGH);
  // delay(500);
  // digitalWrite(LED_PIN, LOW);
  // delay(500);

  canvas.clear();

  sprintf(buf, "%5.1f*", currTemp);
  canvas.text(0, 30, Canvas::font30, buf);

  int16_t brightness = analogRead(BRIGHTNESS_PIN);
  sprintf(buf, "Light: %4d", brightness);
  canvas.fwText(8, 4, buf);

  sprintf(buf, "%5.1fC %d%% %4.0f hPa", currTemp, (uint16_t)round(currHumi), currPres);
  canvas.fwText(2, 5, buf);

  auto trend = predictor.getTrend();
  const char *trendStr = "T?";
  if (trend == Predictor::trendRapidRise)
    trendStr = "Rise^Fast";
  else if (trend == Predictor::trendRise)
    trendStr = "Rise";
  else if (trend == Predictor::trendFlat)
    trendStr = "Stable";
  else if (trend == Predictor::trendSink)
    trendStr = "Sink";
  else if (trend == Predictor::trendRapidSink)
    trendStr = "Sink^Fast";
  canvas.fwText(8, 6, trendStr);

  auto state = predictor.getState();
  const char *stateStr = "S?";
  if (state == Predictor::stateUnstableUp)
    stateStr = "Unstable";
  else if (state == Predictor::stateSun)
    stateStr = "Sun";
  else if (state == Predictor::stateMixed)
    stateStr = "Sun/Cloud";
  else if (state == Predictor::stateCloud)
    stateStr = "Cloud";
  else if (state == Predictor::stateUnstableDown)
    stateStr = "Storm";
  canvas.fwText(72, 6, stateStr);

  flushCanvasToDisplay();

  delay(200);
}
