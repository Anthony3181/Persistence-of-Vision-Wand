/* File Name: POV_Main.ino
 * Author: Suyang Liu, Jordan Baxter, Anthony Bishop
 * Date: 1/1/2019
 * Description:
 */

 /**********************
  * Include Files
  * ********************/
 #include <Adafruit_DotStar.h>
 #include <SPI.h>
 #include <Adafruit_LIS3DH.h>
 #include <Adafruit_Sensor.h>


 #include "letters.h"

  /**********************
  * User Config
  * ********************/

 String myWord = "HIRE US"; //must be in all caps
 const int wordLength = 7;


 /**********************
  * Variable Definitions and Object Instantiations
  * ********************/

 typedef enum {
   NO_SWING,
   SPEED_1,
   SPEED_2
 } swing_speed_t;

 typedef enum {
   LEFT,
   RIGHT
 } direction_t;

 #define NUMPIXELS 18 //Number of LEDs in strip
 #define DATAPIN 11
 #define CLOCKPIN 14

 //int delayTime = 1;
 //int delaySpace = 5;
 swing_speed_t speed = NO_SWING;
 direction_t direction = LEFT;
 unsigned long lastUpdate = 0; //Initializes the variable that will track when the timer was last updated.
 unsigned long lastDirectionChange = 0; // remove maybe?
 unsigned int dT = 7; //Initializes the dT (delta T) variable. Unit is milliseconds
 int16_t last100[100];


 const int wordArrayLength = wordLength*12;


 short myWordArray[wordArrayLength][18];

 Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

 Adafruit_LIS3DH accel = Adafruit_LIS3DH();


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
 *    Takes the populates a 2D array of booleans based off the values of a given string. All Commented sections are test functions.
 * ********************/

void writeWordArray(short (*wordArray)[wordArrayLength][18], String myWord){
  int length = 0;
  for (int i = 0; i < wordLength; i++) {
    //Serial.println(length);
    if (myWord[i] >= 65 && myWord[i] <= 90) { //check if letter
     for (int j = 0; j < 18; ++j) {
       for (int k = 0; k < 12; ++k) {
         (*wordArray)[k+length][j] = charArray[myWord[i]-65][j][k];
         //Serial.print(wordArray[i][j][k]);
       }
       //Serial.println();
     }
     length += 12;
    }
    else if (myWord[i] >= 48 && myWord[i] <= 57) { //check if num
      //Serial.println(length);
      for (int j = 0; j < 18; ++j) {
       for (int k = 0; k < 12; ++k) {
         (*wordArray)[k+length][j] = charArray[myWord[i]-48+26][j][k]; //biased start of array at character 26 in letters.h
         //Serial.print(wordArray[i][j][k]);
       }
       //Serial.println();
      }
      length += 12;
    }
    else if(myWord[i] == 32) { //check if space
      //Serial.println(length);
      for (int j = 0; j < 18; ++j) {
        for (int k = 0; k < 12; ++k) {
         (*wordArray)[k+length][j] = charArray[myWord[i]-32+36][j][k]; //biased start of array at character 36 in letters.h
         //Serial.print(wordArray[i][j][k]);
       }
       //Serial.println();
      }
      length += 12;
    }
  }
}


/******************
 * Function Name:
 *
 *    serialPrintWordArray
 *
 * Parameters:
 *    2D Short Integer array
 *
 * Returns:
 *    Nothing
 *
 * Function Description:
 *    Prints the word array in the serial monitor, written to test the writeWordArray function. All Commented sections are test functions.
 *
 * ******************/
