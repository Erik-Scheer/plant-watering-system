#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define SENSOR_PIN 33
#define MOTOR_PIN 27
#define LED_PIN 33

int sensorRange = 5000;
int ledRange = 25;


int waitingUntilRemeasure = 10000;
int wateringTime = 1000;


#define PIN         33    // Verwendeter GPIO-Pin
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

  for (int i = 0; i < numberLED ; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 50, 0));
  }

Serial.println(analogRead(SENSOR_PIN));


  if (analogRead(SENSOR_PIN) <= 500){
    for (int i = 0; i < 1 ; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 50, 0));
    pixels.show();
    }
  }
  else{
    digitalWrite(MOTOR_PIN, LOW);
  }

  if (analogRead(SENSOR_PIN) >= 2000){
    digitalWrite(MOTOR_PIN, HIGH);
    delay(wateringTime);
    digitalWrite(MOTOR_PIN, LOW);
    delay(waitingUntilRemeasure);
  }
  else{
    digitalWrite(MOTOR_PIN, LOW);
  }
}
