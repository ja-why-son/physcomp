const int LED_OUTPUT_PIN = 3;
const int MAX_ANALOG_OUT = 255;
const int DELAY_MS = 5;

int _currentBrightness = 0;
int _fadeAmount = 1;
unsigned long previousMillis;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_OUTPUT_PIN, OUTPUT);
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= DELAY_MS) {
    previousMillis = currentMillis;
    analogWrite(LED_OUTPUT_PIN, _currentBrightness);
    _currentBrightness += _fadeAmount;

    if (_currentBrightness >= MAX_ANALOG_OUT || _currentBrightness <= 0) {
      _fadeAmount = - _fadeAmount;
    }

  }
  
}
