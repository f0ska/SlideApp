#pragma once

#include "../AbstractEvent.h"
#include "../AbstractSlide.h"

class SlideEvent : public AbstractEvent
{
private:
    AbstractSlide* slide;

public:
    using AbstractEvent::AbstractEvent;

    void execute(unsigned long time)
    {
        this->slide->render();
    }

    void setSlide(AbstractSlide* slide)
    {
        this->slide = slide;
    }
};
