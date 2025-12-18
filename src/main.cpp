#include <SPI.h>
#include <RF24.h>

// CE, CSN pins
RF24 radio(7, 8);

// 5-byte address (must match RX side)
const byte address[6] = "DRONE";

uint16_t throttle = 0;

void setup() {
  Serial.begin(9600);

  radio.begin();
  // RF settings
  radio.setChannel(100); // same channel as RX
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);

  radio.setAutoAck(true);
  radio.setCRCLength(RF24_CRC_16);

  radio.openWritingPipe(address);
  radio.stopListening(); // TX mode

  Serial.println("Transmitter ready");
}

void loop() {
  throttle = analogRead(A0); // read throttle value

  bool ok = radio.write(&throttle, sizeof(throttle));

  Serial.print("Throttle: ");
  Serial.println(throttle);
  Serial.print("ACK: ");
  Serial.println(ok ? "YES" : "NO");

  delay(500); // slight delay to let RX process
}
