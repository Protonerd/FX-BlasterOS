/*
 * OLED.cpp
 *
 * author:     Andras Kun (kun.andras@yahoo.de) based on LSOS by Sebastien CAPOU (neskweek@gmail.com)
 * Source :   https://github.com/Protonerd/FX-BlasterOS
 */

#include "OLED.h"
#include "ConfigMenu.h"
#include <DFPlayer.h>
#include "SoundFont.h"
#include <Fonts/Aurebesh_6pt.h>


#ifdef OLED_DISPLAY
  #define OLED_RESET 4
  #if defined OLED_SCOPE

    extern Adafruit_SSD1306 display;
  #endif
  #if defined OLED_STD

    extern Adafruit_SSD1306 display;
  #endif

  #define DSPL_AC_X (SCREEN_WIDTH - DSPL_AC_W - 7)
  #define DSPL_AC_Y (SCREEN_HEIGHT - DSPL_AC_H)
  #define DSPL_AC_W 32
  #define DSPL_AC_H 18
#endif

extern BlasterStateEnum BlasterState;
extern BlasterStateEnum PrevBlasterState;

extern ActionModeSubStatesEnum PrevActionModeSubStates;
extern ActionModeSubStatesEnum ActionModeSubStates;
extern ConfigModeSubStatesEnum ConfigModeSubStates;

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
// ===                     OLED DISPLAY MANIPULATION FUNCTIONS                 ===
// ==================================================================================== 



void DisplayBlasterStatus(char statustext[6], bool clearScreen) {
#ifdef OLED_DISPLAY
  #if defined OLED_STD
      if (clearScreen) {
        display.clearDisplay();
      }
      display.setTextColor(WHITE);
      display.setTextSize(1);
   // #define SCREEN_WIDTH xx // OLED display width, in pixels
   // #define SCREEN_HEIGHT 4xx8 // OLED display height, in pixels
      
      if (storage.sndProfile[storage.soundFont].DisplaySkins==DS_BH) {
        display.setTextColor(WHITE, BLACK);
        display.setCursor(0,(SCREEN_HEIGHT/2)+10);
        display.fillRect(0,(SCREEN_HEIGHT/2)-5, 70, 25, BLACK);
      }
      else {
        //display.setFont();
        display.setTextColor(BLACK, WHITE);
        display.setCursor((SCREEN_WIDTH/3),(SCREEN_HEIGHT/2));
        display.fillRect((SCREEN_WIDTH/3)-5,(SCREEN_HEIGHT/2)-5, 48, 25, BLACK);        
      }
      //display.setCursor(0,(SCREEN_HEIGHT-7));
      display.print(statustext);
    #endif
    #if defined OLED_SCOPE
      
    #endif
      display.display();   
  #endif
}

void DisplayStartScreen() {
  #ifdef OLED_DISPLAY
    #if defined OLED_STD
      display.clearDisplay();
      display.setFont();
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(0,0);
      display.println(INTROTEXT);
      #if defined V_MK1
        display.println("MK1");
      #else
        #if defined V_MK2
          display.println("MK2")
        #else
          #if defined V_MK3
            display.println("MK3");
          #endif
        #endif
      #endif
    #endif
    #if defined OLED_SCOPE
        display.clearDisplay();
        display.drawCircle(display.width()/2, display.height()/2, (min(display.width(),display.height())/2)-1, WHITE);
        display.drawCircle(display.width()/2, display.height()/2, (min(display.width(),display.height())/4)-1, WHITE);
        display.drawFastVLine(display.width()/2, 0,min(display.width(),display.height()),WHITE);
        display.drawFastHLine(0, display.height()/2,min(display.width(),display.height()),WHITE);
    #endif
    display.display();
  #endif
}

