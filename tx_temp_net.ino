#include <SPI.h>
#include "RF24.h"
#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

// AM2302
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor


/****************** User Config ***************************/
//AM2302
DHT dht(DHTPIN, DHTTYPE);
//
//Which node is this?  Master = 0, Kitchen = 1, Office = 2
int NODE = 0;
//
//Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"0Node","1Node","2Node"};

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24 transmitting AM2302 temp and humidity"));
  Wire.begin();
  dht.begin();
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  //Only one writing pipe can be open at once.  To change the address call stopListening() first.
  radio.openReadingPipe(1,addresses[NODE]);
  
  // Start the radio listening for data
  radio.startListening();
}

void loop() {

  float temp_hum_val[2] = {0};
  byte send_it = 0;

    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;  
  
  while ( ! radio.available() ){                             // While nothing is received
     if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
         timeout = true;
         break;
     }      
   }
       
   if ( timeout ){                                             // Describe the results
       Serial.println(F("Waiting to send"));
   }else{    
    radio.read( send_it, sizeof(send_it) );           // Get the payload
    radio.stopListening();                                    // First, stop listening so we can talk.
   }

  if(send_it){
    Serial.println(F("Now sending"));

    if(!dht.readTempAndHumidity(temp_hum_val)){
        Serial.print(F("Humidity: ")); 
        Serial.print(temp_hum_val[0]);
        Serial.print(F(" %\t"));
        Serial.print(F("Temperature: ")); 
        temp_hum_val[1] = dht.convertCtoF(temp_hum_val[1]);
        Serial.print(temp_hum_val[1]);
        Serial.println(F(" *F"));
    }else{
       Serial.println(F("Failed to get temprature and humidity value."));
    }
 
   if (!radio.write( temp_hum_val, sizeof(temp_hum_val) )){
     Serial.println(F("failed"));
   }
  }
} // Loop
