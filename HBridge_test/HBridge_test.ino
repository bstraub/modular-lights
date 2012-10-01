#define PIN_PWM 9
#define PIN_EN1 10
#define PIN_EN2 11

void setup()
{
  pinMode(PIN_PWM, OUTPUT);
  pinMode(PIN_EN1, OUTPUT);
  pinMode(PIN_EN2, OUTPUT);
  digitalWrite(PIN_PWM, LOW);
  digitalWrite(PIN_EN1, LOW);
  digitalWrite(PIN_EN2, LOW);
  
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
}

void loop()
{
  analogWrite(PIN_PWM, map(analogRead(A0),0,1023,0,255));
  delay(5);
  digitalWrite(PIN_EN1, HIGH);
  delay(5000);
  digitalWrite(PIN_EN1, LOW);
  digitalWrite(PIN_EN2, HIGH);
  delay(5000);
  digitalWrite(PIN_EN1, HIGH);
  delay(5000);
  digitalWrite(PIN_EN1, LOW);
  digitalWrite(PIN_EN2, LOW);
  delay(4995);
}
