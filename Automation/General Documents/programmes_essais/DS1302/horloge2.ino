#include <DS1302.h>

// Init the DS1302
//RST à la pin2
//I/O à la pin3
//SCLK à la pin4
DS1302 rtc(2, 3, 4);

// Init a Time-data structure
Time t;

void setup()
{
  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);

  // Setup Serial connection
  Serial.begin(9600);
  
  //Pour fixer l'heure au dispositif
  //On peut aussi mettre le jour, le mois et l'année
  // The following lines can be commented out to use the values already stored in the DS1302
  //rtc.setDOW(MONDAY);
  //rtc.setTime(21, 39, 0);
  //rtc.setDate(8, 6, 2015);
}

void loop()
{
  t = rtc.getTime();

  Serial.print(t.date, DEC);
  Serial.print("/");
  Serial.print(t.mon, DEC);
  Serial.print("/");
  Serial.print(t.year, DEC);

  Serial.print(" ");
  Serial.print(t.hour, DEC);
  Serial.print(":");
  Serial.print(t.min, DEC);
  Serial.print(":");
  Serial.print(t.sec, DEC);
  Serial.println();

  delay (1000); //On réalise une petite pause d'une seconde
}
