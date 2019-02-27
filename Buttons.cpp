/*
 * Buttons.c
 *
 *  Created on: 6 mars 2016
 * author:     Andras Kun (kun.andras@yahoo.de) based on LSOS by Sebastien CAPOU (neskweek@gmail.com)
 * Source : 	https://github.com/Protonerd/FX-BlasterOS
 */
#include <DFPlayer.h>
#include "Buttons.h"
#include "Config_HW.h"
#include "Config_SW.h"
#include "ConfigMenu.h"
#include "SoundFont.h"
#include "Light.h"
#include "OLED.h"

extern DFPlayer dfplayer;
extern SoundFont soundFont;
extern BlasterStateEnum BlasterState;
extern BlasterStateEnum PrevBlasterState;

extern ActionModeSubStatesEnum PrevActionModeSubStates;
extern ActionModeSubStatesEnum ActionModeSubStates;
extern ConfigModeSubStatesEnum ConfigModeSubStates;

//extern unsigned long sndSuppress;
extern int8_t modification;
extern bool play;
extern int16_t value;
//extern int8_t blink;
extern bool changeMenu;
//extern uint8_t menu;
extern bool enterMenu;
extern uint8_t AmmoCnt;
extern void lightOff();
extern uint8_t ledPins[];
#ifdef JUKEBOX
extern bool jukebox_play;
extern uint8_t jb_track;
#endif
//extern cRGB color;
extern cRGB currentColor;
extern void SinglePlay_Sound(uint8_t track);
extern void LoopPlay_Sound(uint8_t track);
extern void Pause_Sound();
extern void Resume_Sound();
extern void Set_Loop_Playback();
extern void Set_Volume(int8_t volumeSet=-1);
extern void DisplayBlasterStatus(char statustext[6], bool clearScreen=false);
extern void DisplayStartScreen();
extern void DisplayBlasterOnFrames();
//extern void DisplayAmmoCount(uint8_t AmmoCnt);
//extern void DisplayAmmoCount(uint8_t AmmoCnt, uint16_t iAngle=-1);
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

// ====================================================================================
// ===               			BUTTONS CALLBACK FUNCTIONS                 			===
// ====================================================================================

