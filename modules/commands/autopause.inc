#if defined _attdef_cmd_autopause
	#endinput
#endif
#define _attdef_cmd_autopause

YCMD: autopause(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Set player disconnect action in round mode.");

	if (isnull(params))
	{
		ShowPlayerDialog(playerid, DIALOG_AUTOPAUSE, DIALOG_STYLE_LIST, COL_PRIM "Player Disconnect Action", "Do Nothing\nSend Message\nPause Round", "Select", "Cancel");
		return 1;
	}

	new mode;
	if (sscanf(params, "d", mode))
	{
		SendUsageMessage(playerid, "/autopause [0=Do Nothing, 1=Send Message, 2=Pause Round]");
		return 1;
	}

	if (mode < 0 || mode > 2)
	{
		SendErrorMessage(playerid, "Invalid mode. Valid values are 0 (Do Nothing), 1 (Send Message), and 2 (Pause Round).");
		SendUsageMessage(playerid, "/autopause [0=Do Nothing, 1=Send Message, 2=Pause Round]");
		return 1;
	}

	SetAutoPauseMode(playerid, mode);
	LogAdminCommand("autopause", playerid, INVALID_PLAYER_ID);
	return 1;
}
