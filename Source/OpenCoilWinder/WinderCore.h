#ifndef WINDER_CORE_H
#define WINDER_CORE_H

#include <stddef.h>

namespace WinderCore {

const int SLIDER_MOTOR_CW = 1;
const int MOTOR_STOP = 0;
const int SLIDER_MOTOR_CCW = -1;

const int WINDER_MOTOR_STEPS = 200;
const int SLIDER_MOTOR_STEPS = 20;

const float SLIDER_DISTANCE_PER_TURN_MM = 0.5f;
const int STEPPER_CALL_OVERHEAD_MS = 4;

struct Settings {
  float coilHeightMm;
  int layers;
  int turnsPerLayer;
  int windingSpeedRpm;
};

struct ParameterFlags {
  bool velocity;
  bool layers;
  bool turnsPerLayer;
  bool coilHeight;
};

struct MotionPlan {
  int cappedWindingSpeedRpm;
  int winderStepsPerLayer;
  int sliderStepsPerLayer;
  int windingCount;
  float windingError;
  float checkValue;
  int delayMs;
};

Settings defaultSettings();
ParameterFlags clearParameterFlags();

bool appendFourDigitValue(int currentValue, unsigned char digitIndex, char digit, int *nextValue);
bool applyReceivedNumber(char command, int value, Settings *settings, ParameterFlags *flags);
bool hasAllRequiredParameters(const ParameterFlags &flags);
bool calculateMotionPlan(const Settings &settings, MotionPlan *plan);

}  // namespace WinderCore

#endif
