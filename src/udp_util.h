#pragma once

#include <LittleFS.h>
#include <WiFiUdp.h>
#include "config.hpp"

WiFiUDP Udp;
char ip_udp[16];
int port_udp;
char packetBuffer[255]; 
char  ReplyBuffer[] = "acknowledged";

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
    Udp.beginPacket(ip_udp, port_udp);
    Udp.write(ustr.c_str());
}
void handleUDP()
{
    int packetSize = Udp.parsePacket();
    if(packetSize)
    {
        // read the packet into packetBufffer
        int len = Udp.read(packetBuffer, 255);
        if (len > 0) packetBuffer[len] = 0;

        //Serial.println(packetBuffer);

        // send a reply, to the IP address and port that sent us the packet we received

        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(ReplyBuffer);
        Udp.endPacket();
    }
}
