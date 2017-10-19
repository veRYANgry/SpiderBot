/*
 * pythonServoWrapper.cpp
 */

#include <boost/python.hpp>
#include "ServoController.h"
using namespace boost::python;


BOOST_PYTHON_MODULE(ServoController){
	class_<ServoController>("ServoController")
			.def("connectToAddress", &ServoController::connectToAddress)
			.def("setupServoController", &ServoController::setupServoController)
			.def("setServoAmount", &ServoController::setServoAmount)
			;
}
