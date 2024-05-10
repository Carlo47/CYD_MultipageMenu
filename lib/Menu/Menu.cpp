/**
 * Class        Menu
 * Author       2024-05-10 Charles Geiser
 * 
 * Purpose      Implements a class Menu which shows menuitems on a TFT display.
 *              The number of menuitems is not limited. If there are more items
 *              than the display can show, the other menuitems are offered page by page. 
 *              How many menuitems a menu page displays can be specified with the 
 *              optional argument "nbrDisplayedMenuItems". If this argument is 
 *              missing in the constructor, it takes the value 0 and the only menu page 
 *              shows all defined menuitems. The user must then ensure that nbrDisplayedMenuItems 
 *              is not larger than the number of possible lines on the display. 
 *              
 * Board        ESP32 / Touch display, e.g. CYD cheap Yellow Display
 * Library      LovyanGFX 
 * Remarks      
 * References   https://github.com/lovyan03/LovyanGFX
 */
#include "Menu.h"

void Menu::setup()
{
  if (_nbrDisplayedMenuItems == 0) _nbrDisplayedMenuItems = _nbrMenuItems;

  _menuPage = _selectedMenuItem / _nbrDisplayedMenuItems;
  _startMenuItem = _menuPage * _nbrDisplayedMenuItems;
  _stopMenuItem = _startMenuItem + _nbrDisplayedMenuItems;
  
  show();
}


// Show the menu
void Menu::show(bool clearScreen)
{
  if (clearScreen) _lcd.fillScreen(TFT_BLACK);
  _lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  _lcd.setTextFont(4);
  _lcd.setTextSize(1);
  _lcd.setCursor(0,0);
  _lcd.setRotation(1);

  for(int i = _startMenuItem; i < _stopMenuItem; i++)
  {
    if (i == _selectedMenuItem)
      _lcd.setTextColor(TFT_RED, TFT_GREEN); // highlight the selected menuitem
    else
      _lcd.setTextColor(TFT_GREEN, TFT_BLACK);
      
    _lcd.println(_menuItems[i].txt);
  }      
}

/**
 * Called when the display is touched
*/
void Menu::onTouch(uint8_t touchedItem)
{
  switch (_state)
  {
    case 0: // menu with selected menuitem is displayed
      if (_selectedMenuItem == touchedItem + _nbrDisplayedMenuItems * _menuPage)
      { // a selected menuitem is touched, do the associated action
        (_menuItems[_selectedMenuItem]).action();
        _state = 1; // the action is done, last screen of action is still displayed
                    // next touch displays menu again  
      }
      else
      { // an unselected menuitem is touched, select it
        _selectedMenuItem = touchedItem  + _nbrDisplayedMenuItems * _menuPage;
        show();
      }
    break;
    case 1: // show the menu again
      show();
      _state = 0;
    break;
  }
}

/**
 * Called on swipe
 * directions are UP, DOWN, LEFT, RIGHT
*/
void Menu::OnSwipe(uint8_t direction)
{
  switch (direction)
  {
    case UP:
      if ((_menuPage + 1) * _nbrDisplayedMenuItems < _nbrMenuItems)
      {
        _menuPage++;
        _startMenuItem += _nbrDisplayedMenuItems;
        _stopMenuItem = _startMenuItem + _nbrDisplayedMenuItems;
        if (_stopMenuItem >= _nbrMenuItems) _stopMenuItem = _nbrMenuItems;
      }
    break;
    case DOWN:
      if (_menuPage > 0)
      {
        _menuPage--;
        _startMenuItem = _menuPage * _nbrDisplayedMenuItems;
        _stopMenuItem = _startMenuItem + _nbrDisplayedMenuItems;
      }
    break;
    case LEFT:  // not handled
    case RIGHT: // not handled
    break;
  }
  show();
}
    
