#if defined _attdef_cmd_setlevel
	#endinput
#endif
#define _attdef_cmd_setlevel

YCMD: setlevel(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 5 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be level 5 or rcon admin.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "change the level of a player.");

	new GiveID, NewLevel;
	if (sscanf(params, "rd", GiveID, NewLevel))
		return SendUsageMessage(playerid, "/setlevel [Player Name/ID] [Level]");
	if (GiveID == INVALID_PLAYER_ID || !IsPlayerConnected(GiveID))
		return SendErrorMessage(playerid, "That player is not connected.");
	if (!Player[GiveID][Logged])
		return SendErrorMessage(playerid, "That player is not logged in.");
	if (NewLevel < 0 || NewLevel >= MAX_ADMIN_LEVELS)
		return SendErrorMessage(playerid, "Invalid level.");
	if (Player[GiveID][Level] == NewLevel)
		return SendErrorMessage(playerid, "That player is already this level.");

	if (!DatabaseExecuteWrite(
		va_return("UPDATE Players SET Level = %d WHERE Name = '%q'", NewLevel, Player[GiveID][Name]),
		"save player admin level"))
		return SendErrorMessage(playerid, "Could not save the player's level. No changes were applied.");

	if (Player[GiveID][Level] == 0) // Previous level was 0. This means it's a new admin. Guide them.
		SendClientMessage(GiveID, -1, COL_PRIM "\xBB Looks like you're a new admin. Type "COL_SEC"/acmds " COL_PRIM " to see a list of admin commands!");
	Player[GiveID][Level] = NewLevel;
	UpdatePlayerAdminGroup(GiveID);





	LogAdminCommand("setlevel", playerid, GiveID);

	// Adjusted messaging
	if (GiveID == playerid) // The command is executed by the player changing their own level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set their server level to "COL_SEC"%d (%s)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], NewLevel, PLAYER_ROLE_NAME(playerid));
	}
	else // The command is executed by an admin changing another player's level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has set "COL_SEC"%s's " COL_PRIM "server level to "COL_SEC"%d (%s)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[GiveID][Name], NewLevel, PLAYER_ROLE_NAME(GiveID));
	}
}
