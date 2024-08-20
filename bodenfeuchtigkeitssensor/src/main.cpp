#include <Arduino.h>

#define SENSOR_PIN 33
#define MOTOR_PIN 27


int waitingUntilRemeasure = 10000;
int wateringTime = 1000;


void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {

  
  Serial.println(analogRead(SENSOR_PIN));
  if (analogRead(SENSOR_PIN) >= 2000){
    digitalWrite(MOTOR_PIN, HIGH);
    delay(wateringTime)
    digitalWrite(MOTOR_PIN, LOW)
    delay(waitingUntilRemeasure)
  }
  else{
    digitalWrite(MOTOR_PIN, LOW);
  }
}
