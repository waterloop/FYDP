//Will power the winding motor for 2 revolutions

#include "Arduino.h"
#include "qdec.h"

const int ROTARY_PIN_A = 3; // the first pin connected to the rotary encoder
const int ROTARY_PIN_B = 2; // the second pin connected to the rotary encoder
const int motor2pin1 = 5;//motor pin 1
const int motor2pin2 = 4;//motor pin 2
const int ENA_pin = 9;//motor speed control pin
::SimpleHacks::QDecoder qdec(ROTARY_PIN_A, ROTARY_PIN_B, true); // the library class...

// Stores a relative value based on the clockwise / counterclockwise events
volatile int rotaryCount = 0;
// Used in loop() function to track the value of rotaryCount from the
// prior time that loop() executed (to detect changes in the value)
int lastLoopDisplayedRotaryCount = 0;

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

// The only change to the setup() routine is the two lines
// that attach the interrupt for the two pins to the ISR.
void setup() {
  delay(2000);
  
  pinMode(ENA_pin, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  
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
  //turn on the motor
  analogWrite(ENA_pin, 400);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
}

void DoFakeTask(void) {
  // Simulate work that takes significant processing time
  const int EVENTSPERREV = 1200;
  if (rotaryCount > 2*EVENTSPERREV) {
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
  }
  unsigned long millisecondsToDelay = 0;
  //millisecondsToDelay = 25;
  //millisecondsToDelay = 1000;
  delay(millisecondsToDelay);
}

void loop() {
  DoFakeTask();

  // Because we are not getting the events from the library directly,
  // we have to detect the change in value of the rotaryCount variable.
  // Because an interrupt could change the value of rotaryCount at any
  // time, read its value into a local variable, which won't be affected
  // even if another ISR fires and changes rotaryCount.
  int newValue = rotaryCount;
  if (newValue != lastLoopDisplayedRotaryCount) {
    // Also get the difference since the last loop() execution
    int difference = newValue - lastLoopDisplayedRotaryCount;
    
    // act on the change: e.g., modify PWM to be faster/slower, etc.
    lastLoopDisplayedRotaryCount = newValue;
    Serial.print("Change: "); Serial.print(newValue);
    Serial.print(" ("); Serial.print(difference); Serial.println(")");
  }
}
