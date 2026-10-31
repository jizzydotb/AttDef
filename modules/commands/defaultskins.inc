#if defined _attdef_cmd_defaultskins
	#endinput
#endif
#define _attdef_cmd_defaultskins

YCMD: defaultskins(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "reset team skins to default ones.");

	if (!SetDefaultTeamSkins())
		return SendErrorMessage(playerid, "Could not save the default team skins. No changes were applied.");

	foreach (new i : Player)
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

	LogAdminCommand("defaultskins", playerid, INVALID_PLAYER_ID);



	new iString[128];
	format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has changed skins to default.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	return SendClientMessageToAll(-1, iString);
}
