#ifndef PREDICTOR_H
#define PREDICTOR_H

#define MAX_PREDICTOR_READINGS 19 // 3 hours: 6 per hour at 10-minute intervals, plus current

class Predictor
{
public:
  enum State
  {
    stateUnknown,
    stateSun,
    stateMixed,
    stateCloud,
    stateUnstableUp,
    stateUnstableDown,
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
  struct Reading
  {
    float temp;
    float humi;
    float pres;
  };

private:
  uint16_t seconds = 0;
  uint16_t nReadings = 0;
  Reading readings[MAX_PREDICTOR_READINGS];
  Trend trend = trendUnknown;
  State state = stateUnknown;

private:
  void predict();
  static State improveState(State state);
  static State deteriorateState(State state);

public:
  void update(float temp, float humi, float pres);
  Trend getTrend() { return trend; }
  State getState() { return state; }
};

#endif
