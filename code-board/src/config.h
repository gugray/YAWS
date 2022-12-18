#ifndef CONFIG_H
#define CONFIG_H

struct Config
{
  static float ofsPres;
  static float ofsTemp;

  static void load();

  static void save();
};

#endif
