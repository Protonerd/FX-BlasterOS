/*
 * FX-BlasterOS V1.1
 *
 * released on: 01.01.2017
 * author: 		Andras Kun (kun.andras@yahoo.de)
 * Source :   https://github.com/Protonerd/FX-BlasterOS
 * Description:	Operating System for Arduino based prop Blasters
 *
*/
#include <Arduino.h>
#include <DFPlayer.h>
#include <I2Cdev.h>
#include <OneButton.h>
#include <EEPROMex.h>
#include <Adafruit_GFX.h>
#include <avr/wdt.h>

#include "Buttons.h"
#include "Config_HW.h"
#include "Config_SW.h"
#include "ConfigMenu.h"
#include "OLED.h"
#include "Light.h"
#include "SoundFont.h"




#ifdef JUKEBOX
bool jukebox_play=false; // indicate whether a song is being played in JukeBox mode
uint8_t jb_track;  // sound file track number in the directory designated for music playback
#endif

/***************************************************************************************************/


uint8_t AmmoCnt=20;

#ifdef AMMOSWITCH
bool ammoInserted;
#endif

unsigned long timetracker1 = millis();

/***************************************************************************************************
 * LED String variables
 */

extern BlasterStateEnum BlasterState;
extern BlasterStateEnum PrevBlasterState;

extern ActionModeSubStatesEnum ActionModeSubStates;
extern ActionModeSubStatesEnum PrevActionModeSubStates;
extern ConfigModeSubStatesEnum ConfigModeSubStates;
extern DisplaySkins_Enum DisplaySkins;

#ifdef FLAMETHROWER
extern void FireBlade(uint8_t DominantColor=0);
#endif

uint8_t ledPins[] = {LS1}; // redefine variable to hold only LS1, as LS2 and LS3 might be used for different purposes
#ifdef STATUSBAR
  WS2812 pixels(MAX_BARREL_PIXEL+MAX_STATUSBAR_PIXEL);
#else
  WS2812 pixels(MAX_BARREL_PIXEL);
#endif
//cRGB color;
cRGB currentColor;

#ifdef DEEP_SLEEP
  unsigned long sleepTimer = millis();
#endif
/***************************************************************************************************
 * Buttons variables
 */
OneButton mainButton(MAIN_BUTTON, true);
#ifndef SINGLEBUTTON
  OneButton auxButton(AUX_BUTTON, true);
#endif
/***************************************************************************************************
 * DFPLAYER variables
 */
DFPlayer dfplayer;
SoundFont soundFont;


/***************************************************************************************************
 * ConfigMode Variables
 */
int8_t modification = 0;
int16_t value = 0;
//uint8_t menu = 0;
bool enterMenu = false;
bool changeMenu = false;
bool play = false;
unsigned int configAdress = 0;

int16_t sAngle;
uint8_t StatBarAnimPos=0;

#ifdef OLED_DISPLAY
  #define OLED_RESET 4
  #if defined OLED_SCOPE

    Adafruit_SSD1306 display(OLED_RESET);
  #endif
  #if defined OLED_STD

    //Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    Adafruit_SSD1306 display(OLED_RESET);
  #endif
#endif

struct StoreStruct {
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
// ===        	       	   			SETUP ROUTINE  	 	                			===
// ====================================================================================
void setup() {

  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz). Comment this line if having compilation difficulties with TWBR.
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
#endif
  // Serial line for debug
  Serial.begin(115200);

  Disable_FTDI(false);
  Disable_MP3(false);
  
  /***** LOAD CONFIG *****/
  // Get config from EEPROM if there is one
  // or initialise value with default ones set in StoreStruct
  EEPROM.setMemPool(MEMORYBASE, EEPROMSizeATmega328); //Set memorypool base to 32, assume Arduino Uno board
  configAdress = EEPROM.getAddress(sizeof(StoreStruct)); // Size of config object

Serial.print("size of StoreStruct: ");Serial.println(sizeof(StoreStruct));
Serial.println(configAdress);

  if (!loadConfig()) {
    //DumpConfigEEPROM();
    //VirginateConfigBlock();
    for (uint8_t i = 0; i <= 2; i++) {
      storage.version[i] = CONFIG_VERSION[i];
    }
      storage.soundFont = 0;
      storage.volume = VOL;
      storage.NrBarellPixels=4;
      storage.NrStatusBarPixels=4;
    for (uint8_t i=0; i<SOUNDFONT_QUANTITY;i++){
      storage.sndProfile[i].blastColor.r=20;
      storage.sndProfile[i].blastColor.g=0;
      storage.sndProfile[i].blastColor.b=0;
      storage.sndProfile[i].stunColor.r=0;
      storage.sndProfile[i].stunColor.g=0;
      storage.sndProfile[i].stunColor.b=20;
      storage.sndProfile[i].DisplaySkins=0;
    }

    saveConfig();
    #if defined BL_INFO
    		Serial.println(F("DEFAULT VALUE"));
    #endif
    	}
#if defined BL_INFO
	else {
		Serial.println(F("EEPROM LOADED"));
	}
#endif
  
