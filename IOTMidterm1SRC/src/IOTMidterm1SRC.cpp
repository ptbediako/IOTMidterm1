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


SYSTEM_MODE(SEMI_AUTOMATIC);

const int PIXELCOUNT = 9;
int color;
int startPixelTmp, endPixelTmp, startPixelHu, endPixelHu, startPixelWt, endPixelWt;

void pixelFill(int startPixel, int endPixel, int color);

Adafruit_NeoPixel pixel (PIXELCOUNT, SPI1, WS2812B);


void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected, 10000);
  pixel.begin();
  pixel.setBrightness(35);
  pixel.show();
  
}

void loop() {
//Temperature Neopixels
    startPixelTmp=0;
    endPixelTmp= 2;
    pixelFill(startPixelTmp, endPixelTmp, violet);

  
//Humidity Neopixels
    startPixelHu=3;
    endPixelHu=5;
    pixelFill(startPixelHu, endPixelHu, green);
 

//Weight Neopixels
    startPixelWt=6;
    endPixelWt=8;
    pixelFill(startPixelWt, endPixelWt, red);
 
}

void pixelFill(int startPixel, int endPixel, int color){
  int i;
  for(i=startPixel; i<=endPixel; i=i+1){
    pixel.setPixelColor(i, color);
  }
  pixel.show();

}


