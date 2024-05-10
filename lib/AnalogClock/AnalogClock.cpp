/**
 * Class        AnalogClock.cpp
 * Author       2020-04-13 Charles Geiser
 * 
 * Purpose      Implements a class AnalogClock which displays an analog clock face
 *              on a TFT display. The method loop() periodically hands back the 
 *              control to the main program and displays the dial only every
 *              msRefresh seconds. 
 *              The time is fetched from the ESP32 module's internal RTC by calling 
 *              the getlocaltime function.  
 *             
 * Board        ESP32 / TFT 128 x 160 with SPI ST7735 driver 
 * Remarks      The refresh rate in ms is passed to the class contructor
 * References     
 */
#include "AnalogClock.h"


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
void AnalogClock::setCompileTime(int sec_uploadCompensation)
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

bool AnalogClock::isRunning()
{
  return _isRunning;
}

void AnalogClock::stop()
{
  //Serial.println("clock stop");
  _isRunning = false;
}

void AnalogClock::setup()
{
  uint16_t radius = lcd.height() > lcd.width() ? lcd.width()/2 : lcd.height()/2;
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  lcd.setFont(&fonts::Orbitron_Light_24);
  lcd.setTextSize(0.75);
  drawDial(lcd.width()/2, lcd.height()/2, radius - 10);
  _isRunning = true;
}

void AnalogClock::loop()
{
  if (_waitAnalogClock->isOver())
  {
    updateDial();
  }
}

/**
 * Refreshs the dial
 * Called every msRefresh seconds from loop
 */
void AnalogClock::updateDial()
{
  char theDate[12];
  getLocalTime(&_rtcTime);
  _hh = _rtcTime.tm_hour;
  _mm = _rtcTime.tm_min;
  _ss = _rtcTime.tm_sec;
  lcd.fillCircle(_mx, _my, _rTh, TFT_BLACK);          // Erase old hands 
  
  strftime(theDate, 20, "%F", &_rtcTime);             // Date as yyyy-mo-dd
  lcd.drawString(theDate, (lcd.width()-lcd.textWidth(theDate))/2, 3*_r/2); // place date string
  drawHands(_hh, _mm, _ss, _mx, _my, _rH, _rM, _rS);  // Draw new hands
}


void AnalogClock::drawTickMarks(int mx, int my, int r1, int r2, int nbr, int color)
{
  int x1, y1, x2, y2;
  float phi, sinPhi, cosPhi; 
  float phi0 = 2 * PI / nbr;
  
  for (int i = 0; i < nbr; i++)
  {
    phi = phi0 * i;
    sinPhi = sin(phi);
    cosPhi = cos(phi);
    x1 = mx + r1 * cosPhi;
    y1 = my + r1 * sinPhi;
    x2 = mx + r2 * cosPhi;
    y2 = my + r2 * sinPhi;
    lcd.drawLine(x1, y1, x2, y2, color);
  }
}

void AnalogClock::drawHands(int hh, int mm, int ss, int mx, int my, int rH, int rM, int rS)
{
  float wSec, wMin, wHour;
  int xH, yH, xM, yM, xS, yS; 
  int x1H, y1H, x1M, y1M, x1S, y1S;

  wSec  = PI/30.0 * ss;                        // Sekundenwinkel = 6° * ss                                 
  wMin  = PI/30.0 * (mm + ss/60.0);            // Minutenwinkel  = 6° * mm + 6°/60 * ss
  wHour = PI/6.0 * (hh + mm/60.0 + ss/3600.0); // Stundenwinkel  = 30° * hh + 30°/60 * mm + 30°/3600 * ss

  wSec -= PI/2;	// subtract PI/2 to rotate to the 1st quadrant
  wMin -= PI/2;
  wHour -= PI/2;

  xH = rH * cos(wHour) + mx; x1H = mx -rH/4 * cos(wHour);
  yH = rH * sin(wHour) + my; y1H = my -rH/4 * sin(wHour);
  xM = rM * cos(wMin) + mx;  x1M = mx -rM/4 * cos(wMin);
  yM = rM * sin(wMin) + my;  y1M = my -rM/4 * sin(wMin);
  xS = rS * cos(wSec) + mx;  x1S = mx -rS/4 * cos(wSec);
  yS = rS * sin(wSec) + my;  y1S = my -rS/4 * sin(wSec);

  lcd.drawLine(x1H, y1H, xH, yH, TFT_WHITE);
  lcd.drawLine(x1M, y1M, xM, yM, TFT_WHITE);
  lcd.drawLine(x1S, y1S, xS, yS, TFT_RED);

  lcd.fillCircle(mx, my, 4, TFT_RED);  // Draw red center disk      
}

void AnalogClock::drawDial(int mx, int my, int radius)
{
  _mx = mx;       // dial center x
  _my = my;       // dial center y
  _r = radius;    // outer radius of dial
  _rH = _r-38;    // radius of hour hand
  _rM = _r-18;    // radius of minute hand
  _rS = _r-15;    // radius of second hand
  _rTh = _r-13;   // radius of hour tickmarks
  _rTm = _r-7;    // radius of minute tickmarks

  lcd.drawRect(0,0,lcd.width(),lcd.height(), TFT_GREEN); // Draw green border
  lcd.drawArc(_mx, _my, _r, _rTm, 0.0, 360.0, TFT_BLUE); // Draw blue ring  
    
  drawTickMarks(_mx, _my, _r, _rTm,  60, TFT_BLUE);  // Draw 60 short minute tick marks
  drawTickMarks(_mx, _my, _r, _rTh,  12, TFT_BLUE);  // Draw 12 longer hour tickmarks
}
