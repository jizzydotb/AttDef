#if defined _attdef_cmd_demo
	#endinput
#endif
#define _attdef_cmd_demo

YCMD: demo(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "put you in base/arena spectator mode.");
	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use while round is on.");

	new Params[64], Round, CommandID;
	
	if (sscanf(params, "s[64]d", Params, Round))
		return SendUsageMessage(playerid, "/demo [base | arena] [ID]");

	switch (YHash(Params, false))
	{
		case _I<base>: CommandID = 1;
		case _I<arena>: CommandID = 2;
		default: return SendUsageMessage(playerid, "/demo [base | arena] [ID]");
	}

	if (Player[playerid][InDuel])
		return SendErrorMessage(playerid, "Can't use this command during duel. Use /rq instead.");
	if (Player[playerid][InDM] || Player[playerid][DMReadd] != -1)
		return SendErrorMessage(playerid, "Can't use this command during DM.");

	switch (CommandID)
	{
		case 1:
		{
			if (Round < 0 || Round >= MAX_BASES || !Iter_Contains(CurrentBases, Round))
				return SendErrorMessage(playerid, "That base does not exist.");
		}
		case 2:
		{
			if (Round < 0 || Round >= MAX_ARENAS || !Iter_Contains(CurrentArenas, Round))
				return SendErrorMessage(playerid, "That arena does not exist.");
		}
	}

	if (Player[playerid][Spectating])
		StopSpectate(playerid);

	Player[playerid][SpectatingRound] = Round;
	switch (CommandID)
	{
		case 1:
		{ //base
			DisablePlayerCheckpoint(playerid);
			DeleteArenaSpecGangzone(playerid);
			SetPlayerInterior(playerid, BInterior[Round]);
			SetPlayerCheckpoint(playerid, BCPSpawn[Round][0], BCPSpawn[Round][1], BCPSpawn[Round][2], 2);
			SetPlayerPos(playerid, BCPSpawn[Round][0], BCPSpawn[Round][1], BCPSpawn[Round][2]);
			DestroyFlareObjects(playerid);
			
			Player[playerid][SpectatingType] = BASE;
			va_PlayerTextDrawSetString(playerid, TD_RoundSpec[playerid], "%sBase ~n~%s%s (ID: ~r~~h~%d%s)", MAIN_TEXT_COLOUR, MAIN_TEXT_COLOUR, BName[Round], Round, MAIN_TEXT_COLOUR);

			foreach (new playerid2 : Player)
				if (playerid2 != playerid)
					va_SendClientMessage(playerid2, -1, COL_PRIM "\xBB "COL_SEC"%s " COL_PRIM "is spectating Base "COL_SEC"'%s' (ID: %d)", Player[playerid][Name], BName[Round], Round);
			va_SendClientMessage(playerid, -1, COL_PRIM "\xBB Viewing Base "COL_SEC"'%s' (ID: %d)", BName[Player[playerid][SpectatingRound]], Player[playerid][SpectatingRound]);
		}
		case 2:
		{ // Arena
			DisablePlayerCheckpoint(playerid);
			DeleteArenaSpecGangzone(playerid);
			new Float:min_x = AMin[Round][0];
			new Float:min_y = AMin[Round][1];
			new Float:max_x = AMax[Round][0];
			new Float:max_y = AMax[Round][1];
			ArenaSpecGangzone[playerid] = GangZoneCreate(min_x, min_y, max_x, max_y);
			GangZoneShowForPlayer(playerid, ArenaSpecGangzone[playerid], 0xFFFF00AA); // Amarillo semi-transparente
			SetPlayerCheckpoint(playerid, ACPSpawn[Round][0], ACPSpawn[Round][1], ACPSpawn[Round][2], 2);
			SetPlayerPos(playerid, ACPSpawn[Round][0], ACPSpawn[Round][1], ACPSpawn[Round][2]);
			SetPlayerInterior(playerid, AInterior[Round]);
			DestroyArenaFlareObjects(playerid);

			Player[playerid][SpectatingType] = ARENA;
			va_PlayerTextDrawSetString(playerid, TD_RoundSpec[playerid], "%sArena ~n~%s%s (ID: ~r~~h~%d%s)", MAIN_TEXT_COLOUR, MAIN_TEXT_COLOUR, AName[Round], Round, MAIN_TEXT_COLOUR);

			foreach (new playerid2 : Player)
				if (playerid2 != playerid)
					va_SendClientMessage(playerid2, -1, COL_PRIM "\xBB "COL_SEC"%s " COL_PRIM "is spectating Arena "COL_SEC"'%s' (ID: %d)", Player[playerid][Name], AName[Round], Round);
			va_SendClientMessage(playerid, -1, COL_PRIM "\xBB Viewing Arena "COL_SEC"'%s' (ID: %d)", AName[Player[playerid][SpectatingRound]], Player[playerid][SpectatingRound]);
		}
	}
	SendClientMessage(playerid, -1, COL_THR "\xBB Switch between bases/arenas using LMB & RMB. Go normal mode using /specoff.");
	Player[playerid][Spectating] = true;
	return 1;
}
