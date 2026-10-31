#if defined _attdef_cmd_cparena
	#endinput
#endif
#define _attdef_cmd_cparena

YCMD: cparena(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle cp in arena.");

	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is active.");

	CPInArena = !CPInArena;
	LogAdminCommand("cparena", playerid, INVALID_PLAYER_ID);
	return va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Checkpoint in arena.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		CPInArena ? "{AADC93}enabled" : "{EB8989}disabled"
	);
}
