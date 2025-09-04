#pragma once
#include <Bluepad32.h>

extern ControllerPtr myControllers[BP32_MAX_GAMEPADS];

const char* getPressedButtons(ControllerPtr ctl);
void processGamepad(ControllerPtr ctl);
void saveConfig();
void loadConfig();
void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);
