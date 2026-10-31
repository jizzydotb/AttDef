#if defined _attdef_cmd_syncabuse
	#endinput
#endif
#define _attdef_cmd_syncabuse

YCMD: syncabuse(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle sync abuse.");

	SyncAbuse = !SyncAbuse;
	LogAdminCommand("syncabuse", playerid, INVALID_PLAYER_ID);
	return va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Sync abuse.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		SyncAbuse ? "{AADC93}enabled" : "{EB8989}disabled"
	);
}
