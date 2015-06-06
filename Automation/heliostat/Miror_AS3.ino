

//**************************************************************
//  Open Souce Écologie - Solar Heliostat controller
//  Name: SeguiLSol25 - Miror_b
//  Authors: Andrea Sannuto @ndrsan, Jean Walter, 
//  Website: www.osefrance.fr  @OSE_Fr
//  Rev 001 
//  Date 08 April 2015
// This Readme files is meant to collect all the intended modification that need to be integrated into the software.
// this software is released under the GPL V.3 
// for more recents update, check out the git page: https://github.com/osefrance/SolarConcentrator
//**************************************************************

/*
release 1:
- change routines to classes and use State-machine instead of using delay
- reset to 0 position using an optical sensor (as interrupt)
- first setup on the morning (need rtc implementation) using 2 light sensors on the absorber edges (sensors as interrupts)
- last setup on the evening in order to understand the error and use the new delay time for the day after.
release 2:
- multiple mirrors using an IC (need wire implementation)
release 3:
- safety shut off of the mirrors
- statistical information
...
*/


  // MOTEUR PAS A PAS:
  int steps = 200; // modify based on how many steps for a full revolution (ie if stepper is 1.8° 360°/1.8°= 200 steps)
  int microStep = 16; // modify based on microstep configuration
  int totSteps = steps; // added ansenio
  int stepPin = 3;// borne controle pas
  int dirPin = 2;//bvorne de direction
  int a = 0;// totaliseur de steps
  int aGmax;//valeur de totalisateur de step correspondant  lecture maximale photoR gauche
  int aDmax = 0;//valeur de totalisateur de step correspondant  lecture maximale photoR gauche
  int x;//nombre de steps
  int sun;//nombres de pas correspondant a angle pour rflection optimal
  int enablePin = 6;
  //long totSec = 86400;// un jour
 // long del = (totSec/totStep)*mil;//
  //long previousMillis = 0;
  // CAPTEURS OPTIQUES: PHOTORESISTANCES, THERMISTANCES ET PANNEAU PHOTOVOLTAIQUE
  int PhG=A0; // photoresistance horizontale gauche ROUGE (vue face soleil)
  int PhD=A1;// photoresistance horizontale droite MARON
  int PhGA;//lecture analogique A0 horizontale gauche
  int PhDA;//lecture analogique A1 horizontale droite
  int PhGmax = 0;//lecture la valeur maximale en PWM de la photoresistance gauche
  int PhDmax = 0;//lecture actuelle de la valeur en PWM de la photoresistance droite
  //BOUTON POUSSOIR 
  int bpouspin = 7;// bouton poussoir
  int bpouspinstate= 0;// high/low
  
  void setup() { // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PhG,INPUT);
  pinMode(PhD,INPUT);
  pinMode(bpouspin, INPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(stepPin,OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin,LOW); // Set Enable low
  }
  
  void loop() {// put your main code here, to run repeatedly:
int Ebpous = digitalRead(bpouspin);
  if (Ebpous == HIGH && bpouspinstate == 0) {
    aGmax = 0;
    aDmax = 0;
    totSteps = steps * microStep; // added ansenio
    bpouspinstate = 1;// le bouton poussoir devra passer par off avant un nouveau cycle
    digitalWrite(dirPin,HIGH); // Set Dir high
      for(x = 0; x < totSteps; x++) // Loop totSteps times 
      { 
      PhGA = analogRead(PhG);// lecture photorésistance gauche
      PhDA = analogRead(PhD);// lecture photorésistance droite
        if (PhGA > PhGmax) {
          PhGmax = PhGA; // valeur maximale photoresistance gauche 
          aGmax = x;// enregistre numbre de steps correspondant  lecture max
         }
        if (PhDA > PhDmax) {
          PhDmax = PhDA; // valeur maximale photoresistance droite
          aDmax = x;
         }
      digitalWrite(stepPin,HIGH); // Output low 
      delay(1); //  
      digitalWrite(stepPin,LOW); // Output low 
      delay(1); //   
      Serial.print("nbre pas");
      Serial.print(",");
      Serial.print(x);
      Serial.print(",");
      Serial.print("PhR G");
      Serial.print(",");
      Serial.print(PhGA);
      Serial.print(",");
      Serial.print("PhR D");
      Serial.print(",");
      Serial.print(PhDA);
      Serial.print(",");      
      Serial.print("pas max G");
      Serial.print(",");
      Serial.print(aGmax);
      Serial.print(",");      
      Serial.print("pas max D");
      Serial.print(",");
      Serial.print(aDmax);
      Serial.println(",");
      digitalWrite(dirPin,LOW); // Set Dir high  
    }
    sun = (aGmax + aDmax)/2;
    Serial.print(" pas max");
    Serial.print(",");
    Serial.print(sun);
    digitalWrite(dirPin,LOW); // Set Dir EN RETOUR
      for(x = 0; x < totSteps; x++) // Loop 200 times 
      { 
      digitalWrite(stepPin,HIGH); // Output low 
      delay(1); //  
      digitalWrite(stepPin,LOW); // Output low 
      delay(2); // 
      }
}	
  Ebpous = digitalRead(bpouspin);
  if (Ebpous == LOW) 
    {//le bouton est repasse par off, nouveau cycle possible
    bpouspinstate = 0;
    } 
   
}
  
  




























