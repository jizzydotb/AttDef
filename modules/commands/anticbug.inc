#if defined _attdef_cmd_anticbug
	#endinput
#endif
#define _attdef_cmd_anticbug

YCMD: anticbug(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "enable or disable the native anti-C-bug system.");
	if (isnull(params))
		return SendUsageMessage(playerid, "/anticbug [on / off]");

	switch (YHash(params, false))
	{
		case _I<on>:
		{	
			if (!SetStoredConfigBooleanValue(playerid, "AntiCBug", AntiCBug, true))
				return 1;
		}
		case _I<off>:
		{
			if (!SetStoredConfigBooleanValue(playerid, "AntiCBug", AntiCBug, false))
				return 1;
		}
		default: return SendUsageMessage(playerid, "/anticbug [on / off]");
	}
	LogAdminCommand("anticbug", playerid, INVALID_PLAYER_ID);
	return va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has %s "COL_PRIM"Anti-C-bug.",
		PLAYER_ROLE_NAME(playerid), Player[playerid][Name],
		AntiCBug ? "{AADC93}enabled" : "{EB8989}disabled");
}
