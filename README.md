# TeensyMAC
Teensy Serial# and MAC

Works with Teensy LC & Teensy 3.0 .. 3.6

Example:

```
#include <TeensyMAC.h>

void setup() { 
  delay(1000); 
  Serial.printf("Serial: %u\n",teensySerial());
  Serial.printf("MAC: 0x%012llX\n", teensyMAC());
}

void loop() {}
```
