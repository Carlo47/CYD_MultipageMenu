#include "lgfx_ESP32_2432S028.h"

using Callback = void (*)(int x, int y);

class TouchHandler
{
    public:
        TouchHandler(LGFX &lcd) : _lcd(lcd) {}
        void loop();
        void addShortClickCb(Callback cb);
        void addLongClickCb(Callback cb);
        void addSwipeLeftCb(Callback cb);
        void addSwipeRightCb(Callback cb);
        void addSwipeUpCb(Callback cb);
        void addSwipeDownCb(Callback cb);

    private:
        LGFX &_lcd;
        unsigned long _msPenDown = 0UL;
        unsigned long _msPenUp   = 0UL;
        int _xPenDown, _yPenDown;
        int _xPenUp,   _yPenUp;
        int _xDiff, _yDiff;
        const int _yMaxDiff = 50;
        const int _xMaxDiff = 50;
        const unsigned long _msLongClickMinDuration = 280UL;
        const unsigned long _msShortClickMinDuration = 35UL;

        Callback _onShortClick = nullptr;
        Callback _onLongClick  = nullptr;
        Callback _onSwipeLeft   = nullptr;
        Callback _onSwipeRight  = nullptr;
        Callback _onSwipeUp    = nullptr;
        Callback _onSwipeDown  = nullptr;

};