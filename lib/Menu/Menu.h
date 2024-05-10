/**
 * Menu.h
 * 
 * Declaration of the class Menu. The constructor needs a reference to 
 * the TFT object, the array of menu entries and their number, as well 
 * as the number of menuitems to be displayed on a menu page. 
 * If the default value 0 is used, an attempt is made to show all menu 
 * lines, which can, however, result in an overflow on the display.
 */ 
#pragma once
#include "lgfx_ESP32_2432S028.h"

using MenuItem = struct mItem{ const char *txt; void (&action)(); };
enum direction {LEFT, RIGHT, UP, DOWN};

class Menu
{
  public:
    Menu(LGFX &lcd, MenuItem menuItems[], int nbrMenuItems, int nbrDisplayedMenuItems = 0) : 
      _lcd(lcd), 
      _menuItems(menuItems), 
      _nbrMenuItems(nbrMenuItems),
      _nbrDisplayedMenuItems(nbrDisplayedMenuItems)
    { 
    }

    void setup();
    void show(bool clearScreen = true);
    void onTouch(uint8_t touchedItem);
    void OnSwipe(uint8_t direction);

  private:
    LGFX &_lcd;
    MenuItem *_menuItems;   // Pointer to the array of menuitems
    uint8_t  _nbrMenuItems;
    int      _selectedMenuItem = 0;
    int      _startMenuItem = 0;
    int      _stopMenuItem;
    int      _menuPage = 0;
    int      _nbrDisplayedMenuItems;
    int      _state = 0;
};
