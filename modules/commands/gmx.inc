#if defined _attdef_cmd_gmx
	#endinput
#endif
#define _attdef_cmd_gmx

YCMD: gmx(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 5 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a level 5 admin to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "restart your server.");



	va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has set the server to restart.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	DB_Close(sqliteconnection);
	LogAdminCommand("gmx", playerid, INVALID_PLAYER_ID);

	return SendRconCommand("gmx");
}
