/*
  PulseClearPath.h - Library for interfacing with ClearPath motors using an Arduino- Version 1
  Teknic 2014 Brendan Flosenzier

  This library is free software; you can redistribute it and/or
  modify it.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* 
  
  A PulseClearPath is activated by creating an instance of the PulseClearPath class. 

  There can several instances of PulseClearPath however each must be attached to different pins.

  This class is used in conjuntion with the TeknicStepController class, and is related to the StepClearPath class.
  All functions available in this class, are available in the StepClearPath.
  
  Note: Each attached motor must have its direction/B pin connected to one of pins 8-13 in order to work with
		The TeknicStepController object.  Other devices can be connected to pins 8-13 as well

  The functions for a pulse motor are:

   PulseClearPath - default constructor for initializing the motor

   attach() - Attachs pins to this motor, and declares them as input/outputs

   stopMove()  - Interupts the current move, the motor may abruptly stop
 
   calcSteps()     - Internal Function used by TeknicStepController to calculate how many pulses to send to each motor

   getDirPin() - Returns the pin number of the attached Direction Pin

   move() - sets the maximum veloctiy

   getCommand() - sets the acceleration

   Direction() - Sets the motor's direction bit
   
   commandDone() - returns wheter or not there is a valid current command

   disable() - disables the motor

   enable() - enables the motor

   getCommandedPosition() - Returns the absolute cmomanded position where the position on enable=0

   readHLFB() - Returns the value of the HLFB Pin

   altSpeed() - Turns off the Enable pin for a short time in order to select the alternate speed from MSP
 */
#ifndef PulseClearPath_h
#define PulseClearPath_h
#include "Arduino.h"
class PulseClearPath
{
  public:
  PulseClearPath();
  void attach(int);
  void attach(int, int);
  void attach(int, int, int);
  void attach(int, int, int, int);
  virtual void stopMove();
  virtual int calcSteps();
  int getDirPin();
  boolean move(long);
  long getCommand();
  long test();
  void Direction(uint8_t);
  virtual boolean commandDone();
  virtual void disable();
  void enable();
  long getCommandedPosition();
  boolean readHLFB();
  void altSpeed(int time);

protected:

uint8_t _Apin;
uint8_t _Bpin;
uint8_t _Epin;
uint8_t _Hpin;
 boolean _enabled;
 volatile long _commandX;
 boolean _direction;
 uint8_t _BurstX;
volatile long _absPosition;
};
#endif