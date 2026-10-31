#if defined _attdef_cmd_rb
	#endinput
#endif
#define _attdef_cmd_rb

YCMD: rb(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "start a random base.");
	if (Current != -1)
		return SendErrorMessage(playerid, "A round is in progress, please wait for it to end.");
	if (!AllowStartBase)
		return SendErrorMessage(playerid, "Please wait.");

	DoRandomBase(playerid);
	return 1;
}

DoRandomBase(playerid)
{
	AllVsAllMode = false;
	AllowStartBase = false; // Make sure other player or you yourself is not able to start base on top of another base.
	new BaseID	   = DetermineRandomRound(2, false, BASE);
	if (BaseID == -1)
	{
		AllowStartBase = true;
		SendClientMessageToAll(-1, COL_PRIM "\xBB Couldn't start a "COL_SEC"base " COL_PRIM "because the current "COL_SEC"base set " COL_PRIM "doesn't have any.");
		return;
	}
	else if (BaseID == -2)
	{
		AllowStartBase = true;
		SendClientMessageToAll(-1, COL_PRIM "\xBB Couldn't start a "COL_SEC"base " COL_PRIM "with the defined criteria. "COL_SEC"(perhaps the current base set doesn't have exterior bases)");
		return;
	}
	ScheduleRoundStart(BASE, BaseID, 2000);



	new iString[144];
	format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has started random exterior Base "COL_SEC"'%s' (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], BName[BaseID], BaseID);
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
