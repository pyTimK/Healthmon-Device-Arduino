#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

// Time interval for each read
#define REPORTING_PERIOD_MS     500

// INITIALIZE VARS
PulseOximeter pox;
uint32_t tsLastReport = 0;
char strBuf[50];

void setup() {
  // Sets the baudrate for the serial communication
  // This must be same with the RPi
  Serial.begin(115200);

  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiringwh, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}


void loop() {
  // Make sure to call pox.update as fast as possible
  pox.update();

  // Read only after the time interval since the last reading
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

    // Get the measurements of MAX30100
    int pulse = pox.getHeartRate();
    int spo2 = pox.getSpO2();

    // Sent the data to do the Serial port
    // Values will be read by the RPi
    sprintf(strBuf, "%d %d", pulse, spo2);
    Serial.println(strBuf);

    // Update report timestamp
    tsLastReport = millis();
  }
}
