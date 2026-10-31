#if defined _attdef_cmd_noswitch
	#endinput
#endif
#define _attdef_cmd_noswitch

YCMD: noswitch(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Enable or disable team switching.");

	NoSwitch = !NoSwitch;

	if (sqliteconnection)
	{
		DatabaseExecuteWrite(
			va_return("INSERT OR REPLACE INTO Configs (Option, Value) VALUES ('NoSwitch', '%d')", NoSwitch),
			"save noswitch setting");
	}

	va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s has %s "COL_PRIM"the "COL_SEC"/switch "COL_PRIM"and "COL_SEC"/swapme "COL_PRIM"commands during match mode.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		NoSwitch ? "{EB8989}disabled" : "{AADC93}enabled"
	);
	return 1;
}
