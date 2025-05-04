#include "SlideCollection.h"

using namespace std;

void SlideCollection::beforeSlidesRender()
{
    this->display->displayClear();
}

void SlideCollection::afterSlidesRender()
{
    this->display->displayRender();
}

void SlideCollection::moveQueueForward()
{
    this->currentSlide = this->slides.back();
    this->nextSlide = this->slides.front();

    if (this->currentSlide->getIsOneTime()) {
        this->slides.pop_back();
    }

    this->slides.pop_front();
    this->slides.push_back(this->nextSlide);

    this->reindexSlides();
}

void SlideCollection::moveQueueBackward()
{
    this->currentSlide = this->slides.back();
    this->slides.pop_back();

    if (!this->currentSlide->getIsOneTime()) {
        this->slides.push_front(this->currentSlide);
    }

    this->nextSlide = this->slides.back();

    this->reindexSlides();
}

void SlideCollection::moveQueueToSlide(string slideName)
{
    this->currentSlide = this->slides.back();

    if (this->currentSlide->getIsOneTime()) {
        this->slides.pop_back();
    }

    int position = this->slideIndex.at(slideName);
    this->nextSlide = this->slides.at(position);

    auto it = this->slides.begin();
    int i = 0;
    for (auto it = this->slides.begin(); i != position; ++it, ++i) {}
    this->slides.erase(it);
    this->slides.push_back(this->nextSlide);

    this->reindexSlides();
}

void SlideCollection::reindexSlides()
{
    this->slideIndex.clear();
    for (unsigned int i = 0; i < this->slides.size(); i++) {
        this->slideIndex.insert({ this->slides.at(i)->getName(), i });
    }
}

void SlideCollection::refreshCurrentSlide()
{
    if (this->isAnimationRunning) {
        return;
    }

    if (this->slides.size() == 0) {
        return;
    }

    this->currentSlide = this->slides.back();

    this->beforeSlidesRender();
    this->currentSlide->render();
    this->afterSlidesRender();
}

void SlideCollection::loopSlides()
{
    if (this->isAnimationRunning) {
        return;
    }

    if (this->slides.size() == 0) {
        return;
    }

    this->currentSlide = this->slides.back();

    if (this->currentSlide->isAutomatic() && (this->currentTime - this->currentSlideTime) >= this->currentSlide->getSlideTime()) {
        this->currentSlideTime = this->currentTime;
        this->animationBegin();
        return;
    }

    this->beforeSlidesRender();
    this->currentSlide->render();
    this->afterSlidesRender();
}

void SlideCollection::loopAnimation()
{
    if (!this->isAnimationRunning) {
        return;
    }

    short time = this->currentTime - this->animationStartTime;
    if (time > (this->getAnimationDuration() + this->getAnimationDelay())) {
        this->animationEnd();
        return;
    }

    this->currentSlide->setSlidePosition(
        this->animate(time, this->currentSlide->getStartX(), this->currentSlide->getDiffX(), this->getAnimationDuration()),
        this->animate(time, this->currentSlide->getStartY(), this->currentSlide->getDiffY(), this->getAnimationDuration())
    );

    this->nextSlide->setSlidePosition(
        this->animate(max(time - this->getAnimationDelay(), 0), this->nextSlide->getStartX(), this->nextSlide->getDiffX(), this->getAnimationDuration()),
        this->animate(max(time - this->getAnimationDelay(), 0), this->nextSlide->getStartY(), this->nextSlide->getDiffY(), this->getAnimationDuration())
    );

    this->beforeSlidesRender();
    this->currentSlide->render();
    this->nextSlide->render();
    this->afterSlidesRender();
}

void SlideCollection::animationBegin()
{
    this->nextSlide->setIsActive(true);
    this->nextSlide->onSlideActivate();

    this->currentSlide->setSlideStart(0, 0);
    this->currentSlide->setSlideDiff(this->currentSlide->getW() * this->getAnimationDirectionX(), this->currentSlide->getH() * this->getAnimationDirectionY());

    this->nextSlide->setSlideStart(this->nextSlide->getW() * this->getAnimationDirectionX() * -1, this->nextSlide->getH() * this->getAnimationDirectionY() * -1);
    this->nextSlide->setSlideDiff(this->nextSlide->getW() * this->getAnimationDirectionX(), this->nextSlide->getH() * this->getAnimationDirectionY());

    this->animationStartTime = this->currentTime;
    this->isAnimationRunning = true;
}

void SlideCollection::animationEnd()
{
    this->isAnimationRunning = false;

    this->currentSlide->setIsActive(false);
    this->currentSlide->onSlideChange();
    if (this->currentSlide->getIsOneTime()) {
        delete this->currentSlide;
    }

    this->nextSlide->setSlidePosition(
        this->nextSlide->getStartX() + this->nextSlide->getDiffX(),
        this->nextSlide->getStartY() + this->nextSlide->getDiffY()
    );

    this->beforeSlidesRender();
    this->nextSlide->render();
    this->afterSlidesRender();
}

void SlideCollection::forward()
{
    if (this->isAnimationRunning) {
        this->animationEnd();
    }

    this->moveQueueForward();
    this->animationBegin();
}

void SlideCollection::backward()
{
    if (this->isAnimationRunning) {
        this->animationEnd();
    }

    this->moveQueueBackward();
    this->animationBegin();
}

void SlideCollection::moveToSlide(string slideName)
{
    if (this->isAnimationRunning) {
        this->animationEnd();
    }

    this->moveQueueToSlide(slideName);
    this->animationBegin();
}

string SlideCollection::getCurrentSlideName()
{
    if (this->slides.size() == 0) {
        return string("");
    }

    return this->slides.back()->getName();
}

short SlideCollection::animate(short time, short beginningVal, short changeInVal, short duration)
{
    switch (this->animationTransition) {
        case AnimationTransiton::easeIn:
            return this->easeIn(time, beginningVal, changeInVal, duration);
        case AnimationTransiton::linear:
        default:
            return this->linear(time, beginningVal, changeInVal, duration);
    }
}

short SlideCollection::easeIn(short time, short beginningVal, short changeInVal, short duration)
{
    float progress = (float) time / (float) duration;
    return (short) round(progress * progress * progress * (float) changeInVal + (float) beginningVal);
}

short SlideCollection::linear(short time, short beginningVal, short changeInVal, short duration)
{
    float progress = (float) time / (float) duration;
    return (short) round(progress * (float) changeInVal + (float) beginningVal);
}
