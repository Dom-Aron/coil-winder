#include <Stepper.h>

#include "WinderCore.h"

using namespace WinderCore;

const int WINDER_PIN_1 = 9;
const int WINDER_PIN_2 = 10;
const int WINDER_PIN_3 = 11;
const int WINDER_PIN_4 = 12;

const int SLIDER_PIN_1 = 5;
const int SLIDER_PIN_2 = 6;
const int SLIDER_PIN_3 = 7;
const int SLIDER_PIN_4 = 8;

const int COMM_MODE_COMMAND = 11;
const int COMM_MODE_NUMBER = 12;

const int LARGE_SLIDER_JOG_STEPS = SLIDER_MOTOR_STEPS * 2 * 2;  // 2 mm
const int SMALL_SLIDER_JOG_STEPS = 8;                           // 0.2 mm

Stepper stepperWinder(WINDER_MOTOR_STEPS, WINDER_PIN_1, WINDER_PIN_2, WINDER_PIN_3, WINDER_PIN_4);
Stepper stepperSlider(SLIDER_MOTOR_STEPS, SLIDER_PIN_1, SLIDER_PIN_2, SLIDER_PIN_3, SLIDER_PIN_4);

Settings settings;
ParameterFlags parameterFlags;
MotionPlan motionPlan;

byte currentCommand;
int direction;
int directionBackup;
int communicationMode;
bool isPaused;

byte numberDataCount;
int numberData;

int winderStepCount;
int layerCount;
int sliderStepCountdown;
float windingErrorSum;

void resetMotors() {
  digitalWrite(SLIDER_PIN_1, LOW);
  digitalWrite(SLIDER_PIN_2, LOW);
  digitalWrite(SLIDER_PIN_3, LOW);
  digitalWrite(SLIDER_PIN_4, LOW);

  digitalWrite(WINDER_PIN_1, LOW);
  digitalWrite(WINDER_PIN_2, LOW);
  digitalWrite(WINDER_PIN_3, LOW);
  digitalWrite(WINDER_PIN_4, LOW);
}

void resetWindingCounters() {
  winderStepCount = 0;
  layerCount = 0;
  sliderStepCountdown = 0;
  windingErrorSum = 0.0f;
}

void resetRuntimeState() {
  settings = defaultSettings();
  parameterFlags = clearParameterFlags();

  motionPlan.cappedWindingSpeedRpm = settings.windingSpeedRpm;
  motionPlan.winderStepsPerLayer = 0;
  motionPlan.sliderStepsPerLayer = 10;
  motionPlan.windingCount = 0;
  motionPlan.windingError = 0.0f;
  motionPlan.checkValue = 0.0f;
  motionPlan.delayMs = 0;

  currentCommand = 0;
  direction = MOTOR_STOP;
  directionBackup = MOTOR_STOP;
  communicationMode = COMM_MODE_COMMAND;
  isPaused = false;
  numberDataCount = 0;
  numberData = 0;

  resetWindingCounters();
}

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only.
  }

  stepperWinder.setSpeed(75);
  stepperSlider.setSpeed(75);

  resetRuntimeState();
}

void handleCompletedNumber() {
  if (!applyReceivedNumber(currentCommand, numberData, &settings, &parameterFlags)) {
    Serial.println("Command Error while a Number Data is saving");
  }

  numberDataCount = 0;
  numberData = 0;
  communicationMode = COMM_MODE_COMMAND;
}

void handleNumberByte(byte receiveByte) {
  if (communicationMode != COMM_MODE_NUMBER) {
    Serial.println("Received a Number Data during C_MODE");
    return;
  }

  int nextNumberData = numberData;
  if (!appendFourDigitValue(numberData, numberDataCount, static_cast<char>(receiveByte), &nextNumberData)) {
    Serial.println("Count Error of a Number Data");
    return;
  }

  numberData = nextNumberData;
  numberDataCount++;

  if (numberDataCount >= 4) {
    handleCompletedNumber();
  }
}

