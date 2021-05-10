#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Create the display object
Adafruit_SSD1306 _display(128, 64, &Wire, 4);

// Ball global variables
const int _ballRadius = 5;
const int _squareWidth = 5;
int _xBall = 0;  // x location of the ball
int _yBall = 0;  // y location of the ball
int _xSpeed = 0; // x speed of ball (in pixels per frame)
int _ySpeed = 0; // y speed of ball (in pixels per frame)

int _xSquare = 0;
int _ySquare = 0;


void setup() {
  // Initialize the display
  _display.begin(SSD1306_SWITCHCAPVCC, 0x3D);

  _xSquare = _display.width() / 2;
  _ySquare = _display.height() / 2;

  // Gets a random long between min and max - 1
  // https://www.arduino.cc/reference/en/language/functions/random-numbers/random/
  _xSpeed = random(1, 4);
  _ySpeed = random(1, 4);
}

void loop() {
  // Clear the display
  _display.clearDisplay();
  
  // Update ball based on speed location
  _xSquare += _xSpeed;
  _ySquare += _ySpeed;

  // Check for ball bounce. First check for going off left or right side of screen
  if(_xSquare <= 0 || _xSquare + _squareWidth >= _display.width()){
    _xSpeed = _xSpeed * -1; // reverse x direction
  }
  
  // Now check for bouncing on floor or ceiling
  if(_ySquare <= 0 || _ySquare + _squareWidth >= _display.height()){
    _ySpeed = _ySpeed * -1; // reverse y direction
  }

  // Draw circle
//  _display.drawCircle(_xBall, _yBall, _ballRadius, SSD1306_WHITE);
  _display.drawRect(_xSquare, _ySquare, _squareWidth, _squareWidth, SSD1306_WHITE);
  
  // Render buffer to screen
  _display.display();
}
