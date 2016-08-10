#include <SPI.h>
#include <SD.h>
const int chipSelect = 9;  // Chip Select pin is tied to pin 9 on the SparkFun CAN-Bus Shield

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop()
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);   // if the file is available, write to it:

  if (dataFile)   {  
    int timeStamp = millis();
    //write to uSD card
    dataFile.print(timeStamp);
    dataFile.print(" ");
    dataFile.print("09-08-2016");
    dataFile.print(" ");
    dataFile.print("13:47:00");
    dataFile.print(" ");
    dataFile.print("0.000000");
    dataFile.print(" ");
    dataFile.print("0.000000");
    dataFile.print(" ");
    dataFile.print("3420");
    dataFile.print(" ");
    dataFile.print("68");
    dataFile.print(" ");
    dataFile.print("20");
    dataFile.print(" ");
    dataFile.print("50");
    dataFile.print(" ");
    dataFile.print("12.5");
    dataFile.print(" ");
    dataFile.print("2");
    
    //output also on Serial monitor for debugging
    Serial.print(timeStamp);
    Serial.print(", ");
    Serial.print("Date");
    Serial.print(", ");
    Serial.print("Time");
    Serial.print(", ");
    Serial.print("Lat");
    Serial.print(", ");
    Serial.print("Lon");
    Serial.print(", ");
    Serial.print("RPM");
    Serial.print(", ");
    Serial.print("Temp");
    Serial.print(", ");
    Serial.print("Throtle");
    Serial.print(", ");
    Serial.print("Speed");
    Serial.print(", ");
    Serial.print("Fuel");
    Serial.print(", ");
    Serial.print("DTC");

    dataFile.println(); //create a new row to read data more clearly
    dataFile.close();   //close file
    Serial.println();   //print to the serial port too:
    delay(1000);
  }  
  // if the file isn't open, pop up an error:
  else  {
    Serial.println("error opening datalog.txt");
  }
  
}
