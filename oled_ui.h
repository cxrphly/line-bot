#pragma once
#include <Adafruit_SSD1306.h>
#include <Bluepad32.h>

extern Adafruit_SSD1306 display;

void showSplashScreen();
void showMainMenu();
void showConfigMenu();
void showLineFollowerMenu();
void showControllerInfo(ControllerPtr ctl);
void showButtonsInfo(ControllerPtr ctl);
void drawProgressBar(int x, int y, int width, int height, int value, int maxValue);
