#pragma once

#include <string>

using namespace std;

enum class SlideType
{
    NONE,
    MAIN,
    SUB,
};

class AbstractSlide
{
protected:
    string name;
    SlideType type;
    bool isActive = false;
    short x = 0;
    short y = 0;
    unsigned short w = 0;
    unsigned short h = 0;
    short startX;
    short diffX;
    short startY;
    short diffY;
    unsigned long slideTime = 0;
    bool isOneTime = false;
    bool isStatic = true;

public:
    explicit AbstractSlide(string name, SlideType type = SlideType::NONE) : name(name), type(type)
    {
        this->prepare();
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

    bool isAutomatic() {
        return this->type != SlideType::NONE;
    }

    bool getIsMain()
    {
        return this->type != SlideType::SUB;
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

    bool getIsStatic()
    {
        return this->isStatic;
    }

    void setIsOneTime(bool isOneTime)
    {
        this->isOneTime = isOneTime;
    }

    string getName()
    {
        return this->name;
    }

    virtual void prepare() {};

    virtual void onSlideActivate() {};

    virtual void onSlideChange() {};

    virtual void render() = 0;
};
