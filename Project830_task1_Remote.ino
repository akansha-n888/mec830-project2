#include <IRremote.h>
#include <Stepper.h>
#include <Servo.h>

/*----- Variables, Pins -----*/
#define STEPS  32   // Number of steps per revolution of Internal shaft
int  Steps2Take;  // 2048 = 1 Revolution
int receiver = 6; // Signal Pin of IR receiver to Arduino Digital Pin 6
   
   Servo myservo;

Stepper small_stepper(STEPS, 8, 10, 9, 11);
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

void setup()
{ 
  irrecv.enableIRIn(); // Start the receiver
  myservo.attach(4);
  myservo.write(0);
  Serial.begin(9600);
}

void loop()
{
if (irrecv.decode(&results)) // have we received an IR signal?

  {
    switch(results.value)

    {

      case 0xFF629D: // VOL+ button pressed
                      small_stepper.setSpeed(500); //Max seems to be 500
                      Steps2Take  =  2048;  // Rotate CW
                      small_stepper.step(Steps2Take);
                      delay(2000); 
                      break;                                                     //maybe add a loop here to constantly rotate servo?

      case 0xFFA857: // VOL- button pressed
                      small_stepper.setSpeed(500);
                      Steps2Take  =  -2048;  // Rotate CCW
                      small_stepper.step(Steps2Take);
                      delay(2000); 
                      break;                                                  //maybe add a loop here to constantly rotate servo?

      case 0xFF22DD: // Forward Skip button pressed
                     myservo.write(1); 
                     break;

      case 0xFFC23D: // Backward Skip button pressed 
                     myservo.write(180);
                     break;
                                
    }
      irrecv.resume(); // receive the next value
                 digitalWrite(8, LOW);
                 digitalWrite(9, LOW);
                 digitalWrite(10, LOW);
                 digitalWrite(11, LOW);       
  }  
}
