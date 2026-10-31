#if defined _attdef_cmd_givemenu
	#endinput
#endif
#define _attdef_cmd_givemenu

YCMD: givemenu(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "show weapon menu to a specific player.");
	if (Current == -1)
		return SendErrorMessage(playerid, "Round is not active.");
	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/givemenu [Player Name/ID]");

	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isn't connected.");
	if (!Player[pID][Playing])
		return SendErrorMessage(playerid, "That player isn't playing.");

	ShowPlayerGunmenu(pID, 0);



	// Adjusted messaging
	if (pID == playerid)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"put himself back to gunmenu.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	}
	else
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "put "COL_SEC"%s " COL_PRIM "back into the weapon menu.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name]);
	}

}
