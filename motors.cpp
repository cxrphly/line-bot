#include "motors.h"
#include "Config.h"
#include <Arduino.h>

void controlMotors(ControllerPtr ctl) {
  int ly = -ctl->axisY();
  int lx =  ctl->axisX();

  int speed = map(ly, -512, 512, -255, 255);
  int turn  = map(lx, -512, 512, -255, 255);

  int right = constrain(speed + turn, -255, 255);
  int left  = constrain(speed - turn, -255, 255);

  setMotor(left, right);
}

void setMotorRawA(int pwm, bool forward) {
    digitalWrite(AIN1, forward ? HIGH : LOW);
    digitalWrite(AIN2, forward ? LOW : HIGH);
    ledcWrite(CHANNEL_A, pwm);
}

void setMotorRawB(int pwm, bool forward) {
    digitalWrite(BIN1, forward ? HIGH : LOW);
    digitalWrite(BIN2, forward ? LOW : HIGH);
    ledcWrite(CHANNEL_B, pwm);
}

void setMotor(int leftSpeed, int rightSpeed) {
    leftSpeed  = constrain(leftSpeed,  -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);

    setMotorRawB(abs(leftSpeed), leftSpeed >= 0);
    setMotorRawA(abs(rightSpeed), rightSpeed >= 0);
}

void setupMotors() {
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);

    ledcSetup(CHANNEL_A, PWM_FREQ, PWM_RES_BITS);
    ledcAttachPin(PWMA, CHANNEL_A);
    ledcSetup(CHANNEL_B, PWM_FREQ, PWM_RES_BITS);
    ledcAttachPin(PWMB, CHANNEL_B);
}
