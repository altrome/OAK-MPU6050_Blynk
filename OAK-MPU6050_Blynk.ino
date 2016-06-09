/**************************************************************
 * MPU6050 connected to OAK to get Acceleration, Gyroscope and
 * Temperature. The data is sent to Blynk virtual pins 1 to 7
 * The Virtual pin 0 is used for terminal messages
 * App project setup:
 *   1 x Terminal widget attached to Virtual Pin V0
 *   7 x Display widget attached to each of 7 Virtual pins
 * Connections:
 * OAK ------ MPU6050
 * SDA ------ SDA
 * SCL ------ SCL
 * 3.3v ----- VCC
 * GND ------ GND
 **************************************************************/

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

char auth[] = "";
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

SimpleTimer timer;

#ifdef Oak_h
char szAcX[8] = "0";
char szAcY[8] = "0";
char szAcZ[8] = "0";

char szInfo[16];
#endif


// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V0);

void GetValues()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers

  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
  Blynk.virtualWrite(1, AcX/16384.);
  Blynk.virtualWrite(2, AcY/16384.);
  Blynk.virtualWrite(3, AcZ/16384.);
  Blynk.virtualWrite(4, GyX/131.);
  Blynk.virtualWrite(5, GyY/131.);
  Blynk.virtualWrite(6, GyZ/131.);
  Blynk.virtualWrite(7, Tmp/340.+36.53);

}

void setup()
{
  Blynk.config(auth);

  while (Blynk.connect() == false) {
    // Wait until connected
  }
  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println("-------------");
  terminal.println("Initializing I2C devices...");
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  terminal.println("-------------");
  terminal.flush();

  timer.setInterval(200L, GetValues);
}

void loop()
{
  Blynk.run();
  timer.run(); // SimpleTimer is working
}

