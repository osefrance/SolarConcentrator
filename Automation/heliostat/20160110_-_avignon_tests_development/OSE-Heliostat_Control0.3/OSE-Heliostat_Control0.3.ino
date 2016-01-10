#include <Wire.h> // to control the MCP23017 to control the step on all motors

//**************************************************************
//  Open Souce Écologie - Solar Heliostat controller
//  Name: OSE Heliostat Control
//  Authors: Andrea Sannuto @ndrsan @librehw, Jean Walter , Remi @Remi_R, Cherif 
//  Website: www.osefrance.fr  @OSE_Fr
//  Rev 003
//  Date 10 Jan 2015
// features, micro step to follow the sun or heliostat with 1 axis only, as fast as it can to be the most precise.  
// this software is released under the GPL V.3 
// for more recents update, check out the git page: https://github.com/osefrance/SolarConcentrator
// http://bit.ly/1JnwSGd
//  other Open Source Code from:
// LibreHardware.net : seguilsol25-testphotodiode.ino to react to the light and light on the led based on a variable threshold controlled by a potentiometer. https://github.com/librehw/photodiode
//**************************************************************


// 5 main functions (to translate in class)
// function 0 = Awake
// function 1 = Find Midpoint / calibration : 
// function 2 = offset mirror 1 and n
// function 3 = Follow the sun, thanks to the offset mirrors and the interrupts of the photoresistors positioned on the edge of the absorber
// function 4 = sleep. go in position to sleep and sleep.

// Changes logs: Status: Function 3 working based on the feedback from the PdE. moves all mirrors at once when PdE reaches the threshold.
// fixed debug functions for some function, changed buttons pin settings
// Tested up to 8 stepper motors
// Added condition to switch to b port when over 8 motors in turn() function - it needs to change with a case select and the direct pin configuration for either A or B ports of either 1st i/o extender or second (when it will be installed for the motors from 17 to 20)
// currently I will move the motors of both a and b ports at the same time.  
// need to move both serial extender at 0x20  and 0x21
// integrating the photodiode reactivity with threshold on potentiometer.


const int mirrorsQ = 20; // how many mirrors should be controlled. if through the MCP23017 they should be connected incrementally starting from port GPA0 of MCP23017
int mirrorsQbin = 1; // mirrorsQconversion to consecutive bits rappresenting each single motors
// int stepPin[] = {3, 5, 7}; // 1 high and low cycle makes 1 step of the stepper . minimum 1 uSec per state - "A low-to-high transition on the STEP input sequences the translator and advances the motor one increment. "


int mirrorSel = 1; // index for the mirror selector when using the MCP23017 it will be set to 255 to step on all motors at the same time (set to 0 if used as index for stepPin[] matrix, set to 1 if used as pointer for the MCP23017 port extender

const int dirPin = 2;  // This determines the direction of rotation of the motor. Changes to this input do not take effect until the next STEP rising edge.
const int enablePin = 6; // This input turns on or off all of the FET outputs. When set to a logic high, the outputs are disabled. When set to a logic low, the internal control enables the outputs as required. The translator inputs STEP, DIR, and MSx, as well as the internal sequencing logic, all remain active, independent of ENABLE input state.
const int microStepPin = 4; // This input turns on or off all of the FET outputs. When set to a logic high, the outputs are disabled. When set to a logic low, the internal control enables the outputs as required. The translator inputs STEP, DIR, and MSx, as well as the internal sequencing logic, all remain active, independent of ENABLE input state.


// pin settings
int pushWButPin = 12; // push button to move motors manually to West of 1 (micro)step
int pushEButPin = 11; // push button to move motors manually to Est of 1 (micro)step
int potPin = A0; // potentiometer to set the movement speed of the motor - temporary setting

// Joystick control
 int jButPin = 10; // mirror select pin
 // int jPotxPin = A3; // 
//  int jPotyPin = A1; // 

// DEBUG MODE 1 on, 0 off. check serial for output of the variables value.
  int debugmode = 0;
// int debugmode = 1;


// Variables
int dir ; // set the direction
int spd ; // set the speed - the higher the slower

// photosensor related variables

const int pdPin[]= {A3} ; // matrix of the photodiodes. - say A3 for the east photodiode (PdE in the schematics) and Ax for the western one (PdW). PdE is the driving photodiode triggering the motors to step if excited. PdW should be mostly used for calibration 
float pdThreshold = 3.0 ; // threshold at which the sun hits the photodiode, used for calibration and for heliostat function
int photoSensPotPin = A1; // potentiometer to set the photoresistor sensitivity - new setting

