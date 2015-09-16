#include <Wire.h> // to control the MCP23017 to control the step on all motors

//**************************************************************
//  Open Souce Écologie - Solar Heliostat controller
//  Name: SeguiLSol25
//  Authors: Andrea Sannuto @ndrsan, Jean Walter , Remi @Remi_R, Cherif 
//  Website: www.osefrance.fr  @OSE_Fr
//  Rev 001 
//  Date 08 April 2015
// features, micro step to follow the sun or heliostat with 1 axis only, as fast as it can to be the most precise.  
// this software is released under the GPL V.3 
// for more recents update, check out the git page: https://github.com/osefrance/SolarConcentrator
// http://bit.ly/1JnwSGd
//**************************************************************


// 5 main functions (to translate in class)
// function 0 = Awake
// function 1 = Find Midpoint / calibration : it must correct the steps based on the speed the sun is moving.
// function 2 = Offset mirror x to keep the photodiode always slightly under the threshold(or a special threshold) in order to drive all motors to the central position of the 
// function 3 = Heliostat / Follow the sun, thanks to the offset mirrors and the interrupts of the photoresistors positioned on the edge of the absorber
// function 4 = sleep. go in position to sleep and sleep.

// Status / Change log:
// Function 1 Calibration implementation without time check.  
// Function 3 working based on the feedback from the PdE. moves all mirrors at once when PdE reaches the threshold. slightly modified need to be retested.


// Stepper driver Pin configuration  - set up based on Pololu A4988 driver although also DRV8825 will be integrated in this design.

const int mirrorsQ = 2 ; // how many mirrors should be controlled. if through the MCP23017 they should be connected incrementally starting from port A0 of MCP23017
int mirrorsQbin = 1; // mirrorsQconversion to consecutive bits rappresenting each single motors
// int stepPin[] = {3, 5, 7}; // used only if STEP is directly connected to the arduino. not used in case of going through the i/o extender MDCP 32017.  high and low cycle makes 1 step of the stepper . minimum 1 uSec per state - "A low-to-high transition on the STEP input sequences the translator and advances the motor one increment. "

int mirrorSel = 1; // index for the mirror selector when using the MCP23017 it will be set to 255 to step on all motors at the same time (set to 0 if used as index for stepPin[] matrix, set to 1 if used as pointer for the MCP23017 port extender

const int dirPin = 2;  // This determines the direction of rotation of the motor. Changes to this input do not take effect until the next STEP rising edge.
const int enablePin = 6; // This input turns on or off all of the FET outputs. When set to a logic high, the outputs are disabled. When set to a logic low, the internal control enables the outputs as required. The translator inputs STEP, DIR, and MSx, as well as the internal sequencing logic, all remain active, independent of ENABLE input state.
const int microStepPin = 4; // This input turns on or off all of the FET outputs. When set to a logic high, the outputs are disabled. When set to a logic low, the internal control enables the outputs as required. The translator inputs STEP, DIR, and MSx, as well as the internal sequencing logic, all remain active, independent of ENABLE input state.



const int endStopPin[] = {8} ; // the pin of connection of the endstop. should use a i/o port extender as input in stead of the direct Arduino ports.

// pin settings
int pushWButPin = 12; // push button to move motors manually to West of 1 (micro)step
int pushEButPin = 11; // push button to move motors manually to Est of 1 (micro)step
int pushCalibrateButPin = 10; // push button to start the calibration sequence. temporarily activated manually for development purpose... *** to integrate in circuit ***

int potPin = A0; // potentiometer to set the movement speed of the motor - temporary setting

// Joystick control
int jButPin = 13;
// int jPotxPin = A3;
// int jPotyPin = A1;

// DEBUG MODE 1 on, 0 off. check serial for output of the variables value.
 int debugmode = 0;
// int debugmode = 1;


// Variables
const int dirEst = 1; // Change this to correct toward east direction movement of the motors. 
const int dirWest = 0; // Change this to correct toward est direction movement of the motors.  
// int dir ; // set the direction --- removed if using dirEst / dirWest
int spd ; // set the speed - the higher the slower

// initialize the matrix of position based on time ****
int stepsEl[mirrorsQ] ; // initialize the stepsEl variable
int stepsEh[mirrorsQ] ;  // initialize the stepsEh variable
int timeEl[mirrorsQ] ; // initialize the timeEl variable
int timeEh[mirrorsQ] ; // initialize the timeEh variable
int rayWidth[mirrorsQ] ;  // initialize the raywidth variable

// photosensor related variables

