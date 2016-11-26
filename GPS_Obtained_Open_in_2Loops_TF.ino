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
const int chipSelect = 9;  // Chip Select pin is tied to pin 9 on the SparkFun CAN-Bus Shield
TinyGPSPlus gps;// Create a TinyGPS++ object called "gps"
//===============================================================================================
//  SETUP
//===============================================================================================
void setup()
{
  Serial.begin(9600);//Serial Monitor
  Serial1.begin(9600);//GPS
  
  Serial.println(F("GPS_Obtained_Open_in_Loop.ino"));
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
        delay(5000);
      }
      break;
    }
  }

  while(FileName!= "NULL"){
    while (Serial1.available() > 0 ){
      if (gps.encode(Serial1.read())){
        
        Serial.print("File Name = ");
        Serial.println(FileName);
        File dataFile = SD.open(FileName.c_str(), FILE_WRITE);   
        dataFile.print(Get_GPS_Location());
        dataFile.print(Get_GPS_DateANDTime());
        dataFile.println();                                      //create a new row to read data more clearly
        dataFile.close();
        Serial.println();                                        //print to the serial port too:
        delay(5000);
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
  String Space = String (" ");
  String Lat = String(gps.location.lat(),6);
  String Lng = String(gps.location.lng(),6);
  String Location = Lat + Space + Lng + Space;
  Serial.println(Location);
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
  String Space = String (" ");
  String Colon = String (":");
  String Period = String (".");
  String Slash = String ("/");  
  String DateANDTime = Day + Slash + Month + Slash + Year + Space + Hour + Colon + Minute + Colon + Second + Period + Milli;
  Serial.println(DateANDTime);
  return DateANDTime; 
}
