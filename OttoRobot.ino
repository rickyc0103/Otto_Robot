/*
   Otto robot DIY
   for Arduino Workshop @Yuan Ze University 2017.05.04
   combine servo motor control, ultrasonic obstacle detection, and buzzer
   author: Po-Chiang Lin (email: pochianglin@gmail.com)
   originally made by Camilo Parra Palacio
   http://otto.strikingly.com/
   license CC-BY-SA
*/

#include <Servo.h>
#include <SoftwareSerial.h>

// HC-05 Bluetooth module
SoftwareSerial BTSerial(11, 12); // RX | TX
// SoftwareSerial BTSerial(10, 11); // RX | TX

// pin assignment
const int servoUpperLeftPin = 2;
const int servoUpperRightPin = 3;
const int servoLowerLeftPin = 4;
const int servoLowerRightPin = 5;
const int ultrasonicTrigPin = 8;
const int ultrasonicEchoPin = 9;
const int buzzerPin = 10;
// const int buzzerPin = 12;
// const int bluetoothPin = 6;

// servo motors
Servo servoUpperLeft;
Servo servoUpperRight;
Servo servoLowerLeft;
Servo servoLowerRight;

// for ultrasonic distance calculation
const int interval = 100;
const float soundMetersPerSecond = 343.2;
float soundMicrosecondsPerCentimeter = 1 / (soundMetersPerSecond * 100 / 1000000);
int distanceObstacleDetection = 10;  // cm

// calibrate to stand with attention
int middlePositionServoUpperLeft = 90;
int middlePositionServoUpperRight = 85;
int middlePositionServoLowerLeft = 85;
int middlePositionServoLowerRight = 95;

// angle for walking
int rightTurnIn = 20;
int rightTurnOut = 25;
int leftTurnIn = 20;
int leftTurnOut = 25;
int heelAngle = 27;
int tiptoeAngle = 55;

