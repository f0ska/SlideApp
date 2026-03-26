#pragma once
#include <Arduino.h>
#include "../AbstractAction.h"

class ArduinoButtonPress
{
private:
    unsigned short gpio;
    AbstractAction* onPress;
    AbstractAction* onLongPress;
    bool isPressed = false;
    bool isLongPressed = false;
    unsigned long lastPress = 0;
    unsigned short minDelay = 10;
    unsigned short maxDelay = 800;

    void push(unsigned long time)
    {
        if (this->isLongPressed) {
            return;
        }

        this->isPressed = true;
        this->lastPress = time;
    }

    void release(unsigned long time)
    {
        this->isPressed = false;

        if (!this->isLongPressed && this->lastPress + this->minDelay < time) {
            this->onPress->execute();
        }

        this->isLongPressed = false;
    }

    void hold(unsigned long time)
    {
        if (this->isLongPressed || this->lastPress + this->maxDelay > time) {
            return;
        }

        this->isLongPressed = true;
        this->onLongPress->execute();
    }

public:
    ArduinoButtonPress(unsigned short gpio, AbstractAction* onPress, AbstractAction* onLongPress) : gpio(gpio), onPress(onPress), onLongPress(onLongPress)
    {
        pinMode(this->gpio, INPUT_PULLUP);
    };

    void loop(unsigned long time)
    {
        if (!digitalRead(this->gpio) && !this->isPressed) {
            this->push(time);
        }
        if (digitalRead(this->gpio) && this->isPressed) {
            this->release(time);
        }
        if (!digitalRead(this->gpio) && this->isPressed) {
            this->hold(time);
        }
    }
};
