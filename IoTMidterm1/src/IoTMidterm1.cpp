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

const int PIXELCOUNT = 10;
int j, i, k;
int colorIndex, color;
int startPixel, endPixel;

void pixelFill(int startPixel, int endPixel, int color);

Adafruit_NeoPixel pixel (PIXELCOUNT, SPI1, WS2812B);


void setup() {
  pixel.begin();
  pixel.setBrightness(35);
  pixel.show();
  
}

void loop() {
  //for(i=0; i<=10; i++){
    startPixel=0;
    endPixel= 10;
    pixelFill(startPixel, endPixel, violet);
  //}
}

void pixelFill(int startPixel, int endPixel, int color){
  for(j=startPixel; j<=endPixel; j=j+1){
    pixel.setPixelColor(j, color);
  }
  pixel.show();
  //delay(100);
}
