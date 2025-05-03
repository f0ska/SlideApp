#pragma once

#include "../AbstractEvent.h"
#include "../SlideCollection.h"

class SlideSwitchEvent : public AbstractEvent
{
private:
    SlideCollection* collection;

public:
    using AbstractEvent::AbstractEvent;

    void setCollection(SlideCollection* collection)
    {
        this->collection = collection;
    }

    void execute(unsigned long time)
    {
        this->collection->loopSlides();
    }
};