void serialPrintWordArray(short myWordArray[][18]){
  //Serial.println(wordArrayLength);
  for (int i = 0; i < wordArrayLength; i++) {
    for (int j = 17; j >= 0; j--) {
      Serial.print(myWordArray[i][j]);
    }
    Serial.println();
  }
  /*for(int i = 0; i < wordLength; ++i){
    for(int j = 0; j < 18; ++j){
      for(int k = 0; k < 12; ++k){
        Serial.print(wordArray[i][j][k]);
      }
      Serial.println();
    }
  }*/
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
 *    value to the 0th index. All Commented sections are test functions.
 *
 * ******************/

void updatelast100(int16_t last100[100], int16_t aValue){
    for(int i = 99; i > 0; --i){
      last100[i] = last100[i-1];
    }
    last100[0] = aValue;
    //Uncomment this section to test funcitonality.
    // Serial.print('[');
    // for(int i = 0; i < 99; ++i){
    //   Serial.print(last100[i] / 100);
    //   Serial.print(',');
    // }
    // Serial.println(']');

}

/******************
 * Function Name:
 *
 *    updateTiming
 *
 * Parameters:
 *    int16_t, swing_speed_t*, direction_t*
 *
 * Returns:
 *    Nothing
 *
 * Function Description:
 *    Updates direction and swing speed threshold.
 *
 * ******************/


void updateTiming(int16_t last100[100], swing_speed_t* speed, direction_t* direction){
  int noiseFilter = 50;
  int16_t first = last100[0];
  int16_t middle = last100[10];
  int16_t last = last100[20];
  //Serial.println(first);
  if(middle < first - noiseFilter/3 && middle < last - noiseFilter && *direction == LEFT && millis() - lastDirectionChange > 100){
    *direction = RIGHT;
    lastDirectionChange = millis(); //remove maybe?
    //Serial.println("Wand is moving right.");
    if(abs(first) > 8000 && abs(first) < 19000){
      *speed = SPEED_1;
     // Serial.println("Wand is at speed threshold 1.");
   } else if (abs(first) > 19000){
     *speed = SPEED_2;
      //Serial.println("Wand is at speed threshold 2.");
    } else {
      *speed = NO_SWING;
     //Serial.println("Wand is not waving fast enough.");
    }
  } else if (middle > first + noiseFilter/3 && middle > last + noiseFilter && *direction == RIGHT && millis() - lastDirectionChange > 100){
    *direction = LEFT;
    lastDirectionChange = millis();
    //Serial.println("Wand is moving left.");
    if(abs(first) > 8000 && abs(first) < 19000){
     *speed = SPEED_1;
     //Serial.println("Wand is at speed threshold 1.");
   } else if (abs(first) > 19000){
      *speed = SPEED_2;
      //Serial.println("Wand is at speed threshold 2.");
    } else {
      *speed = NO_SWING;
     //Serial.println("Wand is not waving fast enough.");
    }
  }
}

void printWord(short myWordArray[][18]) {
  int red = 17;
  int green = 79;
  int blue = 150;

  int delayTime = 0;

  if (speed == SPEED_1) {
    delayTime = 2;
  }
  else {
    delayTime = 0.5;
  }

  delay(delayTime);
  if (direction == RIGHT) {
    for (int i = 0; i < wordArrayLength; i++) {
      for (int j = 17; j >= 0; j--) {
        if (myWordArray[i][j] == 1) {
          strip.setPixelColor(abs(j-17), red, green, blue);
        }
        else {
          strip.setPixelColor(abs(j-17), 0, 0, 0);
        }
      }
      strip.show();
      delay(delayTime);
    }
  }
  else {
    for (int i = wordArrayLength; i > 0; i--) {
      for (int j = 17; j >= 0; j--) {
        if (myWordArray[i][j] == 1) {
          strip.setPixelColor(abs(j-17), red, green, blue);
        }
        else {
          strip.setPixelColor(abs(j-17), 0, 0, 0);
        }
      }
      strip.show();
      delay(delayTime);
    }
  }

  //This sections ensures all of the LED's are turned off when the word finishes printing.
  for (int i = 0; i < 18; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}


void setup() {

   Serial.begin(9600);
   //configure LED strip
   strip.begin(); // Initialize pins for output
   strip.show();  // Initialize LEDs to off
   strip.setBrightness(80); //value from 0 to 255

   //initialize accelerometer
   accel.begin(0x18);

   //configure accelerometer
   accel.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G

   //populate display array
   writeWordArray(&myWordArray, myWord);
   //serialPrintWordArray(myWordArray);

   randomSeed(analogRead(0)); //initialize random seed using value from pin 0
}

void loop() {
    if(millis() - lastUpdate >= dT){    //This creates a non-blocking sampling and framrate timer. The dT limits the rate to dT/1000 Hz (~140Hz for dT = 7)
      accel.read();
      lastUpdate = millis();
      updatelast100(last100, accel.y);
      //Serial.println(accel.y);
       if(abs(accel.y) > 8000){
        updateTiming(last100, &speed, &direction);
        Serial.print(speed);
        Serial.print(' ');
        Serial.println(direction);
        /*Serial.print(' ');
        float accelData = accel.y;
        Serial.println(accelData);*/


        //PRINT
        if (speed != NO_SWING) {
          printWord(myWordArray);
          speed = NO_SWING;
        }
       }
       //print here?

    }
}
