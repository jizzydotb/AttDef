#if defined _attdef_cmd_togglearmour
	#endinput
#endif
#define _attdef_cmd_togglearmour

forward UpdateArmourPingObjects();

YCMD: togglearmour(playerid, params[], help)
{
	if (help) 
		return SendCommandHelpMessage(playerid, "Toggle the armour object ping feature. Usage: /togglearmour");

	ArmourToggle = !ArmourToggle;

	if (sqliteconnection)
	{
		DatabaseExecuteWrite(
			va_return("INSERT OR REPLACE INTO Configs (Option, Value) VALUES ('ArmourToggle', '%d')", ArmourToggle),
			"save armour toggle setting");
	}

	UpdateArmourPingObjects();

	return va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"armour object ping feature", 
		PLAYER_ROLE_NAME(playerid), 
		Player[playerid][Name], 
		ArmourToggle ? "{AADC93}enabled" : "{EB8989}disabled"
	);
}
