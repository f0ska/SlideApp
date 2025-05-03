#pragma once
#include "../AbstractSlide.h"

class Dummy : public AbstractSlide
{
public:
    using AbstractSlide::AbstractSlide;

    void render();
    void onSlideChange();
    void prepare();
};
