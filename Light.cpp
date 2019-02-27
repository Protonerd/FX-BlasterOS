/*
 * Light.cpp
 *
 * author:     Andras Kun (kun.andras@yahoo.de) based on LSOS by Sebastien CAPOU (neskweek@gmail.com)
 * Source :   https://github.com/Protonerd/FX-BlasterOS
 */
#include "Light.h"
#include "ConfigMenu.h"
#include <DFPlayer.h>
#include "SoundFont.h"

#if defined NEOPIXEL
#include <WS2812.h>
#endif

extern DFPlayer dfplayer;
extern SoundFont soundFont;
extern void SinglePlay_Sound(uint8_t track);
extern void LoopPlay_Sound(uint8_t track);
extern void Pause_Sound();
extern void Resume_Sound();
extern void Set_Loop_Playback();
extern void  Set_Volume(int8_t volumeSet=-1);
// ====================================================================================
// ===              	    			LED FUNCTIONS		                		===
// ====================================================================================

//static uint8_t flickerPos = 0;
//static long lastFlicker = millis();
extern WS2812 pixels;

extern cRGB currentColor;

//uint8_t NozzleFirePixel=0;

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

extern uint8_t ledPins[];

#ifdef FLAMETHROWER
  // COOLING: How much does the air cool as it rises?
  // Less cooling = taller flames.  More cooling = shorter flames.
  // Default 50, suggested range 20-100 
  static uint8_t Fire_Cooling = 150;
  
  // SPARKING: What chance (out of 255) is there that a new spark will be lit?
  // Higher chance = more roaring fire.  Lower chance = more flickery fire.
  // Default 120, suggested range 50-200.
  static uint8_t Fire_Sparking = 150;

  static byte heat[MAX_BARREL_PIXEL];  
#endif

void lightOn(cRGB color, int8_t StartPixel=-1, int8_t StopPixel=-1) {
	// Light On
    if ((StartPixel == -1 and StopPixel==-1) or StopPixel<StartPixel or StartPixel>(MAX_BARREL_PIXEL+MAX_STATUSBAR_PIXEL) or StopPixel>(MAX_BARREL_PIXEL+MAX_STATUSBAR_PIXEL)) {  // if neither start nor stop is defined or invalid range, go through the whole stripe
      for (uint8_t i = 0; i <= MAX_STATUSBAR_PIXEL+MAX_BARREL_PIXEL-1; i++) {
        pixels.set_crgb_at(i, color);
      }
    } 
    else {
        if (StartPixel!=-1) {
          for (uint8_t i = StartPixel; i <= StopPixel; i++) {
            pixels.set_crgb_at(i, color);
          }
        }
      }
    pixels.sync();
} //lightOn

void lightOff(int8_t StartPixel=-1, int8_t StopPixel=-1) {
// shut Off
  cRGB value;
	value.b = 0;
	value.g = 0;
	value.r = 0; // RGB Value -> Off
  if (StartPixel<0 or StopPixel<0) {
    for (uint8_t i = 0; i <= MAX_STATUSBAR_PIXEL+MAX_BARREL_PIXEL-1; i++) {
      pixels.set_crgb_at(i, value);
    }    
  }
  else {
  	for (uint8_t i = StartPixel; i <= StopPixel; i++) {
  		pixels.set_crgb_at(i, value);
  	}
  }
	pixels.sync();
} //lightOff

void StatusBarLightOn(cRGB color) {
cRGB value;
  value.b = color.b/5;
  value.g = color.g/5;
  value.r = color.r/5;
    for (uint8_t i = 0; i < storage.NrStatusBarPixels; i++) {
      pixels.set_crgb_at(i, value);
    }
    pixels.sync();
}

void FX_SingleShot(uint16_t duration) {
  cRGB color;
        #ifdef BUZZMOTOR
        digitalWrite(BUZZMOTOR,HIGH);
        #endif
        for (unsigned int i=storage.NrStatusBarPixels; i < storage.NrStatusBarPixels+storage.NrBarellPixels; i++) { // turn on/off one LED at a time
          for(unsigned int j=storage.NrStatusBarPixels; j<storage.NrStatusBarPixels+storage.NrBarellPixels; j++ ) { // fill up string with data
            if (i!=j) {
                color.r=0;
                color.g=0;
                color.b=0;
                pixels.set_crgb_at(j, color); // Set value at LED found at index j 
            }
            else {
               color.r=currentColor.r;
               color.g=currentColor.g;
               color.b=currentColor.b;
               pixels.set_crgb_at(j, color); // Set value at LED found at index j                
              }
             
            }
            pixels.sync(); // Sends the data to the LEDs
            //delay(10);
            delay(duration/storage.NrBarellPixels);
          }
          lightOff(storage.NrStatusBarPixels + storage.NrBarellPixels-1, storage.NrStatusBarPixels + storage.NrBarellPixels-1);
          #ifdef BUZZMOTOR
          digitalWrite(BUZZMOTOR,LOW);
          #endif
}