  // retreive the sound font ID stored in the EEPROM (last configured)
  soundFont.setID(storage.soundFont);

/* CONFIG ITEMS PRESETS */
/* Set default values to parameters which can be modified in config menu, if the corresponding config menu item is disabled */  
  if (CS_BLASTCOLOR > CS_LASTMEMBER) {
    for (uint8_t i=0; i<SOUNDFONT_QUANTITY;i++){
      storage.sndProfile[i].blastColor.r=MAX_BRIGHTNESS;
      storage.sndProfile[i].blastColor.g=0;
      storage.sndProfile[i].blastColor.b=0;    
    }
  }
  if (CS_STUNCOLOR > CS_LASTMEMBER) {  
    for (uint8_t i=0; i<SOUNDFONT_QUANTITY;i++){
      storage.sndProfile[i].stunColor.r=0;
      storage.sndProfile[i].stunColor.g=30;
      storage.sndProfile[i].stunColor.b=MAX_BRIGHTNESS;    
    }
  }
  if (CS_VOLUME > CS_LASTMEMBER) {
    storage.volume=31;
  } 

  if (CS_PIXELLENGHT_STATUSBAR > CS_LASTMEMBER) {
    storage.NrStatusBarPixels=0; // no status bar
  } 

  if (CS_PIXELLENGHT_BARRELL > CS_LASTMEMBER) {
    storage.NrBarellPixels=1; // only nozzle light
  }

  if (CS_SOUNDFONT > CS_LASTMEMBER) {
    storage.soundFont = 0; // only one sound font
  } 

  if (CS_DSKINS > CS_LASTMEMBER) {
      for (uint8_t i=0; i<SOUNDFONT_QUANTITY;i++){
        storage.sndProfile[i].DisplaySkins=0;
      }
  }
  
  // enable watchdog to avoid system hang
  wdt_reset();
  wdt_enable(WDTO_8S);
  //WDTCSR = (1<<WDCE) | (1<<WDE) | (1<<WDP3) | (1<<WDP0);
  wdt_reset(); 

	/***** LED SEGMENT INITIALISATION  *****/

	// initialize ledstrings segments
	DDRD |= B01101000;
	DDRB |= B00101110;

	//We shut off all pins that could wearing leds,just to be sure
	PORTD &= B10010111;
	PORTB &= B11010001;

	pixels.setOutput(DATA_PIN); // This initializes the NeoPixel library.
  pixelblade_KillKey_Enable();
#if defined BL_DEBUG
  Serial.print(F("Sound Font ID: "));Serial.println(soundFont.getID());
#endif  
  getColor(storage.sndProfile[storage.soundFont].blastColor);
  pixelblade_KillKey_Enable();


  
	//Randomize randomness (no really that's what it does)
	randomSeed(analogRead(2));

	/***** LED SEGMENT INITIALISATION  *****/

	/***** BUTTONS INITIALISATION  *****/
 pinMode(MAIN_BUTTON,INPUT_PULLUP);
  #ifndef SINGLEBUTTON
  pinMode(AUX_BUTTON,INPUT_PULLUP);
  #endif
  // link the Main button functions.
  mainButton.setClickTicks(CLICK);
  mainButton.setPressTicks(PRESS_MAIN);
  mainButton.attachClick(mainClick);
#ifdef SINGLEBUTTON
  //mainButton.attachDoubleClick(mainDoubleClick);
#endif
  mainButton.attachLongPressStart(mainLongPressStart);
  mainButton.attachLongPressStop(mainLongPressStop);
  mainButton.attachDuringLongPress(mainLongPress);

#ifndef SINGLEBUTTON
  auxButton.setClickTicks(CLICK);
  auxButton.setPressTicks(PRESS_AUX);
  auxButton.attachClick(auxClick);
  auxButton.attachDoubleClick(auxDoubleClick);
  auxButton.attachLongPressStart(auxLongPressStart);
  auxButton.attachLongPressStop(auxLongPressStop);
  //auxButton.attachDuringLongPress(auxLongPress);
#endif
	/***** BUTTONS INITIALISATION  *****/
 #ifdef ACCENT_LIGHT
  digitalWrite(ACCENT_LIGHT,HIGH);
 #endif


  #ifdef DEEP_SLEEP
   /************ DEEP_SLEEP MODE SETTINGS **********/
  pinMode(MP3_PSWITCH, OUTPUT);
  pinMode(FTDI_PSWITCH, OUTPUT);
  digitalWrite(MP3_PSWITCH, LOW); // enable MP3 player
  digitalWrite(FTDI_PSWITCH, LOW); // enable FTDI player
  // pin change interrupt masks (see below list)
  PCMSK2 |= bit (PCINT20);   // pin 4 Aux button
  PCMSK0 |= bit (PCINT4);    // pin 12 Main button
  delay(300);
#endif

