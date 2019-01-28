/* File Name: POV_Main.ino
 * Author: Suyang Liu
 * Date: 1/24/2019
 * Revision: 0
 * Description:
 */

 #include <Adafruit_DotStar.h>
 #include <SPI.h>
 #include <Adafruit_LIS3DH.h>
 #include <Adafruit_Sensor.h>

 //#include <string.h>
 #include "letters.h"

 #define NUMPIXELS 18 //Number of LEDs in strip
 #define DATAPIN 11
 #define CLOCKPIN 14

 void printWord(String myWord, int delayTime, int delaySpace, int direction);
 void printCharacter(int character[][10], int delayTime, int red, int green, int blue, int charDirection);

 Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
 Adafruit_LIS3DH lis = Adafruit_LIS3DH();

 int delayTime = 1;
 int delaySpace = 5;
 int direction = 0;

 String myWord = "HEY"; //must be in all caps

 void setup() {
   //configure LED strip
   strip.begin(); // Initialize pins for output
   strip.show();  // Initialize LEDs to off
   strip.setBrightness(64); //value from 0 to 255

   //initialize accelerometer
   lis.begin(0x18);

   //configure accelerometer
   lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G

   randomSeed(analogRead(0)); //initialize random seed using value from pin 0
 }

 void loop() {
   lis.read();
   if (lis.y <= -8191 && direction == 0) { //-2G right acceleration
     //start
     delay(delaySpace);
     printWord(myWord, delayTime, delaySpace, direction);
   }
   else if (lis.y >= 8191 && direction == 0) { //+2G left decceleration
     direction = 1;
   }
   else if (lis.y >= 8191 && direction == 1) { //+2G left acceleration
     //start reverse
     delay(delaySpace);
     printWord(myWord, delayTime, delaySpace, direction);
   }
   else if (lis.y <= -8191 && direction == 1) { //-2G right decceleration
     direction = 0;
   }
 }

 void printWord(String myWord, int delayTime, int delaySpace, int direction) {
   int red = 255/*random(255)*/;
   int green = 0/*random(255)*/;
   int blue = 255/*random(255)*/;

   for (unsigned int i = 0; i < myWord.length(); i++) {
     if (myWord[i] >= 65 && myWord[i] <= 90) { //check if letter
       delay(delaySpace);
       printCharacter(charArray[myWord[i] - 65], delayTime, green, red, blue, direction);
     }
     else if (myWord[i] >= 48 && myWord[i] <= 57) { //check if num
       delay(delaySpace);
       printCharacter(charArray[myWord[i] - 48], delayTime, green, red, blue, direction);
     }
   }
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