void FX_ChargeAndShoot() {
	uint16_t variation;
  cRGB color;
  uint8_t cycle_count=0;
  bool contPlay=true;
  unsigned long starttime;
  
  starttime=millis();
  Serial.println(starttime);
  // Charge phase:
   while (millis() >= starttime and millis()-starttime<=soundFont.getMegaBlastChargeDuration()) {
      //variation = constrain(MAX_BRIGHTNESS - (abs(analogRead(SPK1) - analogRead(SPK2)))/8,0,255);
      variation = constrain((abs(analogRead(SPK1) - analogRead(SPK2)))*31/storage.volume,0,255);
      //Serial.println(variation);
      #ifdef BUZZMOTOR
      digitalWrite(BUZZMOTOR,HIGH);
      #endif
      for(uint8_t j=storage.NrStatusBarPixels; j<storage.NrStatusBarPixels + storage.NrBarellPixels-1; j++ ) { // fill up string with data
          color.r = variation * currentColor.r / 255;
          color.g = variation * currentColor.g / 255;
          color.b = variation * currentColor.b / 255;
          //Serial.print(color.r);Serial.print("\t");Serial.print(color.g);Serial.print("\t");Serial.println(color.b);
        //if (currentColor.r!=0) {color.r=constrain(currentColor.r*(uint8_t)(variation/(100)),20,255);}
        //if (currentColor.g!=0) {color.g=constrain(currentColor.g*(uint8_t)(variation/(100)),20,255);}
        //if (currentColor.b!=0) {color.b=constrain(currentColor.b*(uint8_t)(variation/(100)),20,255);}  
        pixels.set_crgb_at(j, color); // Set value at LED found at index j
      }
    pixels.sync(); // Sends the data to the LEDs
    //currenttime=millis();
   }
   lightOff(storage.NrStatusBarPixels,storage.NrStatusBarPixels + storage.NrBarellPixels);   
   starttime=millis();
   //currenttime=millis();
   // Shooting phase:
     while (millis() >= starttime and millis()-starttime<=soundFont.getMegaBlastShotDuration()) {
        //variation = constrain(MAX_BRIGHTNESS - (abs(analogRead(SPK1) - analogRead(SPK2)))/8,0,255);
        variation = constrain((abs(analogRead(SPK1) - analogRead(SPK2)))*31/storage.volume,0,255);
        #ifdef BUZZMOTOR
        //digitalWrite(BUZZMOTOR,HIGH);
        #endif
        // switch off barrel lights
        //for(uint8_t j=storage.NrStatusBarPixels + storage.NrBarellPixels; j<=storage.NrStatusBarPixels + storage.NrBarellPixels; j++ ) { // fill up string with data
          color.r = variation * currentColor.r / 255;
          color.g = variation * currentColor.g / 255;
          color.b = variation * currentColor.b / 255;
          Serial.print(color.r);Serial.print("\t");Serial.print(color.g);Serial.print("\t");Serial.println(color.b);
          //if (currentColor.r!=0) {color.r=constrain(currentColor.r*(uint8_t)(variation/(100)),20,255);}
          //if (currentColor.g!=0) {color.g=constrain(currentColor.g*(uint8_t)(variation/(100)),20,255);}
          //if (currentColor.b!=0) {color.b=constrain(currentColor.b*(uint8_t)(variation/(100)),20,255);}        
          pixels.set_crgb_at(storage.NrStatusBarPixels + storage.NrBarellPixels-1, color); // Set value at LED found at index j
        //}
      pixels.sync(); // Sends the data to the LEDs

      //currenttime=millis();    
     }
   //}
    lightOff(storage.NrStatusBarPixels + storage.NrBarellPixels-1, storage.NrStatusBarPixels + storage.NrBarellPixels-1);
    #ifdef BUZZMOTOR
    digitalWrite(BUZZMOTOR,LOW);
    #endif
}

