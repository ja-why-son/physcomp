void setup()
{
  pinMode(3, OUTPUT);
  Serial.begin(9600); // for printing values to console
}

void loop()
{
  int potVal = analogRead(A0); // returns 0 - 1023 (due to 10 bit ADC)
  Serial.println(potVal);      // print value to Serial
  delay(50);                   // Reading new values at ~20Hz

  analogWrite(3, 255 * ((float)potVal / 1025));
}
