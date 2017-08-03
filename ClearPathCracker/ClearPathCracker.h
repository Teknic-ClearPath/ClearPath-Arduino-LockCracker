

#ifndef ClearPathCracker_h
#define ClearPathCracker_h


//Import Required libraries to pulse step and direction
#include "Arduino.h"
#include <PulseClearPath.h>
#include <TeknicStepController.h>
#include <StepClearPath.h>
#define NumDetents 12

class ClearPathCracker
{
  public:

ClearPathCracker(StepClearPath* LockMotor, int solenoidpin, int sensorpin);

int findNearestDetent();
void findAllDetents(int *detent_locations);
boolean TryCombination(int firstNum, int secNum, int thirdNum);
boolean TryAllPossibleCombinations(int thirdnum);

  
  
  // Reference a StepClearPath Motor
StepClearPath* X;
int CMDRES = 1600;       //input resolution of the motor )
int backlash_Comp =0;          //backlash_Comp correction factor
int aftermovedwell =40;  //aftermove dwell time in ms
int solenoid;          //define pin for solenoid
int lockOpenSensor;    //define lock open sensor pin
long solveVelocity = 100000;
long solveAccel = 2000000;

// array to store detent positions and widths for sensing the final digit
int detents[2][NumDetents];

// list of all possible combinations derived from limitations in the lock mechanics
int combinations[4][3][10]=
{
{
  {0, 4, 8, 12, 16, 20, 24, 28, 32, 36},
  {2, 6, 10, 14, 18, 22, 26, 30, 34, 38},
  {0, 4, 8, 12, 16, 20, 24, 28, 32, 36}
},
{
  {1, 5, 9, 13, 17, 21, 25, 29, 33, 37},
  {3, 7, 11, 15, 19, 23, 27, 31, 35, 39},
  {1, 5, 9, 13, 17, 21, 25, 29, 33, 37}
},
{
  {2, 6, 10, 14, 18, 22, 26, 30, 34, 38},
  {0, 4, 8, 12, 16, 20, 24, 28, 32, 36},
  {2, 6, 10, 14, 18, 22, 26, 30, 34, 38}
},
{
  {3, 7, 11, 15, 19, 23, 27, 31, 35, 39},
  {1, 5, 9, 13, 17, 21, 25, 29, 33, 37},
  {3, 7, 11, 15, 19, 23, 27, 31, 35, 39}
}
};

};
#endif

