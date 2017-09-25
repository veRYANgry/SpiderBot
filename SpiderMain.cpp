#include <stdio.h>
#include "ServoController.h"
	// Array for iterating over all servo locations.
	ServoController::servoLocation servoLocations[12] = {
			ServoController::kServoLocationFrontRightTip,
			ServoController::kServoLocationFrontRightMiddle,
			ServoController::kServoLocationFrontRightBase,
			ServoController::kServoLocationFrontLeftTip,
			ServoController::kServoLocationFrontLeftMiddle,
			ServoController::kServoLocationFrontLeftBase,
			ServoController::kServoLocationBackRightTip,
			ServoController::kServoLocationBackRightMiddle,
			ServoController::kServoLocationBackRightBase,
			ServoController::kServoLocationBackLeftTip,
			ServoController::kServoLocationBackLeftMiddle,
			ServoController::kServoLocationBackLeftBase};

int main(){
	const int controllerAddress = 0x40;
	ServoController servoController = ServoController();
	servoController.connectToAddress(controllerAddress);
	servoController.setupServoController();
	for (ServoController::servoLocation servo : servoLocations){
		servoController.setServoAmount(0,servo);
	}
}