void FX_SingleStun() {
  cRGB color;
  uint16_t variation;
        for (unsigned int i=0; i < storage.NrBarellPixels*5; i++) { // turn on/off one LED at a time
          for(unsigned int j=storage.NrStatusBarPixels; j<storage.NrStatusBarPixels+storage.NrBarellPixels; j++ ) { // fill up string with data
            if (i % j == 0) {
              pixels.set_crgb_at(j, currentColor);                
            }
            else {
              color.r=constrain(currentColor.r/(i % j),0,255);
              color.g=constrain(currentColor.g/(i % j),0,255);
              color.b=constrain(currentColor.b/(i % j),0,255);               
              pixels.set_crgb_at(j, color); // Set value at LED found at index j
            }      
            }
            pixels.sync(); // Sends the data to the LEDs
            //delay(10);
          delay(soundFont.getStunShotDuration()/(5*storage.NrBarellPixels));
          }

          lightOff(storage.NrStatusBarPixels, storage.NrStatusBarPixels + storage.NrBarellPixels-1);
}

void RampBarrel(uint16_t RampDuration, bool DirectionUpDown, int8_t StartPixel=-1, int8_t StopPixel=-1) {

    unsigned long ignitionStart = millis();  //record start of ramp function
    unsigned int i;
    unsigned int j;
    cRGB value;
    if (StartPixel == -1 or StopPixel==-1 or StopPixel<StartPixel or StartPixel>storage.NrBarellPixels or StopPixel>storage.NrBarellPixels) {  // if neither start nor stop is defined or invalid range, go through the whole stripe    // neopixel ramp code from jbkuma
      StartPixel=storage.NrStatusBarPixels;
      StopPixel= storage.NrStatusBarPixels + storage.NrBarellPixels; 
    }
      #ifdef FLAMETHROWER
      for (i=StartPixel; i<StopPixel; i++) { // turn on/off one LED at a time
         FireBlade(0);
         for(j=StartPixel; j<StopPixel; j++ ) { // fill up string with data
            //if ((DirectionUpDown and j<=i) or (!DirectionUpDown and j<=StopPixel-1-i)){
            if ((DirectionUpDown and j<=i) or (!DirectionUpDown and j>i)){
            }
            else if ((DirectionUpDown and j>i) or (!DirectionUpDown and j>StopPixel-1-i)){
              value.r=0;
              value.g=0;
              value.b=0; 
              if (!DirectionUpDown) {
                heat[j-StartPixel]=0;
              }
              pixels.set_crgb_at(j, value); // Set value at LED found at index j
            }      
          }
            pixels.sync(); // Sends the data to the LEDs
            if ((millis()-ignitionStart)<RampDuration/(StopPixel-StartPixel)) {
              i--;
            }
            else {
              ignitionStart=millis();
            }
        }
        //if (!DirectionUpDown) {
        //  for(j=0; j<storage.NrBarellPixels; j++ ) { // clear the heat static variables
        //    heat[j]=0;
        //  }    
       // }
        #endif // FLAMETHROWER
} // RampBlade

void FX_Firethrower(int8_t StartPixel=-1, int8_t StopPixel=-1) {

    unsigned int i;
    unsigned int j;
    cRGB value;
    if (StartPixel == -1 or StopPixel==-1 or StopPixel<StartPixel or StartPixel>storage.NrBarellPixels or StopPixel>storage.NrBarellPixels) {  // if neither start nor stop is defined or invalid range, go through the whole stripe    // neopixel ramp code from jbkuma
      StartPixel=storage.NrStatusBarPixels;
      StopPixel= storage.NrStatusBarPixels + storage.NrBarellPixels; 
    }
      #ifdef FLAMETHROWER
      FireBlade(0);
      //for (i=StartPixel; i<StopPixel; i++) { // turn on/off one LED at a time
      //   pixels.set_crgb_at(i, value); // Set value at LED found at index j
      //}
      pixels.sync(); // Sends the data to the LEDs
      #endif // FLAMETHROWER
} // Firethrower_FX
/*
 * Colors are defined in percentage of brightness.
 *
 */
void getColor(cRGB color={0,0,0}) {

  currentColor.r = color.r;
  currentColor.g = color.g;
  currentColor.b = color.b;
  

} //getColor

void pixelblade_KillKey_Enable() {
  // cut power to the neopixels stripes by disconnecting their GND signal using the LS pins
    digitalWrite(DATA_PIN,HIGH); // in order not to back-connect GND over the Data pin to the stripes when the Low-Sides disconnect it
    delay(100);
    digitalWrite(LS1, LOW);
    digitalWrite(LS2, LOW);
    digitalWrite(LS3, LOW);
}

