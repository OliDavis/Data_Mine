/*

NOTE: Print the raw GPS strings into the serial monitor.
Arduino UNO
Rx Serial0 pin 0
 
*/


#include <util/delay.h>                   //provides delay of us and ms from AVR library (arduino-1.0\hardware\tools\avr\avr\include\util)


void setup() {
    Serial.begin(9600);
    Serial.println();
}


//-------------------------------------------------------
// LOOP
//-------------------------------------------------------

int inByte = 0;         // incoming serial byte

void loop() {
  //Serial.println("Loop");
  //delay(1000);
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    Serial.write(inByte);
  }
}
