#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "config.h"

const size_t jsonDocSize = 256;
const char *fnConfigFile = "config.json";

int16_t Config::altitude = 42;

void Config::load()
{
  // Parse JSON from file, permissively
  File f = LittleFS.open(fnConfigFile, "r");
  if (!f)
    return;
  StaticJsonDocument<jsonDocSize> doc;
  auto err = deserializeJson(doc, f);
  f.close();
  if (err)
    return;

  // Extract values from JSON permissively
  auto jAltitude = doc["altitude"];
  if (jAltitude)
    Config::altitude = jAltitude.as<int16_t>();
}

void Config::save()
{
  // Create and fill document
  StaticJsonDocument<jsonDocSize> doc;
  doc["altitude"] = Config::altitude;

  // Serialize to file
  File f = LittleFS.open(fnConfigFile, "w");
  if (!f)
    return;
  serializeJson(doc, f);
  f.close();
}

