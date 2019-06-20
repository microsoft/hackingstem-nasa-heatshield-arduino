// ----------------------------------------------------------------------------
// Thermistor.ino is for use with the Hacking STEM NASA Heat Shield lesson plan
// available from Microsoft Education Workshop at http://aka.ms/hackingSTEM 
// 
// This project uses an Arduino UNO microcontroller board. More information can
// be found by visiting the Arduino website: 
// https://www.arduino.cc/en/main/arduinoBoardUno 
//  
// This project reads the resistance of two thermistors and converts that value 
// to temperature using the stienhart-hart formula. Each of the thermistors is
// setup with a voltage divider, so that we can use the voltage divider 
// equation to accurately measure the resistance.
//
// Pins:
// A0 Material Thermistor
// A1 Hair Dryer Thermistor
// 
// Comments, contributions, suggestions, bug reports, and feature requests 
// are welcome! For source code and bug reports see: 
// http://github.com/[TODO: github path to Hacking STEM] 
// 
// Derived from https://github.com/adafruit/Adafruit_Learning_System_Guides/blob/master/Themistor/Example3/thermistor3.ino
// Copyright (c) 2018, Limor Fried, Adafruit Industries
//
// Copyright (c) 2019, Adi Azulay Microsoft EDU Workshop - HackingSTEM, 
// 
// MIT License terms detailed in LICENSE.txt 
// ----------------------------------------------------------------------------

const int kMaterialThermPin = A0;
const int kHairDryerThermPin = A1;

const char kDelimiter = ',';    // Data Streamer expects a comma delimeter
const int kSerialInterval = 1000;   // Interval between serial writes
unsigned long serialPreviousTime; // Timestamp to track serial interval

void setup(void)
{
  Serial.begin(9600);
}
 
void loop(void)
{
  sendDataToSerial();
}

float getResistance(int pin)
{
    const int kSeriesResistor = 10000;
    //Read an analog pin and convert the value to a resistance value in ohms
    float reading = analogRead(pin);
    float voltage = 1023 / reading - 1;
    float resistance = kSeriesResistor / voltage;
    return resistance;
   
}

float getTemperature(float resistanceReading)
{
    // Resistance at 25 C
    const int kThermistorNominal = 10000;
    // Tempatrature for nominal resistance
    const int kTemperatureNominal = 25;
    // Beta coefficient of the thermistor
    const int kBetaCoefficient = 3950;

    // The stienhart-hart formula below is fairly complex for more info see:
    // https://en.wikipedia.org/wiki/Thermistor
    float value;
    value = resistanceReading / kThermistorNominal;
    value = log(value);
    value /= kBetaCoefficient;
    value += 1.0 / (kTemperatureNominal + 273.15);
    value = 1.0 / value;
    value -= 273.15;
    float temperature = value;
    return temperature;
}

// OUTGOING SERIAL DATA PROCESSING CODE----------------------------------------
void sendDataToSerial()
{
  // Enter into this only when serial interval has elapsed
  if((millis() - serialPreviousTime) > kSerialInterval) 
  {
    serialPreviousTime = millis(); // Reset serial interval timestamp
    float materialTempReading = getTemperature(getResistance(kMaterialThermPin));
    float hairDryerTempReading = getTemperature(getResistance(kHairDryerThermPin));
    Serial.print(hairDryerTempReading);
    Serial.print(kDelimiter);
    Serial.print(materialTempReading);

    
    Serial.println(); // Add final line ending character only once
  }
}