/* 
 * Project Midterm 1 Smart Room Controller
 * Author: Phylicia Bediako
 * Date: 07/08/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "neopixel.h"
#include "Colors.h"
#include "Button.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_BME280.h"
#include "IoTTimer.h"
#include "Encoder.h"
#include "HX711.h"
#include "IoTClassroom_CNM.h"


SYSTEM_MODE(MANUAL);
//SYSTEM_MODE(SEMI_AUTOMATIC);

//Neopixel variables
const int PIXELCOUNT = 3;
int color;
const int TMPPIXEL = 0;
const int HUPIXEL = 1;
const int WTPIXEL= 2;

//Encoder variables
const int PINB=D8;
const int PINA=D9;
const int ENCSWITCH = D15;
const float MAXENCPOS = 96;
int encPos, lastEncPos;

//Load cell variables
const int DT=D5;
const int CLK = D4;
const int CALFACTOR = 790; //temporary cal factor
const int SAMPLES = 10;
float weight, lastWgtTime;

//BME variables
float tempC, tempF, humidRH;
bool status;
const char degree = 0xF8;
const int BME280=0x76;
const int OLEDSCRN=0x3c;

//WEMO variables
const int WEMO0=0;
const int WEMO1=1;
const int WEMO2=2;
const int WEMO3=3;
const int WEMO4=4;
const int WEMO5=5;
int WEMONUM;

//Hue light variables
const int BULB1=1;
const int BULB2=2;
const int BULB3=3;
const int BULB4=4;
const int BULB5=5;
const int BULB6=6;
int BULBNUM;
const int MAXBRITE=255;
const int SATURATION=255;
float brightness;


Adafruit_NeoPixel pixel (PIXELCOUNT, SPI1, WS2812B);
Adafruit_BME280 bme;
const int OLED_RESET = -1;
Adafruit_SSD1306 display(OLED_RESET);
Encoder encDial (PINA, PINB);
HX711 snackScale(DT,CLK);

/********************************************************
*********************************************************
*********************************************************/
void setup() {
  Wire.begin();
  Serial.begin(9600);
  waitFor(Serial.isConnected, 10000);

  status=bme.begin(BME280);
  if (status == false){
    Serial.printf("BME280 at address 0x%02x failed to start",BME280);
  }
  display.begin(SSD1306_SWITCHCAPVCC,OLEDSCRN);
  display.display();
  display.clearDisplay();
  
  snackScale.set_scale();
  delay(5000);
  snackScale.tare();
  snackScale.set_scale(CALFACTOR);
  
  WiFi.on();
  WiFi.clearCredentials();
  WiFi.setCredentials("IoTNetwork");
  
  WiFi.connect();
  while(WiFi.connecting()) {
    Serial.printf(".");
  }
  
  Serial.printf("\n\n");

  pixel.begin();
  pixel.setBrightness(35);
  pixel.show();

  //start the hue lights at a medium brightness level for when they first turn on (this can be adjusted below)
  brightness=50*(MAXBRITE/MAXENCPOS); 
  encDial.write(50);
}

/********************************************************
*********************************************************
*********************************************************/
void loop() {
  tempC=bme.readTemperature();
  tempF=(tempC*1.8) +32; //convert temperature reading to Celsius
  humidRH=bme.readHumidity();

  //Create a time buffer between weight readings without using a "delay"
  if((millis()-lastWgtTime) > 3000){
    weight = snackScale.get_units(SAMPLES);
    if (weight < 0){
      weight = 0;
    }
    lastWgtTime=millis();
  }

  //Establish encoder dial related variables so turning the dial changes the brightness of the hue lights
  brightness=encPos*(MAXBRITE/MAXENCPOS);
  encPos= encDial.read();
  if(encDial.read()>95){
    encDial.write(encPos=95);
  }
  
  if(encDial.read()<0){
    encDial.write(encPos=0);
  }
  
  if(encPos != lastEncPos){
    Serial.printf("Encoder position %i\n",encPos);
    lastEncPos = encPos; 
  }

  //Programming the OLED screen to show the environment and weight readings
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.printf("Temperature: %0.1f %cF\nHumidity: %0.1f RH\nWeight: %0.1f g\n",tempF, degree, humidRH, weight);
  Serial.printf("Temperature: %0.1f %cF\nHumidity: %0.1f RH\nWeight: %0.1f g\n",tempF, degree, humidRH, weight);
  display.display();
  
  //Program the neopixels, hue lights, and wemo outlets so they depict temperature, humidity, and weight status

  ////Temperature: Establish a range of acceptable temperatures, turn neopixel green if within range and turn off ODD numbered wemos
  if ((tempF > 73.0) && (tempF < 75.0)){ 
    pixel.setPixelColor(TMPPIXEL,green);
    pixel.show();
    for(WEMONUM=0; WEMONUM < 6; WEMONUM++){
      if(WEMONUM%2 ==1){
        wemoWrite(WEMONUM,LOW);
      }
    }
  }
  //Turn neopixel red if out of range and turn on odd numbered wemos
  else{
    pixel.setPixelColor(TMPPIXEL,red);
    pixel.show();
    for(WEMONUM=0; WEMONUM < 6; WEMONUM++){
      if(WEMONUM%2 ==1){
        wemoWrite(WEMONUM, HIGH);
      }
    }
  }

////Humidity: Establish a range of acceptable humidity values, turn neopixel green if within range and turn off EVEN numbered wemos
  if ((humidRH > 64.8) && (humidRH < 66.0)){
    pixel.setPixelColor(HUPIXEL,green);
    pixel.show();
    
    for(WEMONUM=0; WEMONUM < 6; WEMONUM++){
      if(WEMONUM%2 ==0){
        wemoWrite(WEMONUM, LOW);
      }
    }
  }

else{
    pixel.setPixelColor(HUPIXEL,red);
    pixel.show();
    for(WEMONUM=0; WEMONUM < 6; WEMONUM++){
      if(WEMONUM%2 ==0){
      wemoWrite(WEMONUM, HIGH);
      }
    }
  }

////Weight: Set a minimum weight for the scale, change the neopixel and hue lights green or red based on the weight of remaining snacks
  if(weight < 200){
    pixel.setPixelColor(WTPIXEL,red);
    pixel.show();
    for (BULBNUM=1; BULBNUM<7; BULBNUM++){
      setHue(BULBNUM,TRUE,HueRed,brightness,SATURATION);
      delay(200);
    }
  }
  
    else {
    pixel.setPixelColor(WTPIXEL,green);
    pixel.show();
    for (BULBNUM=1; BULBNUM<7; BULBNUM++){
      brightness=encPos*(MAXBRITE/MAXENCPOS);
      setHue(BULBNUM,TRUE,HueGreen,brightness,SATURATION);
      delay(200);
    }
  }
}