void ConfigMenuButtonEventHandler(ButtonActionEnum ButtonActionType){
    if (ConfigModeSubStates == CS_VOLUME and ButtonActionType==SINGLE_CLICK) {
      confParseValue(storage.volume, 5, 30, 1);
      storage.volume = value;
      BladeMeter(ledPins, value*100/30);
      Set_Volume(storage.volume);
      #if defined LS_INFO
              Serial.println(storage.volume);
      #endif      
    }
    else if (ConfigModeSubStates == CS_BLASTCOLOR and ButtonActionType==SINGLE_CLICK) {
          #ifdef V_MK1
          confParseValue(modification, 0, 2, 1);
          #endif
          #if defined V_MK2 or defined V_MK3
          confParseValue(modification, 0, 7, 1);
          #endif // Variants
          #if defined V_MK4 or defined V_MK5
          confParseValue(modification, 0, 15, 1);
          #endif // Variants
          modification = value;
          getColorFix(modification);
          
        storage.sndProfile[storage.soundFont].blastColor.r=currentColor.r;
        storage.sndProfile[storage.soundFont].blastColor.g=currentColor.g;
        storage.sndProfile[storage.soundFont].blastColor.b=currentColor.b;
        getColor(storage.sndProfile[storage.soundFont].blastColor);
        lightOn(currentColor, storage.NrStatusBarPixels, storage.NrBarellPixels+storage.NrStatusBarPixels-1);
    
    }
    else if (ConfigModeSubStates == CS_STUNCOLOR and ButtonActionType==SINGLE_CLICK) {
          #ifdef V_MK1
          confParseValue(modification, 0, 2, 1);
          #endif
          #if defined V_MK2 or defined V_MK3
          confParseValue(modification, 0, 7, 1);
          #endif // Variants
          #if defined V_MK4 or defined V_MK5
          confParseValue(modification, 0, 15, 1);
          #endif // Variants
          modification = value;
          getColorFix(modification);
          
        storage.sndProfile[storage.soundFont].stunColor.r=currentColor.r;
        storage.sndProfile[storage.soundFont].stunColor.g=currentColor.g;
        storage.sndProfile[storage.soundFont].stunColor.b=currentColor.b;
        getColor(storage.sndProfile[storage.soundFont].stunColor);
        lightOn(currentColor, storage.NrStatusBarPixels, storage.NrBarellPixels+storage.NrStatusBarPixels-1);
    }
    else if (ConfigModeSubStates == CS_PIXELLENGHT_BARRELL and ButtonActionType==SINGLE_CLICK) {
        play = true;

        confParseValue(storage.NrBarellPixels, 0, MAX_BARREL_PIXEL, 1);
        storage.NrBarellPixels=value;
        Serial.println(storage.NrBarellPixels);
        if (storage.NrBarellPixels!=0) {
          lightOn(storage.sndProfile[storage.soundFont].blastColor, storage.NrStatusBarPixels,storage.NrStatusBarPixels+storage.NrBarellPixels-1);
        }
        if (storage.NrBarellPixels==0) {
          lightOff(0, MAX_BARREL_PIXEL+MAX_STATUSBAR_PIXEL-1);
        } else {
          lightOff(storage.NrStatusBarPixels+storage.NrBarellPixels,MAX_BARREL_PIXEL+MAX_STATUSBAR_PIXEL-1);
        }
      }
    else if (ConfigModeSubStates == CS_PIXELLENGHT_STATUSBAR and ButtonActionType==SINGLE_CLICK) {
        play = true;
        confParseValue(storage.NrStatusBarPixels, 0, MAX_STATUSBAR_PIXEL, 1);
        storage.NrStatusBarPixels=value;
        Serial.println(storage.NrStatusBarPixels);
        if (storage.NrStatusBarPixels!=0) {
          lightOn({0,0,10}, 0, storage.NrStatusBarPixels-1);
        }
        if (storage.NrStatusBarPixels==0) {
          lightOff(0, MAX_BARREL_PIXEL+MAX_STATUSBAR_PIXEL-1);
        } else {
          lightOff(storage.NrStatusBarPixels, MAX_BARREL_PIXEL+MAX_STATUSBAR_PIXEL-1);
        }
    }
    else if (ConfigModeSubStates == CS_SOUNDFONT and ButtonActionType==SINGLE_CLICK) {
      play = false;
      confParseValue(storage.soundFont, 0, SOUNDFONT_QUANTITY - 1, 1);
      storage.soundFont = value;
      Serial.println(value);
      soundFont.setID(value);
      SinglePlay_Sound(soundFont.getMenu((storage.soundFont)*NR_FILE_SF));
      //Serial.print("soundfont   "); Serial.print(storage.soundFont); Serial.print("  Offset:   ");Serial.println(soundFont.getMenu((storage.soundFont)*NR_FILE_SF));
      delay(150);    
    }
    else if (ConfigModeSubStates == CS_DSKINS and ButtonActionType==SINGLE_CLICK) {
      confParseValue(storage.sndProfile[storage.soundFont].DisplaySkins, 0, 2, 1);
      storage.sndProfile[storage.soundFont].DisplaySkins=value;
       DisplayBlasterOnFrames();
       DisplayAmmoCount(AmmoCnt);
   }
  

}

