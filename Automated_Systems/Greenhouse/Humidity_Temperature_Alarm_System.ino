#include "DHT.h"  // Include DHT sensor library

// Define pin for the DHT module
#define DHTIN 2

// Specify the DHT sensor type (DHT11 in this case)
#define DHTTYPE DHT11

// Create a DHT object using the defined pin and sensor type
DHT dht(DHTIN, DHTTYPE);

// Calibration offsets for accuracy
float temperatureOffset = -0.8;  // Adjust based on thermometer calibration
float humidityOffset = -4.0;  // Adjust as needed for accuracy

// Alarm system pins
const int red = 13;  // High humidity alert
const int yel = 12;  // High temperature alert
const int blu = 11;  // Low humidity alert
const int whi = 10;  // Low temperature alert
const int gre = 9;   // Optimal range indicator

// Last known sensor values for error recovery
float previousHumidity = 50.0;  // Default reasonable humidity
float previousTemperature = 22.0;  // Default reasonable temperature

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  dht.begin();  // Start the DHT sensor

  // Set alarm pins as outputs and ensure all alarms are initially off
  pinMode(red, OUTPUT);
  pinMode(yel, OUTPUT);
  pinMode(blu, OUTPUT);
  pinMode(whi, OUTPUT);
  pinMode(gre, OUTPUT);
  
  digitalWrite(red, LOW);
  digitalWrite(yel, LOW);
  digitalWrite(blu, LOW);
  digitalWrite(whi, LOW);
  digitalWrite(gre, LOW);

  Serial.println("System Initialized. Monitoring sensors...");
}

void loop() {
  float humidity = dht.readHumidity();  // Read humidity
  float temperature = dht.readTemperature();  // Read temperature

  // Retry logic for failed sensor readings (up to 3 attempts)
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
      humidity = previousHumidity;
      temperature = previousTemperature;
      break;  // Exit retry loop
    }
  }

  // Apply calibration offsets
  float calibratedTemperature = temperature + temperatureOffset;
  float calibratedHumidity = humidity + humidityOffset;

  // Check for unrealistic readings
  if (calibratedHumidity < 0 || calibratedHumidity > 100) {
    Serial.println("Warning: Humidity reading is out of range! Ignoring this value.");
    calibratedHumidity = previousHumidity;
  }
  if (calibratedTemperature < -40 || calibratedTemperature > 80) {
    Serial.println("Warning: Temperature reading is out of range! Ignoring this value.");
    calibratedTemperature = previousTemperature;
  }

  // Display sensor readings
  Serial.print("Calibrated Humidity: ");
  Serial.print(calibratedHumidity);
  Serial.print("%, Calibrated Temp: ");
  Serial.print(calibratedTemperature);
  Serial.println(" °C");

  // Store current values for future use
  previousHumidity = calibratedHumidity;
  previousTemperature = calibratedTemperature;

  // Alarm triggers based on humidity
  digitalWrite(red, calibratedHumidity > 90 ? HIGH : LOW); // High humidity
  digitalWrite(blu, calibratedHumidity < 70 ? HIGH : LOW); // Low humidity

  if (calibratedHumidity >= 70 && calibratedHumidity <= 90) {
    Serial.println("Alarm OFF: Humidity within range.");
  }

  // Alarm triggers based on temperature
  digitalWrite(yel, calibratedTemperature > 25 ? HIGH : LOW); // High temperature
  digitalWrite(whi, calibratedTemperature < 20 ? HIGH : LOW); // Low temperature

  if (calibratedTemperature >= 20 && calibratedTemperature <= 25) {
    Serial.println("Alarm OFF: Temperature within range.");
  }

  // Green light indicates optimal humidity and temperature range
  digitalWrite(gre, (calibratedTemperature >= 20 && calibratedTemperature <= 25 &&
                      calibratedHumidity >= 70 && calibratedHumidity <= 90) ? HIGH : LOW);

  delay(5000);  // Delay before next sensor reading
}