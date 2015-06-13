    
    
    #include <SPI.h>
    #include <SD.h>
    byte AziDB[];
    
    void setup() {
     Serial.begin(9600);
     Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
     pinMode(10, OUTPUT);
   
    if (!SD.begin(4)) {
      Serial.println("initialization failed!");
    }
      Serial.println("initialization done.");
    }

    void loop() {
      // put your main code here, to run repeatedly:
    SD.open(AzDBSD[], FILE_WRITE);
    SD.read(AziDB,j);
    }
