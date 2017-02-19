My Arduino Projects
===================

car1
====

My adventures with [this](http://www.dx.com/p/n20-gear-motor-4wd-bluetooth-controlled-smart-robot-car-kit-w-tutorial-for-arduino-blue-black-425112#.WJ-GGoWcG70) car kit from dx.

![dx photo](http://img.dxcdn.com/productimages/sku_425112_2.jpg)

Notes
-----
* The main car chassis board contains a motor controler. It seems to be a clone of L293D H-Bridge
* am using 2x 14500 Li-Ion batteries  ~3.7v each
* To get the bluetooth working reliably: Use 3.3v on its power pin. Need to add a voltage divider to the bluetooth module's RX line to reduce the signal to 3.3v max. The voltage divider is made with 1 and 2k resisters, see [here](http://www.instructables.com/id/Cheap-2-Way-Bluetooth-Connection-Between-Arduino-a/) for more info.  On an Android phone, am using this [app](https://play.google.com/store/apps/details?id=com.giumig.apps.bluetoothserialmonitor) to control the car via bluetooth. 


Visual Studio Code 
==================

Am using VSCode with an Arduino plugin.
Read about it ...
* Using Visual Studio Code as an ArduinoIDE  https://github.com/fabienroyer/VisualStudioCodeArduino
* build&upload:  Ctrl-shift-p  then tasks, then run tasks, then --upload
* Build:  Ctrl-shift-b
