#if defined _attdef_cmd_netmode
	#endinput
#endif
#define _attdef_cmd_netmode

YCMD: netmode(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Open dialog to set action for bad network conditions.");

	ShowPlayerDialog(playerid, DIALOG_NETMODE, DIALOG_STYLE_LIST, "Netcheck Mode", "Send Message\nRemove From Round\nKick Player\nPause Round", "Select", "Cancel"); 
	return 1;
}
