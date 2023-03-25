int pinRelay = 10; 
void setup() {
  pinMode(pinRelay, OUTPUT); 
}
void loop() { 
   digitalWrite(pinRelay, HIGH);
   delay(5000); 
   digitalWrite(pinRelay, LOW);
   delay(8000); 
}
