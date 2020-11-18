#include "IRremote.h"
#include <Servo.h>

Servo myservo;
#define ENABLE 5
#define DIRA 3
#define DIRB 4
int receiver = 12;
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
        digitalWrite(ENABLE,HIGH); // enable on
        digitalWrite(DIRA,HIGH); //one way
        digitalWrite(DIRB,LOW);
        delay(500); 
                      break;

      case 0xFF629D: // VOL- button pressed
        digitalWrite(DIRA,LOW);  //reverse
        digitalWrite(DIRB,HIGH);
        delay(500);
                      break;

      case 0xFFC23D: // Forward Skip button pressed
                     myservo.write(0); 

      case 0xFF22DD: // Backward Skip button pressed 
                     myservo.write(180);
                
    }
    
      irrecv.resume(); // receive the next value
                 digitalWrite(8, LOW);
                 digitalWrite(9, LOW);
                 digitalWrite(10, LOW);
                 digitalWrite(11, LOW);       
  }  
}