  /***** DF PLAYER INITIALISATION  *****/
  InitDFPlayer();
  delay(200);
  if (storage.volume <= 15) {
    Set_Volume(15);
    delay(200);
  }

  /***** DF PLAYER INITIALISATION  *****/

  //setup finished. Boot ready. We notify !
  SinglePlay_Sound(10);
  delay(850);
  /****** INIT BLASTER STATE VARIABLE *****/
  BlasterState = S_STANDBY;
  PrevBlasterState=S_SLEEP;
  ActionModeSubStates=AS_BLASTER_MODE;

  #ifdef DEEP_SLEEP
    sleepTimer = millis();
  #endif
  
#ifdef AMMOSWITCH
  pinMode(AMMOSWITCH,INPUT_PULLUP);
  if (digitalRead(AMMOSWITCH)==HIGH) {
    ammoInserted=false;
  }
  else { // digitalRead(AMMOSWITCH)==LOW
    ammoInserted=true;
  }
#endif

  Set_Volume(storage.volume);
  delay(1000); // wait for the DFPlayer to start its 3.3V LDO
  /***** OLED DISPLAY INITIALISATION  *****/
  // if the OLED is supplied by the LDO of the DFPlayer, leave time for the DFPlayer to power up and start its LDO
  #ifdef OLED_DISPLAY
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64), obtained using the I2C_Scanner Sketch (http://playground.arduino.cc/Main/I2cScanner)
    // init done
    // Display start screen
    DisplayStartScreen();
  #endif
  /***** OLED DISPLAY INITIALISATION  *****/
}

