#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_teamhpbars
	#endinput
#endif
#define _attdef_mod_teamhpbars

/*
	Team HP Bars
*/

#include <sscanf2> // Sscanf 2.10.x

// Team bars constant co-ordinates
#define TEAMBARS_ATTACKER_START_X		 7.000
#define TEAMBARS_DEFENDER_START_X		 551.000
#define TEAMBARS_BAR_WIDTH				 82.000
#define TEAMBARS_BAR_START_Y			 221.500
#define TEAMBARS_DIFF_Y					 13.500
#define TEAMBARS_TEXT_Y_OFFSET			 0.600
#define TEAMBARS_ATTACKER_TEXT_X		 48.000
#define TEAMBARS_DEFENDER_TEXT_X		 592.000

#define TEAMBARS_BLACKBOARD_COLOR		 0x000000CF
#define TEAMBARS_ATTACKER_COLOR			 0x135224AF
#define TEAMBARS_ATTACKER_FLASH_COLOR	 0x2FD156CF
#define TEAMBARS_DEFENDER_COLOR			 0x1A2B6B9A
#define TEAMBARS_DEFENDER_FLASH_COLOR	 0x3D74FFCF
#define TEAMBARS_MAX_PER_TEAM			 6

enum TeamBarsDataStruct {
	Text: TeamBarBlackboard,
	Text: TeamBarHealth,
	Text: TeamBarText,
	Float: TeamBar_Y,
	TeamBar_Team,
	bool: TeamBar_Flashing,
	Float: TeamBar_HealthWidth
}

new TeamBarsData[MAX_PLAYERS][TeamBarsDataStruct];
new Iterator: PlayerWithTeamBar<MAX_PLAYERS>;
new bool: TeamBarShownForPlayer[MAX_PLAYERS][MAX_PLAYERS];
new TeamBarFlashGen[MAX_PLAYERS];

forward StopPlayerTeamBarFlash(playerid, gen);
forward TriggerPlayerTeamBarFlash(playerid);
forward ShowTeamBarsForAll();
forward HideAllTeamBarsForPlayer(playerid);
forward ShowTeamBarsForPlayer(playerid);
forward HideTeamBarsForPlayer(playerid);
forward UpdatePlayerTeamBar(playerid);
forward CreatePlayerTeamBar(playerid);
forward DeletePlayerTeamBar(playerid);
forward DeleteAllTeamBars();
forward ReorganizeTeamBars(team);
forward CreatePlayerTeamBarAtY(playerid, Float: y);
forward bool: ShouldPlayerSeeTeamBars(playerid);
