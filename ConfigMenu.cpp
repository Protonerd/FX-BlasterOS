/*
 * Config.cpp
 *
 * author:     Andras Kun (kun.andras@yahoo.de) based on LSOS by Sebastien CAPOU (neskweek@gmail.com)
 * Source :   https://github.com/Protonerd/FX-BlasterOS
 *      Author: neskw
 */
#include "ConfigMenu.h"
#include "Config_HW.h"
#include "Config_SW.h"
#include "Light.h"
#include "SoundFont.h"

/*
 * Blaster Finite State Machine Custom Type and State Variable
 */
BlasterStateEnum BlasterState;
BlasterStateEnum PrevBlasterState;

ActionModeSubStatesEnum ActionModeSubStates;
ActionModeSubStatesEnum PrevActionModeSubStates;
ConfigModeSubStatesEnum ConfigModeSubStates;
DisplaySkins_Enum DisplaySkins;

extern int8_t modification;
extern bool play;
extern int16_t value;
extern uint8_t ledPins[];


extern void SinglePlay_Sound(uint8_t track);
extern void LoopPlay_Sound(uint8_t track);
extern void Disable_FTDI(bool ftdi_off);
extern void Disable_MP3(bool mp3_off);
extern void MonitorBattery();
extern void DisplayBlasterStatus(char statustext[6], bool clearScreen=false);
extern void DisplayStartScreen();

#if defined NEOPIXEL
//extern cRGB color;
extern cRGB currentColor;
#endif

extern struct StoreStruct {
 // This is for mere detection if they are our settings
  char version[5];
  // The settings
  uint8_t volume;// 0 to 31
  uint8_t soundFont;// as many as Sound font you have defined in Soundfont.h Max:253
  uint8_t NrBarellPixels;
  uint8_t NrStatusBarPixels;
  struct Profile {
    cRGB blastColor;
    cRGB stunColor;
    uint8_t DisplaySkins;
  }sndProfile[SOUNDFONT_QUANTITY];
}storage;
extern SoundFont soundFont;

// ====================================================================================
// ===           	  	 			CONFIG MODE FUNCTIONS	                		===
// ====================================================================================

void confParseValue(uint16_t variable, uint16_t min, uint16_t max,
		short int multiplier) {

	value = variable + (multiplier * 1);
	if (value < (int) min) {
		value = max;
	} else if (value > (int) max) {
		value = min;
	} else if (value == (int) min and play) {
		play = false;
		SinglePlay_Sound(9);
		delay(150);
	} else if (value == (int) max and play) {
		play = false;
		SinglePlay_Sound(8);
		delay(150);
	}
} //confParseValue

void NextConfigState(){
  if (ConfigModeSubStates!=CS_STUNCOLOR and ConfigModeSubStates!=CS_BLASTCOLOR) {  
    lightOff();
  }
  if (ConfigModeSubStates== CS_LASTMEMBER) {
    ConfigModeSubStates=-1;
  }
  
  ConfigModeSubStates=ConfigModeSubStates+1; // change to next config state in the ordered list
  

  switch(ConfigModeSubStates) {
      case CS_VOLUME: 
        #if defined LS_FSM
          Serial.print(F("Volume"));
        #endif  
        ConfigModeSubStates=CS_VOLUME;
        DisplayBlasterStatus("VOLUME     ", true);
        BladeMeter(ledPins, storage.volume*100/30);
        SinglePlay_Sound(4);
        delay(500);
        break;
      case CS_SOUNDFONT: 
        #if defined LS_FSM
          Serial.print(F("Sound font"));
        #endif        
        lightOff();
        DisplayBlasterStatus("SOUND FONT ", true);
        SinglePlay_Sound(5);
        delay(600);
        SinglePlay_Sound(soundFont.getMenu((storage.soundFont)*NR_FILE_SF));
        delay(500);  
        break; 
      case CS_STUNCOLOR: 
        #if defined LS_FSM
          Serial.print(F("Stun color"));
        #endif        
        DisplayBlasterStatus("STUN COLOR ", true);
        SinglePlay_Sound(7);
        delay(500); 
        getColor(storage.sndProfile[storage.soundFont].stunColor);
        pixelblade_KillKey_Disable();
        lightOn(currentColor, storage.NrStatusBarPixels, storage.NrBarellPixels+storage.NrStatusBarPixels); 
        break;
      case CS_BLASTCOLOR: 
        #if defined LS_FSM
          Serial.print(F("Blast color"));
        #endif        
         DisplayBlasterStatus("BLAST COLOR", true);
       SinglePlay_Sound(6);
        delay(500); 
        getColor(storage.sndProfile[storage.soundFont].blastColor);
        pixelblade_KillKey_Disable();
        lightOn(currentColor, storage.NrStatusBarPixels, storage.NrBarellPixels+storage.NrStatusBarPixels); 
        break;         
      case CS_SLEEPINIT: 
        #if defined LS_FSM
          Serial.print(F("Initialize sleep mode"));
        #endif
        MonitorBattery();
        DisplayBlasterStatus("SLEEP MODE ", true);        
        SinglePlay_Sound(12);
        delay(500);
        break;   
      case CS_PIXELLENGHT_BARRELL:
        #if defined LS_FSM
          Serial.print(F("Pixel-lenght of the blaster barrell"));
        #endif
        DisplayBlasterStatus("BARRELPIXEL", true);
        lightOn(storage.sndProfile[storage.soundFont].blastColor, storage.NrStatusBarPixels,storage.NrStatusBarPixels+storage.NrBarellPixels-1);  
        SinglePlay_Sound(13);
        delay(500); 
        break;
      case CS_PIXELLENGHT_STATUSBAR:
        #if defined LS_FSM
          Serial.print(F("Pixel-lenght of the blaster barrell"));
        #endif
        DisplayBlasterStatus("STATUSPIXEL", true);
        lightOn({0,0,10}, 0, storage.NrStatusBarPixels-1);
        SinglePlay_Sound(14);
        delay(500); 
        break;
      case CS_BATTERYLEVEL:
        DisplayBlasterStatus("BATTERY    ");
        #if defined LS_FSM
          Serial.print(F("Display battery level"));
        #endif        
        MonitorBattery();
        break;   
      case CS_DSKINS:
        SinglePlay_Sound(15);
        DisplayBlasterStatus("DISPLAYSKIN", true);
        #if defined LS_FSM
          Serial.print(F("Display Dislay Skins"));
        #endif        
        break; 
        }  
}
