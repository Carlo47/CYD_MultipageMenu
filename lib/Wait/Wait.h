#include <Arduino.h>
#pragma once

/**
 * Class        Wait
 * Author       2024-04-05 Charles Geiser (https://www.dodeka.ch)
 * 
 * Purpose      Implements a class to wait without delay.
 *              Returns the result true as soon as the msWait milliseconds
 *              specified in the constructor have elapsed.
 * 
 * Usage        void doSomething() { Serial.println("Hello world"); }
 *              Wait waitForSomething(5000);
 *              waitForSomething.begin();
 *              void loop() 
 *              { 
 *                  if (waitForSomething.isOver()) doSomething(); // say hello every 5 sec
 *              }
*/
class Wait
{
    public:
        Wait(uint32_t msWait) : _msWait(msWait){};
        void begin() { _msPrevious = millis(); }
        bool isOver() { return (millis() - _msPrevious >= _msWait) ? (_msPrevious = millis(), true) : false; }
        void msWaitSet(uint32_t msWait) {_msWait = msWait;}

    private:
        uint32_t _msWait;
        uint32_t _msPrevious = 0;
};
