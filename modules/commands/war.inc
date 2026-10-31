#if defined _attdef_cmd_war
	#endinput
#endif
#define _attdef_cmd_war

YCMD: war(playerid, params[], help)
{
	//if(Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be a higher admin level to do that.");
	if (help)
		return SendCommandHelpMessage(playerid, "start a match quickly.");
	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is on.");
	if (WarModeEnding)
		return SendErrorMessage(playerid, "Please wait.");



	new TeamAName[7], TeamBName[7];
	if (sscanf(params, "s[7]S( )[7]", TeamAName, TeamBName))
		return SendUsageMessage(playerid, "/match <Team A> <Team B> <end>");
	if (strcmp(TeamAName, "end", true) == 0 && isnull(TeamBName) && WarMode)
	{
		ScheduleWarEnd(5000);
		va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has scheduled the match to end " COL_MUTED "(%sTeam %s " COL_MUTED "vs %sTeam %s" COL_MUTED ")" COL_PRIM ".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], TextColor[ATTACKER], TeamName[ATTACKER], TextColor[DEFENDER], TeamName[DEFENDER]);
		SendClientMessageToAll(-1, "\xBB " COL_PRIM "Generating match results... " COL_MUTED "Type " COL_SEC "/showagain " COL_MUTED "to view them again.");
		return 1;
	}

	if (WarMode)
		return SendErrorMessage(playerid, "Match-mode is already on.");
	if (isnull(TeamBName))
		return SendUsageMessage(playerid, "/match <Team A> <Team B> <end>");
	if (strlen(TeamAName) > 6 || strlen(TeamBName) > 6)
		return SendErrorMessage(playerid, "Team name is too long.");
	if (strfind(TeamAName, "~") != -1 || strfind(TeamBName, "~") != -1)
		return SendErrorMessage(playerid, "~ not allowed.");

	format(TeamName[ATTACKER], sizeof(TeamAName), TeamAName);
	format(TeamName[ATTACKER_SUB], 24, "%s Sub", TeamName[ATTACKER]);
	format(TeamName[DEFENDER], sizeof(TeamBName), TeamBName);
	format(TeamName[DEFENDER_SUB], 24, "%s Sub", TeamName[DEFENDER]);
	TeamScore[ATTACKER] = 0;
	TeamScore[DEFENDER] = 0;
	CurrentRound = 0;
	UpdateTeamScoreTextDraw();
	UpdateRoundsPlayedTextDraw();
	UpdateTeamNameTextDraw();
	UpdateTeamNamesTextdraw();

	MatchRoundsStarted = 0;
	WarMode			   = true;
	RoundPaused		   = false;
	TeamTacPauseUsed[ATTACKER] = false;
	TeamTacPauseUsed[DEFENDER] = false;
	for (new i = 0; i < 101; i++)
	{
		MatchRoundsRecord[i][round__ID]		   = -1;
		MatchRoundsRecord[i][round__type]	   = -1;
		MatchRoundsRecord[i][round__completed] = false;
		MatchRoundsRecord[i][round__winner]    = -1;
		MatchRoundsRecord[i][round__winnerName][0] = EOS;
	}

	ClearPlayerVariables();
	foreach (new i : Player)
	{
		Player[i][TotalKills]		 = 0;
		Player[i][TotalDeaths]		 = 0;
		Player[i][TotalDamage]		 = 0;
		Player[i][RoundPlayed]		 = 0;
		Player[i][TotalBulletsFired] = 0;
		Player[i][TotalshotsHit]	 = 0;
		UpdateRoundKillDmgText(i);
	}

	// Keep current teams; only players carrying a team tag are moved to it
	AssignMatchTeamsByTag();
	ShowMatchScoreBoard();
	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has started the match " COL_MUTED "(%sTeam %s " COL_MUTED "vs %sTeam %s" COL_MUTED ")" COL_PRIM ".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], TextColor[ATTACKER], TeamName[ATTACKER], TextColor[DEFENDER], TeamName[DEFENDER]);
}
