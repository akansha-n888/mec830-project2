
/* Robot contains 3 caster wheels, 1 fixed wheel, 1 DC motor and 1 servo.
The following program is the attributed tot Task 1 (manual control)*/

//servo parameters
#include <Servo.h>
Servo myservo;

//DC motor paramters
#define ENABLE 5
#define DIRA 3
#define DIRB 4

void setup() {
   myservo.attach(7);
   myservo.write(90);

  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  Serial.begin(9600);

}

void loop() {
  //needs to include remote commands to set conditions for the servo and DC motor

  // move servo 90 deg cw for right turn
  myservo.write(0);
  //move servo 90 deg ccw for left turn
  myservo.write(180);

  digitalWrite(ENABLE,HIGH); // enable on
  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
  delay(500);
  
  digitalWrite(DIRA,LOW);  //reverse
  digitalWrite(DIRB,HIGH);
  delay(500);

}
