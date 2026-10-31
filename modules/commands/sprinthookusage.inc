#if defined _attdef_cmd_sprinthookusage
	#endinput
#endif
#define _attdef_cmd_sprinthookusage

YCMD: sprinthookusage(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle usage of Sprinthook in the server.");

	if (!ToggleStoredConfigValue(playerid, "UseSprinthook", UseSprinthook))
		return 1;

	LogAdminCommand("sprinthookusage", playerid, INVALID_PLAYER_ID);
	va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Sprinthook usage.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		UseSprinthook ? "{AADC93}allowed" : "{EB8989}disallowed"
	);

	if (!UseSprinthook)
	{
		foreach (new i : Player)
		{
			if (Player[i][UsingSprintHook] || Player[i][SprintHookPStatus])
			{
				UpdateModDetectionMessage(i);
				SendClientMessage(i, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"SprintHook.asi "COL_PRIM"from your GTA folder.");
				Discord_SendAntiCheatAlert(i, "SprintHook.asi", "SprintHook usage was disallowed by an administrator while player was using `SprintHook.asi`.");
				SchedulePlayerKick(i, 500);
			}
		}
	}
	return 1;
}
