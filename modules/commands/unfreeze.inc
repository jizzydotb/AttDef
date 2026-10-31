#if defined _attdef_cmd_unfreeze
	#endinput
#endif
#define _attdef_cmd_unfreeze

YCMD: unfreeze(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "unfreeze a player.");
	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/unfreeze [Player Name/ID]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isnt connected.");

	DelayedFreezeGeneration[pID]++;
	TogglePlayerControllable(pID, 1);

	LogAdminCommand("unfreeze", playerid, pID);


	// Adjusted messaging
	if (pID == playerid) // The command is executed by the player changing their own level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"unfroze.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	}
	else // The command is executed by an admin changing another player's level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"unfroze "COL_SEC"%s"COL_PRIM".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name]);
	}

}
