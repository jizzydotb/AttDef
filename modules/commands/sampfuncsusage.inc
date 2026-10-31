#if defined _attdef_cmd_sampfuncsusage
	#endinput
#endif
#define _attdef_cmd_sampfuncsusage

YCMD: sampfuncsusage(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle usage of Sampfuncs in the server.");

	if (!ToggleStoredConfigValue(playerid, "UseSampfuncs", UseSampfuncs))
		return 1;

	LogAdminCommand("sampfuncsusage", playerid, INVALID_PLAYER_ID);
	va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Sampfuncs usage.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		UseSampfuncs ? "{AADC93}allowed" : "{EB8989}disallowed"
	);

	if (!UseSampfuncs)
	{
		foreach (new i : Player)
		{
			if (Player[i][UsingSampfuncs] || Player[i][SampfuncsPStatus])
			{
				UpdateModDetectionMessage(i);
				SendClientMessage(i, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"SAMPFUNCS.asi "COL_PRIM"from your GTA folder.");
				Discord_SendAntiCheatAlert(i, "SAMPFUNCS.asi", "Sampfuncs usage was disallowed by an administrator while player was using `SAMPFUNCS.asi`.");
				SchedulePlayerKick(i, 500);
			}
		}
	}
	return 1;
}
