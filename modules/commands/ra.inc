#if defined _attdef_cmd_ra
	#endinput
#endif
#define _attdef_cmd_ra

YCMD: ra(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "start a random arena.");
	if (Current != -1)
		return SendErrorMessage(playerid, "A round is in progress, please wait for it to end.");
	if (!AllowStartBase)
		return SendErrorMessage(playerid, "Please wait.");

	DoRandomArena(playerid);
	return 1;
}

DoRandomArena(playerid)
{
	AllVsAllMode = false;
	AllowStartBase = false; // Make sure other player or you yourself is not able to start base on top of another base.
	new ArenaID	   = DetermineRandomRound(2, false, ARENA);
	if (ArenaID == -1)
	{
		AllowStartBase = true;
		SendClientMessageToAll(-1, COL_PRIM "\xBB Couldn't start an "COL_SEC"arena " COL_PRIM "because the current "COL_SEC"arena set " COL_PRIM "doesn't have any.");
		return;
	}
	else if (ArenaID == -2)
	{
		AllowStartBase = true;
		SendClientMessageToAll(-1, COL_PRIM "\xBB Couldn't start an "COL_SEC"arena " COL_PRIM "with the defined criteria. "COL_SEC"(perhaps the current arena set doesn't have exterior arenas)");
		return;
	}
	
	ScheduleRoundStart(ARENA, ArenaID, 2000);
	

	
	new iString[144];
	format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has started random exterior Arena "COL_SEC"'%s' (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], AName[ArenaID], ArenaID);
	SendClientMessageToAll(-1, iString);

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

			Player[i][PauseCount] = 0;
			Player[i][gPauseWarnings] = 0; // Reset warnings if the player is not paused
			Player[i][gWasPaused] = 0; // Update state to unpaused
			TogglePlayerControllable(i, 0); // Pause all the players.
			Player[i][ToAddInRound] = true;
			Player[i][gPauseCheck] = false;
		}
	}
}
