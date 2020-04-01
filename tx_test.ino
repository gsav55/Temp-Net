/////////////////////////////////////////
//       Transmitter Test V0.1         //
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
  // TRANSMITTER SETUP
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  //radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void loop() {
  //TRANSMITTER LOOP
  const char text[] = "Hello World!";
  radio.write(&text, sizeof(text));
  delay(1000);
}