// ====================================================================================
// ===               	   			LOOP ROUTINE  	 	                			===
// ====================================================================================
void loop() {

  // pat the dog
  wdt_reset(); // do not remove!!!
  
	mainButton.tick();
#ifndef SINGLEBUTTON
  auxButton.tick();
#endif

//Serial.println(ActionModeSubStates);
  /*//////////////////////////////////////////////////////////////////////////////////////////////////////////
   * ACTION MODE HANDLER
   */ /////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (BlasterState == S_BLASTERON) {
    //Serial.println(timetracker1);
   #if defined OLED_SCOPE
   if (sAngle==360) {
      sAngle=0;
    }
    sAngle=sAngle+5;
    DisplayAmmoCount(AmmoCnt,sAngle);
    #endif
    
    if (PrevBlasterState==S_STANDBY) { // turning on blaster
       pixelblade_KillKey_Disable(); // enable the neopixel LEDs by connecting their GND to the board GND via the Low-side drivers (programmable kill-key)
       SinglePlay_Sound(soundFont.getBoot((storage.soundFont)*NR_FILE_SF));
       #ifdef ACCENT_LIGHT
        digitalWrite(ACCENT_LIGHT,HIGH);
       #endif
       PrevBlasterState=S_BLASTERON;
       lightOff(storage.NrStatusBarPixels+1,storage.NrStatusBarPixels + storage.NrBarellPixels);
       AmmoCnt=MAX_AMMO; // start action mode with fully loaded magazine
       // restart the activity counter
       timetracker1=millis();
       StatBarAnimPos=0;
#ifdef OLED_DISPLAY
       DisplayBlasterOnFrames();
       DisplayAmmoCount(AmmoCnt,sAngle);
       #if defined OLED_STD
        DisplayBlasterStatus("PWRON  ", false);
       #endif
#endif       
  MonitorBattery();
  delay(soundFont.getBootDuration());
  StatusBar_ModeChange(1);
    }
// ********************************************************************************
#ifdef AMMOSWITCH
    else if (digitalRead(AMMOSWITCH)==HIGH and ammoInserted==false and millis()-timetracker1 > AMMOCLIPMINDURATION and ActionModeSubStates!=AS_CLIPOUT and ActionModeSubStates!=AS_SWITCHOFF) {
        //Serial.println("Clip out sensed");
        ActionModeSubStates=AS_CLIPOUT; //AS_EMPTY;
        SinglePlay_Sound(soundFont.getEmpty((storage.soundFont)*NR_FILE_SF));
        //delay(500);
        StatusBar_RampAmmo(AmmoCnt,false,100);

        AmmoCnt=0;       
        DisplayAmmoCount(AmmoCnt,sAngle);
        #if defined OLED_STD
          DisplayBlasterStatus("CLIPOUT");
        #endif
    }
    else if (digitalRead(AMMOSWITCH)==HIGH and ammoInserted==true and ActionModeSubStates!=AS_CLIPOUT and ActionModeSubStates!=AS_SWITCHOFF) {  // ammo magazine un-clipped
    //else if (digitalRead(AMMOSWITCH)==HIGH and ammoInserted==true) {  // ammo magazine un-clipped
      // recognize empty ammo only if state lasts longer as AMMOCLIPMINDURATION
        timetracker1=millis();  // store time stamp of unclip
        ammoInserted=false; // assume valid unclip and set variable
        //Serial.println("Clip out start");
    }
    else if (digitalRead(AMMOSWITCH)==LOW and ActionModeSubStates==AS_CLIPOUT and ammoInserted==true and millis()-timetracker1 > AMMOCLIPMINDURATION) {
        //Serial.println("Clip in sensed");
        SinglePlay_Sound(soundFont.getFull((storage.soundFont)*NR_FILE_SF));
        StatusBar_RampAmmo(0,true,100);
        AmmoCnt=MAX_AMMO;
        //ActionModeSubStates=AS_FULL;
#ifdef OLED_DISPLAY
        DisplayAmmoCount(AmmoCnt,sAngle);
        //DisplayBlasterStatus("FULL   ");
#endif
        //delay(500);
        ActionModeSubStates=AS_BLASTER_MODE;
    }
    else if (digitalRead(AMMOSWITCH)==LOW and ActionModeSubStates==AS_CLIPOUT and ammoInserted==false) {  // ammo magazine clipped-in
      // recognize clip-in ammo magazine only if state lasts longer as AMMOCLIPMINDURATION
        timetracker1=millis();  // store time stamp of unclip
        ammoInserted=true; // assume valid clip and set variable
        //Serial.println("Clip in start");
    }

#endif  // AMMOSWITCH
// ********************************************************************************
    else { // 
      if (ActionModeSubStates == AS_BLAST) {
        #if defined BL_DEBUG
              Serial.println(F("Single Blaster Shot"));
        #endif
        getColor(storage.sndProfile[storage.soundFont].blastColor);
        //StatusBarLightOn(currentColor);
        SinglePlay_Sound(soundFont.getShot((storage.soundFont)*NR_FILE_SF));
  
        FX_SingleShot(300);
        AmmoCnt=AmmoCnt-1;
  
        #ifdef OLED_DISPLAY
              DisplayAmmoCount(AmmoCnt,sAngle);
              //DisplayBlasterStatus("BLASTER");
        #endif
        StatusBar_Ammo(AmmoCnt);
  
        if (AmmoCnt==0) {
          ActionModeSubStates=AS_EMPTY;
          PrevActionModeSubStates=AS_BLASTER_MODE;
          #if defined OLED_STD
            DisplayBlasterStatus("EMPTY  ", false);
          #endif
          //SinglePlay_Sound(soundFont.getEmpty((storage.soundFont)*NR_FILE_SF));
          //delay(500);
        }
        else {
          ActionModeSubStates=AS_BLASTER_MODE;
        }
      }
      else if (ActionModeSubStates == AS_REPEATER) {
        #if defined BL_DEBUG
              Serial.println(F("Repeater Blaster Shot"));
        #endif
        getColor(storage.sndProfile[storage.soundFont].blastColor);
        SinglePlay_Sound(soundFont.getShot((storage.soundFont)*NR_FILE_SF));
  
        FX_SingleShot(200);
        AmmoCnt=AmmoCnt-1;
  
        #ifdef OLED_DISPLAY
              DisplayAmmoCount(AmmoCnt,sAngle);
              //DisplayBlasterStatus("DAKKA  ");
        #endif
        StatusBar_Ammo(AmmoCnt);
  
        if (AmmoCnt==0) {
          ActionModeSubStates=AS_EMPTY;
          #ifdef SINGLEBUTTON
          PrevActionModeSubStates=AS_REPEATER_MODE;
          #endif
          #ifndef SINGLEBUTTON
          PrevActionModeSubStates=AS_BLASTER_MODE;
          #endif
          #if defined OLED_STD
            DisplayBlasterStatus("EMPTY  ", false);
          #endif
          //SinglePlay_Sound(soundFont.getEmpty((storage.soundFont)*NR_FILE_SF));
          //delay(500);
        }
      }
    else if (ActionModeSubStates==AS_STUN) {
        #if defined BL_DEBUG
              Serial.println(F("Stun Shot"));
        #endif
        getColor(storage.sndProfile[storage.soundFont].stunColor);
        SinglePlay_Sound(soundFont.getStun((storage.soundFont)*NR_FILE_SF));
        AmmoCnt=AmmoCnt-1;
        FX_SingleStun();
        StatusBar_Ammo(AmmoCnt);
          #ifdef OLED_DISPLAY
                DisplayAmmoCount(AmmoCnt,sAngle);
                //DisplayBlasterStatus("STUN   ");
          #endif
  
  
  
        if (AmmoCnt==0) {
          ActionModeSubStates=AS_EMPTY;
          #if defined OLED_STD
            DisplayBlasterStatus("EMPTY  ", false);
          #endif
          PrevActionModeSubStates=AS_STUN_MODE;
          //SinglePlay_Sound(soundFont.getEmpty((storage.soundFont)*NR_FILE_SF));
          //delay(500);
        }
        else {
          ActionModeSubStates=AS_STUN_MODE;
        }
     }
    else if (ActionModeSubStates==AS_MEGABLAST) {
        #if defined BL_DEBUG
              Serial.println(F("Mega Blast Shot"));
        #endif
        getColor(storage.sndProfile[storage.soundFont].blastColor);
        SinglePlay_Sound(soundFont.getMegaBlast((storage.soundFont)*NR_FILE_SF));
        AmmoCnt=constrain(AmmoCnt-5,0,MAX_AMMO);
        FX_ChargeAndShoot();
        StatusBar_Ammo(AmmoCnt);
        #ifdef OLED_DISPLAY
              DisplayAmmoCount(AmmoCnt,sAngle);
              //DisplayBlasterStatus("MEGABL ");
        #endif
        if (AmmoCnt==0) {
          PrevActionModeSubStates=AS_MEGABLAST_MODE;
          ActionModeSubStates=AS_EMPTY;
          #if defined OLED_STD
            DisplayBlasterStatus("EMPTY  ", false);
          #endif
          //SinglePlay_Sound(soundFont.getEmpty((storage.soundFont)*NR_FILE_SF));
          //delay(500);
        }
        else {
          ActionModeSubStates=AS_MEGABLAST_MODE;
        }
     }
    #ifdef FLAMETHROWER
    else if (ActionModeSubStates==AS_FLAMETHROWER) {
        #if defined BL_DEBUG
              Serial.println(F("Flamethrower fired"));
        #endif
        FX_Firethrower();
        if (AmmoCnt==0) {
          #ifdef SINGLEBUTTON
          PrevActionModeSubStates=AS_FLAMETHROWER_MODE;
          #endif
          #ifndef SINGLEBUTTON
          PrevActionModeSubStates=AS_MEGABLAST_MODE;
          #endif
          ActionModeSubStates=AS_EMPTY;
          #if defined OLED_STD
            DisplayBlasterStatus("EMPTY  ", false);
          #endif
          //SinglePlay_Sound(soundFont.getEmpty((storage.soundFont)*NR_FILE_SF));
          //delay(500);
        }
        else {
          ActionModeSubStates=AS_FLAMETHROWER;
        }
     }
     #endif // FLAMETHROWER
   else if (ActionModeSubStates==AS_PLASMABLAST) {
    //uint8_t temp_flick_l;
    //temp_flick_l=random(storage.NrBarellPixels/2);
    FX_barellFlicker(storage.sndProfile[storage.soundFont].stunColor, true, storage.NrStatusBarPixels+random(storage.NrBarellPixels/2), storage.NrStatusBarPixels + storage.NrBarellPixels-random(storage.NrBarellPixels/2));
   }
   else if (ActionModeSubStates==AS_PROFILECHANGE) {
    // load back the sub state from which we entered into the profile change mode
    ActionModeSubStates=PrevActionModeSubStates;
   }
   else if (ActionModeSubStates==AS_EMPTY) {
      #ifdef OLED_DISPLAY
              DisplayAmmoCount(AmmoCnt,sAngle);
              //DisplayBlasterStatus("EMPTY  ", false);
      #endif    
      }
    else if (ActionModeSubStates==AS_SWITCHOFF) {
      MonitorBattery();
    }
   else if (ActionModeSubStates==AS_MANUALRELOAD) {
    #ifdef AMMOSWITCH
        if (digitalRead(AMMOSWITCH)==LOW) {  // only allow manual reload, if ammo magazine is in
    #endif
    AmmoCnt=constrain(AmmoCnt+1,0,MAX_AMMO);
    #ifdef OLED_DISPLAY
        DisplayAmmoCount(AmmoCnt,sAngle);
        //DisplayBlasterStatus("RELOAD ", false);
    #endif
    StatusBar_Ammo(AmmoCnt);
    delay(100);
    if (AmmoCnt==MAX_AMMO) {
      SinglePlay_Sound(soundFont.getFull((storage.soundFont)*NR_FILE_SF));
      delay(500);
      ActionModeSubStates=AS_FULL;
      //DisplayBlasterStatus("FULL   ", false);
      #if defined SINGLEBUTTON
        ActionModeSubStates=PrevActionModeSubStates;
        PrevActionModeSubStates=AS_MANUALRELOAD;
      #endif
    }
#ifdef AMMOSWITCH
    }
#endif
   }
    }
  // Status Bar Animation
  if (millis()>timetracker1 and millis()-timetracker1 > STATBARANIM_START) {
    //for (uint8_t dumpi =0; dumpi<sizeof(StatusBar_Animation);dumpi++) {
    //  Serial.println(pgm_read_word_near(StatusBar_Animation + dumpi));
    //}
    //Serial.println("Start Status Bar Animation");
    // check which round it is
    if ((millis()-timetracker1) > (((StatBarAnimPos-1)*STARBARANIM_PERIOD)+STATBARANIM_START) ) {
      lightOff(0, storage.NrBarellPixels-1);
      StatBarAnimPos++;
      Serial.println(sizeof(StatusBar_Animation));
      if (StatBarAnimPos==sizeof(StatusBar_Animation)/2) { // because the size is in bytes and the values in the PROGMEM defined as 16bit chars
        StatBarAnimPos=0;
        timetracker1=millis()-STATBARANIM_START;
      }
      for (uint8_t sba =0; sba<storage.NrStatusBarPixels;sba++) {
          //Serial.print(StatBarAnimPos);Serial.print("\t");Serial.print(sba);Serial.print("\t");Serial.println((uint16_t)StatusBar_Animation[StatBarAnimPos], HEX);
        if ((pgm_read_word_near(StatusBar_Animation + StatBarAnimPos)&(1<<sba))) {
          //lightOn(storage.sndProfile[storage.soundFont].blastColor,sba,sba);
          lightOn({1,0,0},sba,sba);
        }
      }
    }
  }

  } ////END ACTION MODE HANDLER///////////////////////////////////////////////////////////////////////////////////////
	/*//////////////////////////////////////////////////////////////////////////////////////////////////////////
	 * CONFIG MODE HANDLER
	 *//////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (BlasterState==S_CONFIG) {
    if (PrevBlasterState==S_STANDBY) { // entering config mode
      PrevBlasterState=S_CONFIG;
      AmmoCnt=MAX_AMMO;
			SinglePlay_Sound(3);
			delay(600);
      #if defined BL_INFO
      			Serial.println(F("START CONF"));
      #endif
			//browsing = true;
			enterMenu = true;
      ConfigModeSubStates=-1;
      NextConfigState();
    }

