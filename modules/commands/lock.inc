#if defined _attdef_cmd_lock
	#endinput
#endif
#define _attdef_cmd_lock

YCMD: lock(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "lock the server.");
	if (ServerLocked)
		return CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/unlock");

	if (isnull(params))
		return SendUsageMessage(playerid, "/lock [Password]");
	if (strlen(params) > MAX_SERVER_PASS_LENGTH)
		return SendErrorMessage(playerid, "Server password is too long.");
	if (!IsValidServerPassword(params))
		return SendErrorMessage(playerid, "Password may only contain letters, numbers, _, -, ., !, @, #, and +.");

	format(ServerPass, sizeof(ServerPass), "password %s", params);
	SendRconCommand(ServerPass);

	ServerLocked = true;
	PermLocked	 = false;

	new iString[144];

	LogAdminCommand("lock", playerid, INVALID_PLAYER_ID);

	format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has locked the server. [Password: "COL_SEC"%s"COL_PRIM"]", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], params);
	return SendClientMessageToAll(-1, iString);
}
