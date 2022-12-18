#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "config.h"

const size_t jsonDocSize = 256;
const char *fnConfigFile = "config.json";

float Config::ofsPres = 0;
float Config::ofsTemp = 0;

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
  auto jOfsPres = doc["ofsPres"];
  if (jOfsPres)
    Config::ofsPres = jOfsPres.as<float>();
  auto jOfsTemp = doc["ofsTemp"];
  if (jOfsTemp)
    Config::ofsTemp = jOfsTemp.as<float>();
}

void Config::save()
{
  // Create and fill document
  StaticJsonDocument<jsonDocSize> doc;
  doc["ofsPres"] = Config::ofsPres;
  doc["ofsTemp"] = Config::ofsTemp;

  // Serialize to file
  File f = LittleFS.open(fnConfigFile, "w");
  if (!f)
    return;
  serializeJson(doc, f);
  f.close();
}

