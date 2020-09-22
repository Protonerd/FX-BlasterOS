/*
 * Soundfont.h
 *
 * Created on:   27 feb 2016
 * author:     Andras Kun (kun.andras@yahoo.de) based on LSOS by Sebastien CAPOU (neskweek@gmail.com)
 * Source :   https://github.com/Protonerd/FX-BlasterOS
 * Description: Soundfont Config file for FX-BlasterOS
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.
 */

#include "Variants.h"

#ifndef SOUNDFONT_H_
#define SOUNDFONT_H_

#if defined V_MK1
  #define SOUNDFONT_QUANTITY 1
  #define NR_CONFIGFOLDERFILES 15
  #define NR_JUKEBOXSONGS 0
  #define NR_FILE_SF 22
#else
  #if defined V_MK2
    #define SOUNDFONT_QUANTITY 1
    #define NR_CONFIGFOLDERFILES 15
    #define NR_JUKEBOXSONGS 0
    #define NR_FILE_SF 22
  #else
    #if defined V_MK3
      #define SOUNDFONT_QUANTITY 1
      #define NR_CONFIGFOLDERFILES 15
      #define NR_JUKEBOXSONGS 0
      #define NR_FILE_SF 22
    #else
      #ifdef V_MK4
        #define SOUNDFONT_QUANTITY 3
        #define NR_CONFIGFOLDERFILES 15
        #define NR_JUKEBOXSONGS 1
        #define NR_FILE_SF 22
      #else
        #ifdef V_MKX
          #define SOUNDFONT_QUANTITY 3
          #define NR_CONFIGFOLDERFILES 15
          #define NR_JUKEBOXSONGS 1
          #define NR_FILE_SF 22
        #endif
      #endif    
    #endif
  #endif
#endif

class SoundFont {




public:

  SoundFont() {
    ID=0;
    megablastcharge =500;
    megablastshot=500;
    stunshot = 500;
    fireramp = 1000;
    poweron=1000;
  }
  ;
  ~SoundFont() {
    /*
     delete ID;
     delete menu;
     delete boot;
     delete hum;
     delete shot;
     delete stun;
     delete overheat;
     delete clipin;
     delete clipout;
     delete full;
     delete empty;
     delete reload;
     delete jam;
     delete unjam;
     */
  }
  ;

