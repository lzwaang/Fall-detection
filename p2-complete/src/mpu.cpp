#include<Arduino.h>
#include<Wire.h>
#include "myMPU6050.h"
#include "include.h"

const int MPU_addr=0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float acc_x, acc_y, acc_z;
float rot_x, rot_y, rot_z;
float netForce = 0.0;
float netRot = 0.0;


void MPU6050_init(){
	
	Wire.beginTransmission(MPU_addr);
	Wire.write(0x6B); // PWR_MGMT_1 register
	Wire.write(0); // set to zero (wakes up the MPU-6050)
	Wire.endTransmission(true);

	//f：表示倍率时，0代表2g，1代表4g，2代表8g，3代表16g；表示角速度上限时，0代表250°，1代表500°，2代表1000°，3代表2000°
	uint8_t f = 0;
	Wire.beginTransmission(MPU_addr); //开启MPU-6050的传输
	Wire.write(0x1C); //加速度倍率寄存器的地址
	Wire.requestFrom(0x68, 1, true); //先读出原配置
	unsigned char acc_conf = Wire.read();
	acc_conf = ((acc_conf & 0xE7) | (f << 3));
	Wire.write(acc_conf);
	Wire.endTransmission(true); //结束传输，true表示释放总线

	f = 1;
	Wire.beginTransmission(MPU_addr); //开启MPU-6050的传输
	Wire.write(0x1B); //角速度倍率寄存器的地址
	Wire.requestFrom(0x68, 1, true); //先读出原配置
	acc_conf = Wire.read();
	acc_conf = ((acc_conf & 0xE7) | (f << 3));
	Wire.write(acc_conf);
	Wire.endTransmission(true); //结束传输，true表示释放总线

}

void data_get(){
	Wire.beginTransmission(MPU_addr);
	Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
	Wire.endTransmission(false);
	Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
	AcX = Wire.read()<<8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
	AcY = Wire.read()<<8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
	AcZ = Wire.read()<<8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
	Tmp = Wire.read()<<8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53); //equation for temperature in degrees C from datasheet
	GyX = Wire.read()<<8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
	GyY = Wire.read()<<8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
	GyZ = Wire.read()<<8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

	acc_x = 2 * AcX / 32768.0; //加速度，即所受重力的分量值
	acc_y = 2 * AcY / 32768.0;
	acc_z = 2 * AcZ / 32768.0;
	netForce = (acc_x + acc_y + acc_z) / 3; //三轴加速度平均值

	rot_x = 500 * GyX / 32768.0;
	rot_y = 500 * GyY / 32768.0;
	rot_z = 500 * GyZ / 32768.0;
	netRot = sqrt((rot_x * rot_x) + (rot_y * rot_y) + (rot_z * rot_z)); //三轴平均角加速度
}

void data_print(){
	/*Serial.print("AcX = "); Serial.print(acc_x);
	Serial.print(" | AcY = "); Serial.print(acc_y);
	Serial.print(" | AcZ = "); Serial.print(acc_z);
	Serial.print(" | GyX = "); Serial.print(rot_x);
	Serial.print(" | GyY = "); Serial.print(rot_y);
	Serial.print(" | GyZ = "); Serial.print(rot_z);*/
	Serial.print("netForce = "); Serial.print(netForce);
	Serial.print(" | netRot = "); Serial.print(netRot);
	Serial.print("\n");
}

void mpu6050_setup()
{
	pinMode(2, OUTPUT); //设置GOIP2引脚为输出模式
	digitalWrite(2, LOW);
	Wire.begin(SDA, SCL);
	MPU6050_init();
	Serial.begin(9600);
}


void mpu6050_loop()
{
	data_get();
	data_print();
	uint8_t state = 1;
	if(state==1)
  {
    //if(rotX>200 || rotY>200 || rotZ>200 || rotX<-200 || rotY<-200 || rotZ<-200)
    if(netRot>250)
    {
      Serial.println("FALL SENSED");
      state=2;
      delay(100); 
    }
  }
if(state==2)
  {
    //if((gForceZ>0.50 && (gForceY>0.50 || gForceY<-0.50)) || (gForceZ>0.50 && (gForceX>0.50 || gForceX<-0.50)))  //or (XZ>0.5 YZ>0.5 XY>0.5) or (XY<-.5 ....) or (X>0.5Y<-0.5 ....) 
    if(netForce>0.55 || netForce<0.3)
    { 
		digitalWrite(2, HIGH); //设置GOIP2引脚为低电平	                                                                                                          //or (X<-0.5Y>0.5 ....)
        Serial.println("LEGITIMATE FALL");
        fflag = true;
        delay(10000);
	    digitalWrite(2, LOW);
        state=1;
    }
  } 
	delay(1000);
}


