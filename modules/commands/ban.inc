#if defined _attdef_cmd_ban
	#endinput
#endif
#define _attdef_cmd_ban

YCMD: ban(playerid, params[], help)
{
	//if(Player[playerid][Level] < 3 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be a higher admin level.");
	if (help)
		return SendCommandHelpMessage(playerid, "ban a specific player from the server.");
	if (!AllowStartBase)
		return SendErrorMessage(playerid, "Can't ban now. Please wait.");

	new pID, Reason[128];
	if (sscanf(params, "rS( )[128]", pID, Reason))
		return SendUsageMessage(playerid, "/ban [Player Name/ID] [Reason]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isn't connected.");
	if (Player[pID][Level] >= Player[playerid][Level])
		return SendErrorMessage(playerid, "Cannot ban someone of the same or higher admin level as you.");



	if (isnull(Reason))
		va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"banned "COL_SEC"%s "COL_PRIM"[Reason: "COL_SEC"No reason specified"COL_PRIM"]", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name]);
	else
		va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"banned "COL_SEC"%s "COL_PRIM"[Reason: "COL_SEC"%s"COL_PRIM"]", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name], Reason);
	SetTimerEx(
		"BanExDelayed", 350, false, "dis", pID, PlayerConnectionGeneration[pID],
		va_return("%s - %s", Player[playerid][Name], Reason));
	return LogAdminCommand("ban", playerid, pID);
}

forward BanExDelayed(playerid, generation, const reason[]);
public BanExDelayed(playerid, generation, const reason[])
{
	if (!IsPlayerConnected(playerid) || PlayerConnectionGeneration[playerid] != generation)
		return 0;

	BanEx(playerid, reason);
	return 1;
}

forward _BanPlayerDelayed(playerid);
public _BanPlayerDelayed(playerid)
{
	Ban(playerid);
	return 1;
}
