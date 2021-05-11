#include <Arduino.h>
#include<Wire.h>
#include <WiFi.h>
#include "myMPU6050.h"
#include "mywifi.h"
#include "include.h"

#ifndef CONFIG_ARDUINO_LOOP_STACK_SIZE
#define CONFIG_ARDUINO_LOOP_STACK_SIZE 8192
#endif

TaskHandle_t wifiHandle = NULL; //looptask任务的句柄
TaskHandle_t mpu6050Handle = NULL; //looptask任务的句柄

bool fflag = false;


void wifi(void *pvParameters)   //looptask任务体，包括运行一次setup和loop的无限循环
{
    wifi_setup();
    for(;;) {
        wifi_loop();
    }
}

void mpu6050(void *pvParameters)   //looptask任务体，包括运行一次setup和loop的无限循环
{  
    mpu6050_setup();
    for(;;) {
        mpu6050_loop();
    }
}


void setup() {
  xTaskCreate(wifi, "wifi", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &wifiHandle);
  xTaskCreate(mpu6050, "mpu6050", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &mpu6050Handle);
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelete(NULL);//此处delete自身
}