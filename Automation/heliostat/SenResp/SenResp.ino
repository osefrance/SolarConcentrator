 
  //STEPPER
  int stepPin[] = {31, 37, 43}; // pin 31 motor 0; 33 motor 1; 35 motor 2 (yellow wires)
  int dirPin[] = {33, 39, 45}; // pin 33 motor 0; 33 motor 1; 35 motor 2 (white wires)
  int enablePin[] = {35, 41, 47}; //pin 35 motor 0; 33 motor 1; 35 motor 2 (green wire)
  int stepping; // Pin value for loops (yellow wires)
  int sensrot; // Pin value for loops (white wires)
  int enabling; //Pin value for loops (green wire)
  int M;//motor number 0 to 2, 1 center

  //int a0;// steps totolizer motor 1
  //int a1;// steps totolizer motor 2
  //int a2;// steps totolizer motor 3
  
  //int stepscount; //number of steps totaliser, will be set to 0 during miror zeroing - Cma also zeroed then
 // int aLmax;//value of a at reading of the peak brithness of the left Protoresistance within loops
 // int aRmax;//value of a at reading of the peak brithness of the left Protoresistancewithin loops
  //int a0Lmax;//value of a at reading of the peak brithness of the left Protoresistance
  //int a0Rmax;//value of a at reading of the peak brithness of the left Protoresistance
  //int a1Lmax;//value of a at reading of the peak brithness of the left Protoresistance
 // int a1Rmax;//value of a at reading of the peak brithness of the left Protoresistance
 // int a2Lmax;//value of a at reading of the peak brithness of the left Protoresistance
 // int a2Rmax;//value of a at reading of the peak brithness of the left Protoresistance

  //int a;//steps within loop
  int x;//all purpose varaible; delta reading between peak Protoresistance and current reading
  //int y;// all purpose variable
 // int z;// all purpose variable
  //int stepscount;//variable for calculations within loops
  
   // OPTIC SENSORS: PrOTORESISTANCES, THERMISTANCES ET  PrOTOVOLTAIC CELL
  int PrL = A0; // left Protoresistance RED wire
  int PrR = A1;// right Protoresistance MARON wire
  int PrLA;//lecture analogique A0 horizontale gauche
  int PrRA;//lecture analogique A1 horizontale droite
 // int peaks; // number of steps indicating  both peaks have been passed
  //int peakL;//number of steps corresponding to peak of left Protoresistance
  //int peakR;//number of steps corresponding to peak of right Protoresistance
 // int DeltaPrL;// Protoresistance reading - maximum value of past readings
//  int DeltaPrR;// Protoresistance reading - maximum value of past readings
 // int Insol;//indicator of insolation: average reading of two Protoresistances
 // int PrLmax;//lecture la valeur maximale en PWM de la Protoresistance gauche
 // int PrRmax;//lecture actuelle de la valeur en PWM de la Protoresistance droite
  int ThL = A2;//left thermistance, WHITE wire
  int ThR = A3;// right thermoresistance, PURPLE wire
  int ThLA;//analog reading of left thermistance
  int ThRA;// analog reading of right resistance
  int LedYL = A4; //Left yellow LED (green wire)
  int LedYR = A5;// Right yellow LED (blue wire)
  int LedYLA; //Analog reading of left (green wire) LED
  int LedYRA; //Analog reading of right (blue wire) LED
  int Pv = A6;//voltmeter photovoltaique cell (grey wire)
  int PvA;// analog reading of voltmeter

  
    //PUSH PIN TO INITIATE A MIROR SEARCH FOR THE CENTRAL RECEPTOR  
  int pushbut = 22;// push button to start serach
  int pushbutstate;// push button state reading:
  int FDCM[] = {24, 26, 28}; // pin 24 fin de course 0 (blue); 26 Fin de course 1(center, white); 28 motor 2 (green)

  
  void setup() {
    Serial.begin(9600);
    pinMode(pushbut, INPUT);
    pinMode(dirPin[2],OUTPUT);
    pinMode(stepPin[2],OUTPUT);
    pinMode(enablePin[2], OUTPUT);
    digitalWrite(enablePin[2],LOW); // Set Enable low
    pinMode(dirPin[1],OUTPUT);
    pinMode(stepPin[1],OUTPUT);
    pinMode(enablePin[1], OUTPUT);
    digitalWrite(enablePin[1],LOW); // Set Enable low
    pinMode(dirPin[0],OUTPUT);
    pinMode(stepPin[0],OUTPUT);
    pinMode(enablePin[0], OUTPUT);
    digitalWrite(enablePin[0],LOW); // Set Enable low
    }
  
  void loop() {// MOTOR 1 ONLY (CENTRAL MIROR)
   //peakL = 0;  //reset left peak indicator
   //peakR = 0;  //reset right peak indicator
  // peaks = 0;  //reset peak indicator
  // PrLmax = 0;  //reset left Protoresistance max value 
  // PrRmax = 0;  //reset right Protoresistance max value
  pushbutstate = digitalRead(pushbut);  // readi if the push button to start a centering procedure is ON
  if (pushbutstate == HIGH ) {
    M = 1;
    sensrot = dirPin[M];
    stepping = stepPin[M];
    for(x=0; x<3200; x++){
      PrLA = analogRead(PrL);// left Protoresistance reading      
      PrRA = analogRead(PrR);// right Protoresistance reading
      ThLA = analogRead(ThL);//left thermistance reading
      ThRA = analogRead(ThR);// right thermistance reading
      LedYLA = analogRead(LedYL);//left LED
      LedYRA = analogRead(LedYR);//right LED
      PvA = analogRead(Pv);
      Serial.print("steps");
      Serial.print(",");
      Serial.print(x); 
      Serial.print(",");
      Serial.print("Pr L");
      Serial.print(",");
      Serial.print(PrLA); 
      Serial.print(",");
      Serial.print("Pr R");
      Serial.print(",");
      Serial.print(PrRA);
      Serial.print(",");
      Serial.print("Th L");
      Serial.print(",");
      Serial.print(ThLA); 
      Serial.print(",");
      Serial.print("Th R");
      Serial.print(",");
      Serial.print(ThRA); 
      Serial.print("LED L");
      Serial.print(",");
      Serial.print(LedYLA); 
      Serial.print(",");
      Serial.print("LED R");
      Serial.print(",");
      Serial.print(LedYRA); 
      Serial.print(",");
      Serial.print("PV V");
      Serial.print(",");
      Serial.println(PvA); 
      turn();
    }
  }
}
    
  


  
void turn(){ //MODULE ACTIVATES ONE STEP
  digitalWrite(stepping,HIGH);  
    delay(2); //  
    digitalWrite(stepping,LOW); 
    delay(4); //
} 
