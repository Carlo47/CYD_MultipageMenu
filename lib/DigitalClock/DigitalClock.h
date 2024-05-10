/**
 * DigitalClock.h
 * 
 * Declaration of the class DigitalClock.The object tft is  
 * declared as external because it resides in the main program.
 */ 
#pragma once
#include <Arduino.h>
#include "lgfx_ESP32_2432S028.h"
#include "Wait.h"


extern LGFX lcd;


class DigitalClock
{
  public:
    DigitalClock(uint32_t msRefresh) : _msRefresh(msRefresh) {}
    void setCompileTime(int sec_uploadCompensation = 0);
    void setup();
    void loop();
    bool isRunning();
    void stop();

  private:
    uint32_t _msRefresh;
    Wait *_waitDigitalClock = new Wait(_msRefresh);
    bool _isRunning = false;
    struct tm _rtcTime;
    char _s[20];
    
};
