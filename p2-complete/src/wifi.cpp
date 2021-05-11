#include <Arduino.h>
#include <WiFi.h>
#include "mywifi.h"
#include "include.h"

//填写你的WIFI帐号密码
const char* ssid = "Redmi K30 Pro";
const char* password = "3586613wang";

const char* host = "192.168.168.226";//主机（即服务器）动态ip
const int port = 9002;//demo1 tcp 使用 9002端口

const char* id = "1234abcd";
int tick = 1000;
char a[10];

WiFiClient client;



void wifi_setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  //连接WIFI
  WiFi.begin(ssid, password);
  //设置读取socket数据时等待时间为100ms（默认值为1000ms，会显得硬件响应很慢）
  client.setTimeout(100);

  //等待WIFI连接成功
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connecting...");
    delay(500);
  }
  Serial.println("WiFi connected!.");
}

void wifi_loop() {
  // put your main code here, to run repeatedly:
  if (client.connect(host, port))
  {
    Serial.println("host connected!");
    //发送第一条TCP数据，发送ID号
    client.print(id);
  }
  else
  {
    // TCP连接异常
    Serial.println("host connecting...");
    delay(500);
  }

  while (client.connected()) {
    //      接收到TCP数据
    /*if (client.available())
    {
      Serial.println(client.available());
      //String line = client.readStringUntil('\n');
      int count = client.readBytes(a, 1);
      Serial.println(count);
      char line = a[0];
      if (line == '1') {
        Serial.println("command:open led.");
        digitalWrite(LED_BUILTIN, LOW);
        client.print("OK");
      }
      else if (line == '0') {
        Serial.println("command:close led.");
        digitalWrite(LED_BUILTIN, HIGH);
        client.print("OK");
      }
    }
    else {
      //若没收到TCP数据，每隔一段时间打印并发送tick值
      Serial.print("tem:");
      Serial.println(tick);
      client.print(tick);
      tick++;
      delay(1000);
    }*/

    if(fflag){
        client.print("fall down! ");
        Serial.print("跌倒了！ ");
        Serial.print("tem:");
        Serial.println(tick);
    }
    else{
        client.print("not fall down. ");
        Serial.print("没跌倒。");
        Serial.print("tem:");
        Serial.println(tick);
    }
    tick++;
    delay(1000);
  }

}