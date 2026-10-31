#if defined _attdef_cmd_remove
	#endinput
#endif
#define _attdef_cmd_remove

YCMD: remove(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "remove a specific player from the round.");
	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/remove [Player Name/ID]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isn't connected.");
	if (!Player[pID][Playing])
		return SendErrorMessage(playerid, "That player is not playing.");

	new HP[2];
	HP[0] = GetHP(pID);
	HP[1] = GetAP(pID);

	RemovePlayerFromRound(pID);

	LogAdminCommand("remove", playerid, pID);



	// Adjusted messaging
	if (pID == playerid)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"left the round.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	}
	else
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"removed "COL_SEC"%s "COL_PRIM"from the round. "COL_THR"(HP: %d - %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name],
		Player[pID][Name], HP[0], HP[1]);
	}

}
