/**
 * Class        DigitalClock.cpp
 * Author       2020-04-13 Charles Geiser
 * 
 * Purpose      Implements a class DigitalClock which displays time and date on a TFT display.
 *              The method loop() periodically hands back the control to the main
 *              program and updates the display only every msRefresh seconds. 
 *              The time is fetched from the ESP32 module's internal RTC by calling 
 *              the getlocaltime function.  
 *             
 * Board        ESP32 / TFT 128 x 160 with SPI ST7735 driver 
 *              ESP32-2432S028R /TFT 240 x 320 SPI ILI9341 driver
 * Remarks      The refresh rate in ms is passed to the class contructor
 * References     
 */
#include "DigitalClock.h"

/**
 * Sets the RTC of the ESP32 to the compile time and date. The delay  
 * due to the upload time can be compensated by specifying a value 
 * for sec_uploadCompensation (approx. 15 sec).
 * 
 * Example of compile time and date strings:
 * __TIME__   19:32:39
 * __DATE__   Apr 17 2020
 * 
 */
void DigitalClock::setCompileTime(int sec_uploadCompensation)
{
  struct tm time;
  char str_month[5];
  int y, mday;
  timeval sec_musec;
  static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  sscanf(__DATE__, "%s%d%d", str_month, &mday, &y);
  sscanf(__TIME__, "%2d%*c%2d%*c%2d", &time.tm_hour, &time.tm_min, &time.tm_sec);
  //Serial.println(__DATE__);
  //Serial.println(__TIME__);
  time.tm_mon = (strstr(month_names, str_month) - month_names) / 3;  // Calculate the month number
  time.tm_year = y - 1900;
  time.tm_mday = mday;

  sec_musec.tv_sec = mktime(&time) + sec_uploadCompensation;
  sec_musec.tv_usec= 0;
  settimeofday(&sec_musec, NULL); // Set the internal RTC of the ESP32
}

bool DigitalClock::isRunning()
{
    return _isRunning;
}

void DigitalClock::stop()
{
    _isRunning = false;
}

void DigitalClock::setup()
{
    lcd.fillScreen(TFT_MAROON);
    lcd.setRotation(1);
    lcd.setTextSize(1);
    lcd.setTextFont(7);
    lcd.setTextColor(TFT_GREEN, TFT_MAROON);
    _isRunning = true;
}

void DigitalClock::loop()
{
    if (_waitDigitalClock->isOver())  // Get new time every sec
    {
        getLocalTime(&_rtcTime);

        strftime(_s, 20, "%T", &_rtcTime); // Time as hh:mm:ss
        lcd.drawString(_s,(lcd.width() - lcd.textWidth(_s))/2, lcd.height()/2 - lcd.fontHeight());
        strftime(_s, 20, "%F", &_rtcTime); // Date as yyyy-mo-dd
        lcd.drawString(_s,(lcd.width() - lcd.textWidth(_s))/2, lcd.height()/2 + lcd.fontHeight());
    }
}
