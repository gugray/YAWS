#include <Arduino.h>
#include "server_loop.h"
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include "globals.h"

const char *apSSID = "YAWS Network";
const char *apPass = "ya12ws34";

const char *fnFirmware = "firmware.bin";
const char *wifiNetwork = "Cirrus";
const char *wifiPassword = "";
const uint32_t wifiConnectTimeout = 10000;
const char *fileUrl = "https://zydeo.net/firmware.bin";

BearSSL::WiFiClientSecure secureBearClient;
ESP8266WebServer server;
bool serverConfigured = false;
bool serverRunning = false;

enum LoopMode
{
  lmNotStarted, // DBG
  lmQuit,
  lmServe,
};

LoopMode loopMode = lmNotStarted;

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

void handleReadings()
{
  sprintf(buf, "{\"temp\": %5.1f, \"pres\": %4.0f, \"humi\": %d}", currTemp, round(currPres), (uint16_t)round(currHumi));
  server.send(200, "application/json", buf);
}

void configureServer()
{
  server.serveStatic("/", LittleFS, "/index.html");
  server.on("/readings", HTTP_GET, handleReadings);
  serverConfigured = true;
}


void beginServer()
{
  // Start access point
  canvas.clear();
  canvas.fwText(20, 2, "Starting WiFi...");
  flushCanvasToDisplay();
  if (!WiFi.softAP(apSSID, apPass))
  {
    canvas.fwText(20, 3, "Failed :(");
    canvas.fwText(20, 4, "Returning to weather");
    flushCanvasToDisplay();
    loopMode = lmQuit;
    delay(2000);
    return;
  }
  
  // Start web server
  canvas.fwText(20, 2, "Starting server...");
  if (!serverConfigured)
    configureServer();
  delay(1000);
  server.begin(80);
  serverRunning = true;

  // Show info for user to connect
  canvas.clear();
  sprintf(buf, "WiFi: %s", apSSID);
  canvas.fwText(0, 0, buf);
  sprintf(buf, "Password: %s", apPass);
  canvas.fwText(0, 1, buf);
  auto ipStr = WiFi.softAPIP().toString();
  sprintf(buf, "http://%s", ipStr.c_str());
  canvas.fwText(0, 2, buf);
  canvas.fwText(0, 6, "Connect from PC");
  canvas.fwText(0, 7, "Press button to quit");
  flushCanvasToDisplay();
  loopMode = lmServe;
}

bool serverLoop()
{
  if (loopMode == lmNotStarted)
  {
    beginServer();
    return false;
  }
  else if (loopMode == lmQuit)
  {
    if (serverRunning)
    {
      server.close();
      server.stop();
      serverRunning = false;
    }
    return false;
  }

  // Serving
  auto connCount = WiFi.softAPgetStationNum();
  sprintf(buf, "Connections: %d  ", connCount);
  canvas.fwText(0, 3, buf);
  flushCanvasToDisplay();
  
  // Serve requests
  server.handleClient();

  // Keep doing it
  return false;
}

void stopServer()
{

}
