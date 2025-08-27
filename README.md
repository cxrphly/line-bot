# Robô Seguidor de Linha Multifuncional com algoritmo PID.

Projeto open-source de um robô seguidor de linha com funções adicionais,como controle via Bluetooth, modos automáticos e manuais, e suporte a sensores extras alem de contar com algoritmo PID para melhor performance no modo seguidor de linha.  
Acompanha **arquivos para impressão 3D**, **corte a laser** e **lista de peças** para montagem.

Repositório: [https://github.com/cxrphly/line-bot](https://github.com/cxrphly/line-bot)

<p align="center">
  <img src="https://github.com/cxrphly/line-bot/blob/main/design/view/linebot-chassi_only-view.png" alt="LineBot Chassi" width="45%">
  <img src="https://github.com/cxrphly/line-bot/blob/main/design/view/linebot-hardware-view.png" alt="LineBot Com Componentes " width="45%">
    <img src="https://github.com/cxrphly/line-bot/blob/main/design/view/linebot-perfil-view.png" alt="LineBot" width="45%">
  <img src="https://github.com/cxrphly/line-bot/blob/main/design/view/wheels-view.png" alt="Rodas LineBot" width="45%">
</p>


<p align="center">
  <img src="https://github.com/cxrphly/line-bot/blob/main/design/view/linebot-builded.png" alt="Projeto Montado" width="60%">
</p>

---

## Estrutura do Repositório
- **[main.ino](https://github.com/cxrphly/line-bot/blob/main/main.ino)** → Código-fonte principal para ESP32 (Arduino IDE).  
- **[/design/](https://github.com/cxrphly/line-bot/blob/main/design/)** → Arquivos PDF/SVG para corte a laser e GLB/OBJ para impressão 3D.  

---

## Funcionalidades
- Seguidor de linha com sensor QTR-8A.
- Controle manual via controle Bluetooth (Bluepad32).
- Tela OLED interativa para exibição de informações, alteração de parametros e calibração.  
- Modos de operação (manual e automático).  
- Arquitetura visando expansão dos modulos.

---

## Bibliotecas Utilizadas
- **[Bluepad32]([https://github.com/Bluepad32/Bluepad32](https://github.com/ricardoquesada/bluepad32))** — para suporte a controles Bluetooth no ESP32  
  > Créditos: Ricardo Quesada.
- **[QTRSensor]**
- **[Adafruit SSD1306]**
---

## Lista de Peças Utilizadas
- ESP32 DevKit v1  
- Driver de motor TB6612FNG
- Conversor Buck XL4005E1  
- 2x Motores N20 6V 500RPM
  > Recomendo que use motores de maior RPM para melhor desempenho.
- Sensor QTR-8RC
- Tela OLED I2C 0.96" (128x64)  
- Bateria Li-ion 2S 7.4V 3000mAh 
  >Use um BMS apropriado para evitar descarga extrema
- Breadboard para depuração e pré-montagem  
- Estrutura impressa em 3D ou cortada a laser
  > O projeto, a priori, foi pensado para usar corte a laser, mas com poucas adaptaçôes pode ser impresso em 3d.

---

## Licenças

### Código-fonte
[`LICENSE_CODE`](LICENSE_CODE)

### Arquivos de Design
[`LICENSE_DESIGN`](LICENSE_DESIGN)

---

##
## 1. Certifique-se de que possui instalada em sua máquina a **[Arduino IDE](https://www.arduino.cc/en/software/)** e as bibliotecas necessárias

- i. Adicione **Bluepad32** e **ESP32** ao **[Board Manager](https://bluepad32.readthedocs.io/en/latest/plat_arduino/)**.
- ii. Instale as boards: `ESP32 by Espressif`, `Bluepad32 by Ricardo Quesada`.
- iii. Selecione a placa em **Board -> Other boards** e busque o modelo dentro do escopo da Bluepad32.
   > exemplo: `DOIT ESP32 Dev Kit V1 - esp32_bluepad32`.
- iv. Instale as bibliotecas: `QTRSensors`, `Adafruit SSD1306`, `Adafruit GFX` e `Adafruit BusIO`.


## 2. Clone este repositório:
   ```bash
   git clone https://github.com/cxrphly/line-bot.git
   ```

## 3. Configure os pinos em **[config.h](config.h)**. conforme a sua necessidade, ou matenha padrao.
   ```cpp
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
   ```
## 4. Mapeando o Controle

1. Compile no seu ESP32 o **exemplo Controller** da biblioteca `esp32_bluepad32`.
2. Ligue seu controle Bluetooth em **modo pareamento**; o ESP32 deve se conectar automaticamente ao controle.
3. Abra o **Serial Monitor** e identifique:
   - Os inputs do seu controle (botões, gatilhos, DPAD, etc.)
   - A deadzone dos analógicos
4. Essas informações serão úteis para definir variáveis em **[config.h](config.h)**.

<p align="center">
  <img src="https://github.com/cxrphly/line-bot/blob/main/design/gamepad/gamepad-map.png" alt="8bitdo Ultimate 2C - Já mapeado" width="60%">
</p>

    
## 5. Costomizando o menu(OPCIONAL).
  **Trocando a splash screen**
  **[oled_ui.cpp](oled_ui.cpp)**
   ```cpp
   const unsigned char epd_bitmap_pixil_frame_0_1_ [] PROGMEM = {
        [...]
   };
   const int epd_bitmap_allArray_LEN = 1;
   const unsigned char* epd_bitmap_allArray[1] = {
      epd_bitmap_pixil_frame_0_1_
   };
   ```
  Altere o conteudo por qualquer bitmap(dë preferencia à 128x48) usando **[img2cpp](https://javl.github.io/image2cpp/)** ou outra ferramenta.

## 6. Sobre a Bluepad32  
  A documentacao no site oficial é limitada, recomendo ver os codigo-exemplo em File > Exemples > Bluepad32_ESP32.  
  Caminho no windows para algumas keywords:  
   ```
   C:\Users\user\AppData\Local\Arduino15\packages\esp32-bluepad32\hardware\esp32\4.1.0\libraries\Bluepad32_ESP32
   ```
- keywords.txt
```bash
#######################################
# Syntax Coloring Map For WiFiNINA
#######################################

#######################################
# Library (KEYWORD1)
#######################################

Controller KEYWORD1
Gamepad KEYWORD1
Bluepad32 KEYWORD1
BP32 KEYWORD1

#######################################
# Methods and Functions (KEYWORD2)
#######################################

# Bluepad32
firmwareVersion KEYWORD2
setDebug KEYWORD2
pinMode KEYWORD2
digitalRead KEYWORD2
digitalWrite KEYWORD2

# Gamepad
update KEYWORD2
forgetBluetoothKeys KEYWORD2
setup KEYWORD2
a KEYWORD2
b KEYWORD2
x KEYWORD2
y KEYWORD2
l1 KEYWORD2
l2 KEYWORD2
r1 KEYWORD2
r2 KEYWORD2
thumbL KEYWORD2
thumbR KEYWORD2
buttons KEYWORD2
miscButtons KEYWORD2
axisX KEYWORD2
axisY KEYWORD2
axisRX KEYWORD2
axisRY KEYWORD2
brake KEYWORD2
throttle KEYWORD2
miscSystem KEYWORD2
miscBack KEYWORD2
miscHome KEYWORD2
isConnected KEYWORD2
getModel KEYWORD2
setPlayerLEDs KEYWORD2
setColorLED KEYWORD2
setRumble KEYWORD2

#######################################
# Constants (LITERAL1)
#######################################

# Controllers
CONTROLLER_TYPE_UnknownSteamController LITERAL1
CONTROLLER_TYPE_SteamController LITERAL1
CONTROLLER_TYPE_SteamControllerV2 LITERAL1
CONTROLLER_TYPE_UnknownNonSteamController LITERAL1
CONTROLLER_TYPE_XBox360Controller LITERAL1
CONTROLLER_TYPE_XBoxOneController LITERAL1
CONTROLLER_TYPE_PS3Controller LITERAL1
CONTROLLER_TYPE_PS4Controller LITERAL1
CONTROLLER_TYPE_WiiController LITERAL1
CONTROLLER_TYPE_AppleController LITERAL1
CONTROLLER_TYPE_AndroidController LITERAL1
CONTROLLER_TYPE_SwitchProController LITERAL1
CONTROLLER_TYPE_SwitchJoyConLeft LITERAL1
CONTROLLER_TYPE_SwitchJoyConRight LITERAL1
CONTROLLER_TYPE_SwitchJoyConPair LITERAL1
CONTROLLER_TYPE_SwitchInputOnlyController LITERAL1
CONTROLLER_TYPE_MobileTouch LITERAL1
CONTROLLER_TYPE_XInputSwitchController LITERAL1
CONTROLLER_TYPE_PS5Controller LITERAL1
CONTROLLER_TYPE_iCadeController LITERAL1
CONTROLLER_TYPE_SmartTVRemoteController LITERAL1
CONTROLLER_TYPE_EightBitdoController LITERAL1
CONTROLLER_TYPE_GenericController LITERAL1
CONTROLLER_TYPE_NimbusController LITERAL1
CONTROLLER_TYPE_OUYAController LITERAL1

# DPAD
DPAD_UP LITERAL1
DPAD_DOWN LITERAL1
DPAD_RIGHT LITERAL1
DPAD_LEFT LITERAL1

# Buttons
BUTTON_A LITERAL1
BUTTON_B LITERAL1
BUTTON_X LITERAL1
BUTTON_Y LITERAL1
BUTTON_SHOULDER_L LITERAL1
BUTTON_SHOULDER_R LITERAL1
BUTTON_TRIGGER_L LITERAL1
BUTTON_TRIGGER_R LITERAL1
BUTTON_THUMB_L LITERAL1
BUTTON_THUMB_R LITERAL1

# Misc buttons
MISC_BUTTON_SYSTEM LITERAL1
MISC_BUTTON_BACK LITERAL1
MISC_BUTTON_HOME LITERAL1
 ```

---
## A Fazer
- [ ] Melhorar a calibração.
- [ ] Corrigir bug da alternância de modos.
- [ ] Melhorar do Modo Autonomo seguindo o Regulamento da ROBOCORE. 

---
## Agradeço a comunidade open-source por tornar esse projeto possivel.
