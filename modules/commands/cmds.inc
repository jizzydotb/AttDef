#if defined _attdef_cmd_cmds
	#endinput
#endif
#define _attdef_cmd_cmds

YCMD: cmds(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "display server commands");

	new dialog_message[4095];
	strcat(
		dialog_message, COL_PRIM "\xBB You can use {FFFFFF}! " COL_PRIM "to write in team chat\n\xBB Press {FFFFFF}N " COL_PRIM "to request team backup in a round\n\xBB You can press {FFFFFF}H " COL_PRIM "to lead your team in a round\n\xBB If you need help with a command, you can use {FFFFFF}/cmdhelp\n");
	new commandsInLine = 1;
	foreach (new i : Command())
	{
		if (GetCommandLevel(i) == 0)
		{
			strcat(dialog_message, commandsInLine % 6 == 1 ? "\n" : "  ");
			strcat(dialog_message, va_return(COL_PRIM "/{FFFFFF}%s", Command_GetName(i)));
			++commandsInLine;
		}
	}
	return ShowPlayerDialog(playerid, DIALOG_NO_RESPONSE, DIALOG_STYLE_MSGBOX, COL_SEC "Everyone's Commands", dialog_message, "OK", "");
}
