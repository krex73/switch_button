#pragma once
#ifndef ISRELEY
#include "udp_util.h"
#include <ESP8266WiFi.h>
#include "AsyncMqttClient.h"
#include "Bounce2.h"


Bounce * bounce;

extern SimpleTimer Timer;
extern AsyncMqttClient *mqttClient;

void sendState()
{
    if(mqttClient->connected()){
        mqttClient->publish((config.mqttPrefix+"/"+config.topicName).c_str(),0,true, (const char*)(bounce->read() == HIGH ? "open" : "closed"));
    }
}

void doSensorMode()
{
    bounce->update();
    if (bounce->changed())
    {
        sendState();
    }
}

void initSensorMode()
{
    pinMode(config.sensorPin, INPUT_PULLUP);
    bounce = new Bounce();
    bounce->attach(config.sensorPin);

    pinMode(config.sensorStateLEDPin, OUTPUT);
    digitalWrite(config.sensorStateLEDPin, LOW);

    Timer.setInterval(config.sensorSendInterval, sendState);
}
#endif
