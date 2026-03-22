#pragma once

#include <string>
using namespace std;

struct SlideConfig
{
    short slideAnimationDirectionX = -1;
    short slideAnimationDirectionY = 0;
    short slideAnimationDuration = 300;
    short slideAnimationDelay = 100;
    string transitionType = "easeIn";
};
