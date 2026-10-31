#if defined _attdef_cmd_servername
	#endinput
#endif
#define _attdef_cmd_servername

YCMD: servername(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "changes the server name used for the welcome message");
	new svname[52];
	if (sscanf(params, "s[52]", svname))
		return SendUsageMessage(playerid, "/servername [Server Name]");
	if (strlen(svname) > 50)
		return SendErrorMessage(playerid, "This server name exceeds the maximum character limit (50 characters).");



	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = '%q' WHERE Option = 'Server Name'", svname),
		"save server name"))
		return SendErrorMessage(playerid, "Could not save the server name. No changes were applied.");

	format(ServerName, sizeof(ServerName), "%s", svname);
	DestroyMainSpawnText3D();
	CreateMainSpawnText3D();
	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed the server name " COL_PRIM "to "COL_SEC"%s", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], ServerName);
}
