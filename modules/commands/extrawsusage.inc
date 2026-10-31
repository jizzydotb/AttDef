#if defined _attdef_cmd_extrawsusage
	#endinput
#endif
#define _attdef_cmd_extrawsusage

YCMD: extrawsusage(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle usage of ExtraWS in the server.");

	if (!ToggleStoredConfigValue(playerid, "UseExtraws", UseExtraws))
		return 1;

	LogAdminCommand("extrawsusage", playerid, INVALID_PLAYER_ID);
	va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"ExtraWS usage.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		UseExtraws ? "{AADC93}allowed" : "{EB8989}disallowed"
	);

	if (!UseExtraws)
	{
		foreach (new i : Player)
		{
			if (Player[i][UsingExtraws] || Player[i][ExtrawsPStatus])
			{
				UpdateModDetectionMessage(i);
				SendClientMessage(i, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"ExtraWS.asi "COL_PRIM"from your GTA folder.");
				Discord_SendAntiCheatAlert(i, "ExtraWS.asi", "ExtraWS usage was disallowed by an administrator while player was using `ExtraWS.asi`.");
				SchedulePlayerKick(i, 500);
			}
		}
	}
	return 1;
}
