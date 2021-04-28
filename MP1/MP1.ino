#include <PushButton.h>
#include "src/RGBConverter/RGBConverter.h"


// pin configuration
const int MODE_BUTTON_PIN = 2;
const int LIGHT_SENSOR_PIN = A0;
const int VOICE_SENSOR_PIN = A1;
const int LOFI_INPUT_PIN = 10;
const int R_PIN = 3;
const int G_PIN = 5;
const int B_PIN = 6;

// constants
const int MAX_ANALOG_IN = 676;
const int MAX_ANALOG_OUT = 255;
const int SOUND_THRESHOLD = 25;
const float HUE_STEP = 0.01f;
const int DELAY_INTERVAL = 50;

const int MIN_PHOTOCELL_VAL = 550;
const int MAX_PHOTOCELL_VAL = 800;
const boolean PHOTOCELL_IS_R2_IN_VOLTAGE_DIVIDER = true; // set false if photocell is R1  

int _mode = 0;


class RGBLight {
 private:
  const int r_pin;
  const int g_pin;
  const int b_pin;
  byte _rgb[3] = {0, 0, 0};
  RGBConverter _rgbConverter;
  unsigned long previousTime;

  void setColor() {
    analogWrite(r_pin, _rgb[0]);
    analogWrite(g_pin, _rgb[1]);
    analogWrite(b_pin, _rgb[2]);
  }

 public:

  RGBLight(int rPin, int gPin, int bPin) :
   r_pin(rPin), g_pin(gPin), b_pin(bPin)
   {
    pinMode(r_pin, OUTPUT);
    pinMode(g_pin, OUTPUT);
    pinMode(b_pin, OUTPUT);
    previousTime = millis();
   }

   void setRGB(int R, int G, int B) {
    _rgb[0] = R;
    _rgb[1] = G;
    _rgb[2] = B;
    setColor();
   }

   void changeColor() {
    unsigned long currentTime = millis();
    if (currentTime - previousTime < DELAY_INTERVAL) {
      return;
    }
    double hsl[3];
    _rgbConverter.rgbToHsl(_rgb[0], _rgb[1], _rgb[2], hsl);
    Serial.println(hsl[0]);
    hsl[0] += HUE_STEP;
    if (hsl[0] > 1.0) {
      hsl[0] = 0;
    }
    byte temp[3];
    _rgbConverter.hslToRgb(hsl[0], 1, 0.5, _rgb);
    setColor();
    previousTime = millis();
   }

   void changeStatus() {
    int redPace = -5;
    int greenPace = -2;
    if (_rgb[0] == 0 && _rgb[1] == 0 && _rgb[2] == 0) {
      redPace = -redPace;
      greenPace = -greenPace;
    }
    
    for (int i = 0; i < 50; i++) {
      _rgb[0] += redPace;
      _rgb[1] += greenPace;
      _rgb[2] = 0;
      setColor();
      delay(30);
    }
   }

};

PushButton _modeButton(MODE_BUTTON_PIN);
RGBLight _rgbLight(R_PIN, G_PIN, B_PIN);

void setup() {
  Serial.begin(9600);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LOFI_INPUT_PIN, INPUT);
  _modeButton.setActiveLogic(LOW);

}

void loop() {

  _modeButton.update();
  if (_modeButton.isClicked()) {
    _mode = (_mode + 1) % 4;
    if (_mode == 3 || _mode == 0) {
      _rgbLight.setRGB(0, 0, 0);
    } else if (_mode == 2) {
      _rgbLight.setRGB(250, 100, 0);
    }
  }

  if (_mode == 1) {
    int photocellVal = analogRead(LIGHT_SENSOR_PIN);
    Serial.println(photocellVal);
    int ledVal = map(photocellVal, MIN_PHOTOCELL_VAL, MAX_PHOTOCELL_VAL, 0, MAX_ANALOG_OUT);
    ledVal = constrain(ledVal, 0, 255);
    int gVal = map(ledVal, 0, 255, 0, 80);
    _rgbLight.setRGB(ledVal, gVal, 0);
  } else if (_mode == 2) {
    if (digitalRead(LOFI_INPUT_PIN) == HIGH) {
      _rgbLight.changeColor();
    }
  } else if (_mode == 3) {
    long soundLevel = 0;
    for (int i = 0; i < 100; i++) {
      soundLevel += analogRead(VOICE_SENSOR_PIN);
    }
    soundLevel /= 100;
    Serial.println(soundLevel);
    if (soundLevel >= 360) {
      _rgbLight.changeStatus();
    }
  }
}
