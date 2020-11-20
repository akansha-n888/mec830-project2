#include <PID_v1.h>
#include <Servo.h>
#include <Stepper.h>

double angle;
int echoPin = 12;
int trigPin = 11;
long duration;
double distance;

const int stepsPerRevolution = 2048;
Stepper stepper = Stepper(stepsPerRevolution, 8, 9, 10, 11);

Servo myservo;

//PID Parameters
double Setpoint, Input, Output;
double Kp = 3.2, Ki =0.0002, Kd = 0.0002;                           //need to retune/adjust based on your set up
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup()
{
  pinMode(echoPin, INPUT);                                        //initialise sensor paramters
  pinMode(trigPin, OUTPUT);
 
  myservo.attach(7);                                             //set servo to pin 7
  myservo.write(0);
  Setpoint = 7;                                                 //adjust setpoint
     
  stepper.setSpeed(5);                                          //adjust
     
 //Initialize PID parameters
  myPID.SetMode(AUTOMATIC);
  myPID.SetTunings(Kp, Ki, Kd);
  myPID.SetOutputLimits(-255, 255);

  Serial.begin(56000);

}

void loop()
{

  //move stepper to move foward-back
  if (){
      stepper.step(diff/20 - previous);
      previous = diff;
      Serial.print("steps ldr1:");
      Serial.print(diff/20 - previous);
      Serial.print("\t");
      delay(10) 
      }
   else{
          stepper.step(diff/20 - previous);
      previous = diff;
      Serial.print("steps ldr1:");
      Serial.print(diff/20 - previous);
      Serial.print("\t");
      delay(10)
      }

      //move servo motor

  if ()
  distance = calculateDistance();
  delay(20);
  Input = distance;
  delay(10);
  
  myPID.Compute();
  
  angle = map(Output, -25, 25, 0, 120);   //maybe change the mapping to smaller values 
  delay(20);     
  myservo.write(angle);
  delay(10); 
 
  Serial.print("Angle:");
  Serial.print(angle);
  Serial.print("\t");
  delay(10);
  Serial.print("Input:");
  Serial.print(Input);
  Serial.print("\t");
  delay(10);
  Serial.print("Output:");
  Serial.print(Output);
  Serial.print("\t");
  delay(10);
  Serial.print("Setpoint:");
  Serial.println(Setpoint);
  delay(10);
}

// Function for calculating the distance measured by the Ultrasonic sensor
float calculateDistance(){ 
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance= duration*0.034/2;
  return distance;
}
