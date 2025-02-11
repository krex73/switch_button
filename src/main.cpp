#include "config.hpp"
#include "udp_util.h"
#include <ESPAsyncWebServer.h>
#include <FTPServer.h>
#include <LittleFS.h>
#include "SimpleTimer.h"
#include "ArduinoOTA.h"
#include "ssdp.h"
#include "log_util.h"
#include "wifi_setup.hpp"
#include "interface.hpp"
#include <ArduinoJson.h>
#include "sensor.h"
#include "mqtt.h"
#include "relay.h"
#include <GyverNTP.h>

FTPServer ftpSrv(LittleFS);
SimpleTimer Timer;
bool needUpdate = true;

extern WiFiUDP udp;

AsyncWebServer server(80);

int i = 0;

// конструктор страницы
void setup()
{
  Serial.begin(115200);
  LittleFS.begin();
  loadConfig();

  delay(4200);
#ifdef ISRELEY
  initRelayMode();
#else
  initSensorMode();
#endif
  WifiInit();
  NTP.setHost("192.168.7.247");
  NTP.begin(3);
  initUDP();
  SSDP_init();
  httpInit();
  server.begin();

  ftpSrv.begin(config.ftp_name, config.ftp_password); // username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
  mqttInit();
  Timer.setInterval(60000, checkWiFiAndReset);
  
  //Timer.setInterval(60000, );


  ArduinoOTA.begin();
}

void loop()
{
  NTP.tick();
  handleUDP();
#ifndef ISRELEY
  doSensorMode();
#endif
  Timer.run();
  ftpSrv.handleFTP();
  ArduinoOTA.handle();
}
