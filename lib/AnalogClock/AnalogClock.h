/**
 * AnalogClock.h
 * 
 * Declaration of the class AnalogClock.The object myTFT is  
 * declared as external because it resides in the main program.
 */ 
#pragma once
#include <Arduino.h>
#include "lgfx_ESP32_2432S028.h"
#include "Wait.h"

extern LGFX lcd;

class AnalogClock 
{
  public:
    AnalogClock(uint32_t msRefresh) : _msRefresh(msRefresh) {}
    void setCompileTime(int sec_uploadCompensation = 0);
    void setup();
    void loop();
    bool isRunning();
    void stop();
    void updateDial();

  private:
    bool _isRunning = false;
    int _hh, _mm, _ss, _mx, _my, _rH, _rM, _rS, _rTh, _r, _rTm;
    uint32_t _msRefresh;
    Wait *_waitAnalogClock = new Wait(_msRefresh);
    struct tm _rtcTime;
    void drawDial(int mx, int my, int radius);
    void drawTickMarks(int mx, int my, int r1, int r2, int nbr, int color);
    void drawHands(int hh, int mm, int ss, int mx, int my, int rH, int rM, int rS);
};

