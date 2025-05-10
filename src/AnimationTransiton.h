#pragma once

#include <math.h>

using namespace std;

class AnimationTransiton
{
protected:
    short easeIn(short time, short beginningVal, short changeInVal, short duration)
    {
        float progress = (float) time / (float) duration;
        return (short) round(progress * progress * progress * (float) changeInVal + (float) beginningVal);
    }

    short linear(short time, short beginningVal, short changeInVal, short duration)
    {
        float progress = (float) time / (float) duration;
        return (short) round(progress * (float) changeInVal + (float) beginningVal);
    }

public:
    virtual short animate(string transition, short time, short beginningVal, short changeInVal, short duration)
    {
        if (transition == "easeIn") {
            return this->easeIn(time, beginningVal, changeInVal, duration);
        }

        if (transition == "linear") {
            return this->linear(time, beginningVal, changeInVal, duration);
        }

        return beginningVal + changeInVal;
    }
};