void setup() {
  // put your setup code here, to run once:
/*setup of motor's pins connected on the arduino (commented out if you use the MCP23017
for (int a=0; a<mirrorsQ; a++){
  pinMode(stepPin[a], OUTPUT);
}
*/

pinMode(dirPin, OUTPUT);
pinMode(enablePin, OUTPUT);  
pinMode(microStepPin, OUTPUT);  
pinMode(pushWButPin, INPUT);  
pinMode(pushEButPin, INPUT);  
pinMode(jButPin, INPUT);  


// MCP23017 setup
 Wire.begin(); // wake up I2C bus

// set I/O pins to outputs for MCP23017 hardware address 0x20
 
 Wire.beginTransmission(0x20);
 Wire.write(0x00); // IODIRA register
 Wire.write(0x00); // set all of port A to outputs
 Wire.endTransmission();
 Wire.beginTransmission(0x20);
 Wire.write(0x01); // IODIRB register
 Wire.write(0x00); // set all of port B to outputs
 Wire.endTransmission();


// set I/O pins to outputs for MCP23017 hardware address 0x21 
 Wire.beginTransmission(0x21);
 Wire.write(0x00); // IODIRA register
 Wire.write(0x00); // set all of port A to outputs
 Wire.endTransmission();
 Wire.beginTransmission(0x21);
 Wire.write(0x01); // IODIRB register
 Wire.write(0x00); // set all of port B to outputs
 Wire.endTransmission();

 
if (debugmode == 1) { 
  Serial.begin (9600); 
  }
 
for (int a=1; a<= mirrorsQ; a++) { // converts the last motor decimal identifier in to binary for later comparison in selectMirror() when switching from one motor to the next.
   mirrorsQbin= mirrorsQbin * 2;
   }
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(enablePin,HIGH ); // set to low to keep the stepper motors engaged in the position. it consumes power but essential in direct drive with environmental perturbances (wind) 
 digitalWrite(microStepPin, HIGH); // microstepping enabled (based on hardware configuration HHH = 1/16th)
// digitalWrite(microStepPin, LOW); // microstepping disabled

// int jButRead = digitalRead(jButPin);

if (digitalRead(jButPin) == HIGH )   { selectMirror(); }

if (digitalRead(pushWButPin) == HIGH  ) {
    dir = 0;
    turn(dir,mirrorSel);//moves the mirror(s)
    return ;
    }
if (digitalRead(pushEButPin) == HIGH ) {
    dir = 1;
    turn(dir, mirrorSel);//moves the mirror(s)
    return ;
    }  

float pdreading = readPd (pdPin[0]);
if (pdreading < pdThreshold)  {
    dir = 1;
    turn(dir, 255);//moves all mirrors
    }  
}

