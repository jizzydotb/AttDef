#if defined _attdef_cmd_cptime
	#endinput
#endif
#define _attdef_cmd_cptime

YCMD: cptime(playerid, params[], help)
{
	//if(Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be a higher admin level to do that.");
	if (help)
		return SendCommandHelpMessage(playerid, "set the time needed to capture a checkpoint.");
	new cpTime;
	if (sscanf(params, "i", cpTime))
		return SendUsageMessage(playerid, "/cptime [Seconds (20 - 40)]");



	cpTime = clamp(cpTime, 20, 40);
	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %d WHERE Option = 'CP Time'", cpTime),
		"save checkpoint capture time"))
		return SendErrorMessage(playerid, "Could not save the checkpoint capture time. No changes were applied.");

	ConfigCPTime  = cpTime;
	CurrentCPTime = ConfigCPTime + 1;

	new iString[144];
	LogAdminCommand("cptime", playerid, INVALID_PLAYER_ID);

	format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed the "COL_SEC"CP time " COL_PRIM "to "COL_SEC"%d seconds", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], cpTime);
	return SendClientMessageToAll(-1, iString);
}
