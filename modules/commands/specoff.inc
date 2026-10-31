#if defined _attdef_cmd_specoff
	#endinput
#endif
#define _attdef_cmd_specoff

YCMD: specoff(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "turn off spectate mode.");
	if (Player[playerid][Spectating])
	{
		StopSpectate(playerid);
		return 1;
	}
	return SendErrorMessage(playerid, "You are not spectating anyone.");
}
