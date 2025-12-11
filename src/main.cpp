#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[6] = "00001";

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
  byte throttle = map(analogRead(A0), 0, 1023, 0, 255); // map 0-1023 to 0-255
  byte pitch = map(analogRead(A1), 0, 1023, 0, 255); // map 0-1023 to 0-255
  byte roll = map(analogRead(A2), 0, 1023, 0, 255); // map 0-1023 to 0-255
  radio.write(&throttle, sizeof(throttle));
  radio.write(&pitch, sizeof(pitch));
  radio.write(&roll, sizeof(roll));

  Serial.print("Throttle: ");
  Serial.print(throttle);
  Serial.print("\t|\tPitch: ");
  Serial.print(pitch);
  Serial.print("\t|\tRoll: ");
  Serial.println(roll);
  delay(10);
}
