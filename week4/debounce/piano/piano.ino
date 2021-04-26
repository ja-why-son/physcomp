#include <PushButton.h>

#define KEY_C 262  // 261.6256 Hz (middle C)
#define KEY_D 294  // 293.6648 Hz
#define KEY_E 330  // 329.6276 Hz
#define KEY_F 350  // 349.2282 Hz
#define KEY_G 392  // 391.9954 Hz
#define KEY_A 440
#define KEY_B 493
#define KEY_CC 523

const int INPUT_BUTTON_C_PIN = 2;
const int INPUT_BUTTON_D_PIN = 3;
const int INPUT_BUTTON_E_PIN = 4;
const int INPUT_BUTTON_F_PIN = 5;
const int INPUT_BUTTON_G_PIN = 6;
const int INPUT_BUTTON_A_PIN = 7;
const int INPUT_BUTTON_B_PIN = 8;
const int INPUT_BUTTON_CC_PIN = 9;

const int OUTPUT_PIEZO_PIN = 10;
const int OUTPUT_LED_PIN = LED_BUILTIN; // visual feedback on button press

const boolean _buttonsAreActiveLow = true;

PushButton cButton(INPUT_BUTTON_C_PIN);
PushButton dButton(INPUT_BUTTON_D_PIN);
PushButton eButton(INPUT_BUTTON_E_PIN);
PushButton fButton(INPUT_BUTTON_F_PIN);
PushButton gButton(INPUT_BUTTON_G_PIN);
PushButton aButton(INPUT_BUTTON_A_PIN);
PushButton bButton(INPUT_BUTTON_B_PIN);
PushButton ccButton(INPUT_BUTTON_CC_PIN);

void setup() {
  // put your setup code here, to run once:
  pinMode(INPUT_BUTTON_C_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_D_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_E_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_F_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_G_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(INPUT_BUTTON_CC_PIN, INPUT_PULLUP);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(OUTPUT_LED_PIN, OUTPUT);

  cButton.setActiveLogic(LOW);
  dButton.setActiveLogic(LOW);
  eButton.setActiveLogic(LOW);
  fButton.setActiveLogic(LOW);
  gButton.setActiveLogic(LOW);
  aButton.setActiveLogic(LOW);
  bButton.setActiveLogic(LOW);
  ccButton.setActiveLogic(LOW);
}

void loop() {
  cButton.update();
  dButton.update();
  eButton.update();
  fButton.update();
  gButton.update();
  aButton.update();
  bButton.update();
  ccButton.update();
  

  if(cButton.isActive()){
    tone(OUTPUT_PIEZO_PIN, KEY_C);
  }else if(dButton.isActive()){
    tone(OUTPUT_PIEZO_PIN, KEY_D);
  }else if(eButton.isActive()){
    tone(OUTPUT_PIEZO_PIN, KEY_E);
  }else if(fButton.isActive()){
    tone(OUTPUT_PIEZO_PIN, KEY_F);
  }else if(gButton.isActive()){
    tone(OUTPUT_PIEZO_PIN, KEY_G);
  }else if(aButton.isActive()){
    tone(OUTPUT_PIEZO_PIN, KEY_A);
  }else if(bButton.isActive()){
    tone(OUTPUT_PIEZO_PIN, KEY_B);
  }else if(ccButton.isActive()){
    tone(OUTPUT_PIEZO_PIN, KEY_CC);    
  }else{
    noTone(OUTPUT_PIEZO_PIN); // turn off the waveform
    digitalWrite(OUTPUT_LED_PIN, LOW);
  }

}

boolean isButtonPressed(int btnPin){
  int btnVal = digitalRead(btnPin);
  if(_buttonsAreActiveLow && btnVal == LOW){
    // button is hooked up with pull-up resistor
    // and is in a pressed state
    digitalWrite(OUTPUT_LED_PIN, HIGH);
    return true;
  }else if(!_buttonsAreActiveLow && btnVal == HIGH){
    // button is hooked up with a pull-down resistor
    // and is in a pressed state
    digitalWrite(OUTPUT_LED_PIN, HIGH);
    return true;
  }

  // button is not pressed
  return false;
}