const int pdPin[]= {A3} ; // matrix of the photodiodes. - say A3 for the east photodiode (PdE in the schematics) and Ax for the western one (PdW). PdE is the driving photodiode triggering the motors to step when PdE gets in the sun blade reflection. PdW should be mostly used for calibration only but testing if it can be removed.
const float pdThreshold = 3 ; // threshold at which the sun hits the photodiode, used for calibration and for heliostat function

void setup() {

/*
setup of motor's pins connected on the arduino - commented out when using the MCP23017
for (int a=0; a<mirrorsQ; a++){
  pinMode(stepPin[a], OUTPUT);
}
*/

pinMode(dirPin, OUTPUT);
pinMode(enablePin, OUTPUT);
pinMode(microStepPin, OUTPUT);
pinMode(pushWButPin, INPUT);
pinMode(pushEButPin, INPUT);
pinMode(pushCalibrateButPin, INPUT);


// MCP23017 setup

 Wire.begin(); // wake up I2C bus

// set I/O pins to outputs

 Wire.beginTransmission(0x20);
 Wire.write(0x00); // IODIRA register
 Wire.write(0x00); // set all of port A to outputs
 Wire.endTransmission();
 Wire.beginTransmission(0x20);
 Wire.write(0x01); // IODIRB register
 Wire.write(0x00); // set all of port B to outputs
 Wire.endTransmission();

 if (debugmode == 1) { Serial.begin (9600); }
 
 for (int a=1; a<= mirrorsQ; a++) { // converts the decimal quantity to binary for later comparison when switching from one motor to the next
   mirrorsQbin= mirrorsQbin * 2;
   }
}

void loop() {
   // put your main code here, to run repeatedly:
   digitalWrite(enablePin,LOW ); // set to low to keep the stepper motors engaged in the position. it consumes power but essential in direct drive with environmental perturbances (wind) 
   digitalWrite(microStepPin, HIGH); // microstepping enabled (based on hardware configuration HHH = 1/16th)
   // digitalWrite(microStepPin, LOW); // microstepping disabled
  
   // int jButRead = digitalRead(jButPin);
  
  if (digitalRead(jButPin) == HIGH )   { 
      selectMirror(); 
      }
  
  if (digitalRead(pushWButPin) == HIGH ) {
      turn(dirWest,mirrorSel);//moves the mirror(s) toward West
      return ;
      }

  if (digitalRead(pushEButPin) == HIGH ) {
      turn(dirEst, mirrorSel);//moves the mirror(s) toward East
      return ;
      }  
  
  if (digitalRead(pushCalibrateButPin) == HIGH ) {
      calibrate(); // launches the calibration function 
      offsetPosition (); // set the mirror x to the offset position in order to highlight the photodiode 
      return ;
      } 
  
  
  heliostat(); // keeps the photodiode always under the threshold and moves all mirrors when the reading is higher than the threshold


}

void turn (int dirT, int spin){
    digitalWrite(enablePin, LOW);
    digitalWrite(dirPin, dirT);

  spd = map(analogRead (potPin), 1023, 780, 1,3000);
/*  digitalWrite(stepPin[spin], HIGH);
  delayMicroseconds(spd);
  digitalWrite(stepPin[spin], LOW);
  delayMicroseconds(spd);
*/

   // turn A Ports ON based on mirrorSel (Make 1 step)
if (debugmode == 1) {   Serial.println (spin); }
  Wire.beginTransmission(0x20);
       Wire.write(0x12); // GPIOA
       Wire.write(spin); // port A
  Wire.endTransmission();
     // B Ports
  /*   Wire.beginTransmission(0x20);
       Wire.write(0x13); // GPIOB
       Wire.write(a); // port B
     Wire.endTransmission();
  */
  delayMicroseconds(spd); // Delay will be removed with a state-machine variable ****

  // turn OFF A Ports
 
  Wire.beginTransmission(0x20);
       Wire.write(0x12); // GPIOA
       Wire.write(0x00); // port A
  Wire.endTransmission();
     // turn off B Ports
 /*    Wire.beginTransmission(0x20);
       Wire.write(0x13); // GPIOB
       Wire.write(0x00); // port B
     Wire.endTransmission();
  */
 
delayMicroseconds(spd); // Delay will be removed with a state-machine variable ****

if (debugmode == 1) {   Serial.print ("mirrorSel ; mirrorsQbin");  Serial.print (mirrorSel); Serial.print (" ; "); Serial.println (mirrorsQbin); }

}

void selectMirror() {
      mirrorSel = mirrorSel *2;
      if (mirrorSel > mirrorsQbin) { mirrorSel = 1;}
      while (digitalRead(jButPin) == HIGH )   {}
    }