void pixelblade_KillKey_Disable() {
  // cut power to the neopixels stripes by disconnecting their GND signal using the LS pins
    digitalWrite(LS1, HIGH);
    //digitalWrite(LS2, HIGH);
    //digitalWrite(LS3, HIGH);
}

void BladeMeter (uint8_t ledPins[], int meterLevel) {  //expects input of 0-100
  //normalize data if to max and min if out of range
  if (meterLevel <= 0) { meterLevel = 0; } 
  if (meterLevel >= 100) { meterLevel = 100; }

#ifdef NEOPIXEL // light blade as 3 color meter proportionate to length
  cRGB value;

  //set rest of blade
  for (unsigned int i = 0; i < storage.NrStatusBarPixels; i++) { // turn on/off one LED at a time
      if (i < storage.NrStatusBarPixels * meterLevel / 100){
        if (i < (30 * storage.NrStatusBarPixels / 100)) {
          value.r = 10;
          value.g = 0;
          value.b = 0;
        } else if (i < (60 * storage.NrStatusBarPixels / 100)) {
          value.r = 10;
          value.g = 10;
          value.b = 0;
        } else {
          value.r = 0;
          value.g = 10;
          value.b = 0;
        }
      } else {
      value.r=0;
      value.g=0;
      value.b=0;      
      }      
      pixels.set_crgb_at(i, value);
    }
    pixels.sync(); // Sends the data to the LEDs
//    delay(3);
#endif
}

void StatusBar_Ammo(uint8_t Ammo) {
  cRGB color;
  int8_t fadestep=MAX_AMMO/storage.NrStatusBarPixels;
  uint8_t RemainingAmmo2Show=Ammo;

 for (int8_t i  = 0; i < storage.NrStatusBarPixels; i++) {
  color.r=constrain(RemainingAmmo2Show - ((i)*fadestep),0,20);//=5*(j+1);
  color.g=0;
  color.b=0;
  pixels.set_crgb_at(i, color);
 }
  pixels.sync(); // Sends the data to the LEDs
}
void StatusBar_RampAmmo(uint8_t Ammo, bool RampUp, uint16_t timedelay) {
  uint8_t i;
  if (RampUp) {
    for (i=0; i<MAX_AMMO; i++) {
      StatusBar_Ammo(i);
      delay(timedelay/MAX_AMMO);
    }
  }
  else { // RampDown
    for (i=Ammo; i>0; i--) {
      StatusBar_Ammo(i-1);
      delay(timedelay/MAX_AMMO);
    }
    
  }
}

void StatusBar_ModeChange(uint8_t Mode) {

  cRGB SB_color;
  SB_color.r=0;
  SB_color.g=5;
  SB_color.b=10;
  lightOff(0, storage.NrStatusBarPixels+storage.NrBarellPixels);
  for (uint8_t i=0; i<Mode;i++) {
    if (storage.NrStatusBarPixels>=5) {
      lightOn(SB_color, i, i);
    }
    SinglePlay_Sound(1);
    delay(235); // lenght of sound 9
  }
}

