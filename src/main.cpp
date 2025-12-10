#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(9, 10);   // CE, CSN
const byte address[6] = "00001";

// Structure for 4-channel data
struct ControlPacket {
  byte throttle;
  byte pitch;
  byte roll;
  byte yaw;
};
ControlPacket packet;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  Serial.println("Transmitter ready...");
}

void loop() {

  // ----- Read analog stick values -----
  packet.throttle = map(analogRead(A0), 0, 1023, 0, 255);
  packet.pitch    = map(analogRead(A1), 0, 1023, 0, 255);
  packet.roll     = map(analogRead(A2), 0, 1023, 0, 255);
  packet.yaw      = map(analogRead(A3), 0, 1023, 0, 255);

  // ----- Send the full packet -----
  radio.write(&packet, sizeof(packet));

  // Debug monitor
  Serial.print("T: "); Serial.print(packet.throttle);
  Serial.print(" | P: "); Serial.print(packet.pitch);
  Serial.print(" | R: "); Serial.print(packet.roll);
  Serial.print(" | Y: "); Serial.println(packet.yaw);

  delay(10);
}
