#if not defined VARIANTS_H_
#define VARIANTS_H_

//#define V_MK1
//#define V_MK2
#define V_MK3

#ifdef V_MK1
  #define SINGLEBUTTON
  //#define STATUSBAR
  //#define AMMOSWITCH    14 // set ammo switch mode or manual reaload depending on sleep mode exit or power-on reset ammo switch status
  #define CONFIG_VERSION     "L01"
  #define DEEP_SLEEP
  enum ConfigModeSubStatesEnum {CS_VOLUME, CS_SLEEPINIT, CS_LASTMEMBER, CS_DSKINS, CS_SOUNDFONT, CS_BLASTCOLOR, CS_STUNCOLOR, CS_BATTERYLEVEL, CS_PIXELLENGHT_BARRELL, CS_PIXELLENGHT_STATUSBAR, CS_DSKINS}; // never delete CS_LASTMEMBER!!! Needed to calculate number of elements in the enum type!!!#endif
#endif


#ifdef V_MK2
  #define SINGLEBUTTON
  //#define STATUSBAR
  //#define AMMOSWITCH    14 // set ammo switch mode or manual reaload depending on sleep mode exit or power-on reset ammo switch status
  #define CONFIG_VERSION     "L02"
  #define DEEP_SLEEP
  enum ConfigModeSubStatesEnum {CS_BLASTCOLOR, CS_STUNCOLOR,CS_VOLUME, CS_SLEEPINIT, CS_PIXELLENGHT_BARRELL, CS_LASTMEMBER, CS_DSKINS, CS_PIXELLENGHT_STATUSBAR, CS_BATTERYLEVEL, CS_SOUNDFONT}; // never delete CS_LASTMEMBER!!! Needed to calculate number of elements in the enum type!!!#endif
#endif


#ifdef V_MK3
  #define SINGLEBUTTON
  #define STATUSBAR
  #define OLED_DISPLAY
  #if defined OLED_DISPLAY
    #define OLED_STD
  #endif
  #define CONFIG_VERSION     "L03"
  #define BUZZMOTOR  9 // buzz motor on LS3
  #define JUKEBOX
  #define DEEP_SLEEP
  enum ConfigModeSubStatesEnum {CS_DSKINS, CS_BLASTCOLOR, CS_STUNCOLOR, CS_VOLUME, CS_SLEEPINIT, CS_PIXELLENGHT_STATUSBAR, CS_PIXELLENGHT_BARRELL, CS_LASTMEMBER, CS_BATTERYLEVEL, CS_SOUNDFONT}; // never delete CS_LASTMEMBER!!! Needed to calculate number of elements in the enum type!!!#endif
#endif

#endif /* VARIANTS_H_ */
