#pragma once

#include <string>

using namespace std;

class AbstractSlide
{
protected:
    string name;
    unsigned long slideTime = 0;
    bool isActive = false;
    short x = 0;
    short y = 0;
    unsigned short w = 0;
    unsigned short h = 0;
    short startX;
    short diffX;
    short startY;
    short diffY;
    bool isOneTime = false;
    bool reversed = false;
    string transitionType = "";
    short directionX = 0;
    short directionY = 0;
    bool directionOverriden = false;
    bool reverseAnimation = false;

public:
    explicit AbstractSlide(string name, unsigned short slideTime = 0) : name(name), slideTime(slideTime)
    {
    }

    virtual ~AbstractSlide() {}

    unsigned long getSlideTime()
    {
        return this->slideTime;
    }

    void setSlideTime(unsigned short slideTime)
    {
        this->slideTime = slideTime;
    }

    bool isReversed()
    {
        return this->reversed;
    }

    void setReversed(bool reversed)
    {
        this->reversed = reversed;
    }

    bool getIsActive()
    {
        return this->isActive;
    }

    void setIsActive(bool isActive)
    {
        this->isActive = isActive;
    }

    short getX()
    {
        return this->x;
    }

    short getY()
    {
        return this->y;
    }

    short getW()
    {
        return this->w;
    }

    short getH()
    {
        return this->h;
    }

    short getStartX()
    {
        return this->startX;
    }

    short getStartY()
    {
        return this->startY;
    }

    short getDiffX()
    {
        return this->diffX;
    }

    short getDiffY()
    {
        return this->diffY;
    }

    void setSlidePosition(short x, short y)
    {
        this->x = x;
        this->y = y;
    }

    void setSlideStart(short startX, short startY)
    {
        this->startX = startX;
        this->startY = startY;
        this->x = startX;
        this->y = startY;
    }

    void setSlideDiff(short diffX, short diffY)
    {
        this->diffX = diffX;
        this->diffY = diffY;
    }

    void setSlideSize(unsigned short w, unsigned short h)
    {
        this->w = w;
        this->h = h;
    }

    bool getIsOneTime()
    {
        return this->isOneTime;
    }

    void setIsOneTime(bool isOneTime)
    {
        this->isOneTime = isOneTime;
    }

    string getTransitionType()
    {
        return this->transitionType;
    }

    void setTransitionType(string transitionType)
    {
        this->transitionType = transitionType;
    }

    short getDirectionX()
    {
        return this->directionX;
    }

    short getDirectionY()
    {
        return this->directionY;
    }

    bool isDirectionOverriden()
    {
        return this->directionOverriden;
    }

    void setAnimationDirection(short directionX, short directionY)
    {
        this->directionX = directionX;
        this->directionY = directionY;
        this->directionOverriden = true;
    }

    string getName()
    {
        return this->name;
    }
    
    bool getReverseAnimation()
    {
        return this->reverseAnimation;
    }
    
    void setReverseAnimation(bool reverseAnimation)
    {
        this->reverseAnimation = reverseAnimation;
    }

    virtual void prepare() {};

    virtual void onSlideActivate() {};

    virtual void onSlideChange() {};

    virtual void render() = 0;
};
