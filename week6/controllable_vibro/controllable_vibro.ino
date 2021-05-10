const int VIBROMOTOR_OUTPUT_PIN = 3;
const int POTENTIOMETER_INPUT_PIN = A0;

void setup() {
  pinMode(VIBROMOTOR_OUTPUT_PIN, OUTPUT);
  pinMode(POTENTIOMETER_INPUT_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  int strength = analogRead(POTENTIOMETER_INPUT_PIN);
  int mappedStrength = map(strength, 0, 1024, 0, 255);
  Serial.println(strength);
  analogWrite(VIBROMOTOR_OUTPUT_PIN, mappedStrength);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  analogWrite(VIBROMOTOR_OUTPUT_PIN, 0);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
