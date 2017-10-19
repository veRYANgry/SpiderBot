import ServoController

controller = ServoController.ServoController()
controller.connectToAddress(0x40)
controller.setupServoController()
for x in range(0,13):
    controller.setServoAmount(170,x)
