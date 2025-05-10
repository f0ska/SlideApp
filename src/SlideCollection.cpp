#include "SlideCollection.h"

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

    if (!this->isAnimationPaused() && (this->currentTime - this->currentSlideTime) >= this->currentSlide->getSlideTime()) {
        this->currentSlideTime = this->currentTime;
        this->moveQueueForward();
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
        this->animation->animate(this->getTransitionType(this->currentSlide), time, this->currentSlide->getStartX(), this->currentSlide->getDiffX(), this->getAnimationDuration()),
        this->animation->animate(this->getTransitionType(this->currentSlide), time, this->currentSlide->getStartY(), this->currentSlide->getDiffY(), this->getAnimationDuration())
    );

    this->nextSlide->setSlidePosition(
        this->animation->animate(this->getTransitionType(this->nextSlide), max(time - this->getAnimationDelay(), 0), this->nextSlide->getStartX(), this->nextSlide->getDiffX(), this->getAnimationDuration()),
        this->animation->animate(this->getTransitionType(this->nextSlide), max(time - this->getAnimationDelay(), 0), this->nextSlide->getStartY(), this->nextSlide->getDiffY(), this->getAnimationDuration())
    );

    this->beforeSlidesRender();
    this->currentSlide->render();
    this->nextSlide->render();
    this->afterSlidesRender();
}

void SlideCollection::animationBegin(bool reverse)
{
    this->nextSlide->setIsActive(true);
    this->nextSlide->setReversed(reverse);
    this->nextSlide->onSlideActivate();

    this->currentSlide->setSlideStart(0, 0);
    this->currentSlide->setReversed(reverse);
    this->currentSlide->setSlideDiff(this->currentSlide->getW() * this->getAnimationDirectionX(this->currentSlide), this->currentSlide->getH() * this->getAnimationDirectionY(this->currentSlide));

    this->nextSlide->setSlideStart(this->nextSlide->getW() * this->getAnimationDirectionX(this->nextSlide) * -1, this->nextSlide->getH() * this->getAnimationDirectionY(this->nextSlide) * -1);
    this->nextSlide->setSlideDiff(this->nextSlide->getW() * this->getAnimationDirectionX(this->nextSlide), this->nextSlide->getH() * this->getAnimationDirectionY(this->nextSlide));

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

void SlideCollection::forward(bool reverse)
{
    if (this->isAnimationRunning) {
        this->animationEnd();
    }

    this->moveQueueForward();
    this->animationBegin(reverse);
}

void SlideCollection::backward(bool reverse)
{
    if (this->isAnimationRunning) {
        this->animationEnd();
    }

    this->moveQueueBackward();
    this->animationBegin(reverse);
}

void SlideCollection::moveToSlide(string slideName, bool reverse)
{
    if (this->isAnimationRunning) {
        this->animationEnd();
    }

    this->moveQueueToSlide(slideName);
    this->animationBegin(reverse);
}

string SlideCollection::getCurrentSlideName()
{
    if (this->slides.size() == 0) {
        return string("");
    }

    return this->slides.back()->getName();
}