  void setID(uint16_t id) {
    uint16_t menu[2];
    uint16_t boot[2];
    uint16_t hum[2];
    uint16_t shot[2];
    uint16_t stun[2];
    uint16_t overheat[2];
    uint16_t clipin[2];
    uint16_t clipout[2];
    uint16_t full[2];
    uint16_t empty[2];
    uint16_t reload[2];
    uint16_t jam[2];
    uint16_t unjam[2];


    this->ID = id;

    switch (id) {

#if defined V_MK1
    default:
    case 0:
      // soundFont directory 01 :
      this->megablastcharge =1370;// 1673[ms]
      this->megablastshot=900;// 948[ms]
      this->stunshot = 700;// 1067[ms]
      this->fireramp = 1000;// [ms]
      this->poweron = 4812;// [ms]
      break;
#endif
#if defined V_MK2 or defined V_MK3
    default:
    case 0:
      // soundFont directory 01 :
      this->megablastcharge =1370;// 1673[ms]
      this->megablastshot=800;// 948[ms]
      this->stunshot = 700;// 1067[ms]
      this->fireramp = 1000;// [ms]
      this->poweron = 4812;// [ms]
      break;
    case 1:
      // soundFont directory 02 :
      this->megablastcharge =1800;// 2182[ms]
      this->megablastshot=800;// 924[ms]
      this->stunshot = 1900;//2101[ms]
      this->fireramp = 500;// [ms]
      this->poweron = 3204;// [ms]
      break;
#endif
#if defined V_MK4 or defined V_MKX
    default:
    case 0:
      // soundFont directory 02 :
      this->megablastcharge =1800;// 2182[ms]
      this->megablastshot=800;// 924[ms]
      this->stunshot = 1900;//2101[ms]
      this->fireramp = 500;// [ms]
      this->poweron = 3204;// [ms]
      break;
    case 1:
      this->megablastcharge =1370;// 1673[ms]
      this->megablastshot=800;// 948[ms]
      this->stunshot = 1900;// 2174[ms]
      this->fireramp = 300;// [ms]
      this->poweron = 2127;// [ms]
      break;
    case 2:
      this->megablastcharge =750;// 850[ms]
      this->megablastshot=750;// 856[ms]
      this->stunshot = 500;// 962[ms]
      this->fireramp = 700;// [ms]
      this->poweron = 511;// [ms]
      break;
#endif
    }

  }

// MK1 has blast and stun only
#if defined V_MK1
    #define SF_MENU_OFFSET 1
    #define SF_BOOT_OFFSET 2
    #define SF_SHOT_OFFSET 3
    #define SF_STUN_OFFSET 11
    #define SF_MENU_NR 1
    #define SF_BOOT_NR 1
    #define SF_SHOT_NR 8
    #define SF_STUN_NR 1
#endif
// MK2 has blast, stun only
#if defined V_MK2
    #define SF_MENU_OFFSET 1
    #define SF_BOOT_OFFSET 2
    #define SF_SHOT_OFFSET 3
    #define SF_STUN_OFFSET 11
    #define SF_MENU_NR 1
    #define SF_BOOT_NR 1
    #define SF_SHOT_NR 8
    #define SF_STUN_NR 1
#endif
// MK3 has a status indicator, therefore can have blast, stun, empty, reload, full
#if defined V_MK3
    #define SF_MENU_OFFSET 1
    #define SF_BOOT_OFFSET 2
    #define SF_SHOT_OFFSET 3
    #define SF_STUN_OFFSET 11
    #define SF_FULL_OFFSET 15
    #define SF_EMPTY_OFFSET 16
    #define SF_RELOAD_OFFSET 17
    #define SF_MENU_NR 1
    #define SF_BOOT_NR 1
    #define SF_SHOT_NR 8
    #define SF_STUN_NR 1
    #define SF_FULL_NR 1
    #define SF_EMPTY_NR 1
    #define SF_RELOAD_NR 1
#endif
// MK4 has the full set, bar hum
#if defined V_MK4
    #define SF_MENU_OFFSET 1
    #define SF_BOOT_OFFSET 2
    #define SF_SHOT_OFFSET 3
    #define SF_STUN_OFFSET 11
    #define SF_OVERHEAT_OFFSET 12
    #define SF_CLIPIN_OFFSET 13
    #define SF_CLIPOUT_OFFSET 1
    #define SF_FULL_OFFSET 15
    #define SF_EMPTY_OFFSET 16
    #define SF_RELOAD_OFFSET 17
    #define SF_ARCH_OFFSET 18
    #define SF_ZAP_OFFSET 19
    #define SF_MEGABLAST_OFFSET 20
    #define SF_FLAMETHROWER_OFFSET 21
    #define SF_HUM_OFFSET 22
    #define SF_MENU_NR 1
    #define SF_BOOT_NR 1
    #define SF_SHOT_NR 8
    #define SF_STUN_NR 1
    #define SF_OVERHEAT_NR 1
    #define SF_CLIPIN_NR 1
    #define SF_CLIPOUT_NR 1
    #define SF_FULL_NR 1
    #define SF_EMPTY_NR 1
    #define SF_RELOAD_NR 1
    #define SF_ARCH_NR 1
    #define SF_ZAP_NR 1
    #define SF_MEGABLAST_NR 1
    #define SF_FLAMETHROWER_NR 1
    #define SF_HUM_NR 1
#endif
// MK5 has the full set, plus hum and flamethrower
#if defined V_MKX
    #define SF_MENU_OFFSET 1
    #define SF_BOOT_OFFSET 2
    #define SF_SHOT_OFFSET 3
    #define SF_STUN_OFFSET 11
    #define SF_OVERHEAT_OFFSET 12
    #define SF_CLIPIN_OFFSET 13
    #define SF_CLIPOUT_OFFSET 1
    #define SF_FULL_OFFSET 15
    #define SF_EMPTY_OFFSET 16
    #define SF_RELOAD_OFFSET 17
    #define SF_ARCH_OFFSET 18
    #define SF_ZAP_OFFSET 19
    #define SF_MEGABLAST_OFFSET 20
    #define SF_FLAMETHROWER_OFFSET 21
    #define SF_HUM_OFFSET 22
    #define SF_MENU_NR 1
    #define SF_BOOT_NR 1
    #define SF_SHOT_NR 8
    #define SF_STUN_NR 1
    #define SF_OVERHEAT_NR 1
    #define SF_CLIPIN_NR 1
    #define SF_CLIPOUT_NR 1
    #define SF_FULL_NR 1
    #define SF_EMPTY_NR 1
    #define SF_RELOAD_NR 1
    #define SF_ARCH_NR 1
    #define SF_ZAP_NR 1
    #define SF_MEGABLAST_NR 1
    #define SF_FLAMETHROWER_NR 1
    #define SF_HUM_NR 1
#endif    
  uint8_t getID() const {
    #ifdef LINKEDLIST
      return this->ID;
    #else
      return 1;
    #endif
    
  }

