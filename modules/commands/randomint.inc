#if defined _attdef_cmd_randomint
	#endinput
#endif
#define _attdef_cmd_randomint

YCMD: randomint(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "start a random round in an interior.");
	if (Current != -1)
		return SendErrorMessage(playerid, "A round is in progress, please wait for it to end.");
	if (!AllowStartBase)
		return SendErrorMessage(playerid, "Please wait.");

	if (isnull(params) || IsNumeric(params))
		return SendUsageMessage(playerid, "/randomint [base | arena]");

	switch (YHash(params, false))
	{
		case _I<base>:
		{
			AllowStartBase = false; // Make sure other player or you yourself is not able to start base on top of another base.
			new BaseID	   = DetermineRandomRound(1, false, BASE);
			if (BaseID == -1)
			{
				AllowStartBase = true;
				SendClientMessageToAll(-1, COL_PRIM "\xBB Couldn't start a base " COL_SEC "because the current base pack " COL_PRIM "doesn't have any.");
				return 1;
			}
			else if (BaseID == -2)
			{
				AllowStartBase = true;
				SendClientMessageToAll(-1, COL_PRIM "\xBB Couldn't start a base with the defined criteria. "COL_SEC"(current base set doesn't have interior bases)");
				return 1;
			}
			ScheduleRoundStart(BASE, BaseID, 2000);

			GameType = BASE;



			new iString[144];
			format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has started randomly interior Base "COL_SEC"'%s' (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], BName[BaseID], BaseID);
			SendClientMessageToAll(-1, iString);
		}
		case _I<arena>:
		{
			AllowStartBase = false; // Make sure other player or you yourself is not able to start base on top of another base.
			new ArenaID	   = DetermineRandomRound(1, false, ARENA);
			if (ArenaID == -1)
			{
				AllowStartBase = true;
				SendClientMessageToAll(-1, COL_PRIM "\xBB Couldn't start an arena " COL_SEC "because the current arena set " COL_PRIM "doesn't have any.");
				return 1;
			}
			else if (ArenaID == -2)
			{
				AllowStartBase = true;
				SendClientMessageToAll(-1, COL_PRIM "\xBB Couldn't start an arena " COL_PRIM "with the defined criteria. "COL_SEC"(current arena set doesn't have interior arenas)");
				return 1;
			}
			ScheduleRoundStart(ARENA, ArenaID, 2000);

			GameType = ARENA;



			new iString[144];
			format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has started randmly interior Arena "COL_SEC"'%s' (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], AName[ArenaID], ArenaID);
			SendClientMessageToAll(-1, iString);
		}
		default: return SendUsageMessage(playerid, "/randomint [base | arena]");
	}

	foreach (new i : Player)
	{
		if (Player[i][Team])
		{
			NormalListViewingCommands[i] = true;
		}

		if (Player[i][Team])
		{
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

			Player[i][gPauseWarnings] = 0; // Reset warnings if the player is not paused
			Player[i][gWasPaused] = 0; // Update state to unpaused
			TogglePlayerControllable(i, 0); // Pause all the players.
			Player[i][ToAddInRound] = true;
			Player[i][gPauseCheck] = false;
		}
	}
	return 1;
}
