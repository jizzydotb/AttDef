#if defined _attdef_cmd_maxping
	#endinput
#endif
#define _attdef_cmd_maxping

YCMD: maxping(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "set maximum ping limit.");
	new iPacket;
	if (sscanf(params, "i", iPacket))
		return SendUsageMessage(playerid, "/maxping [Maximum Ping]");

	if (iPacket <= 0 || iPacket > 500)
		return SendErrorMessage(playerid, "Ping limit can be between 0 and 500 maximum.");



	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %d WHERE Option = 'Maximum Ping'", iPacket),
		"save maximum ping"))
		return SendErrorMessage(playerid, "Could not save the maximum ping. No changes were applied.");
	Max_Ping = iPacket;

	new iString[144];
	LogAdminCommand("maxping", playerid, INVALID_PLAYER_ID);

	format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed maximum ping " COL_PRIM "limit to "COL_SEC"%d Ping", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], iPacket);
	return SendClientMessageToAll(-1, iString);
}
