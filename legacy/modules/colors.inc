#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_colors
	#endinput
#endif
#define _attdef_mod_colors

/*
	Gamemode colors: teams, messages... etc
*/

#include "modules/this_core.inc"

// Teams colors

/*Attackers Colors*/
#define ATTACKER_PLAYING     0x2ED955AA  // Vibrant emerald green color
#define ATTACKER_NOT_PLAYING 0x2ED95555  // Vibrant emerald green with 55 transparency
#define ATTACKER_SUB_COLOR   0xA4F2B4AA  // Soft light sage green
#define ATTACKER_ASKING_HELP 0x54F07888  // Bright green with 88 transparency
#define ATTACKER_TEAM_LEADER 0x24B545AA  // Medium green with AA transparency

/*Defenderss Colors*/
#define DEFENDER_PLAYING     0x3D82FFAA  // Vibrant royal blue color
#define DEFENDER_NOT_PLAYING 0x3D82FF55  // Vibrant royal blue with 55 transparency
#define DEFENDER_SUB_COLOR   0x8CE0FCAA  // Soft light icy sky cyan
#define DEFENDER_ASKING_HELP 0x6BA0FF88  // Bright blue with 88 transparency
#define DEFENDER_TEAM_LEADER 0x2B6BE5AA  // Medium royal blue with AA transparency

#define REFEREE_COLOR		 0xFFFF0055 // Bright Yellow color

/* GangZone colors*/
#define GANGZONE_ARENA		 0x00000000 // No Color
#define GANGZONE_CPARENA	 0x00000033 // Black Transperent
#define GANGZONE_CP			 0x3D82FF99 // Vibrant Royal Blue
#define GANGZONE_AttSpawn	 0x2ED955AA // Vibrant Emerald Green - Attackers Spawn
#define GANGZONE_FLASH		 0x04EB00AA // Flash color when CP is being taken (Red)

// Client message color palette (Pastel / Soft Aesthetic)
#define COL_PRIM_HEX		 0x93DCACFF // Primary color (Soft Mint Green)
#define COL_SEC_HEX			 0xFFFFFFFF // Secondary color (Crisp White for contrast)
#define COL_THR_HEX			 0xAADC93FF // Tertiary color (Soft Light Green)
#define COL_ERROR_HEX		 0xEB8989FF // Error color (Soft Coral / Pastel Red)
#define COL_WARNING_HEX		 0xF7D070FF // Warning color (Soft Pastel Yellow)
#define COL_USAGE_HEX		 0xBD93DCFF // Usage color (Soft Lavender Purple)
#define COL_TIP_PRIM_HEX	 0xBD93DCFF // Tip color (Soft Lavender Purple)
#define COL_INFO_HEX		 0xBD93DCFF // Info / Duel tag color (Soft Lavender Purple)

#define COL_PRIM			 "{93DCAC}" // Primary color (Soft Mint Green)
#define COL_SEC				 "{FFFFFF}" // Secondary color (Crisp White for contrast)
#define COL_THR				 "{AADC93}" // Tertiary color (Soft Light Green)
#define COL_ERROR			 "{EB8989}" // Error color (Soft Coral / Pastel Red)
#define COL_WARNING			 "{F7D070}" // Warning color (Soft Pastel Yellow)
#define COL_USAGE			 "{BD93DC}" // Usage color (Soft Lavender Purple)
#define COL_TIP_PRIM		 "{BD93DC}" // Tip color (Soft Lavender Purple)
#define COL_TIP				 "{BD93DC}" // Tip color (Soft Lavender Purple)
#define COL_INFO			 "{BD93DC}" // Info / Duel tag color
#define COL_MUTED			 "{CCCCCC}" // Muted / Parenthesis color
#define COL_AC				 "{EB8989}" // AC alerts (Soft Pastel Red)

// Team text colors
new TextColor[MAX_TEAMS][10];
new TDC[MAX_TEAMS][13];

// Fixed textdraw colors retained for existing HUD elements.
#define MAIN_BACKGROUND_COLOUR 0x00000044
#define MAIN_TEXT_COLOUR "~w~"
new ColScheme[10] = COL_PRIM;

new ServerName[50];
