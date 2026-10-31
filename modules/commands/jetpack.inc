#if defined _attdef_cmd_jetpack
	#endinput
#endif
#define _attdef_cmd_jetpack

YCMD: jetpack(playerid, params[], help)
{
	if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");
	if (help)
		return SendCommandHelpMessage(playerid, "spawn a jetpack.");
	if (Player[playerid][Playing])
		return SendErrorMessage(playerid, "Can't use this command in round.");

	new pID;
	sscanf(params, "R(-1)", pID);
	if (pID == -1)
		pID = playerid;

	else if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isn't connected.");

	SetPlayerSpecialAction(pID, 2);



	// Adjusted messaging
	if (pID == playerid)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "gave a jetpack to himself.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	}
	else
	{
		return va_SendClientMessageToAll(-1, COL_PRIM "%s "COL_SEC"%s " COL_PRIM "gave a jetpack to "COL_SEC"%s.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name]);
	}

}
