/*
 * ServoController.h
 *
 *  Created on: Sep 22, 2017
 *      Author: veryangry
 */

#pragma once
#include <wiringPiI2C.h>
#include <stdio.h>
#include <utility>

// Descriptions for servo legs used in movement.
#define kServoLegFrontRight 0
#define kServoLegFrontLeft 1
#define kServoLegBackRight 2
#define kServoLegBackLeft 3

#define tipOffset  0
#define middleOffset  1
#define baseOffset  2


class ServoController {
public:
	enum servoLocation{
		kServoLocationFrontRightTip = 0,
		kServoLocationFrontRightMiddle = 1,
		kServoLocationFrontRightBase = 2,
		kServoLocationFrontLeftTip = 3,
		kServoLocationFrontLeftMiddle = 4,
		kServoLocationFrontLeftBase = 5,
		kServoLocationBackRightTip = 6,
		kServoLocationBackRightMiddle = 7,
		kServoLocationBackRightBase = 8,
		kServoLocationBackLeftTip = 9,
		kServoLocationBackLeftMiddle = 10,
		kServoLocationBackLeftBase = 11
	};

	ServoController();
	virtual ~ServoController();
	void connectToAddress(int address);
	void setupServoController();
	void setServoAmount(float rotation, int location);
private:
	int servoConterollerFD;

	// Offset array for calibration of servos.
	int kServoMinSetArray[12] {0,200,100,0,0,0,0,0,20,0,200,100};

	// Offset array for calibration of servos.
	int kServoMaxSetArray[12] {-100,0,-100,0,-280,-280,0,-240,-200,0,-10,-50};

};

