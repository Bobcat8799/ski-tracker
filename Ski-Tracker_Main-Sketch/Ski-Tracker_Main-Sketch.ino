#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "FS.h"
#include "SparkFun_LSM6DSV16X.h"
#include <Wire.h>

SparkFun_LSM6DSV16X myLSM;

// Structs for X,Y,Z data
sfe_lsm_data_t accelData;
sfe_lsm_data_t gyroData;

// Sd card reader selector
const int chipSelect = 5;

File dataFile;

unsigned long lastSample = 0;
const unsigned long interval = 10000;

const int BUF_SIZE = 4096;
char buf[BUF_SIZE];
int bufPos = 0;

void setup() {
  // Sd card setup below
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

  dataFile.println("t_us,ax_mg,ay_mg,az_mg,gx_mdps,gy_mdps,gz_mdps");

  // IMU setup below from public sketch below
  Wire.begin();
  Wire.setClock(100000);

  delay(100);

  Serial.begin(115200);
  while (!Serial)

  Serial.println("LSM6DSV16X Example 1 - Basic Readings I2C");

  if (!myLSM.begin(0x6A)) {
    Serial.println("Did not begin, check your wiring and/or I2C address!");
    while (1)
      ;
  }

  // Reset the device to default settings. This if helpful is you're doing multiple
  // uploads testing different settings.
  myLSM.deviceReset();

  // Wait for it to finish reseting
  while (!myLSM.getDeviceReset()) {
    delay(1);
  }

  Serial.println("Board has been Reset.");
  Serial.println("Applying settings.");

  // Accelerometer and Gyroscope registers will not be updated
  // until read.
  myLSM.enableBlockDataUpdate();

  // Set the output data rate and precision of the accelerometer
  myLSM.setAccelDataRate(LSM6DSV16X_ODR_AT_120Hz);
  myLSM.setAccelFullScale(LSM6DSV16X_16g);

  // Set the output data rate and precision of the gyroscope
  myLSM.setGyroDataRate(LSM6DSV16X_ODR_AT_120Hz);
  myLSM.setGyroFullScale(LSM6DSV16X_2000dps);

  // Enable filter settling.
  myLSM.enableFilterSettling();

  // Turn on the accelerometer's filter and apply settings.
  myLSM.enableAccelLP2Filter();
  myLSM.setAccelLP2Bandwidth(LSM6DSV16X_XL_STRONG);

  // Turn on the gyroscope's filter and apply settings.
  myLSM.enableGyroLP1Filter();
  myLSM.setGyroLP1Bandwidth(LSM6DSV16X_GY_ULTRA_LIGHT);

  Serial.println("Ready.");
}

void loop() {
  if (micros() - lastSample >= interval) {
    lastSample += interval;
    if (myLSM.checkStatus()) {
      myLSM.getAccel(&accelData);
      myLSM.getGyro(&gyroData);
    }
    bufPos += snprintf(buf + bufPos, BUF_SIZE - bufPos,
                       "%lu,%.4f,%.4f,%.4f,%.3f,%.3f,%.3f\n",
                       micros(), accelData.xData, accelData.yData, accelData.zData,
                       gyroData.xData, gyroData.yData, gyroData.zData);

    if (bufPos > BUF_SIZE - 100) {
      dataFile.write((uint8_t*)buf, bufPos);
      dataFile.flush();
      bufPos = 0;
    }
  }
  delay(1);
}