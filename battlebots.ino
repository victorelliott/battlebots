#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

int lc_pulse;
int rc_pulse;
int sc_pulse;

int R_STICK = 8;
int L_STICK = 9;
int DIAL = 10;

int MAX_SPEED = 255;  
int R_NEUTRAL = 1410;
int R_MAX = 1800;
int R_MIN = 1100;
int L_NEUTRAL = 1430;
int L_MAX = 1800;
int L_MIN = 1000;
int BUFFER = 20;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *r_wheel = AFMS.getMotor(1);
Adafruit_DCMotor *spinner = AFMS.getMotor(2);
Adafruit_DCMotor *l_wheel = AFMS.getMotor(3);

// channel 1 is left right on right part
// channel 2 is up down on right 
// channel 3 is up/down on left joy stick-
// channel 4 is left/right on left joystick
// channel 5 is right dial
// channel 6 is left dial

void setup()
{
  AFMS.begin();
  Serial.begin(9600);
  pinMode(R_STICK, INPUT);
  pinMode(L_STICK, INPUT);
  pinMode(DIAL, INPUT);
}

void loop()
{
  rc_pulse = pulseIn(R_STICK, HIGH);
  lc_pulse = pulseIn(L_STICK, HIGH);
  sc_pulse = pulseIn(DIAL, HIGH);

  // Debug
  Serial.println(rc_pulse);

  // Right stick neutral
  if (rc_pulse >= R_NEUTRAL - BUFFER && rc_pulse <= R_NEUTRAL + BUFFER) {
    // Left stick neutral
    if (lc_pulse >= L_NEUTRAL - BUFFER && lc_pulse <= L_NEUTRAL + BUFFER) {
      l_wheel->run(RELEASE);
      r_wheel->run(RELEASE);
    // Left stick left
    } else if (lc_pulse >= L_MIN && lc_pulse < L_NEUTRAL - BUFFER) {
      l_wheel->run(RELEASE);
      r_wheel->setSpeed(map(lc_pulse, L_MIN, L_NEUTRAL - BUFFER, MAX_SPEED, 0));
      r_wheel->run(BACKWARD);
    // Left stick right
    } else if (lc_pulse > L_NEUTRAL + BUFFER && lc_pulse <= L_MAX) {
      r_wheel->run(RELEASE);
      l_wheel->setSpeed(map(lc_pulse, L_NEUTRAL + BUFFER, L_MAX, 0, MAX_SPEED));
      l_wheel->run(BACKWARD);
    // Invalid left stick input
    } else {
      l_wheel->run(RELEASE);
      r_wheel->run(RELEASE);
    }
  // Right stick forward
  } else if (rc_pulse > R_NEUTRAL + BUFFER && rc_pulse <= R_MAX){
    // Left stick neutral
    if (lc_pulse >= L_NEUTRAL - BUFFER && lc_pulse <= L_NEUTRAL + BUFFER) {
      r_wheel->setSpeed(map(rc_pulse, R_NEUTRAL + BUFFER, R_MAX, 0, MAX_SPEED));
      l_wheel->setSpeed(map(rc_pulse, R_NEUTRAL + BUFFER, R_MAX, 0, MAX_SPEED));
      r_wheel->run(BACKWARD);
      l_wheel->run(BACKWARD);
    // Left stick left
    // turning left while moving forward
    } else if (lc_pulse >= L_MIN && lc_pulse < L_NEUTRAL - BUFFER) {
      r_wheel->setSpeed(map(rc_pulse, R_NEUTRAL + BUFFER, R_MAX, 0, MAX_SPEED));
      l_wheel->setSpeed(map(rc_pulse, R_NEUTRAL + BUFFER, R_MAX, 0, MAX_SPEED / 2));
      r_wheel->run(BACKWARD);
      l_wheel->run(BACKWARD);
    // Left stick right
    // turning right while moving forward
    } else if (lc_pulse > L_NEUTRAL + BUFFER && lc_pulse <= L_MAX) {
      r_wheel->setSpeed(map(rc_pulse, R_NEUTRAL + BUFFER, R_MAX, 0, MAX_SPEED / 2));
      l_wheel->setSpeed(map(rc_pulse, R_NEUTRAL + BUFFER, R_MAX, 0, MAX_SPEED));
      r_wheel->run(BACKWARD);
      l_wheel->run(BACKWARD);
    // Invalid left stick input
    } else {
      l_wheel->run(RELEASE);
      r_wheel->run(RELEASE);
    }
  // Right stick backward
  } else if (rc_pulse >= R_MIN && rc_pulse < R_NEUTRAL - BUFFER) {
    // Left stick neutral
    if (lc_pulse >= L_NEUTRAL - BUFFER && lc_pulse <= L_NEUTRAL + BUFFER) {
      r_wheel->setSpeed(map(rc_pulse, R_MIN, R_NEUTRAL - BUFFER, MAX_SPEED, 0));
      l_wheel->setSpeed(map(rc_pulse, R_MIN, R_NEUTRAL - BUFFER, MAX_SPEED, 0));
      r_wheel->run(FORWARD);
      l_wheel->run(FORWARD);
    // Left stick left
    // turning right in reverse
    } else if (lc_pulse >= L_MIN && lc_pulse < L_NEUTRAL - BUFFER) {
      r_wheel->setSpeed(map(rc_pulse, R_MIN, R_NEUTRAL - BUFFER, MAX_SPEED / 2, 0));
      l_wheel->setSpeed(map(rc_pulse, R_MIN, R_NEUTRAL - BUFFER, MAX_SPEED, 0));
      r_wheel->run(FORWARD);
      l_wheel->run(FORWARD);
    // Left stick right
    // turning left in reverse
    } else if (lc_pulse > L_NEUTRAL + BUFFER && lc_pulse <= L_MAX) {
      r_wheel->setSpeed(map(rc_pulse, R_MIN, R_NEUTRAL - BUFFER, MAX_SPEED, 0));
      l_wheel->setSpeed(map(rc_pulse, R_MIN, R_NEUTRAL - BUFFER, MAX_SPEED / 2, 0));
      r_wheel->run(FORWARD);
      l_wheel->run(FORWARD);
    // Invalid left stick input
    } else {
      l_wheel->run(RELEASE);
      r_wheel->run(RELEASE);
    }
  // Invalid right stick input
  } else {
    l_wheel->run(RELEASE);
    r_wheel->run(RELEASE);
  }

  // Dial activated
  // activates weaponized motor
  if (sc_pulse > 1200) {
    spinner->setSpeed(MAX_SPEED);
    spinner->run(BACKWARD);
  } else {
    spinner->run(RELEASE);
  }
}
