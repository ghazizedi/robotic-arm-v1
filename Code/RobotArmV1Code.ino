#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm(0x40);

const int joy1XPin = A0;
const int joy1YPin = A1;
const int joy2XPin = A2;
const int joy2YPin = A3;

const int buttonPin = 2;

const int ch0 = 0;
const int ch1 = 1;
const int ch2 = 2;
const int ch3 = 3;
const int ch4 = 4;
const int ch5 = 5;

const int SERVO_MIN_180 = 110;
const int SERVO_MAX_180 = 510;

const int SERVO_MIN_270 = 90;
const int SERVO_MAX_270 = 560;

const float MAX_ANGLE_180 = 180.0;
const float MAX_ANGLE_270 = 270.0;

const int deadzone = 60;
const float maxVel = 1.0;
const float accelFactor = 0.08;

const float stopSnapVel = 0.02;

bool modeAlt = false;
int lastButtonState = HIGH;
unsigned long lastToggleMs = 0;
const unsigned long debounceMs = 180;

float pos0 = 90;
float pos1 = 135;
float pos2 = 135;
float pos3 = 90;
float pos4 = 90;
float pos5 = 90;

float vel0 = 0;
float vel1 = 0;
float vel2 = 0;
float vel3 = 0;
float vel4 = 0;
float vel5 = 0;

const bool invert0 = false;
const bool invert1 = false;
const bool invert2 = false;
const bool invert3 = false;
const bool invert4 = true;
const bool invert5 = false;

const float min0 = 0,   max0 = 180;
const float min1 = 0,   max1 = 270;
const float min2 = 0,   max2 = 270;
const float min3 = 0,   max3 = 180;
const float min4 = 0,   max4 = 180;
const float min5 = 0,   max5 = 180;

float clampf(float v, float lo, float hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

float targetVelFromOffset(int offset) {
  int a = abs(offset);
  if (a <= deadzone) return 0;
  float norm = (a - deadzone) / (512.0 - deadzone);
  if (norm < 0) norm = 0;
  if (norm > 1) norm = 1;
  return norm * maxVel * (offset > 0 ? 1 : -1);
}

int angleToPulse(float angle, float maxAngle, int pMin, int pMax) {
  angle = clampf(angle, 0, maxAngle);
  float t = angle / maxAngle;
  return (int)(pMin + t * (pMax - pMin) + 0.5);
}

bool is270Channel(int ch) {
  return (ch == ch1) || (ch == ch2);
}

int pulseForChannel(int ch, float angle) {
  if (is270Channel(ch)) {
    return angleToPulse(angle, MAX_ANGLE_270, SERVO_MIN_270, SERVO_MAX_270);
  }
  return angleToPulse(angle, MAX_ANGLE_180, SERVO_MIN_180, SERVO_MAX_180);
}

float maxAngleForChannel(int ch) {
  return is270Channel(ch) ? MAX_ANGLE_270 : MAX_ANGLE_180;
}

void writeChannel(int ch, float rawAngle, bool invert) {
  float maxA = maxAngleForChannel(ch);
  float a = invert ? (maxA - rawAngle) : rawAngle;
  pwm.setPWM(ch, 0, pulseForChannel(ch, a));
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  pwm.begin();
  pwm.setPWMFreq(50);

  pos0 = clampf(pos0, min0, max0);
  pos1 = clampf(pos1, min1, max1);
  pos2 = clampf(pos2, min2, max2);
  pos3 = clampf(pos3, min3, max3);
  pos4 = clampf(pos4, min4, max4);
  pos5 = clampf(pos5, min5, max5);

  writeChannel(ch0, pos0, invert0);
  writeChannel(ch1, pos1, invert1);
  writeChannel(ch2, pos2, invert2);
  writeChannel(ch3, pos3, invert3);
  writeChannel(ch4, pos4, invert4);
  writeChannel(ch5, pos5, invert5);

  delay(200);
}

void loop() {
  int b = digitalRead(buttonPin);
  unsigned long now = millis();
  if (lastButtonState == HIGH && b == LOW && (now - lastToggleMs) > debounceMs) {
    modeAlt = !modeAlt;
    lastToggleMs = now;
  }
  lastButtonState = b;

  int off0 = analogRead(joy1XPin) - 512;
  int off1 = analogRead(joy1YPin) - 512;
  int off2 = analogRead(joy2XPin) - 512;
  int off3 = analogRead(joy2YPin) - 512;

  float t0 = targetVelFromOffset(off0);
  float t1 = targetVelFromOffset(off1);

  float t2 = 0, t3 = 0, t4 = 0, t5 = 0;

  if (!modeAlt) {
    t2 = targetVelFromOffset(off2);
    t3 = targetVelFromOffset(off3);
  } else {
    t4 = targetVelFromOffset(off2);
    t5 = targetVelFromOffset(off3);
  }

  vel0 += (t0 - vel0) * accelFactor;
  vel1 += (t1 - vel1) * accelFactor;
  vel2 += (t2 - vel2) * accelFactor;
  vel3 += (t3 - vel3) * accelFactor;
  vel4 += (t4 - vel4) * accelFactor;
  vel5 += (t5 - vel5) * accelFactor;

  if (t0 == 0 && abs(vel0) < stopSnapVel) vel0 = 0;
  if (t1 == 0 && abs(vel1) < stopSnapVel) vel1 = 0;
  if (t2 == 0 && abs(vel2) < stopSnapVel) vel2 = 0;
  if (t3 == 0 && abs(vel3) < stopSnapVel) vel3 = 0;
  if (t4 == 0 && abs(vel4) < stopSnapVel) vel4 = 0;
  if (t5 == 0 && abs(vel5) < stopSnapVel) vel5 = 0;

  pos0 = clampf(pos0 + vel0, min0, max0);
  pos1 = clampf(pos1 + vel1, min1, max1);
  pos2 = clampf(pos2 + vel2, min2, max2);
  pos3 = clampf(pos3 + vel3, min3, max3);
  pos4 = clampf(pos4 + vel4, min4, max4);
  pos5 = clampf(pos5 + vel5, min5, max5);

  writeChannel(ch0, pos0, invert0);
  writeChannel(ch1, pos1, invert1);
  writeChannel(ch2, pos2, invert2);
  writeChannel(ch3, pos3, invert3);
  writeChannel(ch4, pos4, invert4);
  writeChannel(ch5, pos5, invert5);

  delay(15);
}
