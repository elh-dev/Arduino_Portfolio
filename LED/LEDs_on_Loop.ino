// LED pins
const int ledPins[] = {13, 12, 11, 10, 9, 8};  // Array of LEDs
const int buttonPin = 3;  // Button pin

void setup() {
    Serial.begin(9600);  // Initialize serial for debugging

    // Set LED pins as outputs
    for (int i = 0; i < 6; i++) {
        pinMode(ledPins[i], OUTPUT);
    }

    // Set button pin as input
    pinMode(buttonPin, INPUT);
}

void loop() {
    // Read button state correctly
    if (digitalRead(buttonPin) == HIGH) {
        Serial.println("Button Pressed! Activating LEDs...");
    
        // Turn LEDs on and off sequentially
        for (int i = 0; i < 6; i++) {
            digitalWrite(ledPins[i], HIGH);
            delay(1000);
            digitalWrite(ledPins[i], LOW);
        }
    }
}