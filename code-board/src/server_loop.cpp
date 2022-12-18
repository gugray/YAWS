#include <Arduino.h>
#include "server_loop.h"
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "globals.h"
#include "config.h"
#include "server_index.h"

// WiFi access point details
const char *apSSID = "YAWS Network";
const char *apPass = "ya12ws34";

// For serving JSON responses
const size_t jsonDocSize = 256;
const size_t respBufSize = 256;
const char *respBuf = new char[respBufSize];

// Firmware upload & update
const char *fnFirmware = "firmware.bin";
File uploadFile;

// Web server
ESP8266WebServer server;
bool serverConfigured = false;
bool serverRunning = false;

enum LoopMode
{
  lmQuit,
  lmServe,
  lmUpdating,
};

LoopMode loopMode = lmQuit;

void runUpdate()
{
  // Shut down server
  server.close();
  server.stop();
  serverRunning = false;

  // Show "Updating" message
  canvas.clear();
  canvas.fwText(20, 2, "Updating...");
  flushCanvasToDisplay();

  // Do the update!
  File f = LittleFS.open(fnFirmware, "r");
  Update.begin(f.size());
  Update.writeStream(f);
  if (Update.end())
  {
    ESP.restart();
  }
  else
  {
    canvas.clear();
    canvas.fwText(20, 2, "Update failed :(");
    canvas.fwText(20, 3, "Restarting...");
    flushCanvasToDisplay();
    delay(2000);
    ESP.restart();
  }
}

void handleReadings()
{
  StaticJsonDocument<jsonDocSize> doc;
  doc["temp"] = currTemp;
  doc["pres"] = currPres;
  doc["humi"] = round(currHumi);
  serializeJson(doc, buf, bufSize);
  server.send(200, "application/json", buf);
}

void handleRestart()
{
  server.send(200, "application/json", "Restarting");
  ESP.restart();
}

void handleGetConfig()
{
  StaticJsonDocument<jsonDocSize> doc;
  doc["ofsPres"] = Config::ofsPres;
  doc["ofsTemp"] = Config::ofsTemp;
  serializeJson(doc, buf, bufSize);
  server.send(200, "application/json", buf);
}

void handlePostConfig()
{
  if (!server.hasArg("plain"))
  {
    server.send(400, "text/plain", "Body not received");
    return;
  }
  // Info comes as JSON in body
  const String &body = server.arg("plain");
  StaticJsonDocument<jsonDocSize> doc;
  auto err = deserializeJson(doc, body);
  if (err)
  {
    server.send(400, "text/plain", "Failed to parse JSON in body");
    return;
  }
  
  // Extract values from JSON permissively
  auto jOfsPres = doc["ofsPres"];
  if (jOfsPres)
    Config::ofsPres = jOfsPres.as<float>();
  auto jOfsTemp = doc["ofsTemp"];
  if (jOfsTemp)
    Config::ofsTemp = jOfsTemp.as<float>();
  
  // Save new values
  Config::save();

  // OK response
  server.send(200, "text/plain", "Config updated");
}

void handleUpload()
{
  server.send(200);
}

void processUpload()
{
  HTTPUpload &upload = server.upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    if (LittleFS.exists(fnFirmware))
      LittleFS.remove(fnFirmware);
    uploadFile = LittleFS.open(fnFirmware, "w");
    if (!uploadFile)
    {
      server.send(500, "text/plain", "Failed to create file in YAWS");
      return;
    }
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    if (!uploadFile)
      return;
    // Write the received bytes to the file
    uploadFile.write(upload.buf, upload.currentSize);
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (!uploadFile)
      return;
    // Close the file
    uploadFile.close();
    sprintf(buf, "File successfully uploaded. Size: %d", upload.totalSize);
    server.send(200, "text/plain", buf);
    loopMode = lmUpdating;
    runUpdate();
  }
}

void handleIndex()
{
    server.send(200, "text/html", FPSTR(text));
}

void configureServer()
{
  // server.serveStatic("/", LittleFS, "/index.html");
  server.on("/", HTTP_GET, handleIndex);
  server.on("/readings", HTTP_GET, handleReadings);
  server.on("/config", HTTP_GET, handleGetConfig);
  server.on("/config", HTTP_POST, handlePostConfig);
  server.on("/upload", HTTP_POST, handleUpload, processUpload);
  server.on("/restart", HTTP_POST, handleRestart);
  serverConfigured = true;
}

bool beginServer()
{
  loopMode = lmQuit;

  // Start access point
  canvas.clear();
  canvas.fwText(20, 2, "Starting WiFi...");
  flushCanvasToDisplay();

  bool wifiOk = WiFi.forceSleepWake();
  wifiOk &= WiFi.softAP(apSSID, apPass);
  // WiFi.mode(WIFI_AP);
  if (!wifiOk)
  {
    canvas.fwText(20, 3, "Failed :(");
    canvas.fwText(20, 4, "Returning to weather");
    flushCanvasToDisplay();
    loopMode = lmQuit;
    delay(2000);
    return false;
  }

  // Start web server
  canvas.fwText(20, 2, "Starting server...");
  if (!serverConfigured)
    configureServer();
  server.begin(80);
  serverRunning = true;
  delay(2000);

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

  return true;
}

bool serverLoop()
{
  if (loopMode == lmQuit)
  {
    stopServer();
    return true;
  }
  else if (loopMode == lmUpdating)
  {
    // NOP. runUpdate() is doing its job.
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
  canvas.clear();
  canvas.fwText(10, 2, "Stopping server...");
  canvas.fwText(10, 3, "YAWS will restart");
  flushCanvasToDisplay();
  delay(2000);

  if (serverRunning)
  {
    server.close();
    server.stop();
    serverRunning = false;
  }
  WiFi.disconnect();
  WiFi.forceSleepBegin();

  ESP.restart();
}
