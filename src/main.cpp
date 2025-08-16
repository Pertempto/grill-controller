#include <Arduino.h>
#include <math.h>

const int thermistorPin = PA1;
const float seriesResistor = 22000.0;
const float vcc = 3.29;

// Values calculated from https://www.desmos.com/calculator/j0msjbrgxe
float calculateTemperatureF(float resistance) {
  return 932.9 - 76.9 * log(resistance);
}

void setup() {
  Serial.begin(9600);
  pinMode(thermistorPin, INPUT);
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

