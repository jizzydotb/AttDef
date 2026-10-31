#if defined _attdef_cmd_maxfps
	#endinput
#endif
#define _attdef_cmd_maxfps

YCMD: maxfps(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "set maximum FPS limit.");
	new iPacket;
	if (sscanf(params, "i", iPacket))
		return SendUsageMessage(playerid, "/maxfps [Maximum FPS]");

	if (iPacket < 20 || iPacket > 300)
		return SendErrorMessage(playerid, "FPS limit can be between 20 and 300.");



	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %d WHERE Option = 'Maximum FPS'", iPacket),
		"save maximum FPS"))
		return SendErrorMessage(playerid, "Could not save the maximum FPS. No changes were applied.");
	Max_FPS = iPacket;

	new iString[144];
	LogAdminCommand("maxfps", playerid, INVALID_PLAYER_ID);

	format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed maximum FPS " COL_PRIM "limit to "COL_SEC"%d FPS", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], iPacket);
	return SendClientMessageToAll(-1, iString);
}
