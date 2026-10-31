#if defined _attdef_cmd_addmetime
	#endinput
#endif
#define _attdef_cmd_addmetime

YCMD: addmetime(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "change the addme time limit.");

	new newTime;
	if (sscanf(params, "i", newTime))
		return SendUsageMessage(playerid, "/addmetime [time in seconds]");
	
	if (newTime < 0)
		return SendErrorMessage(playerid, "Time cannot be negative.");

	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %d WHERE Option = 'AddmeTime'", newTime),
		"save addme time limit"))
		return SendErrorMessage(playerid, "Could not save the addme time limit. No changes were applied.");
	gAddmeTime = newTime;

	LogAdminCommand("addmetime", playerid, INVALID_PLAYER_ID);

	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has changed the addme time limit to "COL_SEC"%d"COL_PRIM" seconds.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], newTime);
}
