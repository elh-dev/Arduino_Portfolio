// LED pin
const int led = 13;

void setup() {
    // set LED pin as output 
    pinMode(led, OUTPUT);

}

void loop() {
  
    // LED on for 2 seconds 
    digitalWrite(led, HIGH);
    delay(2000);
    // LED off for 1 seconds 
    digitalWrite(led, LOW);
    delay(1000);

}