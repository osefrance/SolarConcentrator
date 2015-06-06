 
  //STEPPER
  int stepPin = 3;// pin controling each step
  int dirPin = 2;//direction fo rotation
  int enablePin = 6;// 
  int microStepPin = 4;//activate miucro steps (LOW: full steps, HIGH: microsteps (as wired in Seguisol 2)
  int microStep = 16; // set the number of microsteps of your wiring.
  int a;// steps totolizer
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
  int PhLmax = 30;//lecture la valeur maximale en PWM de la photoresistance gauche
  int PhRmax = 30;//lecture actuelle de la valeur en PWM de la photoresistance droite
  int ThL = A4;//left thermistance, WHITE wire
  int ThR = A5;// right thermoresistance, PURPLE wire
  int ThLA;//analog reading of left thermistance
  int ThRA;// analog reading of right resistance
  
    //PUSH PIN TO INITIATE A MIROR SEARCH FOR THE CENTRAL RECEPTOR  
  int pushbut = 7;// push button to start serach
  int pushbutstate;// push button state reading:
  int peaks = 0; // number of steps indicating  both peaks have been passed
  int peakL;//number of steps corresponding to peak of left photoresistance
  int peakR;//number of steps corresponding to peak of right photoresistance
  int DeltaPhL;// photoresistance reading - maximum value of past readings
  int DeltaPhR;// photoresistance reading - maximum value of past readings
  int microTrig = 0; //
   int runonce = 1;
  void setup() {
    Serial.begin(9600);
    pinMode(pushbut, INPUT);
    pinMode(dirPin,OUTPUT);
    pinMode(stepPin,OUTPUT);
    pinMode(enablePin, OUTPUT);
    pinMode(microStepPin,OUTPUT);
    digitalWrite(enablePin,LOW); // Set Enable low
    digitalWrite(microStepPin,LOW); // Set Enable low
    }
  
  void loop() {
   peakL = 0;  //reset left peak indicator
   peakR = 0;  //reset right peak indicator
   peaks = 0;  //reset peak indicator
   PhLmax = 0;  //reset left Photoresistance max value 
   PhRmax = 0;  //reset right Photoresistance max value
  pushbutstate = digitalRead(pushbut);  // readi if the push button to start a centering procedure is ON
  if (pushbutstate == HIGH ) {
    microTrig = 0;
    digitalWrite(dirPin,HIGH);   // Set Dir high
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
    a = a - (aRmax + aLmax)/2;//number of steps corresponding to reflection on the center of the receptor
    Serial.print("steps return");
    Serial.print(",");
    Serial.print(a);
    Serial.print(",");
    digitalWrite(dirPin,HIGH); // change direction
    for (x = 0; x < a; x++) {//goes back the number of steps for reflection centered on the receptor
   turn();
   Serial.println(x);
     } 
     }//program starts here if the zeroing push button is OFF
   PhLA = analogRead(PhL);// left photoresistance reading      
   PhRA = analogRead(PhR);// right photoresistance reading
   Insol = (PhRA + PhLA)/2;//gives an idea of the ambiant sun intensity
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
       digitalWrite(dirPin,HIGH); // change direction
       digitalWrite(stepPin,HIGH); // Output low 
       delay(1); //  
       digitalWrite(stepPin,LOW); // Output low 
       delay(1); //
       Serial.print("step");
       Serial.print(",");
       Serial.print("1");
       Serial.print(",");
        }
      else{
        digitalWrite(dirPin,LOW); // change direction
   turn();
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
 
   void search(){// MODULE LOOKS FOR MIROR POSITION FOR REFLECTION ON RECEPTOR
    PhLA = analogRead(PhL);// left photoresistance reading      
    PhRA = analogRead(PhR);// right photoresistance reading

 if (microTrig > 0 & runonce > 0)  {
          digitalWrite(dirPin,HIGH); // change direction
          turn();
          turn();
          turn();
          turn();
          turn();
          digitalWrite(dirPin,LOW); // change direction
          digitalWrite(microStepPin,HIGH); // enter in microstep 
          runonce = 0;
          Serial.print("runonce = ");
          Serial.println(runonce);
 
  }
         else{
 
    if (PhLA > PhLmax) {
          PhLmax = PhLA; // peak reading left photoresistance 
          aLmax = a;// record steps number corresponding to photoresistance max reading
       }
         else{
           x = PhLmax - PhLA;
             if (x > 200){ //differential max reading - current reading for potential peak 
                peakL = 1; //a POTENTIAL left peak
                microTrig = 1;
             }
          }
        if (PhRA > PhRmax) {
          PhRmax = PhRA; // peak reading right photoresistance
          aRmax = a;//record steps number corresponding to photoresistance max reading
            }
            else{
             x = PhRmax - PhRA;
               if (x > 150){ //differential max reading - current reading for potential peak
            peakR = 1; //a POTENTIAL right peak
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
    Serial.print(",");
     Serial.print("  MicroTrig");
    Serial.print(",");
    Serial.print(microTrig);
    Serial.println(",");
 
    x = aRmax - aLmax;// check if peaks are within reasonnable distance from one another
    x = abs(x);
    if(x<15 || x>40){ // range of acceptable distance between peaks right and left 
      peakL = 0;// peaks are likely to be noise, scanning continue
      peakR = 0;// peaks are likely to be noise, scanning continue
    } 
    peaks = peakL + peakR;//peaks will stop the loop if two peaks have been identified and are within reasonable distance
    
 if (microTrig = 1){
          a++;
          }
      else {
        a = a+microStep;
        }


   }
 }   
void turn() {
  digitalWrite(stepPin,HIGH);  
    delay(10); //  
    digitalWrite(stepPin,LOW); 
    delay(50); //
} 
