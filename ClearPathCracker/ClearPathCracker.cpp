#include "Arduino.h"
#include <PulseClearPath.h>
#include <TeknicStepController.h>
#include <StepClearPath.h>
#include <ClearPathCracker.h>

ClearPathCracker::ClearPathCracker(StepClearPath* LockMotor, int solenoidpin, int sensorpin)
{
	X=LockMotor;
	solenoid=solenoidpin;
	lockOpenSensor=sensorpin;
	pinMode(solenoid,OUTPUT);
  pinMode(lockOpenSensor,INPUT_PULLUP);   // pull-up becuase the sensor is open collector
  
  //Randomize to remove pattern used to find lock combo
  for(int c=0;c<4;c++)
  {
  for(int b=0; b<3;b++)
  {
  for (int a=0; a<10; a++)
	{
	int r = random(a,9); // dont remember syntax just now, random from a to 8 included.
	int temp = combinations[c][b][a];
	combinations[c][b][a] = combinations[c][b][r];
	combinations[c][b][r] = temp;
	}
  }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This function finds the nearest detent position, measures the width, 
//moves the motor to the center of the detent, and returns the width
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ClearPathCracker::findNearestDetent()
{
  int width = 0;
  int positiveside = 0;
  int negativeside = 0;

  //Set velocity SLOW for sensing move
  X->setMaxVel(400);
   X->setMaxAccel(10000);

  //Turn on solenoid to apply pressure to shackle
  digitalWrite(solenoid,HIGH);
  delay(40);  //Allow time for solenoid to turn on

  //12 detent positions equally spaced  CMDRES/12 ~= 133 counts 
  //command move that is longer to garuntee to hit detent
  X->move(-300);

  //Spin on command done in case motor doesn't go "Out of Range"
  while(!X->commandDone())
   { 
    //Look for when motor goes "Out of Range"
    if(X->readHLFB())
    {
        //abruptly stop move once detent is hit
        X->stopMove();
		
        //record actively commanded position
        //NOTE: Command will lead actual position because of RAS delay
        //      This won't matter because the amount of error will be 
        //      consistent and were comparing relative sizes, not absolute sizes
        positiveside=X->getCommandedPosition();


        // make move smaller than width of detent to allow the motor to go "In Range"
        //delay(500);  //Allow time for offest move to complete

        //command longer move that is garunteed to hit the other side of detent
        X->move(300);
		delay(50);
        //Spin on command done in case motor doesn't go "Out of Range"
        while(!X->commandDone())
        {
          //Look for when motor goes "Out of Range"
          if(X->readHLFB())
          {
            //abruptly stop move once detent is hit
            X->stopMove();

            //record actively commanded position
            //NOTE: Command will lead actual position because of RAS delay
            //      This won't matter because the amount of error will be 
            //      consistent and were comparing relative sizes, not absolute sizes
             negativeside=X->getCommandedPosition();
          }
        }}}   

     //turn off solenoid
     digitalWrite(solenoid,LOW);
     delay(500);                   //Allow time for any moves to finish/ solenoid to turn off

     //Move motor to middle of detent 
     X->move(X->getCommandedPosition() - (positiveside+negativeside)/2);
     delay(500);                  //Allow more than enough time to complete centering move

     //return width
     width= positiveside - negativeside;
     return width;
}

void ClearPathCracker::findAllDetents(int *detent_locations)
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Sensing
//This section senses and measures the 12 detent positions and store the values in an array
///////////////////////////////////////////////////////////////////////////////////////////////////////////
for(int a=0; a<NumDetents; a++)
{
  detents[0][a]=findNearestDetent();
  detents[1][a]=(int)((X->getCommandedPosition()+5)/40)%40;      //The + 5 is for rounding at .875 ie: 1.875 will round 
                                                      //up to 2, 1.874 will round down to one.  
                                                      //Why? because I said so...

 // if(detents[0][a]>130)
  {
	//  detents[0][a]=0;
  }
  
  //Print out detent locations and widths
  Serial.print("Width: ");
  Serial.print(detents[0][a]);
  Serial.print(" location ");
  Serial.println(detents[1][a]);

  //move to approximate next location at higher speed
  X->setMaxVel(20000);
  X->move(-118);
  //delay to allow time to complete move 
  delay(500);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Bubble sort, oh yeah!
// detents[][] is sorted by width, widest to smallest
////////////////////////////////////////////////////////////////////////////////////////////////////////
int  n = NumDetents;
int swapWidth, swapPosition;
  for (int c = 0 ; c < ( n - 1 ); c++)
  {
    for (int d = 0 ; d < n - c - 1; d++)
    {
      if (detents[0][d] < detents[0][d+1]) /* For increasing order use > */
      {
        swapWidth       = detents[0][d];
        swapPosition    = detents[1][d];
        detents[0][d]   = detents[0][d+1];
        detents[1][d]   = detents[1][d+1];
        detents[0][d+1] = swapWidth;
        detents[1][d+1] = swapPosition;
      }
    }
  } 
  
  ////////////////////////////////////////////////////////////////////////////////////
  //Output  sorted detent locations to input array
  ////////////////////////////////////////////////////////////////////////////////////
  
for(int b=0; b<NumDetents;b++)
{
	detent_locations[b]=detents[1][b];
}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This function attempts a combination.
// if successfull the function will display the correct combination, and hang preventing any further motion
// If unsuccesful, the function will display "Combination Failed", and return to the main loop
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
boolean ClearPathCracker::TryCombination(int firstNum, int secNum, int thirdNum)
{
	X->setMaxVel(solveVelocity);
   X->setMaxAccel(solveAccel);
  //Print out combination to be attempted
    Serial.print("Attempting Combination : ");
    Serial.print(firstNum); 
    Serial.print(", ");
    Serial.print(secNum); 
    Serial.print(", ");
    Serial.println(thirdNum); 

    
  // determine starting position (0, 1600, 3200 etc all equal position 0
 long startposition = X->getCommandedPosition()%CMDRES;

 
  //depending on whether the first position is greater than the starting position,
  //command the motor to move 2 revs, plus the distance to the first position
  if(40*firstNum<startposition){  X->move(2*CMDRES - startposition+40*firstNum );}
  else{  X->move(3*CMDRES - 40*firstNum + startposition );}
   
  // wait until the command is finished
   while(!X->commandDone())
   { 
    delay(1);  //because I said so...
   }
   delay(aftermovedwell);  //After move delay to account for RAS delay, and mechanical settling.
  
  //depending on whether the second position is greater than the first position,
  //command the motor to move 1 revs, plus the distance to the second position
if(40*secNum<40*firstNum){  X->move(-2*CMDRES + 40*firstNum - 40*secNum -backlash_Comp);}
  else{  X->move(-CMDRES - 40*secNum + 40*firstNum -backlash_Comp );}
   
// wait until the command is finished
   while(!X->commandDone())
   { 
    delay(1);  //because I said so...
   }
   delay(aftermovedwell);  //After move delay to account for RAS delay, and mechanical settling.

  //depending on whether the third position is greater than the second position,
  //command the motor to move the distance to the third position
   if(thirdNum<secNum){ X->move(-40*thirdNum+40*secNum+backlash_Comp);}
  else{ X->move(CMDRES-40*thirdNum+40*secNum+backlash_Comp);}
   
// wait until the command is finished
   while(!X->commandDone())
   { 
    delay(1);  //because I said so...
   }
   delay(aftermovedwell);  //After move delay to account for RAS delay, and mechanical settling.

   //Turn on solenoid to attempt to open
   digitalWrite(solenoid,HIGH);
   delay(100);              //Allow the solenoid time to open, and for the shackle to move
   
   //Check to see if lock is opened
   if(digitalRead(lockOpenSensor))
   {
    //delay for visual effect before turning off solenoid, and disabling motor
    delay(500);
    digitalWrite(solenoid,LOW);
    X->disable();

    //print successful combination
    Serial.print("Lock Broken!!! Combination : ");
    Serial.print(firstNum); 
    Serial.print(", ");
    Serial.print(secNum); 
    Serial.print(", ");
    Serial.println(thirdNum); 

    return true;
   }
   else{  //If lock failed to open, print "Combination Failed"
    Serial.println("Combination Failed");}

   //turn off solenoid before exiting function
   digitalWrite(solenoid,LOW);
   
   //if dwell is longer than solenoid actuation (100ms) then dwell remaining time.
   if(aftermovedwell>100)
	   delay(aftermovedwell-100);
   
   return false;
}

boolean ClearPathCracker::TryAllPossibleCombinations(int thirdnum)
{
	int firstNumber;
    int secondNumber;
	
 //define traversing loop to try all possible combinations at 
 //each of the 12 detent locations starting with the widest
  for(int j=0; j<10; j++)
  {
    for(int k = 0; k<10; k++)
    {
      //Third number should be a detent location
      //Choose 2nd and 1st number by looking up the cooresponding possible 
      //combinations asociated with the current detent location
      //NOTE:  Third number mod 4 indicates which section of combinations we are working in.
       firstNumber = combinations[thirdnum%4][0][j];
       secondNumber = combinations[thirdnum%4][1][k];

      //Eliminate combinations where the first and second number, or second and third number are too close 
      if(firstNumber-secondNumber > 1 || firstNumber-secondNumber < -1)
          if(secondNumber-thirdnum > 1 || secondNumber-thirdnum < -1)
			  if(firstNumber!=thirdnum)
                if(TryCombination(firstNumber,secondNumber,thirdnum))
					return true;
    }
  }
  return false;
 }


