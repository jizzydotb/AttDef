#if defined _attdef_cmd_mute
	#endinput
#endif
#define _attdef_cmd_mute

YCMD: mute(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "mute a specific player.");
	new pID, Reason[128], iString[256];
	if (sscanf(params, "rS( )[128]", pID, Reason))
		return SendUsageMessage(playerid, "/mute [Player Name/ID] [Reason]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isnt connected.");

	if (Player[pID][Mute])
		return SendErrorMessage(playerid, "That player is already muted.");
	/*if (Player[playerid][Level] <= Player[pID][Level] && playerid != pID)
		return SendErrorMessage(playerid, "That player is higher admin level than you.");*/

	Player[pID][Mute] = true;

	LogAdminCommand("mute", playerid, pID);



	// Adjusted messaging
	if (pID == playerid)
	{
		if (isnull(Reason))
			format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has muted himself. [Reason: "COL_SEC"No reason specified"COL_PRIM"]", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
		else
			format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has muted himself. [Reason: "COL_SEC"%s"COL_PRIM"]", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Reason);
		return SendClientMessageToAll(-1, iString);
	}
	else
	{
		if (isnull(Reason))
			format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has muted "COL_SEC"%s"COL_PRIM". [Reason: "COL_SEC"No reason specified"COL_PRIM"]", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name]);
		else
			format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has muted "COL_SEC"%s"COL_PRIM". [Reason: "COL_SEC"%s"COL_PRIM"]", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name], Reason);
		return SendClientMessageToAll(-1, iString);
	}

}
