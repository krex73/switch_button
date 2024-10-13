#include "config.hpp"
#include <GyverPortal.h>
#include "udp_util.h"
#include <FTPServer.h>
#include <LittleFS.h>
#include "SimpleTimer.h"
#include "ArduinoOTA.h"
#include "ssdp.h"
#include "log_util.h"
#include "EspMQTTClient.h"
#include <Bounce2.h>
#include "wifi_setup.hpp"
#include "interface.hpp"
#include <ArduinoJson.h>

FTPServer ftpSrv(LittleFS);
SimpleTimer Timer;
bool needUpdate=true;

extern WiFiUDP Udp;

Bounce *bounce;

GyverPortal ui(&LittleFS); // для проверки файлов
EspMQTTClient *client;

int i = 0;

void onConnectionEstablished()
{
  Serial.println("connected");
  client->subscribe(config.mqttPrefix + "/power_state", [](const String &payload)
                    { Serial.println("Command On"); });
  // client->publish(config.mqttPrefix + "/temperature", "15");
}

// конструктор страницы
void setup()
{
  Serial.begin(115200);

  LittleFS.begin();
  loadConfig();

  WifiInit();

  Udp.begin(config.changePort);
  initUDP();

  ftpSrv.begin(config.ftp_name, config.ftp_password); // username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
  if (config.enableMQTT)
  {
    Serial.print((config.mqttPrefix + "userKrex").c_str());
    client = new EspMQTTClient(config.mqttServer.c_str(), config.mqttPort, config.mqttUser.c_str(), config.mqttPassword.c_str(), "userKrex238");
    // config.mqttUser.c_str()
    client->enableDebuggingMessages(true);
  };

  Timer.setInterval(300000, reset_device);
  // Только для блока управления
  if (!config.thisSensorMode)
  {
    requestSSDP();
    Timer.setInterval(60000, requestSSDP);
  }
  else
  {
    bounce = new Bounce();
    pinMode(config.sensorPin, INPUT_PULLUP);
    bounce->attach(config.sensorPin);
  };

  // подключаем конструктор и запускаем
  ui.attachBuild(build);
  ui.attach(action);
  ui.start(((config.thisSensorMode) ? config.sensorSSDPName : config.controlSSDPName).c_str());
  ArduinoOTA.begin();
  SSDP_init();
}

void loop()
{
  handleUDP();
  if (config.thisSensorMode)
  {
    bounce->update();
    if (bounce->changed())
      if (bounce->read() == HIGH)
      {
        sendUDP("open");
      }
      else
      {
        sendUDP("closed");
      }
  }
else
{
  handleSSDP();
};
Timer.run();
ftpSrv.handleFTP();
ui.tick();
ArduinoOTA.handle();
handleSSDP();
if (config.enableMQTT)
  client->loop();
}
