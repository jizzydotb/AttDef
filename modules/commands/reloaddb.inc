#if defined _attdef_cmd_reloaddb
	#endinput
#endif
#define _attdef_cmd_reloaddb

YCMD: reloaddb(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 3 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "re-load the server database. This might have dangerous results.");
	if (Current != -1 || MatchRoundsStarted > 0)
		return SendErrorMessage(playerid, "You can only reload the database while in the lobby and no match is active.");
	return SetDatabaseToReload(playerid);
}
