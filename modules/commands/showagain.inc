#if defined _attdef_cmd_showagain
	#endinput
#endif
#define _attdef_cmd_showagain

YCMD: showagain(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "display round or match results.");

	// Check if no game has been played
	if (ServerLastPlayed == -1 && ServerLastPlayedType == -1)
	{
		SendErrorMessage(playerid, "No game has been played yet. Results are not available.");
	}
	else
	{
		ShowEndRoundTextDraw(playerid);
	}
	
	return 1;
}
