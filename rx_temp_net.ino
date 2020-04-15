/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

//Which node is this?  Master = 0, Kitchen = 1, Office = 2
int NODE = 0;
/****************** User Config ***************************/

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"0Node","1Node","2Node"};

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
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

  // We are going to loop through the number of radios in our network and poll each one
  // one at a time to get their sensor data.  We will then write that to a CSV (probably write a linux
  // script for that part)

  for  (int i=1; i <= sizeof(addresses); i++){
    radio.stopListening();
    radio.openWritingPipe(addresses[i]);
    Serial.println(F("Pinging radio "));
    Serial.print(i);
    
    byte send_it = 1;                             //tell the sensors to send that shit
    if (!radio.write(send_it, sizeof(byte))){
      Serial.println(F("Didn't send it :("));             //couldn't connect to sensor
    }

    radio.startListening();
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
    }else{    
      float get_weather[2] = {0};
      radio.read( get_weather, sizeof(get_weather) );           // Get the payload
      
      Serial.print(get_weather[1]);
      Serial.print(F(" F "));
      Serial.print(get_weather[0]);
      Serial.println(F(" % Humidity"));  
    }
 }
} // Loop
