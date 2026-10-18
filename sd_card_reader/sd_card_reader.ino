#include <SPI.h>
#include <SD.h>

const int SD_CS = 5;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("1. Sketch started");

  SPI.begin(18, 19, 23, SD_CS);
  Serial.println("2. SPI started, calling SD.begin...");

  bool ok = SD.begin(SD_CS, SPI, 400000);
  Serial.println("3. SD.begin returned");

  if (!ok) {
    Serial.println("Mount failed");
    return;
  }

  Serial.printf("Card type: %d\n", SD.cardType());
  Serial.printf("Card size: %llu MB\n", SD.cardSize() / (1024 * 1024));
}

void loop() {}