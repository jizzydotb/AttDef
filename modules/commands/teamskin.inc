#if defined _attdef_cmd_teamskin
	#endinput
#endif
#define _attdef_cmd_teamskin

YCMD: teamskin(playerid, params[], help)
{
	/*if(Player[playerid][Level] < 4 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be a higher level admin to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "change a specific team skins.");
	/*if(isnull(params))
		return SendUsageMessage(playerid, "/teamskin [Team ID: 0 Attacker | 1 Defender | 2 Referee] [optional: skin id]");*/
	new teamid, skin[8];
	if (sscanf(params, "iS( )[8]", teamid, skin))
		return SendUsageMessage(playerid, "/teamskin [Team ID: 0 Att | 1 Def | 2 Ref | 3 Att_Sub | 4 Def_Sub] [optional: skin id]");
	if (teamid < 0 || teamid > 4)
		return SendErrorMessage(playerid, "Invalid team ID.");

	ChangingSkinOfTeam[playerid] = teamid + 1;
	if (isnull(skin))
	{
		switch (ChangingSkinOfTeam[playerid])
		{
			// Colors match the gamemode palette defined in legacy/modules/impl/colors.inc
			case ATTACKER:     ShowModelSelectionMenu(playerid, teamskinlist, "Select Attacker Skin",     0x2ED955BB, 0x2ED95599, 0x2ED955AA); // Vibrant emerald green
			case DEFENDER:     ShowModelSelectionMenu(playerid, teamskinlist, "Select Defender Skin",     0x3D82FFBB, 0x3D82FF99, 0x3D82FFAA); // Vibrant royal blue
			case REFEREE:      ShowModelSelectionMenu(playerid, teamskinlist, "Select Referee Skin",      0xFFFF88BB, 0xFFFF8899, 0xFFFF88AA); // Bright yellow
			case ATTACKER_SUB: ShowModelSelectionMenu(playerid, teamskinlist, "Select Attacker Sub Skin", 0xA4F2B4BB, 0xA4F2B499, 0xA4F2B4AA); // Soft light sage green
			case DEFENDER_SUB: ShowModelSelectionMenu(playerid, teamskinlist, "Select Defender Sub Skin", 0x8CE0FCBB, 0x8CE0FC99, 0x8CE0FCAA); // Soft light icy sky cyan
			default:           ShowModelSelectionMenu(playerid, teamskinlist, "Select Team Skin");
		}
		LogAdminCommand("teamskin", playerid, INVALID_PLAYER_ID);
		return 1;
	}

	new skinid = strval(skin);
	if (skinid < 0 || skinid > 311)
		return SendErrorMessage(playerid, "Invalid skin ID.");

	if (!PersistTeamSkin(ChangingSkinOfTeam[playerid], skinid))
	{
		ChangingSkinOfTeam[playerid] = -1;
		return SendErrorMessage(playerid, "Could not save the team skin. No changes were applied.");
	}

	new iString[128];

	foreach (new i : Player)
	{
		if (Player[i][Team] == ChangingSkinOfTeam[playerid])
		{
			if (Player[i][pSkin] != -1)
			{
				SetPlayerSkin(i, Player[i][pSkin]);
			}
			else
			{
				if (Player[i][Team] == ATTACKER)
					SetPlayerSkin(i, Skin[ATTACKER]);
				else if (Player[i][Team] == ATTACKER_SUB)
					SetPlayerSkin(i, Skin[ATTACKER_SUB]);
				else if (Player[i][Team] == DEFENDER)
					SetPlayerSkin(i, Skin[DEFENDER]);
				else if (Player[i][Team] == DEFENDER_SUB)
					SetPlayerSkin(i, Skin[DEFENDER_SUB]);
				else if (Player[i][Team] == REFEREE)
					SetPlayerSkin(i, Skin[REFEREE]);
			}
		}
	}

	LogAdminCommand("teamskin", playerid, INVALID_PLAYER_ID);



	format(
		iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed "COL_SEC"%s " COL_PRIM "skin to "COL_SEC"ID %d", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], TeamName[ChangingSkinOfTeam[playerid]],
		skinid);
	ChangingSkinOfTeam[playerid] = -1;
	return SendClientMessageToAll(-1, iString);
}
