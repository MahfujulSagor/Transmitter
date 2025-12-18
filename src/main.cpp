#include <SPI.h>
#include <RF24.h>

// CE, CSN pins
RF24 radio(7, 8);

// 5-byte address (must match RX side)
const byte address[6] = "DRONE";

struct ControlPacket {
  uint16_t throttle;
  uint16_t yaw;
  uint16_t pitch;
  uint16_t roll;
};

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
  ControlPacket packet;
  // A0 is Throttle, A1 is Yaw, A2 is roll, A3 is Pitch
  packet.throttle = analogRead(A0); // 0-1023
  packet.yaw      = analogRead(A1);
  packet.roll     = analogRead(A2);
  packet.pitch    = analogRead(A3);

  bool ok = radio.write(&packet, sizeof(packet));

  Serial.print("Throttle: ");
  Serial.print(packet.throttle);
  Serial.print("| Yaw: ");
  Serial.print(packet.yaw);
  Serial.print("| Pitch: ");
  Serial.print(packet.pitch);
  Serial.print("| Roll: ");
  Serial.println(packet.roll);

  Serial.print("ACK: ");
  Serial.println(ok ? "Yes" : "No");

  delay(20); // slight delay to let RX process
}
