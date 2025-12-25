#include <RF24.h>
#include <SPI.h>

// CE, CSN pins
RF24 radio(7, 8);

// 5-byte address (must match RX side)
const byte address[6] = {'D', 'R', 'O', 'N', 'E'};

struct __attribute__((packed)) ControlPacket {
  uint16_t throttle = 0;
  uint16_t yaw = 0;
  uint16_t pitch = 0;
  uint16_t roll = 0;
};
ControlPacket rxData;

void setup() {
  Serial.begin(9600);

  radio.begin();
  // RF settings
  radio.setChannel(100); // same channel as RX
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setAddressWidth(5);

  radio.setAutoAck(true);
  radio.setRetries(5, 15);
  radio.setCRCLength(RF24_CRC_16);

  radio.openWritingPipe(address);
  radio.stopListening(); // TX mode

  Serial.println("Transmitter ready");
}

void loop() {
  // A0 is Throttle, A1 is Yaw, A2 is roll, A3 is Pitch
  rxData.throttle = analogRead(A0); // 0-1023
  rxData.yaw = analogRead(A1);
  rxData.pitch = analogRead(A2);
  rxData.roll = analogRead(A3);

  bool ok = radio.write(&rxData, sizeof(rxData));

  // Serial.print("T: ");
  // Serial.print(rxData.throttle);
  // Serial.print(" | Y: ");
  // Serial.print(rxData.yaw);
  // Serial.print(" | P: ");
  // Serial.print(rxData.pitch);
  // Serial.print(" | R: ");
  // Serial.print(rxData.roll);

  // Print whether the packet was acknowledged
  Serial.print(" | ACK: ");
  Serial.println(ok ? "Yes" : "No");

  delay(20);
}
