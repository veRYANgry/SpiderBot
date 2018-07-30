from mpu6050 import mpu6050

sensor = mpu6050(0x68)
accelData = sensor.get_accel_data()
print accelData

gyroData = sensor.get_gyro_data()
print gyroData