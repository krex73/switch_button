#pragma once

#define ISRELEY 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

// Пин шины OneWire для поиска и опроса датчиков
#define DBG_OUTPUT_PORT 1
// 1- Serial
// 2 - UDP
// 3 - UDP & Serial
#if DBG_OUTPUT_PORT == 1
#define DBG(...) Serial.print(__VA_ARGS__)
#else
#if DBG_OUTPUT_PORT == 2
#define DBG(...) logUDP(__VA_ARGS__)
#else
#if DBG_OUTPUT_PORT == 3
#define DBG(...)             \
  Serial.print(__VA_ARGS__); \
  logUDP(__VA_ARGS__);
#else
#define DBG(...)
#endif
#endif
#endif

struct
{
#ifdef ISRELEY
  String topicSensor;
#endif
  int controlPin = 0;
  int sensorPin = 0;         // D1
  int WifiSTAPin = 0;        // D2
  int sensorStateLEDPin = 0; // D

  boolean enableIntervalPolling = true;
  int sensorSendInterval = 60000;
  String pingHost;

  String mqttPrefix;
  String mqttServer;
  String mqttPassword;
  String mqttUser;
  String topicName;

  int mqttPort;

  String udpServer;
  int udpPort;

  String SSDPName = "";

  int interval = 10;
  int changePort = 7777;

  String ssid = "SV";
  String pass = "Sv123654789";

  String ssid_td = "sens_contr";
  String pass_td = "123654789";
  String ftp_name = "ftp";
  String ftp_password = "ftp";
  String sensorIP = "";
} config;

void loadConfig()
{
// Чтение файла конфигурации
#ifdef ISRELEY
  File configFile = LittleFS.open("/setup_rele.json", "r");
#else
  File configFile = LittleFS.open("/setup_sensor.json", "r");
#endif

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, configFile);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

#ifdef ISRELEY
  config.topicSensor = doc["topicSensor"].as<String>() ? doc["topicSensor"].as<String>() : "sensor_1";
#endif

                  config.mqttServer = doc["mqttServer"].as<String>();
  config.mqttUser = doc["mqttUser"].as<String>();
  config.mqttPassword = doc["mqttPassword"].as<String>();
  config.mqttPort = doc["mqttPort"];
  config.interval = doc["interval"];
  config.mqttPrefix = doc["mqttPrefix"].as<String>();
  config.topicName = doc["topicName"].as<String>();

  config.SSDPName = doc["SSDPName"].as<String>();
  config.pingHost = doc["pingHost"].as<String>();

  config.udpServer = doc["udpServer"].as<String>() ? doc["udpServer"].as<String>() : "192.168.7.247";
  config.udpPort = doc["udpPort"] ? doc["udpPort"] : 8888;

  config.interval = doc["interval"];

  config.controlPin = doc["controlPin"] ? doc["controlPin"] : 0;

  config.changePort = doc["changePort"];
  config.sensorPin = doc["sensorPin"];

  config.WifiSTAPin = doc["WifiSTAPin"] ? doc["WifiSTAPin"] : 2;
  config.WifiSTAPin = doc["WifiSTAPin"] ? doc["WifiSTAPin"] : 3;

  config.sensorStateLEDPin = doc["sensorStateLEDPin"];
  config.sensorSendInterval = doc["sensorSendInterval"];

  config.ssid = doc["ssid"].as<String>();
  config.pass = doc["pass"].as<String>();
  config.ssid_td = doc["ssid_td"].as<String>();
  config.pass_td = doc["pass_td"].as<String>();
  config.ftp_name = doc["ftp_name"].as<String>() ? doc["ftp_name"].as<String>() : "ftp";
  config.ftp_password = doc["ftp_password"].as<String>() ? doc["ftp_password"].as<String>() : "ftp";

  configFile.close();
}

void saveConfig()
{
  File configFile = LittleFS.open("/setup.json", "r");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, configFile);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));
  configFile.close();
  File file = LittleFS.open("/setup.json", "w");
  if (!file)
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  doc["ssid"] = config.ssid;
  doc["pass"] = config.pass;
  doc["ssid_td"] = config.ssid_td;
  doc["pass_td"] = config.pass_td;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }
  file.close();
}
