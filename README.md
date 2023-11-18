# TuffyTimer2000
This C++ code is for a xenon lamp controller with time logging.

The program uses RS232 ports for communication with commerical xenon lamps. It can then send _start_ and _stop_ commands to ignite and quench the lamp. Error codes are read and logged from the lamp controller. Most importantly, a running time can be set for the lamp, e.g. 3 hours. This way, the lamp is automatically stopped after the desired illumination time.

This was used in a research lab with commercial xenon lamps acting as solar simulators. 

Example program and log are given below.

![TimerProgram1](https://github.com/OscarBrownbread/TuffyTimer2000/assets/26405187/002f5679-5b1a-4012-a148-72be65c479c2)


```
This is the log of TuffyTimer2000. Lamp time operation and start time.
Check the power supply manual for error codes
NUL	  = No error code
ESR01 = Operation complete (everything is OK)
ESR20 = Command error (lamp was already OK)
ESR10 = Execution error
===================================================================================
Start Code []
 Stop Code [] 
Lamp Start: 8/5/2015_17:24:23  You gained 81 Awesome points! 
Lamp  Stop: 8/5/2015_17:24:41 Lamp on for 0:0:18 
======================================================= 
Start Code: 
 Stop Code:
Lamp Start: 8/5/2015_18:2:31  You gained 4 Awesome points! 
Lamp  Stop: 8/5/2015_18:2:39 Lamp on for 0:0:8 
======================================================= 
Start Code: 
 Stop Code:
Lamp Start: 8/5/2015_18:3:39  You gained 4 Awesome points! 
Lamp  Stop: 8/5/2015_18:3:49 Lamp on for 0:0:10 
======================================================= 
```
