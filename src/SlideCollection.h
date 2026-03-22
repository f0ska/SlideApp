#pragma once
#include <deque>
#include <string>
#include <map>
#include <math.h>
#include "AbstractSlide.h"
#include "SlideConfig.h"
#include "SlideDisplayInterface.h"
#include "AnimationTransiton.h"

using namespace std;

typedef std::map<std::string, int> SlideIndex;

class SlideCollection
{
private:
    SlideConfig* config;
    SlideDisplayInterface* display;
    AnimationTransiton* animation;
    deque<AbstractSlide*> slides;
    SlideIndex slideIndex;
    bool isAnimationRunning = false;
    bool animationPaused = false;
    unsigned long animationStartTime = 0;
    unsigned long currentSlideTime = 0;
    unsigned long currentTime = 0;
    AbstractSlide* currentSlide = nullptr;
    AbstractSlide* nextSlide = nullptr;
    string transitionType = "";

    void beforeSlidesRender();
    void afterSlidesRender();
    void moveQueueForward();
    void moveQueueBackward();
    void moveQueueToSlide(string slideName);
    void reindexSlides();

public:
    SlideCollection(SlideConfig* config, SlideDisplayInterface* display, AnimationTransiton* animation) : config(config), display(display), animation(animation) {}

    void updateCurrentTime(unsigned long millis)
    {
        this->currentTime = millis;
    }

    void addSlide(AbstractSlide* slide)
    {
        slide->setSlideSize(this->display->getDisplayWidth(), this->display->getDisplayHeight());
        slide->prepare();
        slides.push_back(slide);
    }

    void addOneTimeSlide(AbstractSlide* slide)
    {
        slide->setSlideSize(this->display->getDisplayWidth(), this->display->getDisplayHeight());
        slide->prepare();
        slide->setIsOneTime(true);
        this->slides.push_back(slide);
    }

    bool isAnimationPaused()
    {
        return this->animationPaused;
    }

    void setAnimationPaused(bool animationPaused)
    {
        this->animationPaused = animationPaused;
    }

    short getAnimationDuration()
    {
        return config->slideAnimationDuration;
    }

    short getAnimationDelay()
    {
        return config->slideAnimationDelay;
    }

    short getAnimationDirectionX(AbstractSlide* slide)
    {
        int reverse = slide->isReversed() ? 1 : -1;
        if (slide->isDirectionOverriden()) {
            return slide->getDirectionX();
        }
        return this->config->slideAnimationDirectionX * reverse;
    }

    short getAnimationDirectionY(AbstractSlide* slide)
    {
        int reverse = slide->isReversed() ? 1 : -1;
        if (slide->isDirectionOverriden()) {
            return slide->getDirectionY();
        }
        return this->config->slideAnimationDirectionY * reverse;
    }

    string getTransitionType(AbstractSlide* slide)
    {
        if (slide->getTransitionType() != "") {
            return slide->getTransitionType();
        }
        if (this->transitionType != "") {
            return this->transitionType;
        }
        return this->config->transitionType;
    }

    void setTransitionType(string transitionType)
    {
        this->transitionType = transitionType;
    }

    void refreshCurrentSlide();
    void loopSlides();
    void loopAnimation();
    void animationBegin(bool reverse = false);
    void animationEnd();

    void forward(bool reverse = false);
    void backward(bool reverse = true);
    void moveToSlide(string slideName, bool reverse = false);
    string getCurrentSlideName();
};
