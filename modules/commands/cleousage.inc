#if defined _attdef_cmd_cleousage
	#endinput
#endif
#define _attdef_cmd_cleousage

YCMD: cleousage(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle usage of Cleo in the server.");

	if (!ToggleStoredConfigValue(playerid, "UseCleo", UseCleo))
		return 1;

	LogAdminCommand("cleousage", playerid, INVALID_PLAYER_ID);
	va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Cleo usage.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		UseCleo ? "{AADC93}allowed" : "{EB8989}disallowed"
	);

	if (!UseCleo)
	{
		foreach (new i : Player)
		{
			if (Player[i][UsingCleo] || Player[i][CleoPStatus])
			{
				UpdateModDetectionMessage(i);
				SendClientMessage(i, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"Cleo.asi "COL_PRIM"from your GTA folder.");
				Discord_SendAntiCheatAlert(i, "Cleo.asi", "Cleo usage was disallowed by an administrator while player was using `Cleo.asi`.");
				SchedulePlayerKick(i, 500);
			}
		}
	}
	return 1;
}
