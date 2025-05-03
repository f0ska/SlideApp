#pragma once
#include <Arduino.h>
#include <deque>
#include <string>
#include <map>
#include <math.h>
#include "AbstractSlide.h"
#include "SlideConfig.h"
#include "SlideDisplayInterface.h"

using namespace std;

enum class AnimationTransiton
{
    linear,
    easeIn,
};

typedef std::map<std::string, int> SlideIndex;

class SlideCollection
{
private:
    deque<AbstractSlide*> slides;
    SlideIndex slideIndex;
    bool isAnimationRunning = false;
    unsigned long animationStartTime;
    unsigned long currentSlideTime = 0;
    unsigned long currentTime = 0;
    AnimationTransiton animationTransition = AnimationTransiton::linear;
    SlideConfig* config;
    SlideDisplayInterface* display;
    AbstractSlide* currentSlide = nullptr;
    AbstractSlide* nextSlide = nullptr;

    void beforeSlidesRender();
    void afterSlidesRender();
    void moveQueueForward();
    void moveQueueBackward();
    void moveQueueToSlide(string slideName);
    void reindexSlides();

public:
    SlideCollection(SlideConfig* config, SlideDisplayInterface* display) : config(config), display(display) {}

    void updateCurrentTime(unsigned long millis)
    {
        this->currentTime = millis;
    }

    void addSlide(AbstractSlide* slide)
    {
        slides.push_back(slide);
    }

    void addOneTimeSlide(AbstractSlide* slide)
    {
        slide->setIsOneTime(true);
        this->slides.push_back(slide);
        if (!this->isAnimationRunning) {
            this->currentSlideTime = 0;
        }
    }

    short getAnimationDuration()
    {
        return config->slideAnimationDuration;
    }

    short getAnimationDelay()
    {
        return config->slideAnimationDelay;
    }

    short getAnimationDirectionX()
    {
        int reverse = this->config->reverseOnSubSlide && this->currentSlide->getIsMain() ? 1 : -1;
        return this->config->slideAnimationDirectionX * reverse;
    }

    short getAnimationDirectionY()
    {
        int reverse = this->config->reverseOnSubSlide && this->currentSlide->getIsMain() ? 1 : -1;
        return this->config->slideAnimationDirectionY * reverse;
    }

    void loopSlides();
    void loopAnimation();
    void animationBegin();
    void animationEnd();

    void forward();
    void backward();
    void moveToSlide(string slideName);
    string getCurrentSlideName();


    short animate(short currentTime, short beginningVal, short changeInVal, short duration);
    short easeIn(short currentTime, short beginningVal, short changeInVal, short duration);
    short linear(short currentTime, short beginningVal, short changeInVal, short duration);
};

