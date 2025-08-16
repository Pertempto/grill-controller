#include <Arduino.h>
#include <math.h>
#include "SevSeg.h"

SevSeg sevseg;
const int thermistorPin = PA0;
const float seriesResistor = 22000.0;
const float vcc = 3.29;
unsigned long previousMillis = 0;
const int interval = 1000;
float temperatureF = 0.0;

// Values calculated from https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html
// See https://www.desmos.com/calculator/j0msjbrgxe
float calculateTemperatureF(float resistance) {
  float tempK = 1 / (1.346e-3 + 0.7368e-4 * log(resistance) + 6.833e-7 * pow(log(resistance), 3));
  return (tempK - 273.15) * 1.8 + 32;
}

void setup() {
  Serial.begin(9600);
  pinMode(thermistorPin, INPUT);
  analogReadResolution(12);

  byte numDigits = 4;
  byte digitPins[] = {PA7, PA4, PA3, PA1};
  byte segmentPins[] = {PA6, PA2, PB6, PB5, PB4, PA5, PB7, PC15};
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins, false, false, false);
  sevseg.setBrightness(100);
}

void loop() {
  // --- This function MUST be called repeatedly in the loop to update the display ---
  sevseg.refreshDisplay();

  // --- Non-Blocking Temperature Reading ---
  // It's time to read the temperature again (e.g., 1 second has passed)
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Save the last time you read the sensor
    previousMillis = currentMillis;
    int analogValue = analogRead(thermistorPin);
    float voltage = analogValue * vcc / 4095.0;

    // Calculate resistance of the thermistor
    float thermistorResistance = 0;
    if (vcc > voltage && voltage > 0) {
      thermistorResistance = (voltage * seriesResistor) / (vcc - voltage);
    }

    // Calculate temperatue reading
    temperatureF = calculateTemperatureF(thermistorResistance);

    Serial.print("Temperature: ");
    Serial.print(temperatureF);
    Serial.println(" F");

    sevseg.setNumber(temperatureF, 0);
  }
}

