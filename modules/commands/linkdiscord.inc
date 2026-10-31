#if defined _attdef_cmd_linkdiscord
	#endinput
#endif
#define _attdef_cmd_linkdiscord

stock ShowLinkDiscord(playerid)
{
	if (!Player[playerid][Logged])
		return SendErrorMessage(playerid, "You must be logged in to link your account.");

	// Security: Check if this account is already linked
	new checkQuery[192];
	format(checkQuery, sizeof(checkQuery), "SELECT `DiscordID` FROM `DiscordLinks` WHERE `AccountName` = '%q'", Player[playerid][Name]);
	new DBResult: checkRes = DB_ExecuteQuery(sqliteconnection, checkQuery);
	if (checkRes && DB_GetRowCount(checkRes) > 0)
	{
		DB_FreeResultSet(checkRes);
		return SendErrorMessage(playerid, "This account is already linked to a Discord user! Contact an admin to unlink.");
	}
	if (checkRes) DB_FreeResultSet(checkRes);

	new code;
	new bool: collision;
	do
	{
		collision = false;
		code = 100000 + random(900000);
		foreach (new i : Player)
		{
			if (g_discordVerifyCode[i] == code)
			{
				collision = true;
				break;
			}
		}
	} while (collision);

	g_discordVerifyCode[playerid] = code;
	g_discordVerifyExpire[playerid] = gettime() + 300;
	format(g_discordVerifyAccount[playerid], MAX_PLAYER_NAME, "%s", Player[playerid][Name]);

	va_SendClientMessage(playerid, -1, "{93DCAC}\xBB Discord Code: {FFFFFF}%d{93DCAC}. In Discord, type: {FFFFFF}" DISCORD_PREFIX_STR "verify %d", code, code);
	SendClientMessage(playerid, -1, "{CCCCCC}This code is valid for 5 minutes.");
	return 1;
}

YCMD: linkdiscord(playerid, params[], help)
{
	#pragma unused params
	if (help)
		return SendCommandHelpMessage(playerid, "generate a code to link your account with Discord.");
	return ShowLinkDiscord(playerid);
}

YCMD: discord(playerid, params[], help)
{
	#pragma unused params
	if (help)
		return SendCommandHelpMessage(playerid, "generate a code to link your account with Discord.");
	return ShowLinkDiscord(playerid);
}
