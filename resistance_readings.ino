const int analogPin = A5;
const float seriesResistor = 22000.0;
const float vcc = 5.05;

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


  delay(1000);
}


// Values calculated from https://www.desmos.com/calculator/j0msjbrgxe
float calculateTemperatureF(float resistance) {
  return 932.9 - 76.9 * log(resistance);
}

