#if defined _attdef_cmd_add
	#endinput
#endif
#define _attdef_cmd_add

YCMD: add(playerid, params[], help)
{
	//if(Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be a higher admin level.");
	if (help)
		return SendCommandHelpMessage(playerid, "add a specific player to the round.");

	new targetid;
	if (sscanf(params, "r", targetid))
		return SendUsageMessage(playerid, "/add [Player Name/ID]");

	PerformCommandAdd(playerid, targetid);
	return 1;
}

PerformCommandAdd(playerid, targetid)
{
	if (Current == -1)
		return SendErrorMessage(playerid, "Round is not active.");

	if (targetid == INVALID_PLAYER_ID || !IsPlayerConnected(targetid))
		return SendErrorMessage(playerid, "That player is not connected.");
	if (Player[targetid][Playing])
		return SendErrorMessage(playerid, "That player is already playing.");
	if (Player[targetid][InDuel])
		return SendErrorMessage(playerid, "That player is in a duel.");
	if (Player[targetid][Team] != ATTACKER && Player[targetid][Team] != DEFENDER)
		return SendErrorMessage(playerid, "You must be either in attacker or defender team");

	if (Player[targetid][Spectating])
		StopSpectate(targetid); //no more need to ask players to do /specoff in order to add them
	if (GameType == BASE)
		AddPlayerToBase(targetid);
	else if (GameType == ARENA)
		AddPlayerToArena(targetid);

	LogAdminCommand("add", playerid, targetid);

	if (Player[targetid][Team] == ATTACKER)
	{
		format(teamRole, sizeof(teamRole), "(Attacker)");
	}
	if (Player[targetid][Team] == DEFENDER)
	{
		format(teamRole, sizeof(teamRole), "(Defender)");
	}



	// Adjusted messaging
	if (targetid == playerid)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has joined the round as "COL_SEC"%s"COL_PRIM".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], teamRole);
	}
	else
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has added "COL_SEC"%s "COL_PRIM"to the round as "COL_SEC"%s"COL_PRIM".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[targetid][Name], teamRole);
	}
}
