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
//SYSTEM_THREAD(ENABLED);


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
const int TIMER = 500;
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
//int HueRainbow[] = {HueRed, HueOrange, HueYellow, HueGreen, HueBlue, HueIndigo, HueViolet};


Adafruit_NeoPixel pixel (PIXELCOUNT, SPI1, WS2812B);
Adafruit_BME280 bme;
const int OLED_RESET = -1;
Adafruit_SSD1306 display(OLED_RESET);
IoTTimer measTimer;
Button eSwitch(ENCSWITCH);
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
  //measTimer.startTimer();

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

  brightness=50*(MAXBRITE/MAXENCPOS);
  encDial.write(50);
}

/********************************************************
*********************************************************
*********************************************************/
void loop() {

  // if(measTimer.isTimerReady()){
  tempC=bme.readTemperature();
  tempF=(tempC*1.8) +32;
  humidRH=bme.readHumidity();

  if((millis()-lastWgtTime) > 3000){
    weight = snackScale.get_units(SAMPLES);
    if (weight < 0){
      weight = 0;
    }
    lastWgtTime=millis();
  }

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

      
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.printf("Temperature: %0.1f %cF\nHumidity: %0.1f RH\nWeight: %0.1f g\n",tempF, degree, humidRH, weight);
  Serial.printf("Temperature: %0.1f %cF\nHumidity: %0.1f RH\nWeight: %0.1f g\n",tempF, degree, humidRH, weight);
  display.display();
  
  ////Temperature Neopixels

  if ((tempF > 73.0) && (tempF < 75.0)){
  //else{
    pixel.setPixelColor(TMPPIXEL,green);
    pixel.show();
    for(WEMONUM=0; WEMONUM < 6; WEMONUM++){
      if(WEMONUM%2 ==1){
        wemoWrite(WEMONUM,LOW);
      }
    }
  }

  else{
  //if ((tempF < 74.0) || (tempF > 75.1)){
    pixel.setPixelColor(TMPPIXEL,red);
    pixel.show();
    for(WEMONUM=0; WEMONUM < 6; WEMONUM++){
      if(WEMONUM%2 ==1){
        wemoWrite(WEMONUM, HIGH);
      }
    }
  }

////Humidity Neopixels
  //else{
  if ((humidRH > 64.8) && (humidRH < 66.0)){
    pixel.setPixelColor(HUPIXEL,green);
    pixel.show();
    
    for(WEMONUM=0; WEMONUM < 6; WEMONUM++){
      if(WEMONUM%2 ==0){
        wemoWrite(WEMONUM, LOW);
      }
    }
  }
//if ((humidRH < 42.8) || (humidRH > 60.1)){
else{
    pixel.setPixelColor(HUPIXEL,red);
    pixel.show();
    for(WEMONUM=0; WEMONUM < 6; WEMONUM++){
      if(WEMONUM%2 ==0){
      wemoWrite(WEMONUM, HIGH);
      }
    }
  }




////Weight Neopixels
  if(weight < 200){ //at CALFACTOR XX, two nutrigrain bars in the box registered at min ~88g
    //and two sandwich crackers in the box registered at min ~83g, neither went over 100g
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

