#if defined _attdef_cmd_sethp
	#endinput
#endif
#define _attdef_cmd_sethp

YCMD: sethp(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "set a player health to a specific value.");

	new pID, Amount;
	if (sscanf(params, "rd", pID, Amount))
		return SendUsageMessage(playerid, "/sethp [Player Name/ID] [Amount]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isn't connected.");

	Amount = clamp(Amount, 0, 100);
	SetHP(pID, Amount);

	LogAdminCommand("sethp", playerid, pID);



	// Adjusted messaging
	if (pID == playerid)
	{
		UpdatePlayerHPTextDraw(playerid);
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set his health to "COL_SEC"%d HP"COL_PRIM".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Amount);
	}
	else
	{
		UpdatePlayerHPTextDraw(pID);
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"%s's "COL_PRIM"health to "COL_SEC"%d HP"COL_PRIM".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name], Amount);
	}

}
