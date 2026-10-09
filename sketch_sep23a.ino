#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "FS.h"

const int chipSelect = 5;

File dataFile;

unsigned long lastSample = 0;
const unsigned long interval = 10000; 

float ax, ay, az, gx, gy, gz;

void updateFakeIMU() {
  float t = micros() / 1000000.0;

  ax = 1.0  * sin(2 * PI * 0.5 * t);
  ay = 1.0  * sin(2 * PI * 0.7 * t);
  az = 1.0  * sin(2 * PI * 0.3 * t);

  gx = 50.0 * sin(2 * PI * 1.0 * t);
  gy = 50.0 * sin(2 * PI * 1.3 * t);
  gz = 50.0 * sin(2 * PI * 0.8 * t);
}

const int BUF_SIZE = 4096;
char buf[BUF_SIZE];
int bufPos = 0;
void setup() {

  Serial.begin(115200);

  if (!SD.begin(chipSelect)) {
    Serial.println("Card mount failed");
    return;
  }
  Serial.println("Card ready");

  dataFile = SD.open("/datalog.csv", FILE_WRITE);
  if (!dataFile) {
    Serial.println("File open failed");
    return;
  }

  lastSample = micros();

  dataFile.println("t_us,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps");
  }

void loop() {
  if (micros() - lastSample >= interval) {
    lastSample += interval;

    updateFakeIMU();

    bufPos += snprintf(buf + bufPos, BUF_SIZE - bufPos,
                       "%lu,%.4f,%.4f,%.4f,%.3f,%.3f,%.3f\n",
                       micros(), ax, ay, az, gx, gy, gz);

    if (bufPos > BUF_SIZE - 100) {
      dataFile.write((uint8_t*)buf, bufPos);
      dataFile.flush();
      bufPos = 0;
    }
  }
  delay(1);
}