// which analog pin to connect
//#define THERMISTORPIN A0
// resistance at 25 degrees C
//#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
//#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
//#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    
 
uint16_t samples[NUMSAMPLES];

const int kFrontThermPin = A0;
const int kBackThermPin = A1;


void setup(void) {
  Serial.begin(9600);
  //analogReference(EXTERNAL);
}
 
void loop(void) {
  uint8_t i;
  float average;
 
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(kFrontThermPin);
   delay(10);
  }
 
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  // Serial.print("Average analog reading "); 
  // Serial.println(average);
 
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  // Serial.print("Thermistor resistance "); 
  // Serial.println(average);
 
//   float steinhart;
//   steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
//   steinhart = log(steinhart);                  // ln(R/Ro)
//   steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
//   steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
//   steinhart = 1.0 / steinhart;                 // Invert
//   steinhart -= 273.15;                         // convert to C

    float backTempReading = steinhart(average);
    // float front = analogRead(kBackThermPin);
    // front = 1023 / front - 1;
    // front = SERIESRESISTOR / front;
    float front = getResistance(kBackThermPin);
    float forntTempReading = steinhart(front);

//   Serial.print("Temperature "); 
  Serial.print(backTempReading);
  Serial.print(',');
  Serial.print(forntTempReading);
  Serial.println();
//   Serial.println(" *C");
 
  delay(1000);
}

float getResistance (int pin){
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