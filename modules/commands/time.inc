#if defined _attdef_cmd_time
	#endinput
#endif
#define _attdef_cmd_time

YCMD: time(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "change your day time.");
	if (!Player[playerid][Logged])
		return SendErrorMessage(playerid, "You need to log in.");
	new mytime, previousTime;
	if (sscanf(params, "i", mytime))
		return SendUsageMessage(playerid, "/time [Hour]");

	mytime = clamp(mytime, 0, TimeLimit);

	previousTime = Player[playerid][Time];
	if (!DatabaseExecuteWrite(
		va_return("UPDATE Players SET Time = %d WHERE Name = '%q'", mytime, Player[playerid][Name]),
		"save player time"))
		return SendErrorMessage(playerid, "Could not save your time setting. No changes were applied.");

	SetPlayerTime(playerid, mytime, 0);
	Player[playerid][Time] = mytime;
	return va_SendClientMessage(playerid, -1, "\xBB Your Time changed to: %d (Previous Time: %d)", mytime, previousTime);
}
