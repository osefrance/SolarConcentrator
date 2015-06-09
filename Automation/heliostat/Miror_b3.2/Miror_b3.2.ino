 
  //POSITIONNEMENT INPUT
  float Tma = 30;// Target miror Angle, as calculated from sun Azimut; an input to this module in degrees
  float Cma;//Current miror angle
  float h; //local time in hours indicated as input
  float angstep = 1.8;//angle per steps
  float angmicstep = 0.113;//angle per micro steps

  //STEPPER
  int stepPin = 3;// pin controling each step
  int dirPin = 2;//pin direction fo rotation
  int enablePin = 6;// pin to block moteur
  int microStepPin = 4;//HIGH activate micro steps (LOW: full steps, HIGH: 3200 microsteps (as wired in Seguisol 2)
  int a;// steps totolizer for loops
  int stepscount; //number of steps totaliser, will be set to 0 during miror zeroing - Cma also zeroed then
  int micstepscount;//number of micro-steps totaliser, will be set to 0 during miror zeroing
  int aLmax;//value of a at reading of the peak brithness of the left photoresistance
  int aRmax;//value of a at reading of the peak brithness of the left photoresistance
  int x;//all purpose varaible; delta reading between peak photoresistance and current reading
  int y;// all purpose variable
  int z;// all purpose variable
  int retard;//number of millis between steps ex: 2 for moving to a target, 15 for scanning with photoresistance
  
   // OPTIC SENSORS: PHOTORESISTANCES, THERMISTANCES ET  PHOTOVOLTAIC CELL
  int PhL = A0; // left photoresistance RED wire
  int PhR = A1;// right photoresistance MARON wire
  int PhLA;//lecture analogique A0 horizontale gauche
  int PhRA;//lecture analogique A1 horizontale droite
  int Insol;//indicator of insolation: average reading of two photoresistances
  int PhLmax = 0;//lecture la valeur maximale en PWM de la photoresistance gauche
  int PhRmax = 0;//lecture actuelle de la valeur en PWM de la photoresistance droite
  int ThL = A4;//left thermistance, WHITE wire
  int ThR = A5;// right thermoresistance, PURPLE wire
  int ThLA;//analog reading of left thermistance
  int ThRA;// analog reading of right resistance
  
    //PUSH PIN TO INITIATE A MIROR SEARCH FOR THE CENTRAL RECEPTOR  
  int pushbut = 7;// push button to start serach
  int pushbutstate;// push button state reading:
  int peaks; // number of steps indicating  both peaks have been passed
  int peakL;//number of steps corresponding to peak of left photoresistance
  int peakR;//number of steps corresponding to peak of right photoresistance
  int DeltaPhL;// photoresistance reading - maximum value of past readings
  int DeltaPhR;// photoresistance reading - maximum value of past readings
  
  
  void setup() {
    Serial.begin(9600);
    pinMode(pushbut, INPUT);
    pinMode(dirPin,OUTPUT);
    pinMode(stepPin,OUTPUT);
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin,LOW); // Set Enable low
    digitalWrite(microStepPin,OUTPUT); // Set Enable low
    }
  
  
  void loop() {
   peakL = 0;  //reset left peak indicator
   peakR = 0;  //reset right peak indicator
   peaks = 0;  //reset peak indicator
   PhLmax = 0;  //reset left Photoresistance max value 
   PhRmax = 0;  //reset right Photoresistance max value
   pushbutstate = digitalRead(pushbut);  // readi if the push button to start a centering procedure is ON
   if (pushbutstate == HIGH ) {// triggers to move by a Tma angle target in full steps, stops 2 degrees before target, then scan
       digitalWrite(microStepPin,LOW);//we will move to calculated miror angle in full steps
       digitalWrite(dirPin,HIGH);   // Set Dir high
       retard = 2;//small delay when moving toward a target
       y = (Tma-2)/angstep;//number of steps to reach sun azimut, stoping 2 degrees before target to switch to scan
       for(x=0; x<y; x++){ 
          turn();// turn toward sun azimut
          stepscount = stepscount + 1; //counter record number of steps
          }    
       while(peaks < 2){   //a while loop to keep scanning while the two peaks have not yet been identified
         search();   //a module that search for the two peaks
         }  // the two peaks ahve been identified, we now move the miror back the the centrola point between the peaks
      Serial.print("steps for Rmax");
      Serial.print(",");
      Serial.print(aRmax);
      Serial.print(",");
      Serial.print("steps for Lmax");
      Serial.print(",");
      Serial.print(aLmax);
      Serial.println(",");
      } 
     
   //program starts here if the zeroing push button is OFF(assumes mirors are centered)
   // a centering test (exemple, puissance du recepteur insuffisant pour l'insolation en cours)
   // may be required here 
   //THIS PART OF THE SKETCH USES THERMISTANCES TO SENSE DEVIATION OF THE REFLERCTED SUN
  
   digitalWrite(microStepPin,HIGH);//we will move to calculated miror angle in microsteps
   retard = 2;//small delay between steps impulses
   ThLA = analogRead(ThL);//left thermistance reading
   ThRA = analogRead(ThR);// right thermistance reading
   z = ThLA-ThRA;// sign of z will indictaed whic direction to take
   y = abs(z);// amplitude of thermal differential
   y = y - 15;//correction for differential between thermistance, no sun condition
  Serial.print("therm gauche");
  Serial.print(",");
  Serial.print(ThLA);
  Serial.print(",");
  Serial.print("therm droite");
  Serial.print(",");
  Serial.print(ThRA);
  Serial.print(",");
  Serial.print(" Delta therm");
  Serial.print(",");
  Serial.print(z);
  Serial.print(", ");
  Serial.print("PhR G");
  Serial.print(",");
  Serial.print(PhLA);
  Serial.print(",");
  Serial.print("PhR D");
  Serial.print(",");
  Serial.print(PhRA);
  Serial.print(","); 
  Serial.print("insolation");
  Serial.print(",");
  Serial.print(Insol);
  Serial.println(",");     
   if(y > 30){// differatial temperature indicating a drift of the refelcted beam
     if(z < 0){//checks for which direction the motor should be set
         digitalWrite(dirPin,HIGH); // change direction
         for(x=0; x<33;x++){
           turn();
           micstepscount = micstepscount +1;
         }
         Serial.print("step");
         Serial.print(",");
         Serial.print("1");
         Serial.print(",");
        }
      else{
         digitalWrite(dirPin,LOW); // change direction
         for(x=0; x<34;x++){
           turn();
           micstepscount = micstepscount - 1;
         }
         Serial.print("step");
         Serial.print(",");
         Serial.print("-1");
         Serial.print(",");
         }
       }
       else{
          Serial.print("step");
         Serial.print(",");
         Serial.print("0");
         Serial.print(",");
         }
    delay(2000);
   }
 
   void search(){//               MODULE LOOKS FOR MIROR POSITION FOR REFLECTION ON RECEPTOR
     digitalWrite(microStepPin,HIGH);//we will move to calculated miror angle in micro-steps
     digitalWrite(dirPin,HIGH);   // Set Dir high
    retard = 50;// delay with rest time for no vibration photoresistance reading
    while (peaks < 2){//routine while two candidate peaks havbe not yet been identified
      PhLA = analogRead(PhL);// left photoresistance reading      
      PhRA = analogRead(PhR);// right photoresistance reading
      if (PhLA > PhLmax) {
         PhLmax = PhLA; // peak reading left photoresistance 
         aLmax = a;// record steps number corresponding to photoresistance max reading
      }else{
         x = PhLmax - PhLA;
         if (x > 150){ //DIFFERENTIAL MAXIMUM READING - current reading for potential peak 
           peakL = 1; //a POTENTIAL left peak
             }
          }
        if (PhRA > PhRmax) {
          PhRmax = PhRA; // peak reading right photoresistance
          aRmax = a;//record steps number corresponding to photoresistance max reading
            }
            else{
             x = PhRmax - PhRA;
               if (x > 150){ //DIFFERENTIAL MAXIMUM READING - current reading for potential peak
            peakR = 1; //a POTENTIAL right peak
           }  
           }
     peaks = peakL + peakR;//peaks will stop the loop if two peaks have been identified and are within reasonable distance
      x = aRmax - aLmax;// check if peaks are within reasonnable distance from one another
      x = abs(x);
      if(x<40 || x>80){ // range of acceptable distance between peaks right and left(65 calculated)
        peakL = 0;// peaks are likely to be noise, scanning continue
        peakR = 0;// peaks are likely to be noise, scanning continue
        }
      else{
        a = micstepscount - (aRmax + aLmax)/2;//number of steps corresponding to reflection on the center of the receptor
        Serial.print("steps return");
        Serial.print(",");
        Serial.print(a);
        Serial.print(",");
        digitalWrite(dirPin,LOW); // change direction
        for (x = 0; x < a; x++) {//goes back the number of steps for reflection centered on the receptor
          turn();
          micstepscount = micstepscount - 1;//takes a microstep off while returning
          }
    turn();
    micstepscount = micstepscount + 1;//counting microsteps
    Serial.print("steps");
    Serial.print(",");
    Serial.print(a); 
    Serial.print(",");
    Serial.print("Ph L");
    Serial.print(",");
    Serial.print(PhLA); 
    Serial.print(",");
    Serial.print("Ph R");
    Serial.print(",");
    Serial.print(PhRA);
    Serial.print(",");
    Serial.print("  step max L");
    Serial.print(",");
    Serial.print(aLmax);
    Serial.print(",");
    Serial.print("  step max R");
    Serial.print(",");
    Serial.print(aRmax);
    Serial.println(",");
      }
    a++;//one more microstep
    }   
  }
  
void turn(){ //MODULE ACTIVATES ONE STEP
  digitalWrite(stepPin,HIGH);  
    delay(1); //  
    digitalWrite(stepPin,LOW); 
    delay(retard); //
} 
