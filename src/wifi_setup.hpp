#pragma once
#include "udp_util.h"
#include "config.hpp"
#include <ESP8266Ping.h>
#include "log_util.h"
#include <GyverNTP.h>
#include "mqtt.h"

extern SimpleTimer Timer;

bool onLine = false;
String myIp;

int TIMER_WIFI_CONNECT_BLINK = 0;

void onWifiGotIP(WiFiEventStationModeGotIP ipInfo)
{
  WiFi.waitForConnectResult();
  myIp=ipInfo.ip.toString();
  onLine=true;
  Serial.printf("Got IP: %s\r\n", ipInfo.ip.toString().c_str());
  Serial.println(NTP.toString());
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected &event)
{
  myIp="";
  onLine=false;
  Serial.printf("Disconnected from SSID: %s\n", event.ssid.c_str());
  Serial.printf("Reason: %d\n", event.reason);
}

void wifi_scan()
{
  Serial.println("WiFI mode STA");
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println("Scan begin");
  int n = WiFi.scanComplete();
  if (n == WIFI_SCAN_FAILED)
  {
    WiFi.scanNetworks(true);
    Serial.println("Scan filed");
  }
  else if (n)
  {

    for (int i = 0; i < n; ++i)
    {
      Serial.print(WiFi.SSID(i));
      Serial.println(WiFi.RSSI(i));
    }
    WiFi.scanDelete();
    if (WiFi.scanComplete() == WIFI_SCAN_FAILED)
    {
      WiFi.scanNetworks(true);
    }
  }
}

void WifiInit()
{
  // wifi_scan();
  Serial.println(config.WifiSTAPin);
  pinMode(config.WifiSTAPin, INPUT_PULLUP);

  if (digitalRead(config.WifiSTAPin) == LOW)
  {
    Serial.println("Try AP mode");
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(config.ssid_td, config.pass_td);
  }
  else
  {
    static WiFiEventHandler wifiGetIp, wifiDisconnectHandler;

    wifiGetIp = WiFi.onStationModeGotIP(onWifiGotIP);
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
    Serial.print("\nTry SSID: ");
    Serial.println(config.ssid.c_str());
    Serial.print("PASS: ");
    Serial.println(config.pass.c_str());

    WiFi.mode(WIFI_OFF);

    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid.c_str(), config.pass.c_str());
    while (!WiFi.isConnected())
    {
      delay(10);
    }
  }
}
void checkWiFiAndReset()
{
  if (WiFi.getMode() == WIFI_STA)
  {
    if (WiFi.isConnected())
    {
      DBG(NTP.toString());
      DBG("Ping host "+config.pingHost+"\n");
      if (!Ping.ping(config.pingHost.c_str(), 2)){
        WifiInit();
      }
      else{
        //Мы на связи
        sendILive();
      }  
    }
    else
      WifiInit();
  }
}