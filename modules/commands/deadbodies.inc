#if defined _attdef_cmd_deadbodies
	#endinput
#endif
#define _attdef_cmd_deadbodies

YCMD: deadbodies(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle Dead Bodies.");

	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is active.");

	DeadBodies = !DeadBodies;
	LogAdminCommand("deadbodies", playerid, INVALID_PLAYER_ID);
	return va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Dead bodies.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		DeadBodies ? "{AADC93}enabled" : "{EB8989}disabled"
	);
}
