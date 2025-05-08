Ultrasonic Distance Alert System (Arduino + HC-SR04)
This Arduino-based system monitors object distance using an HC-SR04 ultrasonic sensor and triggers LED and buzzer alerts based on proximity. It includes error handling, retry logic for failed distance measurements, and structured alert mechanisms to ensure reliable operation.

Features
- Real-time distance measurement via HC-SR04 ultrasonic sensor
- Dynamic alert system using LEDs and buzzers
- Error handling with retry logic (up to 3 retries for sensor failures)
- Improved pulse timing for accurate readings
- Optimized code structure for readability and efficienc

Sensor & Hardware Requirements
- Arduino board (Uno, Mega, etc.)
- HC-SR04 ultrasonic distance sensor
- Piezo buzzers (high and low alert)
- 2 LEDs (Green = Safe zone, Red = Danger zone)
- Wiring & breadboard for connections

How It Works
- The system sends out an ultrasonic pulse and measures the duration of its echo to determine distance.
- If the sensor fails to read, it retries up to 3 times before reporting an error.
- Based on the distance, the system triggers different alert modes:
- Green LED (Safe Zone) → Distance > 100cm
- Red LED + High Buzzer (Danger Zone) → Distance < 50cm
- Flashing Red/Green LEDs + Low Buzzer Pulse (Warning Zone) → Distance between 50cm and 100cm
- The measured distance is displayed via serial output every 900ms.

Future Improvements
- Add LCD display for real-time distance visualization
- Integrate motor control or automated response based on proximity
- Expand Wi-Fi connectivity for remote monitoring
