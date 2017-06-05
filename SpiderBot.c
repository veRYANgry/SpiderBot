/*
  Test code for spiderBot
*/

#include "simpletools.h"                      // Include simpletools header

i2c *eeBus;                                   // I2C bus ID
void setupServoController(const i2c *eeBus);
void setServo(const i2c *eeBus, long start, long stop);

int main()                                    // Main function
{
  eeBus = i2c_newbus(28,  29,   0);           // Set up I2C bus, get bus ID
  setupServoController(eeBus);
  long temp = 0;
  char i = 0;
  char writeChar = 0;
  int totalcounts = 205;
  int pulseSize = 110;
  while(1){
    for(int counter = pulseSize;counter < 540;counter += 410){
      setServo(eeBus,0, counter);
      print("counter = %d \n", counter);
      sleep(1); 
    }
  }     
}

// Setup the servo controller PCA9685 with Auto increment and a 50 hz freq.
// @param eeBus The bus the controller is on.
void setupServoController(const i2c *eeBus){
  const char freq = 121; // set the freq register with 121 to get 50 hz (25Mhz / (4096* frq)) - 1.
  const char sleep = 0x10;
  const char autoInc = 0x20;
  const char addressFrequency = 0xFE;
  const char addressMode1 = 0x00;
  char writeChar = 0;
  writeChar |= sleep;
  i2c_out(eeBus, 0x40, addressMode1, 1, &writeChar, sizeof(writeChar));   // Enable sleep to set the freq register.
  i2c_out(eeBus, 0x40, addressFrequency, 1, &freq, sizeof(freq));   // Set the freq to 50hz (20ms) for the servos.
  writeChar = 0 | autoInc;
  i2c_out(eeBus, 0x40, addressMode1, 1, &writeChar, sizeof(writeChar)); // Set sleep off (default state) and enable auto increment.
}

void setServo(const i2c *eeBus, long start, long stop){
  i2c_out(eeBus, 0x40, 0x06, 1, &start, sizeof(start));  // On pwm.
  i2c_out(eeBus, 0x40, 0x08, 1, &stop, sizeof(stop));  // Off pwm.  
}  

void setServo(const i2c *eeBus, long start, long stop){
  i2c_out(eeBus, 0x40, 0x06, 1, &start, sizeof(start));  // On pwm.
  i2c_out(eeBus, 0x40, 0x08, 1, &stop, sizeof(stop));  // Off pwm.  
}  