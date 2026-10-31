#if defined _attdef_cmd_defendersvehicles
	#endinput
#endif
#define _attdef_cmd_defendersvehicles

YCMD: defendersvehicles(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle defenders vehicles.");

	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is active.");

	DefendersWithVehicles = !DefendersWithVehicles;
	LogAdminCommand("defendersvehicles", playerid, INVALID_PLAYER_ID);
	return va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Defenders with vehicles.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		DefendersWithVehicles ? "{AADC93}enabled" : "{EB8989}disabled"
	);
}
