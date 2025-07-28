#include <Servo.h>

// Pins
const int trigPin = 8;
const int echoPin = 9;
const int warningPin = 10;
const int safePin = 11;
const int servoPin = 13;
const int buzzerPin = 4;

// Variables
long duration;
int distance;
Servo myServo;

const int threshold = 40;
const int servoMin = 15;
const int servoMax = 165;

// Timing variables
unsigned long lastLEDUpdate = 0;
const unsigned long warningDelay = 500;
const unsigned long safeDelay = 200;

bool tunePlayed = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(warningPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(safePin, OUTPUT);

  Serial.begin(9600);
  myServo.attach(servoPin);
}

void loop() {
  // Sweep from min to max
  for (int angle = servoMin; angle <= servoMax; angle++) {
    myServo.write(angle);
    delay(25);
    distance = calculateDistance();
    updateLEDs(distance);
    
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" | Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  // Sweep from max to min
  for (int angle = servoMax; angle >= servoMin; angle--) {
    myServo.write(angle);
    delay(5);
    distance = calculateDistance();
    updateLEDs(distance);

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" | Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}

// Updates LEDs and plays Star theme
void updateLEDs(int d) {
  unsigned long now = millis();

  if (d <= threshold && now - lastLEDUpdate >= warningDelay) {
    digitalWrite(warningPin, HIGH);
    delay(80);
    // tone(buzzerPin, 2500, 500);
    digitalWrite(safePin, LOW);
    lastLEDUpdate = now;

    if (!tunePlayed) {
      playStarWars(false);
      tunePlayed = true;
    }
  } 
  else if (d > threshold && now - lastLEDUpdate >= safeDelay) {
    digitalWrite(warningPin, LOW);
    digitalWrite(safePin, HIGH);
    lastLEDUpdate = now;
    tunePlayed = false; // Reset when safe
  }
}

// Calculates distance with ultrasonic sensor
int calculateDistance() {
  long total = 0;
  const int samples = 3;

  for (int i = 0; i < samples; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    total += duration * 0.034 / 2;
    delay(10);
  }

  return total / samples;
}

// Star war theme
void playStarWars(bool play) {
  if (play == true) {


  
  int melody[] = {
    440, 440, 440, 349, 523, 440, 349, 523, 440, 
    659, 659, 659, 698, 523, 415, 349, 523, 440
  };

  int noteDurations[] = {
    500, 500, 500, 350, 150, 500, 350, 150, 1000,
    500, 500, 500, 350, 150, 500, 350, 150, 1000
  };

  for (int i = 0; i < sizeof(melody)/sizeof(int); i++) {
    int note = melody[i];
    int duration = noteDurations[i];
    tone(buzzerPin, note, duration);
    delay(duration * 1.3);
  }
  }
  else if(play == false) {

  tone(buzzerPin, 2500, 400);
  delay(duration * 1.3);

  }


  noTone(buzzerPin);
}
