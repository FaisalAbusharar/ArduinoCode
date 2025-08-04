#include <LiquidCrystal.h>
#include <math.h>

// LCD wiring: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(9, 6, 3, 10, 11, 5);

// Thermistor configuration
const int thermistorPin = A0;
const float seriesResistor = 10000.0;       // 10k resistor
const float nominalResistance = 10000.0;    // 10k thermistor
const float nominalTemp = 25.0;             // 25°C baseline
const float bCoefficient = 3950.0;          // Beta coefficient


const int buzzerPin = 7;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Hello, world!");
  lcd.setCursor(0, 1);
  lcd.print("LCD Test :)");
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  delay(2000);
}

void loop() {
  int analogValue = analogRead(thermistorPin);
  float voltage = analogValue * (5.0 / 1023.0);
  float resistance = seriesResistor * voltage / (5.0 - voltage);

  float steinhart;
  steinhart = resistance / nominalResistance;
  steinhart = log(steinhart);
  steinhart /= bCoefficient;
  steinhart += 1.0 / (nominalTemp + 273.15);
  steinhart = 1.0 / steinhart;
  float tempC = steinhart - 273.15;

  if (tempC >= 32.0) {
    tone(buzzerPin, 1000);  // Play 1 kHz tone
    delay(1000);            // Play for 1 second
    noTone(buzzerPin);      // Stop sound
    delay(1000);   
  }
    digitalWrite(buzzerPin, LOW);   // Turn buzzer off
    lcd.display();                  // Turn LCD back on
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(tempC, 1);
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Volt: ");
    lcd.print(voltage, 2);
  
  delay(3000);
}
