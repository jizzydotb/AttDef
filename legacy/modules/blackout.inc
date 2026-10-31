#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_blackout
	#endinput
#endif
#define _attdef_mod_blackout

forward DestroyPlayerBlackoutZone(playerid);
forward CreatePlayerBlackoutZone(playerid);
forward DestroyBlackoutZones();
forward CreateBlackoutZone();
