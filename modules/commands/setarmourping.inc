#if defined _attdef_cmd_setarmourping
	#endinput
#endif
#define _attdef_cmd_setarmourping

forward UpdateArmourPingObjects();

YCMD: setarmourping(playerid, params[], help)
{
	if (help) 
		return SendCommandHelpMessage(playerid, "Set the minimum ping for the armour object. Usage: /setarmourping [min]");

	if (isnull(params))
		return SendUsageMessage(playerid, "/setarmourping [min]");

	new pingMin = strval(params);
	if (pingMin < 1)
		return SendErrorMessage(playerid, "Ping must be a positive number.");

	ArmourPingMin = pingMin;

	if (sqliteconnection)
	{
		DatabaseExecuteWrite(
			va_return("INSERT OR REPLACE INTO Configs (Option, Value) VALUES ('ArmourPingMin', '%d')", ArmourPingMin),
			"save armour ping setting");
	}

	UpdateArmourPingObjects();

	return va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has set the armour object's minimum ping to: "COL_SEC"%d.", 
		PLAYER_ROLE_NAME(playerid), 
		Player[playerid][Name], 
		ArmourPingMin
	);
}
