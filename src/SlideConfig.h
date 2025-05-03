#pragma once

struct SlideConfig
{
    short slideAnimationDirectionX = -1;
    short slideAnimationDirectionY = 0;
    short slideAnimationDuration = 1000;
    short slideAnimationDelay = 500;
    short mainSlideTime = 10;
    short subSlideTime = 5;
    bool reverseOnBackward = true;
    bool reverseOnSubSlide = true;
};
