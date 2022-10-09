// Include the Arduino Stepper Library
#include <Stepper.h>
#include <SoftwareSerial.h>
// Number of steps per output rotation
const int stepsPerRevolution = 200;

// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

bool meme1;
bool meme2;
bool meme3;

void setup()
{
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
	// set the speed at 60 rpm:
	myStepper.setSpeed(13);
	// initialize the serial port:
	Serial.begin(9600);
}

void loop() 
{
  /*if(Serial.available() > 0) {
    String valInput = Serial.readString();
    switch (valInput[0]) {
      case 't':
      Serial.print("oof");
      break;
      case 'u':
        //meme1 = true;
	      while(true) {
        myStepper.step(stepsPerRevolution);
        }
	      break;
        case 'd':
        while(valInput[0] == 'd'){
        myStepper.step(-stepsPerRevolution);
        break;
        }
        /*meme1 = 0;
        meme2 = 1;
        meme3 = 0;*/
        //break;
        /*case 'r':
        myStepper.step(-5*stepsPerRevolution);
        myStepper.step(5*stepsPerRevolution);
        break;*/
    
	// step one revolution in one direction:
  	

	// step one revolution in the other direction:
	/*Serial.println("counterclockwise");
	myStepper.step(-3*stepsPerRevolution);
	delay(500);*/
  while (true) {
    myStepper.step(3*stepsPerRevolution);
    myStepper.step(-stepsPerRevolution*3);
  }
  
  }  
