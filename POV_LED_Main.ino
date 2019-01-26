/*  File Name: POV_LED_Main
 *  Author: Suyang Liu
 *  Date: 1/24/2019
 *  Revision: 0
 *  Description:
 */

#include <Adafruit_DotStar.h>
#include <SPI.h>
#include "letters.h"

#define NUMPIXELS 18 //Number of LEDs in strip
#define DATAPIN 11
#define CLOCKPIN 14

void printCharacter(int character[][10], int delayTime, int red, int green, int blue, int charDirection);

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

void setup() {
  strip.begin(); // Initialize pins for output
  strip.show();  // Initialize LEDs to off
  strip.setBrightness(255);

  randomSeed(analogRead(0)); //initialize random seed using value from pin 0
}

void loop() {
  int red = 255/*random(255)*/;
  int green = 0/*random(255)*/;
  int blue = 0/*random(255)*/;
  int delayTime = 2;
  
  delay(10);
  printCharacter(letterH, delayTime, green, red, blue, 0);
  delay(10);
  printCharacter(letterE, delayTime, green, red, blue, 0);
  delay(10);
  printCharacter(letterY, delayTime, green, red, blue, 0);
  
  delay(10);
  printCharacter(letterH, delayTime, green, red, blue, 1);
  delay(10);
  printCharacter(letterE, delayTime, green, red, blue, 1);
  delay(10);
  printCharacter(letterY, delayTime, green, red, blue, 1);
}

void printCharacter(int character[][10], int delayTime, int red, int green, int blue, int charDirection) {
  if (!charDirection) {
    for (int i = 0; i < 10; i++) {
      for (int j = 17; j >= 0; j--) {
        if (character[j][i] == 1) {
          strip.setPixelColor(abs(j-17), red, green, blue);
        }
        else {
          strip.setPixelColor(abs(j-17), 0);
        }
      }
      strip.show();
      delay(delayTime);
    }
    
    //reset to 0
    for(int i = 0; i < 18; i++) {
      strip.setPixelColor(i, 0);
    }
    strip.show();
    delay(delayTime);
  }
  else {
    for (int i = 9; i >= 0; i--) {
      for (int j = 17; j >= 0; j--) {
        if (character[j][i] == 1) {
          strip.setPixelColor(abs(j-17), red, green, blue);
        }
        else {
          strip.setPixelColor(abs(j-17), 0);
        }
      }
      strip.show();
      delay(delayTime);
    }
    //reset to 0
    for(int i = 0; i < 18; i++) {
      strip.setPixelColor(i, 0);
    }
    strip.show();
    delay(delayTime);
  }
}
