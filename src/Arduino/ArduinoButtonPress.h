#pragma once
#include <Arduino.h>

using function_type = void();

class ArduinoButtonPress
{
private:
    unsigned short gpio;
    function_type* onPress;
    function_type* onLongPress;
    bool isPreessed = false;
    bool isLongPreessed = false;
    unsigned long lastPress = 0;
    unsigned short minDelay = 10;
    unsigned short maxDelay = 800;

    void push(unsigned long time)
    {
        if (this->isLongPreessed) {
            return;
        }

        this->isPreessed = true;
        this->lastPress = time;
    }

    void release(unsigned long time)
    {
        this->isPreessed = false;

        if (!this->isLongPreessed && this->lastPress + this->minDelay < time) {
            this->onPress();
        }

        this->isLongPreessed = false;
    }

    void hold(unsigned long time)
    {
        if (this->isLongPreessed || this->lastPress + this->maxDelay > time) {
            return;
        }

        this->isLongPreessed = true;
        this->onLongPress();
    }

public:
    ArduinoButtonPress(unsigned short gpio, function_type* onPress, function_type* onLongPress) : gpio(gpio), onPress(onPress), onLongPress(onLongPress)
    {
        pinMode(this->gpio, INPUT_PULLUP);
    };

    void loop(unsigned long time)
    {
        if (!digitalRead(this->gpio) && !this->isPreessed) {
            this->push(time);
        }
        if (digitalRead(this->gpio) && this->isPreessed) {
            this->release(time);
        }
        if (!digitalRead(this->gpio) && this->isPreessed) {
            this->hold(time);
        }
    }
};
