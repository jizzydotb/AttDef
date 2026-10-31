#if defined _attdef_cmd_start
	#endinput
#endif
#define _attdef_cmd_start

YCMD: start(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "start a round.");
	if (Current != -1)
		return SendErrorMessage(playerid, "A round is in progress, please wait for it to end.");
	if (!AllowStartBase)
		return SendErrorMessage(playerid, "Please wait.");

	new mapID, mapType[12];
	if (sscanf(params, "s[*]I(-1)", sizeof(mapType), mapType, mapID))
		return SendUsageMessage(playerid, "/start [base | arena | allvsall | last] [ID]");

	new iString[144];



	switch (YHash(mapType, false))
	{
		case _I<allvsall>, _I<allvs>, _I<ava>, _I<all>:
		{
			if (mapID < 0)
			{
				DoRandomAllVsAll(playerid);
				return 1;
			}
			if (!Iter_Contains(CurrentArenas, mapID))
				return SendErrorMessage(playerid, "That arena does not exist.");

			AllVsAllMode = true;
			GameType = ARENA;
			AllowStartBase = false; // Make sure other player or you yourself is not able to start base on top of another base.
			ScheduleRoundStart(ARENA, mapID, 2000);

			format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has started All-vs-All in Arena "COL_SEC"'%s' (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], AName[mapID], mapID);
		}
		case _I<last>:
		{
			AllVsAllMode = false;
			if (ServerLastPlayed == -1 && ServerLastPlayedType == -1)
				return SendErrorMessage(playerid, "No bases/arenas have been played lately!");

			if (ServerLastPlayedType == 1)
			{
				new BaseID = ServerLastPlayed;

				if (!Iter_Contains(CurrentBases, BaseID))
					return SendErrorMessage(playerid, "The last played base does not exist.");

				GameType	   = BASE;

				AllowStartBase = false; // Make sure other player or you yourself is not able to start base on top of another base.
				ScheduleRoundStart(BASE, BaseID, 2000);

				format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has started the last played Base "COL_SEC"'%s' (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], BName[BaseID], BaseID);
			}
			else if (ServerLastPlayedType == 0)
			{
				new ArenaID = ServerLastPlayed;

				if (!Iter_Contains(CurrentArenas, ArenaID))
					return SendErrorMessage(playerid, "The last played arena does not exist.");

				GameType	   = ARENA;

				AllowStartBase = false; // Make sure other player or you yourself is not able to start base on top of another base.
				ScheduleRoundStart(ARENA, ArenaID, 2000);

				format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has started the last played Arena "COL_SEC"'%s' (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], AName[ArenaID], ArenaID);
			}
		}
		case _I<base>:
		{
			AllVsAllMode = false;
			if (mapID < 0)
				return SendUsageMessage(playerid, "/start base [ID]");
			if (!Iter_Contains(CurrentBases, mapID))
				return SendErrorMessage(playerid, "That base does not exist.");

			GameType	   = BASE;

			AllowStartBase = false; // Make sure other player or you yourself is not able to start base on top of another base.
			ScheduleRoundStart(BASE, mapID, 2000);

			format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has started Base "COL_SEC"'%s' (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], BName[mapID], mapID);
		}
		case _I<arena>:
		{
			AllVsAllMode = false;
			if (mapID < 0)
				return SendUsageMessage(playerid, "/start arena [ID]");
			if (!Iter_Contains(CurrentArenas, mapID))
				return SendErrorMessage(playerid, "That arena does not exist.");

			GameType	   = ARENA;

			AllowStartBase = false; // Make sure other player or you yourself is not able to start base on top of another base.
			ScheduleRoundStart(ARENA, mapID, 2000);

			format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has started Arena "COL_SEC"'%s' (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], AName[mapID], mapID);
		}
		default: return SendUsageMessage(playerid, "/start [base | arena | allvsall | last] [ID]");
	}

	foreach (new i : Player)
	{
		if (Player[i][Team])
		{
			NormalListViewingCommands[i] = true;
			BlackListViewingCommands[i] = true;
		}

		if (Player[i][ToAddInRound])
		{
			AllowedViewingCommands[i] = true;
		}

		if (CanPlay(i))
		{
			if (Player[i][SpectatingType] == ARENA || Player[i][SpectatingType] == BASE && (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER))
			{
				StopSpectate(i);
			}

			Player[i][PauseCount] = 0;
			Player[i][gPauseWarnings] = 0;
			Player[i][gWasPaused] = 0;
			TogglePlayerControllable(i, 0); // Pause all the players.
			Player[i][ToAddInRound] = true;
			Player[i][gPauseCheck] = false;
		}
	}
	return SendClientMessageToAll(-1, iString);
}
