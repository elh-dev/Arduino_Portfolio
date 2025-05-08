// Soil Moisture Sensor Configuration
const int sensorPin = A1;  // Analog pin for soil moisture sensor

// Calibration settings for mapping raw values to percentage
const int dryValue = 0;    // Adjust based on dry soil readings
const int wetValue = 677;  // Adjust based on saturated soil readings

// Last known moisture level for error recovery
int previousMoistureLevel = 50;  // Default reasonable moisture level

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  Serial.println("System Initialized. Monitoring soil moisture...");
}

void loop() {
  // Read sensor value
  int sensorValue = analogRead(sensorPin);

  // Retry logic for failed sensor readings (up to 3 attempts)
  int retryCount = 0;
  const int maxRetries = 3;

  while (sensorValue < 0 || sensorValue > 1023) {  // Ensures the reading is within valid ADC range
    Serial.println("Sensor error, retrying...");
    delay(1000);  // Wait before retrying
    sensorValue = analogRead(sensorPin);

    retryCount++;
    if (retryCount >= maxRetries) {
      Serial.println("Sensor error: Maximum retries reached. Using last known value.");
      sensorValue = map(previousMoistureLevel, 0, 100, dryValue, wetValue);  // Convert previous percentage to raw value
      break;  // Exit retry loop
    }
  }

  // Map reading to 0-100% moisture level
  int moistureLevel = map(sensorValue, dryValue, wetValue, 0, 100);

  // Check for unrealistic values
  if (moistureLevel < 0 || moistureLevel > 100) {
    Serial.println("Warning: Moisture reading out of range! Ignoring this value.");
    moistureLevel = previousMoistureLevel;
  }

  // Print moisture level to serial monitor
  Serial.print("Moisture Level: ");
  Serial.print(moistureLevel);
  Serial.println("%");

  // Store current reading for future reference
  previousMoistureLevel = moistureLevel;

  delay(5000);  // Delay before next sensor reading
}