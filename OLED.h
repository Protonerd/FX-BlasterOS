/*
 * OLED.h
 *
 *  Created on: 6 mars 2016
 * author:     Andras Kun (kun.andras@yahoo.de) based on LSOS by Sebastien CAPOU (neskweek@gmail.com)
 * Source :   https://github.com/Protonerd/FX-BlasterOS
 */

#if not defined OLED_H_
#define OLED_H_

#include <Arduino.h>
#include "Config_HW.h"
#include "Config_SW.h"
#include "SoundFont.h"

#if defined OLED_DISPLAY

#if defined OLED_SCOPE
  #define SSD1306_64_48
  #include <Adafruit_SSD1306_64x48.h>

  #define SCREEN_WIDTH 64 // OLED display width, in pixels
  #define SCREEN_HEIGHT 48 // OLED display height, in pixels
#endif
#if defined OLED_STD
  #define SSD1306_128_32
  #include <Adafruit_SSD1306.h>
//  #include <Adafruit_SSD1306.h>
  #define SCREEN_WIDTH 128 // OLED display width, in pixels
  #define SCREEN_HEIGHT 32 // OLED display height, in pixels
#endif
#endif

// ************* Important note when using LCD Assistant to convert bitmaps:
// Use Bye Orientation Horizontal!!!
// *************************************************************************

