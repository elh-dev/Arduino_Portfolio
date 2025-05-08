#include "DHT.h"  // Include DHT sensor library

// Define pin for the DHT module
#define DHTIN 2

// Specify the DHT sensor type (DHT11 in this case)
#define DHTTYPE DHT11

// Create a DHT object using the defined pin and sensor type
DHT dht(DHTIN, DHTTYPE);

// Calibration offsets for accuracy
float temperatureOffset = -3.0;  // Adjust based on thermometer calibration
float humidityOffset = 0.0;  // Adjust as needed for accuracy

// Fan control pin
const int fan = 13;  // Fan connected to pin 13

// Timer variables for fan operation
unsigned long previousMillis = 0;

// Fan durations (testing values for short intervals)
const unsigned long fanOnDuration = 15 * 60000;  // 15 minutes
const unsigned long fanOffDuration = 10 * 60000; // 10 minutes

// Fan state tracker
bool fanState = false;

// Last known sensor values for error recovery
float previousHumidity = 50.0;  // Default reasonable humidity
float previousTemperature = 22.0;  // Default reasonable temperature

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  dht.begin();  // Start the DHT sensor

  // Set fan pin as output and ensure it's off initially
  pinMode(fan, OUTPUT);
  digitalWrite(fan, LOW);

  Serial.println("System Initialized. Monitoring sensors and fan control...");
}

void loop() {
  float humidity = dht.readHumidity();  // Read humidity
  float temperature = dht.readTemperature();  // Read temperature
  unsigned long currentMillis = millis();  // Track elapsed time

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

  // Fan control logic based on timing
  if (fanState && (currentMillis - previousMillis >= fanOnDuration)) {
    // Turn the fan off after designated duration
    digitalWrite(fan, LOW);
    fanState = false;
    previousMillis = currentMillis;  // Reset the timer
    Serial.println("Fan turned OFF");
  } else if (!fanState && (currentMillis - previousMillis >= fanOffDuration)) {
    // Turn the fan on after designated duration
    digitalWrite(fan, HIGH);
    fanState = true;
    previousMillis = currentMillis;  // Reset the timer
    Serial.println("Fan turned ON");
  }

  delay(10000);  // Delay before next sensor reading
}