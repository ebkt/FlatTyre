// serial output code and conversion from Joshua Noble's "Programming Interactivity: A Designer's Guide to
// Processing, Arduino, and openFrameworks" (p. 228-229, 2012)
// available at https://books.google.co.uk/books?id=sAsHA1HM1WcC&pg=PA225&lpg=PA225&dq=serial+input+from+arduino+openframeworks&source=bl&ots=KHz3UccApe&sig=mI6JR-iB4WiLT9Q-opHa-OZiNZQ&hl=en&sa=X&ved=0ahUKEwjmvtnD_I7aAhWBWhQKHW5EAKw4ChDoAQg1MAI#v=snippet&q=ofSerial&f=false

// Adafruit VL53L0X sensor code from example
// available at https://learn.adafruit.com/adafruit-vl53l0x-micro-lidar-distance-sensor-breakout/arduino-code


#include "Adafruit_VL53L0X.h"

// declare sensor and value variable
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int val;

void setup() {
    Serial.begin(115200);     // this is the only speed the sensor runs at
  
    while (! Serial) {        // wait until serial port opens for native USB devices
      delay(1);
    }
  
    // serial.print calls are commented out to avoid adding unwanted info to what is sent to openFrameworks, 
    // but are useful for debugging, so not removed completely
    
    //Serial.println("Adafruit VL53L0X test");
    if (!lox.begin()) {
      //Serial.println(F("Failed to boot VL53L0X"));
      while(1);
    }
    
    //Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
    
}

// ----------------------------------------------------------------------------
void loop() {
     VL53L0X_RangingMeasurementData_t measure;
  
     lox.rangingTest(&measure, false);    // pass in 'true' to get debug data printout!
  
     val = measure.RangeMilliMeter;       // set the value variable to the distance from the sensor
      
     if(Serial.available() > 0){
      Serial.read();
      printVal(val);
      }
   
  //  Serial.print("Reading a measurement... ");
  
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
  ///   Serial.print("Distance (mm): "); 
  //    Serial.println(val,'\n');
    } else {
  //    Serial.println(" out of range ");
    }
  
  //  Serial.println(val);
    delay(1);
  
}

// ----------------------------------------------------------------------------
void printVal(int val){
  
    // break the integer into two bytes
    // to be read and converted back into integers in openFrameworks
    
    byte highByte = ((val >> 8) & 0xFF); 
    byte lowByte = ((val >> 0) & 0xFF); 
  
    Serial.write(highByte);
    Serial.write(lowByte); 
}

