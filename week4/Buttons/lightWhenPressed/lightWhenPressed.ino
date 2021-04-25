void setup() {
  pinMode(12, INPUT_PULLUP);
  pinMode(3, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonVal = digitalRead(12);

  digitalWrite(3, buttonVal);
}
