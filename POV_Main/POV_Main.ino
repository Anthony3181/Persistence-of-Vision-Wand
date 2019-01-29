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

 /**********************
  * Variable Definitions and Object Instantiations
  * ********************/

 #define NUMPIXELS 18 //Number of LEDs in strip
 #define DATAPIN 11
 #define CLOCKPIN 14

 int delayTime = 1;
 int delaySpace = 5;
 int hLocation = 0; //Initializes horizontal location marker.
 float speed; //Initializes speed variable
 bool displayEn;
 unsigned long waveStart;
 int16_t last20[20];

 string myWord = "HEY"; //must be in all caps

 bool myWordArray = [myWord.length()][18][12];

 Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

 Adafruit_LIS3DH lis = Adafruit_LIS3DH();


 /*****************
  * Function Definitions
  * ***************/

/*********************
 * Funciton Name: 
 * 
 *    printWord
 * 
 * Parameters:
 * 
 *    string myWord, int delayTime, it delaySpace, int direction
 * 
 * Returns: 
 * 
 *    Nothing
 * 
 * Function Description: 
 * 
 * Takes in the word and delay time delay space and direction formats the word into an array the LED strip can display.
 * 
 * ********************/

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
     else if(myWord[i] == 32){ //check if space

     }
   }
 }


void printCharacter(int character[][12], int delayTime, int red, int green, int blue, int charDirection) {
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



 void printWord(String myWord, int delayTime, int delaySpace, int direction);
 void printCharacter(int character[][10], int delayTime, int red, int green, int blue, int charDirection);





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
   updateLast20(last20);
   leftInterruptEvent(&waveStart,&displayEn,&hLocation);
  if(displayEnable)
 }

 

 
