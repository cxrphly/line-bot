#include <Arduino.h>
#include "LineFollower.h"
#include "Config.h"
#include "motors.h"
// o proximo grande passo desse projeto será melhorar o modo seguidor de linha seguindo o regulamento da ROBOCORE.
QTRSensors qtr;
uint16_t sensorValues[8];

bool calibrating = false;
int calibCount = 0;
unsigned long calibLastMillis = 0;

void setupQTR() {
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[])
        { QTR_D1, QTR_D2, QTR_D3, QTR_D4, QTR_D5, QTR_D6, QTR_D7, QTR_D8 }, SensorCount);
    qtr.setEmitterPin(QTR_IR);
    pinMode(QTR_IR, OUTPUT);
    digitalWrite(QTR_IR, HIGH);
}

void startCalibration() {
    Serial.println("=== Iniciando calibracao ===");
    calibCount = 0;
    calibrating = true;
    calibLastMillis = millis();
}

void processCalibration() {
    if (!calibrating) return;
    if (millis() - calibLastMillis >= 20) {
        qtr.calibrate();
        calibCount++;
        calibLastMillis = millis();
        if (calibCount >= 400) {
            calibrating = false;
            Serial.println("=== Calibracao concluida ===");
        }
    }
}

void debugQTR(uint16_t position, int error, int motorCorr, int left, int right) {
    if (millis() - lastDebug >= debugInterval) {
        lastDebug = millis();
        Serial.print("Pos: "); Serial.print(position);
        Serial.print(" | Erro: "); Serial.print(error);
        Serial.print(" | PID: "); Serial.print(motorCorr);
        Serial.print(" | Esq: "); Serial.print(left);
        Serial.print(" | Dir: "); Serial.print(right);
        Serial.print(" | Sensores: ");
        for(int i=0; i<SensorCount; i++){
            Serial.print(sensorValues[i]>threshold?"Preto ":"Branco ");
        } 
        Serial.println();
    }
}

void lineFollowerControl() {
  uint16_t position = qtr.readLineBlack(sensorValues);
  int error = (int)position - 3500;

  filteredError = 0.7f * filteredError + 0.3f * error;

  int motorCorr = (int)(Kp * filteredError + Kd * (filteredError - lastError));
  lastError = filteredError;

  motorCorr = constrain(motorCorr, -maxSpeed, maxSpeed);

  int left  = maxSpeed + motorCorr;
  int right = maxSpeed - motorCorr;
  left  = constrain(left, 0, 255);
  right = constrain(right, 0, 255);

  debugQTR(position, error, motorCorr, left, right);
  setMotor(left, right);
}
