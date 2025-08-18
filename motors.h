// motors.h
#pragma once
#include "Config.h"
#include <Arduino.h>
#include <Bluepad32.h>

void controlMotors(ControllerPtr ctl);
void setupMotors();
void setMotor(int leftSpeed, int rightSpeed);
void setMotorRawA(int pwm, bool forward);
void setMotorRawB(int pwm, bool forward);
