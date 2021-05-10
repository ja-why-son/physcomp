const int VIBROMOTOR_OUTPUT_PIN = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(VIBROMOTOR_OUTPUT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(VIBROMOTOR_OUTPUT_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(VIBROMOTOR_OUTPUT_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
