/*
   climateNodeIO.h - Library for handling the climate node IO on a ESP32 devkit C board.
   Created by Håvard Olai Kopperstad, November 20, 2020.
   Released into the public domain.
   Used tutorial for how to create library and inspiration: https://www.arduino.cc/en/Hacking/libraryTutorial
 */

#ifndef climateNodeIO_h                                         //Wrapping code up in construct to prevent double inclusion of library
#define climateNodeIO_h

#include "Arduino.h"                                            //Statement that gives access to the standard types and constants of Arduino C language in this library


/**
 * Class for declaration of digital LED alarm pin. Includes method for pulling LED high and low.
 */
class climateNodeIO
{
    public:
        /**
         * Function that declares the given pin number as an output.
         * @param pin The pin number where the anode of the LED is connected to.
         */
        climateNodeIO(int pin);

        /**
         * Function that pulls the digital output high or low.
         * @param value The boolean value for pulling pin high or low.
         */
        void LEDstate(bool value);

    private:
        int _pin;                                               //"_" means that variabl is private. Also servers as an indicator to distinguish name from argument in function
};


#endif