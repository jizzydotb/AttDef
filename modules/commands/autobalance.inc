#if defined _attdef_cmd_autobalance
	#endinput
#endif
#define _attdef_cmd_autobalance

YCMD: autobalance(playerid, params[], help)
{
	//if(Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be a higher admin level.");
	if (help)
		return SendCommandHelpMessage(playerid, "toggle automatic team balancing when match mode is off.");
	return ToggleAutoBalance(playerid);
}

ToggleAutoBalance(playerid)
{

	AutoBal = !AutoBal;
	LogAdminCommand("autobalance", playerid, INVALID_PLAYER_ID);
	return va_SendClientMessageToAll(-1, COL_PRIM "%s "COL_SEC"%s " COL_PRIM "has %s "COL_SEC"auto-balance " COL_PRIM "in non match mode.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], AutoBal ? "{AADC93}enabled" : "{EB8989}disabled");
}
