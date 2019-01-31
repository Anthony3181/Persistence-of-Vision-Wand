/* File Name: POV_Main.ino
 * Author: Suyang Liu
 * Date: 1/24/2019
 * Revision: 0
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
  * Variable Definitions and Object Instantiations
  * ********************/

 typedef enum
 {
   NO_SWING,
   SPEED_1,
   SPEED_2,
   SPEED_3
 } swing_speed_t;

 typedef enum
 {
   LEFT,
   RIGHT
 } direction_t;

 #define NUMPIXELS 18 //Number of LEDs in strip
 #define DATAPIN 11
 #define CLOCKPIN 14

 int delayTime = 1;
 int delaySpace = 5;
 int hLocation = 0; //Initializes horizontal location marker.
 float yVelocity = 0; //Initializes Y velocity variable
 swing_speed_t speed = NO_SWING;
 direction_t direction = LEFT;
 unsigned long lastUpdate = 0; //Initializes the variable that will track when the timer was last updated.
 int dT = 7; //Initializes the dT (delta T) variable. Unit is milliseconds
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
 *    checkWaveStart
 * 
 * Parameters:
 *    int16_t array, bool*
 * 
 * Returns:
 *    Nothing
 * 
 * Function Description:
 *    Checks to see if the wand is being waved and that it is at a local minimum value. If these conditions are met
 *    the function will record the time of start and enable the display.
 * 
 * ******************/

void checkWaveStart(int16_t last100[100], swing_speed_t* speed, direction_t* direction){
  float first = last100[0] / 100;
  float middle = last100[50] / 100;
  float last = last100[99] / 100;
  if(middle < first && middle < last){
    *direction = RIGHT;
    *speed = SPEED_1;
    Serial.println("Wand has started waving.");
    Serial.println("Wand is moving right.");
  }
}

/******************
 * Function Name:
 * 
 *    checkWaveStop
 * 
 * Parameters:
 *    int16_t array, bool*, float*
 * 
 * Returns:
 *    Nothing
 * 
 * Function Description:
 *    Checks to see if the wand has stopped waving. If this condition is met
 *    it sets the speed to NO_SWING.
 * 
 * ******************/


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

//TODO: Create boolean value to allow the inturupt to only happen once per peak.

void updateTiming(int16_t last100[100], swing_speed_t* speed, direction_t* direction){
  int16_t first = last100[0];
  int16_t middle = last100[25];
  int16_t last = last100[50];
  //Serial.println(first);
  if(middle < first && middle < last && *direction == LEFT){
    *direction = RIGHT;
    //Serial.println("Wand is moving right.");
    if(abs(first) > 8000 && abs(first) < 12000){
      *speed = SPEED_1;
     // Serial.println("Wand is at speed threshold 1.");
    } else if (abs(first) > 12000){
     *speed = SPEED_2;
      //Serial.println("Wand is at speed threshold 2.");
    } else {
      *speed = NO_SWING;
     //Serial.println("Wand is not waving fast enough.");
    }
  } else if (middle > first && middle > last && *direction == RIGHT){
    *direction = LEFT;
    //Serial.println("Wand is moving left.");
    if(abs(first) > 8000 && abs(first) < 12000){
     *speed = SPEED_1;
     //Serial.println("Wand is at speed threshold 1.");
    } else if (abs(first) > 12000){
      *speed = SPEED_2;
      //Serial.println("Wand is at speed threshold 2.");
    } else {
      *speed = NO_SWING;
     //Serial.println("Wand is not waving fast enough.");
    }
  }
}




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
    if(millis() - lastUpdate >= dT){    //This creates a non-blocking framrate timer. The dT limits the framrate to 1000/dT FPS
      lis.read();
      lastUpdate = millis();
      updatelast100(last100, lis.y);
      //Serial.println(lis.y);
       if(abs(lis.y) > 8000){
        updateTiming(last100, &speed, &direction);
        Serial.print(speed);
        Serial.print(' ');
        Serial.print(direction);
        Serial.print(' ');
        float lisData = lis.y / 10000;
        Serial.println(lisData);
       }
      
    }
}

 

 
