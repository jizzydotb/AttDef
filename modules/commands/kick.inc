#if defined _attdef_cmd_kick
	#endinput
#endif
#define _attdef_cmd_kick

YCMD: kick(playerid, params[], help)
{
	//if(Player[playerid][Level] < 3 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be a higher admin level.");
	if (help)
		return SendCommandHelpMessage(playerid, "kick a specific player from the server.");
	if (!AllowStartBase)
		return SendErrorMessage(playerid, "Can't kick now. Please wait.");

	new pID, reason[128];
	if (sscanf(params, "rS( )[128]", pID, reason))
		return SendUsageMessage(playerid, "/kick [Player Name/ID] [Reason]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isn't connected.");
	if (Player[pID][Level] >= Player[playerid][Level])
		return SendErrorMessage(playerid, "Cannot kick someone of the same admin level as you or higher.");



	if (isnull(reason))
		va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"kicked "COL_SEC"%s "COL_PRIM"[Reason: "COL_SEC"No reason specified"COL_PRIM"]", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name]);
	else
		va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"kicked "COL_SEC"%s "COL_PRIM"[Reason: "COL_SEC"%s"COL_PRIM"]", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name], reason);

	SchedulePlayerKick(pID, 350);
	LogAdminCommand("kick", playerid, pID);
	return 1;
}