void turn (int dirT, int spin){
    digitalWrite(enablePin, LOW);
    digitalWrite(dirPin, dirT);

  spd = map(analogRead (potPin), 1023, 780, 1,3000);
/*  
 *   these 4 lines can be used if controlling 1 stepper motor only without mcp23017 ... can be left for later reference when solar concentrator will use a single stepper motor only
  digitalWrite(stepPin[spin], HIGH);
  delayMicroseconds(spd);
  digitalWrite(stepPin[spin], LOW);
  delayMicroseconds(spd);
*/

  if (debugmode == 1) {   
  Serial.print ("inside turn, variables: spin ; dirT "); 
  Serial.print (spin); 
  Serial.print (" ; "); 

  Serial.println (dirT); 
  } 

// MCP23017 address 0x20

// turn A Ports ON based on mirrorSel (Make 1 step)
  Wire.beginTransmission(0x20);
       Wire.write(0x12); // GPIOA
       Wire.write(spin); // port A
  Wire.endTransmission();
  
  if (debugmode == 1) {   
  Serial.print ("inside turn - after if port A, variables: spin ; dirT "); 
  Serial.print (spin); 
  Serial.print (" ; "); 

  Serial.println (dirT); 
  } 

     // B Ports
  //   int mirrorpin = spin - 255;
     Wire.beginTransmission(0x20);
       Wire.write(0x13); // GPIOB
       Wire.write(spin); // port B
     Wire.endTransmission();
     
  if (debugmode == 1) {   
  Serial.print ("inside turn - after if port B, variables: spin ; mirrorpin ; dirT "); 
  Serial.print (spin); 
  Serial.print (" ; "); 
  //  Serial.print (mirrorpin); 
  Serial.print (" ; "); 
  Serial.println (dirT); 
  } 

// MCP23017 address 0x21

// turn A Ports ON based on mirrorSel (Make 1 step)
  Wire.beginTransmission(0x21);
       Wire.write(0x12); // GPIOA
       Wire.write(spin); // port A
  Wire.endTransmission();
  
  if (debugmode == 1) {   
  Serial.print ("inside turn - after if port A, variables: spin ; dirT "); 
  Serial.print (spin); 
  Serial.print (" ; "); 

  Serial.println (dirT); 
  } 

     // B Ports
  //   int mirrorpin = spin - 255;
     Wire.beginTransmission(0x21);
       Wire.write(0x13); // GPIOB
       Wire.write(spin); // port B
     Wire.endTransmission();
     
  if (debugmode == 1) {   
  Serial.print ("inside turn - after if port B, variables: spin ; mirrorpin ; dirT "); 
  Serial.print (spin); 
  Serial.print (" ; "); 
  //  Serial.print (mirrorpin); 
  Serial.print (" ; "); 
  Serial.println (dirT); 
  } 


     
  delayMicroseconds(spd);


  // turn OFF A Ports first MCP23017 serial 0x20
 
  Wire.beginTransmission(0x20);
       Wire.write(0x12); // GPIOA
       Wire.write(0x00); // port A
  Wire.endTransmission();
 
 // turn off B Ports first MCP23017 serial  0x20
 
  Wire.beginTransmission(0x20);
       Wire.write(0x13); // GPIOB
       Wire.write(0x00); // port B
  Wire.endTransmission();

  // turn OFF A Ports second MCP23017 serial 0x21 
 
  Wire.beginTransmission(0x21);
       Wire.write(0x12); // GPIOA
       Wire.write(0x00); // port A
  Wire.endTransmission();
 
 // turn off B Ports second MCP23017 serial  0x21
 
  Wire.beginTransmission(0x21);
       Wire.write(0x13); // GPIOB
       Wire.write(0x00); // port B
  Wire.endTransmission();
 
 
 delayMicroseconds(spd);
if (debugmode == 1) {   
      Serial.print ("mirrorSel ; mirrorsQbin ");  
      Serial.print (mirrorSel); 
      Serial.print (" ; "); 
      Serial.println (mirrorsQbin); 
    }

}

 void selectMirror() {
  mirrorSel = 255;
/*      
      mirrorSel = mirrorSel *2;
      if (mirrorSel == mirrorsQbin) { mirrorSel = 255;}
      else if (mirrorSel > mirrorsQbin) { mirrorSel = 1;}
      while (digitalRead(jButPin) == HIGH )   {}
*/
  }


float readPd (int photoDiodedPin) // read the photo diode 
{
 // pdThreshold = map(analogRead (photoSensPotPin), 520, 1023, 1.0, 5.0);
 pdThreshold = analogRead (photoSensPotPin);
 pdThreshold = pdThreshold  / 1023 * 5.0;
 int pdRead = analogRead( photoDiodedPin);
 float pdVolt = pdRead / 1024.0 * 5.0;
 if (debugmode == 1)
  {
  Serial.begin (9600);
  Serial.print("inside pdRead : pdThreshold; pdVolt ; pdRead = ");
  Serial.print(analogRead (photoSensPotPin));
  Serial.print(" ; ");
  Serial.print(pdThreshold);
  Serial.print(" ; ");
  Serial.print(pdVolt);
  Serial.print(" ; ");
  Serial.print(pdRead);
  Serial.println();
  }
  
return pdVolt;
}



/*
Board  I2C / TWI pins
Uno, Ethernet A4 (SDA), A5 (SCL)
Mega2560  20 (SDA), 21 (SCL)
Leonardo  2 (SDA), 3 (SCL)
Due 20 (SDA), 21 (SCL), SDA1, SCL1

MCP23017 I2C chip
SCL = 12
SDA = 13

// pins 15~17 to GND, I2C bus address is 0x20
*/

// binary table for cycling


/* DRV8825 layout
MODE2 MODE1 MODE0 STEP MODE
0 0 0 Full step (2-phase excitation) with 71% current
0 0 1 1/2 step (1-2 phase excitation)
0 1 0 1/4 step (W1-2 phase excitation)
0 1 1 8 microsteps/step 
1 0 0 16 microsteps/step
1 0 1 32 microsteps/step
1 1 0 32 microsteps/step
1 1 1 32 microsteps/step
*/


// reference; first 8 bits 1111 1111 = 255b10 = 0xff  
// seconds 8 bits 1111 1111 0000 0000 = 65280b10 = 0xff00  (65535 = ffff)
// third 4 bits = 983040b10 = 0xf0000 
