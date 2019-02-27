/*
 * Config.h
 *
 * author:     Andras Kun (kun.andras@yahoo.de) based on LSOS by Sebastien CAPOU (neskweek@gmail.com)
 * Source :   https://github.com/Protonerd/FX-BlasterOS
 */

#include <Arduino.h>
#include "DFPlayer.h"
#include "Config_HW.h"
#include "Config_SW.h"
#include "Variants.h"

#if not defined CONFIGMENU_H_
#define CONFIGMENU_H_

#include <WS2812.h>

enum BlasterStateEnum {S_STANDBY, S_BLASTERON, S_CONFIG, S_SLEEP, S_JUKEBOX};
enum ActionModeSubStatesEnum {AS_BLASTER_MODE, AS_STUN_MODE, AS_MEGABLAST_MODE, AS_REPEATER_MODE, AS_FLAMETHROWER_MODE, AS_BLAST, AS_STUN, AS_MEGABLAST,AS_REPEATER,AS_FLAMETHROWER, AS_CLIPIN, AS_CLIPOUT, AS_EMPTY, AS_FULL, AS_MANUALRELOAD, AS_SWITCHOFF, AS_JAM, AS_UNJAM, AS_OVERHEAT};
enum DisplaySkins_Enum {DS_E11, DS_BH, DS_BARS};

// ====================================================================================
// ===           	  	 			CONFIG MODE FUNCTIONS	                		===
// ====================================================================================

void confParseValue(uint16_t variable, uint16_t min, uint16_t max,
		short int multiplier);

void NextConfigState();

#endif /* CONFIG_H_ */
