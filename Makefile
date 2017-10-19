# location of the Python header files
 
PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
 
# location of the Boost Python include files and library
 
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib/arm-linux-gnueabihf/
 
# compile mesh classes


TARGET = pythonServoWrapper

$(TARGET).so: $(TARGET).o ServoController.o
	g++ -shared -Wl,--export-dynamic $(TARGET).o ServoController.o -lwiringPi -L$(BOOST_LIB) -lboost_python-py27 -L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION) -o ServoController.so
 
$(TARGET).o: $(TARGET).cpp
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c $(TARGET).cpp
	
ServoController.o: ServoController.cpp
	g++ -o ServoController.o -c ServoController.cpp

