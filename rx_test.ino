/////////////////////////////////////////
//         Receiver Test V0.1          //
//             3/31/20                 //
//         Grayson Savage              //
//        Tutorial found at:           //
// www.youtube.com/watch?v=7rcVeFFHcFM //
/////////////////////////////////////////

#include <SPI.h>
#include <RF24.h>

RF24 radio(7 ,8);     // (CNS, CE) pins

const byte address[6] = "00001"; // Radio channel.  Can be any 5 digit string.

void setup() {
  // RECEIVER SETUP
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  //radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}

void loop() {
  //RECEIVER LOOP
  if (radio.available()){
    char text[32] = "";
    radio.read(&text, sizeof(text));
   // Serial.println("receiving");
    Serial.println(text);
  }
}
