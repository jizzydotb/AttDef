#if defined _attdef_cmd_gunmenutime
	#endinput
#endif
#define _attdef_cmd_gunmenutime

YCMD: gunmenutime(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "set the gunmenu time limit.");
	
	new time;
	if (sscanf(params, "i", time))
		return SendUsageMessage(playerid, "/gunmenutime [time in seconds]");

	if (time < 0)
		return SendErrorMessage(playerid, "Time cannot be negative.");

	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %d WHERE Option = 'GunmenuTime'", time),
		"save Gunmenu time limit"))
		return SendErrorMessage(playerid, "Could not save the Gunmenu time limit. No changes were applied.");
	gGunmenuTime = time;

	LogAdminCommand("gunmenutime", playerid, INVALID_PLAYER_ID);

	return va_SendClientMessageToAll(-1, COL_PRIM "%s " COL_SEC "%s " COL_PRIM "has set the gunmenu time limit to "COL_SEC"%d "COL_PRIM"seconds.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], gGunmenuTime);
}
