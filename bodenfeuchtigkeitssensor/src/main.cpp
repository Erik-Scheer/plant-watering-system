#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define SENSOR_PIN 33
#define MOTOR_PIN 27
#define LED_PIN 32

int sensorRange = 5000;
int ledRange = 25;


int waitingUntilRemeasure = 10000;
int wateringTime = 1000;


#define PIN         32    // Verwendeter GPIO-Pin
#define NUMPIXELS   25    // Anzahl an LEDs in Strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);




void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pixels.begin();    
  pixels.setBrightness(125);
  
}


void loop() {
  Serial.println(analogRead(SENSOR_PIN));

  int sensorInput = analogRead(SENSOR_PIN);

  int numberLED = (sensorInput * ledRange) / sensorRange;
  Serial.println(numberLED + "--------------------------------------------------------------------------------------------------------");

  pixels.clear(); 

  for (int i = 0; i < (25 - numberLED) ; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 50, 0));
  }
  pixels.show(); 

Serial.println(sensorInput);
Serial.println(numberLED);


  // if (analogRead(SENSOR_PIN) <= 500){
    
  // }
  // else{
  //   digitalWrite(MOTOR_PIN, LOW);
  // }

  if (analogRead(SENSOR_PIN) >= 2000){
    digitalWrite(MOTOR_PIN, HIGH);
    delay(wateringTime);
    digitalWrite(MOTOR_PIN, LOW);
    delay(waitingUntilRemeasure);
    delay(wateringTime);
    digitalWrite(MOTOR_PIN, LOW);
    delay(waitingUntilRemeasure);
  }
  else{
    digitalWrite(MOTOR_PIN, LOW);
  }
}
