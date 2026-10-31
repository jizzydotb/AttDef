#if defined _attdef_cmd_dmg
	#endinput
#endif
#define _attdef_cmd_dmg

YCMD: dmg(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Toggle Floating Damage Informer (use '/dmg me' for personal toggle).");

	new bool: isAdmin = (Player[playerid][Level] >= 1 || IsPlayerAdmin(playerid));

	if (!isnull(params))
	{
		if (!strcmp(params, "me", true))
		{
			Player[playerid][ShowFloatingDamage] = !Player[playerid][ShowFloatingDamage];
			return va_SendClientMessage(playerid, -1, ""COL_PRIM"\xBB Floating Damage Informer is now %s"COL_PRIM" for you.", Player[playerid][ShowFloatingDamage] ? "{AADC93}enabled" : "{EB8989}disabled");
		}
		if (!strcmp(params, "server", true) || !strcmp(params, "all", true))
		{
			if (!isAdmin)
				return SendErrorMessage(playerid, "You are not authorized to change server configuration.");

			ToggleFloatingDamage(playerid);
			LogAdminCommand("dmg", playerid, INVALID_PLAYER_ID);
			return 1;
		}
	}

	if (isAdmin)
	{
		ToggleFloatingDamage(playerid);
		LogAdminCommand("dmg", playerid, INVALID_PLAYER_ID);
		return 1;
	}
	else
	{
		Player[playerid][ShowFloatingDamage] = !Player[playerid][ShowFloatingDamage];
		return va_SendClientMessage(playerid, -1, ""COL_PRIM"\xBB Floating Damage Informer is now %s"COL_PRIM" for you.", Player[playerid][ShowFloatingDamage] ? "{AADC93}enabled" : "{EB8989}disabled");
	}
}
