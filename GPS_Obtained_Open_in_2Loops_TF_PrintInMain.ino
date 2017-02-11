//===============================================================================================
//  NOTES
//===============================================================================================
/*
NOTE: this sketch is a continuation of the 'GPS_My_Format_SD'
GPS data (lat long date and time) is logged to the SD card
The data only logs on SD when the LAT AND LONG is Valid
The GPS uses Serial1
Designed for Arduino Mega
*/

//===============================================================================================
//  LIbrary AND Declirations
//===============================================================================================
#include <TinyGPS++.h>
#include <SPI.h>
#include <SD.h>
String FileName = String ("NULL");
int LED_Power = 22;
int LED_GPS = 24;
int LED_Log = 26;
int LED_DTC = 30;

const int chipSelect = 9;  // Chip Select pin is tied to pin 9 on the SparkFun CAN-Bus Shield
TinyGPSPlus gps;// Create a TinyGPS++ object called "gps"
//===============================================================================================
//  SETUP
//===============================================================================================
void setup()
{
  Serial.begin(9600);//Serial Monitor
  Serial1.begin(9600);//GPS
  
  InitiateLEDs(); 
  LED_Status(0);
  
  Serial.println(F("GPS_Obtained_Open_in_Loop.ino"));
  Serial.println(F("Arduino Mega with GPS module Logging to SD - My own data format. "));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Oliver Davis"));
  Serial.println();
  
  Serial.print("Initializing SD card...");
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    LED_Status(4);
    return;
  }
  else { Serial.println(" Complete. ");}
}

//===============================================================================================
//  LOOP
//    Print GPS Values.
//===============================================================================================
void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial1.available() > 0 ) {
    if (gps.encode(Serial1.read())){
      if (gps.location.isValid() && gps.date.isValid()){
        FileName = GetFileName();
        LED_Status(2);
        delay(1000);
      }
      else  {
        Serial.print(gps.time.minute());
        Serial.print(gps.time.second());
        Serial.print(" File Name = ");
        Serial.print(FileName); 
        Serial.print(" Waiting for Location ... ");
        Serial.print(gps.location.lat(),6);
        Serial.println(gps.location.lng(),6);
        LED_Status(1);
        delay(4000);
      }
      break;
    }
  }

  while(FileName!= "NULL"){
    while (Serial1.available() > 0 ){
      if (gps.encode(Serial1.read())){
        File dataFile = SD.open(FileName.c_str(), FILE_WRITE);   
        Serial.print("File Name = ");
        Serial.println(FileName);
        Serial.println();        
        dataFile.print(Get_GPS_Location());
        dataFile.print(Get_GPS_DateANDTime());
        LED_Status(3);
        dataFile.println();
        dataFile.close(); 
        delay(4000);
      }
    } 
  }
}       

    
  
//===============================================================================================
//  SUB-Routine
//    Log Date AND Time to SD card.
//===============================================================================================
/*String Get_DateANDTime(void)
{
    
  //PRINT LAT AND LONG to SD and Serial
    Serial.print(F("LAT AND LONG: "));
    dataFile.print(F("LAT AND LONG: "));
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
}
*/
//===============================================================================
// LED Status
//===============================================================================
int InitiateLEDs(void)
{
  int Flash = 0;
  pinMode(LED_DTC, OUTPUT);
  pinMode(LED_GPS, OUTPUT);
  pinMode(LED_Power, OUTPUT);
  pinMode(LED_Log, OUTPUT);
  
  while (Flash<=2){
    Serial.println(Flash);
    digitalWrite(LED_Power, HIGH);
    digitalWrite(LED_DTC, HIGH);
    digitalWrite(LED_GPS, HIGH);
    digitalWrite(LED_Log, HIGH);
    delay(500);
    digitalWrite(LED_Power, LOW);
    digitalWrite(LED_DTC, LOW);
    digitalWrite(LED_GPS, LOW);
    digitalWrite(LED_Log, LOW);
    delay(500);
    Flash = Flash+1; 
  }
}
//-----------------------------------------------------------------------------
int LED_Status(int Status)
{
  switch(Status){
  case 0:{                        //Power On
    digitalWrite(LED_Power, HIGH);
    break;
    }
  case 1:{                        //Searching for GPS
    digitalWrite(LED_GPS, HIGH);
    delay(100);
    digitalWrite(LED_GPS, LOW);
    break;
    }
  case 2:{                        //GPS Obtained
    digitalWrite(LED_GPS, HIGH);
    break;
    }
  case 3:{                        //Data Logging
    digitalWrite(LED_Log, HIGH);
    delay(100);
    digitalWrite(LED_Log, LOW);
    break;
    }
  case 4:{                        //DTC ERROR
    digitalWrite(LED_DTC, HIGH);
    break;
    }
  }
}
//===============================================================================
// Sub GetTimeDate
//===============================================================================
String GetFileName(void)
{
  String Day = String(gps.date.day());
  String Month = String(gps.date.month());
  String Hour = String(gps.time.hour());
  String Minute = String(gps.time.minute());
  String FileEx = String(".txt");
  Serial.println(Day + Month + Hour + Minute + FileEx);
  return Day + Month + Hour + Minute + FileEx;
}
//-------------------------------------------------------------------------------
String Get_GPS_Location(void)
{
  String Lat = String(gps.location.lat(),6);
  String Space = String(" ");
  String Long = String(gps.location.lng(),6);
  String Location = Lat + Space + Long + Space;
  Serial.print(gps.location.lat(),6);
  Serial.print(" ");
  Serial.println(gps.location.lng(),6);
  return Location;
}
//-------------------------------------------------------------------------------
String Get_GPS_DateANDTime(void)
{
  String Day = String(gps.date.day());
  String Month = String(gps.date.month());
  String Year = String(gps.date.year());
  String Hour = String(gps.time.hour());
  String Minute = String(gps.time.minute());
  String Second = String(gps.time.second());
  String Milli = String(gps.time.centisecond());
  String Slash = String("/");
  String Space = String(" ");
  String Colon = String(":");
  String Period = String(".");
  String DateANDTime = Day + Slash + Month + Slash + Year + Space + Hour + Colon + Minute + Colon + Second + Period + Milli + Space;
  
  Serial.print(gps.date.day());
  Serial.print("/");
  Serial.print(gps.date.month());
  Serial.print("/");
  Serial.print(gps.date.year());
  Serial.print(" ");
  Serial.print(gps.time.hour());
  Serial.print(":");
  Serial.print(gps.time.minute());
  Serial.print(":");
  Serial.print(gps.time.second());
  Serial.print(".");
  Serial.println(gps.time.centisecond());

  return DateANDTime;
}
