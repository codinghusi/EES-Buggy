# Buggy

## Preparation
* install wiringPi
* enable i2c

## Build project
`make`  
`make run`

## Commands
`w` - Drive forward  
`s` - Drive backward  
`a` - Drive forward with turn until 90 degrees, then drive forward  
`d` - Drive forward with turn until 90 degrees, then drive forward  
`+` - Increase speed +5%  
`-` - Decrease speed -5%  
  
`q` - Rotate for 90 degrees (anti-clockwise)  
`e` - Rotate for 90 degrees (clockwise)  
  
`c` - Program: Circumnavigate without gyro  
`C` - Program: Circumnavigate with gyro  
  
`r` - Program: Drive in a rectangle without gyro  
`R` - Program: Drive in a rectangle with a gyro  
  
`o` - Program: Drive a slalom without sensors  
`p` - Program: Drive a slalom with ultra sonic without gyro  
`O` - Program: Drive a slalom with gyro and ultra sonic  
  
`u` - Program: Run over an object in front (please be careful!)  

`k` - Resets absolute gyroscope
`g` - Outputs the current gyroscope.z value 

## config.h
Defines to configure your pins and speeds