void DisplayBlasterOnFrames() {
  #ifdef OLED_DISPLAY
    #if defined OLED_STD
      display.clearDisplay();
      display.setFont();
      display.setCursor(0,0); 
      switch(storage.sndProfile[storage.soundFont].DisplaySkins) {
        default:
        case DS_E11:
    /*      display.setTextSize(1);
          display.setRotation(3);
          display.print("Ammo Cnt");
          display.setRotation(0);
          display.drawRect(22,0, display.width(),display.height(), WHITE);
          display.setCursor(24,9); // 9+15 (it seems that a size 3 char is 15 pixels wide)
          display.setTextColor(WHITE);
          display.setTextSize(3);
          display.print("0000");
          if (AmmoCnt<10) {
            display.print("0");
          }
           display.print(AmmoCnt);*/
          break;
        case DS_BH:
    /*      display.setTextSize(1);
          display.setTextColor(WHITE);
          display.setFont(&Aurebesh6pt7b);
          display.setCursor(0,0);
          display.println("Ammo DLT-5511");
          display.setRotation(3);
          display.println(AmmoCnt);
          display.setRotation(0);*/
          break;
        case DS_BARS:
          display.setTextSize(1);
          display.setTextColor(WHITE);
           display.println(CUSTOMTEXT);
          //display.println("Gabor Dan-Dan");
          display.setCursor(45,DSPL_AC_Y+DSPL_AC_H/2);
          display.println("Ammo");
          // draw an ammo count rectange at the lower left corner
          display.drawRect(DSPL_AC_X,DSPL_AC_Y, DSPL_AC_W, DSPL_AC_H, WHITE); // do not shift to the very rigth corner as it is covered by the blaster body
          break;
        
      }
    #endif
    #if defined OLED_SCOPE
        display.clearDisplay();
        display.drawCircle(display.width()/2, display.height()/2, (min(display.width(),display.height())/2)-1, WHITE);
        display.drawCircle(display.width()/2, display.height()/2, (min(display.width(),display.height())/4)-1, WHITE);
        display.drawFastVLine(display.width()/2, 0,min(display.width(),display.height()),WHITE);
        display.drawFastHLine(0, display.height()/2,min(display.width(),display.height()),WHITE);
    #endif 
      display.display();
  #endif
}

void DisplayAmmoCount(uint8_t AmmoCnt, int16_t iAngle=-1) {
//void DisplayAmmoCount(uint8_t AmmoCnt) {
  #ifdef OLED_DISPLAY
    #if defined OLED_STD
      // clear only that portion of the display which is supposed to change
      //display.setFont();
       switch(storage.sndProfile[storage.soundFont].DisplaySkins) {
        default:
        case DS_E11:
          display.clearDisplay();
          display.setCursor(0,0);
          display.setTextSize(1);
          display.setRotation(3);
          display.print("Ammo");
          display.setRotation(0);
          display.drawRect(14,0, SCREEN_WIDTH-14,SCREEN_HEIGHT, WHITE);
          display.setCursor(16,6); // 9+15 (it seems that a size 3 char is 15 pixels wide)
          display.setTextColor(WHITE);
          display.setTextSize(3);
          display.print("0000");
          if (AmmoCnt<10) {
            display.print("0");
          }
          display.print(AmmoCnt);
          break;   
        case DS_BH:
          //uint8_t tempx;
          //uint8_t tempy;
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setFont(&Aurebesh6pt7b);
          display.setTextSize(1);
          display.setCursor(0,10);
          display.println(CUSTOMTEXT);
          display.setRotation(3);
          display.println(AmmoCnt);
          display.setRotation(0);
          // draw bars showing the ammo count
            //tempx=0;
            //tempy=0;
            display.setRotation(2);
            for (uint8_t i=0; i<AmmoCnt; i++ ) {
              //display.fillRect(34+i*8,10,5,20, WHITE); // 10x20 pixel high are the ammo bars
              display.fillRect(i*4,0,2,2+i, WHITE); // 10x20 pixel high are the ammo bars
            }
            display.setRotation(0);
        break;
        case DS_BARS:
          display.fillRect(DSPL_AC_X+1,DSPL_AC_Y+1, DSPL_AC_W-2, DSPL_AC_H-2, BLACK);
          display.setCursor(DSPL_AC_X+4,DSPL_AC_Y+2);
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.print(AmmoCnt);
          break;
      }   
    #endif // OLED_STD
    #if defined OLED_SCOPE
        display.clearDisplay();
        display.drawCircle(display.width()/2, display.height()/2, (min(display.width(),display.height())/2)-1, WHITE);
        display.drawCircle(display.width()/2, display.height()/2, (min(display.width(),display.height())/4)-1, WHITE);
        display.drawFastVLine(display.width()/2, 0,min(display.width(),display.height()),WHITE);
        display.drawFastHLine(0, display.height()/2,display.width(),WHITE);
        if (iAngle>-1) {
          display.drawLine(display.width()/2, display.height()/2, (display.width()/2 + (sin (iAngle*PI/180) * min(display.width(),display.height()))/2)-1, (display.height()/2 + (cos (iAngle*PI/180) * min(display.width(),display.height()))/2)-1, WHITE);
          Serial.print(iAngle);Serial.print("     ");Serial.print(sin (iAngle*PI/180)); Serial.print("     "); Serial.println(cos (iAngle*PI/180));    
        }
        display.setCursor(3,3);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print(AmmoCnt);
        display.setCursor(display.width()-3-8,display.height()-3-8);
        display.print(ActionModeSubStates);
    #endif
    display.display();
  #endif // OLED_DISPLAY
}
