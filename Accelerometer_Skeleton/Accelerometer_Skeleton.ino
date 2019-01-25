// Basic demo for accelerometer readings from Adafruit LIS3DH

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>




/* DIFFERENT BOARD?
#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif
*/



// I2C
Adafruit_LIS3DH lis = Adafruit_LIS3DH(); //creates object lis of LIS3DH class

void setup(void) {
/*
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
*/

  Serial.begin(9600);
//  Serial.println("LIS3DH test!");

/*
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
*/
  
 // Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G! WE NEED TO DECIDE
  
  Serial.print("Range = "); 
  Serial.print(2 << lis.getRange() ); // 2<< bitshift to make correct value display
  Serial.println("G");
  
}



int ledSpeed = 0; //delay speed for LED changing based on accelerometer

void loop() {
  
  lis.read();      // get X Y and Z data at once
  // Then print out the raw data
  Serial.print("X:  "); Serial.print(lis.x); 
  Serial.print("  \tY:  "); Serial.print(lis.y); 
  Serial.print("  \tZ:  "); Serial.print(lis.z); 


  /* Or....get a new sensor event, normalized */ 
  sensors_event_t event; 
  lis.getEvent(&event);  
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  Serial.print(" \tY: "); Serial.print(event.acceleration.y); 
  Serial.print(" \tZ: "); Serial.print(event.acceleration.z); 
  Serial.println(" m/s^2 ");


  Serial.println(); 
  delay(200); 


//MY CODE accelerometer data range -32768 -- 32767
// 32767/4 = 8191.75 so 8191 for each speed range
// ranges can be reduced from full spectrum as data is accuired
  if( (lis.x >= -8191) && (lis.x <= 8191) ){ //potentially use (abs(lis.x) <= 8191)
      ledSpeed = 500; //to be changed
    }

  else if( abs(lis.x) > 8191 && abs(lis.x) <= 16382 ){
      ledSpeed = 1000;
    }

  else if( abs(lis.x) > 16382 && abs(lis.x) <= 24573 ){
      ledSpeed = 1500;
    }

  else if ( abs(lis.x) > 24573 && abs(lis.x) <= 32767 ){
      ledSpeed = 2000;
    }

  else{
      Serial.print("ranging error");
    }

}
