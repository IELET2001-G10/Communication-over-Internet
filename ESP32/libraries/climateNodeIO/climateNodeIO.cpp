/*
   climateNodeIO.h - Library for handling the climate node IO on a ESP32 devkit C board.
   Created by Håvard Olai Kopperstad, November 20, 2020.
   Released into the public domain.
   Used tutorial for how to create library and inspiration: https://www.arduino.cc/en/Hacking/libraryTutorial
 */

#include "Arduino.h"                                            //Statement that gives access to the standard types and constants of Arduino C language
#include "climateNodeIO.h"                                      //Statement that gives access to the definitions in header file

climateNodeIO::climateNodeIO(int pin)                           //"::" means that following function is part of the climateNode class
{
    pinMode(pin, OUTPUT);
    _pin = pin;
}

void climateNodeIO::LEDstate(bool value)
{
    digitalWrite(_pin, value);
}
