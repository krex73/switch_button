#pragma once

#define TIMER_WIFI_CONNECT 1
#define TIMER_SEND_UDP 2
#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

// Пин шины OneWire для поиска и опроса датчиков
#define ONE_WIRE_BUS 4

#define WIDTH_NAME_SENSORS 2

struct
{
  // false - режим блока управления, true режим датчика
  boolean thisSensorMode = false;

  int sensorPin = 5;
  int controlPin = 6;
  int ledPin = 7;

  boolean enableMQTT = false;
  String mqttPrefix = "";
  String mqttServer = "";
  String mqttPassword = "";
  String mqttUser = "";
  int mqttPort = 1883;

  String controlSSDPName = "control_1";
  String sensorSSDPName = "sensor_1";

  int interval = 10;
  int changePort = 8888;
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
  File configFile = LittleFS.open("/setup.json", "r");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, configFile);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  config.enableMQTT = doc["enableMQTT"].as<boolean>();
  if (config.enableMQTT)
  {
    config.mqttServer = doc["mqttServer"].as<String>();
    config.mqttUser = doc["mqttUser"].as<String>();
    Serial.println(F("-----------------------------------"));
    Serial.println(config.mqttUser);
    Serial.println(F("-----------------------------------"));
    config.mqttPassword = doc["mqttPassword"].as<String>();
    config.mqttPort = doc["mqttPort"];
    config.interval = doc["interval"];
    config.mqttPrefix = doc["mqttPrefix"].as<String>();
  };

  config.thisSensorMode = doc["thisSensorMode"].as<boolean>();

  config.controlSSDPName = doc["controlSSDPName"].as<String>();
  config.sensorSSDPName = doc["sensorSSDPName"].as<String>();

  config.interval = doc["interval"];

  config.changePort = doc["changePort"];
  config.ledPin = doc["ledPin"];
  
  config.ssid = doc["ssid"].as<String>();
  config.pass = doc["pass"].as<String>();
  config.ssid_td = (config.thisSensorMode) ? config.sensorSSDPName : config.controlSSDPName;
  config.pass_td = doc["pass_td"].as<String>();
  config.ftp_name = doc["ftp_name"].as<String>();
  config.ftp_password = doc["ftp_password"].as<String>();

  Serial.println("-------------------------------------------------------------");
  Serial.println(config.ssid_td);
  Serial.println("-------------------------------------------------------------");
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
