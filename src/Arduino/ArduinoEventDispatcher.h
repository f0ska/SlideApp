#pragma once

#include <Arduino.h>

class ArduinoEventDispatcher : public AbstractEventDispatcher
{
protected:
    unsigned long getMilliseconds()
    {
        return millis();
    }
};
