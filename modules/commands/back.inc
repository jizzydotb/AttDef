#if defined _attdef_cmd_back
	#endinput
#endif
#define _attdef_cmd_back

YCMD: back(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "get you out of AFK mode.");
	if (!Player[playerid][IsAFK])
		return SendErrorMessage(playerid, "You are not AFK?");

	Player[playerid][Team] = REFEREE;
	DelayedFreezeGeneration[playerid]++;
	TogglePlayerControllable(playerid, 1);
	Player[playerid][IsAFK] = false;
	SetHP(playerid, 100);

	new iString[128];
	new ct[5];
	
	foreach (new i : Player)
	{
		switch (Player[i][Team])
		{
			case ATTACKER: ct[0]++;
			case DEFENDER: ct[1]++;
			case REFEREE: ct[2]++;
			case ATTACKER_SUB: ct[3]++;
			case DEFENDER_SUB: ct[4]++;
		}
	}

	format(iString, sizeof(iString), 
		"Team\t\tPlayers\n\n%s%s\t\t\t      %d\n%s%s\t\t\t      %d\n%sReferee\t\t\t      %d\n%s%s \t\t\t      %d\n%s%s \t\t\t      %d",
		TextColor[ATTACKER], TeamName[ATTACKER], ct[0], 
		TextColor[DEFENDER], TeamName[DEFENDER], ct[1], 
		TextColor[REFEREE], ct[2], 
		TextColor[ATTACKER_SUB], TeamName[ATTACKER_SUB], ct[3], 
		TextColor[DEFENDER_SUB], TeamName[DEFENDER_SUB], ct[4]);
	return ShowPlayerDialog(playerid, DIALOG_SWITCH_TEAM, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Team Selection", iString, "Select", "");
}
