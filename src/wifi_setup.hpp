#pragma once
#include "udp_util.h"
#include "config.hpp"

extern SimpleTimer Timer;

bool state_setup = false;
bool wait_wifi = true;

void break_cycle()
{
  wait_wifi = false;
};

void WifiInit()
{ 
  Serial.println("Try SSID");
  Serial.println(config.ssid);
  Serial.print("PASS ");
  Serial.println(config.pass);

  WiFi.mode(WIFI_STA);
  WiFi.begin(config.ssid, config.pass);
  Serial.println("Connected");
  Timer.setTimer(20000, break_cycle, TIMER_WIFI_CONNECT);
  while ((WiFi.status() != WL_CONNECTED) & wait_wifi)
  {
    delay(300);
    Serial.print(".");
    Timer.run();
  }
  Timer.disable(TIMER_WIFI_CONNECT);

  Serial.println("Connected");

  if (!wait_wifi)
  {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(config.ssid_td, config.pass_td);
    //Serial.print("Включаем точку доступа");
    state_setup = true;
  }
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.status());
}
void reset_device()
{
    if (WiFi.status() != WL_CONNECTED)
    {
      add_log((char*)"reset");
      sendUDP((char*)"reset");
      ESP.reset();
    }
}