void mainClick() {
#if defined BL_DEBUG
    Serial.println(F("Main button click."));
#endif
//S_STANDBY, S_BLASTERON, S_CONFIG, S_SLEEP, S_JUKEBOX
  if (BlasterState==S_BLASTERON) {
 
     // AS_BLASTER_MODE, AS_STUN_MODE, AS_REPEATER, AS_BLAST, AS_STUN, AS_CLIPIN, AS_CLIPOUT, AS_EMPTY, AS_FULL, AS_MANUALRELOAD, AS_SWITCHOFF, AS_JAM, AS_UNJAM, AS_OVERHEAT
    if (ActionModeSubStates==AS_BLASTER_MODE) {
     /*
     * Single Shot
     */
      ActionModeSubStates=AS_BLAST;
    }
    else if (ActionModeSubStates==AS_STUN_MODE) {
      ActionModeSubStates=AS_STUN;
    }    
    else if (ActionModeSubStates==AS_MEGABLAST_MODE) {
      ActionModeSubStates=AS_MEGABLAST;
    }
    #ifdef SINGLEBUTTON
    else if (ActionModeSubStates==AS_REPEATER_MODE) { // start dakka-dakka
      ActionModeSubStates=AS_REPEATER;
    } 
    else if (ActionModeSubStates==AS_REPEATER) { // stop dakka-dakka
      ActionModeSubStates=AS_REPEATER_MODE;
    } 
    #endif // SINGLEBUTTON
    else if (ActionModeSubStates==AS_FULL) { // activate last blaster mode before empty state
      ActionModeSubStates=PrevActionModeSubStates;
    } 
    else if (ActionModeSubStates==AS_EMPTY or ActionModeSubStates==AS_CLIPOUT) {
      SinglePlay_Sound(soundFont.getEmpty((storage.soundFont)*NR_FILE_SF));
      delay(500);      
    }
    else if (ActionModeSubStates==AS_SWITCHOFF) {  // return to IDLE/START-UP MODE
     BlasterState=S_STANDBY;
     PrevBlasterState=S_BLASTERON;     
    } 
  }
  else if (BlasterState==S_CONFIG) {
    /*
     * ACTION TO DEFINE
     */
    #ifdef DEEP_SLEEP
    if (ConfigModeSubStates==CS_SLEEPINIT) {
        BlasterState=S_SLEEP;
        PrevBlasterState=S_CONFIG;
    }
    #endif // DEEP_SLEEP
    ConfigMenuButtonEventHandler(SINGLE_CLICK);  
  }
  else if (BlasterState==S_STANDBY) {
    /*
     * enter BlasterOn mode (activate blaster)
     */    
    #if defined BL_DEBUG
    Serial.println(F("Enter BlasterOn mode."));
    #endif
    BlasterState=S_BLASTERON;
    PrevBlasterState=S_STANDBY;
    ActionModeSubStates=AS_BLASTER_MODE;
  }
#ifdef JUKEBOX 
  else if (BlasterState==S_JUKEBOX) {
    #if defined LS_BUTTON_DEBUG
        Serial.print(F("Play/Pause current song "));Serial.print(jb_track);
    #endif
    if (jukebox_play) {
      // pause the song
      #if defined LS_BUTTON_DEBUG
            Serial.println(F("Pause Song"));
      #endif
      jukebox_play=false;
      Pause_Sound();
    } else {
      // resume playing the song
      #if defined LS_BUTTON_DEBUG
            Serial.println(F("Resume Song"));
      #endif
      jukebox_play=true;
      Resume_Sound();
    }
  } 
#endif // JUKEBOX 
} // mainClick

