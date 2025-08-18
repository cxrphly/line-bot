#pragma once
#include <Arduino.h>
#include <Bluepad32.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define MISC_BUTTON_HOME 0x01
#define MISC_BUTTON_START 0x04
#define MISC_BUTTON_SELECT 0x02

#define AIN1  5
#define AIN2  18
#define PWMA  19
#define BIN1  4
#define BIN2  2
#define PWMB  15

#define CHANNEL_A  0
#define CHANNEL_B  1
#define PWM_FREQ     1000
#define PWM_RES_BITS 8

#define QTR_D1 14
#define QTR_D2 27
#define QTR_D3 26
#define QTR_D4 25
#define QTR_D5 33
#define QTR_D6 32
#define QTR_D7 35
#define QTR_D8 34
#define QTR_IR 13
constexpr uint8_t SensorCount = 8;

extern int maxSpeed;
extern float Kp;
extern float Kd;
extern int lastError;
extern int threshold;
extern float filteredError;

enum AppState {
  STATE_SPLASH,
  STATE_MENU,
  STATE_EIXOS,
  STATE_BOTOES,
  STATE_LINEFOLLOWER_MENU
};
extern AppState appState;

extern int menuIndex;
extern bool lineFollowerActive;
extern bool controllerModeActive;

extern const int menuItemCount;
extern const char* menuItems[];

extern int lfMenuIndex;
extern const int lfMenuItemCount;
extern const char* lfMenuItems[];

extern unsigned long lastDebug;
extern const unsigned long debugInterval;

extern unsigned long lastInputTime;
extern const unsigned long inputDelay;
