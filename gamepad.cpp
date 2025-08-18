#include <Arduino.h>
#include "Gamepad.h"
#include "OLED_UI.h"
#include "Motors.h"
#include "LineFollower.h"
#include "Config.h"

const char* getPressedButtons(ControllerPtr ctl) {
  static char buffer[64];
  buffer[0] = '\0';

  if (ctl->a()) strcat(buffer, "A ");
  if (ctl->b()) strcat(buffer, "B ");
  if (ctl->x()) strcat(buffer, "X ");
  if (ctl->y()) strcat(buffer, "Y ");
  if (ctl->l1()) strcat(buffer, "LB ");
  if (ctl->r1()) strcat(buffer, "RB ");
  if (ctl->l2()) strcat(buffer, "LT ");
  if (ctl->r2()) strcat(buffer, "RT ");
  if (ctl->thumbL()) strcat(buffer, "LS ");
  if (ctl->thumbR()) strcat(buffer, "RS ");

  uint8_t misc = ctl->miscButtons();
  if (misc & MISC_BUTTON_HOME)   strcat(buffer, "BACK ");
  if (misc & MISC_BUTTON_START)  strcat(buffer, "START ");
  if (misc & MISC_BUTTON_SELECT) strcat(buffer, "SELECT ");

  uint8_t dpad = ctl->dpad();
  if (dpad & DPAD_UP)    strcat(buffer, "^ ");
  if (dpad & DPAD_DOWN)  strcat(buffer, "v ");
  if (dpad & DPAD_LEFT)  strcat(buffer, "< ");
  if (dpad & DPAD_RIGHT) strcat(buffer, "> ");

  return buffer;
}

void processGamepad(ControllerPtr ctl) {
  uint8_t dpad = ctl->dpad();
  uint8_t misc = ctl->miscButtons();
  unsigned long now = millis();

  switch (appState) {
    case STATE_MENU:
      if ((dpad & DPAD_DOWN) && now - lastInputTime > inputDelay) { menuIndex = (menuIndex + 1) % menuItemCount; lastInputTime = now; }
      if ((dpad & DPAD_UP) && now - lastInputTime > inputDelay) { menuIndex = (menuIndex - 1 + menuItemCount) % menuItemCount; lastInputTime = now; }

      if ((misc & MISC_BUTTON_START) && now - lastInputTime > inputDelay) {
        if (menuIndex == 0) appState = STATE_LINEFOLLOWER_MENU;
        else if (menuIndex == 1) controllerModeActive = !controllerModeActive;
        else if (menuIndex == 2) appState = STATE_EIXOS;
        else if (menuIndex == 3) appState = STATE_BOTOES;
        lastInputTime = now;
      }
      showMainMenu();
      break;

    case STATE_LINEFOLLOWER_MENU:
      if ((dpad & DPAD_DOWN) && now - lastInputTime > inputDelay) { lfMenuIndex = (lfMenuIndex + 1) % lfMenuItemCount; lastInputTime = now; }
      if ((dpad & DPAD_UP) && now - lastInputTime > inputDelay) { lfMenuIndex = (lfMenuIndex - 1 + lfMenuItemCount) % lfMenuItemCount; lastInputTime = now; }

      if ((misc & MISC_BUTTON_START) && now - lastInputTime > inputDelay) {
        if (lfMenuIndex == 0) lineFollowerActive = !lineFollowerActive;
        else if (lfMenuIndex == 1) startCalibration();
        lastInputTime = now;
      }

      if (lfMenuIndex >= 2) {
        if (ctl->l1() && now - lastInputTime > inputDelay) {
          if (lfMenuIndex == 2) Kp = max(0.0f, Kp - 0.01f);
          else if (lfMenuIndex == 3) Kd = max(0.0f, Kd - 0.01f);
          else if (lfMenuIndex == 4) threshold = max(0, threshold - 10);
          lastInputTime = now;
        }
        if (ctl->r1() && now - lastInputTime > inputDelay) {
          if (lfMenuIndex == 2) Kp += 0.01f;
          else if (lfMenuIndex == 3) Kd += 0.01f;
          else if (lfMenuIndex == 4) threshold += 10;
          lastInputTime = now;
        }
      }

      if (misc & MISC_BUTTON_HOME) appState = STATE_MENU;
      else showLineFollowerMenu();
      break;

    case STATE_EIXOS:
      if (misc & MISC_BUTTON_HOME) appState = STATE_MENU;
      else showControllerInfo(ctl);
      break;

    case STATE_BOTOES:
      if (misc & MISC_BUTTON_HOME) appState = STATE_MENU;
      else showButtonsInfo(ctl);
      break;

    case STATE_SPLASH:
      showSplashScreen();
      break;
  }

  if ((appState == STATE_MENU || appState == STATE_EIXOS || appState == STATE_BOTOES) && controllerModeActive) {
    if (!lineFollowerActive) controlMotors(ctl);
  }
}

void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      myControllers[i] = ctl;
      Serial.println(ctl->getModelName());
      appState = STATE_MENU;
      showMainMenu();
      return;
    }
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      myControllers[i] = nullptr;
      break;
    }
  }
  appState = STATE_SPLASH;
  showSplashScreen();
}