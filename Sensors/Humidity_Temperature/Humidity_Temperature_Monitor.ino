#include "DHT.h"  // Include DHT sensor library

// Define pin for the DHT module
#define DHTIN 2

// Specify the DHT sensor type (DHT11 in this case)
#define DHTTYPE DHT11

// Create a DHT object using the defined pin and sensor type
DHT dht(DHTIN, DHTTYPE);

// Soil Moisture Sensor – Moved to a different pin to avoid conflicts
const int soilSensorPin = A0;  // Changed from digital pin to analog pin

// Variables to store last known sensor values (for error recovery)
float previousHumidity = 50.0;  // Default reasonable value
float previousTemperature = 22.0; // Default reasonable value

void setup() {
  Serial.begin(9600);  // Initialize serial communication

  dht.begin();  // Start the DHT sensor

  Serial.println("System Initialized. Monitoring sensors...");
}

void loop() {
  float humidity = dht.readHumidity();  // Read humidity
  float temperature = dht.readTemperature();  // Read temperature

  // Retry logic for sensor failure (up to 3 attempts)
  int retryCount = 0;
  const int maxRetries = 3;

  while (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT Module, retrying...");
    delay(1000);  // Wait before retrying
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    
    retryCount++;
    if (retryCount >= maxRetries) {
      Serial.println("Sensor error: Maximum retries reached. Using last known values.");
      humidity = previousHumidity; // Use previous value
      temperature = previousTemperature;
      break;  // Break out of loop
    }
  }

  // Check for unreasonable sensor values
  if (humidity < 0 || humidity > 100) {
    Serial.println("Warning: Humidity reading is out of range! Ignoring this value.");
    humidity = previousHumidity; // Use last known good value
  }
  if (temperature < -40 || temperature > 80) {
    Serial.println("Warning: Temperature reading is out of range! Ignoring this value.");
    temperature = previousTemperature;
  }

  // Print sensor data
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Temp: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Store current values as last known values for future use
  previousHumidity = humidity;
  previousTemperature = temperature;

  delay(2000);  // Delay before next reading
}