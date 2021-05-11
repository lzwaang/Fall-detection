#ifndef myMPU6050_H
#define myMPU6050_H

#include<Arduino.h>
#include<Wire.h>
#define SDA 18
#define SCL 19
#define  G  9.801 //北京地区的重力加速度
 //跌倒了亮灯。



extern const int MPU_addr; // I2C address of the MPU-6050
extern int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
extern float acc_x, acc_y, acc_z;
extern float rot_x, rot_y, rot_z;
extern float netForce;
extern float netRot;


void MPU6050_init();
void data_get();
void data_print();
void mpu6050_setup();
void mpu6050_loop();

#endif