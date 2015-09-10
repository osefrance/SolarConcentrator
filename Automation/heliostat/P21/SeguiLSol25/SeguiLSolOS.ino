//**************************************************************
//  Open Souce Écologie - Solar Heliostat controller
//  Name: SeguiLSol25
//  Authors: Andrea Sannuto @ndrsan, Jean Walter, 
//  Website: www.osefrance.fr  @OSE_Fr
//  Rev 001 
//  Date 08 April 2015
// features, micro step to follow the sun or heliostat with 1 axis only, as fast as it can to be the most precise.  
// this software is released under the GPL V.3 
// for more recents update, check out the git page: https://github.com/osefrance/SolarConcentrator
//**************************************************************


// 5 main functions (to translate in class)
// function 0 = Awake
// function 1 = Find Midpoint / calibration : 
// function 2 = offset mirror 1 and n
// function 3 = Follow the sun, thanks to the offset mirrors and the interrupts of the photoresistors positioned on the edge of the absorber
// function 4 = sleep. go in position to sleep and sleep.


// include : soon wire , rtc
//  #include 


#define nMirrors = 20;
int mirror = [nMirrors];

// STEPPER MOTOR SETUP 
//The number of steps required for your stepper to make one revolution. (Don't forget to take into 
//account the settings on your driver board. i.e. Microstepping, half stepping etc.)
  int steps = 200;
  int microStep = 16; /* 
// for pololu A4988 table of microstepping 
//Table 1: Microstepping Resolution Truth Table
//MS1 MS2 MS3 Microstep Resolution Excitation Mode
//L L L Full Step 2 Phase
//H L L Half Step 1-2 Phase
//L H L Quarter Step W1-2 Phase
//H H L Eighth Step 2W1-2 Phase
//H H H Sixteenth Step 4W1-2 Phase
// The MS1 and MS3 pins have a 100 kΩ pull-down resistance, and the MS2 pin has a 50 kΩ pull-down resistance.
*/

// Debug information
int debug=1;    //debug off =0; on =1;
int counta = 0; // Counter A used to count... used for debugging loops oonly ... can add more detailed description  ** 
int mil = 1000;  // if not debugging, set it = 1000; used to set the delay time. will be dismissed by using millis in stead of delays
int nsteps = 1; // if not debugging set it =1; used to loop more steps at once

// Stepper driver Pin configuration  - set up based on Pololu A4988 driver
    int stepPin = 3; // 1 high and low cycle makes 1 step of the stepper . minimum 1 uSec per state - "A low-to-high transition on the STEP input sequences the translator and advances the motor one increment. "
    int dirPin = 2;  // This determines the direction of rotation of the motor. Changes to this input do not take effect until the next STEP rising edge.
    int enablePin = 6; // This input turns on or off all of the FET outputs. When set to a logic high, the outputs are disabled. When set to a logic low, the internal control enables the outputs as required. The translator inputs STEP, DIR, and MSx, as well as the internal sequencing logic, all remain active, independent of ENABLE input state.
    int sleepPin = x ; // A logic high allows normal operation, A logic low puts the A4988 into Sleep mode. disables much of the internal circuitry including the output FETs, current regulator, and charge pump.

// delay first setup calculation. later should be calculated based on the 2 input light sensors positioned at the absorber edges.
//  long del = ((24*3600)/(steps*microStep))*1000 ;
  int totStep = steps*microStep; 
  long totSec = 86400; // tOtal seconds in a day

// state machine variables
long interval = (totSec/totStep)*mil; // interval to at which make next step
long previousMillis = 0; // will store last time motor update


  int dir = 0; // set the direction for all movements butfor find the 0 point. 0 for left or right .... **********

// Global variables
int rayWidth = 15; // ray width 
int photoDiodeDist = 15; // photo diode distance 
int midPoint = 0 ;  // find the mid point 



int lightLev = 50; // define the tolerance level for the sensor (photo diode)

/* for later implementations: 

// optical or mechanical Limit Switch Pin Assignment.  also possible through potentiometer
    const int limitPin = 7;

//Wind Protection Switch Pin Assignment
    const int WindProtectionSwitch = 10;
    
*/
  
void setup() {
//  Wire.begin();
if (debug==1) { Serial.begin(9600);} // debug session

pinMode(stepPin, OUTPUT);
pinMode(dirPin, OUTPUT);
pinMode(enablePin, OUTPUT);  
pinMode(sleepPin, OUTPUT);  

 } // end debug section



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

// pushbutstate = digitalRead(pushbut);  // read if the push button to start a centering procedure is ON
unsigned long currentMillis = millis(); //

if (pushbutstate == HIGH ) {// triggers to move by a Tma angle target in full steps, stops 2 degrees before target, then scan
	moveMotor(nsteps, dir);
   }

if (currentMillis - previousMillis > interval) { 
	previousMillis = currentMillis;
	moveMotor(nsteps, dirPin);
}

// general assumption for testing based on sun movement of 0.25° every minute, 0.125° every 30 sec. 0.062° every 15sec
// stepper move 1.8° every full step, 0.9 half, 0.45 fourth, 0.225° eighth, 0.11275° sixteenth
// in order to act as heliostat the stepper should make 1/3200 (or one microstep of 1/16th of 200 full steps) every 54 seconds.
// if acting as suntracker it should microstep (at 1/16th of 200 steps)  every 27 seconds (or 27000 millis)
// ATTENTION : torque falls down exponentially so 10% of the torque is available at 1/16 MS. about 40% of the torque available at 1/4th MS.
// ATTENTION : perceived sun movement varies based on the equation of time, which variates throughout the day and throughout the year of +- 17min.

if (debug==1) { 
	counta = counta+nsteps;
	Serial.print("steps done since last reset: ");
	Serial.println(counta);
	Serial.print("awaiting for delay: ");
	Serial.println(del);
	}
// del = del - 2;
// delay (del);


 }


void moveMotor(int numOfSteps,int dirM){
 digitalWrite(enablePin, LOW);/
 digitalWrite(dirPin, dirM);
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

void midPoint () { // this action must be cycled for every mirror.
//	findZero(!dir); // find the zero point of the first stepper motor 
	int lLight= lightLev; ///  reset to the lower light level
	int rLight= lightLev; /// reset to the lower light level
	int prevLl = lightLev;
	int prevRl = lightLev;
	for (int x=0; x << totStep; x++) {
	moveMotor(nsteps, dir); // starts turning toward the light sensor nSteps at a time
	lLight= memoLightL(); /// memorize the information and return it to local variable
	rLight= memoLightR (); /// memorize the information and return it to the local variable
	if (lLight >> prevLl) {   // if lLight current reading is higher than the previous reading register the 2 information

		prevLl = lLight;
		prevRl = rLight;
	}; 
 /// to be continued... 
	}
	return rw, pd, mp; // return the ray width, photo sensor distance and the midpoint all in microsteps
}


void offSet (int mirrA, int mirrB){ // sets 2 mirrors slightly offset, 1 toward the left sensor, the other toward the right sensor to allow sensors to send feedback of the ray position. all other mirrors rays are centered between these 2 offseted mirrors. in this way we ensure all mirrors except 2 are always centered.\
	mp[mirrA] = mp + st3;
	mp[mirrB] = mp + st2;

}  

//

void staticSun (int dir){
	for (int x=0; x<
}


