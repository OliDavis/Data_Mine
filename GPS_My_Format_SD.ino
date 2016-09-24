//===============================================================================================
//  NOTES
//===============================================================================================
/*
NOTE: this sketch is a continuation of the 'GPS_My_Format'
GPS data (lat long date and time) ia logged to the SD card
The GPS uses Serial1
Designed for Arduino Mega
*/

//===============================================================================================
//  LIbrary & Declirations
//===============================================================================================
#include <TinyGPS++.h>
#include <SPI.h>
#include <SD.h>
const int chipSelect = 9;  // Chip Select pin is tied to pin 9 on the SparkFun CAN-Bus Shield
TinyGPSPlus gps;// Create a TinyGPS++ object called "gps"

//===============================================================================================
//  SETUP
//===============================================================================================
void setup()
{
  Serial.begin(9600);//Serial Monitor
  Serial1.begin(9600);//GPS
    
  Serial.println(F("GPS_My_Format_SD.ino"));
  Serial.println(F("Arduino Mega with GPS module Logging to SD - My own data format. "));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Oliver Davis"));
  Serial.println();
  
  Serial.print("Initializing SD card...");
  pinMode(chipSelect, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
}

//===============================================================================================
//  LOOP
//    Print GPS Values.
//===============================================================================================
void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial1.available() > 0)
    if (gps.encode(Serial1.read()))
      LogToSD();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected- Check Wiring")); //error message
    while(true);
  }
}

//===============================================================================================
//  SUB-Routine
//    Log data to SD card.
//===============================================================================================
void LogToSD()
{
  // Open the SD file
  File dataFile = SD.open("datalog.txt", FILE_WRITE);   
  if (dataFile){
   
    //PRINT LAT AND LONG to SD and Serial
    Serial.print(F("LAT & LONG: "));
    dataFile.print(F("LAT & LONG: "));
    if (gps.location.isValid())
    {
      Serial.print(gps.location.lat(), 6);
      Serial.print(F(" "));
      Serial.print(gps.location.lng(), 6);
      dataFile.print(gps.location.lat(), 6);
      dataFile.print(F(" "));
      dataFile.print(gps.location.lng(), 6);
    }
    else  {
      Serial.print(F("NA NA"));
      dataFile.print(F("NA NA"));
    }
      
    //Print DATE to SD and Serial
    Serial.print(F("  M/D/Y: "));
    dataFile.print(F("  M/D/Y: "));
    if (gps.date.isValid())
    {
      if (gps.date.day() < 10) {  //DAY  
        Serial.print(F("0"));
        dataFile.print(F("0"));
      }
      Serial.print(gps.date.day());
      dataFile.print(gps.date.day());

      if (gps.date.month() < 10) {  //MONTH
        Serial.print(F("0"));
        dataFile.print(F("0"));
      }
      Serial.print(gps.date.month());
      dataFile.print(gps.date.month());
      
      Serial.print(gps.date.year());  //YEAR
      dataFile.print(gps.date.year());
    }
    else  {    
      Serial.print(F("NA NA NA"));
      dataFile.print(F("NA NA NA"));
    }  
      
      
    // Print TIME to SD and Serial
    Serial.print(F("  Hour/Min/Sec/Mil: "));
    dataFile.print(F("  Hour/Min/Sec/Mil: "));
    if (gps.time.isValid())
    {
      if (gps.time.hour() < 10) {  // HOUR
        Serial.print(F("0"));
        dataFile.print(F("0"));
      }
      Serial.print(gps.time.hour());
      dataFile.print(gps.time.hour());
      
      if (gps.time.minute() < 10) {  // MINUTES
        Serial.print(F("0"));
        dataFile.print(F("0"));
      }
      Serial.print(gps.time.minute());
      dataFile.print(gps.time.minute());
      
      if (gps.time.second() < 10) {  // SECONDS
        Serial.print(F("0"));
        dataFile.print(F("0"));
      }
      Serial.print(gps.time.second());
      dataFile.print(gps.time.second());
      
      if (gps.time.centisecond() < 10) {  // MILISECONDS
        Serial.print(F("0"));
        dataFile.print(F("0"));
      }
      Serial.print(gps.time.centisecond());
      dataFile.print(gps.time.centisecond());
    }
    else  
    {    
      Serial.print(F("NA NA NA NA"));
      dataFile.print(F("NA NA NA NA"));
    }
  
  
  
    dataFile.println(); //create a new row to read data more clearly
    dataFile.close();   //close file
    Serial.println();   //print to the serial port too:
    delay(1000);
  }
      
  else  {
    Serial.println("error opening datalog.txt");
  }
}

