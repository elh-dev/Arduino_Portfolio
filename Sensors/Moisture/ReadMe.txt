Soil Moisture Monitoring System (Arduino + Analog Sensor)
This Arduino-based system measures soil moisture levels using an analog soil moisture sensor and displays readings via serial output. It incorporates error handling, retry logic for sensor failures, and calibration adjustments to ensure reliable and accurate moisture level tracking.

Hardware Requirements
- Arduino board (Uno, Mega, etc.)
- Analog soil moisture sensor
- Wiring & breadboard for connections

How It Works
- The system reads soil moisture levels via an analog input.
- If the sensor fails, it attempts up to 3 retries before falling back on the last known value.
- The raw sensor reading is mapped to a percentage scale based on user-defined dry and wet calibration values.
- Moisture levels are printed to the serial monitor every 5 seconds.

Future Improvements
- Add relay-controlled irrigation based on moisture thresholds
- Integrate data logging for trend analysis
- Expand to Wi-Fi connectivity for remote monitorin
