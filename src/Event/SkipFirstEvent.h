#pragma once

#include "../AbstractEvent.h"

// Base class for events that must skip their first execution.
// Useful for HOUR/SEC events that would fire spuriously on startup.
class SkipFirstEvent : public AbstractEvent
{
private:
    bool _firstSkipped = false;

protected:
    // Returns true (and consumes the skip) on the very first call.
    bool skipFirst()
    {
        if (!_firstSkipped) {
            _firstSkipped = true;
            return true;
        }
        return false;
    }

public:
    using AbstractEvent::AbstractEvent;
};
