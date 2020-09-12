ArduinoMotionSensorExample
======================

MPU6050/MPU6500/MPU9150/MPU9250 over I2c for Arduino

A clean example of using INV_MPU library on Arduino. Tested on Atmega328.


Features:
- uses FastWire and I2Cdev from Jeff Rowberg
- DMP enabled
- calculates and displays gyro and quaternions
- Makefile provided for Arduino-Makefile (https://github.com/sudar/Arduino-Makefile)


When compiling please ensure one of the following is defined (see Makefile)
- #define MPU6050
- #define MPU9150
- #define MPU6500
- #define MPU9250

This has been tested using Arduino 1.0.5 libraries and a few variants of Arduino Pro Mini boards.
