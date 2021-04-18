void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(3, HIGH);
  Serial.println("Pin 3 is HIGH (5V)");
  delay(1000);
  digitalWrite(3, LOW);
  Serial.println("Pin 3 is LOW (0v)");
  delay(1000);
}
