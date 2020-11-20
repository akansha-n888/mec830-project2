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

int previous = 0;
int pos =0;

void setup()
{
  pinMode(echoPin, INPUT);                                        //initialise sensor paramters
  pinMode(trigPin, OUTPUT);
 
  myservo.attach(7);                                             //set servo to pin 7
  myservo.write(0);
     
  stepper.setSpeed(50);                                          //adjust
     
 //Initialize PID parameters
  myPID.SetMode(AUTOMATIC);
  myPID.SetTunings(Kp, Ki, Kd);
  myPID.SetOutputLimits(-255, 255);

  Serial.begin(56000);

}

void loop()
{

  int obstacle = 7;                        //7 cm from path
  distance = calculateDistance();
  int val = distance;
  
  //move stepper to move foward-back (motorknob)
  
  if (distance > obstacle){
      stepper.step(val - previous);       //move a number of steps equal to change in sensor reading
      previous = val;
      
      Serial.print("distance:");
      Serial.print(distance);
      Serial.print("\t");
      delay(10);
      Serial.print("steps:");
      Serial.print(val - previous);
      Serial.print("\t");
      delay(10); 
      }
   else{
      stepper.step(val + previous);       //move a number of steps equal to change in sensor reading
      previous = val;
      
      Serial.print("distance:");
      Serial.print(distance);
      Serial.print("\t");
      delay(10);
      Serial.print("steps:");
      Serial.print(val + previous);
      Serial.print("\t");
      delay(10); 
      }

  Input = distance;
  Setpoint = obstacle;
  
  myPID.Compute();
  pos = map(Output, 0, 255, 0, 180);   //maybe change the mapping to smaller values 

  if (distance == obstacle){
  myservo.write(pos);
  }
  
  Serial.print("Input:");
  Serial.print(Input);
  Serial.print("  ");
  delay(50);
  Serial.print("Output:");
  Serial.print(Output);
  Serial.print("  ");
  delay(50);
  Serial.print("Servo Angle:");
  Serial.print(pos);
  Serial.print("  ");
  delay(50);
  Serial.print("Setpoint:");
  Serial.println(Setpoint);
  delay(50);
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
