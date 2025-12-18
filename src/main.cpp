#include <SPI.h>
#include <RF24.h>

// CE, CSN pins
RF24 radio(7, 8);

// 5-byte address (must match RX side)
const byte address[6] = "DRONE";

// Value to send (0-255)
// const char message[6] = "SAGOR";
struct Data {
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
};

void setup()
{
  Serial.begin(9600);

  radio.begin();
  // RF settings
  radio.setChannel(100); // same channel as RX
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);

  radio.setAutoAck(true);
  radio.setRetries(5, 15); // delay, count
  radio.setCRCLength(RF24_CRC_16);

  radio.openWritingPipe(address);
  radio.stopListening(); // TX mode

  Serial.println("Text transmitter ready");
}

void loop()
{
  Data dataToSend;
  dataToSend.throttle = 150; // example throttle value
  dataToSend.yaw = 128;      // example yaw value
  dataToSend.pitch = 100;    // example pitch value
  dataToSend.roll = 200;     // example roll value
  bool ok = radio.write(&dataToSend, sizeof(dataToSend));
  if (ok)
  {
    Serial.println("TX: Sent successfully");
  }
  else
  {
    Serial.println("TX: Send failed");
  }
  delay(500); // slight delay to let RX process
}
