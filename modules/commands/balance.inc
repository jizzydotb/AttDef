#if defined _attdef_cmd_balance
	#endinput
#endif
#define _attdef_cmd_balance

YCMD: balance(playerid, params[], help)
{
	//if(Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be a higher admin level.");
	if (help)
		return SendCommandHelpMessage(playerid, "balance the teams.");
	if (Current != -1)
		return SendErrorMessage(playerid, "Can't balance when round is active.");

	BalanceTeams();



	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"balanced the teams.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
}
