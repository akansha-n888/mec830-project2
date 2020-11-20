#include "IRremote.h"
#include <Stepper.h>
#include <Servo.h>

Servo myservo;

const int stepsPerRevolution = 2048;
Stepper stepper = Stepper(stepsPerRevolution, 8, 9, 10, 11);                 


int receiver = 12;
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;  

void setup() {
  myservo.attach(7);

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

      case 0xFFA857:                                     // VOL+ button pressed - move system forward with stepper
      stepper.step(2048);                                 //maybe add loop here
      delay(10);
                      break;

      case 0xFF629D:                                     // VOL- button pressed - move system backward with stepper
      stepper.step(-2048);                                //maybe add a loop here?
      delay(10);
                      break;

      case 0xFFC23D:                                     // Forward Skip button pressed
          myservo.write(180);                            //cw
          delay(50);     

      case 0xFF22DD:                                     // Backward Skip button pressed          
          myservo.write(0);                                  //ccw
          delay(500);
    }
    
      irrecv.resume(); // receive the next value
                 digitalWrite(8, LOW);
                 digitalWrite(9, LOW);
                 digitalWrite(10, LOW);
                 digitalWrite(11, LOW);       
  }  
}
