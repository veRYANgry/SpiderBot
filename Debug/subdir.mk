################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ServoController.cpp \
../SpiderMain.cpp \
../pythonServoWrapper.cpp 

OBJS += \
./ServoController.o \
./SpiderMain.o \
./pythonServoWrapper.o 

CPP_DEPS += \
./ServoController.d \
./SpiderMain.d \
./pythonServoWrapper.d 


# Each subdirectory must supply rules for building sources it contributes
ServoController.o: ../ServoController.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/home/veryangry/rpi/wiringPi/wiringPi -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"ServoController.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/home/veryangry/rpi/wiringPi/wiringPi -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

pythonServoWrapper.o: ../pythonServoWrapper.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/usr/include -I/usr/include/python2.7 -I/home/veryangry/rpi/wiringPi/wiringPi -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -shared -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"pythonServoWrapper.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


