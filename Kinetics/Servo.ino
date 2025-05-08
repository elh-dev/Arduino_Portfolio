#include "Servo.h"  // Include the Servo library for motor control

// Create a Servo object to control the motor
Servo myservo;

// Define the servo control pin
#define servoPin 9

void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging
    
  // Attach the servo object to the defined pin
  myservo.attach(servoPin);

  Serial.println("Servo system initialized.");
}

void loop() {
  // Basic servo positioning
  Serial.println("Setting servo to 90 degrees...");
  myservo.write(90);  // Move servo to 90 degrees
  delay(1000);

  Serial.println("Setting servo to 180 degrees...");
  myservo.write(180);  // Move servo to 180 degrees
  delay(1000);

  Serial.println("Setting servo to 0 degrees...");
  myservo.write(0);  // Move servo to 0 degrees
  delay(1000);

  // Sweep motion: slowly move from 0 to 180 degrees
  Serial.println("Sweeping servo from 0 to 180 degrees...");
  for (int angle = 0; angle <= 180; angle += 1) {
      myservo.write(angle);  // Increment position gradually
      delay(15);  // Smooth movement delay
  }

  // Reverse sweep: slowly move from 180 back to 0 degrees
  Serial.println("Sweeping servo from 180 to 0 degrees...");
  for (int angle = 180; angle >= 0; angle -= 1) {
      myservo.write(angle);  // Decrease position gradually
      delay(15);  // Smooth movement delay
  }

  Serial.println("Cycle complete. Restarting in 1 second...");
  delay(1000);  // Pause before restarting loop
}