#if defined _attdef_cmd_acmds
	#endinput
#endif
#define _attdef_cmd_acmds

YCMD: acmds(playerid, params[], help)
{
	new pLevel = Player[playerid][Level];
	if (IsPlayerAdmin(playerid))
		pLevel = MAX_ADMIN_LEVELS - 1;
	else if (pLevel < 1 || pLevel >= MAX_ADMIN_LEVELS)
	{
		SendErrorMessage(playerid, "You need admin level 1 to view admin commands.");
		return 1;
	}

	if (help)
		return SendCommandHelpMessage(playerid, "display admin commands");

	new dialog_message[4095];
	strcat(dialog_message, COL_PRIM "\xBB You can use {FFFFFF}@ " COL_PRIM "to write in admin chat\n\xBB If you need help with a command, you can use {FFFFFF}/cmdhelp");
	for (new currentLevel = MAX_ADMIN_LEVELS - 1; currentLevel > 0; --currentLevel)
	{
		new commandsInLine = 1;
		if (pLevel >= currentLevel)
		{
			strcat(dialog_message, va_return(COL_PRIM "\n\nLevel {FFFFFF}%d" COL_PRIM ":", currentLevel));
			foreach (new i : Command())
			{
				if (GetCommandLevel(i) == currentLevel)
				{
					strcat(dialog_message, commandsInLine % 6 == 1 ? "\n" : "  ");
					strcat(dialog_message, va_return(COL_PRIM "/{FFFFFF}%s", Command_GetName(i)));
					++commandsInLine;
				}
			}
		}
	}
	return ShowPlayerDialog(playerid, DIALOG_NO_RESPONSE, DIALOG_STYLE_MSGBOX, COL_SEC "Admin Commands", dialog_message, "OK", "");
}