void prepareMotionPlan() {
  if (!calculateMotionPlan(settings, &motionPlan)) {
    Serial.println("Invalid winding parameters.");
    return;
  }

  settings.windingSpeedRpm = motionPlan.cappedWindingSpeedRpm;
  stepperWinder.setSpeed(settings.windingSpeedRpm);

  if (isPaused) {
    direction = directionBackup;
    isPaused = false;
  }

  if (!hasAllRequiredParameters(parameterFlags)) {
    return;
  }

  if (direction != MOTOR_STOP) {
    Serial.println("Can't start the winder because the winder already is operating.");
  } else {
    direction = SLIDER_MOTOR_CW;
  }

  parameterFlags = clearParameterFlags();
}

void stopWinding() {
  direction = MOTOR_STOP;
  resetWindingCounters();
  resetMotors();
}

void handleCommandByte(byte receiveByte) {
  switch (receiveByte) {
    case 's':
      prepareMotionPlan();
      break;

    case 'v':
    case 'y':
    case 't':
    case 'h':
      currentCommand = receiveByte;
      communicationMode = COMM_MODE_NUMBER;
      numberData = 0;
      numberDataCount = 0;
      break;

    case 'l':
      stepperSlider.step(-LARGE_SLIDER_JOG_STEPS);
      break;

    case 'r':
      stepperSlider.step(LARGE_SLIDER_JOG_STEPS);
      break;

    case 'm':
      stepperSlider.step(-SMALL_SLIDER_JOG_STEPS);
      break;

    case 'n':
      stepperSlider.step(SMALL_SLIDER_JOG_STEPS);
      break;

    case 'p':
      if (direction != MOTOR_STOP) {
        directionBackup = direction;
        direction = MOTOR_STOP;
        isPaused = true;
        resetMotors();
      }
      break;

    case 'q':
      stopWinding();
      break;

    case 'o':
      Serial.println("o");
      break;

    default:
      Serial.println("Received a wrong command.");
      break;
  }
}

void handleSerialInput() {
  if (Serial.available() <= 0) {
    return;
  }

  const byte receiveByte = Serial.read();
  if (receiveByte >= '0' && receiveByte <= '9') {
    handleNumberByte(receiveByte);
  } else {
    handleCommandByte(receiveByte);
  }
}

void stepSliderIfNeeded() {
  sliderStepCountdown++;

  if (sliderStepCountdown < motionPlan.windingCount) {
    return;
  }

  if (windingErrorSum < motionPlan.checkValue) {
    if (direction == SLIDER_MOTOR_CW) {
      stepperSlider.step(1);
    }
    if (direction == SLIDER_MOTOR_CCW) {
      stepperSlider.step(-1);
    }

    windingErrorSum += motionPlan.windingError;
    sliderStepCountdown = 0;
  } else {
    windingErrorSum -= motionPlan.checkValue;
    sliderStepCountdown--;
  }
}

void finishLayerIfNeeded() {
  if (winderStepCount < motionPlan.winderStepsPerLayer) {
    return;
  }

  layerCount++;

  if (layerCount >= settings.layers) {
    direction = MOTOR_STOP;
    layerCount = 0;
    resetMotors();
  } else if (direction == SLIDER_MOTOR_CW) {
    direction = SLIDER_MOTOR_CCW;
  } else if (direction == SLIDER_MOTOR_CCW) {
    direction = SLIDER_MOTOR_CW;
  }

  winderStepCount = 0;
  windingErrorSum = 0.0f;
  sliderStepCountdown = 0;
}

void rotateMotors() {
  if (direction != SLIDER_MOTOR_CW && direction != SLIDER_MOTOR_CCW) {
    return;
  }

  stepperWinder.step(-1);
  stepSliderIfNeeded();
  delay(motionPlan.delayMs);

  winderStepCount++;
  finishLayerIfNeeded();
}

void loop() {
  handleSerialInput();
  rotateMotors();
}
