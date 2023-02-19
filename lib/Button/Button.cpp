#include "Button.h"

Button::Button(uint8_t pin, bool activeState) 
: pin_(pin), activeState_(activeState), lastState_(0), lastCheckTime_(0)
{
    pinMode(pin_, activeState ? INPUT : INPUT_PULLUP);
}

bool Button::isPressed()
{
    return digitalRead(pin_) == activeState_;
}

Button::Event Button::getLastEvent()
{
    Event lastEvent = NOTHING;
    bool state = isPressed();
    
    if (state != lastState_ && millis() - lastCheckTime_ > 5) {
        lastCheckTime_ = millis();
        if (!lastState_ && state)
            lastEvent = PRESS;
        else if (lastState_ && !state)
            lastEvent = RELEASE;
        lastState_ = state;
    }
    return lastEvent;
}