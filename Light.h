/*
 * Light.h
 *
 *  Created on: 6 mars 2016
 * author:     Andras Kun (kun.andras@yahoo.de) based on LSOS by Sebastien CAPOU (neskweek@gmail.com)
 * Source :   https://github.com/Protonerd/FX-BlasterOS
 */

#if not defined LIGHT_H_
#define LIGHT_H_

#include <Arduino.h>
#include <WS2812.h>
#include "Config_HW.h"
#include "Config_SW.h"
#include "SoundFont.h"




// ====================================================================================
// ===              	    			LED FUNCTIONS		                		===
// ====================================================================================


void FX_SingleShot(uint16_t duration);
void FX_ChargeAndShoot();
void FX_SingleStun();
void FX_Firethrower(int8_t StartPixel=-1, int8_t StopPixel=-1);
void lightOn(cRGB color,int8_t StartPixel=-1, int8_t StopPixel=-1);
void lightOff(int8_t StartPixel=-1, int8_t StopPixel=-1);

void StatusBarLightOn(cRGB color);
void getColor(cRGB color={0,0,0}); //getColor
void getColorFix(uint8_t colorID);

void pixelblade_KillKey_Enable();
void pixelblade_KillKey_Disable();

void StatusBar_Ammo(uint8_t Ammo);
void StatusBar_RampAmmo(uint8_t Ammo, bool RampUp, uint16_t timedelay);
void StatusBar_ModeChange(uint8_t Mode);
void RampBarrel(uint16_t RampDuration, bool DirectionUpDown, int8_t StartPixel=-1, int8_t StopPixel=-1);
void BladeMeter (uint8_t ledPins[], int meterLevel); 
void JukeBox_Strobe();

#ifdef FLAMETHROWER
void FireBlade(uint8_t DominantColor=0);
cRGB HeatColor( uint8_t temperature, uint8_t DominantColor);
uint8_t scale8_video( uint8_t i, uint8_t scale);
#endif
#endif /* LIGHT_H_ */