void setup()
{
  // Serial.begin(9600);

  pinMode(ultrasonicTrigPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  stand();
  delay(1000);
//  pinMode(bluetoothPin, OUTPUT);
//  delay(1000);
//  digitalWrite(bluetoothPin, HIGH);
//  delay(1000);
  BTSerial.begin(115200);  // HC-05 default speed in AT command more
  delay(100);  
}

void loop()
{
  float duration, distance;

  digitalWrite(ultrasonicTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTrigPin, LOW);
  duration = pulseIn (ultrasonicEchoPin, HIGH, 1000000);
  distance = (duration / 2) / soundMicrosecondsPerCentimeter;
//  if ((distance > 0) && (distance < 400)) {
//    Serial.print("d = ");
//    Serial.print(distance);
//    Serial.println(" cm");
//  }
//  else {
//    Serial.println("Nothing within range!");
//  }
  // delay(interval);

  if ((distance > 0) && (distance < distanceObstacleDetection)) {
//    BTSerial.print("Distance = ");
//    BTSerial.print(distance);
//    BTSerial.println("cm");
    // alarmBeep(buzzerPin);
    // BTSerial.write("Distance = ");
    // BTSerial.write(&String(distance));
    // BTSerial.write("cm\n");
    // turnLeft();
    // turnLeft();
  } else {
    if (BTSerial.available()) {
      switch (BTSerial.read()) {
        case 'w':
          walk();
          break;
        case 'l':
          turnLeft();
          break;
        case 's':
          stand();
          break;
        case 'b':
          alarmBeep(buzzerPin);
          break;
        case 'd':
          dance();
          break;
        case 'a':
          sing();
          break;
        default:
          stand();
          break;
      }
      //      if (BTSerial.read() == 'w') {
      //        walk();
      //      }
    } else {
      stand();
    }
  }
  // delay(500);
}

void stand() {
  servoUpperLeft.attach(servoUpperLeftPin);
  servoUpperRight.attach(servoUpperRightPin);
  servoLowerLeft.attach(servoLowerLeftPin);
  servoLowerRight.attach(servoLowerRightPin);
  
  servoUpperLeft.write(middlePositionServoUpperLeft);
  servoUpperRight.write(middlePositionServoUpperRight);
  servoLowerLeft.write(middlePositionServoLowerLeft);
  servoLowerRight.write(middlePositionServoLowerRight);
  
//  servoUpperLeft.detach();
//  servoUpperRight.detach();
//  servoLowerLeft.detach();
//  servoLowerRight.detach();
}

void walk() {
  servoUpperLeft.attach(servoUpperLeftPin, 500, 2400);
  servoUpperRight.attach(servoUpperRightPin, 500, 2400);
  servoLowerLeft.attach(servoLowerLeftPin, 500, 2400);
  servoLowerRight.attach(servoLowerRightPin, 500, 2400);

  // left step
  for (int i = middlePositionServoLowerLeft; i <= middlePositionServoLowerLeft + tiptoeAngle + 10; i += 1) {
    servoLowerLeft.write(i);
    delay(5);
  }
  delay(200);

  for (int i = middlePositionServoLowerRight; i <= middlePositionServoLowerRight + heelAngle + 10; i += 1) {
    servoLowerRight.write(i);
    delay(5);
  }
  delay(500);

  for (int i = middlePositionServoLowerLeft + tiptoeAngle + 10; i >= middlePositionServoLowerLeft; i -= 1) {
    servoLowerLeft.write(i);
    delay(5);
  }
  delay(200);

//   for (int i = middlePositionServoUpperLeft + leftTurnIn; i >= middlePositionServoUpperLeft - leftTurnOut; i -= 1) {
  for (int i = middlePositionServoUpperLeft; i >= middlePositionServoUpperLeft - leftTurnOut; i -= 1) {
    servoUpperLeft.write(i);
    delay(5);
  }
  delay(200);

//  for (int i = middlePositionServoUpperRight + rightTurnOut; i >= middlePositionServoUpperRight - rightTurnIn; i -= 1) {
  for (int i = middlePositionServoUpperRight; i >= middlePositionServoUpperRight - rightTurnIn; i -= 1) {
    servoUpperRight.write(i);
    delay(5);
  }
  delay(200);

  for (int i = middlePositionServoLowerRight + heelAngle + 10; i >= middlePositionServoLowerRight; i -= 1) {
    servoLowerRight.write(i);
    delay(10);
  }
  delay(200);

  // right step
  for (int i = middlePositionServoLowerRight; i >= middlePositionServoLowerRight - tiptoeAngle; i -= 1) {
    servoLowerRight.write(i);
    delay(5);
  }
  delay(200);

  for (int i = middlePositionServoLowerLeft; i >= middlePositionServoLowerLeft - heelAngle - 10; i -= 1) {
    servoLowerLeft.write(i);
    delay(5);
  }
  delay(500);

  for (int i = middlePositionServoLowerRight - tiptoeAngle; i <= middlePositionServoLowerRight; i += 1) {
    servoLowerRight.write(i);
    delay(5);
  }
  delay(200);

  for (int i = middlePositionServoUpperRight - rightTurnIn; i <= middlePositionServoUpperRight + rightTurnOut; i += 1) {
    servoUpperRight.write(i);
    delay(5);
  }
  delay(200);

  for (int i = middlePositionServoUpperLeft - leftTurnOut; i <= middlePositionServoUpperLeft + leftTurnIn; i += 1) {
    servoUpperLeft.write(i);
    delay(5);
  }
  delay(200);

  for (int i = middlePositionServoLowerLeft - heelAngle; i <= middlePositionServoLowerLeft; i += 1) {
    servoLowerLeft.write(i);
    delay(10);
  }
  delay(200);

  servoUpperLeft.detach();
  servoUpperRight.detach();
  servoLowerLeft.detach();
  servoLowerRight.detach();
}

void turnLeft() {
  stand();

  servoUpperLeft.attach(servoUpperLeftPin, 500, 2400);
  servoUpperRight.attach(servoUpperRightPin, 500, 2400);
  servoLowerLeft.attach(servoLowerLeftPin, 500, 2400);
  servoLowerRight.attach(servoLowerRightPin, 500, 2400);

  // left step
  for (int i = middlePositionServoLowerLeft; i <= middlePositionServoLowerLeft + tiptoeAngle; i += 1) {
    servoLowerLeft.write(i);
    delay(5);
  }
  delay(100);

  for (int i = middlePositionServoLowerRight; i <= middlePositionServoLowerRight + heelAngle; i += 1) {
    servoLowerRight.write(i);
    delay(5);
  }
  delay(200);

  for (int i = middlePositionServoLowerLeft + tiptoeAngle; i >= middlePositionServoLowerLeft; i -= 1) {
    servoLowerLeft.write(i);
    delay(5);
  }
  // delay(100);

  for (int i = middlePositionServoUpperLeft; i >= middlePositionServoUpperLeft - leftTurnOut; i -= 1) {
    servoUpperLeft.write(i);
    delay(5);
  }
  // delay(100);

  for (int i = middlePositionServoUpperRight; i <= middlePositionServoUpperRight + rightTurnOut; i += 1) {
    servoUpperRight.write(i);
    delay(5);
  }
  delay(100);

  for (int i = middlePositionServoLowerRight + heelAngle; i >= middlePositionServoLowerRight; i -= 1) {
    servoLowerRight.write(i);
    delay(10);
  }
  delay(100);

  // right step
  for (int i = middlePositionServoLowerRight; i >= middlePositionServoLowerRight - tiptoeAngle; i -= 1) {
    servoLowerRight.write(i);
    delay(5);
  }
  delay(100);

  for (int i = middlePositionServoLowerLeft; i >= middlePositionServoLowerLeft - heelAngle - 10; i -= 1) {
    servoLowerLeft.write(i);
    delay(5);
  }
  delay(200);

  for (int i = middlePositionServoLowerRight - tiptoeAngle; i <= middlePositionServoLowerRight; i += 1) {
    servoLowerRight.write(i);
    delay(5);
  }
  // delay(100);

  for (int i = middlePositionServoUpperRight + rightTurnOut; i >= middlePositionServoUpperRight; i -= 1) {
    servoUpperRight.write(i);
    delay(5);
  }
  // delay(100);

  for (int i = middlePositionServoUpperLeft - leftTurnOut; i <= middlePositionServoUpperLeft; i += 1) {
    servoUpperLeft.write(i);
    delay(5);
  }
  delay(100);

  for (int i = middlePositionServoLowerLeft - heelAngle; i <= middlePositionServoLowerLeft; i += 1) {
    servoLowerLeft.write(i);
    delay(10);
  }
  delay(100);

  servoUpperLeft.detach();
  servoUpperRight.detach();
  servoLowerLeft.detach();
  servoLowerRight.detach();
}

void alarmBeep(int pin) {
  int i;

  for (i = 500; i <= 2000; i += 100) {
    tone(pin, i, 20);
    delay(30);
  }

  for (i = 2000; i >= 500; i -= 100) {
    tone(pin, i, 20);
    delay(30);
  }
}

void dance() {
    servoUpperLeft.attach(servoUpperLeftPin, 500, 2400);
  servoUpperRight.attach(servoUpperRightPin, 500, 2400);
  servoLowerLeft.attach(servoLowerLeftPin, 500, 2400);
  servoLowerRight.attach(servoLowerRightPin, 500, 2400);

  // put your main code here, to run repeatedly:
  for (int i = 90; i <= 150; i++) {
    servoLowerLeft.write(i);
    delay(5);
  }
  for (int i = 90; i >= 30; i--) {
    servoLowerRight.write(i);
    delay(5);
  }
  for (int i = 150; i >= 90; i--) {
    servoLowerLeft.write(i);
    delay(5);
  }
  for (int i = 30; i <= 90; i++) {
    servoLowerRight.write(i);
    delay(5);
  }

  servoUpperLeft.detach();
  servoUpperRight.detach();
  servoLowerLeft.detach();
  servoLowerRight.detach();
}

void sing() {
  // "Suger"
  tone(buzzerPin, 1047, 500);
  delay(500);
  
  tone(buzzerPin, 880, 1000);
  delay(1000);
  delay(500);
  tone(buzzerPin, 1047, 500);
  delay(500);
  
  tone(buzzerPin, 880, 1000);
  delay(1000);
  delay(250);
  tone(buzzerPin, 784, 250);
  delay(250);
  tone(buzzerPin, 880, 250);
  delay(250);
  
  tone(buzzerPin, 1319, 500);
  delay(500);
  tone(buzzerPin, 1175, 250);
  delay(250);
  tone(buzzerPin, 1175, 250);
  delay(250);
  tone(buzzerPin, 1047, 250);
  delay(250);
  tone(buzzerPin, 1047, 250);
  delay(250);
  tone(buzzerPin, 1175, 500);
  delay(500);
  tone(buzzerPin, 1047, 1250);
  delay(1250);
  delay(500);
}

