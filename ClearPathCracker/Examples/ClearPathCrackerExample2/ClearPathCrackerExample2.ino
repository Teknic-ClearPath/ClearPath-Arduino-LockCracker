
/*
  ClearPathCracker-Example 2
  This example incorperates sensing of the lock, to narrow down possible combinations.
  Final all possible combinations at each detent location are attempted. Nominally, this 
  process takes 1-2min.
  This code assumes the lock dial is in the zero position when starting.  
  
 */
//Import library
#include <ClearPathCracker.h>

// initialize a StepClearPath Motor
StepClearPath X;

//initialize the ClearPathCracker object, passing the motor, solenoid pin(3), and open sensor pin (5) 
ClearPathCracker masterCracker(&X,3,5);

//initialize the controller and pass the reference to the motor we are controlling
TeknicStepController machine(&X);

//initialize array to store detent positions relating to the final digit
int detent_locations[NumDetents];

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

//This function traverses the lock looking for 12 detent locations.  Once found the list of detent 
//positions is sorted by width with the widest location being the most likely last digit of the 
//combination.  Depending on lock to lock variations, backlash, and how effectively the locks dial
//is gripped, this function may not measure the third digit location as the widest, so all 
//locations should be tried just in case.
//This takes an array as an input, after the function completes, the values in the array are
//changed to reflect the detent positions for most [0] to least [NumDetents] likely. 
 masterCracker.findAllDetents(detent_locations);
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


 //Traverse the list of detent positions, and attempt all possible combinations at each location
  for(int i=0;i<NumDetents;i++)
  {
      //This function tries all possible combinations given the third digit (detent location)
      //returns true if one combination is successful.  ~100 combinations will be tried.
      if(masterCracker.TryAllPossibleCombinations(detent_locations[i]))
        i=NumDetents;   //This short circuits the for loop if the combination is found.
  }
   
 //If all combinations fail, disable and end
   X.disable();
   while(true)
   {}
   
}
