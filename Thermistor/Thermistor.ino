// ----------------------------------------------------------------------------
// Thermistor.ino is for use with the Hacking STEM NASA Heat Shield lesson plan
// available from Microsoft Education Workshop at http://aka.ms/hackingSTEM 
// 
// This project uses an Arduino UNO microcontroller board. More information can
// be found by visiting the Arduino website: 
// https://www.arduino.cc/en/main/arduinoBoardUno 
//  
// This project reads the resistance of 2 thermistors and converts that value 
// to temprature using the stienhart-hart formula. Each of the thermistors is 
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

// Serial data variables ------------------------------------------------------
const byte kNumberOfChannelsFromExcel = 6; //Number of channels coming in from Excel
// IMPORTANT: This must be equal to number of channels set in Data Streamer

const char kDelimiter = ',';    // Data Streamer expects a comma delimeter
const int kSerialInterval = 1000;   // Interval between serial writes
unsigned long serialPreviousTime; // Timestamp to track serial interval

char* arr[kNumberOfChannelsFromExcel]; //Array of data coming in from Excel

void setup(void) {
  Serial.begin(9600);
  //analogReference(EXTERNAL);
}
 
void loop(void) {
  processIncomingSerial();
  if (strcmp("#paused", arr[0]) != 0){
    sendDataToSerial();
  } 
}

float getResistance (int pin){
  //TODO: Test out sampling for data quality
//   const int kNumSamples = 5;
//   int samples[kNumSamples];

//   // take N samples in a row, with a slight delay
//   for (int i=0; i< kNumSamples; i++) {
//    samples[i] = analogRead(pin);
//    delay(10);
//   }
 
//   // average all the samples out
//   float average = 0;
//   for (int i=0; i< kNumSamples; i++) {
//      average += samples[i];
//   }
//   average /= kNumSamples;
// //  float reading = average;

    const int kSeriesResistor = 10000;
    //Read an analog pin and convert the value to a resistance value in ohms
    float reading = analogRead(pin);
    float voltage = 1023 / reading - 1;
    float resistance = kSeriesResistor / voltage;
    return resistance;
   
}

float steinhart (float resistanceReading){
    // Resistance at 25 C
    const int kThermistorNominal = 10000;
    // Tempatrature for nominal resistance
    const int kTempratureNominal = 25;
    // Beta coefficient of the thermistor
    const int kBetaCoefficient = 3950;

    float temprature;
    temprature = resistanceReading / kThermistorNominal;     // (R/Ro)
    temprature = log(temprature);                  // ln(R/Ro)
    temprature /= kBetaCoefficient;                   // 1/B * ln(R/Ro)
    temprature += 1.0 / (kTempratureNominal + 273.15); // + (1/To)
    temprature = 1.0 / temprature;                 // Invert
    temprature -= 273.15;                         // convert to C
    return temprature;
}

// OUTGOING SERIAL DATA PROCESSING CODE----------------------------------------
void sendDataToSerial()
{
  float materialResistanceReading = getResistance(kMaterialThermPin);
  float materialTempReading = steinhart(materialResistanceReading);
  float hairDryerResistanceReading = getResistance(kHairDryerThermPin);
  float hairDryerTempReading = steinhart(hairDryerResistanceReading);

  // Enter into this only when serial interval has elapsed
  if((millis() - serialPreviousTime) > kSerialInterval) 
  {
    serialPreviousTime = millis(); // Reset serial interval timestamp

    Serial.print(materialTempReading);
    Serial.print(kDelimiter);
    Serial.print(hairDryerTempReading);
    
    Serial.println(); // Add final line ending character only once
  }
}

//-----------------------------------------------------------------------------
// DO NOT EDIT ANYTHING BELOW THIS LINE
//-----------------------------------------------------------------------------

// INCOMING SERIAL DATA PROCESSING CODE----------------------------------------
void processIncomingSerial()
{
  if(Serial.available()){
    parseData(GetSerialData());
  }
}

// Gathers bytes from serial port to build inputString
char* GetSerialData()
{
  static char inputString[64]; // Create a char array to store incoming data
  memset(inputString, 0, sizeof(inputString)); // Clear the memory from a pervious reading
  while (Serial.available()){
    Serial.readBytesUntil('\n', inputString, 64); //Read every byte in Serial buffer until line end or 64 bytes
  }
  return inputString;
}

// Seperate the data at each delimeter
void parseData(char data[])
{
    char *token = strtok(data, ","); // Find the first delimeter and return the token before it
    int index = 0; // Index to track storage in the array
    while (token != NULL){ // Char* strings terminate w/ a Null character. We'll keep running the command until we hit it
      arr[index] = token; // Assign the token to an array
      token = strtok(NULL, ","); // Conintue to the next delimeter
      index++; // incremenet index to store next value
    }
}