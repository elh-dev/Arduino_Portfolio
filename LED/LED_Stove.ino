// LED Pins
const int ledPinsW[] = {13, 12, 11, 10, 9, 8, 7, 6, 5};
const int ledPinsR[] = {14, 15, 16, 17, 18, 19, 20, 21, 22};

// Button Pin
const int buttonPin = 2;

// Configurable Settings
bool redOn = false, whiteOn = true, spacing = false;
int sep = 100; // Time spacing between LED groups

// Strobe Timing
unsigned long onTimeW = 50, offTimeW = 320;
unsigned long onTimeR = 500, offTimeR = 1000;

// State Variables
bool ledsOnW = false, ledsOnR = false, flashingW = false, flashingR = false;
bool lastButtonState = LOW;
unsigned long lastDebounceTime = 0, debounceDelay = 50;
unsigned long lastFlashTimeW = 0, lastFlashTimeR = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

  // Configure LED pins
  for (int i = 0; i < 9; i++) {
    pinMode(ledPinsW[i], OUTPUT);
    pinMode(ledPinsR[i], OUTPUT);
  }
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Debounce button press
  if (reading != lastButtonState) lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay && reading == HIGH) {
    flashingW = whiteOn ? !flashingW : flashingW;
    flashingR = redOn ? !flashingR : flashingR;
    delay(100); // Prevent multiple rapid toggles
  }

  unsigned long currentTime = millis();
  
  // Handle flashing logic using a function
  ledsOnW = updateFlashState(flashingW, ledsOnW, currentTime, lastFlashTimeW, onTimeW, offTimeW);
  ledsOnR = updateFlashState(flashingR, ledsOnR, currentTime, lastFlashTimeR, onTimeR, offTimeR);

  // Update LED states
  updateLEDs(ledPinsW, ledsOnW, spacing ? sep : 0);
  updateLEDs(ledPinsR, ledsOnR, 0);

  lastButtonState = reading;
}

// Function to update flash timing logic
bool updateFlashState(bool flashing, bool ledsOn, unsigned long currentTime, unsigned long &lastFlashTime, unsigned long onTime, unsigned long offTime) {
  if (!flashing) return false; // Ensure LEDs turn off when flashing is inactive
  
  if (ledsOn && (currentTime - lastFlashTime >= onTime)) {
    lastFlashTime = currentTime;
    return false;
  } else if (!ledsOn && (currentTime - lastFlashTime >= offTime)) {
    lastFlashTime = currentTime;
    return true;
  }
  
  return ledsOn;
}

// Function to update LED output
void updateLEDs(const int ledPins[], bool state, int spacing) {
  for (int i = 0; i < 9; i++) {
    digitalWrite(ledPins[i], state ? HIGH : LOW);
    if (spacing) delay(spacing);
  }
}