  const uint16_t getShot(uint8_t offset = 0) {
    #if defined V_MK1 or defined V_MK2 or defined V_MK3 or defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_SHOT_OFFSET + random(0,SF_SHOT_NR);
    #endif
}

  const uint16_t getBoot(uint8_t offset = 0) {
    #if defined V_MK1 or defined V_MK2 or defined V_MK3 or defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_BOOT_OFFSET + random(0,SF_BOOT_NR);
    #endif
  }

  const uint16_t getArch(uint8_t offset = 0) {
    #if defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_ARCH_OFFSET + random(0,SF_ARCH_NR);
    #endif
  }

  const uint16_t getZap(uint8_t offset = 0) {
    #if defined V_MK4 or defined V_MKX
   return NR_CONFIGFOLDERFILES + offset + SF_ZAP_OFFSET + random(0,SF_ZAP_NR);
    #endif
  }
  
  const uint16_t getHum(uint8_t offset = 0) {
    #if defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_HUM_OFFSET + random(0,SF_HUM_NR);
    #endif
  }

  const uint16_t getStun(uint8_t offset = 0) {
    #if defined V_MK1 or defined V_MK2 or defined V_MK3 or defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_STUN_OFFSET + random(0,SF_STUN_NR);
    #endif
  }

  const uint16_t getEmpty(uint8_t offset = 0) {
    #if defined V_MK3 or defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_EMPTY_OFFSET + random(0,SF_EMPTY_NR);
    #endif
  }

  const uint16_t getReload(uint8_t offset = 0) {
    #if defined V_MK3 or defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_RELOAD_OFFSET + random(0,SF_RELOAD_NR);
    #endif
  }

  const uint16_t getMenu(uint8_t offset = 0) {
    #if defined V_MK1 or defined V_MK2 or defined V_MK3 or defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_MENU_OFFSET + random(0,SF_MENU_NR);
    #endif
  }

  const uint16_t getClipin(uint8_t offset = 0) {
    #if defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_CLIPIN_OFFSET + random(0,SF_CLIPIN_NR);
    #endif
  }

  const uint16_t getClipout(uint8_t offset = 0) {
    #if defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_CLIPOUT_OFFSET + random(0,SF_CLIPOUT_NR);
    #endif
  }

  const uint16_t getFull(uint8_t offset = 0) {
    #if defined V_MK3 or defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_FULL_OFFSET + random(0,SF_FULL_NR);
    #endif
  }

  const uint16_t getOverheat(uint8_t offset = 0) {
    #if defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_OVERHEAT_OFFSET + random(0,SF_OVERHEAT_NR);
    #endif
  }

  const uint16_t getMegaBlast(uint8_t offset = 0) {
    #if defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_MEGABLAST_OFFSET + random(0,SF_MEGABLAST_NR);
    #endif
  }

  const uint16_t getFlameThrower(uint8_t offset = 0) {
    #if defined V_MK4 or defined V_MKX
    return NR_CONFIGFOLDERFILES + offset + SF_FLAMETHROWER_OFFSET + random(0,SF_FLAMETHROWER_NR);
    #endif
  }
  
  uint16_t getMegaBlastChargeDuration() const {
    return megablastcharge;
  }

    uint16_t getMegaBlastShotDuration() const {
    return megablastshot;
  }
  
  uint16_t getStunShotDuration() const {
    return stunshot;
  }

  uint16_t getFirethrowerRampDuration() const {
    return fireramp;
  }

  uint16_t getBootDuration() const {
    return poweron;
  }
private:
  uint16_t ID;
  uint16_t megablastcharge;
  uint16_t megablastshot;
  uint16_t stunshot;
  uint16_t fireramp;
  uint16_t poweron;
};

#endif /* SOUNDFONT_H_ */
