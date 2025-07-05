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

//Scanning for BME, OLED, maybe Load Cell Amp
byte status, address, nDevices;

SYSTEM_MODE(SEMI_AUTOMATIC);

const int PIXELCOUNT = 9;
const float MAXENCPOS = 96;

const int PINA=D8;
const int PINB=D9;

const int ENCSWITCH = D15;

bool switchState1, switchState2, switchClicked;

int color;
int startPixelTmp, endPixelTmp, startPixelHu, endPixelHu, startPixelWt, endPixelWt;
int encPos, lastEncPos;

void pixelFill(int startPixel, int endPixel, int color);

Adafruit_NeoPixel pixel (PIXELCOUNT, SPI1, WS2812B);
Button encSwitch(ENCSWITCH);


void setup() {
  Wire.begin();
  Serial.begin(9600);
  waitFor(Serial.isConnected, 10000);
  Serial.printf("I2C Scanner\n") //part of scanning for I2C devices

  //pixel.begin();
  //pixel.setBrightness(35);
  //pixel.show();
  
}

void loop() {
  Serial.printf("Starting scan: \n-------------\n");
  nDevices=0;
  for(address=0; address <=127; address++){
    Wire.beginTransmission(address);
    status=Wire.endTransmission();
    if(status == 0);
  }
  if (encSwitch.isClicked()){
    switchClicked = !switchClicked;

  }
////Temperature Neopixels
  startPixelTmp=0;
  endPixelTmp= 2;
  pixelFill(startPixelTmp, endPixelTmp, violet);

  
////Humidity Neopixels
  startPixelHu=3;
  endPixelHu=5;
  if (humidity > 50.00()){
    pixelFill(startPixelHu, endPixelHu, red);
  }
  else{
    pixelFill(startPixelHu, endPixelHu, green);
  }


////Weight Neopixels
  startPixelWt=6;
  endPixelWt=8;
  if(weight<=2.00()){
    pixelFill(startPixelWt, endPixelWt, red);
  }

void pixelFill(int startPixel, int endPixel, int color){
  int i;
  for(i=startPixel; i<=endPixel; i=i+1){
    pixel.setPixelColor(i, color);
  }
  pixel.show();

}