void getColorFix(uint8_t colorID) {
#if defined V_MK1 or defined V_MK2
  switch (colorID) {
  case 0:
//Red
    currentColor.r = MAX_BRIGHTNESS;
    currentColor.g = 0;
    currentColor.b = 0;
    break;
  case 1:
//Orange
    currentColor.r = MAX_BRIGHTNESS;
    currentColor.g = MAX_BRIGHTNESS/4;
    currentColor.b = 0;
    break;
  case 2:
//Green
    currentColor.r = 0;
    currentColor.g = MAX_BRIGHTNESS;
    currentColor.b = 0;
    break;
  case 3:
//Lime
    currentColor.r = MAX_BRIGHTNESS*34/100;
    currentColor.g = MAX_BRIGHTNESS;
    currentColor.b = 0;
    break;
  case 4:
//Blue
    currentColor.r = 0;
    currentColor.g = 0;
    currentColor.b = MAX_BRIGHTNESS;
    break;
  case 5:
//Ice Blue
    currentColor.r = 0;
    currentColor.g = MAX_BRIGHTNESS;
    currentColor.b = MAX_BRIGHTNESS;
    break;
  }
#endif // V_GENERIC
#if defined V_MK3 or defined V_MK4 or defined V_MK5
  switch (colorID) {
  case 0:
//Red
    currentColor.r = MAX_BRIGHTNESS;
    currentColor.g = 0;
    currentColor.b = 0;
    break;
  case 1:
//Orange
    currentColor.r = MAX_BRIGHTNESS;
    currentColor.g = MAX_BRIGHTNESS/4;
    currentColor.b = 0;
    break;
  case 2:
//Amber
    currentColor.r = MAX_BRIGHTNESS;
    currentColor.g = MAX_BRIGHTNESS*66/100;
    currentColor.b = 0;
    break;
  case 3:
//Yellow
    currentColor.r = MAX_BRIGHTNESS;
    currentColor.g = MAX_BRIGHTNESS;
    currentColor.b = 0;
    break;
  case 4:
//Lime
    currentColor.r = MAX_BRIGHTNESS*34/100;
    currentColor.g = MAX_BRIGHTNESS;
    currentColor.b = 0;
    break;
  case 5:
//Green
    currentColor.r = 0;
    currentColor.g = MAX_BRIGHTNESS;
    currentColor.b = 0;
    break;
  case 6:
//Cyan
    currentColor.r = 0;
    currentColor.g = MAX_BRIGHTNESS;
    currentColor.b = MAX_BRIGHTNESS*34/100;
    break;
  case 7:
//Blue
    currentColor.r = 0;
    currentColor.g = 0;
    currentColor.b = MAX_BRIGHTNESS;
    break;
  case 8:
//Light Blue
    currentColor.r = 0;
    currentColor.g = MAX_BRIGHTNESS;
    currentColor.b = MAX_BRIGHTNESS*63/100;
    break;
  case 9:
//Ice Blue
    currentColor.r = 0;
    currentColor.g = MAX_BRIGHTNESS;
    currentColor.b = MAX_BRIGHTNESS;
    break;
  case 10:
//Mauve
    currentColor.r = MAX_BRIGHTNESS*12/100;
    currentColor.g = 0;
    currentColor.b = MAX_BRIGHTNESS;
    break;
  case 11:
//Purple
    currentColor.r = MAX_BRIGHTNESS*35/100;
    currentColor.g = 0;
    currentColor.b = MAX_BRIGHTNESS;
    break;
  case 12:
//Pink
    currentColor.r = MAX_BRIGHTNESS;
    currentColor.g = 0;
    currentColor.b = MAX_BRIGHTNESS;
    break;
  case 13:
//Crimson
    currentColor.r = MAX_BRIGHTNESS;
    currentColor.g = 0;
    currentColor.b = MAX_BRIGHTNESS*5/100;
    break;
  case 14:
//White
    currentColor.r = MAX_BRIGHTNESS;
    currentColor.g = MAX_BRIGHTNESS;
    currentColor.b = MAX_BRIGHTNESS;
    break;
  case 15:
// LED1 and LED2 full on
    currentColor.r = MAX_BRIGHTNESS;
    currentColor.g = MAX_BRIGHTNESS;
    currentColor.b = 0;
    break;
  case 16:
// only LED3 is on
    currentColor.r = 0;
    currentColor.g = 0;
    currentColor.b = MAX_BRIGHTNESS;
    break;
  default:
// White (if enough voltage)
    currentColor.r = 100;
    currentColor.g = 100;
    currentColor.b = 100;
    break;
  }
#endif
} //getColorFix

void JukeBox_Strobe() {
  uint16_t variation;
  
       variation = constrain((abs(analogRead(SPK1) - analogRead(SPK2)))*31/storage.volume*255/100,0,255);
      BladeMeter(ledPins, variation);
      /*for(uint8_t j=0; j<storage.NrStatusBarPixels; j++ ) { // fill up string with data
        Serial.println(j);
          color.r = variation * currentColor.r / 255;
          color.g = variation * currentColor.g / 255;
          color.b = variation * currentColor.b / 255;
          pixels.set_crgb_at(j, color); // Set value at LED found at index j
      }
      pixels.sync(); // Sends the data to the LEDs
   }*/
}


