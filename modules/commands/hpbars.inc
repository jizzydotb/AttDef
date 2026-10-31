#if defined _attdef_cmd_hpbars
	#endinput
#endif
#define _attdef_cmd_hpbars

YCMD: hpbars(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle HP Bars system.");

	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is active.");

	ShowHPBars = !ShowHPBars;
	LogAdminCommand("hpbars", playerid, INVALID_PLAYER_ID);
	return va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"HP Bars system.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		ShowHPBars ? "{AADC93}enabled" : "{EB8989}disabled"
	);
}
