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
 float speed = 0; //Initializes speed variable
 bool displayEn = 0;
 unsigned long waveStartTime;
 int16_t last100[100];

 String myWord = "HEY"; //must be in all caps
 const int wordLength = 3;


 short myWordArray[wordLength][18][12];

 Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

 Adafruit_LIS3DH lis = Adafruit_LIS3DH();


 /*****************
  * Function Definitions
  * ***************/

/*************************
 * Function Name:
 *    writeWordArray
 * 
 * Parameters:
 *    Short array
 *    String
 * 
 * Returns:
 *    Nothing
 * 
 * Description:
 *    Takes the populates an array of booleans base off the value of a given string. 
 * ********************/

void writeWordArray(short wordArray[wordLength][18][12], String myWord){
   for (unsigned int i = 0; i < wordLength; i++) {
   if (myWord[i] >= 65 && myWord[i] <= 90) { //check if letter
     for(int j = 0; j < 18; ++j){
       for(int k = 0; k < 12; ++k){
         wordArray[i][j][k] = charArray[myWord[i]-65][j][k];
         Serial.print(wordArray[i][j][k]);
       }
       Serial.println();
     }
   }
   else if (myWord[i] >= 48 && myWord[i] <= 57) { //check if num
     for(int j = 0; j < 18; ++j){
       for(int k = 0; k < 12; ++k){
         wordArray[i][j][k] = charArray[myWord[i]-48+26][j][k]; //biased start of array at character 26 in letters.h
         Serial.print(wordArray[i][j][k]);
       }
       Serial.println();
     }
   }
   else if(myWord[i] == 32)
   { //check if space
    for(int j = 0; j < 18; ++j)
    {
       for(int k = 0; k < 12; ++k){
         wordArray[i][j][k] = charArray[myWord[i]-32+36][j][k]; //biased start of array at character 36 in letters.h
         Serial.print(wordArray[i][j][k]);
       }
       Serial.println();
    }
   }
   }
}


/******************
 * Function Name:
 * 
 *    serialPrintWordArray
 * 
 * Parameters:
 *    3D Short array
 * 
 * Returns:
 *    Nothing
 * 
 * Function Description:
 *    Prints the word array in the serial monitor, written to test the writeWordArray function.
 * 
 * ******************/

void serialPrintWordArray(short wordArray[wordLength][18][12]){
  for(int i = 0; i < wordLength; ++i){
    for(int j = 0; j < 18; ++j){
      for(int k = 0; k < 12; ++k){
        Serial.print(wordArray[i][j][k]);
      }
      Serial.println();
    }
  }
}


/******************
 * Function Name:
 * 
 *    updatelast100
 * 
 * Parameters:
 *    uint_t array, uint_t acceleration value
 * 
 * Returns:
 *    Nothing
 * 
 * Function Description:
 *    Update the array of the last 20 acceleration values by shifting the entire array by one and adding the new 
 *    value to the 0th index.
 * 
 * ******************/

void updatelast100(int16_t last100[100], int16_t aValue){
    for(int i = 99; i > 0; --i){
      last100[i] = last100[i-1];
    }
    last100[0] = aValue;
    //Uncomment this section to test funcitonality.
    Serial.print('[');
    for(int i = 0; i < 99; ++i){
      Serial.print(last100[i] / 100);
      Serial.print(',');
    }
    Serial.println(']');
    
}


/******************
 * Function Name:
 * 
 *    checkWaveStart
 * 
 * Parameters:
 *    int16_t array, bool* , unsigned long*
 * 
 * Returns:
 *    Nothing
 * 
 * Function Description:
 *    Checks to see if the wand is being waved and that it is at a local minimum value. If these conditions are met
 *    the function will record the time of start and enable the display.
 * 
 * ******************/

void checkWaveStart(int16_t last100[100], bool* displayEn, unsigned long* waveStartTimep){
  float first = last100[0] / 100;
  float middle = last100[50] / 100;
  float last = last100[99] / 100;
  //Serial.print(first);
  //Serial.print(' ');
  //Serial.print(middle);
  //Serial.print(' ');  
  //Serial.println(last); 
  if(middle < first && middle < last){
    *waveStartTimep = millis();
    *displayEn = 1;
    Serial.println("Wave Start Triggered");
    Serial.println(*waveStartTimep);
    Serial.println(*displayEn);
  }
}



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


/*
void printWord(String myWord, int delayTime, int delaySpace, int direction) {
   int red = 255;
   int green = 0;
   int blue = 255;

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
 }*/





 void setup() {

   Serial.begin(9600);
   //configure LED strip
   strip.begin(); // Initialize pins for output
   strip.show();  // Initialize LEDs to off
   strip.setBrightness(64); //value from 0 to 255

   //initialize accelerometer
   lis.begin(0x18);

   //configure accelerometer
   lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G

   //populate display array
  writeWordArray(myWordArray, myWord);


   randomSeed(analogRead(0)); //initialize random seed using value from pin 0
 }

 void loop() {
    lis.read();
    if(!displayEn){
      updatelast100(last100, lis.y);
      checkWaveStart(last100, &displayEn, &waveStartTime);
      delay(20);
    }
    
 }

 

 