void mainLongPressStart() {
#if defined LS_BUTTON_DEBUG
  Serial.println(F("Main button longPress start"));
#endif
//S_STANDBY, S_BLASTERON, S_CONFIG, S_SLEEP, S_JUKEBOX
     // AS_BLASTER_MODE, AS_STUN_MODE, AS_REPEATER, AS_BLAST, AS_STUN, AS_CLIPIN, AS_CLIPOUT, AS_EMPTY, AS_FULL, AS_MANUALRELOAD, AS_SWITCHOFF, AS_JAM, AS_UNJAM, AS_OVERHEAT
  if (BlasterState==S_BLASTERON) {
  #ifdef SINGLEBUTTON
    // in blaster on mode long press serves as mode switch trigger
    if (ActionModeSubStates==AS_SWITCHOFF or ActionModeSubStates==AS_MANUALRELOAD) {
     ActionModeSubStates=AS_BLASTER_MODE;
     //DisplayBlasterStatus("BLAST  ", false);
     StatusBar_ModeChange(1);  
     delay(500);  
    }    
    else if (ActionModeSubStates==AS_BLASTER_MODE) {
      #if defined V_MK1 or defined V_MK2 // goto stun mode
        ActionModeSubStates=AS_STUN_MODE;
        //DisplayBlasterStatus("STUN   ", false);
        StatusBar_ModeChange(2);  
        delay(500);      
      #endif
      #if defined V_MK3
        ActionModeSubStates=AS_REPEATER_MODE;
        //DisplayBlasterStatus("REPEAT ", false);
        StatusBar_ModeChange(2);  
        delay(500);  
      #endif
      #if defined V_MK4 or defined V_MK5
        ActionModeSubStates=AS_STUN_MODE;
        //DisplayBlasterStatus("STUN   ", false);
        StatusBar_ModeChange(2);  
        delay(500);  
      #endif
    }
    else if (ActionModeSubStates==AS_REPEATER_MODE) {
      #if defined V_MK3
        ActionModeSubStates=AS_STUN_MODE;
        //DisplayBlasterStatus("STUN   ", false);
        StatusBar_ModeChange(3);  
        delay(500);
      #endif  
    }
    else if (ActionModeSubStates==AS_STUN_MODE) {
      #if defined V_MK1 or defined V_MK2 // goto stun mode
        ActionModeSubStates=AS_SWITCHOFF;
        #if defined OLED_STD      
          DisplayBlasterStatus("PWRDWN ", false);
        #endif
        StatusBar_ModeChange(3);  
        delay(500);    
      #endif
      #if defined V_MK3
        ActionModeSubStates=AS_SWITCHOFF;      
        DisplayBlasterStatus("PWRDWN ", false);
        StatusBar_ModeChange(4);  
        delay(500); 
      #endif
      #if defined V_MK4 or defined V_MK5
        ActionModeSubStates=AS_MEGABLAST_MODE;
        //DisplayBlasterStatus("SPECIAL", false);
        StatusBar_ModeChange(3);  
        delay(500);
      #endif
   
    }
    else if (ActionModeSubStates==AS_MEGABLAST_MODE or ActionModeSubStates==AS_CLIPOUT) {
      ActionModeSubStates=AS_SWITCHOFF;      
      DisplayBlasterStatus("PWRDWN ", false);
      StatusBar_ModeChange(4);  
      delay(500);  
    }
    if (ActionModeSubStates==AS_JAM) {
      ActionModeSubStates=AS_UNJAM;
    }
    else if (ActionModeSubStates==AS_EMPTY) {
      ActionModeSubStates=AS_MANUALRELOAD;
    }   
    /*
     * ACTION TO DEFINE
     */
     // AS_BLASTER_MODE, AS_STUN_MODE, AS_REPEATER, AS_BLAST, AS_STUN, AS_CLIPIN, AS_CLIPOUT, AS_EMPTY, AS_FULL, AS_MANUALRELOAD, AS_SWITCHOFF, AS_JAM, AS_UNJAM, AS_OVERHEAT
  #endif // SINGLEBUTTON
  #ifndef SINGLEBUTTON
    if (ActionModeSubStates==AS_MEGABLAST_MODE) {
      //SinglePlay_Sound(soundFont.getFlameThrower((storage.soundFont)*NR_FILE_SF));
      LoopPlay_Sound(soundFont.getFlameThrower((storage.soundFont)*NR_FILE_SF));
      #ifdef BUZZMOTOR
      digitalWrite(BUZZMOTOR,HIGH);
      #endif
      RampBarrel(soundFont.getFirethrowerRampDuration(), true);
      ActionModeSubStates=AS_FLAMETHROWER;
    }
    
  #endif
  }
  else if (BlasterState==S_CONFIG) {
    if (ConfigModeSubStates== CS_LASTMEMBER-1) {
      // exit config mode upon reaching the last config menu item
      changeMenu = false;
      BlasterState=S_STANDBY;
      PrevBlasterState=S_CONFIG;
    }
    else {
      // long press to transit to next config mode menu item
      NextConfigState();
    }
  }
  else if (BlasterState==S_STANDBY) {
      BlasterState=S_CONFIG;
      PrevBlasterState=S_STANDBY;
  }
#ifdef JUKEBOX
  else if (BlasterState==S_JUKEBOX) {
    // jump to next song and start playing it
    if (jb_track==NR_CONFIGFOLDERFILES+(SOUNDFONT_QUANTITY*NR_FILE_SF)+NR_JUKEBOXSONGS) {
      jb_track=NR_CONFIGFOLDERFILES+(SOUNDFONT_QUANTITY*NR_FILE_SF)+1;  // fold back to first song in the dir designated for music playback
    }
    else {
      jb_track++;
    }
    SinglePlay_Sound(jb_track);
  } 
#endif // JUKEBOX 

} // mainLongPressStart

