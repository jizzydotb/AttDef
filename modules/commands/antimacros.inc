#if defined _attdef_cmd_antimacros
	#endinput
#endif
#define _attdef_cmd_antimacros

YCMD: antimacros(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle anti macros system.");

	AntiMacros = !AntiMacros;
	LogAdminCommand("antimacros", playerid, INVALID_PLAYER_ID);
	return va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Anti-macros system.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		AntiMacros ? "{AADC93}enabled" : "{EB8989}disabled"
	);
}
