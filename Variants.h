#if not defined VARIANTS_H_
#define VARIANTS_H_


#define V_MKX



  //#define SINGLEBUTTON
  #define OLED_DISPLAY
  #if defined OLED_DISPLAY
    //#define OLED_STD
    #define OLED_SCOPE
  #endif 
  #define STATUSBAR
  //#define ACCENT_LIGHT 14
  //#define AMMOSWITCH    14 // set ammo switch mode or manual reaload depending on sleep mode exit or power-on reset ammo switch status
  #define CONFIG_VERSION     "L05"
  #define DEEP_SLEEP
  #define BUZZMOTOR  9 // buzz motor on LS3
  #define JUKEBOX
  #define FLAMETHROWER
  enum ConfigModeSubStatesEnum {CS_SOUNDFONT, CS_DSKINS, CS_BLASTCOLOR, CS_STUNCOLOR, CS_VOLUME, CS_SLEEPINIT, CS_PIXELLENGHT_STATUSBAR, CS_PIXELLENGHT_BARRELL, CS_LASTMEMBER, CS_BATTERYLEVEL}; // never delete CS_LASTMEMBER!!! Needed to calculate number of elements in the enum type!!!#endif
#endif /* VARIANTS_H_ */
