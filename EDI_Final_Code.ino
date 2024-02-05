#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "your_blynk_auth_token";
char ssid[] = "your_wifi_ssid";
char pass[] = "your_wifi_password";

const int trigPin[] = {2, 3, 4, 5, 6};
const int echoPin[] = {7, 8, 9, 10, 11};
const int irSensorLeft = A0;
const int irSensorCenter = A1;
const int irSensorRight = A2;
const int leftMotorPin1 = 13;
const int leftMotorPin2 = 12;
const int rightMotorPin1 = 10;
const int rightMotorPin2 = 9;

Servo steeringServo;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  for (int i = 0; i < 5; i++) {
    pinMode(trigPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
  }
  pinMode(irSensorLeft, INPUT);
  pinMode(irSensorCenter, INPUT);
  pinMode(irSensorRight, INPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  steeringServo.attach(11);
}

void loop() {
  Blynk.run();

  int distances[5];
  for (int i = 0; i < 5; i++) {
    distances[i] = getDistance(trigPin[i], echoPin[i]);
  }

  int irLeft = digitalRead(irSensorLeft);
  int irCenter = digitalRead(irSensorCenter);
  int irRight = digitalRead(irSensorRight);

  if (distances[0] < 20 || distances[1] < 20 || distances[2] < 20 || distances[3] < 20 || distances[4] < 20) {
    stopMotors();
    delay(500);
    turnRight();
    sendAccidentNotification();
  } else {
    if (irLeft == LOW && irCenter == HIGH && irRight == LOW) {
      forward();
    } else if (irLeft == HIGH && irCenter == LOW && irRight == HIGH) {
      stopMotors();
    } else if (irLeft == LOW && irCenter == LOW && irRight == HIGH) {
      turnRight();
    } else if (irLeft == HIGH && irCenter == LOW && irRight == LOW) {
      turnLeft();
    } else if (irLeft == LOW && irCenter == HIGH && irRight == HIGH) {
      slightLeft();
    } else if (irLeft == HIGH && irCenter == HIGH && irRight == LOW) {
      slightRight();
    }
  }
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

void forward() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
}

void stopMotors() {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, LOW);
}

void turnLeft() {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
}

void turnRight() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
}

void slightLeft() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
}

void slightRight() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
}

void sendAccidentNotification() {
  Blynk.email("recipient@example.com", "Accident Detected", "Emergency! Accident detected. Please respond immediately.");
}
