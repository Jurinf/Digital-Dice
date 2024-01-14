#include "I2Cdev.h"
#include <arduino.h>
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define OUTPUT_READABLE_ACCELGYRO

#define LED_PIN 13
bool blinkState = false;

#define buzzer 12

MPU6050 mpu;

const int Empfindlichkeit = 100;
int segmentPins[] = { 8, 9, 3, 4, 5, 7, 6 };
bool motion = true;
int number = 1;

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(buzzer, OUTPUT);
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  Serial.begin(38400);
  mpu.initialize();
  pinMode(LED_PIN, OUTPUT);
  displayNumber(1);
}

void loop() {

  motion_detaction();
  sevseg_disliay();
}

void motion_detaction() {

  pinMode(buzzer, OUTPUT);
  // Lese die Beschleunigungsdaten
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Berechne die Gesamtbeschleunigung
  int16_t totalAcceleration = abs(ax) + abs(ay) + abs(az);

  if (totalAcceleration > Empfindlichkeit) {
    motion = false;
  } else {
    motion = true;
  }

  delay(100);
}

void displayNumber(int num) {
  byte numbers[][7] = {
    { 0, 1, 1, 0, 0, 0, 0 },
    { 1, 1, 0, 1, 1, 0, 1 },
    { 1, 1, 1, 1, 0, 0, 1 },
    { 0, 1, 1, 0, 0, 1, 1 },
    { 1, 0, 1, 1, 0, 1, 1 },
    { 1, 0, 1, 1, 1, 1, 1 }
  };

  // Übertrage die Segmente für die gewünschte Zahl auf die Pins

  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], numbers[num - 1][i]);
  }
}

void sevseg_disliay() {

  if (motion == true) {
    for (int i = 0; i < 6; i++) {
      for (int j = 1; j <= 6; j++) {
        displayNumber(j);
        delay(20);
      }

      number = random(1, 7);
      tone(buzzer, 900, 10);
      motion = false;
    }
  } else if (motion == false) {
    displayNumber(number);
  }
}