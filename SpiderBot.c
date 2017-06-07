/*
  Test code for spiderBot
*/

#include "simpletools.h"                  

i2c *eeBus;
void setupServoController(const i2c *eeBus);
void setServoAmount(const i2c *eeBus, uint16_t rotation, uint8_t location);

int main()
{
  eeBus = i2c_newbus(28,  29,   0); // Set up I2C bus
  setupServoController(eeBus);
  while(1){ // Test code
    for(int counter = 0;counter <= 180;counter += 1){
      setServoAmount(eeBus, counter, 0);
      setServoAmount(eeBus, (counter + 90) % 180, 1);
      sleep(1); 
    }
  }     
}

// Setup the servo controller PCA9685 with Auto increment and a 50 hz freq.
// @param eeBus The bus the controller is on.
void setupServoController(const i2c *eeBus){
  const uint8_t controllerAddress = 0x40;
  const uint8_t freq = 121; //(25Mhz / (4096* frq)) - 1.
  const uint8_t sleep = 0x10;
  const uint8_t autoInc = 0x20;
  const uint8_t addressFrequency = 0xFE;
  const uint8_t addressMode1 = 0x00;
  uint8_t writeChar = 0;
  writeChar |= sleep;
  i2c_out(eeBus, controllerAddress, addressMode1, 1, &writeChar, sizeof(writeChar));   // Enable sleep to set the freq register.
  i2c_out(eeBus, controllerAddress, addressFrequency, 1, &freq, sizeof(freq));   // Set the freq for the servos.
  writeChar = 0 | autoInc;
  i2c_out(eeBus, controllerAddress, addressMode1, 1, &writeChar, sizeof(writeChar)); // Set sleep off (default state) and enable auto increment.
}

// Set the servo from 0 to 180 degrees.
// @param eeBus The bus the controller is on.
// @param rotation 0 to 180 corresponding to 0 and 180 degrees.
// @param location 0 to 15 corresponding to the servo slot.
void setServoAmount(const i2c *eeBus, uint16_t rotation, uint8_t location){
  const uint8_t offsetStartRegister = 0x06;
  const uint8_t offsetStopRegister = 0x08;
  const uint8_t offsetAmount = 0x04;
  location %= 16; // Do not allow access out of bounds.
  uint8_t startRegister = 0x06 + 0x04 * location;
  uint8_t stopRegister = 0x08 + 0x04 * location;
  const int max = 540;
  const int min = 110;
  const uint16_t startLocation = 0;
  uint16_t stopLocation = (rotation * (max - min)) / 180 + min;
  print("counter = %d \n", stopLocation);
  i2c_out(eeBus, 0x40, startRegister, 1, &startLocation, sizeof(startLocation));  // On pwm.
  i2c_out(eeBus, 0x40, stopRegister, 1, &stopLocation, sizeof(stopLocation));  // Off pwm.  
}  
