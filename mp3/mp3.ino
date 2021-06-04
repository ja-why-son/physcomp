#include <PushButton.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_LIS3DH.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LIS3DH_CLK 18
#define LIS3DH_MISO 32
#define LIS3DH_MOSI 19
#define LIS3DH_CS 14

const int BUTTON_PIN = 21;

// Accelerometer object
Adafruit_LIS3DH lis(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
PushButton _button(BUTTON_PIN);
BluetoothSerial SerialBT;

boolean connected;
boolean gravity = true;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  _button.setActiveLogic(LOW);
  
  if (!SerialBT.begin("ESP32test")) {
    Serial.println("We failed to start Bluetooth");
  }

  SerialBT.setPin("1234");
  Serial.println("Bluetooth started");
  while (!SerialBT.isReady()) {
    Serial.println("Waiting for bluetooth...");
    vTaskDelay(1000);
  }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  Serial.println("LIS3DH test!");
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1) yield();
  }
  Serial.println("LIS3DH found!");

  _display.setRotation(2);
  _display.clearDisplay();         
}

void loop() {
  // On each loop, we'll want to clear the display so we're not writing over
  // previously drawn data

  // received via serial 
  String received = "";

  if (SerialBT.available()) {
    received = SerialBT.readStringUntil('\n');
  } else if (Serial.available()) {
    received = Serial.readStringUntil('\n');
  }
  if (received == "start") {
    connected = true;
  } else if (received == "end") {
    connected = false;
  }

  // accelerometer
  sensors_event_t event;
  lis.getEvent(&event);
  float acceleration_square_sum = pow(event.acceleration.x, 2) 
                              + pow(event.acceleration.y, 2) 
                              + pow(event.acceleration.z, 2);
  
  float acc = sqrt(acceleration_square_sum);
  Serial.print(event.acceleration.x);
  Serial.print(",");
  Serial.print(event.acceleration.y);
  Serial.print(",");
  Serial.println(event.acceleration.z);
  
  SerialBT.print(event.acceleration.x);
  SerialBT.print(",");
  SerialBT.print(event.acceleration.y);
  SerialBT.print(",");
  SerialBT.println(event.acceleration.z);

  _button.update();

  if (_button.isClicked()) {
    Serial.println("buttonPressed");
    SerialBT.println("buttonPressed");
  }
  
  oledDisplayMessage();

  delay(50);
}

void oledDisplayMessage() {
  _display.clearDisplay(); 
  
  int16_t x, y;
  uint16_t textWidth, textHeight;
  char strHello[] = "Hello Makers!";

  // Setup text rendering parameters
  _display.setTextSize(1);
  _display.setTextColor(WHITE, BLACK);

  // Measure the text with those parameters
  _display.getTextBounds(strHello, 0, 0, &x, &y, &textWidth, &textHeight);

  // Center the text on the display
  _display.setCursor(_display.width() / 2 - textWidth / 2, _display.height() / 2 - textHeight / 2);

  // Print out the string
  if (connected) {
    _display.print("Serial connected");
  } else {
    _display.print("Waiting to be connected to serial...");
  }


  // Render the graphics buffer to screen
  _display.display();
}
