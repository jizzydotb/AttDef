#if defined _attdef_cmd_netcheck
	#endinput
#endif
#define _attdef_cmd_netcheck

YCMD: netcheck(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "toggle net status check on a specific player.");
	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/netcheck [Player Name/ID]");

	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player is not connected.");

	new bool: enabled = !Player[pID][NetCheck];
	if (!DatabaseExecuteWrite(
		va_return("UPDATE Players SET NetCheck = %d WHERE Name = '%q'", enabled, Player[pID][Name]),
		"save player net-check setting"))
		return SendErrorMessage(playerid, "Could not save the player's net-check setting. No changes were applied.");

	LogAdminCommand("netcheck", playerid, pID);



	Player[pID][NetCheck] = Player[pID][FPSCheck] = Player[pID][PingCheck] = Player[pID][PLCheck] = enabled;

	// Adjusted messaging
	if (pID == playerid)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Net-Check on himself.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][NetCheck] ? "{AADC93}enabled" : "{EB8989}disabled");
	}
	else
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Net-Check " COL_PRIM "on "COL_SEC"%s.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][NetCheck] ? "{AADC93}enabled" : "{EB8989}disabled",
		Player[pID][Name]);
	}
}
