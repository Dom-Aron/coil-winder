#include <cmath>
#include <iostream>

#include "../Source/OpenCoilWinder/WinderCore.h"

namespace {

void expectTrue(bool condition, const char *message) {
  if (!condition) {
    std::cerr << "FAIL: " << message << '\n';
    std::exit(1);
  }
}

void expectEqual(int actual, int expected, const char *message) {
  if (actual != expected) {
    std::cerr << "FAIL: " << message << " (actual=" << actual << ", expected=" << expected
              << ")\n";
    std::exit(1);
  }
}

void expectNear(float actual, float expected, float tolerance, const char *message) {
  if (std::fabs(actual - expected) > tolerance) {
    std::cerr << "FAIL: " << message << " (actual=" << actual << ", expected=" << expected
              << ")\n";
    std::exit(1);
  }
}

void parsesFourDigitNumbersLikeOriginalSketch() {
  int value = 0;

  expectTrue(WinderCore::appendFourDigitValue(value, 0, '1', &value), "digit 1 should parse");
  expectEqual(value, 1000, "first digit should be thousands");

  expectTrue(WinderCore::appendFourDigitValue(value, 1, '2', &value), "digit 2 should parse");
  expectEqual(value, 1200, "second digit should be hundreds");

  expectTrue(WinderCore::appendFourDigitValue(value, 2, '3', &value), "digit 3 should parse");
  expectEqual(value, 1230, "third digit should be tens");

  expectTrue(WinderCore::appendFourDigitValue(value, 3, '4', &value), "digit 4 should parse");
  expectEqual(value, 1234, "fourth digit should be units");

  expectTrue(!WinderCore::appendFourDigitValue(value, 4, '5', &value), "fifth digit is invalid");
  expectTrue(!WinderCore::appendFourDigitValue(value, 0, 'x', &value), "non-digit is invalid");
}

void appliesSerialParameters() {
  WinderCore::Settings settings = WinderCore::defaultSettings();
  WinderCore::ParameterFlags flags = WinderCore::clearParameterFlags();

  expectTrue(WinderCore::applyReceivedNumber('v', 100, &settings, &flags), "speed command");
  expectEqual(settings.windingSpeedRpm, 100, "speed value");
  expectTrue(flags.velocity, "speed flag");

  expectTrue(WinderCore::applyReceivedNumber('y', 3, &settings, &flags), "layers command");
  expectEqual(settings.layers, 3, "layers value");
  expectTrue(flags.layers, "layers flag");

  expectTrue(WinderCore::applyReceivedNumber('t', 50, &settings, &flags), "turns command");
  expectEqual(settings.turnsPerLayer, 50, "turns value");
  expectTrue(flags.turnsPerLayer, "turns flag");

  expectTrue(WinderCore::applyReceivedNumber('h', 125, &settings, &flags), "height command");
  expectNear(settings.coilHeightMm, 1.25f, 0.0001f, "height uses original /100 scale");
  expectTrue(flags.coilHeight, "height flag");

  expectTrue(WinderCore::hasAllRequiredParameters(flags), "all required flags");
  expectTrue(!WinderCore::applyReceivedNumber('x', 1, &settings, &flags), "unknown command");
}

void calculatesMotionPlanLikeOriginalSketch() {
  WinderCore::Settings settings;
  settings.coilHeightMm = 5.0f;
  settings.layers = 2;
  settings.turnsPerLayer = 100;
  settings.windingSpeedRpm = 100;

  WinderCore::MotionPlan plan;
  expectTrue(WinderCore::calculateMotionPlan(settings, &plan), "valid plan");

  expectEqual(plan.winderStepsPerLayer, 20000, "winder steps per layer");
  expectEqual(plan.sliderStepsPerLayer, 200, "slider steps per layer");
  expectEqual(plan.windingCount, 100, "winding count");
  expectNear(plan.windingError, 0.0f, 0.0001f, "winding error");
  expectNear(plan.checkValue, 0.01f, 0.0001f, "check value");
  expectEqual(plan.cappedWindingSpeedRpm, 75, "speed is capped to original max");
  expectEqual(plan.delayMs, 0, "delay for capped speed");
}

void keepsRemainderCompensationValues() {
  WinderCore::Settings settings;
  settings.coilHeightMm = 3.0f;
  settings.layers = 4;
  settings.turnsPerLayer = 17;
  settings.windingSpeedRpm = 60;

  WinderCore::MotionPlan plan;
  expectTrue(WinderCore::calculateMotionPlan(settings, &plan), "valid remainder plan");

  expectEqual(plan.winderStepsPerLayer, 3400, "remainder winder steps");
  expectEqual(plan.sliderStepsPerLayer, 120, "remainder slider steps");
  expectEqual(plan.windingCount, 28, "remainder winding count");
  expectNear(plan.windingError, 0.0119047f, 0.0001f, "remainder winding error");
  expectNear(plan.checkValue, 0.0357142f, 0.0001f, "remainder check value");
  expectEqual(plan.delayMs, 1, "delay at 60 rpm");
}

void rejectsUnsafeMotionPlans() {
  WinderCore::MotionPlan plan;
  WinderCore::Settings settings = WinderCore::defaultSettings();

  expectTrue(!WinderCore::calculateMotionPlan(settings, &plan), "default zero values are invalid");

  settings.coilHeightMm = 5.0f;
  settings.layers = 1;
  settings.turnsPerLayer = 1;
  settings.windingSpeedRpm = 0;
  expectTrue(!WinderCore::calculateMotionPlan(settings, &plan), "zero speed is invalid");
}

}  // namespace

int main() {
  parsesFourDigitNumbersLikeOriginalSketch();
  appliesSerialParameters();
  calculatesMotionPlanLikeOriginalSketch();
  keepsRemainderCompensationValues();
  rejectsUnsafeMotionPlans();

  std::cout << "All WinderCore tests passed.\n";
  return 0;
}
