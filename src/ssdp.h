#include <ESP8266SSDP.h>
#include <GyverPortal.h>
#include "config.hpp"

extern GyverPortal ui;

WiFiUDP Udp2;

void SSDP_init(void)
{
  // SSDP дескриптор
  ui.server.on("/description.xml", HTTP_GET, []()
               { SSDP.schema(ui.server.client()); });
  // Если версия  2.0.0 закаментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName((config.thisSensorMode) ? config.sensorSSDPName : config.controlSSDPName);
  SSDP.setSerialNumber("001788102201");
  SSDP.setURL("/");
  SSDP.setModelName((config.thisSensorMode) ? config.sensorSSDPName : config.controlSSDPName);
  SSDP.setModelNumber("001");
  SSDP.setModelURL("");
  SSDP.setManufacturer("Krex");
  SSDP.setManufacturerURL("");
  SSDP.begin();
  Udp2.begin(1901);
}
void requestSSDP()
{ 
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Request SSDP");
    IPAddress ssdpAdress(239, 255, 255, 250);
    unsigned int ssdpPort = 1900;
    char ReplyBuffer[] = "M-SEARCH * HTTP/1.1\r\nHost:239.255.255.250:1900\r\nST:upnp:rootdevice\r\nMan:\"ssdp:discover\"\r\nMX:3\r\n\r\n";
    Udp2.beginPacket(ssdpAdress, ssdpPort);
    Udp2.write(ReplyBuffer);
    Udp2.endPacket();
  }else{
    Serial.println("Wi-Fi not connected (Request SSDP)");
  }
}

String deleteBeforeDelimiter(String str, String found) {
  int p = str.indexOf(found) + found.length();
  return str.substring(p);
}

// -------------------Удаляем строку от конца строки до маркера ---------------------
String deleteToMarkerLast (String str, String found) {
  int p = str.lastIndexOf(found);
  return str.substring(0, p);
}

void handleSSDP()
{
  if (WiFi.status() == WL_CONNECTED)
  { // Если есть подключение к роутеру
    String input_string;
    String chipIDremote;
    String ssdpName;
    char packetBuffer[512];
    int packetSize = Udp2.parsePacket();
    if (packetSize)
    {
      int len = Udp2.read(packetBuffer, 512);
      if (len > 0) packetBuffer[len] = 0;
      input_string += packetBuffer;
      int i = input_string.indexOf(config.sensorSSDPName);
      //Serial.println(packetBuffer);
//    Если нашли кнопку
      if (i > 0){
        //Serial.println(packetBuffer);
        chipIDremote = deleteBeforeDelimiter(input_string, "LOCATION: http://");
        chipIDremote = deleteToMarkerLast(chipIDremote,":80/description.xml");
        Serial.print("================ ");
        Serial.println(chipIDremote);
        config.sensorIP=chipIDremote;
      }
    }
  }
}
