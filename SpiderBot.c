/*
  Test code for spiderBot
*/

#include "simpletools.h"                  

i2c *eeBus;
void setupServoController(const i2c *eeBus);
void setServoAmount(const i2c *eeBus, uint16_t rotation, uint8_t location);
void interpolateServo(const i2c *eeBus, float time,uint16_t startPosition , uint16_t endPosition, uint8_t location);
void moveServoArm(float x, float y, uint8_t location, uint8_t isUp);
uint16_t invert(uint16_t position, uint8_t invert);
// Servo location constants from the tail as the back looking down.
#define kServoLocationFrontRightTip 0
#define kServoLocationFrontRightMiddle 1
#define kServoLocationFrontRightBase 2

#define kServoLocationFrontLeftTip 3
#define kServoLocationFrontLeftMiddle  4
#define kServoLocationFrontLeftBase 5

#define kServoLocationBackRightTip 6
#define kServoLocationBackRightMiddle 7
#define kServoLocationBackRightBase 8

#define kServoLocationBackLeftTip 9
#define kServoLocationBackLeftMiddle 10
#define kServoLocationBackLeftBase 11

// Descriptions for servo legs used in movement.
#define kServoLegFrontRight 0
#define kServoLegFrontLeft 1
#define kServoLegBackRight 2
#define kServoLegBackLeft 3

#define tipOffset  0
#define middleOffset  1
#define baseOffset  2

 uint16_t kServoInversionArray[] = 
 { [kServoLegFrontRight] = 0,
 [kServoLegFrontLeft] = 1,
 [kServoLegBackRight] = 1,
 [kServoLegBackLeft] = 0
 };

// Offset array for calibration of servos.
 int16_t kServoMinSetArray[] = 
{[kServoLocationFrontRightTip] = 0,
  [kServoLocationFrontRightMiddle]= 100,
  [kServoLocationFrontRightBase] = 100,
  [kServoLocationFrontLeftTip] = 0,
  [kServoLocationFrontLeftMiddle] = 0,
  [kServoLocationFrontLeftBase] = 0,
  [kServoLocationBackRightTip] = 0,
  [kServoLocationBackRightMiddle] = 0,
  [kServoLocationBackRightBase] = 0,
  [kServoLocationBackLeftTip] = 0,
  [kServoLocationBackLeftMiddle] = 100,
  [kServoLocationBackLeftBase] = 100};
  
   int16_t kServoMaxSetArray[] = 
{[kServoLocationFrontRightTip] = 0,
  [kServoLocationFrontRightMiddle]= 0,
  [kServoLocationFrontRightBase] = 0,
  [kServoLocationFrontLeftTip] = 0,
  [kServoLocationFrontLeftMiddle] = -240,
  [kServoLocationFrontLeftBase] = -150,
  [kServoLocationBackRightTip] = 0,
  [kServoLocationBackRightMiddle] = -190,
  [kServoLocationBackRightBase] = -200,
  [kServoLocationBackLeftTip] = 0,
  [kServoLocationBackLeftMiddle] = -10,
  [kServoLocationBackLeftBase] = -50};
  
int main()
{
  eeBus = i2c_newbus(28,  29,   0); // Set up I2C bus
  setupServoController(eeBus);
    //scan("%d %d %d\n", &minServoSetting, &maxServoSetting, &location)
  
  while(1){
    moveServoArm(1,0, kServoLegFrontRight,0);
    moveServoArm(1,0, kServoLegFrontLeft,0);
    moveServoArm(1,0, kServoLegBackRight,0);
    moveServoArm(1,0, kServoLegBackLeft,0);
    sleep(1);
    moveServoArm(0,1, kServoLegFrontRight,0);
    moveServoArm(0,1, kServoLegFrontLeft,0);
    moveServoArm(0,1, kServoLegBackRight,0);
    moveServoArm(0,1, kServoLegBackLeft,0);
    sleep(1);
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
  int max = 530 + kServoMaxSetArray[location];
  int min = 110 + kServoMinSetArray[location];
  const uint16_t startLocation = 0;
  uint16_t stopLocation = (rotation * (max - min) ) / 180 + min;
  i2c_out(eeBus, 0x40, startRegister, 1, &startLocation, sizeof(startLocation));  // On pwm.
  i2c_out(eeBus, 0x40, stopRegister, 1, &stopLocation, sizeof(stopLocation));  // Off pwm.  
}  

// @param startPosition rotation 0 to 180 corresponding to 0 and 180 degrees for the time at 0.
// @param endPosition rotation 0 to 180 corresponding to 0 and 180 degrees for the time at 1.
// @param location 0 to 15 corresponding to the servo slot.
void interpolateServo(const i2c *eeBus, float time, uint16_t startPosition , uint16_t endPosition, uint8_t location){
  setServoAmount(  eeBus, time * endPosition + (time - 1) * startPosition, location);
}

uint16_t invert(uint16_t position, uint8_t invert){
  if(invert){
    return 180 - position;
  } else {
    return position;
  }        
}  

void moveServoArm(float x, float y, uint8_t location, uint8_t isUp){
  if(isUp){
      setServoAmount(  eeBus,invert(10, kServoInversionArray[location] * 180), location * 3 + tipOffset);
  } else {
    setServoAmount(  eeBus,invert(150, kServoInversionArray[location] * 180), location * 3 + tipOffset);
  }
  
  float rotation;
  if(x == 0){
    rotation = 90;
  }    
  rotation = atan(y/x);
  setServoAmount(  eeBus,invert(rotation, kServoInversionArray[location]), location * 3 + middleOffset);
}  