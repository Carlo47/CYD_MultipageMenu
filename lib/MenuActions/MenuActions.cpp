#include "MenuActions.h"

extern LGFX tft;
extern DigitalClock digitalClock;
extern AnalogClock  analogClock;


int TFTcolor[] = {  TFT_BLACK,
                    TFT_NAVY,
                    TFT_DARKGREEN,
                    TFT_DARKCYAN,
                    TFT_MAROON,
                    TFT_PURPLE,
                    TFT_OLIVE,
                    TFT_LIGHTGREY,
                    TFT_DARKGREY,
                    TFT_BLUE,
                    TFT_GREEN,
                    TFT_CYAN,
                    TFT_RED,
                    TFT_MAGENTA,
                    TFT_YELLOW,
                    TFT_WHITE,
                    TFT_ORANGE,
                    TFT_GREENYELLOW,
                    TFT_PINK,
                    TFT_BROWN,
                    TFT_GOLD,
                    TFT_SILVER,
                    TFT_SKYBLUE,
                    TFT_VIOLET
                  };

constexpr int nbrTFTcolors = sizeof(TFTcolor) / sizeof(TFTcolor[0]);


/**
 * Convert HSV to RGB color space and return a
 * 16 bit color value as rrrrrggggggbbbbb
 * HSV: 0<= h <360,   0<= s,v <=1 
 * RGB: 0 <= R, G, B =< 255
*/
uint16_t HSVtoRGB( uint8_t &R, uint8_t &G, uint8_t &B, float h, float s, float v )
{
  int i;
  float r, g, b, f, p, q, t;

  if( s == 0 ) 
  {
    // achromatic (grey)
    R = G = B = 255 * v;
    return lcd.color565(R, G, B);
  }

  h /= 60;      // sector 0 to 5
  i = floor( h );
  f = h - i;      // factorial part of h
  p = v * ( 1 - s );
  q = v * ( 1 - s * f );
  t = v * ( 1 - s * ( 1 - f ) );

  switch( i ) 
  {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default:    // case 5:
      r = v;
      g = p;
      b = q;
      break;
  }
  R = 255 * r;
  G = 255 * g;
  B = 255 * b;
  return lcd.color565(R, G, B);
}

/**
 * Converts 0.0 <= r,g,b <=1.0 float values into a
 * 16-bit rgb565: rrrrrggggggbbbbb unit16_t and stores
 * the 0 <= R,G,B <= 255 into the provided addresses.
*/
uint16_t rgbToColor565(float r, float g, float b, uint8_t &R, uint8_t &G, uint8_t &B)
{
  uint16_t __R;
  uint16_t __G;
  uint16_t __B;
  R = r * 32; __R = (R & 0b11111000) << 8;
  G = g * 64; __G = (G & 0b11111100) << 3;
  B = b * 32; __B = B >> 3;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return __R | __G | __B;
}

