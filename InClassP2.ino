//int pwmPin = 9; // output pin supporting PWM
//int inPin = 0; // voltage connected to analog pin 3, e.g. a potentiometer
int val = 0; // variable to store the read value
float volt = 0; // variable to hold the voltage read

void setup() {
  pinMode(2, OUTPUT); // sets the pin as output
  Serial.begin(9600);
}

void loop() {
  val = analogRead(A0); // read the input pin
  volt = (5.0 * val) / 1023;
  Serial.println(volt);
  val = 255 * (volt / 5);
  analogWrite(2, val);
}
