#pragma once

class SlideDisplayInterface
{
public:
    virtual void displayClear() = 0;
    virtual void displayRender() = 0;
    virtual short getDisplayWidth() = 0;
    virtual short getDisplayHeight() = 0;
};
