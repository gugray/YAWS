#ifndef PREDICTOR_H
#define PREDICTOR_H

#define MAX_PREDICTOR_READINGS 37 // 3 hours: 12 per hour at 5-minute intervals, plus current
#define PREDICTOR_PERIOD_SECONDS 300

class Predictor
{
public:
  enum State
  {
    stateUnknown,
    stateSun,
    stateMixed,
    stateCloud,
    stateUnstableRise,
    stateStorm,
  };

  enum Trend
  {
    trendUnknown,
    trendRapidRise,
    trendRise,
    trendFlat,
    trendSink,
    trendRapidSink,
  };

private:
  uint16_t seconds = 0;
  uint16_t nReadings = 0;
  float readings[MAX_PREDICTOR_READINGS];
  Trend trend = trendUnknown;
  State state = stateUnknown;

private:
  void predict();
  void getSlopes(float &fullSlope, float &hourSlope);

public:
  // Must be called once per second.
  void update(float pres);

  Trend getTrend() { return trend; }
  State getState() { return state; }
};

#endif
