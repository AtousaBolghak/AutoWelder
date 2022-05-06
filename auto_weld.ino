
/*
  Automatic Welder - one revolution

  Created 4 April. 2022

  by Atousa Bolghak

*/

#include <Stepper.h>
#include <Servo.h>

int weld_count = 0;
const int max_weld = 10;
const int greenLed = 4;
const int redLed = 5;
const int buttonPin = 2;
const int servoPin = 12;
const int buzzer = 3; //buzzer to arduino pin 3
volatile int state = LOW;
int servoPos = 0; // variable to store the servo position

// change this to fit the number of steps per revolution
// for your motor
const int stepsPerRevolution = 200;

// create servo object to control a servo
Servo myservo;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);

  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), toggle, LOW);
  pinMode(buzzer, OUTPUT); // Set buzzer
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);

//  Serial.begin(9600); // DEBUG: Will not work while interrupt is enabled
}

void loop() {
  digitalWrite(greenLed, HIGH);

  while (state && weld_count < max_weld) {
    tone(buzzer, 1250); // Send 1.2KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(250);        // ...for 0.25 sec

    // Trigger weld:
    servoPos = 75;
    myservo.write(servoPos);
    delay(3000);

    servoPos = 0;
    myservo.write(servoPos);

    tone(buzzer, 1200); // Send 1.2KHz sound signal...
    digitalWrite(greenLed, LOW);
    delay(250);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    digitalWrite(greenLed, HIGH);
    delay(250);        // ...for 1sec
    myStepper.step(stepsPerRevolution);
    weld_count++;
    delay(500);
  }
  if (weld_count >= max_weld){
    state = LOW;
    digitalWrite(redLed, state);
    weld_count = 0;
  }
}

void toggle() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    state = !state;
    digitalWrite(redLed, state);
  }
  last_interrupt_time = interrupt_time;
}