/*
// CS_SOUNDFONT, CS_SLEEPINIT, CS_BLASTCOLOR, CS_STUNCOLOR, CS_LASTMEMBER, CS_VOLUME, CS_BATTERYLEVEL, CS_STORAGEACCESS, CS_UARTMODE
		switch (ConfigModeSubStatesEnum) {
 		case CS_SOUNDFONT: // SOUNDFONT
			break;      
    case CS_BLASTCOLOR: // BLASTER MAIN COLOR
      break;
    case CS_STUNCOLOR: //STUN COLOR
      break;
    case CS_VOLUME: //VOLUME
      break;
		}
   */
	} //END CONFIG MODE HANDLER

	/*//////////////////////////////////////////////////////////////////////////////////////////////////////////
	 * STANDBY MODE
	 *//////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (BlasterState==S_STANDBY) {

		if (PrevBlasterState==S_BLASTERON) { // we just leaved Blaster On
      #if defined DEEP_SLEEP
        sleepTimer=millis(); // reset sleep time counter
      #endif
			//SinglePlay_Sound(soundFont.getMenu((storage.soundFont)*NR_FILE_SF));
       #ifdef ACCENT_LIGHT
        digitalWrite(ACCENT_LIGHT,LOW);
      #endif
      PrevBlasterState=S_STANDBY;
      ActionModeSubStates=AS_BLASTER_MODE;
#ifdef OLED_DISPLAY
      DisplayStartScreen();
#endif
      delay(500);
			changeMenu = false;
			modification = 0;

#if defined BL_INFO
			Serial.println(F("Enter Stand-by Mode, Blaster turned off"));
#endif
#if defined NEOPIXEL
  pixelblade_KillKey_Enable();
#endif

		}
		//if (browsing) { // we just leaved Config Mode
   if (PrevBlasterState==S_CONFIG) { // we just leaved Config Mode
			saveConfig();
      DisplayStartScreen();
      PrevBlasterState=S_STANDBY;
      #if defined DEEP_SLEEP
        sleepTimer=millis(); // reset sleep time counter
      #endif
			/*
			 * RESET CONFIG
			 */
//			for (unsigned int i = 0; i < EEPROMSizeATmega328; i++) {
//				//			 if (EEPROM.read(i) != 0) {
//				EEPROM.update(i, 0);
//				//			 }
//			}

			lightOff();

			SinglePlay_Sound(soundFont.getMenu((storage.soundFont)*NR_FILE_SF));
			//browsing = false;
			enterMenu = false;
			modification = 0;
			//dfplayer.setVolume(soundFont.getVolume());
      delay(300);
			//menu = 0;
#if defined LUXEON
			//getColor(currentColor, storage.mainColor);
#endif
#if defined NEOPIXEL
			//getColor(storage.sndProfile[storage.soundFont].mainColor);
#endif

#if defined BL_INFO
			Serial.println(F("END CONF"));
#endif
		}
    #if defined DEEP_SLEEP and SLEEPYTIME>5000
      if (millis() - sleepTimer > SLEEPYTIME) { // after the defined max idle time SLEEPYTIME automatically go to sleep mode
        BlasterState=S_SLEEP;
        PrevBlasterState=S_STANDBY;
        // play a beep 3 times
        SinglePlay_Sound(1);
        delay(500);
        SinglePlay_Sound(1);
        delay(500);    
        SinglePlay_Sound(1);
        delay(500);
      }
    #endif
	} // END STANDBY MODE