void mainLongPress() {
#if defined LS_BUTTON_DEBUG
  Serial.println(F("Main button longPress..."));
#endif
  if (BlasterState==S_BLASTERON) {
    #ifndef SINGLEBUTTON
    if (ActionModeSubStates==AS_BLASTER_MODE) {
      ActionModeSubStates=AS_REPEATER;
    }  
    else if (ActionModeSubStates==AS_MEGABLAST_MODE) {
      ActionModeSubStates=AS_FLAMETHROWER;
    }   
    else if (ActionModeSubStates==AS_EMPTY or ActionModeSubStates==AS_CLIPOUT) {
      SinglePlay_Sound(soundFont.getEmpty((storage.soundFont)*NR_FILE_SF));
      delay(500);      
    }
    #endif // not SINGLEBUTTON
  }
} // mainLongPress

void mainLongPressStop() {
#if defined LS_BUTTON_DEBUG
  Serial.println(F("Main button longPress stop"));
#endif
//S_STANDBY, S_BLASTERON, S_CONFIG, S_SLEEP, S_JUKEBOX
  if (BlasterState==S_BLASTERON) {
    #ifdef SINGLEBUTTON
    if (ActionModeSubStates==AS_FULL or ActionModeSubStates== AS_MANUALRELOAD) {
      PrevActionModeSubStates=AS_MANUALRELOAD;
      delay(500);
      ActionModeSubStates=PrevActionModeSubStates;
      if (ActionModeSubStates== AS_REPEATER) {
        ActionModeSubStates=AS_REPEATER_MODE;
      }
      else if (ActionModeSubStates== AS_BLAST) {
        ActionModeSubStates= AS_BLASTER_MODE;
      }
      else if (ActionModeSubStates== AS_STUN) {
        ActionModeSubStates= AS_STUN_MODE;       
      }
      else if (ActionModeSubStates== AS_MEGABLAST) {
        ActionModeSubStates= AS_MEGABLAST_MODE;        
      }
      else if (ActionModeSubStates== AS_FLAMETHROWER) {
        ActionModeSubStates= AS_FLAMETHROWER_MODE;
        RampBarrel(soundFont.getFirethrowerRampDuration(), false);
        Pause_Sound();
        delay(1000);
        lightOff(storage.NrStatusBarPixels,storage.NrStatusBarPixels + storage.NrBarellPixels);
        #ifdef BUZZMOTOR        
        digitalWrite(BUZZMOTOR,LOW);
        #endif
      }
    }
    #endif // SINGLEBUTTON
    #ifndef SINGLEBUTTON
    if (ActionModeSubStates==AS_REPEATER) {
      ActionModeSubStates=AS_BLASTER_MODE;
    }  
    else if (ActionModeSubStates==AS_FLAMETHROWER) {
      ActionModeSubStates=AS_MEGABLAST_MODE;
      RampBarrel(soundFont.getFirethrowerRampDuration(), false);
      Pause_Sound();
      lightOff(storage.NrStatusBarPixels,storage.NrStatusBarPixels + storage.NrBarellPixels);
      #ifdef BUZZMOTOR
      digitalWrite(BUZZMOTOR,LOW);
      #endif
    }   
    else if (ActionModeSubStates==AS_FULL) { // activate last blaster mode before empty state
      ActionModeSubStates=PrevActionModeSubStates;
    } 
    #endif // not SINGLEBUTTON
  }
} // mainLongPressStop

