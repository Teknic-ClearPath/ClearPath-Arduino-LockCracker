/*
  TeknicStepController.h - Interrupt driven library for controlling ClearPath motors using an Arduino- Version 1
  Teknic 2014 Brendan Flosenzier

  This library is free software; you can redistribute it and/or
  modify it.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* 
  
  A TeknicStepController is activated by creating an instance of the TeknicStepController class, and passing the constructor references to the motors it will control.
  The motors are pulsed in the background according to move information stored within each motor.
  Only motors of type; PulseClearPath, or substype StepClearPath, may be used.

  There can only be one instance of Step Controller at any time.

  This class uses Timer2, so other functions and classes which use timer 2 will not work correctly ie: tone(), MsTimer2() etc.

  The ISR is set to 2KHz, nominally

  Note: Each attached motor must have its direction/B pin connected to one of pins 8-13

  other devices can be connected to pins 8-13 as well

  The methods are:

   TeknicStepController - Class for manipulating ClearPath motors connected to Arduino up to 6 motors may be controlled.

   HandleInterupt(pin )  - Attaches a servo motor to an i/o pin.
   
 
   Start(time)     - gets Direction pins for all connected motors (make sure all motors have been attached before this is called
						Configures the ISR to run at 2kHz, time must be 249

   Stop() - disables the ISR in this class
   
 */
#ifndef TeknicStepController_h
#define TeknicStepController_h

#include "Arduino.h"
#include "StepClearPath.h"

class TeknicStepController
{
  public:
  TeknicStepController(PulseClearPath* motor1);
  TeknicStepController(PulseClearPath* motor1, PulseClearPath* motor2);
  TeknicStepController(PulseClearPath* motor1, PulseClearPath* motor2, PulseClearPath* motor3);
  TeknicStepController(PulseClearPath* motor1, PulseClearPath* motor2, PulseClearPath* motor3, PulseClearPath* motor4);
  TeknicStepController(PulseClearPath* motor1, PulseClearPath* motor2, PulseClearPath* motor3, PulseClearPath* motor4, PulseClearPath* motor5);
  TeknicStepController(PulseClearPath* motor1, PulseClearPath* motor2, PulseClearPath* motor3, PulseClearPath* motor4, PulseClearPath* motor5, PulseClearPath* motor6);
  void Start(int time);
  void Stop();
  int getsum();


};
#endif