#ifdef JUKEBOX
  /*//////////////////////////////////////////////////////////////////////////////////////////////////////////
   * JUKEBOX MODE (a.k.a. MP3 player mode
   *//////////////////////////////////////////////////////////////////////////////////////////////////////////

  else if (BlasterState==S_JUKEBOX) {
    if (PrevBlasterState==S_STANDBY) {  // just entered JukeBox mode
      PrevBlasterState=S_JUKEBOX;
      SinglePlay_Sound(11);  // play intro sound of JukeBox mode
      delay(1500);
#if defined LS_INFO
            Serial.println(F("START JUKEBOX"));
#endif       
      // start playing the first song
      jb_track=NR_CONFIGFOLDERFILES+(SOUNDFONT_QUANTITY*NR_FILE_SF)+1;
      SinglePlay_Sound(jb_track);  // JukeBox dir/files must be directly adjecent to config sounds on the SD card
    }
    else if (PrevBlasterState==S_JUKEBOX) { // JukeBox already running
      JukeBox_Strobe();
      delay(100);
    }
  }
#endif // JUKEBOX
#ifdef DEEP_SLEEP
  else if (BlasterState == S_SLEEP) {
    pixelblade_KillKey_Enable();
     #ifdef ACCENT_LIGHT
      digitalWrite(ACCENT_LIGHT,LOW); // switch off accent LED
    #endif
    //if (PrevSaberState == S_CONFIG or ) { // just entered Sleep mode
      //byte old_ADCSRA = ADCSRA;
      // disable ADC to save power
      // disable ADC

      // repeat the interupt mask again here, it is already done in the setup() function
      // but for an unknown reason sometimes the device fails to wake up...
      // pin change interrupt masks (see below list)
      //PCMSK2 |= bit (PCINT20);   // pin 4 Aux button
      //PCMSK0 |= bit (PCINT4);    // pin 12 Main button
      //delay(300);
      //Serial.println("LoL");
      ADCSRA = 0;  // reduces another ~100uA!
      // disable watchdog before going into sleep mode
      wdt_disable(); // do not remove!!!
      SleepModeEntry();
      // .. and the code will continue from here

      SleepModeExit();
      BlasterState = S_STANDBY;
      PrevBlasterState = S_SLEEP;
      ADCSRA = 135; // old_ADCSRA;   // re-enable ADC conversion
    //}
  }