#ifndef SINGLEBUTTON
void auxClick() {
   Serial.println(F("Aux button click"));
  if (BlasterState==S_BLASTERON) {
  #ifndef SINGLEBUTTON
    // in blaster on mode long press serves as mode switch trigger
    if (ActionModeSubStates==AS_SWITCHOFF or ActionModeSubStates==AS_MANUALRELOAD) {
     ActionModeSubStates=AS_BLASTER_MODE;
     //DisplayBlasterStatus("BLAST  ");
     StatusBar_ModeChange(1);  
     delay(500);  
    }    
    else if (ActionModeSubStates==AS_BLASTER_MODE) {
      #if defined V_MK1 or V_MK2 // goto stun mode
        ActionModeSubStates=AS_STUN_MODE;
        //DisplayBlasterStatus("STUN   ");
        StatusBar_ModeChange(2);  
        delay(500);      
      #endif
      #if defined V_MK3
        ActionModeSubStates=AS_REPEATER_MODE;
        //DisplayBlasterStatus("REPEAT ");
        StatusBar_ModeChange(2);  
        delay(500);  
      #endif
      #if defined V_MK4 or defined V_MK5
        ActionModeSubStates=AS_STUN_MODE;
        //DisplayBlasterStatus("STUN   ");
        StatusBar_ModeChange(2);  
        delay(500);  
      #endif
    }
    else if (ActionModeSubStates==AS_REPEATER_MODE) {
      #if defined V_MK3
        ActionModeSubStates=AS_STUN_MODE;
        //DisplayBlasterStatus("STUN   ");
        StatusBar_ModeChange(3);  
        delay(500);
      #endif  
    }
    else if (ActionModeSubStates==AS_STUN_MODE) {
      #if defined V_MK1 or defined V_MK2 // goto stun mode
        ActionModeSubStates=AS_SWITCHOFF;      
        DisplayBlasterStatus("PWRDWN ");
        StatusBar_ModeChange(3);  
        delay(500);    
      #endif
      #if defined V_MK3
        ActionModeSubStates=AS_SWITCHOFF;      
        DisplayBlasterStatus("PWRDWN ");
        StatusBar_ModeChange(4);  
        delay(500); 
      #endif
      #if defined V_MK4 or defined V_MK5
        ActionModeSubStates=AS_MEGABLAST_MODE;
        //DisplayBlasterStatus("MEGABL ");
        StatusBar_ModeChange(3);  
        delay(500);
      #endif
   
    }
    else if (ActionModeSubStates==AS_MEGABLAST_MODE or ActionModeSubStates==AS_CLIPOUT) {
      ActionModeSubStates=AS_SWITCHOFF;      
      DisplayBlasterStatus("PWRDWN ");
      StatusBar_ModeChange(4);  
      delay(500);  
    }
    if (ActionModeSubStates==AS_JAM) {
      ActionModeSubStates=AS_UNJAM;
    }  
    /*
     * ACTION TO DEFINE
     */
     // AS_BLASTER_MODE, AS_STUN_MODE, AS_REPEATER, AS_BLAST, AS_STUN, AS_CLIPIN, AS_CLIPOUT, AS_EMPTY, AS_FULL, AS_MANUALRELOAD, AS_SWITCHOFF, AS_JAM, AS_UNJAM, AS_OVERHEAT
     #endif // #ifndef SINGLEBUTTON
  }  
} // auxLongPressStart

void auxDoubleClick() {
  // exit blaster on mode (safety measure)
  if (BlasterState==S_BLASTERON) {
      BlasterState=S_STANDBY;
      PrevBlasterState=S_BLASTERON;
  }
}

void auxLongPressStart() {
  // exit config mode immediately if the aux button is pressed for a long time
  if (BlasterState==S_CONFIG) {
      BlasterState=S_STANDBY;
      PrevBlasterState=S_CONFIG;
  }
  else if (BlasterState==S_BLASTERON) {
    if (ActionModeSubStates==AS_EMPTY) {
      ActionModeSubStates=AS_MANUALRELOAD;
    }     
  }
  else if (BlasterState==S_STANDBY) {
      BlasterState=S_JUKEBOX;
      PrevBlasterState=S_STANDBY;    
  }
  else if (BlasterState==S_JUKEBOX) {
      BlasterState=S_STANDBY;
      PrevBlasterState=S_JUKEBOX;
      Pause_Sound();    
  }
} // auxLongPressStart

//void auxLongPress() {
//} // auxLongPress

void auxLongPressStop() {
  if (BlasterState==S_BLASTERON) {
    if (ActionModeSubStates==AS_FULL) { // activate last blaster mode before empty state
      ActionModeSubStates=PrevActionModeSubStates;
    } 
    else if (ActionModeSubStates==AS_MANUALRELOAD) { // return to previouslt active mode
      ActionModeSubStates=PrevActionModeSubStates;
    }
  } // auxLongPressStop
}

#endif
