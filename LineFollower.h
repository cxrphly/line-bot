#pragma once
#include <QTRSensors.h>
#include "Config.h"

extern QTRSensors qtr;
extern uint16_t sensorValues[8];
extern const uint8_t SensorCount;

void saveConfig();
void loadConfig();
void setupQTR();
void startCalibration();
void processCalibration();
void lineFollowerControl();
void saveCalibration();
bool loadCalibration();
void debugQTR(uint16_t position, int error, int motorCorr, int left, int right);
