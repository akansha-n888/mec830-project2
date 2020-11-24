/*

IR codes (same layout as the remote):

  FFA25D - Power Button     FF629D - VOL+           FFE21D - FUNC/STOP
  FF22DD - Backward         FF02FD - Play/Pause     FFC23D - Forward
  FFE01F - Down Arrow       FFA857 - VOL-           FF906F - Up Arrow
  FF6897 - 0                FF9867 - EQ             FFB04F - ST/REPT
  FF30CF - 1                FF18E7 - 2              FF7A85 - 3
  FF10EF - 4                FF38C7 - 5              FF5AA5 - 6
  FF42BD - 7                FF4AB5 - 8              FF52AD - 9

*/

#include <IRremote.h>

const int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
}

void loop(){
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        irrecv.resume();
  }
}
