#pragma once

#include <string>

struct SlideConfig
{
    short slideAnimationDirectionX = -1;
    short slideAnimationDirectionY = 0;
    short slideAnimationDuration = 300;
    short slideAnimationDelay = 100;
    std::string transitionType = "easeIn";
};
