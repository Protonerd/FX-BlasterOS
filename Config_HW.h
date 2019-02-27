/*
 * Config_HW.h
 *
 * Created on: 30 March 2017
 * author:    Andras Kun (kun.andras@yahoo.de)
 * Source :   https://github.com/neskweek/LightSaberOS
 * This tab contains all settings necessary to define your saber board's electronic hardware configuration
 */
#include "Variants.h"


#if not defined CONFIG_HW_H_
#define CONFIG_HW_H_

/* Board definitions
 *  Define what kind of Arduino compatible board you have in your saber.
 *  DIYino Prime and Stardust are both dedicated, fully integrated
 *  saber boards manufactured by ProtoWerkstatt.
 *  For more information on DIYino products, see:
 *  https://github.com/Protonerd/DIYino
 *  https://www.facebook.com/ProtoWerkstatt-740015792832712/
 *  If you have a home-brew solution using the Arduino Nano, choose DIYINO_PRIME below
 */


/***** BOARD PINOUT DEFINITIONS ******/

/*
 * Definition of the MAIN and AUX buttons of your saber.
 * These 2 buttons are used to interact with the saber electronics in the hilt
 * With the SINGLEBUTTON compile directive you can configure your saber electronics
 * to use only a single button to interact with your board.
 */

// If your blaster has only a single button to interact with the electronics, uncomment the next line
// in case you have 2 buttons (referred to as main and aux buttons) leave this line commented out

#define MAIN_BUTTON      12
#ifndef SINGLEBUTTON
  #define AUX_BUTTON   11
#endif

/*
 * POWER SAVING CIRCUITRY
 * Definition of the power switches (DIYino Prime v1.5 or greater, STARDUST
 * The MP3 power switch will cut power to the MP3 chipset as well as disable the audio amp
 * The FTDI power switch will cut power to the Serial-to-UART FTDI chipset, saving power and
 * allowing a PC to access the SD-card or SPI flash via the MP3 chipset over USB.
 * Enable DEEP_SLEEP in software config to make use of the power saving options.
 */
#define MP3_PSWITCH 17 // A3
#define FTDI_PSWITCH 16 // A2

/*
/*
 * BLASTER LED TYPE
 *
 * RGB LED OR NEOPIXEL users:
 * Comment the following line will
 * disable and remove all corresponding
 * blocks from compile
 *************************************/
#define NEOPIXEL


#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#endif

/************************************/

/*
 * PIXEL CHAIN DEFINITION
 *

 *************************************/
#if defined NEOPIXEL
  // How many leds in one strip?
  // my DoopyDoos E11 prop:
  // 1st stripe: 27 pixels in the main barrel
  // 2nd stripe: 5 additional pixels in the status bar
  // 1 nozzle pixel
  // Gergo's Death Guard blaster:
  // 1st stripe: 8  pixels in the status bar
  // 2nd stripe: 30 pixels in the main barrel
  // 3rd 2 nozzle pixels
  
  /*
  // 1st E11 DoopyDoo
  #define BARRELSTRIPE_LENGTH 27
  #define STATUSBARSTRIPE_LENGTH 5
  #define NOZZLESTRIPE_LENGTH 1
  #define NUMPIXELS 33 // (BARRELSTRIPE_LENGTH+STATUSBARSTRIPE_LENGTH+NOZZLESTRIPE_LENGTH)
  #define STATUSBAR_OFFSET 28
  #define BARRELSTRIPE_OFFSET 0 // STATUSBARSTRIPE_LENGTH // first pixel of the status bar
  #define NOZZLE_OFFSET 27 // (STATUSBARSTRIPE_LENGTH + BARRELSTRIPE_LENGTH)
  */
  
  /*
  // 2nd Nerf blaster
  #define BARRELSTRIPE_LENGTH 14
  #define STATUSBARSTRIPE_LENGTH 8
  #define NOZZLESTRIPE_LENGTH 1
  #define NUMPIXELS 23 // (BARRELSTRIPE_LENGTH+STATUSBARSTRIPE_LENGTH+NOZZLESTRIPE_LENGTH)
  #define STATUSBAR_OFFSET 0
  #define BARRELSTRIPE_OFFSET 8 // STATUSBARSTRIPE_LENGTH // first pixel of the status bar
  #define NOZZLE_OFFSET 22 // (STATUSBARSTRIPE_LENGTH + BARRELSTRIPE_LENGTH)
  */

  #define MAX_BARREL_PIXEL 31 // including the nozzle, which is always the last one
  #ifdef STATUSBAR
    #define MAX_STATUSBAR_PIXEL 10
  #endif

  // For led chips like NEOPIXELs, which have a data line, ground, and power, you just
  // need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
  // ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
  #define DATA_PIN       13 //D13
#endif


/*
 * PINS DEFINITION
 */



#define LS1       5
#define LS2       6
#define LS3      9

#define DFPLAYER_RX     8
#define DFPLAYER_TX     7
#define SPK1        20 //A6
#define SPK2        21 //A7



#ifdef AMMOSWITCH
  #define AMMOCLIPMINDURATION 700  // min time duration in [ms] to recognize ammo magazine removed/inserted
#endif

#endif /* CONFIG_HW_H_ */
