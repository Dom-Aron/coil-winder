#include "WinderCore.h"

namespace WinderCore {

Settings defaultSettings() {
  Settings settings;
  settings.coilHeightMm = 0.0f;
  settings.layers = 0;
  settings.turnsPerLayer = 0;
  settings.windingSpeedRpm = 100;
  return settings;
}

ParameterFlags clearParameterFlags() {
  ParameterFlags flags;
  flags.velocity = false;
  flags.layers = false;
  flags.turnsPerLayer = false;
  flags.coilHeight = false;
  return flags;
}

bool appendFourDigitValue(int currentValue, unsigned char digitIndex, char digit, int *nextValue) {
  if (nextValue == 0 || digit < '0' || digit > '9' || digitIndex > 3) {
    return false;
  }

  const int digitValue = digit - '0';
  const int multipliers[] = {1000, 100, 10, 1};
  *nextValue = currentValue + digitValue * multipliers[digitIndex];
  return true;
}

bool applyReceivedNumber(char command, int value, Settings *settings, ParameterFlags *flags) {
  if (settings == 0 || flags == 0) {
    return false;
  }

  switch (command) {
    case 'v':
      settings->windingSpeedRpm = value;
      flags->velocity = true;
      return true;

    case 'y':
      settings->layers = value;
      flags->layers = true;
      return true;

    case 't':
      settings->turnsPerLayer = value;
      flags->turnsPerLayer = true;
      return true;

    case 'h':
      settings->coilHeightMm = static_cast<float>(value) / 100.0f;
      flags->coilHeight = true;
      return true;

    default:
      return false;
  }
}

bool hasAllRequiredParameters(const ParameterFlags &flags) {
  return flags.velocity && flags.layers && flags.turnsPerLayer && flags.coilHeight;
}

bool calculateMotionPlan(const Settings &settings, MotionPlan *plan) {
  if (plan == 0 || settings.windingSpeedRpm <= 0 || settings.turnsPerLayer <= 0 ||
      settings.layers <= 0 || settings.coilHeightMm <= 0.0f) {
    return false;
  }

  const int winderStepsPerLayer = settings.turnsPerLayer * WINDER_MOTOR_STEPS;
  const int sliderStepsPerLayer =
      static_cast<int>(settings.coilHeightMm / SLIDER_DISTANCE_PER_TURN_MM * SLIDER_MOTOR_STEPS);

  if (winderStepsPerLayer <= 0 || sliderStepsPerLayer <= 0) {
    return false;
  }

  const float stepRatio = static_cast<float>(winderStepsPerLayer) /
                          static_cast<float>(sliderStepsPerLayer);
  const int stepRatioQuotient = static_cast<int>(stepRatio);

  if (stepRatioQuotient <= 0) {
    return false;
  }

  const float stepRatioRemainder = stepRatio - static_cast<float>(stepRatioQuotient);
  const float maxSpeed = 60.0f /
                         (static_cast<float>(STEPPER_CALL_OVERHEAD_MS) * WINDER_MOTOR_STEPS) *
                         1000.0f;
  int cappedSpeed = settings.windingSpeedRpm;
  if (cappedSpeed > static_cast<int>(maxSpeed)) {
    cappedSpeed = static_cast<int>(maxSpeed);
  }

  int delayMs = static_cast<int>(
      60.0f / (static_cast<float>(cappedSpeed) * WINDER_MOTOR_STEPS) * 1000.0f -
      STEPPER_CALL_OVERHEAD_MS);
  if (delayMs < 0) {
    delayMs = 0;
  }

  plan->cappedWindingSpeedRpm = cappedSpeed;
  plan->winderStepsPerLayer = winderStepsPerLayer;
  plan->sliderStepsPerLayer = sliderStepsPerLayer;
  plan->windingCount = stepRatioQuotient;
  plan->windingError = stepRatioRemainder / static_cast<float>(stepRatioQuotient);
  plan->checkValue = 1.0f / static_cast<float>(stepRatioQuotient);
  plan->delayMs = delayMs;
  return true;
}

}  // namespace WinderCore
