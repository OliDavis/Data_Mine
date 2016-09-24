//===============================================================================================
//  NOTES
//===============================================================================================
/*
NOTE: this sketch is a continuation of the sparkfun standard 'GPS_Refined_Sparkfun'
The comments and unnecessary code have been removed
The GPS now uses Serial1
The GPS Baud has been increased to 9600
Designed for Arduino Mega

The GPS data obtained
  - date 
  - time
  - Long
  - Lat
  
 This example uses software serial and the TinyGPS++ library by Mikal Hart
 Based on TinyGPSPlus/DeviceExample.ino by Mikal Hart
 Modified by acavis
*/

//===============================================================================================
//  LIbrary & Declirations
//===============================================================================================
#include <TinyGPS++.h>
TinyGPSPlus gps;// Create a TinyGPS++ object called "gps"

//===============================================================================================
//  SETUP
//===============================================================================================
void setup()
{
  Serial.begin(9600);//Serial Monitor
  Serial1.begin(9600);//GPS
    
  Serial.println(F("GPS_Refined_Sparkfun_Mega.ino"));
  Serial.println(F("Arduino Mega with GPS module - LAT LONG DATE TIME "));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Oliver Davis"));
  Serial.println();
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
      displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial1 port, show a error message
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected- Check Wiring")); //error message
    while(true);
  }
}

//===============================================================================================
//  SUB-Routine
//    Check if GPS is valid.
//===============================================================================================
void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

