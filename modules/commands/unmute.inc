#if defined _attdef_cmd_unmute
	#endinput
#endif
#define _attdef_cmd_unmute

YCMD: unmute(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "unmute a specific player.");

	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/unmute [Player Name/ID]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isnt connected.");
	if (!Player[pID][Mute])
		return SendErrorMessage(playerid, "That player is not muted.");

	Player[pID][Mute] = false;

	LogAdminCommand("unmute", playerid, pID);



	// Adjusted messaging
	if (pID == playerid)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"unmuted themselves.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	}
	else
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"unmuted "COL_SEC"%s"COL_PRIM".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name]);
	}
}
