#pragma once
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <WiFiUdp.h>
#include "config.hpp"

WiFiUDP udp;

char ip_udp[16];
int port_udp;
char packetBuffer[255];
char ReplyBuffer[] = "acknowledged";

void initUDP()
{
    // PairsFile sensFile(&LittleFS, "/udp.ini", 3000);
    // sensFile.begin();
    // sensFile.get("ip").toChar(ip_udp, 16);
    // port_udp = sensFile.get("port").toInt();

    // sensFile.clear();
}

void sendUDP(String ustr)
{
    //    Udp.beginPacket(config., port_udp);
    //    Udp.write(ustr.c_str());
}

void logUDP(String ustr)
{
    Serial.println(config.udpServer.c_str());
    Serial.println(config.udpPort);
    if (WiFi.status() == WL_CONNECTED)
    {
        udp.beginPacket(config.udpServer.c_str(), config.udpPort);
        udp.write(ustr.c_str());
        udp.endPacket();
    }
}

void logUDP(boolean ustr)
{
    Serial.println(config.udpServer.c_str());
    Serial.println(config.udpPort);
    if (WiFi.status() == WL_CONNECTED)
    {
        udp.beginPacket(config.udpServer.c_str(), config.udpPort);
        udp.write(ustr);
        udp.endPacket();
    }
}

void handleUDP()
{
    // int packetSize = Udp.parsePacket();
    // if (packetSize)
    // {
    //     // read the packet into packetBufffer
    //     int len = Udp.read(packetBuffer, 255);
    //     if (len > 0)
    //         packetBuffer[len] = 0;

    //     // Serial.println(packetBuffer);

    //     // send a reply, to the IP address and port that sent us the packet we received

    //     Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //     Udp.write(ReplyBuffer);
    //     Udp.endPacket();
    // }
}
