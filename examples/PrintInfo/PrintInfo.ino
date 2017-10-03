#include <TeensyMAC.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(500);
  Serial.printf("Serial: %u\n", teensySerial());
  Serial.printf("MAC: 0x%012llX\n", teensyMAC());
}
