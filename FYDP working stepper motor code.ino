// Include the Arduino Stepper Library
#include <Stepper.h>

// Number of steps per output rotation
const int stepsPerRevolution = 200;

// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
int numIteration = 1;

void setup()
{
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
	// set the speed at 60 rpm:
	myStepper.setSpeed(60);
	// initialize the serial port:
	Serial.begin(9600);
}

void loop() 
{
  while (numIteration > 0)
  {
	// step one revolution in one direction:
	Serial.println("clockwise");
	myStepper.step(3*stepsPerRevolution);
	delay(500);

	// step one revolution in the other direction:
	Serial.println("counterclockwise");
	myStepper.step(-3*stepsPerRevolution);
	delay(500);
  numIteration--;
  }  
}