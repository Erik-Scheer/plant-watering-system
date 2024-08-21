#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
// display
#include <U8g2lib.h>  
#include <Wire.h> 

#define SENSOR_PIN 33
#define MOTOR_PIN 27
#define LED_PIN 32

int sensorRange = 5000;
int ledRange = 25;


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




  // display loop
  display.firstPage();  // Initialisierung Picture-Loop
    do {
        /* Hier Anzeigeinhalt frei definieren */
        display.setFont(u8g2_font_ncenB14_tr);  // optional: gewünschte Schriftart
        // display.print(analogRead(SENSOR_PIN));
        display.drawStr( 0,20,String(analogRead(SENSOR_PIN)).c_str());
        // display.drawStr(0, 20, "Hello World!");
    } while (display.nextPage());  // Sobald Inhalt angezeigt: Verlassen der Schleife
    /* Ende Picture-Loop */
}
