#if defined _attdef_cmd_pausemode
	#endinput
#endif
#define _attdef_cmd_pausemode

YCMD: pausemode(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Select how to handle pauses.");

	ShowPlayerDialog(playerid, DIALOG_PAUSEMODE, DIALOG_STYLE_LIST, "Pause Mode", "Send Message\nPause Round\nDo Nothing", "Select", "Cancel");
	return 1;
}
