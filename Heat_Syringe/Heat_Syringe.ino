/*
 Heat_Syringe.ino is for use with the Hacking STEM NASA Heat Shield lesson plan
 available from Microsoft Education Workshop at https://aka.ms/heatshield
 http://aka.ms/hackingSTEM 

 This project uses an Arduino UNO microcontroller board. More information can
 be found by visiting the Arduino website: 
 https://www.arduino.cc/en/main/arduinoBoardUno 

 Comments, contributions, suggestions, bug reports, and feature requests 
 are welcome! For source code and bug reports see: 
 https://github.com/microsoft/hackingstem-nasa-heatshield-arduino

 MPL3115A2 Barometric Pressure Sensor Library Example Code
 By: Nathan Seidle
 SparkFun Electronics
 Date: September 24th, 2013
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 Uses the MPL3115A2 library to display the current altitude and temperature
 
 Hardware Connections (Breakoutboard to Arduino):
 -VCC = 3.3V
 -SDA = A4 (use inline 10k resistor if your board is 5V)
 -SCL = A5 (use inline 10k resistor if your board is 5V)
 -INT pins can be left unconnected for this demo
 
 During testing, GPS with 9 satellites reported 5393ft, sensor reported 5360ft (delta of 33ft). Very close!
 During testing, GPS with 8 satellites reported 1031ft, sensor reported 1021ft (delta of 10ft).
 
 Available functions:
 .begin() Gets sensor on the I2C bus.
 .readAltitude() Returns float with meters above sealevel. Ex: 1638.94
 .readAltitudeFt() Returns float with feet above sealevel. Ex: 5376.68
 .readPressure() Returns float with barometric pressure in Pa. Ex: 83351.25
 .readTemp() Returns float with current temperature in Celsius. Ex: 23.37
 .readTempF() Returns float with current temperature in Fahrenheit. Ex: 73.96
 .setModeBarometer() Puts the sensor into Pascal measurement mode.
 .setModeAltimeter() Puts the sensor into altimetery mode.
 .setModeStandy() Puts the sensor into Standby mode. Required when changing CTRL1 register.
 .setModeActive() Start taking measurements!
 .setOversampleRate(byte) Sets the # of samples from 1 to 128. See datasheet.
 .enableEventFlags() Sets the fundamental event flags. Required during setup.
 
*/

#include <Wire.h>
#include "SparkFunMPL3115A2.h"

//Create an instance of the object
MPL3115A2 myPressure;
float tempC = 0;


void setup()
{
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output

  myPressure.begin(); // Get sensor online

  // Configure the sensor
  //myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
}

void loop()
{
//
//  int sensorInput = analogRead(A0); // read the analog sensor
//  tempC = (double)sensorInput / 1024; // divide by 1024 to get percentage of
//                                      // input reading in millivolts
//  tempC = tempC * 3.3;                  // multiply by 3.3V to get voltage
//  tempC = tempC - 0.5; // Subtract the offset specified in TMP36 datasheet
//  tempC = tempC * 100; // Convert to degrees
//    
  /*float altitude = myPressure.readAltitude();
  Serial.print("Altitude(m):");
  Serial.print(altitude, 2);

  altitude = myPressure.readAltitudeFt();
  Serial.print(" Altitude(ft):");
  Serial.print(altitude, 2);*/

  float pressure = myPressure.readPressure();
  float temperature = myPressure.readTemp();
  float temperatureF = myPressure.readTempF();

  Serial.print(",");
  Serial.print(pressure, 2);
  Serial.print(",");
  Serial.print(temperature, 2);
  Serial.print(",");
  Serial.print(temperatureF, 2);


  Serial.println();
}
