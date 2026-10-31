#if defined _attdef_cmd_plcheck
	#endinput
#endif
#define _attdef_cmd_plcheck

YCMD: plcheck(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "toggle packet-loss status check on a specific player.");
	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/plcheck [Player Name/ID]");

	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player is not connected.");
	if (!Player[pID][NetCheck])
		return SendErrorMessage(playerid, "That player has netcheck disabled on him.");

	LogAdminCommand("plcheck", playerid, pID);

	Player[pID][PLCheck] = !Player[pID][PLCheck];



	// Adjusted messaging
	if (pID == playerid)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Packet-Check on himself.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][PLCheck] ? "{AADC93}enabled" : "{EB8989}disabled");
	}
	else
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Packet-Check " COL_PRIM "on "COL_SEC"%s.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][PLCheck] ? "{AADC93}enabled" : "{EB8989}disabled",
		Player[pID][Name]);
	}	

}
