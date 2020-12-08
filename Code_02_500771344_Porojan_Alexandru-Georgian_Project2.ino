#include "IRremote.h"
#include "Servo.h"
#include "Stepper.h"
#include "SR04.h"

#define SERVO_PIN 7
#define SERVO_LEFT 0
#define SERVO_MID 70
#define SERVO_RIGHT 140
#define SERVO_INC 10
#define SERVO_TURN_DELAY 200
#define IR_DATA 4
#define STEPS 96 // Steps per revolution of stepper internal shaft; normally 32 but 96 is faster
#define QUARTER_REV -256 // Steps for a quarter revolution
#define FULL_REV -2048 // Steps for one full revolution
#define STEPPER_SPEED 500
#define ECHO_PIN 2
#define TRIG_PIN 3

int action = 0, previousAction = 0;
int servoAngle = SERVO_MID;
long distance;

IRrecv irrecv(IR_DATA);
decode_results results;
Servo myservo;
Stepper mystepper(STEPS, 8, 10, 9, 11);
SR04 ultrasonic = SR04(ECHO_PIN,TRIG_PIN);

void setup() {
  
  myservo.attach(SERVO_PIN);
  myservo.write(servoAngle);
  mystepper.setSpeed(STEPPER_SPEED);
  
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  
  if (irrecv.decode(&results)){
    switch(results.value){
      case 0xFF629D: // VOL+ button pressed
        action = 1; // Turn stepper forward
        previousAction = 1; break;

      case 0xFFA857: // VOL- button pressed
        action = 2; // Turn stepper backward
        previousAction = 2; break;

      case 0xFFC23D: // Forward button pressed
        action = 3; // Turn servo to the right
        previousAction = 3; break;

      case 0xFF22DD: // Backward button pressed 
        action = 4; // Turn servo to the left
        previousAction = 4; break;

      case 0xFF18E7: // Button 2 pressed
        action = 5; // Complete task 2
        previousAction = 0; break;

      case 0xFF7A85: // Button 3 pressed
        action = 6; // Complete task 3
        previousAction = 0; break;

      case 0xFF6897: // Button 0 pressed
        action = 7; // Print ultrasonic readings to serial monitor
        previousAction = 7; break;

      case 0xFF02FD: // Play/Pause button pressed
        action = 8; // Straighten servo
        previousAction = 0; break;

      case 0xFFFFFFFF: // Previously pressed button held down
        action = previousAction; break;
    }
  irrecv.resume(); // Receive the next value
  }

  switch(action){
    case 0:
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      break;
 
    case 1: // Drive forwards
      mystepper.setSpeed(STEPPER_SPEED);
      mystepper.step(QUARTER_REV);
      action = 0; break;

    case 2: // Drive backwards
      mystepper.setSpeed(STEPPER_SPEED);
      mystepper.step(-QUARTER_REV);
      action = 0; break;

    case 3: // Rotate servo right
      servoAngle = constrain(servoAngle+SERVO_INC, SERVO_LEFT, SERVO_RIGHT);
      myservo.write(servoAngle); action = 0; break;

    case 4: // Rotate servo left
      servoAngle = constrain(servoAngle-SERVO_INC, SERVO_LEFT, SERVO_RIGHT);
      myservo.write(servoAngle); action = 0; break;

    case 5: // Task 2
      // Drive straight until p1
      mystepper.setSpeed(STEPPER_SPEED);
      mystepper.step(FULL_REV); mystepper.step(FULL_REV); mystepper.step(int(0.3*FULL_REV)); // total of 2.3 x FULL_REV
      delay(500);

      // Turn right
      for(servoAngle=SERVO_MID; servoAngle<SERVO_RIGHT; servoAngle+=SERVO_INC) {
        servoAngle = constrain(servoAngle, SERVO_MID, SERVO_RIGHT);
        myservo.write(servoAngle);
        delay(SERVO_TURN_DELAY); }
      
      mystepper.step(FULL_REV);

      for(servoAngle=SERVO_RIGHT; servoAngle>SERVO_MID; servoAngle-=SERVO_INC) {
        servoAngle = constrain(servoAngle, SERVO_MID, SERVO_RIGHT);
        myservo.write(servoAngle);
        delay(SERVO_TURN_DELAY); }
      
      // Drive straight until p2
      mystepper.step(FULL_REV); mystepper.step(FULL_REV); mystepper.step(int(0.3*FULL_REV)); // total of 2.3 x FULL_REV
      delay(500);

      // Turn left
      for(servoAngle=SERVO_MID; servoAngle>SERVO_LEFT; servoAngle-=SERVO_INC) {
        servoAngle = constrain(servoAngle, SERVO_LEFT, SERVO_MID);
        myservo.write(servoAngle);
        delay(SERVO_TURN_DELAY); }
   
      mystepper.step(int(0.8*FULL_REV));

      for(servoAngle=SERVO_LEFT; servoAngle<SERVO_MID; servoAngle+=SERVO_INC) {
        servoAngle = constrain(servoAngle, SERVO_LEFT, SERVO_MID);
        myservo.write(servoAngle);
        delay(SERVO_TURN_DELAY); }

      myservo.write(SERVO_MID+SERVO_INC/2);
      delay(500);
      
      // Drive straight until p3
      mystepper.step(FULL_REV); mystepper.step(FULL_REV); mystepper.step(int(0.3*FULL_REV)); // total of 2.3 x FULL_REV
      delay(500);
      action = 0; break;

    case 6: // Task 3
      mystepper.setSpeed(STEPPER_SPEED);
      distance = ultrasonic.Distance();
      while( distance > 30 ) {
        mystepper.step(QUARTER_REV*2);
        distance = ultrasonic.Distance(); }

      // Evasive maneuver
      myservo.write(int((SERVO_MID+SERVO_RIGHT)/2)); // steer right
      mystepper.step(FULL_REV); delay(250); // turn
      myservo.write(SERVO_MID); // straigthen
      mystepper.step(FULL_REV*2); // advance past obstacle
      myservo.write(int((SERVO_LEFT+SERVO_MID)/2)); // steer left
      mystepper.step(FULL_REV*2); delay(500); // turn twice as much as before
      myservo.write(SERVO_MID); // straigthen
      mystepper.step(FULL_REV*2); // advance back to linear path
      myservo.write(int((SERVO_MID+SERVO_RIGHT)/2)); // steer right
      mystepper.step(FULL_REV); delay(250); // turn
      myservo.write(SERVO_MID); // straigthen
      
      // Go to finish
      distance = ultrasonic.Distance();
      while( distance > 15 ) {
        mystepper.step(QUARTER_REV*2);
        distance = ultrasonic.Distance(); }
      action = 0; break;

    case 7: // See ultrasonic sensor readings in Serial Monitor
      distance = ultrasonic.Distance();
      Serial.println(distance); action = 0; break;

    case 8: // Straighten servo
      servoAngle = SERVO_MID;
      myservo.write(servoAngle); action = 0; break;
  }
}