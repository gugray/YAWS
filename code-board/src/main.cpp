#include <Arduino.h>
#include <DOG7565R.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "canvas.h"

const char *fnFirmware = "firmware.bin";
const char *wifiNetwork = "Cirrus";
const char *wifiPassword = "brouhaha";
const uint32_t wifiConnectTimeout = 10000;
const char *fileUrl = "https://zydeo.net/firmware.bin";

#define LED_PIN LED_BUILTIN

#define DOG_CS_PIN D8
#define DOG_A0_PIN D3
#define DOG_RESET_PIN D0

DOG7565R dog;
Canvas canvas;
BearSSL::WiFiClientSecure secureBearClient;

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

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(500);

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

  // analogWriteRange(1023);
  // analogWriteFreq(880);
  dog.initialize(DOG_CS_PIN, DOG_A0_PIN, DOG_RESET_PIN, DOGM128);
  canvas.clear();
  canvas.line(5, 5, 122, 5);
  canvas.fwText(20, 7, "Hello world!");
  flushCanvasToDisplay();

  digitalWrite(LED_PIN, LOW);
  delay(500);
}

void loop()
{
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
