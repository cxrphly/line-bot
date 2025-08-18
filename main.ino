#include "Config.h"
#include "OLED_UI.h"
#include "Motors.h"
#include "LineFollower.h"
#include "Gamepad.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

ControllerPtr myControllers[BP32_MAX_GAMEPADS] = {nullptr};

int maxSpeed = 200;
float Kp = 0.05f;
float Kd = 0.25f;
int lastError = 0;
int threshold = 700;
float filteredError = 0;

AppState appState = STATE_SPLASH;

int menuIndex = 0;
bool lineFollowerActive = false;
bool controllerModeActive = false;

const int menuItemCount = 4;
const char* menuItems[] = {
  "@Line Follower",
  "$Manual",
  "@Ver Eixos",
  "@Ver Botoes"
  };

int lfMenuIndex = 0;
const int lfMenuItemCount = 5;
const char* lfMenuItems[] = {
  "$Ativar",
  "#Calibrar",
  "*Kp",
  "*Kd",
  "*Threshold"
  };

unsigned long lastDebug = 0;
const unsigned long debugInterval = 100;
unsigned long lastInputTime = 0;
const unsigned long inputDelay = 200;

void setup() {
  Serial.begin(115200);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  ledcSetup(CHANNEL_A, PWM_FREQ, PWM_RES_BITS);
  ledcAttachPin(PWMA, CHANNEL_A);
  ledcSetup(CHANNEL_B, PWM_FREQ, PWM_RES_BITS);
  ledcAttachPin(PWMB, CHANNEL_B);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) while(true);
  display.clearDisplay();
  display.display();

  setupQTR();

  BP32.setup(&onConnectedController, &onDisconnectedController);
}

void loop() {
  BP32.update();
  processCalibration();

  bool controleConectado = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] && myControllers[i]->isConnected()) {
      processGamepad(myControllers[i]);
      controleConectado = true;
      break;
    }
  }

  if (!controleConectado) showSplashScreen();

  if (lineFollowerActive) lineFollowerControl();
}