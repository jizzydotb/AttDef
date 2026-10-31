#if defined _attdef_cmd_connstats
	#endinput
#endif
#define _attdef_cmd_connstats

YCMD: connstats(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 3 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher level admin to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "fetch connection statistics of a player.");

	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/connStats [Player Name/ID]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isn't connected.");

	return va_SendClientMessage(playerid, -1, COL_PRIM "\xBB "COL_SEC"%s(%d)'s "COL_PRIM"current connection status: "COL_SEC"%i.", Player[pID][Name], pID, NetStats_ConnectionStatus(pID));
}