#endif // DEEP_SLEEP
} //loop



// ====================================================================================
// ===           	  			EEPROM MANIPULATION FUNCTIONS	            		===
// ====================================================================================

inline bool loadConfig() {
  bool equals = true;
  EEPROM.readBlock(configAdress, storage);
  for (uint8_t i = 0; i <= 2; i++) {
    if (storage.version[i] != CONFIG_VERSION[i]) {
      equals = false;
      Serial.println("Wrong config!");
    }
  }
  Serial.println(storage.version);
  //Serial.print("ADC status:"); Serial.println(ADCSRA);
  return equals;
} //loadConfig

inline void saveConfig() {
  EEPROM.updateBlock(configAdress, storage);

} //saveConfig

void VirginateConfigBlock() {
  for (uint8_t i = configAdress; i < configAdress+sizeof(StoreStruct); i++) {
    EEPROM.writeByte(i,0);
  }
}

void DumpConfigEEPROM() {
//#ifdef LS_DEBUG
  // dump values stored in EEPROM
  for (uint8_t i = configAdress; i < configAdress+sizeof(StoreStruct); i++) {
    Serial.print(i); Serial.print("\t"); Serial.println(EEPROM.readByte(i));
  }
//#endif
}

// ====================================================================================
// ===                          SOUND FUNCTIONS                                     ===
// ====================================================================================

void InitDFPlayer() {
  dfplayer.setSerial(DFPLAYER_TX, DFPLAYER_RX);
}

void SinglePlay_Sound(uint8_t track) {
  dfplayer.playPhysicalTrack(track);
}

void LoopPlay_Sound(uint8_t track) {
  dfplayer.playSingleLoop(track);
}

void Set_Loop_Playback() {
  dfplayer.setSingleLoop(true);
}

void Pause_Sound() {
  dfplayer.pause();
}

void Resume_Sound() {
  dfplayer.play();
}

void Set_Volume(int8_t volumeSet) {
  dfplayer.setVolume(volumeSet); // Too Slow: we'll change volume on exit
  delay(50);
}

#ifdef DEEP_SLEEP
// ====================================================================================
// ===                          SLEEP MODE FUNCTIONS                                ===
// ====================================================================================

void sleepNow()         // here we put the arduino to sleep
{

    power_all_disable ();   // turn off all modules -> no measurable effect
     
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin 

    // turn off brown-out enable in software -> no measurable effect
    MCUCR = bit (BODS) | bit (BODSE);
    MCUCR = bit (BODS); 
  
    PCIFR  |= bit (PCIF0) | bit (PCIF1) | bit (PCIF2);   // clear any outstanding interrupts
    PCICR  |= bit (PCIE0) | bit (PCIE1) | bit (PCIE2);   // enable pin change interrupts

    //PCMSK0 |= bit (PCINT4);    // enable pin change interrupt pin 12 Main button
    //delay(300);
    //enableInterrupt(12, SleepModeExit, CHANGE);
    
    sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP


  
    sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
    detachInterrupt(0);      // disables interrupt 0 on pin 2 so the 
                             // wakeUpNow code will not be executed 
                             // during normal running time.

                             // enable watchdog to avoid system hang
}

