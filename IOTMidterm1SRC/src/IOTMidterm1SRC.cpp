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


SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);


//Neopixel variables
const int PIXELCOUNT = 9;
const float MAXENCPOS = 96;
int color;
int startPixelTmp, endPixelTmp, startPixelHu, endPixelHu, startPixelWt, endPixelWt;

//Encoder variables
const int PINA=D8;
const int PINB=D9;
const int ENCSWITCH = D15;
bool switchState1, switchState2, switchClicked;
int encPos, lastEncPos;

//Load cell variables
const int DT=D5;
const int CLK = D6;
const int CALFACTOR = 26450; //temporary cal factor
const int SAMPLES = 10;
float weight, RawData, calibration;
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
const int WEMO1=1;
const int WEMO2=2;
const int WEMO3=3;
const int WEMO4=4;
const int WEMO5=5;
const int WEMO0=0;
bool onOffWemo;

//Hue light variables
const int BULB1=1;
const int BULB2=2;
const int BULB3=3;
const int BULB4=4;
const int BULB5=5;
const int BULB6=6;
const int SATURATION=255;
float brightness;
bool onOffHue;

Adafruit_NeoPixel pixel (PIXELCOUNT, SPI1, WS2812B);
Adafruit_BME280 bme;
const int OLED_RESET = -1;
Adafruit_SSD1306 display(OLED_RESET);
IoTTimer measTimer;
Button encSwitch(ENCSWITCH);
Encoder encDial (PINA, PINB);
HX711 snackScale(DT,CLK);

void pixelFill(int startPixel, int endPixel, int color);

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

  //snackScale.set_scale();
  //delay(5000);
  //snackScale.tare();
  //snackScale.set_scale(CALFACTOR);
  
  //pixel.begin();
  //pixel.setBrightness(35);
  //pixel.show();
  
}

/********************************************************
*********************************************************
*********************************************************/
void loop() {

 // if(measTimer.isTimerReady()){
    tempC=bme.readTemperature();
    tempF=(tempC*1.8) +32;
    humidRH=bme.readHumidity();
   // weight = snackScale.get_units(SAMPLES);
    delay(5000);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    //Don't forget to add weight to the display print
    display.clearDisplay();
    display.printf("Temperature: %0.1f %cF\nHumidity: %0.1f RH\n",tempF, degree, humidRH);
    Serial.printf("Temperature: %0.1f %cF\nHumidity: %0.1f RH\n",tempF, degree, humidRH);
    display.display();
   // measTimer.startTimer(TIMER);
    

   // if (encSwitch.isClicked){
   //   switchClicked = !switchClicked;
  //    Serial.printf("Encoder clicked %i\n,switchClicked")
  //  }
  /*
  ////Temperature Neopixels
    startPixelTmp=0;
    endPixelTmp= 2;
    if (tempF > 72.00){
    pixelFill(startPixelTmp, endPixelTmp, red);
    }

    
  ////Humidity Neopixels
    startPixelHu=3;
    endPixelHu=5;
    if (humidRH > 50.00){
      pixelFill(startPixelHu, endPixelHu, red);
    }
    else{
      pixelFill(startPixelHu, endPixelHu, green);
    }


  ////Weight Neopixels
    startPixelWt=6;
    endPixelWt=8;
    if(weight<=0.60){
      pixelFill(startPixelWt, endPixelWt, red);
    }
  }*/
}

/********************************************************
*********************************************************
*********************************************************/
void pixelFill(int startPixel, int endPixel, int color){
  int i;
  for(i=startPixel; i<=endPixel; i=i+1){
    pixel.setPixelColor(i, color);
  }
  pixel.show();
}

