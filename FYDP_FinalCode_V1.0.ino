/*
Mathew Hanson Oct 8 2022
Version 1.0
Take in number of desired loops and run the motor while the encoder reads
less than number of loops. While this is happening run stepper motor down 5
revolutions up 5 while continuously checking the encoder value
*/
#include "Arduino.h"
#include "qdec.h"//include encoder library
#include <Stepper.h>//include the stepper library
#include <SoftwareSerial.h>//include serial interface library


const int ROTARY_PIN_A = 3;//the first pin connected to the encoder (Brown)
const int ROTARY_PIN_B = 2;//the second pin connected to the encoder (Yellow)
const int MOTOR_PIN1 = 5;//First Pin connected to the motor (Red)
const int MOTOR_PIN2 = 4;//Second Pin connected to the motor (Grey)
const int ENA_PIN = 6;//ENA_PIN controls the speed of the winding motor (Green)
const int ENC_PER_REVOLUTION = 1200;
const int STEPS_PER_REVOLUTION = 200;//steps per revolution 360/1.8
const int NUM_LOOP = 5;//change this value for different # of loops
Stepper myStepper(STEPS_PER_REVOLUTION, 8, 9, 10, 11);
//Pins 8 & 9 are one phase of the stepper, 10 & 11 are the other phase
//8 (Blue) 9 (White) 10 (Red) 11(Grey)
::SimpleHacks::QDecoder qdec(ROTARY_PIN_A, ROTARY_PIN_B, true);//uses encoder wired to 2 & 3 for interrupt feature

// Stores a relative value based on the clockwise / counterclockwise events
volatile int rotaryCount = 0;
// Used in loop() function to track the value of rotaryCount from the
// prior time that loop() executed (to detect changes in the value)
//int lastLoopDisplayedRotaryCount = 0;

// This is the ISR (interrupt service routine).
// This ISR fires when the pins connected to the rotary encoder change
// state.  Therefore, the update() function is called to update the
// decoder state and then (if an event was indicated), the rotaryCount
// variable is updated.
// CAUTION: some Arduion functions will not work if called from an ISR.
void IsrForQDEC(void) { // do absolute minimum possible in any ISR ...
  // placing the `using namespace` line into a function limits
  // allows accessing the types and functions of that namespace,
  // but only while in this function.
  using namespace ::SimpleHacks;
  QDECODER_EVENT event = qdec.update();
  if (event & QDECODER_EVENT_CW) {
    rotaryCount = rotaryCount + 1;
  } else if (event & QDECODER_EVENT_CCW) {
    rotaryCount = rotaryCount - 1;
  }
  return;
}
//Create a function that determines when the motor should stop
bool EnoughLoops() {
  if(rotaryCount > NUM_LOOP * ENC_PER_REVOLUTION) {
    digitalWrite(MOTOR_PIN1, LOW);
    digitalWrite(MOTOR_PIN2, LOW);
    analogWrite(ENA_PIN, 0);
    return false;
  }
  int CurrentLoop = rotaryCount/NUM_LOOP;
  Serial.print("Current Loops: "); Serial.print(CurrentLoop);
  return true;
}



void setup() {
  delay(2000);
  //setup winding motor pins:
  pinMode(ENA_PIN, OUTPUT);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  //setup stepper motor pins:
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  //set stepper speed:
  myStepper.setSpeed(13.3636);//speed may cause slipping*
  //initialize serial port
  Serial.begin(115200);//remember to change serial port to 115200 Baud
  while (!Serial) {} // wait for serial port to connect... needed for boards with native USB serial support
  Serial.print("Beginning QDecoder Sketch ");  
  Serial.println(__FILE__); // becomes the sketch's filename during compilation

  // initialize the rotary encoder
  qdec.begin();

  // attach an interrupts to each pin, firing on any change in the pin state
  // no more polling for state required!
  attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_A), IsrForQDEC, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_B), IsrForQDEC, CHANGE);
}

void loop() {
  //read user input:
  if(Serial.available() > 0) {
    String valInput = Serial.readString();
    switch(valInput[0]) {
      case 'r':
        //1/4 Events per rev is from motor datasheet
        //const int EVENTSPERREV = 1195
        rotaryCount = 0;//reset Encoder
        //turn motor on:
        digitalWrite(MOTOR_PIN1, HIGH);
        digitalWrite(MOTOR_PIN2, LOW);
        analogWrite(ENA_PIN, 400);//experiment with this speed***
        while (EnoughLoops()) {
          //loop while the winding motor still needs to turn
          int direction = 0;
          for(int count = 1; count <=80; count++) {
            if (count <= 40) {
              direction = -1;//go down first 5 revolutions
            }
            else {
              direction = 1;//go up last five revolutions
            }
            myStepper.step(direction * STEPS_PER_REVOLUTION / 8);
            /*check encoder ever 1/8 revolution--> can be shortened 
            by adjusting count and step value;0.561 second intervals @13.36rpm
            = +0.786 loop error @ 84rpm*/ 
            if (EnoughLoops()) {
              break;
            }
            }
          }
      break;
      case 'u':
        myStepper.step(STEPS_PER_REVOLUTION);
      break;
      case 'd':
        myStepper.step(-STEPS_PER_REVOLUTION);
      break;
      default:
      break;
    }
  }
}
