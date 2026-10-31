#if defined _attdef_cmd_spas
	#endinput
#endif
#define _attdef_cmd_spas

YCMD: spas(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle spas selection in gunmenu.");



	// Find the index of Spas in gunmenu
	new idx = -1;
	for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
	{
		if (GunmenuData[i][GunID] == WEAPON_SHOTGSPA)
		{
			idx = i;
			break;
		}
	}
	if (idx == -1)
		return SendErrorMessage(playerid, "Spas doesn't exist in the gunmenu.");
	new limit = GunmenuData[idx][GunLimit] <= 0 ? 1 : 0;
	if (!DatabaseExecuteWrite(
		va_return("UPDATE `Gunmenu` SET `Limit`=%d WHERE `Weapon`=%d", limit, GunmenuData[idx][GunID]),
		"save SPAS gunmenu limit"))
		return SendErrorMessage(playerid, "Could not save the SPAS limit. No changes were applied.");

	GunmenuData[idx][GunLimit] = limit;
	if (limit == 1)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed "COL_SEC"Spas " COL_PRIM "limit to "COL_SEC"1", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	}


	va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed "COL_SEC"Spas " COL_PRIM "limit to "COL_SEC"0", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	if (Current == -1)
		return 1;

	// If there's a round in progress
	// Loop through all players who are in round
	foreach (new i : PlayersInRound)
	{
		// The following code checks if this player (i) has this gun (spas)
		if (GunmenuData[idx][HasGun][i])
		{
			// Show them gunmenu
			ShowPlayerGunmenu(i, 0);
			va_SendClientMessageToAll(-1, COL_PRIM "\xBB "COL_SEC"%s " COL_PRIM "was shown the gunmenu because they had Spas.", Player[i][Name]);
		}
	}
	return 1;
}
