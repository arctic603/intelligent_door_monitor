# intelligent_door_monitor
## 基于STM32F4系列的MQTT智能门禁锁

### 通过STM32F407ZET6的裸机开发(本想遵从于FreeROTS设计，但是甚至觉得有点麻烦)
- 由于我的开发板是只有三个串口，导致串口不够用，故蓝牙/WIFI解锁/MQTT_WIFI,提供了三个版本

```C

1，指纹解锁(采用串口的电容指纹组模块(SFM-V1.7)，可以完成指纹注册与注销指纹操作。
2，RFID门禁解锁，采用RC522实现门禁卡解锁，可做无效卡与有效卡处理及支持NFC解锁。
3，数字密码键盘解锁（支持虚位密码解锁），手势识别解锁(PAJ7620U2)模块实现。
4，串口蓝牙(BT-05)解锁及串口WIFI模块ESP8266解锁的实现。
5，阿里云的MQTT协议解锁，通过实现PC机推送主题，然后MQTT.fx软件监视推送和订阅主题情况，单片机的ESP8266进行订阅主题读取报文信息进行解锁。
6，支持查看解锁记录（可通过连接的串口设备如蓝牙终端发送申请查看解锁记录）。
7，OLED显示温湿度信息，解锁反馈信息等。
```

如有任何疑问可以联系我
rayonsun@outlook.com
