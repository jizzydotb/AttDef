#if defined _attdef_cmd_unlock
	#endinput
#endif
#define _attdef_cmd_unlock

YCMD: unlock(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "unlock the server.");
	if (!ServerLocked)
		return SendErrorMessage(playerid, "Server is not locked.");

	SendRconCommand("password 0");

	ServerLocked = false;
	PermLocked	 = false;

	LogAdminCommand("unlock", playerid, INVALID_PLAYER_ID);



	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has unlocked the server.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
}