#if defined OLED_SCOPE
static const unsigned char PROGMEM StormTrooper[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x03,
0xFF, 0xFF, 0x80, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00,
0x00, 0x1F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x3F, 0xFF, 0xFF,
0xFC, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x3F,
0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x20, 0x00, 0x00, 0x08, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x07, 0xE0, 0x78, 0x00, 0x00, 0x78, 0x07, 0xE0,
0x1E, 0x00, 0x01, 0xF8, 0x1E, 0x78, 0x1F, 0x00, 0x01, 0xF8, 0x3F, 0xFC, 0x3E, 0x80, 0x01, 0xFC,
0xFF, 0xFF, 0x3E, 0x80, 0x01, 0xFF, 0xFF, 0xFF, 0xF6, 0x80, 0x01, 0xEF, 0xBF, 0xFD, 0xFF, 0x80,
0x00, 0xE6, 0xFF, 0xFF, 0x6F, 0x80, 0x00, 0xF7, 0xFF, 0xFF, 0xEF, 0x00, 0x01, 0xEF, 0xF8, 0x7F,
0xFF, 0x80, 0x03, 0xFF, 0xF2, 0x4F, 0xFF, 0xC0, 0x07, 0x7F, 0xF7, 0xEF, 0xFF, 0xE0, 0x0F, 0xBF,
0xDF, 0xFB, 0xFD, 0xE0, 0x0F, 0x7F, 0xBF, 0xFD, 0xF6, 0xF0, 0x0F, 0xCF, 0xFF, 0xFF, 0xEB, 0xF0,
0x0F, 0xFD, 0xFF, 0xFF, 0xB7, 0xE0, 0x07, 0xE7, 0xFF, 0xFF, 0xEF, 0xE0, 0x03, 0xDF, 0xFF, 0xFF,
0xFB, 0xC0, 0x01, 0xBF, 0xFE, 0x7F, 0xFD, 0x80, 0x01, 0x7F, 0xFC, 0x3F, 0xFE, 0x00, 0x00, 0xFF,
0xF8, 0x3F, 0xFE, 0x00, 0x00, 0xFE, 0xF8, 0x1F, 0x7E, 0x00, 0x00, 0x7C, 0xF0, 0x0F, 0x3E, 0x00,
0x00, 0x7C, 0xB0, 0x0D, 0x3E, 0x00, 0x00, 0x3C, 0xBC, 0x3C, 0x3C, 0x00, 0x00, 0x1C, 0x1F, 0xF8,
0x38, 0x00, 0x00, 0x0F, 0x7C, 0x3E, 0xF0, 0x00, 0x00, 0x01, 0xF0, 0x0F, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM PW_Logo[] = {
0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
0xC0, 0x00, 0x00, 0x0C, 0x0E, 0x70, 0x30, 0x00, 0x00, 0x30, 0x7E, 0x7E, 0x0C, 0x00, 0x00, 0x41,
0xFE, 0x7F, 0x82, 0x00, 0x00, 0x87, 0xFE, 0x7F, 0xE1, 0x00, 0x01, 0x0F, 0xFE, 0x7F, 0xF0, 0x80,
0x02, 0x3F, 0xFE, 0x7F, 0xFC, 0x40, 0x04, 0x7F, 0xFE, 0x7F, 0xFE, 0x20, 0x08, 0x7F, 0xFE, 0x7F,
0xFF, 0x10, 0x08, 0xFF, 0xFE, 0x71, 0xC7, 0x10, 0x10, 0x00, 0x0E, 0x61, 0xC7, 0x08, 0x10, 0x00,
0x06, 0x61, 0xC7, 0x08, 0x20, 0x00, 0x06, 0x61, 0xC7, 0x04, 0x27, 0xFF, 0x86, 0x71, 0xC7, 0x04,
0x40, 0x00, 0x0E, 0x71, 0xC7, 0x02, 0x40, 0x00, 0x0E, 0x71, 0xC7, 0x12, 0x48, 0x00, 0x0E, 0x71,
0xC7, 0x12, 0x88, 0xFF, 0xFE, 0x71, 0xFF, 0xF2, 0x88, 0xFF, 0xFE, 0x70, 0x00, 0x11, 0x88, 0xFF,
0xFE, 0x70, 0x00, 0x11, 0x88, 0xFF, 0xFE, 0x60, 0x00, 0x11, 0x88, 0xFF, 0xFE, 0x30, 0x7F, 0xF9,
0x8F, 0xFF, 0xFE, 0x7F, 0xFF, 0xF9, 0x8F, 0xFF, 0xFA, 0x1F, 0xFF, 0xF9, 0x8F, 0xFF, 0xFC, 0x3F,
0xFF, 0xF9, 0x8F, 0xFF, 0xF8, 0xFF, 0xFF, 0xF1, 0x8F, 0xFF, 0xFE, 0x5F, 0xFF, 0xF2, 0x4F, 0xFF,
0xFE, 0x3F, 0xFF, 0xF2, 0x47, 0xFF, 0xFC, 0x3F, 0xFF, 0xE2, 0x47, 0xFF, 0xC0, 0x00, 0x00, 0x22,
0x22, 0x00, 0x00, 0x00, 0x00, 0x44, 0x22, 0x00, 0x00, 0x00, 0x00, 0xC4, 0x11, 0x80, 0x00, 0x00,
0x0F, 0x88, 0x11, 0xC0, 0x00, 0x00, 0x7F, 0x88, 0x08, 0xF0, 0x00, 0x00, 0xFF, 0x10, 0x08, 0x7E,
0x00, 0x01, 0xFE, 0x10, 0x04, 0x3F, 0xF8, 0x03, 0xFC, 0x20, 0x02, 0x1F, 0xFC, 0x03, 0xF8, 0x40,
0x01, 0x0F, 0xFC, 0x03, 0xF0, 0x80, 0x00, 0x83, 0xFC, 0x03, 0xC1, 0x00, 0x00, 0x40, 0xFC, 0x01,
0x82, 0x00, 0x00, 0x30, 0x38, 0x00, 0x0C, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x30, 0x00, 0x00, 0x03,
0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00
};

static const unsigned char PROGMEM crosshait_style1 [] = {
0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00,
0x00, 0x08, 0x28, 0x00, 0x00, 0x00, 0x00, 0x14, 0x44, 0x00, 0x00, 0x00, 0x00, 0x24, 0x82, 0x00,
0x00, 0x00, 0x00, 0x42, 0x81, 0x00, 0x00, 0x00, 0x00, 0x82, 0x80, 0x80, 0x00, 0x00, 0x01, 0x02,
0x80, 0x40, 0x00, 0x00, 0x02, 0x01, 0x80, 0x20, 0x00, 0x00, 0x04, 0x01, 0x80, 0x30, 0x00, 0x00,
0x0C, 0x01, 0x80, 0x28, 0x00, 0x00, 0x14, 0x01, 0x80, 0x27, 0xFF, 0xFF, 0xE4, 0x01, 0xE0, 0x24,
0x00, 0x00, 0x24, 0x01, 0x98, 0x24, 0x07, 0xE0, 0x24, 0x03, 0x86, 0x24, 0x08, 0x90, 0x24, 0x1D,
0x81, 0xA4, 0x30, 0x8C, 0x25, 0xE1, 0x80, 0x64, 0x40, 0x82, 0x2E, 0x01, 0x80, 0x3C, 0x40, 0x82,
0x34, 0x01, 0x80, 0x24, 0x80, 0x81, 0x24, 0x01, 0x80, 0x24, 0x80, 0x81, 0x24, 0x01, 0x80, 0x25,
0x00, 0x80, 0xA4, 0x01, 0x80, 0x25, 0x00, 0x80, 0xA4, 0x01, 0x80, 0x25, 0xFF, 0xFF, 0xA4, 0x01,
0x80, 0x25, 0x00, 0x80, 0xA4, 0x01, 0x80, 0x25, 0x00, 0x80, 0xA4, 0x01, 0x80, 0x24, 0x80, 0x81,
0x24, 0x01, 0x80, 0x24, 0x80, 0x81, 0x24, 0x01, 0x80, 0x24, 0x40, 0x82, 0x24, 0x01, 0x80, 0x2C,
0x40, 0x82, 0x34, 0x01, 0x80, 0x74, 0x30, 0x8C, 0x2E, 0x01, 0x83, 0xA4, 0x0C, 0xB0, 0x25, 0xE1,
0x9C, 0x24, 0x03, 0xC0, 0x24, 0x1D, 0xE0, 0x24, 0x00, 0x00, 0x24, 0x03, 0x80, 0x27, 0xFF, 0xFF,
0xE4, 0x01, 0x80, 0x24, 0x00, 0x00, 0x14, 0x01, 0x80, 0x28, 0x00, 0x00, 0x0C, 0x01, 0x80, 0x30,
0x00, 0x00, 0x04, 0x01, 0x80, 0x3F, 0xFF, 0xFF, 0xFE, 0x01, 0x80, 0x40, 0x00, 0x00, 0x01, 0x01,
0x80, 0x80, 0x00, 0x00, 0x00, 0x82, 0x81, 0x01, 0xFF, 0xFF, 0x80, 0x42, 0x42, 0x01, 0x00, 0x00,
0x80, 0x22, 0x44, 0x01, 0x00, 0x00, 0x80, 0x14, 0x28, 0x01, 0x00, 0x00, 0x80, 0x0C, 0x10, 0x01,
0x00, 0x00, 0x80, 0x1C, 0x0F, 0x01, 0x00, 0x00, 0x81, 0xE0, 0x00, 0xFF, 0xFF, 0xFF, 0xFE, 0x00
};

static const unsigned char PROGMEM crosshait_style2 [] = {
0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x01, 0x80, 0x00, 0x03, 0x00, 0x00, 0x01, 0x80,
0x00, 0x01, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x01, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x01, 0x00, 0x01,
0xC1, 0x87, 0x80, 0x01, 0x00, 0x07, 0x01, 0x80, 0xE0, 0x01, 0x00, 0x0C, 0x3F, 0xFC, 0x30, 0x01,
0x00, 0x30, 0xE1, 0x8F, 0x18, 0x03, 0x00, 0x63, 0x81, 0x81, 0xC4, 0x01, 0x00, 0xC6, 0x00, 0x00,
0x62, 0x00, 0x00, 0x8C, 0x00, 0x00, 0x31, 0x00, 0x01, 0x18, 0x1F, 0xF0, 0x19, 0x80, 0x03, 0x30,
0x70, 0x1C, 0x0C, 0xC0, 0x02, 0x60, 0xC0, 0x03, 0x04, 0x40, 0x06, 0x41, 0x80, 0x01, 0x82, 0x60,
0x04, 0xC3, 0x00, 0x00, 0xC3, 0x20, 0x04, 0x82, 0x00, 0x00, 0x41, 0x20, 0x0D, 0x84, 0x00, 0x00,
0x61, 0x30, 0x09, 0x84, 0x03, 0xC0, 0x21, 0x90, 0x09, 0x0C, 0x0E, 0x60, 0x30, 0x90, 0x09, 0x08,
0x08, 0x10, 0x10, 0x90, 0x09, 0x08, 0x18, 0x18, 0x10, 0x90, 0x7F, 0xC8, 0x10, 0x18, 0x13, 0xFE,
0x7F, 0xC8, 0x10, 0x18, 0x13, 0xFE, 0x09, 0x08, 0x18, 0x10, 0x10, 0x90, 0x09, 0x08, 0x0C, 0x30,
0x30, 0x90, 0x09, 0x0C, 0x07, 0xE0, 0x21, 0x90, 0x09, 0x84, 0x01, 0x80, 0x21, 0xB0, 0x0C, 0x86,
0x00, 0x00, 0x61, 0x30, 0x04, 0x82, 0x00, 0x00, 0xC3, 0x20, 0x04, 0xC1, 0x00, 0x00, 0x82, 0x60,
0x06, 0x61, 0x80, 0x03, 0x06, 0x40, 0x02, 0x20, 0x60, 0x06, 0x04, 0xC0, 0x01, 0x30, 0x3C, 0x7C,
0x08, 0x80, 0x01, 0x98, 0x07, 0xE0, 0x19, 0x80, 0x00, 0xCC, 0x00, 0x00, 0x73, 0x00, 0x00, 0x67,
0x00, 0x00, 0xC6, 0x00, 0x80, 0x31, 0xC1, 0x83, 0x8C, 0x01, 0x80, 0x18, 0x79, 0x9E, 0x18, 0x01,
0x80, 0x0E, 0x0F, 0xF0, 0x70, 0x01, 0x80, 0x03, 0x81, 0x81, 0xC0, 0x01, 0x80, 0x00, 0xF1, 0x8F,
0x00, 0x01, 0x80, 0x00, 0x1F, 0xF8, 0x00, 0x01, 0x80, 0x00, 0x01, 0x80, 0x00, 0x01, 0x80, 0x00,
0x01, 0x80, 0x00, 0x01, 0x80, 0x00, 0x01, 0x80, 0x00, 0x03, 0xFF, 0xC0, 0x00, 0x00, 0x03, 0xFF
};

static const unsigned char PROGMEM crosshait_style3 [] = {
0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
0xC0, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x30, 0x10, 0x08, 0x32, 0x00, 0x00, 0x2C, 0x20, 0x04, 0x41,
0x00, 0x00, 0x42, 0x40, 0x02, 0x80, 0x80, 0x00, 0x81, 0x80, 0x01, 0x00, 0x40, 0x01, 0x00, 0x80,
0x02, 0x00, 0x3F, 0xFE, 0x00, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x08, 0x00, 0x00, 0x00,
0x00, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x01,
0x00, 0x02, 0x00, 0x08, 0x20, 0x02, 0x81, 0x03, 0x00, 0x04, 0x20, 0x04, 0x41, 0x04, 0xC0, 0x04,
0x40, 0x08, 0x21, 0x08, 0x20, 0x02, 0x40, 0x10, 0x11, 0x00, 0x10, 0x02, 0x40, 0x20, 0x07, 0xC0,
0x08, 0x02, 0x80, 0x40, 0x09, 0x20, 0x06, 0x02, 0x80, 0x80, 0x11, 0x10, 0x01, 0x01, 0x9F, 0x00,
0x21, 0x08, 0x00, 0xF9, 0x90, 0x00, 0x41, 0x04, 0x00, 0x09, 0x90, 0x00, 0x41, 0x04, 0x00, 0x09,
0x90, 0x07, 0xFF, 0xFF, 0xC0, 0x09, 0x90, 0x00, 0x41, 0x04, 0x00, 0x09, 0x90, 0x00, 0x41, 0x04,
0x00, 0x09, 0x90, 0x00, 0x21, 0x08, 0x00, 0x09, 0x9E, 0x00, 0x11, 0x10, 0x00, 0x7A, 0x41, 0x00,
0x09, 0x20, 0x00, 0x82, 0x40, 0x80, 0x07, 0xC0, 0x01, 0x02, 0x40, 0x40, 0x01, 0x00, 0x02, 0x02,
0x20, 0x20, 0x21, 0x08, 0x0C, 0x04, 0x20, 0x10, 0x41, 0x04, 0x18, 0x04, 0x10, 0x0C, 0x81, 0x02,
0x20, 0x08, 0x10, 0x03, 0x01, 0x01, 0x40, 0x08, 0x08, 0x01, 0x00, 0x00, 0x80, 0x10, 0x08, 0x00,
0x00, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x01, 0xFF, 0xFF, 0x00, 0x40,
0x01, 0x02, 0x00, 0x01, 0x00, 0x80, 0x01, 0x84, 0x00, 0x00, 0xC1, 0x40, 0x02, 0x48, 0x00, 0x00,
0x32, 0x20, 0x04, 0x30, 0x00, 0x00, 0x0C, 0x10, 0x08, 0x0C, 0x00, 0x00, 0x30, 0x00, 0x00, 0x03,
0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00
};
#endif // OLED_SCOPE

#if defined OLED_STD

static const unsigned char PROGMEM PW_Logo[] = {
0x00, 0x07, 0xF0, 0x00, 0x00, 0x78, 0x1E, 0x00, 0x00, 0xC7, 0x61, 0x00, 0x03, 0x3F, 0x7C, 0xC0,
0x04, 0x7F, 0x7F, 0x20, 0x0D, 0xFF, 0x7F, 0xD0, 0x13, 0xFF, 0x7F, 0xC8, 0x17, 0xFF, 0x7F, 0xE4,
0x20, 0x07, 0x46, 0x64, 0x60, 0x03, 0x46, 0x66, 0x5F, 0xF3, 0x66, 0x62, 0x40, 0x07, 0x66, 0x65,
0x50, 0x07, 0x66, 0x65, 0x97, 0xFF, 0x67, 0xFD, 0x97, 0xFF, 0x60, 0x05, 0x97, 0xFF, 0x63, 0xFD,
0x9F, 0xFF, 0x7F, 0xFD, 0x9F, 0xFF, 0x7F, 0xFD, 0x9F, 0xFC, 0xFF, 0xFD, 0xDF, 0xFF, 0x7F, 0xFD,
0x5F, 0xFE, 0x7F, 0xF9, 0x5F, 0xF0, 0x00, 0x1B, 0x48, 0x00, 0x00, 0x12, 0x26, 0x00, 0x03, 0xF4,
0x17, 0x00, 0x07, 0xE4, 0x13, 0xFC, 0x0F, 0xCC, 0x09, 0xFE, 0x0F, 0x90, 0x04, 0xFE, 0x0F, 0x30,
0x02, 0x1E, 0x04, 0x40, 0x01, 0xCC, 0x01, 0xC0, 0x00, 0x60, 0x06, 0x00, 0x00, 0x1F, 0xFC, 0x00
};

static const unsigned char PROGMEM StormTrooper[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x7F, 0xFC, 0x00,
0x00, 0x7F, 0xFF, 0x00, 0x01, 0xFF, 0xFF, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0xC0,
0x01, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xFF, 0xC0, 0x01, 0x00, 0x00, 0x80, 0x01, 0xE3, 0xC3, 0x80,
0x03, 0x83, 0xC1, 0xC0, 0x07, 0x8F, 0xF1, 0xF0, 0x07, 0xDF, 0xFD, 0xD0, 0x07, 0xFF, 0xFF, 0xF0,
0x07, 0x5F, 0xFF, 0xF0, 0x07, 0xFF, 0xFF, 0xF0, 0x0F, 0xFD, 0x5F, 0xF0, 0x1F, 0xFF, 0xFF, 0xF8,
0x1F, 0xFF, 0xF7, 0x5C, 0x1F, 0xFF, 0xFF, 0xFC, 0x1F, 0x7F, 0xFF, 0xF8, 0x0F, 0xFF, 0xFF, 0xF0,
0x07, 0xFE, 0x7F, 0xC0, 0x07, 0xFC, 0x7F, 0xC0, 0x03, 0xDC, 0x1D, 0xC0, 0x03, 0xDE, 0x75, 0xC0,
0x01, 0xC7, 0xF1, 0x80, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM AmmoCntFrame[] = {
0x00, 0x0F, 0xF0, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x60, 0x04, 0x00, 0x00, 0x50, 0x42, 0x00,
0x01, 0x90, 0x81, 0x00, 0x01, 0x1F, 0x00, 0x80, 0x06, 0x00, 0x00, 0x40, 0x04, 0x1F, 0x00, 0x20,
0x18, 0x10, 0x80, 0x10, 0x10, 0x10, 0x40, 0x08, 0x64, 0x00, 0x00, 0x04, 0x48, 0x00, 0x00, 0x02,
0x91, 0xFF, 0xFF, 0x81, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01,
0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01,
0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01,
0x80, 0x00, 0x00, 0x01, 0x81, 0xFF, 0xFF, 0x89, 0x80, 0x04, 0x20, 0x11, 0x40, 0x09, 0x90, 0x22,
0x20, 0x11, 0x88, 0x04, 0x10, 0x09, 0x90, 0x08, 0x10, 0x04, 0x20, 0x08, 0x1F, 0xFF, 0xFF, 0xF8
};

static const unsigned char PROGMEM E11_oled [] = { // 75x32
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xF0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF,
0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x26, 0x1F, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x02, 0x05, 0xB0, 0x1F, 0xFF, 0xFF, 0xC0, 0x3F, 0xFF,
0xFE, 0x3F, 0xFF, 0xFE, 0xFA, 0x00, 0x7F, 0xFF, 0xFF, 0xF8, 0x8C, 0x3F, 0xFF, 0xFE, 0x00, 0x7F,
0xFF, 0xFF, 0xFC, 0x7F, 0xFF, 0xFF, 0xFE, 0x00, 0x70, 0x00, 0x00, 0x7F, 0x7F, 0xFF, 0xFF, 0xFE,
0x00, 0x70, 0x00, 0x00, 0x7F, 0x5F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x7F, 0x37, 0xF8,
0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x68, 0x18, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7,
0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8E, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xE0, 0x00,
0x7F, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xE9, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x4C,
0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
0xF8, 0x33, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x1D, 0x20, 0x7B, 0x0D, 0x0A, 0x30, 0x78, 0x30, 0x30, 0x2C, 0x20, 0x30

};

#endif // OLED_STD
/***************************************************************************************************
* SSD_1306 based 128x64 I2C OLED monochrome display
* Adafruit_SSD1306.h reconfigured to use only 128x32 pixels, otherwise sketch runs out of RAM
* 
 */
 
void DisplayBlasterStatus(char statustext[6], bool clearScreen);
void DisplayStartScreen();
void DisplayBlasterOnFrames();
void DisplayAmmoCount(uint8_t AmmoCnt, int16_t iAngle=-1);


#endif /* OLED_H_ */
