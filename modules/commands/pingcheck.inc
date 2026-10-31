#if defined _attdef_cmd_pingcheck
	#endinput
#endif
#define _attdef_cmd_pingcheck

YCMD: pingcheck(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "toggle ping status check on a specific player.");
	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/pingcheck [Player Name/ID]");

	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player is not connected.");
	if (!Player[pID][NetCheck])
		return SendErrorMessage(playerid, "That player has netcheck disabled on him.");

	LogAdminCommand("pingcheck", playerid, pID);

	Player[pID][PingCheck] = !Player[pID][PingCheck];



	// Adjusted messaging
	if (pID == playerid)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Ping-Check on himself.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][PingCheck] ? "{AADC93}enabled" : "{EB8989}disabled");
	}
	else
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Ping-Check " COL_PRIM "on "COL_SEC"%s.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][PingCheck] ? "{AADC93}enabled" : "{EB8989}disabled",
		Player[pID][Name]);
	}	
}
