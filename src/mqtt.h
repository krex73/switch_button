#pragma once
#include "AsyncMqttClient.h"
#include "config.hpp"
#include "relay.h"
#include <Ticker.h>
#include "wifi_setup.hpp"
#include "str_util.hpp"
//#include <GyverNTP.h>

AsyncMqttClient *mqttClient;

Ticker mqttReconnectTimer;
extern bool onLine;
extern String myIp;
//extern Sring myIp;
//extern Sring myIp;

void sendILive()
{
  mqttClient->publish((config.mqttPrefix+"/"+config.topicName+"/IP" ).c_str(),0,true, myIp.c_str());
  mqttClient->publish((config.mqttPrefix+"/"+config.topicName+"/lastTime").c_str(),0,true, (const char*)(NTP.toString().c_str()));
  char ct[20] ;
  sprintf(ct,"%ld", NTP.toSeconds());
  mqttClient->publish((config.mqttPrefix+"/"+config.topicName+"/lastSec").c_str(),0,true, ct);
}


void connectToMqtt()
{
  DBG("Connecting to MQTT...\n");
  if (mqttClient)
    mqttClient->connect();
}

void connectWiFiMqtt(WiFiEventStationModeGotIP ipInfo)
{
  connectToMqtt();
}

void onMqttConnect(bool sessionPresent)
{
  DBG("Connected to MQTT.  ");
  DBG("Session present: ");
  DBG(sessionPresent);
  DBG("\n");
  
  if(mqttClient->connected()){
        uint32_t connectSec = NTP.toSeconds();
        Serial.println("tttttttt");
        Serial.println(connectSec);
        sendILive();
  }

#ifdef ISRELEY
  mqttClient->subscribe((config.mqttPrefix + "/" + config.topicName).c_str(), 0);
#endif
}



void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  DBG("Publish received. Topic: ");
  DBG(topic);
  DBG("\n");
  DBG("  payload: ");
  char pay[100];
  strncpy(pay, payload, len);
  pay[len] = 0;
  DBG(pay);
  DBG("\n");

  doRelayMode(pay);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  DBG("Disconnected from MQTT.\n");

  if (WiFi.isConnected())
  {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void mqttInit()
{
  static WiFiEventHandler wifiGetIpMQTT;

  wifiGetIpMQTT = WiFi.onStationModeGotIP(connectWiFiMqtt);

  mqttClient = new AsyncMqttClient();
  mqttClient->setServer(config.mqttServer.c_str(), config.mqttPort);
  String clId = WiFi.macAddress();
  clId.replace(":", "");
  clId.concat("_ID \n");
  DBG(clId);
  mqttClient->setClientId(clId.c_str());
  mqttClient->setKeepAlive(15);
  mqttClient->onConnect(onMqttConnect);
  mqttClient->onDisconnect(onMqttDisconnect);
  mqttClient->onMessage(onMqttMessage);
  mqttClient->connect();
}
