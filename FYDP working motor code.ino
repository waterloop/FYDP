
int motor2pin1 = 5;
int motor2pin2 = 4;
int ENA_pin = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(ENA_pin, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:   
  analogWrite(ENA_pin, 200);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  
}
