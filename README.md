# ski-tracker

IB MYP Personal Project. A boot-mounted device that logs
6-axis motion data during skiing, for analysis of turn
dynamics and edge angle.

## Hardware
- ESP32-WROOM-32
- LSM6DSV16X IMU (I2C, address 0x6B)
- MicroSD module (SPI)

## Wiring
| IMU | ESP32 |
|-----|-------|
| VCC | 3V3   |
| GND | GND   |
| SDA | GPIO21|
| SCL | GPIO22|

## Progress
- [x] Fixed-rate sampling loop
- [x] SD card logging with RAM buffer
- [x] IMU communicating, WHO_AM_I = 0x70
- [ ] Reliable continuous readings
- [ ] Python analysis
- [ ] Sensor fusion
