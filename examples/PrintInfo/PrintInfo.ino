#include <TeensyMAC.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(500);
  Serial.printf("Serial: %u\n", teensySerial());
  Serial.printf("MAC: 0x%012llX\n", teensyMAC());

  Serial.print("MAC as array: ");
  uint8_t mac[6];
  teensyMAC(mac);
  for (uint8_t i = 0; i < 6; ++i) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) {
      Serial.print(":");
    }
  }
  Serial.println("");
}
