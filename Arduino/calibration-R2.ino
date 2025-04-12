/*
   MCP41050 Dual 50K 

   Ce programme réalise une recherche de maximum pour déterminer la valeur optimale de R2.
   Ensuite il récupère la valeur de tension en sortie de l'amplificateur et calcule la résistance du capteur de graphite.
*/

#include <SPI.h>

// initialize pins and constants
const byte csPin           = 10;      // MCP42100 chip select pin
const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 positions
const long rAB             = 50000;   // valeur à remplacer par la valeur réelle
const byte rWiper          = 125;     // 125 ohms pot wiper resistance
const byte pot0            = 0x11;    // pot0 addr // B 0001 0001
const byte pot0Shutdown    = 0x21;    // pot0 shutdown // B 0010 0001 
const int adcPin           = A0 ;     // analog pin ADC is connected to 
const byte verifPin        = 7 ;      // digital pin used to check wether the research algorithm must be ran or not (choose a pin thatis not used by another device)

const float r1 = 100000 ;
const float r3 = 100000 ;
const float r4 = 10000 ;
const float Vcc = 5 ;
float r2 ;


void setPotWiper(int addr, int pos) {
  pos = constrain(pos, 0, 255);            // restrict possible positions from 0 to 255
  digitalWrite(csPin, LOW);                // enable chip select
  SPI.transfer(addr);                      
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // disable chip select
}

void setup() {
  Serial.begin(9600);
  Serial.println("MCP41100 Demo");
  
  digitalWrite(csPin, HIGH);        // chip select default to de-selected
  pinMode(csPin, OUTPUT);           // configure chip select as output
  SPI.begin();

  digitalWrite(verifPin, HIGH) ;    // initialize the pin to high
  pinMode(verifPin, OUTPUT) ;
  
}

void loop() {
    
    Serial.println("Recherche de maximum ");

    //initialize our variables
    float adc_max   = 0.0 ;
    int optimal_pos = 0 ;
  
   for (int i = 1; i < 256; i++) {
      setPotWiper(pot0, i);
      float adc = analogRead(adcPin) ;
      adc = adc*Vcc/1024;
      if ((adc > adc_max) && (adc < 5.0)) {
        optimal_pos = i ;
        adc_max = adc ;
      }
      // print pot resistance between wiper and B terminal
      long resistanceWB = ((rAB * i) / maxPositions ) + rWiper;
      Serial.print("Wiper position: ");
      Serial.println(i);
      Serial.print("Resistance wiper to B terminal: ");
      Serial.print(resistanceWB);
      Serial.println(" ohms");
      Serial.print ("Output voltage :") ;
      Serial.print (adc) ;
      Serial.println (" volts") ;
      delay(500);
    }

    Serial.print("The optimal position is :") ;
    Serial.println(optimal_pos) ;
    digitalWrite(verifPin, LOW) ;
    
    // move pot0 wiper to optimal position
    setPotWiper(pot0,optimal_pos);

    // compute r2 (resistance value corresponding to the optimal position)
    r2 =((rAB * optimal_pos) / maxPositions ) + rWiper ;
}
