/*
 * Config.h
 *
 * Created on: 01.01.2017
 * author:     Andras Kun (kun.andras@yahoo.de) based on LSOS by Sebastien CAPOU (neskweek@gmail.com)
 * Source :   https://github.com/Protonerd/FX-BlasterOS
 */

#if not defined CONFIG_H_
#define CONFIG_H_


/************************************/

/*
 * BLASTER LED TYPE
 *
 * RGB LED OR NEOPIXEL users:
 * Comment the following line will
 * disable and remove all corresponding
 * blocks from compile
 *************************************/
//#define LUXEON
#define NEOPIXEL

/*
 * DO NOT MODIFY
 * Unless you know what you're doing
 *************************************/
#define CONFIG_VERSION     "L04"  // config version of BlasterOS
#define MEMORYBASE      32
/************************************/

/*
 * DEFAULT CONFIG PARAMETERS
 * Will be overriden by EEPROM settings
 * once the first save will be done
 *************************************/
#define VOL         31
#define SOUNDFONT       1
/************************************/

/************************************/

/*
 * OLED DISPLAY
 *

 *************************************/
//#define OLED_DISPLAY

#if defined NEOPIXEL
// How many leds in one strip?
#define STRIPE1        40 // nozzle running light
#define STRIPE2       1  // main blaster light
#define STRIPE3       2  // 2 auxiliary blaster lights
#define NUMPIXELS STRIPE1+STRIPE2+STRIPE3

// Number of color defined
static const uint8_t rgbFactor = 100;

// For led chips like NEOPIXELs, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 			13 //D13
#endif

#define COLORS 14


/************************************/ // BLADE TYPE





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
#define MAX_BRIGHTNESS		200

/* LIGHT_EFFECTS
 *
 * Enable / disable  all light and
 * color effects of the blade
 * If you a device with a CPU wich is not
 * an Atmega328 : COMMENT THIS
 ************************************/
//#define LIGHT_EFFECTS


/* DEEP_SLEEP
 * If you want to enable/disable
 * deep sleep capabalities
 * If you a device with a CPU wich is not
 * an Atmega328 : COMMENT THIS
 *************************************/
//#define DEEP_SLEEP
#if defined DEEP_SLEEP
#define SLEEP_TIMER			300000 //5min = 300000 millisecs
#endif




/*
 * PINS DEFINITION
 */



#ifdef NEOPIXEL
#define LS1       3
#define LS2       5
#define LS3       6
#define LS4       9
#define LS5       10
#define LS6       11

#endif

#if defined LUXEON

#define LED_RED 			9
#define LED_GREEN 			10
#define LED_BLUE 			11
#endif




#define DFPLAYER_RX			8
#define DFPLAYER_TX			7
#define SPK1				20 //A6
#define SPK2				21 //A7


#define MAIN_BUTTON			12
#define AUX_BUTTON		4
//#define AMMOSWITCH    14
#ifdef AMMOSWITCH
#define AMMOCLIPMINDURATION 700  // min time duration in [ms] to recognize ammo magazine removed/inserted
#endif

//#define BUZZMOTOR  17 //A3
//#define BUTTONLEDPIN 16 //A2


/*
 * CONFIG MENU PARAMETERS
 */
#define JUKEBOX
#if defined LUXEON
#define CONFIG_BLADE_MAIN_COLOR
#define CONFIG_BLADE_CLASH_COLOR
#endif

#if defined NEOPIXELS
#define CONFIG_BLADE_MAIN_COLOR
#define CONFIG_BLADE_CLASH_COLOR
#define CONFIG_POWERON_EFFECT
#define CONFIG_POWEROFF_EFFECT
#define CONFIG_FLICKER_EFFECT
#endif

#if defined LEDSTRINGS
#define CONFIG_POWERON_EFFECT
#define CONFIG_POWEROFF_EFFECT
#define CONFIG_FLICKER_EFFECT
#endif
/*
 * DEBUG PARAMETERS
 */
/* BL_INFO
 * For daily use I recommend you comment BL_INFO
 * When you plug your device to USB uncomment BL_INFO !
 */
#define BL_INFO
#define BL_DEBUG

#endif /* CONFIG_H_ */
