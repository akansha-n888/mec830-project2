#include "IRremote.h"
#include "Stepper.h"
#include <Servo.h>

#define STEPS  32   // Number of steps per revolution of Internal shaft
int  Steps2Take;

Servo myservo;
#define ENABLE 5
#define DIRA 4
#define DIRB 3
int receiver = 12;

Stepper small_stepper(STEPS, 8, 10, 9, 11);
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;  

void setup() {
   myservo.attach(7);
   myservo.write(90);

  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);

   irrecv.enableIRIn();
  
  Serial.begin(9600);

}

void loop() 

{
if (irrecv.decode(&results)) // have we received an IR signal?

  {
    switch(results.value)

    {

      case 0xFFA857: // VOL+ button pressed
        digitalWrite(ENABLE,50); // enable on
        digitalWrite(DIRA,HIGH); //one way
        digitalWrite(DIRB,LOW);
        delay(3000);
        digitalWrite(ENABLE,LOW); //slow stop
        delay(1000); 
                      break;

      case 0xFF629D: // VOL- button pressed
        digitalWrite(ENABLE,50);
        digitalWrite(DIRA,LOW);  //reverse
        digitalWrite(DIRB,HIGH);
        delay(3000);
        digitalWrite(ENABLE,LOW); //slow stop
        delay(1000);
                      break;

      case 0xFFC23D: // Forward Skip button pressed
                      small_stepper.setSpeed(500); //Max seems to be 500
                      Steps2Take  =  512;  // Rotate CW
                      small_stepper.step(Steps2Take);
                      delay(100); 
                      break;
                      
      case 0xFF22DD: // Backward Skip button pressed 
                      small_stepper.setSpeed(500);
                      Steps2Take  =  -512;  // Rotate CCW
                      small_stepper.step(Steps2Take);
                      delay(100); 
                      break;               
    }
    
      irrecv.resume(); // receive the next value
                 digitalWrite(8, LOW);
                 digitalWrite(9, LOW);
                 digitalWrite(10, LOW);
                 digitalWrite(11, LOW);       
  }  
}
