
#include "AbstractEvent.h"
#include <bits/stdc++.h>
#include <map>

using namespace std;

typedef unsigned long (*getRealtimePtr)();

class AbstractEventDispatcher
{
private:
    unsigned long prevSeconds = 0;
    unsigned long prevMiliseconds = 0;
    unsigned long prevMinute = 0;
    unsigned long prevHour = 0;
    getRealtimePtr realtime = nullptr;
    multimap<EventType, AbstractEvent*> observers;

    void runEvents(EventType type);
    bool canTriggerEvent(AbstractEvent* event);

protected:
    virtual unsigned long getMilliseconds() = 0;
    virtual unsigned long getSeconds();
    virtual unsigned long getMinutes();
    virtual unsigned long getHours();

public:
    void dispatchTimeEvents();

    void addEvent(AbstractEvent* event)
    {
        observers.insert({ event->getType(), event });
    }

    void setRealTimeFunction(getRealtimePtr realtime)
    {
        this->realtime = realtime;
    }
};
