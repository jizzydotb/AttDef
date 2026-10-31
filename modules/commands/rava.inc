#if defined _attdef_cmd_rava
	#endinput
#endif
#define _attdef_cmd_rava

YCMD: rava(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "start a random All-vs-All arena.");
	if (Current != -1)
		return SendErrorMessage(playerid, "A round is in progress, please wait for it to end.");
	if (!AllowStartBase)
		return SendErrorMessage(playerid, "Please wait.");

	DoRandomAllVsAll(playerid);
	return 1;
}

DoRandomAllVsAll(playerid)
{
	new availablePlayers;
	foreach (new i : Player)
	{
		if (CanPlay(i))
			availablePlayers++;
	}

	AllowStartBase = false;
	AllVsAllMode = true;

	new ArenaID = DetermineRandomRound(2, false, ARENA);
	if (ArenaID < 0)
		ArenaID = DetermineRandomRound(0, false, ARENA);
	if (ArenaID < 0)
	{
		AllVsAllMode = false;
		AllowStartBase = true;
		return SendClientMessageToAll(-1, ArenaID == -1
			? COL_PRIM "\xBB Couldn't start All-vs-All because the current arena set doesn't have any arenas."
			: COL_PRIM "\xBB Couldn't start All-vs-All with the defined criteria. (Perhaps there are no exterior arenas.)");
	}

	GameType = ARENA;
	ScheduleRoundStart(ARENA, ArenaID, 2000);

	new iString[144];
	format(iString, sizeof(iString), COL_PRIM "%s "COL_SEC"%s "COL_PRIM"has started random All-vs-All in Arena "COL_SEC"'%s' (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], AName[ArenaID], ArenaID);
	SendClientMessageToAll(-1, iString);

	foreach (new i : Player)
	{
		if (Player[i][Team])
		{
			NormalListViewingCommands[i] = true;
			BlackListViewingCommands[i] = true;
		}
		if (Player[i][ToAddInRound])
			AllowedViewingCommands[i] = true;

		if (!CanPlay(i))
			continue;

		if (Player[i][SpectatingType] == ARENA || Player[i][SpectatingType] == BASE && (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER))
			StopSpectate(i);

		Player[i][PauseCount] = 0;
		Player[i][gPauseWarnings] = 0;
		Player[i][gWasPaused] = 0;
		Player[i][gPauseCheck] = false;
		TogglePlayerControllable(i, 0);
		Player[i][ToAddInRound] = true;
	}
	return 1;
}
