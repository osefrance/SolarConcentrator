//**************************************************************
//  Open Souce Écologie - Solar Heliostat controller
//  Name: SeguiLSol25
//  Authors: Andrea Sannuto @ndrsan, Jean Walter, 
//  Website: www.osefrance.fr  @OSE_Fr
//  Rev 001 
//  Date 08 April 2015
// features, micro step to follow the sun with 1 axis only, as fast as it can to be the most precise.  
// this software is released under the GPL V.3 
// for more recents update, check out the git page: https://github.com/osefrance/SolarConcentrator
//**************************************************************

// include : soon wire, rtc
//  #include 




// STEPPER MOTOR SETUP 
//The number of steps required for your stepper to make one revolution. (Don't forget to take into 
//account the settings on your driver board. i.e. Microstepping, half stepping etc.)
  int steps = 200;
  int microStep = 16;

// Debug information
int debug=1;    //debug off =0; on =1;
int counta = 0;
int mil = 10;  // if not debugging, set it = 1000;
int nsteps = 1; // if not debugging set it =1;

// Stepper Pin configuuration
    int stepPin = 3;
    int dirPin = 2;  
    int enablePin = 6;

// delay first setup calculation. later should be calculated based on the 2 input light sensors positioned at the absorber edges.
//  long del = ((24*3600)/(steps*microStep))*1000 ;
  int totStep = steps*microStep;
  long totSec = 86400;
  long del = (totSec/totStep)*mil;

/* for later implementations: 
// optical Limit Switch Pin Assignment
    const int limitPin = 7;


    //Manual Control Pin Assignment
    const int manualModeOnOffPin = 8;
    //Wind Protection Switch Pin Assignment
    const int WindProtectionSwitch = 10;
    //Heliostat to Sun Tracker Toggle Switch pin
    const int HeliostatToSun = 9;\
    
    //Joystick Up Down and Left Right pins.
    int joystickUDAnalogPin = A0;
    int joystickLRAnalogPin = A1;
    int invertUD = -1;//Change value from 1 to -1 to invert up/down during joystick control
    int invertLR = -1;//Change value from 1 to -1 to invert left/right during joystick control


// LATITUDE, LONGITUDE, AND TIME configuration
  float latitude = 48.86;
  float longitude = 2.36;
  float timezone = +1;
  
//If you live in the northern hemisphere, put 0 here. If you live in the southern hemisphere put 1.
  int useNorthAsZero = 0;

*/
  
void setup() {
//  Wire.begin();
if (debug==1) { Serial.begin(9600);}

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);  



 }


void loop() {
 if (debug==1) { 
Serial.println("Debug mode enabled");
Serial.print("stepping: ");
Serial.println(steps*microStep);
Serial.print("totStep: ");
Serial.println(totStep);
Serial.print("totSec: ");
Serial.println(totSec);
Serial.print("Delay to wait: ");
Serial.println(del);

}

  
moveMotor(nsteps, stepPin, dirPin);
// sun moves 0.25° every minute, 0.125° every 30 sec. 0.062° every 15sec
//stepper move 1.8° every full step, 0.9 half, 0.45 fourth, 0.225° eighth, 0.11275° sixteenth
// in order to act as heliostat the stepper should make 1/3200 (or one microstep of 1/16th of 200 full steps) every 54 seconds.
// if acting as suntracker it should microstep (at 1/16th of 200 steps)  every 27 seconds (or 27000 millis)

if (debug==1) { 
counta = counta+nsteps;
  Serial.print("steps done since last reset: ");
Serial.println(counta);
Serial.print("awaiting for delay: ");
Serial.println(del);
}
// del = del - 2;
delay (del*2);


 }


  void moveMotor(int numOfSteps,int stepPin, int dirPin){
         digitalWrite(enablePin, LOW);
          digitalWrite(dirPin, LOW);
  if (debug==1) { 
  Serial.print("inside moveMotor, numOfSteps: ");
  Serial.println(numOfSteps);
  }



     for (int doSteps=1; doSteps <= numOfSteps; doSteps++){
  if (debug==1) { 
  Serial.print(" inside the for, steps to do ");
  Serial.println(numOfSteps);
  }

       
         digitalWrite(stepPin, HIGH);
         delay(1);
         digitalWrite(stepPin, LOW);
         delay(1);
        
      } 
  if (debug==1) { 
  Serial.println(" for completed, complete moveMotor ");
  }
} 
