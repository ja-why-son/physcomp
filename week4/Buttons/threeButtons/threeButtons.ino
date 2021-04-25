const int input1 = 13;
const int input2 = 12;
const int input3 = 11;

const int output1 = 5;
const int output2 = 4;
const int output3 = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(input1, INPUT); // pulldown
  pinMode(input2, INPUT_PULLUP); // internal pullup
  pinMode(input3, INPUT); // pullup
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int button1Value = digitalRead(input1);
  int button2Value = digitalRead(input2);
  int button3Value = digitalRead(input3);

  // reverse bc pullup resistor
  button2Value = !button2Value;
  button3Value = !button3Value;

  digitalWrite(output1, button1Value);
  digitalWrite(output2, button2Value);
  digitalWrite(output3, button3Value);

  delay(5);
}
