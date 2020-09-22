/*
 * Buttons.h
 *
 *  Created on: 01.01.2017
 * author:     Andras Kun (kun.andras@yahoo.de) based on LSOS by Sebastien CAPOU (neskweek@gmail.com)
 * Source :   https://github.com/Protonerd/FX-BlasterOS
 */
#include <Arduino.h>
#include "ConfigMenu.h"
#if not defined BUTTONS_H_
#define BUTTONS_H_


/*
 * BUTTONS PARAMETERS
 ************************************/
#define CLICK        200 //5    // ms you need to press a button to be a click
#define PRESS_MAIN    600 //200  // ms you need to press the main button to be a long press
#define PRESS_AUX    600 //400  // duration in ms you need to remove/insert ammo clip to be recognized as long press
/************************************/




// ====================================================================================
// ===                    BUTTONS CALLBACK FUNCTIONS                      ===
// ====================================================================================

// button action enum
enum ButtonActionEnum{SINGLE_CLICK, DOUBLE_CLICK, LONGPRESS, LONGPRESS_START, LONGPRESS_STOP};

void mainClick();
void mainDoubleClick();
void mainLongPressStart();
void mainLongPress();
void mainLongPressStop();

#ifndef SINGLEBUTTON
void auxClick();
void auxDoubleClick();
void auxLongPressStart();
void auxLongPress();
void auxLongPressStop();
#endif

void ConfigMenuButtonEventHandler(ButtonActionEnum ButtonActionType);


#endif /* BUTTONS_H_ */