void SleepModeEntry() {
  // switch off all LS channels
  for (uint8_t i = 0; i < sizeof(ledPins); i++) {
    digitalWrite(ledPins[i], LOW);
  }
  // pin change interrupt masks (see below list)
  //PCMSK2 |= bit (PCINT20);   // pin 4 Aux button
  PCMSK0 |= bit (PCINT4);    // pin 12 Main button
  // contrain the communication signals to the MP3 player to low, otherwise they will back-supply the module
  pinMode(DFPLAYER_RX, OUTPUT);
  digitalWrite(DFPLAYER_RX, LOW);
  pinMode(DFPLAYER_TX, OUTPUT);
  digitalWrite(DFPLAYER_TX, LOW);
  Disable_MP3(true);
  //mpu.setSleepEnabled(true); // included as dummy, for an unknown reason if it's not here and the dfplayer.sleep() is commented out, sound is disabled
  //dfplayer.sleep();
  delay (300);
  Disable_FTDI(true);
  sleepNow();     // sleep function called here
}

void SleepModeExit() {

  // cancel sleep as a precaution
  sleep_disable();
  power_all_enable ();   // enable modules again
  Disable_FTDI(false);
  Disable_MP3(false);
  pinMode(DFPLAYER_RX, OUTPUT);
  pinMode(DFPLAYER_TX, INPUT);
  
  // enable watchdog to avoid system hang
  wdt_reset();
  wdt_enable(WDTO_8S);
  //WDTCSR = (1<<WDCE) | (1<<WDE) | (1<<WDP3) | (1<<WDP0);
  wdt_reset(); 
  //digitalWrite(11,HIGH);
  //delay(1000);
  //digitalWrite(11,LOW);
  // MPU init after wake up to avoid system hang
  /*
  delay(300);
  Serial.println("Waking up MPU - who knows, maybe it still sleeps");
  mpu.setSleepEnabled(false);
  Serial.println("Reset MPU I2C Master");
mpu.resetI2CMaster();
Serial.println("Reseting MPU");
  mpu.reset();
  delay(100);
Serial.println("Initializing MPU");
  mpu.initialize();
  */
  setup(); // redo all initializations
}


#endif // DEEP_SLEEP

void Disable_FTDI(bool ftdi_off) {

  if (ftdi_off) {  //  disable FTDI
    digitalWrite(FTDI_PSWITCH, HIGH); // disable the FTDI chip

  }
  else {  //  enable ftdi
    digitalWrite(FTDI_PSWITCH, LOW); // enable the FTDI chip
  }
  
}

void Disable_MP3(bool mp3_off) {

  if (mp3_off) {  //  disable MP3
    digitalWrite(MP3_PSWITCH, HIGH); // disable the MP3 chip and the audio amp
  }
  else {  //  enable MP3
    digitalWrite(MP3_PSWITCH, LOW); // enable the MP3 chip and the audio amp
  }
  
}
// ====================================================================================
// ===                         BATTERY CHECKING FUNCTIONS                           ===
// ====================================================================================


void BatLevel_ConfigEnter() {
      // flush out the ADC
      getBandgap();
      getBandgap();
      getBandgap();
      int batLevel=((getBandgap()/37)*10);
      Serial.println(batLevel);
      if (batLevel > 95) {        //full
        SinglePlay_Sound(19);
      } else if (batLevel > 60) { //nominal
        SinglePlay_Sound(15);
      } else if (batLevel > 30) { //diminished
        SinglePlay_Sound(16);
      } else if (batLevel > 0) {  //low
        SinglePlay_Sound(17);
      } else {                    //critical
        SinglePlay_Sound(18);
      }
      BladeMeter(ledPins, batLevel);
      delay(1000);
}

void MonitorBattery() {
    BladeMeter(ledPins,((getBandgap()-300)/7)*10);
    //BladeMeter(ledPins,(getBandgap()/37)*10);
    //Serial.println(getBandgap());
}
 
// Code courtesy of "Coding Badly" and "Retrolefty" from the Arduino forum
// results are Vcc * 100
// So for example, 5V would be 500.
int getBandgap () 
  {
  // REFS0 : Selects AVcc external reference
  // MUX3 MUX2 MUX1 : Selects 1.1V (VBG)  
   ADMUX = bit (REFS0) | bit (MUX3) | bit (MUX2) | bit (MUX1);
   ADCSRA |= bit( ADSC );  // start conversion
   while (ADCSRA & bit (ADSC))
     { }  // wait for conversion to complete
   int results = (((InternalReferenceVoltage * 1024) / ADC) + 5) / 10; 
   //Serial.print("battery voltage: ");Serial.println(results);
   return results;
  } // end of getBandgap
