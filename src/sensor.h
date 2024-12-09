#pragma once
#include "udp_util.h"
#include <Bounce2.h>
#include <ESP8266WiFi.h>
extern Bounce *bounce;

void doSensorMode()
{
    bounce->update();
    if (WiFi.status() != WL_CONNECTED)
    {
        if (bounce->changed())
             sendUDP(bounce->read() == HIGH ? "open":"closed");
    }
}
