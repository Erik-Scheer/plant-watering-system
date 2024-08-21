#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>  
#include <Wire.h> 

#define SENSOR_PIN 33
#define MOTOR_PIN 27
#define LED_PIN 32

int sensorRange = 5000;
int ledRange = 25;
int sensorDryOffset = 1700;
int sensorWetOffset = 5000 - 3500;

int pumpSensorLimit = 2000;


int waitingUntilRemeasure = 10000;
int wateringTime = 1000;


int pumpTimeStamp = -10000;
bool pumpIsOn = false;

#define PIN         32    // Verwendeter GPIO-Pin
#define NUMPIXELS   25    // Anzahl an LEDs in Strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


// display
#define DISP_ADDR 0x78
#define DISP_SDA 21 
#define DISP_SCL 22 

U8G2_SSD1306_128X64_NONAME_1_SW_I2C display(U8G2_R0, DISP_SCL, DISP_SDA);



void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pixels.begin();    
  pixels.setBrightness(125);


  // display
  
  display.setI2CAddress(DISP_ADDR);  
  display.begin();  
  
}


void loop() {
  // Serial.println(analogRead(SENSOR_PIN));

  int sensorInput = analogRead(SENSOR_PIN);

  int newSensorInput = sensorRange - sensorInput;

  int numberLED = ((newSensorInput - sensorDryOffset) * ledRange) / (sensorRange - sensorDryOffset - sensorWetOffset);

  // int numberLED = ((sensorInput + sensorOffset) * ledRange) / (sensorRange - sensorOffset);
  Serial.println(numberLED + "--------------------------------------------------------------------------------------------------------");

  pixels.clear();

  for (int i = 0; i < (numberLED) ; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 50, 0));
  }
  pixels.show();

Serial.println(newSensorInput);
// Serial.println(numberLED);

  if (analogRead(SENSOR_PIN) >= pumpSensorLimit){

    if (!pumpIsOn) {
      if (pumpTimeStamp + waitingUntilRemeasure < millis()){
        digitalWrite(MOTOR_PIN, HIGH);
        pumpIsOn = true;
        pumpTimeStamp = millis();
      }
    }
    else {
      if (pumpTimeStamp + wateringTime < millis()){
        digitalWrite(MOTOR_PIN, LOW);
        pumpIsOn = false;
        pumpTimeStamp = millis();
      }
    }

  }
  else{
    digitalWrite(MOTOR_PIN, LOW);
  }


  // int humidityPercentage = ((newSensorInput - sensorDryOffset) * 100) / (sensorRange - sensorDryOffset - sensorWetOffset);
  float humidityPercentage = ((newSensorInput - sensorDryOffset) * 100 * 1000) / (sensorRange - sensorDryOffset - sensorWetOffset);
  humidityPercentage /= 1000;
  
  if (humidityPercentage > 100) humidityPercentage = 100;
  if (humidityPercentage < 0) humidityPercentage = 0;

  // float humidityPercentage = sensorInput / sensorRange;
  Serial.print("humidity percenatage: ");
  Serial.println(humidityPercentage);
  Serial.print("new SensorInput: ");
  Serial.println(newSensorInput);

  String outputDisplayString1 = "Humidity:";
  String outputDisplayString2 = (String(humidityPercentage) + "%");
  if (outputDisplayString2.length() == 5) {
    outputDisplayString2 = "  " + outputDisplayString2;
  }
  if (outputDisplayString2.length() == 6) {
    outputDisplayString2 = " " + outputDisplayString2;
  }

  // display loop
  display.firstPage();  // Initialisierung Picture-Loop
    do {
        /* Hier Anzeigeinhalt frei definieren */
        display.setFont(u8g2_font_ncenB14_tr);  // optional: gewünschte Schriftart
        // display.print(analogRead(SENSOR_PIN));
        display.drawStr( 10,20,(outputDisplayString1.c_str()));
        display.drawStr( 30,50,(outputDisplayString2.c_str()));
        // display.drawStr(0, 20, "Hello World!");
    } while (display.nextPage());  // Sobald Inhalt angezeigt: Verlassen der Schleife
    /* Ende Picture-Loop */
}



