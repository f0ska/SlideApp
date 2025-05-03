#include "AbstractEventDispatcher.h"

using namespace std;

void AbstractEventDispatcher::dispatchTimeEvents()
{
    unsigned long currentMillis = this->getMilliseconds();
    unsigned int sec = this->getSeconds();
    unsigned int min = round(sec / 60);
    unsigned int hour = round(min / 60);

    if (currentMillis != this->prevMiliseconds) {
        this->prevMiliseconds = currentMillis;
        this->runEvents(EventType::MIL);
    }

    if (sec != this->prevSeconds) {
        this->prevSeconds = sec;
        this->runEvents(EventType::SEC);
    }

    if (min != this->prevMinute) {
        this->prevMinute = min;
        this->runEvents(EventType::MIN);
    }

    if (hour != this->prevHour) {
        this->prevHour = hour;
        this->runEvents(EventType::HOUR);
    }
}

void AbstractEventDispatcher::runEvents(EventType type)
{
    auto found = this->observers.find(type);

    if (found == this->observers.end()) {
        return;
    }

    auto range = this->observers.equal_range(type);
    for (auto i = range.first; i != range.second; ++i) {
        if (this->canTriggerEvent(i->second)) {
            i->second->execute(this->prevMiliseconds);
            i->second->setPreviousTime(this->prevMiliseconds);
        }
    }
}

bool AbstractEventDispatcher::canTriggerEvent(AbstractEvent* event)
{
    switch (event->getType()) {
        case EventType::SEC:
            return this->prevSeconds % event->getInterval() == 0;
        case EventType::MIN:
            return this->prevMinute % event->getInterval() == 0;
        case EventType::HOUR:
            return this->prevHour % event->getInterval() == 0;
        default:
            break;
    }

    int spent = this->prevMiliseconds - event->getPreviousTime();
    return spent >= event->getInterval();
}

unsigned long AbstractEventDispatcher::getSeconds()
{
    if (this->realtime != nullptr) {
        return this->realtime();
    }
    return round(this->getMilliseconds() / 1000);
}

unsigned long AbstractEventDispatcher::getMinutes()
{
    return round(this->getSeconds() / 60);
}

unsigned long AbstractEventDispatcher::getHours()
{
    return round(this->getMinutes() / 60);
}