#ifdef FLAMETHROWER
void FireBlade(uint8_t DominantColor) {
// Array of temperature readings at each simulation cell
  int pixelnumber;
  
  // Step 1.  Cool down every cell a little
    for( int i = 0; i < storage.NrBarellPixels; i++) {
      // the random() function in this loop causes phantom swings
      heat[i] = constrain(heat[i] - random(((Fire_Cooling * 10) / storage.NrBarellPixels) + 2),0,255);
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= storage.NrBarellPixels - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random(255) < Fire_Sparking ) {
      int y = random(7);
      heat[y] = constrain(heat[y] + random(95)+160,0,255 );
    }

    // Step 4.  Map from heat cells to LED colors 
    for( int j = storage.NrStatusBarPixels; j <= storage.NrStatusBarPixels + storage.NrBarellPixels-1; j++) {
       cRGB color = HeatColor( heat[j-storage.NrStatusBarPixels],DominantColor);
        pixels.set_crgb_at(j, color); // Set value at LED found at index j
    }
}

// CRGB HeatColor( uint8_t temperature)
//
// Approximates a 'black body radiation' spectrum for
// a given 'heat' level.  This is useful for animations of 'fire'.
// Heat is specified as an arbitrary scale from 0 (cool) to 255 (hot).
// This is NOT a chromatically correct 'black body radiation'
// spectrum, but it's surprisingly close, and it's fast and small.
//
// On AVR/Arduino, this typically takes around 70 bytes of program memory,
// versus 768 bytes for a full 256-entry RGB lookup table.

cRGB HeatColor( uint8_t temperature, uint8_t DominantColor)
{
    cRGB heatcolor;

    // Scale 'heat' down from 0-255 to 0-191,
    // which can then be easily divided into three
    // equal 'thirds' of 64 units each.
    uint8_t t192 = scale8_video( temperature, 192);
     //Serial.print(F("scale8_video_result: "));
     //Serial.print(temperature);Serial.print("/t");Serial.println(t192);

    // calculate a value that ramps up from
    // zero to 255 in each 'third' of the scale.
    uint8_t heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252

    // now figure out which third of the spectrum we're in:
    if( t192 & 0x80) {
        if (DominantColor==0 or DominantColor>2) { // red
          // we're in the hottest third
          heatcolor.r = 255; // full red
          heatcolor.g = 255; // full green
          heatcolor.b = heatramp; // ramp up blue
        }
        else if (DominantColor==1) { // green
          // we're in the hottest third
          heatcolor.g = 255; // full green
          heatcolor.b = 255; // full blue
          heatcolor.r = heatramp; // ramp up red
        }
        else if (DominantColor==2) { // blue
          // we're in the hottest third
          heatcolor.b = 255; // full blue
          heatcolor.g = 255; // full green
          heatcolor.r = heatramp; // ramp up red
        }
      } else if( t192 & 0x40 ) {
        // we're in the middle third
        if (DominantColor==0 or DominantColor>2) { // red
          heatcolor.r = 255; // full red
          heatcolor.g = heatramp; // ramp up green
          heatcolor.b = 0; // no blue
        }
        else if (DominantColor==1) { // green
          heatcolor.g = 255; // full green
          heatcolor.b = heatramp; // ramp up blue
          heatcolor.r = 0; // no red
        }
        else if (DominantColor==2) { // blue
          heatcolor.b = 255; // full blue
          heatcolor.g = heatramp; // ramp up green
          heatcolor.r = 0; // no red
        }
    } else {
        // we're in the coolest third
        if (DominantColor==0 or DominantColor>2) { // red
          heatcolor.r = heatramp; // ramp up red
          heatcolor.g = 0; // no green
          heatcolor.b = 0; // no blue
        }
        else if (DominantColor==1) { // green
          heatcolor.g = heatramp; // ramp up green
          heatcolor.b = 0; // no blue
          heatcolor.r = 0; // no red          
        }
        else if (DominantColor==2) { // blue
          heatcolor.b = heatramp; // ramp up blue
          heatcolor.g = 0; // no green
          heatcolor.r = 0; // no red         
        }
    }

    return heatcolor;
}


uint8_t scale8_video( uint8_t i, uint8_t scale)
{
//    uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
//    // uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
//    // uint8_t j = (i == 0) ? 0 : (((int)i * (int)(scale) ) >> 8) + nonzeroscale;
//    return j;
    uint8_t j=0;
    asm volatile(
        "  tst %[i]\n\t"
        "  breq L_%=\n\t"
        "  mul %[i], %[scale]\n\t"
        "  mov %[j], r1\n\t"
        "  clr __zero_reg__\n\t"
        "  cpse %[scale], r1\n\t"
        "  subi %[j], 0xFF\n\t"
        "L_%=: \n\t"
        : [j] "+a" (j)
        : [i] "a" (i), [scale] "a" (scale)
        : "r0", "r1");

    return j;
}
#endif // FLAMETHROWER
