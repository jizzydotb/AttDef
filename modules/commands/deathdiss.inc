#if defined _attdef_cmd_deathdiss
	#endinput
#endif
#define _attdef_cmd_deathdiss

YCMD: deathdiss(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "set a disrespect message to be shown for players whom you kill.");
	if (isnull(params))
		return SendUsageMessage(playerid, "/deathdiss [Disrespect Message]");
	if (strlen(params) <= 3)
		return SendErrorMessage(playerid, "Too short!");
	if (strlen(params) >= 32)
		return SendErrorMessage(playerid, "Too long!");

	new iString[128];
	format(iString, sizeof(iString), "UPDATE `Players` SET `DeathMessage` = '%q' WHERE `Name` = '%q'", params, Player[playerid][Name]);
	if (!DatabaseExecuteWrite(iString, "save player death message"))
		return SendErrorMessage(playerid, "Could not save your death message; it was not changed.");

	format(DeathMessageStr[playerid], 32, "%s", params);
	Player[playerid][HasDeathQuote] = true;
	return SendClientMessage(playerid, -1, COL_PRIM "\xBB Death diss message has been changed successfully!");
}
