/*
 * Config_SW.h
 *
 * Created on: 30 March 2017
 * author:    Andras Kun (kun.andras@yahoo.de)
 * Source :   https://github.com/neskweek/LightSaberOS
 * This tab contains all settings necessary to define your saber board's electronic software configuration
 */

#include "Config_HW.h"

#if not defined CONFIG_SW_H_
#define CONFIG_SW_H_


/*
 * DO NOT MODIFY
 * Unless you know what you're doing
 *************************************/
#define MEMORYBASE      32
#define VOL         15
#define SOUNDFONT       1
/************************************/

/***********************************/
// DEFINE CHARACTER SET TO USE
/***********************************/
#ifdef OLED_DISPLAY
// if you want to have the iconic Star Wars alphabet, uncomment AUREBESH
// if line remains commented, code will revert back to default ASCII characters
  //#define AUREBESH
#define CUSTOMTEXT "TK-83607"
#define INTROTEXT "FX-BlasterOS"
#endif

/************************************/

// MAX_AMMO should be a multile of the statusbargraph length for smooth count display
#define MAX_AMMO 20

/* MAX_BRIGHTNESS
 *
 * Maximum output voltage to apply to LEDS
 * Default = 100 (39,2%) Max=255 Min=0(Off)
 *
 * WARNING ! A too high value may burn
 * your leds. Please make your maths !
 * BE VERY CAREFULL WITH THIS ONE OR 
 * YOU'LL BURN YOUR BLADE'S LED 
 ************************************/
#define MAX_BRIGHTNESS    230
#define HUM_RELAUNCH     5000
#define PI 3.14159265

/* LIGHT_EFFECTS
 *
 * Enable / disable  all light and
 * color effects of the blade
 * If you a device with a CPU wich is not
 * an Atmega328 : COMMENT THIS
 ************************************/


/* DEEP_SLEEP
 * If you want to enable/disable
 * deep sleep capabalities
 * Starting from v1.6, deep sleep can be entered from the config menu after the volume setting
 * If you a device with a CPU wich is not
 * an Atmega328 : COMMENT THIS
 *************************************/
#define DEEP_SLEEP
#if defined DEEP_SLEEP
  #define SLEEPYTIME (1000UL * 60 * 5) // 5 mins
  #include <avr/sleep.h>
  #include <avr/power.h>
#endif  // DEEP_SLEEP

/***** Battery meter settings *****/
const long InternalReferenceVoltage = 1062;  // Adjust this value to your board's specific internal BG voltage
#define BATTERY_FACTOR 5 // Callibration value to compensate for component variation
#define LOW_BATTERY 3.3 // low voltage for battery, a 5v Arduino or DIYino requires 3.3v 
#define FULL_BATTERY 4.15 // full voltage for battery, nominally 4.2 for a 3.7v battery

/*
 * DEBUG PARAMETERS
 */
/* BL_INFO
 * For daily use I recommend you comment BL_INFO
 * When you plug your device to USB uncomment BL_INFO !
 */
//#define BL_INFO
//#define BL_DEBUG






#endif /* CONFIG_SW_H_ */
