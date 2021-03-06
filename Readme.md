# README

the aim of this module is to send data from/to GODOT <-> Arduino
(tested and working on linux platform)

![Demo](/_._doxygen/demo.gif)

in this demo :
- at the top between < >, data coming from arduino
- at the bottom, integer sent to arduino and controling the intensity of the red LED.

# Godot Engine

![Godot logo](/_._doxygen/godot_logo_md.png "Godot logo")

https://godotengine.org/

# Arduino

![Arduino logo](/_._doxygen/Arduino_logo_md.png "Arduino logo")

https://www.arduino.cc/en/Guide/Introduction

# Godot module for communication with Arduino 

![Godot Arduino love](/_._doxygen/logo_doxy.png "Godot Arduino love")

## Arduino_code folder
in this project, the arduino nano is wired with MPU9250/MPU650 module SDA --> A4 and SCL --> A5

[links to MPU9250 description](https://invensense.tdk.com/products/motion-tracking/9-axis/mpu-9250/)

[links to MPU9250 example](http://arduinolearning.com/code/arduino-mpu-9250-example.php)

[links to library rpicopter](https://github.com/rpicopter/ArduinoMotionSensorExample)

![MPU9250](/_._doxygen/s-l1600_md.png)

https://www.robot-maker.com/shop/capteurs/217-imu-9-axes-mpu9250-6500-spi-217.html

a led is also wired to D3 in order to be controled from godot

## Godot module folder

to be copied in Godot module folder (godot-3.x.y-stable/modules/arduino_f/)

in file :
comments the line (if not, it is a define for using this module in the main file -> see main.cpp)
~~~~~~.c
// #define TEST_arduino_f_module_in_main
~~~~~~

and build again with scons in the main directory /godot-3.x.y-stable (x11 for linux)
~~~~~~.bash
scons platform='x11' -j4
~~~~~~
(to use 4 threads)

code could be used as is with the main.cpp
go to the Godot module folder and :
~~~~~~.bash
scons -j4
~~~~~~

[links to CeSerial library](https://github.com/yan9a/serial/)
(many thanks to the developer Yan Naing Aye)

## Godot_Codes folder

Godot code to be opened in Godot

## _._doxygen folder 

open doxyfile with doxygen --> run doxygen --> show HTML output 
--> enjoy browsing in this beautiful documentation ;)

# ideas for future development

- add crc check for the strings transmitted from and to Godot

- add the possibility to choose in Godot directly the speed of sending message (for the moment every 10 ms)

- add a second loop to have two channels of sending message in different speed ; for example, fast message every 10 ms and low message every 1000 ms

- add connectivity via WiFi, Bluetooth
