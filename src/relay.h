#pragma once
#ifdef ISRELEY
void initRelayMode()
{
    pinMode(config.controlPin,OUTPUT);
    digitalWrite(config.controlPin,LOW);
}

void doRelayMode(String payload)
{
    if (payload.equals("open"))
    {
        digitalWrite(config.controlPin, HIGH);
    }
    else
    {
        digitalWrite(config.controlPin, LOW);
    }
}
#endif