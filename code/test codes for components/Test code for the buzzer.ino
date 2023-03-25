#include <Adafruit_Fingerprint.h>
int pinBuzzer = 13; 
void setup() {
  pinMode(pinBuzzer, OUTPUT); 
} 
void loop() { 
   long frequency = 300; 
   tone(pinBuzzer, frequency );
   delay(1000); 
   noTone(pinBuzzer);
   delay(2000); 
}
