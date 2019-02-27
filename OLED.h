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
    #define SCREEN_WIDTH 64 // OLED display width, in pixels
    #define SCREEN_HEIGHT 48 // OLED display height, in pixels
    #include <Adafruit_SSD1306_64x48.h>
#endif
#if defined OLED_STD
    #define SCREEN_WIDTH 128 // OLED display width, in pixels
    #define SCREEN_HEIGHT 32 // OLED display height, in pixels
    #include <Adafruit_SSD1306.h>
#endif
#endif

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
