#pragma once

#include <Arduino.h>

/**
 * Тактовая кнопка.
 * Программное устранение дребезга не блокирует поток выполнения.
 */
class Button
{
public:
    enum Event {
        NOTHING,
        PRESS,
        RELEASE
    };

    Button(uint8_t pin, bool activeState);

    /**
     * Возврашает
     * true, если кнопка нажата в данный момент;
     * false в противном случае.
     */
    bool isPressed();

    /**
     * Возврашает 
     * PRESS, если между предыдущим и текущим вызовом этого метода
     *      кнопка была нажата;
     * RELEASE, если между предыдущим и текущим вызовом этого метода
     *      кнопка была отпущена из нажатого состояния;
     * NOTHING в остальных случаях.
     */
    Event getLastEvent();

private:
    const uint8_t pin_;
    const bool activeState_;
    bool lastState_;
    uint32_t lastCheckTime_;
};