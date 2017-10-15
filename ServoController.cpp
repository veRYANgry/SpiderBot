/*
 * ServoController.cpp
 *
 *  Created on: Sep 22, 2017
 *      Author: veryangry
 */

#include "ServoController.h"

ServoController::ServoController() {
	// TODO Auto-generated constructor stub

}

ServoController::~ServoController() {
	// TODO Auto-generated destructor stub
}

// Connect to the servo controller at the specified address.
// @param address The location of the servo device on the i2c bus.
void ServoController::connectToAddress(int address){
	int servoConterollerFD = wiringPiI2CSetup(address);
	if(servoConterollerFD == -1){
		perror("Problem opening servo controller");
	} else {
		this->servoConterollerFD = servoConterollerFD;
	}
}

// Set up the servo controller with the right frequency and settings to control servos.
void ServoController::setupServoController(){
	const int freq = 121; //(25Mhz / (4096* frq)) - 1.
	const int sleep = 0x10;
	const int autoInc = 0x20;
	const int addressFrequency = 0xFE;
	const int addressMode1 = 0x00;
	int writeChar = 0;
	writeChar |= sleep;
	wiringPiI2CWriteReg8(servoConterollerFD,addressMode1,writeChar);
	wiringPiI2CWriteReg8(servoConterollerFD,addressFrequency,freq);
	writeChar = 0 | autoInc;
	wiringPiI2CWriteReg8(servoConterollerFD,addressMode1,writeChar);
}

// Set the servo from 0 to 180 degrees.
// @param rotation 0 to 180 corresponding to 0 and 180 degrees.
// @param location 0 to 15 corresponding to the servo slot.
void ServoController::setServoAmount(float rotation, servoLocation location) {
	int startRegister = 0x06 + 0x04 * location;
	int stopRegister = 0x08 + 0x04 * location;
	int max = 530 + kServoMaxSetArray[location];
	int min = 110 + kServoMinSetArray[location];
	const int startLocation = 0;
	int stopLocation = (rotation * (max - min)) / 180 + min;
	wiringPiI2CWriteReg16(servoConterollerFD,startRegister,startLocation);// On pwm.
	wiringPiI2CWriteReg16(servoConterollerFD,stopRegister,stopLocation);// Off pwm.
}

