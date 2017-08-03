
/*
  ClearPathCracker Example 1
  This example implements the basics of entering predetermined combinations.  
  This example can be used for troubleshooting when using a lock you already know
  the combination to.  This examples the lock be at the zero mark when starting,
  otherwise the numbers in the combination will be offset.
  
 */
//Import library
#include <ClearPathCracker.h>

// initialize a StepClearPath Motor
StepClearPath X;

//initialize the ClearPathCracker object, passing the motor, solenoid pin(3), and open sensor pin (5) 
ClearPathCracker masterCracker(&X,3,5);

//initialize the controller and pass the reference to the motor we are controlling
TeknicStepController machine(&X);


// the setup routine runs once when you press reset:
void setup()
{  
 //DEBUG declare pin 2 as output.  This shows the ISR rate for the ZCC StepController
  pinMode(2,OUTPUT);
  
  //Begin Serial Communication NOTE: this is needed as the ClearPathCracker Class returns serial statements
  Serial.begin(115200);

  //attach ClearPath motor using pins 8,9,6,4
  X.attach(8,9,6,4);          //Direction/A is pin 8, Step/B is pin 9, Enable is pin 6, HLFB is pin 4

  //2 sec pause to allow user to zero the lock  (could implement a wait for button here)
  delay(2000);
  
  // Enable motor, reset the motor position to 0
  X.enable();
  delay(100);     //Allow time for motor the recieve enable command, pause before starting controller
  
  // Set up the ISR to constantly corect motor position.  PARAMETER MUST BE SET TO 249
  machine.Start(249);

}

// the loop routine runs over and over again forever, till it stops...
void loop()
{   
  //Velocity used to enter combinations can be between 2-100,000 steps/sec
  masterCracker.solveVelocity= 100000;

  //Acceleration used to enter combinations.  Can be between 4,000 and 2,000,000 steps/sec/sec
  //NOTE:  A low acceleration will prevent the motor from achieveing high velocity for short moves
  //NOTE: Increasing acceleration beyond 1,000,000 may be too aggressive for a typical masterlock
  //      try lubricating the lock to insure smooth operation
  masterCracker.solveAccel= 1000000;
  
  //Dwell in ms between entered numbers.  NOTE: this value should be above 20 to ensure the mechanics have 
  //fully stopped due to the backlash in the system, and to account for the 16ms RAS within the ClearPath
  //Increase this time to see a noticeable dwell between each number entered 
  masterCracker.aftermovedwell=30;
  
  
  //Try preset combinations:  These function will execute sequential moves to enter the combinations
  //then try to open the lock while printing the attempted combination, and wether the combination 
  //was successful to the Serial port.  This function also returns true, or false depending whether
  //the combination was successfull.
  if(masterCracker.TryCombination(7,21,31)){}
  else
    if(masterCracker.TryCombination(5,31,37)){}
  else
    if(masterCracker.TryCombination(15,17,39)){}
  else
    if(masterCracker.TryCombination(24,38,4)){}

   
 //disable and end
   X.disable();
   while(true)
   {}
   
}
