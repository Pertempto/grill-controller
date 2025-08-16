#include <Arduino.h>
#include <math.h>

const int thermistorPin = PA0;
const float seriesResistor = 22000.0;
const float vcc = 3.29;

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
}

void loop() {
  int analogValue = analogRead(thermistorPin);
  float voltage = analogValue * vcc / 4095.0;

  // Calculate resistance of the thermistor
  float thermistorResistance = 0;
  if (vcc > voltage && voltage > 0) {
    thermistorResistance = (voltage * seriesResistor) / (vcc - voltage);
  }

  // Calculate temperatue reading
  float temperatureF = calculateTemperatureF(thermistorResistance);

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" Analog: ");
  Serial.print(analogValue);
  Serial.print(" Resistance: ");
  Serial.print(thermistorResistance);
  Serial.println(" ohms");
  Serial.print("Temperature: ");
  Serial.print(temperatureF);
  Serial.println(" F");

  delay(100);
}

