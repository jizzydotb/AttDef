#if defined _attdef_cmd_fpscheck
	#endinput
#endif
#define _attdef_cmd_fpscheck

YCMD: fpscheck(playerid, params[], help)
{
	//if(Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be a higher admin level to do that.");
	if (help)
		return SendCommandHelpMessage(playerid, "toggle FPS status check on a specific player.");
	
	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/fpscheck [Player Name/ID]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player is not connected.");
	if (!Player[pID][NetCheck])
		return SendErrorMessage(playerid, "That player has netcheck disabled on him.");

	Player[pID][FPSCheck] = !Player[pID][FPSCheck];

	LogAdminCommand("fpscheck", playerid, pID);



	// Adjusted messaging
	if (pID == playerid)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"FPS-Check on himself.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][FPSCheck] ? "{AADC93}enabled" : "{EB8989}disabled");
	}
	else
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"FPS-Check on "COL_SEC"%s.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][FPSCheck] ? "{AADC93}enabled" : "{EB8989}disabled",
		Player[pID][Name]);
	}	
}
