#if defined _attdef_cmd_minfps
	#endinput
#endif
#define _attdef_cmd_minfps

YCMD: minfps(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "set minimum FPS limit.");
	new iPacket;
	if (sscanf(params, "i", iPacket))
		return SendUsageMessage(playerid, "/minfps [Minimum FPS]");


	if (iPacket < 20 || iPacket > 90)
		return SendErrorMessage(playerid, "FPS limit can be between 20 and 90 maximum.");



	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %d WHERE Option = 'Minimum FPS'", iPacket),
		"save minimum FPS"))
		return SendErrorMessage(playerid, "Could not save the minimum FPS. No changes were applied.");
	Min_FPS = iPacket;

	new iString[144];
	LogAdminCommand("minfps", playerid, INVALID_PLAYER_ID);

	format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed minimum FPS " COL_PRIM "limit to "COL_SEC"%d FPS", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], iPacket);
	return SendClientMessageToAll(-1, iString);
}
