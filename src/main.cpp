/**
 * Program      CYDmultipageMenu.cpp
 * 
 * Author       2024-05-10 Charles Geiser (www.dodeka.ch)
 * 
 * Purpose      Shows how to implement a multi-page menu for calling up various actions.
 *              To control the menu, a touch handler is implemented that can distinguish 
 *              between the events click, longclick and swipe in the four directions up, 
 *              down, left and right.
 * 
 * Board        ESP32-2432S028R ESP32 with resistive touch screen 320x240 and SD card,
 *              aka Cheap Yellow Display or CYD, with ILI9391 display driver an XPT2046
 *              touch controller
 * 
 * Wiring       No wiring is required
 *  
 * Libraries    LovyanGFX
 * 
 * Remarks      
 * 
 * Links        https://github.com/lovyan03/LovyanGFX
 *              https://github.com/rzeldent/platformio-espressif32-sunton/blob/main/esp32-2432S028R.json
 *              https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display
 *              https://github.com/BOlaerts/ESP32-2432s028
 *              https://github.com/hexeguitar/ESP32_lcd_PIO/blob/main/Pics/CYD_Audio2_0.gif (Audio modification)
*/
#include "lgfx_ESP32_2432S028.h"
#include "Menu.h"
#include "MenuActions.h"
#include "PulseGen.h"
#include "Wait.h"
#include "TouchHandler.h"

using Action = void(&)(LGFX &lcd);

LGFX lcd;
GFXfont myFont = fonts::DejaVu18;
TouchHandler touchHandler(lcd);

extern void nop(LGFX &lcd);
extern void initDisplay(LGFX &lcd, uint8_t rotation=0, GFXfont *theFont=&myFont, Action greet=nop);
extern void initRTC(const char *timeZone, bool disconnect = false);
extern void initWiFi();
extern void printConnectionDetails();
extern void printDateTime(int format);
extern void printNearbyNetworks();
extern void printSystemInfo();

extern const char *MEZ_MESZ;

const int MS_REFRESH = 1000;
const int NBR_DISPLAYED_MENUITEMS = 9;  // Number of menuitems on a page


// Portrait = 0, Landscape = 1, Portrait reversed = 2, Landscape reversed = 3
// and the corresponding mirrored orientations 4..7
enum ROT { PORTRAIT,   LANDSCAPE,   R_PORTRAIT,  R_LANDSCAPE, 
           M_PORTRAIT, M_LANDSCAPE, RM_PORTRAIT, RM_LANDSCAPE };

const bool DISCONNECT_WIFI = false;
const char *timeZone       = MEZ_MESZ; 
const int  TIME_FORMAT     = 5;  // 0..6 see function printDateTime()


MenuItem menuItems[] =
{
  {"Text Fonts",            showTextFonts},
  {"Digit Fonts",           showDigitFonts},
  {"Text Sizes",            showTextSizes},
  {"Rotated Text",          showRotatedText},
  {"Show Pi",               showPi} ,  
  {"Lines from Corner",     showCornerLines},
  {"Shrinking Rectangles",  showShrinkingRectangles},
  {"Rounded Rectangles",    showRoundedRectangles},
  {"Colored Circles",       showFilledColorCircles},
  {"Colored Triangles",     showColoredTriangles},
  {"Sierpinsky Dreieck",    showSierpinskyTriangle},
  {"Mandelbrot Set",        showMandelbrotSet},
  {"Barnsley Fern ",        showBarnsleyFern},
  {"Random Walk",           showRandomWalk},
  {"HSV Color Circle",      showHSVcircle},
  {"RGB Palettes",          showRGB565palettes},
  {"HSV Colored Screen",    showHSVcoloredScreen},
  {"Grayscale",             showGrayScale},
  {"Digital Clock",         showDigitalClock},
  {"Analog Clock ",         showAnalogClock},  
};
constexpr int nbrMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);


Menu         menu(lcd, menuItems, nbrMenuItems, NBR_DISPLAYED_MENUITEMS);
DigitalClock digitalClock = DigitalClock(MS_REFRESH);
AnalogClock  analogClock  = AnalogClock(MS_REFRESH);


/**
 * A touch was detected, calculate the touched menu item
*/
void touched(int x, int y)
{
  int menuItemTouched = y / lcd.fontHeight();
  menu.onTouch((uint8_t) menuItemTouched);
}


/**
 * Generates 3 pulse generators, each of which  
 * causes one of the RGB LEDs to flash.
 * Runs as an independent process on the second core.
*/
void blinkTask(void* arg)
{
    PulseGen pulseGenRed(RGB_LED_R, 3000000, 100000, 0);
    PulseGen pulseGenGreen(RGB_LED_G, 3000000, 100000, 3000000/3);
    PulseGen pulseGenBlue(RGB_LED_B, 3000000, 100000, 2*3000000/3);
        // Enable the pulse generators
    pulseGenRed.on();
    pulseGenGreen.on();
    pulseGenBlue.on();
    while (true)
    {
    // Let the LEDs blink
    pulseGenRed.loop();
    pulseGenGreen.loop();
    pulseGenBlue.loop();
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}


void onShortClick(int x, int y)
{
  //log_i("Short Click x = %3d  y = %3d", x, y);
  int menuItemTouched = y / lcd.fontHeight();
  menu.onTouch((uint8_t) menuItemTouched);
}


void onLongClick(int x, int y)
{
  log_i("Long Click x = %3d  y = %3d", x, y);
}


void onSwipeLeft(int x, int y)
{
  log_i("Swipe <--- x = %3d  y = %3d", x, y);
}


void onSwipeRight(int x, int y)
{
  log_i("Swipe ---> x = %3d  y = %3d", x, y);
}


void onSwipeUp(int x, int y)
{
  //log_i("Swipe ^^^^ x = %3d  y = %3d", x, y);
  menu.OnSwipe(UP);
}


void onSwipeDown(int x, int y)
{
  //log_i("Swipe vvvv x = %3d  y = %3d", x, y);
  menu.OnSwipe(DOWN);
}


void setup() 
{
  Serial.begin(115200);

  initWiFi();
  initRTC(timeZone);
  initDisplay(lcd, LANDSCAPE);

  menu.setup();
  printDateTime(TIME_FORMAT);
  printSystemInfo();
  printConnectionDetails();
  printNearbyNetworks();

  // Starts the blinking task, which causes the RGB LED to flash 
  // red, green and blue alternately every second
  xTaskCreate(blinkTask, "blinkTask", 1024, NULL, 10, NULL);

  // Add the callbacks
  touchHandler.addShortClickCb(onShortClick);
  touchHandler.addLongClickCb(onLongClick);
  touchHandler.addSwipeLeftCb(onSwipeLeft);
  touchHandler.addSwipeRightCb(onSwipeRight);
  touchHandler.addSwipeUpCb(onSwipeUp);
  touchHandler.addSwipeDownCb(onSwipeDown);
}


void loop() 
{ 
  touchHandler.loop();
}