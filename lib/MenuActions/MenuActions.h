/**
 * MenuActions.h
 * 
 * Includes and extern-declarations needed for the test functions
 */ 
#pragma once
#include <Arduino.h>
#include "DigitalClock.h"
#include "AnalogClock.h"

uint16_t HSVtoRGB( uint8_t &R, uint8_t &G, uint8_t &B, float h, float s, float v );
uint16_t rgbToColor565(float r, float g, float b, uint8_t &R, uint8_t &G, uint8_t &B);
void showRGB565palettes();
void showTextFonts();
void showDigitFonts();
void showTextSizes();
void showRotatedText();
void showPi();
void showShrinkingRectangles();
void showCornerLines();
void showRoundedRectangles();
void showFilledColorCircles();
void showColoredTriangles();
void showSierpinskyTriangle();
void showMandelbrotSet();
void showBarnsleyFern();
void showRandomWalk();
void showTFTcolors();
void showHSVcircle();
void showHSVcoloredScreen();
void showGrayScale();
void showDigitalClock();
void showAnalogClock();














