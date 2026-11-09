#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "FS.h"
#include "SparkFun_LSM6DSV16X.h"
#include <Wire.h>
#include <math.h>

SparkFun_LSM6DSV16X myLSM;

// Structs for X,Y,Z data
sfe_lsm_data_t accelData;
sfe_lsm_data_t gyroData;

// Sd card reader selector
const int chipSelect = 5;

File dataFile;

unsigned long lastSample = 0;
const unsigned long interval = 8333.33333333;

  float gx_a = 0;
  float gy_a = 0;
  float gz_a = 0;

  float AngleRoll = 0;
  float AnglePitch = 0;

  int LastUpdate = 0;

  float xcal = 0;
  float ycal = 0;
  float zcal = 0;

  float sum = 0;
void setup() {
  

  lastSample = micros();

  // IMU setup below from public sketch below
  Wire.begin();
  Wire.setClock(100000);

  delay(100);

  Serial.begin(115200);
  while (!Serial);

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

  Serial.println("Setup Succesfull");
  Serial.println("Starting Calibration Please don't move device");

  unsigned long calStart = micros();
  lastSample = calStart;

  while(micros() - calStart <3000000){
    if (micros() - lastSample >= interval) {
      lastSample += interval;
      if (myLSM.checkStatus()) {
        myLSM.getAccel(&accelData);
        myLSM.getGyro(&gyroData);
        xcal += gyroData.xData;
        ycal += gyroData.yData;
        zcal += gyroData.zData;
        sum += 1;
      }
    }
  }
  xcal/=sum;
  ycal/=sum;
  zcal/=sum;
  Serial.println("Calibration Succesful");
}

void loop() {


  if (micros() - lastSample >= interval) {
    lastSample += interval;
    if (myLSM.checkStatus()) {
      myLSM.getAccel(&accelData);
      myLSM.getGyro(&gyroData);
      gx_a += ((gyroData.xData - xcal)/1000)*0.00833333333;
      gy_a += ((gyroData.yData - ycal)/1000)*0.00833333333;
      gz_a += ((gyroData.zData - zcal)/1000)*0.00833333333;

      AngleRoll = (atan2(accelData.yData, accelData.zData))*180/M_PI;
      AnglePitch = (atan2(accelData.xData, sqrt(accelData.yData*accelData.yData+accelData.zData*accelData.zData)))*180/M_PI;
      LastUpdate += 1;
    }
  if (LastUpdate == 15) {
    Serial.print(gx_a);
    Serial.print(",");
    Serial.print(gy_a);
    Serial.print(",");
    Serial.println(gz_a);
    LastUpdate = 0;
  }
  delay(1);
  }
}
  