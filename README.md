# OAK-MPU6050_Blynk

MPU6050 connected to OAK to get Acceleration, Gyroscope and Temperature. The data is sent to Blynk virtual pins 1 to 7. Virtual pin 0 is used for terminal messages

App project setup:
 *   1 x Terminal widget attached to Virtual Pin V0
 *   7 x Display widget attached to each of 7 Virtual pins

Connections:
 
 * OAK ------ MPU6050
 * SDA ------ SDA
 * SCL ------ SCL
 * 3.3v ----- VCC
 * GND ------ GND
