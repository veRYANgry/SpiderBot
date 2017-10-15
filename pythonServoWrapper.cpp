/*
 * pythonServoWrapper.cpp
 */

#include <boost/python.hpp>
#include "ServoController.h"
using namespace boost::python;


BOOST_PYTHON_MODULE(servo){
	class_<ServoController>("ServoController")
			.def("connectToAddress", &ServoController::connectToAddress)
			.def("setupServoController", &ServoController::setupServoController)
			.def("setServoAmount", &ServoController::setServoAmount)
			;
}
