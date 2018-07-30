import ServoController
import time
import sys
import math
import random as rnd
import pickle
from mpu6050 import mpu6050
import thread


def getDegressFromUser(inputList):
    while True:
        motorInput = raw_input("motor?")
        
        degreeInput = raw_input("degrees?")
        inputList[0] = (int(degreeInput),int(motorInput))
    
servoInversionList = [1,-1,-1,-1,1,1,-1,1,1,1,-1,-1]

def main():
    controller = ServoController.ServoController()
    controller.connectToAddress(0x40)
    controller.setupServoController()
    
    sensor = mpu6050(0x68)
    
    inputList = [(90,0)]
    thread.start_new_thread(getDegressFromUser, (inputList,))
    while True:
        motor = inputList[0][1]
        controller.setServoAmount((inputList[0][0] - 90) * servoInversionList[motor] + 90, motor)
        time.sleep(.5)

main()