void showRGB565palettes()
{
  int x = 0;
  int y = 0;
  int side = lcd.height() / 32;
  char s[16];
  GFXfont font = fonts::DejaVu12;
  
  // Create all 64 color palettes with varying G value between
  // 0 and 64 (6 bit)
  for (uint8_t G = 0; G < 64; G++)
  {
    x = 0;
    y = 0;
    lcd.fillScreen(TFT_BLACK);
    sprintf(s, "G = %d", G);  // show G value
    lcd.drawString(s, 240, 36, &font);

    // Creates a color palette consisting of 32 x 32 square color fields. 
    // Each color field is colored with a combination of R and B with 
    // values between 0 and 32 (5 bit) at a constant G value.
    for (uint8_t R = 0; R < 32; R++)  // vary R in y direction
    {
      sprintf(s, "R = %d", R);
      lcd.drawString(s, 240, 20, &font);  // show R value

      for (uint8_t B = 0; B < 32; B++)  // vary B in x direction
      {
        uint16_t rgb565 = ((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3);
        lcd.fillRect(x, y, side, side, rgb565);
        sprintf(s, "B = %d", B);
        lcd.drawString(s, 240, 52, &font);  // show B value
        x += side;
      }
      x = 0;
      y += side;
    }
    delay(1000);
    if (lcd.getTouch(&x, &y)) break;
  }
}


void showTextFonts()
{
  int y = 0;
  const lgfx::v1::IFont  *savedFont = lcd.getFont();
  lcd.fillScreen(TFT_BLACK);
  lcd.setCursor(0,y);
  GFXfont font = fonts::DejaVu12;
  lcd.setFont(&font);
  lcd.println("DejaVu12");

  y += 20; lcd.setCursor(0,y);
  font = fonts::FreeMono12pt7b;
  lcd.setFont(&font);
  lcd.println("FreeMono12pt7b");

  y += 20; lcd.setCursor(0,y);
  font = fonts::FreeMonoBold12pt7b;
  lcd.setFont(&font);
  lcd.println("FreeMonoBold12pt7b");

  y += 20; lcd.setCursor(0,y);
  font = fonts::FreeMonoOblique12pt7b;
  lcd.setFont(&font);
  lcd.println("FreeMonoOblique12pt7b");

  y += 20; lcd.setCursor(0,y);
  font = fonts::FreeMonoBoldOblique12pt7b;
  lcd.setFont(&font);
  lcd.println("FreeMonoBoldObl12pt7b");

  y += 20; lcd.setCursor(0,y);
  font = fonts::FreeSans12pt7b;
  lcd.setFont(&font);
  lcd.println("FreeSans12pt7b");

  y += 20; lcd.setCursor(0,y);
  font = fonts::FreeSansBold12pt7b;
  lcd.setFont(&font);
  lcd.println("FreeSansBold12pt7b");

  y += 20; lcd.setCursor(0,y);
  font = fonts::FreeSansOblique12pt7b;
  lcd.setFont(&font);
  lcd.println("FreeSansOblique12pt7b");

   y += 20; lcd.setCursor(0,y);
  font = fonts::FreeSansBoldOblique12pt7b;
  lcd.setFont(&font);
  lcd.println("FreeSansBoldObl12pt7b");

   y += 20; lcd.setCursor(0,y);
  font = fonts::FreeSerif12pt7b;
  lcd.setFont(&font);
  lcd.println("FreeSerif12pt7b");

   y += 20; lcd.setCursor(0,y);
  font = fonts::FreeSerifItalic12pt7b;
  lcd.setFont(&font);
  lcd.println("FreeSerifItalic12pt7b");

  
  y += 24; lcd.setCursor(0,y);
  font = fonts::Orbitron_Light_24;
  lcd.setFont(&font);
  lcd.println("Orbitron_Light_24"); 

/*   y += 20; lcd.setCursor(0,y);
  font = fonts::Yellowtail_32;
  lcd.setFont(&font);
  lcd.println("Yellowtail_32");

  y += 20; lcd.setCursor(0,y);
  font = fonts::Satisfy_24;
  lcd.setFont(&font);
  lcd.println("Satisfy_24");  
*/

  lcd.setFont(savedFont);
}

void showDigitFonts()
{
  const lgfx::v1::IFont  *savedFont = lcd.getFont();
  lcd.fillScreen(TFT_BLACK);
  lcd.setCursor(0, 200);
  lcd.print("Fonts 6, 7, 8\n");

  lgfx::v1::RLEfont font6 = fonts::Font6;
  lcd.setCursor(0, 0, &font6);
  lcd.setTextColor(TFT_RED);
  lcd.println ("0123456789.:");

  lgfx::v1::RLEfont font7 = fonts::Font7;
  lcd.setCursor(0, 50, &font7);
  lcd.setTextColor(TFT_GREEN);
  lcd.println ("0123456789");

  lgfx::v1::RLEfont font8 = fonts::Font8;
  lcd.setCursor(0, 100, &font8);
  lcd.setTextColor(TFT_BLUE);
  lcd.println ("01234:");

  lcd.setFont(savedFont);
}

// Shows the font sizes for the font
// DejaVu 9, 12, 18, 24,40, 56, 72
void showTextSizes()
{
  int y = 0;
  const lgfx::v1::IFont  *savedFont = lcd.getFont();
  lcd.fillScreen(TFT_BLACK);
  lcd.setCursor(0,y);
  GFXfont font = fonts::DejaVu9;
  lcd.setFont(&font);
  lcd.println("DejaVu9");

  y += 15; lcd.setCursor(0,y);
  font = fonts::DejaVu12;
  lcd.setFont(&font);
  lcd.println("DejaVu12");

  y += 20; lcd.setCursor(0,y);
  font = fonts::DejaVu18;
  lcd.setFont(&font);
  lcd.println("DejaVu18");

  y += 25; lcd.setCursor(0,y);
  font = fonts::DejaVu24;
  lcd.setFont(&font);
  lcd.println("DejaVu24");

  y += 35; lcd.setCursor(0,y);
  font = fonts::DejaVu40;
  lcd.setFont(&font);
  lcd.println("DejaVu40");

  y += 50; lcd.setCursor(0,y);
  font = fonts::DejaVu56;
  lcd.setFont(&font);
  lcd.println("DejaVu56");

  y += 65; lcd.setCursor(0,y);
  font = fonts::DejaVu72;
  lcd.setFont(&font);
  lcd.println("D..Vu72");

  lcd.setFont(savedFont);    
}

// Shows text in the 4 directions 0..3
void showRotatedText()
{
  uint8_t savedRot = lcd.getRotation();
  uint8_t savedSize = lcd.getTextSizeY();
  for (int i = savedRot; i < savedRot + 5; i++)
  {
    lcd.fillScreen(TFT_BLACK);
    lcd.setRotation(i % 4); 
    lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    lcd.setTextSize(1);
    int pos_x = (lcd.width() - lcd.textWidth("Good By! Good By!")) / 2;  // Den ersten Text zentrieren
    lcd.setCursor(pos_x,0);
    lcd.println("Good By! Good By!");   // println() setzt den Curser auf 
                                          // den Anfang der nächsten Zeile
    
    lcd.setTextColor(TFT_RED, TFT_BLACK);
    lcd.setTextSize(2);
    lcd.println("Good By!");
  
    lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    lcd.setTextSize(3);
    lcd.println("Good By");
    delay(500);
  }
  lcd.setTextSize(savedSize);
  lcd.setRotation(savedRot);
}

void showPi()
{
  GFXfont font = fonts::FreeSerif12pt7b;
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextColor(TFT_SKYBLUE);
  lcd.setCursor(10, 100, &font);
  lcd.printf("Pi < 355 / 113 = %1.8f", 355.0 / 113.0);
}

// Zeichnet kleiner werdende Rechtecke
void showShrinkingRectangles()
{
  int w = lcd.width();
  int h = lcd.height();
  
  lcd.fillScreen(TFT_BLACK);
  for (int i = 0; i < h/2; i += 5)
  {
    lcd.drawRect(i, i, w-2*i, h-2*i, TFTcolor[random(0,nbrTFTcolors)]);
    delay(100);  
  }
}

// Zeichnet Linien ausgehend von allen 4 Ecken
void showCornerLines()
{
  int w = lcd.width();
  int h = lcd.height();
  
  lcd.fillScreen(TFT_BLACK);
  for(int i = 0; i < w; i +=7 )
  {
    lcd.drawLine(0, 0, w, i, TFT_GREEN);
    delay(50);
    lcd.drawLine(w, 0, w-i, h, TFT_BLUE);
    delay(50);
    lcd.drawLine(w, h, 0, h-i, TFT_RED);
    delay(50);
    lcd.drawLine(0, h, i, 0, TFT_YELLOW);
    delay(50); 
  }
}

// Zeichnet abgerundete Rechtecke mit unterschiedlichen Radien
void showRoundedRectangles() 
{
  lcd.fillScreen(TFT_BLACK);
  int w = lcd.width();
  int h = lcd.height();
  int rectH =40;
  int rectR = 15;

  for(int i = 0 ; i < h/2; i+=7) 
  {
    rectR = i+1;
    rectH = 2 * rectR;
    lcd.drawRoundRect(0, (h-rectH)/2, w, rectH, rectR, TFT_RED);
    lcd.drawRoundRect((w-rectH)/2, 0, rectH, h, rectR, TFT_VIOLET);
    delay(100);
  }
}


// Zeichnet gefüllte Farbkreise
void showFilledColorCircles()
{
  int w = lcd.width();
  int h = lcd.height();

  lcd.fillScreen(TFT_BLACK);
  for(int i = 0; i < h/2; i += 3)
  {
    lcd.fillCircle(w/2, h/2, h/2 - 2*i, TFTcolor[random(0,nbrTFTcolors)]); 
    delay(100);
  }
}

// Zeichnet blaue und rote Dreiecke
void showColoredTriangles()
{
  int w = lcd.width()-1;
  int h = lcd.height()-1;
  lcd.fillScreen(TFT_BLACK);
  for(int i = 0; i < w/2; i += 5)
  {
    lcd.drawTriangle(w/2, 0, 0, h/2, i, i*h/w, TFT_RED); 
    delay(30);
    lcd.drawTriangle(0, h/2, w/2, h, i, h - i*h/w, TFT_BLUE); 
    delay(30);
    lcd.drawTriangle(w/2, 0, w, h/2, w-i, i*h/w, TFT_BLUE);  
    delay(30);
    lcd.drawTriangle(w, h/2, w/2, h, w-i, h - i*h/w, TFT_RED); 
    delay(30);
    
  }
}

// Fraktales Sierpinsky-Dreieck
// 1) Wähle einen beliebigen Anfangspunkt, hier P(20,20)
// 2) Wähle zufällig eine Ecke, markiere die Mitte der Strecke P-Ecke
// 3) Die Mitte wird zum neuen Punkt P
// 4) Wiederhole ab 3)
void showSierpinskyTriangle()
{
  //int ecke[3][2] = {{0,0}, {159,0}, {79,127}};
  int ecke[3][2] = {{0,0}, {lcd.width(), 0}, {lcd.width()/2, lcd.height()}};
  int farbe[] = {TFT_RED, TFT_GREEN, TFT_BLUE};
  int p[] = {20,20};
  int k, x, y, mx, my;
  
  lcd.fillScreen(TFT_BLACK);
  for(int i = 0; i < 20000; i++)
  {
    k = random(0,3);
    x = ecke[k][0];
    y = ecke[k][1];
    mx = (x-p[0])/2 + p[0]; 
    my = (y-p[1])/2 + p[1];
    lcd.drawPixel(mx, my, farbe[k]);
    p[0] = mx;
    p[1] = my;
    delayMicroseconds(1000);
    if (lcd.getTouch(&x, &y)) break;
  }
  lcd.drawRect(0, 0, lcd.width(), lcd.height(), TFT_YELLOW);
}

// Mandelbrot-Apfelmännchen darstellen
void showMandelbrotSet()
{
  uint8_t savedRot = lcd.getRotation();
  lcd.setRotation(1);
  int maxIteration = 1000;
  int w = lcd.width();
  int h = lcd.height();
  unsigned int farbe;
  lcd.fillScreen(TFT_WHITE);

  for(int zeile = 0; zeile < h; zeile++)
  {
    for(int spalte = 0; spalte < w; spalte++)
    {
      float c_re = (spalte - w/2.0) * 4.0 / w;
      float c_im = (zeile- h/2.0) * 4.0 / h;
      float x = 0;
      float y = 0;
      int iteration = 0;
      while (x*x + y*y <=4 && iteration < maxIteration)
      {
        float x_neu = x*x - y*y + c_re;
        y = 2*x*y + c_im;
        x = x_neu;
        iteration++;
      }
      if (iteration < maxIteration)
      {
        if (iteration < nbrTFTcolors) farbe = TFTcolor[iteration];
        else farbe = TFT_WHITE;
        lcd.drawPixel(spalte, zeile, farbe);
      }
      else
          lcd.drawPixel(spalte, zeile, TFT_BLACK);
    }
  }
  lcd.drawRect(0, 0, lcd.width(), lcd.height(), TFT_BLUE);
  lcd.setRotation(savedRot);
}

/**
 * Draws a self-similar fractal known as Barnsleys Fern
 * https://en.wikipedia.org/wiki/Barnsley_fern
*/
void showBarnsleyFern() 
{
  float x = 0;
  float y = 0;
  uint8_t savedRot = lcd.getRotation();
  lcd.setRotation(2);

  lcd.fillScreen(TFT_BLACK);

  for (unsigned int i = 0; i < 32000; i++) 
  {
    float xt = 0;
    float yt = 0;
 
    int r = random(0, 100);
 
    if (r <= 1) 
    {
      xt = 0;
      yt = 0.16*y;
    } else if (r <= 8) 
    {
      xt = 0.20*x - 0.26*y;
      yt = 0.23*x + 0.22*y + 1.60;
    } else if (r <= 15) 
    {
      xt = -0.15*x + 0.28*y;
      yt =  0.26*x + 0.24*y + 0.44;
    } else 
    {
      xt =  0.85*x + 0.04*y;
      yt = -0.04*x + 0.85*y + 1.60;
    }
 
    x = xt;
    y = yt;
 
    int m = round(lcd.width()/2 + 30*x);
    int n = lcd.height()-round(30*y);
 
    lcd.drawPixel(m, n, TFT_GREEN);
  }
  lcd.drawRect(0, 0, lcd.width(), lcd.height(), TFT_GREEN);
  lcd.setRotation(savedRot);
}

/**
 * Randomly move from the center of the display to
 * one of the 8 neighboring pixels or rest in place.
*/
void showRandomWalk()
{
  int x = lcd.width()/2;
  int y = lcd.height()/2;
  int color = TFTcolor[random(0,24)];

  lcd.fillScreen(TFT_BLACK);
  
  for(int i = 0; i < 32000; i++)
  {
    x += random(0, 3) - 1;  // 0, 1, 2 ==> -1, 0, 1
    y += random(0, 3) - 1;
    if (x >= 0 && x < lcd.width() && y >= 0 && y < lcd.height())
      { 
        if (i % 500 == 0) color = TFTcolor[random(0,24)]; // change the color after every 500 steps
        lcd.drawPixel(x, y, color); 
      }
    delayMicroseconds(200); // slow down the walk
    if (lcd.getTouch(&x, &y)) break;
  }
  lcd.drawRect(0, 0, lcd.width(), lcd.height(), TFT_GREEN);
}


/**
 * show TFT colors
*/
void showTFTcolors()
{
  int w = lcd.width() / nbrTFTcolors;
  int h = lcd.height();
  int x = 0;
  lcd.fillScreen(TFT_BLACK);

  for (int i = 0; i < nbrTFTcolors; i++)
  {
    lcd.fillRect(x, 0, w, h, TFTcolor[i]);
    x += w;
  }
}

// Display HSV color circle as a polygon with 360°/alpha sides.
// The color value (H = Hue) changes from 0 ... 360° in steps of alpha°.
// Saturation S and brightness (V = Value) are set to 1.0, the maximum.
void showHSVcircle()
{
  constexpr float DEGTORAD = PI/180;
  int xm = lcd.width() / 2;
  int ym = lcd.height() /2;
  int xA, yA, xB, yB;
  int alpha = 3; // Change to vary the number of sides of the color polygon
  uint16_t radius = lcd.height() > lcd.width() ? lcd.width()/2 - 10 : lcd.height()/2 -10;
  uint8_t R, G, B;
  uint16_t color16;
  
  lcd.fillScreen(TFT_BLACK);
  for( int phi = 0; phi < 360; phi += alpha)
  {
    xA = xm + radius * sin(DEGTORAD * phi);
    yA = ym + radius * cos(DEGTORAD * phi);
    xB = xm + radius * sin(DEGTORAD * (phi+alpha));
    yB = ym + radius * cos(DEGTORAD * (phi+alpha)); 

    color16 = HSVtoRGB(R, G, B, phi, 0.9, 0.9);
    lcd.fillTriangle(xm, ym, xA, yA, xB, yB, color16); 
  }
  lcd.drawRect(0, 0, lcd.width(), lcd.height(), TFT_BLUE);
}

void showHSVcoloredScreen()
{
  uint8_t R, G, B; 
  uint16_t color16;
  lcd.fillScreen(TFT_BLACK);
  delay(500);

  for (int phi = 0; phi < lcd.width(); phi++) 
  {
    color16 = HSVtoRGB(R, G, B, phi, 0.9, 0.9);
    lcd.drawFastVLine(phi, 0, lcd.height(), color16);
  }
}

void showGrayScale()
{
  uint16_t g, color16;
  lcd.fillScreen(TFT_BLACK);
  for (int i = 0; i < lcd.width(); i++)
  {
    g = i * 255 / lcd.width();
    color16 = lcd.color565(g, g, g);
    lcd.drawFastVLine(i, 0, lcd.height(), color16);
  }
}

// Zeigt die Zeit digital an
void showDigitalClock()
{
  uint8_t savedRot = lcd.getRotation();
  uint8_t savedSize = lcd.getTextSizeY();
  const lgfx::v1::IFont  *savedFont = lcd.getFont();

  digitalClock.setup();
  //digitalClock.setCompileTime(12);
  while (digitalClock.isRunning())
  {
    int x, y;
    if (lcd.getTouch(&x, &y)) digitalClock.stop();
    digitalClock.loop();;
  }

  lcd.setRotation(savedRot);
  lcd.setTextSize(savedSize);
  lcd.setFont(savedFont);
}

// Zeigt die Zeit analog an
void showAnalogClock()
{
  uint8_t savedRot  = lcd.getRotation();
  uint8_t savedSize = lcd.getTextSizeY();
  const lgfx::v1::IFont  *savedFont = lcd.getFont();

  analogClock.setup();
  //analogClock.setCompileTime(12);
  while (analogClock.isRunning())
  {
    int x, y;
    if (lcd.getTouch(&x, &y)) analogClock.stop();
    analogClock.loop();
  }

  lcd.setRotation(savedRot);
  lcd.setTextSize(savedSize);
  lcd.setFont(savedFont);
}