float readPd (int photoDiodedPin) // read the photo diode 
{
int pdRead = analogRead( photoDiodedPin);
float pdVolt = pdRead / 1024.0 * 5.0;  // can be avoided to have a float variable as the pdread can be used directly and threshold can be set with a  value between 1 and 1024. ****
if (debugmode == 1)
  {
  Serial.begin (9600);
  Serial.print("inside readPD sub-Function : pdVolt ; pdRead = ");
  Serial.print(pdVolt);
  Serial.print(" ; ");
  Serial.print(pdRead);
  Serial.println();
  }
 
return pdVolt;
}

void heliostat(){ // keeps the reflected image of the sun of all mirrors centered to the receptor, by keeping the photodiode signal under the threshold

  if (debugmode == 1) {   Serial.print ("inside the heliostat function");  }
  
  //  following part is duplicated in findERW() just mirrorSel should be 255 to move all mirrors in stead of anything else. Heliostat() function can be simplified to call a modified version of findERW ****
  
  float pdreading = readPd (pdPin[0]);
  while(pdreading < pdThreshold)  {
      turn(dirEst, 255);//moves all mirrors
      pdreading = readPd (pdPin[0]);
      }  
}  

void calibrate(){ // performs the calibration sequence and memorize the matrix of values = Find0, FindEl event, FindEh event, perform the correction check,
  find0(); // Find the 0 position of each mirrors
  findERW(); // find the ray width on the eastern sensor per each motor
  //  findCorrection ();   // find the correction factor in term of steps per minute that should be applied to the mid position of each blade 
  //  midPosition (); // set all motors to the mid position 
 
} 

void find0(){ // find the 0 position for all mirrors
  mirrorSel = 1;
  for (int i = 0; i < mirrorsQ; i++) { //   sets all mirrors to 0 position
    while (endStopPin == LOW) { 
         turn(dirWest, mirrorSel);//moves the mirror(s)
    }
    selectMirror();
  }
}


void findERW(){ // find the ray width on the eastern sensor AKA:
// it creates the matrix informations with: 
// a- the number of steps (steps-el[n]) and the time (time-el[n]) when the eastern photodiode was hit by the reflected sun ray (voltage pdreading  lower than threshold)
// b- the number of steps (steps-eh[n]) and the time (time-eh[n]) when the eastern photodiode was left by the reflected sun ray (voltage pdreading higher than threshold)
// c- it defines the difference of steps that defines the ray-width at a given time for the selected mirror : rayWidth[n] = steps-eh[n]- steps-el[n]

// this function is almost duplicate with the find0() function, only direction and trigger changes. also to find El and Eh events is about the same code. can be further simplified. ****
  
  mirrorSel = 1;  // all the rest is executed starting from the first mirror. it can be improved to let select the mirror[x] inside the parent function and loop it in there. ****
  for (int i = 0; i < mirrorsQ; i++) { //   loops to work on all mirrors one after the other
 
     float pdreading = readPd (pdPin[0]); // updates the pdreading before entering the loop. I could readPd(pdPin[0]) inside the while condition and remove 2 lines of code (? to test) ****
     while(pdreading < pdThreshold)  {
      turn(dirEst, mirrorSel);//moves the mirrors one by one
      stepsEl[i]++; // increase the steps counter of 1 while in the loop.
      pdreading = readPd (pdPin[0]); // updates the pdreading at the end of the loop
      }
      // timeEl[i] = currentTime();  **** develop the function current time based on the clock DS1302 (yet to integrate)
      stepsEh[i] = stepsEl[i]; // increase the steps counter of 1 while in the loop.
      while(pdreading > pdThreshold)  {
      turn(dirEst, mirrorSel);//moves the mirrors one by one
      stepsEh[i]++ ;// increase the steps counter of 1 while in the loop.
      pdreading = readPd (pdPin[0]); // updates the pdreading at the end of the loop
      }
      // timeEh[i] = currentTime();  **** develop the function current time based on the clock DS1302 (yet to integrate)
   rayWidth[i] = steps-eh[i]- steps-el[i];
   selectMirror(); // select the next mirror
   if (mirrorSel == 1) { return;}
  }
}


/*
Board	I2C / TWI pins
Uno, Ethernet	A4 (SDA), A5 (SCL)
Mega2560	20 (SDA), 21 (SCL)
Leonardo	2 (SDA), 3 (SCL)
Due	20 (SDA), 21 (SCL), SDA1, SCL1

MCP23017 I2C chip
SCL = 12
SDA = 13

// pins 15~17 to GND, I2C bus address is 0x20
*/
