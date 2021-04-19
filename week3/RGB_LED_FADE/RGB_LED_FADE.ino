enum RGB {
  RED,
  GREEN,
  BLUE,
  NUM_COLORS
};

const int RGB_RED_PIN = 6;
const int RGB_GREEN_PIN = 5;
const int RGB_BLUE_PIN = 3;
const int DELAY_MS = 20;
const int FADE_STEP = 3;
const int MAX_COLOR_VALUE = 255;

int _rgbLedValues[] = {255, 0, 0};
enum RGB _curFadingUp = GREEN;
enum RGB _curFadingDown = RED;

void setup() {
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Red, Green, Blue");

  setColor(_rgbLedValues[RED], _rgbLedValues[GREEN], _rgbLedValues[BLUE]);
  delay(DELAY_MS);

}

void loop() {

  _rgbLedValues[_curFadingUp] += FADE_STEP;
  _rgbLedValues[_curFadingDown] -= FADE_STEP;

  if (_rgbLedValues[_curFadingUp] > MAX_COLOR_VALUE) {
    _rgbLedValues[_curFadingUp] = MAX_COLOR_VALUE;
    _curFadingUp = (RGB)((int)_curFadingUp + 1);

    if (_curFadingUp > (int)BLUE) {
      _curFadingUp = RED;
    }
  }

  if (_rgbLedValues[_curFadingDown] < 0) {
    _rgbLedValues[_curFadingDown] = 0;
    _curFadingDown = (RGB)((int)_curFadingDown + 1);

    if (_curFadingDown > (int)BLUE) {
      _curFadingDown = RED;
    }
  }

  setColor(_rgbLedValues[RED], _rgbLedValues[GREEN], _rgbLedValues[BLUE]);
  delay(DELAY_MS);

}

void setColor(int red, int green, int blue)
{
  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.println(blue);

//  // If a common anode LED, invert values
//  if(COMMON_ANODE == true){
//    red = MAX_COLOR_VALUE - red;
//    green = MAX_COLOR_VALUE - green;
//    blue = MAX_COLOR_VALUE - blue;
//  }
  analogWrite(RGB_RED_PIN, red);
  analogWrite(RGB_GREEN_PIN, green);
  analogWrite(RGB_BLUE_PIN, blue);  
}
