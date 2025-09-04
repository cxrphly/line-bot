#include <Arduino.h>
#include <Preferences.h>
#include "LineFollower.h"
#include "Config.h"
#include "motors.h"

QTRSensors qtr;
uint16_t sensorValues[SensorCount];
Preferences prefs;

bool calibrating = false;
int calibCount = 0;
unsigned long calibLastMillis = 0;

float integral = 0;

void setupQTR() {
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[])
        { QTR_D1, QTR_D2, QTR_D3, QTR_D4, QTR_D5, QTR_D6, QTR_D7, QTR_D8 }, SensorCount);
    qtr.setEmitterPin(QTR_IR);

    pinMode(QTR_IR, OUTPUT);
    digitalWrite(QTR_IR, HIGH);

    if (!loadCalibration()) {
        Serial.println("Nenhuma calibracao salva. Execute calibracao!");
    } else {
        Serial.println("Calibracao carregada da memoria!");
    }
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

        if (calibCount % 50 < 25) setMotor(120, -120);
        else setMotor(-120, 120);

        if (calibCount >= 400) {
            calibrating = false;
            setMotor(0, 0);
            Serial.println("=== Calibracao concluida ===");
            saveCalibration();
        }
    }
}

void saveCalibration() {
    prefs.begin("qtr", false);

    if (!qtr.calibrationOn.initialized || !qtr.calibrationOn.minimum || !qtr.calibrationOn.maximum) {
        Serial.println("Erro: tentativa de salvar sem calibracao valida!");
        prefs.end();
        return;
    }

    for (uint8_t i = 0; i < SensorCount; i++) {
        prefs.putUInt(("min" + String(i)).c_str(), qtr.calibrationOn.minimum[i]);
        prefs.putUInt(("max" + String(i)).c_str(), qtr.calibrationOn.maximum[i]);
    }

    prefs.end();
    Serial.println("Calibracao salva na memoria!");
}

bool loadCalibration() {
    prefs.begin("qtr", true);
    bool found = true;

    if (!qtr.calibrationOn.initialized) {
        qtr.calibrationOn.minimum = (uint16_t*)malloc(sizeof(uint16_t) * SensorCount);
        qtr.calibrationOn.maximum = (uint16_t*)malloc(sizeof(uint16_t) * SensorCount);
        if (!qtr.calibrationOn.minimum || !qtr.calibrationOn.maximum) {
            Serial.println("Erro: falha ao alocar memória para calibração!");
            prefs.end();
            return false;
        }
        qtr.calibrationOn.initialized = true;
        for (uint8_t i = 0; i < SensorCount; i++) {
            qtr.calibrationOn.minimum[i] = 0;
            qtr.calibrationOn.maximum[i] = 1000;
        }
    }

    for (uint8_t i = 0; i < SensorCount; i++) {
        uint16_t minVal = prefs.getUInt(("min" + String(i)).c_str(), 65535);
        uint16_t maxVal = prefs.getUInt(("max" + String(i)).c_str(), 0);

        if (minVal == 65535 && maxVal == 0) {
            found = false;
            break;
        }

        qtr.calibrationOn.minimum[i] = minVal;
        qtr.calibrationOn.maximum[i] = maxVal;
    }

    prefs.end();
    return found;
}
void saveConfig() {
    prefs.begin("pid", false);
    prefs.putFloat("Kp", Kp);
    prefs.putFloat("Ki", Ki);
    prefs.putFloat("Kd", Kd);
    prefs.putUInt("maxSpeed", maxSpeed);
    prefs.putUInt("threshold", threshold);
    prefs.end();
    Serial.println("Configurações salvas");
}

void loadConfig() {
    prefs.begin("pid", true);
    Kp = prefs.getFloat("Kp", Kp);
    Ki = prefs.getFloat("Ki", Ki);
    Kd = prefs.getFloat("Kd", Kd);
    maxSpeed = prefs.getUInt("maxSpeed", maxSpeed);
    threshold = prefs.getUInt("threshold", threshold);
    prefs.end();
    Serial.printf("Config carregada: Kp=%.3f Ki=%.3f Kd=%.3f maxSpeed=%d threshold=%d\n",Kp, Ki, Kd, maxSpeed, threshold);
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
        for (int i = 0; i < SensorCount; i++) {
            Serial.print(sensorValues[i]);
            Serial.print(sensorValues[i] > threshold ? "(P) " : "(B) ");
        }
        Serial.println();
    }
}

void lineFollowerControl() {
    uint16_t position = qtr.readLineBlack(sensorValues);

    bool allWhite = true;
    for (int i = 0; i < SensorCount; i++) {
        if (sensorValues[i] > threshold) { allWhite = false; break; }
    }
    if (allWhite) {
        setMotor(0, 0);
        integral *= 0.8;
        Serial.println("Linha perdida");
        return;
    }

    int error = (int)position - 3500;

    integral += error;
    integral = constrain(integral, -500, 500);
    int derivative = error - lastError;
    lastError = error;

    int motorCorr = (int)(Kp * error + Ki * integral + Kd * derivative);
    motorCorr = constrain(motorCorr, -maxSpeed, maxSpeed);

    int baseSpeed = maxSpeed - abs(motorCorr) / 2;
    baseSpeed = constrain(baseSpeed, maxSpeed / 3, maxSpeed);

    int left  = baseSpeed + motorCorr;
    int right = baseSpeed - motorCorr;

    left  = constrain(left, 0, 255);
    right = constrain(right, 0, 255);

    debugQTR(position, error, motorCorr, left, right);
    setMotor(left, right);
}
