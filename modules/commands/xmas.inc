#if defined _attdef_cmd_xmas
	#endinput
#endif
#define _attdef_cmd_xmas

YCMD: xmas(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle christmas mode (if filterscript is available)");
	if (isnull(params))
		return SendUsageMessage(playerid, "/xmas [on / off]");



	switch (YHash(params, false))
	{
		case _I<on>:
		{
			SendRconCommand("loadfs xmas");
			return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has attempted to load "COL_SEC"Xmas filterscript" COL_PRIM ".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
		}
		case _I<off>:
		{
			SendRconCommand("unloadfs xmas");
			return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has attempted to unload "COL_SEC"Xmas filterscript" COL_PRIM ".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
		}
	}
	return SendUsageMessage(playerid, "/xmas [on / off]");
}
