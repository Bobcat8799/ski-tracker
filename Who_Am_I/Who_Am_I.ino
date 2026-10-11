#include <Wire.h>

const int SDA_PIN = 21;   // change to the pins you actually wired
const int SCL_PIN = 22;

uint8_t readReg(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return 0xFF;
  Wire.requestFrom(addr, (uint8_t)1);
  return Wire.available() ? Wire.read() : 0xFF;
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  Serial.println("Scanning I2C bus...");
  int found = 0;
  for (uint8_t a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) {
      Serial.printf("Device at 0x%02X\n", a);
      found++;
    }
  }
  if (!found) Serial.println("Nothing found: wiring, pins or power.");

  for (uint8_t a : {0x6A, 0x6B}) {
    Serial.printf("WHO_AM_I at 0x%02X = 0x%02X (expect 0x70)\n", a, readReg(a, 0x0F));
  }
}

void loop() {}
