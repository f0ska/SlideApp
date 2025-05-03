#pragma once

enum class EventType
{
    MIL,
    SEC,
    MIN,
    HOUR,
    BG
};

class AbstractEvent
{
protected:
    EventType type;
    bool canRun = true;
    unsigned long prevTime = 0;
    unsigned short interval;

public:
    explicit AbstractEvent(EventType type, unsigned short interval = 1) : type(type), interval(interval) {};

    virtual void execute(unsigned long time) = 0;

    virtual void prepare() {}

    EventType getType()
    {
        return type;
    }

    void setInterval(unsigned short interval)
    {
        this->interval = interval;
    }

    unsigned short getInterval()
    {
        return this->interval;
    }

    void setPreviousTime(unsigned long time)
    {
        this->prevTime = time;
    }

    unsigned long getPreviousTime()
    {
        return this->prevTime;
    }
};
