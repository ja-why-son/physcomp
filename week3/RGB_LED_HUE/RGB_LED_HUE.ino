#include "src/RGBConverter/RGBConverter.h"

const boolean COMMON_ANODE = false;
const int RGB_RED_PIN = 6;
const int RGB_GREEN_PIN  = 5;
const int RGB_BLUE_PIN  = 3;
const int DELAY_INTERVAL = 30; 
const byte MAX_RGB_VALUE = 255;

float _hue = 0; //hue varies between 0 - 1
float _step = 0.01f;
float _saturation = 1;
float _saturationStep = 0.05f;



void setup() {
  // put your setup code here, to run once:
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte rgb[3];
  RGBConverter _rgbConverter;
  _rgbConverter.hslToRgb(_hue, _saturation, 0.5, rgb);

  setColor(rgb[0], rgb[1], rgb[2]);

  _hue += _step;
  _saturation -= _saturationStep;

  if (_hue > 1.0) {
    _hue = 0;
  }

  if (_saturation < 0.4 | _saturation >= 1) {
    _saturationStep = -_saturationStep;
  }

  delay(DELAY_INTERVAL);
}

void setColor(int red, int green, int blue)
{
  // If a common anode LED, invert values
  if(COMMON_ANODE == true){
    red = MAX_RGB_VALUE - red;
    green = MAX_RGB_VALUE - green;
    blue = MAX_RGB_VALUE - blue;
  }
  analogWrite(RGB_RED_PIN, red);
  analogWrite(RGB_GREEN_PIN, green);
  analogWrite(RGB_BLUE_PIN, blue);  
}
