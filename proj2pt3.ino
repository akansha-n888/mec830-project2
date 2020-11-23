#include <Servo.h>
#include <IRremote.h>
#include <Stepper.h>
#include <Servo.h>

/*----- Variables, Pins -----*/
#define STEPS  32   // Number of steps per revolution of Internal shaft
int  Steps2Take;  // 2048 = 1 Revolution
int receiver = 2; // Signal Pin of IR receiver to Arduino Digital Pin 6


float wheel_dia = 6.5; //Stepper wheel diameter
float dist_per_rot = 3.14*pow((wheel_dia/2.0),2.0);
float steps_per_cm = 2048/dist_per_rot;
   
int trig = 4;
int echo = 3;

const int scale = 58;
bool start = false;
bool pathfound = false;
float initial_distance = 0;
float current_dist = 0;
int angle = 0;


Servo myservo;

Stepper small_stepper(STEPS, 8, 10, 9, 11);
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

void setup(){ 
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  irrecv.enableIRIn(); // Start the receiver
  myservo.attach(5);
  myservo.write(90);
  Serial.begin(9600);
  Serial.println("rdy");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results)){ // have we received an IR signal?
    switch(results.value){
      case 0xFFA857:// VOL- button pressed
        start = true;
        Serial.println("start");
        break;
    }
    irrecv.resume();//maybe add a loop here to constantly rotate servo?
  }
  if (start){
    initial_distance = distance();
    if (!pathfound){
      for(int i = 18; i<36; i++){
        myservo.write(i*5);
        delay(100);
        angle = i*5;
        current_dist = distance();
        Serial.println(current_dist);
        if (current_dist > 50){
          pathfound = true;
          break;
          }  //Set this to whatever you want as long as itll activate when it doesnt see the obj anymore
      }
    }
    if (pathfound){
      myservo.write(angle+10); //rotates an extra 10deg to guarantee not hitting the obj
  
      Steps2Take = steps_per_cm*initial_distance; //Calculates distance to midpoint
      
      small_stepper.step(int(Steps2Take));
      
      myservo.write(170-angle); //rotates to the complementary angle
  
      small_stepper.step(int(Steps2Take));
      start = false;
    }
  }
}

float distance(){
  long dist = 0;
  float cm = 0;
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  dist = pulseIn(echo, HIGH);
  cm = double(dist)/scale;
  return cm;
}
