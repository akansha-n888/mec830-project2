
/* Robot contains 3 caster wheels, 2 fixed wheels, 1 DC motor and 1 stepper.
The following program is the attributed tot Task 1 (manual control)*/

#include "IRremote.h"
#include <Stepper.h>

const int stepsPerRevolution = 2048;
Stepper stepper = Stepper(stepsPerRevolution, 8, 9, 10, 11);                 

//DC motor paramters
#define ENABLE 5
#define DIRA 3
#define DIRB 4

int receiver = 12;
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;  

void setup() {

  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);

  stepper.step(0);                  
  stepper.setSpeed(10);   

  irrecv.enableIRIn();
  
  Serial.begin(9600);

}

void loop() 

{
if (irrecv.decode(&results))                             // have we received an IR signal?

  {
    switch(results.value)

    {

      case 0xFFA857:                                     // VOL+ button pressed
        digitalWrite(ENABLE,HIGH);                       // enable on
        digitalWrite(DIRA,HIGH);                         //one way
        digitalWrite(DIRB,LOW);
        delay(500); 
                      break;

      case 0xFF629D:                                     // VOL- button pressed
        digitalWrite(DIRA,LOW);                          //reverse
        digitalWrite(DIRB,HIGH);
        delay(500);
                      break;

      case 0xFFC23D:                                     // Forward Skip button pressed
          stepper.step(1000);                            //cw
          delay(50);     

      case 0xFF22DD:                                     // Backward Skip button pressed          
          stepper.step(-1000);                           //ccw
          delay(500);
    }
    
      irrecv.resume(); // receive the next value
                 digitalWrite(8, LOW);
                 digitalWrite(9, LOW);
                 digitalWrite(10, LOW);
                 digitalWrite(11, LOW);       
  }  
}
