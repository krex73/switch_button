#pragma once

#include <LittleFS.h>

void add_log(char* logstr)
{
    File f = LittleFS.open("/log.txt", "a");
    if (!f)
    {
        Serial.println("file open failed"); //  "открыть файл не удалось"
        return;
    }
    f.write((const char*)logstr);
    f.close();
}
