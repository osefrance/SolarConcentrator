 
  //STEPPER
  int stepPin = 3;// pin controling each step
  int dirPin = 2;//direction fo rotation
  int enablePin = 6;// 
  int a;// steps totolizer
  int aLmax;//value of a at reading of the peak brithness of the left photoresistance
  int aRmax = 0;//value of a at reading of the peak brithness of the left photoresistance
  int x;//number of steps in for loop
  int sun;//value of a for a central position of the center between the two photoresistance peak readings
  
   // OPTIC SENSORS: PHOTORESISTANCES, THERMISTANCES ET  PHOTOVOLTAIC CELL
  int PhL=A0; // left photoresistance RED wire
  int PhR=A1;// right photoresistance MARON wire
  int PhLA;//lecture analogique A0 horizontale gauche
  int PhRA;//lecture analogique A1 horizontale droite
  int PhLmax = 0;//lecture la valeur maximale en PWM de la photoresistance gauche
  int PhRmax = 0;//lecture actuelle de la valeur en PWM de la photoresistance droite
  
    //PUSH PIN TO INITIATE A MIROR SEARCH FOR THE CENTRAL RECEPTOR  
  int pushbut = 7;// push button to start serach
  int pushbutstate;// push button state reading:
  int peaks = 0; // number of steps indicating  both peaks have been passed
  int peakL;//number of steps corresponding to peak of left photoresistance
  int peakR;//number of steps corresponding to peak of right photoresistance
  int DeltaPhL;// photoresistance reading - maximum value of past readings
  int DeltaPhR;// photoresistance reading - maximum value of past readings
  
  void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(pushbut, INPUT);
    pinMode(dirPin,OUTPUT);
    pinMode(stepPin,OUTPUT);
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin,LOW); // Set Enable low
    }
  
  void loop() {
    // put your main code here, to run repeatedly:
  pushbutstate = digitalRead(pushbut);
  if (pushbutstate == HIGH ) {
    digitalWrite(dirPin,HIGH); // Set Dir high
      while(a < 3000){
       search();
      }
    a = 0;
    Serial.print("max PhL reading");
    Serial.print(",");
    Serial.print(PhLmax);
    Serial.print(",");
    Serial.print("steps for L");
    Serial.print(",");
    Serial.print(aLmax);
    Serial.println(",");
     } 
   }
   void search(){
    PhLA = analogRead(PhL);// left photoresistance reading      
    PhRA = analogRead(PhR);// right photoresistance reading
    if (PhLA > PhLmax) {
          PhLmax = PhLA; // peak reading left photoresistance 
          aLmax = a;// record steps number corresponding to photoresistance max reading
         }
        if (PhRA > PhRmax) {
          PhRmax = PhRA; // peak reading right photoresistance
          aRmax = a;//record steps number corresponding to photoresistance max reading
         }
    digitalWrite(stepPin,HIGH); // Output low 
    delay(1); //  
    digitalWrite(stepPin,LOW); // Output low 
    delay(2); // 
    a++;
   }   
 
