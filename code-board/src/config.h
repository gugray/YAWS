#ifndef CONFIG_H
#define CONFIG_H

struct Config
{
  static int16_t altitude;

  static void load();

  static void save();
};

#endif
