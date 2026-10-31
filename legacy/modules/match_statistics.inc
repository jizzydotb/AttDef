#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_match_statistics
	#endinput
#endif
#define _attdef_mod_match_statistics

#define MAP_RESULT_REJECTED				0
#define MAP_RESULT_PERSISTED			1
#define MAP_RESULT_PENDING_PERSISTENCE	2

forward SaveMapRoundStatistics(BaseOrArenaID, IsArena);
forward RecordMapRoundResult(mapid, isArena, winnerid);
