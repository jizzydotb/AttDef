#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_match_statistics
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_match_statistics
	#endinput
#endif
#define _attdef_impl_match_statistics

stock SaveMapRoundStatistics(BaseOrArenaID, IsArena)
{
	new query[256];
	new operation[96];
	if (IsArena)
	{
		format(
			query, sizeof(query),
			"UPDATE `%s` SET `APlayed` = %d, `AAttWins` = %d, `ADefWins` = %d WHERE `ID` = %d",
			CurrentArenaSet, APlayed[BaseOrArenaID], AAttWins[BaseOrArenaID], ADefWins[BaseOrArenaID], BaseOrArenaID);
		format(operation, sizeof(operation), "save arena statistics for %s:%d", CurrentArenaSet, BaseOrArenaID);
	}
	else
	{
		format(
			query, sizeof(query),
			"UPDATE `%s` SET `BPlayed` = %d, `BAttWins` = %d, `BDefWins` = %d WHERE `ID` = %d",
			CurrentBaseSet, BPlayed[BaseOrArenaID], BAttWins[BaseOrArenaID], BDefWins[BaseOrArenaID], BaseOrArenaID);
		format(operation, sizeof(operation), "save base statistics for %s:%d", CurrentBaseSet, BaseOrArenaID);
	}
	return DatabaseExecuteWrite(query, operation, true);
}

stock RecordMapRoundResult(mapid, isArena, winnerid)
{
	if (winnerid != -1 && winnerid != ATTACKER && winnerid != DEFENDER)
		return MAP_RESULT_REJECTED;

	if (isArena)
	{
		if (mapid < 0 || mapid >= MAX_ARENAS || !AExist[mapid])
			return MAP_RESULT_REJECTED;

		APlayed[mapid]++;
		if (winnerid == ATTACKER)
			AAttWins[mapid]++;
		else if (winnerid == DEFENDER)
			ADefWins[mapid]++;
	}
	else
	{
		if (mapid < 0 || mapid >= MAX_BASES || !BExist[mapid])
			return MAP_RESULT_REJECTED;

		BPlayed[mapid]++;
		if (winnerid == ATTACKER)
			BAttWins[mapid]++;
		else if (winnerid == DEFENDER)
			BDefWins[mapid]++;
	}

	return SaveMapRoundStatistics(mapid, isArena);
}
