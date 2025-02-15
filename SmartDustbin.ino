#include <Servo.h>

Servo servo1;

// Pin Definitions
const int trigPin = 12;
const int echoPin = 11;
const int potPin = A0;

// Variables
long duration;
int distance = 0;
int soil = 0;
int fsoil = 0;

// Calibration Constants
int maxDryValue = 10;      
int Ultra_Distance = 12;   

// Setup
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo1.attach(8);
  servo1.write(90); // Ensure servo is in a neutral position
  Serial.println("Soil Sensor     Ultrasonic          Servo");
}

void loop() {
  distance = getDistance(); // Get distance using ultrasonic sensor

  if (distance < Ultra_Distance && distance > 1) {
    delay(500); // Reduced delay to make it faster

    fsoil = getSoilMoisture(); // Get soil moisture value

    Serial.print("Humidity: ");
    Serial.print(fsoil);
    Serial.print("%    Distance: ");
    Serial.print(distance);
    Serial.print(" cm");

    if (fsoil > maxDryValue) { // Wet waste
      Serial.println("     ==> WET Waste ");
      servo1.write(170); // Move servo to the wet waste bin
    } else { // Dry waste
      Serial.println("     ==> Dry Waste ");
      servo1.write(10); // Move servo to the dry waste bin
    }

    delay(2000); // Allow time for servo to complete its movement
    servo1.write(90); // Reset servo to neutral position
  }

  delay(500); // Reduced delay for faster readings
}

// Function to Get Distance from Ultrasonic Sensor
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Calculate distance in cm
}

// Function to Get Average Soil Moisture Value
int getSoilMoisture() {
  int totalSoil = 0;

  for (int i = 0; i < 3; i++) {
    soil = analogRead(potPin);
    soil = constrain(soil, 485, 1023); // Calibrate sensor readings
    totalSoil += map(soil, 485, 1023, 100, 0); // Convert to percentage
    delay(50); // Reduced delay for faster sampling
  }

  return totalSoil / 3; // Return average soil moisture value
  
}
