const int analogPin = A5;
const float seriesResistor = 22000.0;
const float vcc = 5.05;

// Calculated from https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html

// Readings taken:
//      R - T
//  46200 - 66.11
// 428129 - 13.88
// 239697 - 25
const float A = -0.578e-3;
const float B = 3.61e-4;
const float C = -2.84e-7;
const float R0 = 249646.26;
const float Beta = 4150.99;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  int analogValue = analogRead(analogPin);
  float voltage = analogValue * vcc / 1023.0;

  // Calculate resistance of the thermistor
  float thermistorResistance = (voltage * seriesResistor) / (vcc - voltage);

  // Calculate temperatue reading
  float temperatureF = calculateTemperatureF(thermistorResistance);
  float temperatureF2 = calculateTemperatureF2(thermistorResistance);

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
  Serial.print("Temperature: ");
  Serial.print(temperatureF2);
  Serial.println(" F");


  delay(1000);
}


float calculateTemperatureF(float resistance) {
  // Calculate temperature in Kelvin using the Steinhart-Hart equation
  float logR = log(resistance);
  float invT = A + B * logR + C * logR * logR * logR;
  float tempK = 1.0 / invT;

  // Convert Kelvin to Fahrenheit
  float tempF = (tempK - 273.15) * 9.0 / 5.0 + 32.0;
  return tempF;
}

float calculateTemperatureF2(float resistance) {
  float tempK = 1.0 / (log(resistance / R0) / Beta + 1.0 / (298.15));

  return (tempK - 273.15) * 9.0 / 5.0 + 32.0;
}