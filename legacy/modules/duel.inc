#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_duel
	#endinput
#endif
#define _attdef_mod_duel

/*
	Duel system
*/

#define DUEL_MODE_FIRST_TO 0
#define DUEL_MODE_BEST_OF_FIVE 1
#define DUEL_MAX_TARGET 99

#if !defined MAX_SKINS
	#define MAX_SKINS 300
#endif

forward SetupDuellerSpawn(playerid, bool: forcePos = true, bool: freeze = false);
