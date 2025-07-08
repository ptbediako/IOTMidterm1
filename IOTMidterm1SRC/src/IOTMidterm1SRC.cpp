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


//SYSTEM_MODE(MANUAL);
SYSTEM_MODE(SEMI_AUTOMATIC);
//SYSTEM_THREAD(ENABLED);


//Neopixel variables
const int PIXELCOUNT = 3;
int color;
int startPixelTmp, endPixelTmp, startPixelHu, endPixelHu, startPixelWt, endPixelWt;
const int TMPPIXEL = 0;
const int HUPIXEL = 1;
const int WTPIXEL= 2;

//Encoder variables
const int PINA=D8;
const int PINB=D9;
const int ENCSWITCH = D15;
const float MAXENCPOS = 96;
bool switchState1, switchState2, switchClicked;
int encPos, lastEncPos;

//Load cell variables
const int DT=D5;
const int CLK = D4;
const int CALFACTOR = 790; //temporary cal factor
const int SAMPLES = 10;
float weight, weight2, RawData, calibration;
int offset;

//BME variables
float tempC, tempF, humidRH;
float tempSet, humidSet;
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
bool onOffWemo;

//Hue light variables
const int BULB1=1;
const int BULB2=2;
const int BULB3=3;
const int BULB4=4;
const int BULB5=5;
const int BULB6=6;
const int MAXBRITE=255;
const int SATURATION=255;
float brightness;
bool onOffHue;

Adafruit_NeoPixel pixel (PIXELCOUNT, SPI1, WS2812B);
Adafruit_BME280 bme;
const int OLED_RESET = -1;
Adafruit_SSD1306 display(OLED_RESET);
IoTTimer measTimer;
Button eSwitch(ENCSWITCH);
Encoder encDial (PINA, PINB);
HX711 snackScale(DT,CLK);

//void pixelFill(int startPixel, int endPixel, int color);

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
  
  // WiFi.on();
  // WiFi.clearCredentials();
  // WiFi.setCredentials("IoTNetwork");
  
  // WiFi.connect();
  // while(WiFi.connecting()) {
  //   Serial.printf(".");
  // }
  
  // Serial.printf("\n\n");


  pixel.begin();
  pixel.setBrightness(35);
  pixel.show();
  
}

/********************************************************
*********************************************************
*********************************************************/
void loop() {
  //if (eSwitch.isClicked()){
    //switchClicked = !switchClicked;
    //Serial.printf("Encoder clicked %i\n", switchClicked);
    
    //if(switchClicked){
  // if(measTimer.isTimerReady()){
      tempC=bme.readTemperature();
      tempF=(tempC*1.8) +32;
      humidRH=bme.readHumidity();
      weight = snackScale.get_units(SAMPLES);
      weight2=weight;
      if (weight < 0){
        weight2 = 0;
      }
      delay(3000);

      encPos= encDial.read();
      if(encDial.read()>95){
      encDial.write(encPos=95);
      }
      if(encDial.read()<0){
        encDial.write(encPos=0);
      }
      
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.clearDisplay();
  //    display.printf("Temperature: %0.1f %cF\nHumidity: %0.1f RH\n",tempF, degree, humidRH);
  //    Serial.printf("Temperature: %0.1f %cF\nHumidity: %0.1f RH\n",tempF, degree, humidRH);
      display.printf("Temperature: %0.1f %cF\nHumidity: %0.1f RH\nWeight: %0.1f g\n",tempF, degree, humidRH, weight2);
      Serial.printf("Temperature: %0.1f %cF\nHumidity: %0.1f RH\nWeight: %0.1f g\n",tempF, degree, humidRH, weight2);
      display.display();
    // measTimer.startTimer(TIMER);
      
    
    ////Temperature Neopixels
      //startPixelTmp=0;
      //endPixelTmp= 2;
      if ((tempF < 74.8) || (tempF > 75.1)){
        pixel.setPixelColor(TMPPIXEL,red);
        pixel.show();
        //pixelFill(startPixelTmp, endPixelTmp, red);
        //  wemoWrite(WEMO0,HIGH);
        //  wemoWrite(WEMO1,HIGH);
        //  wemoWrite(WEMO2,HIGH);
        //  wemoWrite(WEMO3,HIGH);
        //  wemoWrite(WEMO4,HIGH);
        //  wemoWrite(WEMO5,HIGH);
      }
      else{
        pixel.setPixelColor(TMPPIXEL,green);
        pixel.show();
        //pixelFill(startPixelTmp, endPixelTmp, green);
        //  wemoWrite(WEMO0,LOW);
        //  wemoWrite(WEMO1,LOW);
        //  wemoWrite(WEMO2,LOW);
        //  wemoWrite(WEMO3,LOW);
        //  wemoWrite(WEMO4,LOW);
        //  wemoWrite(WEMO5,LOW);
      }

      
    ////Humidity Neopixels
      //startPixelHu=3;
      //endPixelHu=5;
      if ((humidRH < 42.8) || (humidRH > 50.1)){
        pixel.setPixelColor(HUPIXEL,red);
        pixel.show();
        //pixelFill(startPixelHu, endPixelHu, red);
      }
      else{
        pixel.setPixelColor(HUPIXEL,green);
        pixel.show();        
        //pixelFill(startPixelHu, endPixelHu, green);
      }


    ////Weight Neopixels
      //startPixelWt=6;
      //endPixelWt=8;

      if(weight2 < 180){
        pixel.setPixelColor(WTPIXEL,red);
        pixel.show();
      //  Serial.printf("Turning on Hue# %i\n",BULB6);
      //  setHue(BULB6,TRUE,HueRainbow[red],brightness,SATURATION);
      //  setHue(BULB4,TRUE,HueRainbow[red],brightness,SATURATION);

       if(encPos != lastEncPos){
        Serial.printf("Encoder position %i\n",encPos);
        brightness=encPos*(MAXBRITE/MAXENCPOS);
        lastEncPos = encPos; 
        }
      }
        else {
        pixel.setPixelColor(WTPIXEL,green);
        pixel.show();
      //  Serial.printf("Turning on Hue# %i\n",BULB6);
      //  setHue(BULB6,FALSE);//,green,brightness,SATURATION);
      //  setHue(BULB4,FALSE);//,green,brightness,SATURATION);
      }
      

    //}
  //}
}

/********************************************************
*********************************************************
*********************************************************
void pixelFill(int startPixel, int endPixel, int color){
  int i;
  for(i=startPixel; i<=endPixel; i=i+1){
    pixel.setPixelColor(i, color);
  }
  pixel.show();
}
*/
