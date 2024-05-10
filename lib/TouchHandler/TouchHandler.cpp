#include "TouchHandler.h"

void TouchHandler::loop()
{
    int x, y;

  if (_lcd.getTouch(&x, &y))
  {
    if (_msPenDown == 0UL) 
    {
      _msPenDown = millis();  // save time, x and y
      _xPenDown  = x;         // of first touch (pen down) 
      _yPenDown  = y;
    }
    else
    {
      _xPenUp = x;  // save last x and y
      _yPenUp = y;
    }
  }
  else
  {
    if (_msPenDown > 0UL)
    { 
      //log_i("PEN_UP");
      _msPenUp = millis(); // save time when pen goes up 

      if (_msPenUp - _msPenDown > _msLongClickMinDuration)
      {
        _xDiff = _xPenUp - _xPenDown;
        _yDiff = _yPenUp - _yPenDown;
        if (_xDiff > _xMaxDiff) // swipe to the left
        {
            if (_onSwipeRight) _onSwipeRight(_xPenUp, _yPenUp);
        }
        else if (_xDiff < -_xMaxDiff) // swipe to the right
        {
            if (_onSwipeLeft) _onSwipeLeft(_xPenUp, _yPenUp);
        }
        else if (_yDiff > _yMaxDiff) // swipe down
        {
            if (_onSwipeDown) _onSwipeDown(_xPenUp, _yPenUp);
        }
        else if (_yDiff < -_yMaxDiff) // swipe up
        {
            if (_onSwipeUp) _onSwipeUp(_xPenUp, _yPenUp);
        }
        else
        {   // pen was long held down on same position 
            if (_onLongClick) _onLongClick(_xPenUp, _yPenUp);
        }
      }
      else if (_msPenUp - _msPenDown > _msShortClickMinDuration)
      {
           // pen was short held down on same position       
          if (_onShortClick) _onShortClick(_xPenUp, _yPenUp); 
      } 
      _msPenDown = 0UL;
    }
  }

}

void TouchHandler::addShortClickCb(Callback cb)
{ _onShortClick = cb; }

void TouchHandler::addLongClickCb(Callback cb)
{ _onLongClick = cb; }

void TouchHandler::addSwipeLeftCb(Callback cb)
{ _onSwipeLeft = cb; }

void TouchHandler::addSwipeRightCb(Callback cb)
{ _onSwipeRight = cb; }

void TouchHandler::addSwipeUpCb(Callback cb)
{ _onSwipeUp = cb; }

void TouchHandler::addSwipeDownCb(Callback cb)
{ _onSwipeDown = cb; }