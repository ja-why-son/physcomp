const int VIBROMOTOR_OUTPUT_PIN = 3;
const int POTENTIOMETER_INPUT_PIN = A0;

int _step = 20;

int _strength = 100;

void setup() {
  pinMode(VIBROMOTOR_OUTPUT_PIN, OUTPUT);
  pinMode(POTENTIOMETER_INPUT_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
//  int strength = analogRead(POTENTIOMETER_INPUT_PIN);
  int mappedStrength = map(_strength, 0, 1024, 0, 255);
  Serial.println(_strength);
  analogWrite(VIBROMOTOR_OUTPUT_PIN, mappedStrength);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(40);
  analogWrite(VIBROMOTOR_OUTPUT_PIN, 0);
  digitalWrite(LED_BUILTIN, LOW);
  delay(40);
  _strength += _step;
  if (_strength > 500 || _strength < 100) {
    _step = -_step;
  }
}
