/* *****************************************************************

   Download latest Blinker library here:
   https://github.com/blinker-iot/blinker-library/archive/master.zip


   Blinker is a cross-hardware, cross-platform solution for the IoT.
   It provides APP, device and server support,
   and uses public cloud services for data transmission and storage.
   It can be used in smart home, data monitoring and other fields
   to help users build Internet of Things projects better and faster.

   Make sure installed 2.7.4 or later ESP8266/Arduino package,
   if use ESP8266 with Blinker.
   https://github.com/esp8266/Arduino/releases

   Make sure installed 1.0.5 or later ESP32/Arduino package,
   if use ESP32 with Blinker.
   https://github.com/espressif/arduino-esp32/releases

   Docs: https://diandeng.tech/doc


 * *****************************************************************

   Blinker 库下载地址:
   https://github.com/blinker-iot/blinker-library/archive/master.zip

   Blinker 是一套跨硬件、跨平台的物联网解决方案，提供APP端、设备端、
   服务器端支持，使用公有云服务进行数据传输存储。可用于智能家居、
   数据监测等领域，可以帮助用户更好更快地搭建物联网项目。

   如果使用 ESP8266 接入 Blinker,
   请确保安装了 2.7.4 或更新的 ESP8266/Arduino 支持包。
   https://github.com/esp8266/Arduino/releases

   如果使用 ESP32 接入 Blinker,
   请确保安装了 1.0.5 或更新的 ESP32/Arduino 支持包。
   https://github.com/espressif/arduino-esp32/releases

   文档: https://diandeng.tech/doc


 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_ALIGENIE_MULTI_OUTLET

#include <Blinker.h>


// 定义引脚
const int Open = 5;
const int Close = 4;
const int Unlock = 14;

// 新建组件对象
BlinkerButton Button1("btn-open");
BlinkerButton Button2("btn-colse");

char auth[] = "5139739470d7";
char ssid[] = "TP-LINK_04DD";
char pswd[] = "1234567890";

bool oState[5] = { false };

// 按下按键即会执行该函数
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // 引脚14输出高电平
  digitalWrite(Unlock, HIGH);
  delay(100);
  digitalWrite(Unlock, LOW);
  delay(100);
  digitalWrite(Open, HIGH);
  delay(100); // 延时1秒
  digitalWrite(Open, LOW);
}

// 按下按键即会执行该函数
void button2_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    digitalWrite(Unlock, HIGH);
  delay(100);
  digitalWrite(Unlock, LOW);
  delay(100);
  digitalWrite(Close, HIGH);
  delay(100); // 延时1秒
  digitalWrite(Close, LOW);
}

void aligeniePowerState(const String & state, uint8_t num)
{
  if (num == 0)
  {
    // 引脚14输出高电平
  digitalWrite(Unlock, HIGH);
  delay(100);
  digitalWrite(Unlock, LOW);
  delay(100);
  digitalWrite(Open, HIGH);
  delay(100); // 延时1秒
  digitalWrite(Open, LOW);
  }
  if (num == 1)
  {
    // 引脚14输出高电平
  digitalWrite(Unlock, HIGH);
  delay(100);
  digitalWrite(Unlock, LOW);
  delay(100);
  digitalWrite(Close, HIGH);
  delay(100); // 延时1秒
  digitalWrite(Close, LOW);
  }
  BLINKER_LOG("need set outlet: ", num, ", power state: ", state);

  if (state == BLINKER_CMD_ON) {
    digitalWrite(LED_BUILTIN, HIGH);

    BlinkerAliGenie.powerState("on", num);
    BlinkerAliGenie.print();

    oState[num] = true;
  }
  else if (state == BLINKER_CMD_OFF) {
    digitalWrite(LED_BUILTIN, LOW);

    BlinkerAliGenie.powerState("off", num);
    BlinkerAliGenie.print();

    oState[num] = true;

    if (num == 0)
    {
      for (uint8_t o_num = 0; o_num < 5; o_num++)
      {
        oState[o_num] = false;
      }
    }
  }
}

void aligenieQuery(int32_t queryCode, uint8_t num)
{
  BLINKER_LOG("AliGenie Query outlet: ", num, ", codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_ALL_NUMBER :
      BLINKER_LOG("AliGenie Query All");
      BlinkerAliGenie.powerState(oState[num] ? "on" : "off", num);
      BlinkerAliGenie.print();
      break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
      BLINKER_LOG("AliGenie Query Power State");
      BlinkerAliGenie.powerState(oState[num] ? "on" : "off", num);
      BlinkerAliGenie.print();
      break;
    default :
      BlinkerAliGenie.powerState(oState[num] ? "on" : "off", num);
      BlinkerAliGenie.print();
      break;
  }
}

void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}

void setup()
{
  Serial.begin(115200);

  // 设置引脚为输出模式
  pinMode(Open, OUTPUT);
  pinMode(Close, OUTPUT);
  pinMode(Unlock, OUTPUT);

  BLINKER_DEBUG.stream(Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Blinker.begin(auth, ssid, pswd);
  Blinker.attachData(dataRead);

  BlinkerAliGenie.attachPowerState(aligeniePowerState);
  BlinkerAliGenie.attachQuery(aligenieQuery);
  Button1.attach(button1_callback);
  Button2.attach(button2_callback);
}

void loop()
{
  Blinker.run();
}
