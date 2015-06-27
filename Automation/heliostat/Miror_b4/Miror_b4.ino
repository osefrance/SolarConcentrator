 
  //STEPPER
  int stepPin1 = 31;// pin controling each step of motor 1 (yellow wire)
  int dirPin1 = 33;//pin direction fo rotation of motor 1 (white wire)
  int enablePin1 = 35;// pin to block moteur of motor 1 (green wire)
  int stepPin2 = 31;// pin controling each step of motor 2 (yellow wire)
  int dirPin2 = 33;//pin direction fo rotation of motor 2 (white wire)
  int enablePin2 = 35;// pin to block moteur of motor 2 (green wire)
  int stepPin3 = 37;// pin controling each step of motor 2 (yellow wire)
  int dirPin3 = 39;//pin direction fo rotation of motor 2 (white wire)
  int enablePin3 = 41;// pin to block moteur of motor 2 (green wire)

  int a;// steps totolizer
  //int stepscount; //number of steps totaliser, will be set to 0 during miror zeroing - Cma also zeroed then
  int aLmax;//value of a at reading of the peak brithness of the left photoresistance
  int aRmax;//value of a at reading of the peak brithness of the left photoresistance
  int x;//all purpose varaible; delta reading between peak photoresistance and current reading
  int y;// all purpose variable
  int z;// all purpose variable
  
   // OPTIC SENSORS: PHOTORESISTANCES, THERMISTANCES ET  PHOTOVOLTAIC CELL
  int PhL = A0; // left photoresistance RED wire
  int PhR = A1;// right photoresistance MARON wire
  int PhLA;//lecture analogique A0 horizontale gauche
  int PhRA;//lecture analogique A1 horizontale droite
  int Insol;//indicator of insolation: average reading of two photoresistances
  int peaks; // number of steps indicating  both peaks have been passed
  int peakL;//number of steps corresponding to peak of left photoresistance
  int peakR;//number of steps corresponding to peak of right photoresistance
  int DeltaPhL;// photoresistance reading - maximum value of past readings
  int DeltaPhR;// photoresistance reading - maximum value of past readings
  int Insol;//indicator of insolation: average reading of two photoresistances
  int PhLmax;//lecture la valeur maximale en PWM de la photoresistance gauche
  int PhRmax;//lecture actuelle de la valeur en PWM de la photoresistance droite
  int ThL = A4;//left thermistance, WHITE wire
  int ThR = A5;// right thermoresistance, PURPLE wire
  int ThLA;//analog reading of left thermistance
  int ThRA;// analog reading of right resistance
  int LedYL = A12; //Left yellow LED (yellow wire)
  int LedBL = A13; // Left blue LED (blue wire)
  int LedYR = A14;// Right yellow LED (yellow wire)
  int LedBR = A15;// Rifght blue LED (blue wire)
  int LedYLA; //Analog reading of left yellow LED
  int LedBLA; //Analog reading of left blue LED
  int LedYRA; //Analog reading of right yellow LED
  int LedBRA; //Analog reading of right blue LED
  
    //PUSH PIN TO INITIATE A MIROR SEARCH FOR THE CENTRAL RECEPTOR  
  int pushbut = 22;// push button to start serach
  int pushbutstate;// push button state reading:
  int FDCM1 = 24;// Contact switch for Miror 1
  int FDCM2 = 26;// Contact switch for Miror 2
  int FDCM3 = 28;// Contact switch for Miror 3

  
  void setup() {
    Serial.begin(9600);
    pinMode(pushbut, INPUT);
    pinMode(dirPin,OUTPUT);
    pinMode(stepPin,OUTPUT);
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin,LOW); // Set Enable low
    digitalWrite(microStepPin,LOW); // Set Enable low
    }
  
  void loop() {// MOTOR 2 ONLY (CENTRAL MIROR)
   peakL = 0;  //reset left peak indicator
   peakR = 0;  //reset right peak indicator
   peaks = 0;  //reset peak indicator
   PhLmax = 0;  //reset left Photoresistance max value 
   PhRmax = 0;  //reset right Photoresistance max value
  pushbutstate = digitalRead(pushbut);  // readi if the push button to start a centering procedure is ON
  if (pushbutstate == HIGH ) {
    digitalWrite(dirPin2,HIGH);   // Set Dir high
      while(peaks < 2){   //a while loop to keep scanning while the two peaks have not yet been identified
       search();   //a module that search for the two peaks
       }  // the two peaks ahve been identified, we now move the miror back the the centrola point between the peaks 
     }//PROGRAM STARTS HERE IF PUSH BUTTON IS OFF
   ThLA = analogRead(ThL);//left thermistance reading
   ThRA = analogRead(ThR);// right thermistance reading
   z= ThLA-ThRA;
   y = abs(z);
   y = y - 15;//correction for differential between thermistance (no sun)
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
   if(y > 30){//activate motor if differatial temperature between therm exceeds 30 ua
     if(z < 0){//checks for which direction the motor should be set
       digitalWrite(dirPin2,HIGH); // change direction
         for(x=0; x<33;x++){
           turn();
           a = a +1;
         }
         Serial.print("step");
         Serial.print(",");
         Serial.print("1");
         Serial.print(",");
        }
      else{
         digitalWrite(dirPin2,LOW); // change direction
         for(x=0; x<34;x++){
           turn();
           a = a - 1;
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
   }



 
    void search(){//    MODULE LOOKS FOR MIROR POSITION FOR REFLECTION ON RECEPTOR
     digitalWrite(microStepPin,HIGH);//we will move to calculated miror angle in micro-steps
     digitalWrite(dirPin,HIGH);   // Set Dir high
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
        stepscount = a - (aRmax + aLmax)/2;//number of steps corresponding to reflection on the center of the receptor
        Serial.print("steps return");
        Serial.print(",");
        Serial.print(stepscount);
        Serial.print(",");
        digitalWrite(dirPin2,LOW); // change direction
        for (x = 0; x < stepscount; x++) {//goes back the number of steps for reflection centered on the receptor
          turn();
          }
        } 
    turn();
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



  
void turn(){ //MODULE ACTIVATES ONE STEP
  digitalWrite(stepPin,HIGH);  
    delay(1); //  
    digitalWrite(stepPin,LOW); 
    delay(1); //
} 
