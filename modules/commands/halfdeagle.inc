#if defined _attdef_cmd_halfdeagle
	#endinput
#endif
#define _attdef_cmd_halfdeagle

GetAdjustedDeagleLimit()
{
	new attackerCount = 0, defenderCount = 0;
	foreach (new i : Player)
	{
		switch (Player[i][Team])
		{
			case ATTACKER: attackerCount++;
			case DEFENDER: defenderCount++;
		}
	}

	new maxTeamCount = max(attackerCount, defenderCount);
	if (maxTeamCount <= 4)
		return 1;

	new limit = 1 + ((maxTeamCount - 4) / 2);
	return limit > 10 ? 10 : limit;
}

GetDeagleGunmenuIndex()
{
	for (new idx = 0; idx < MAX_GUNMENU_GUNS; idx++)
	{
		if (GunmenuData[idx][GunID] == WEAPON_DEAGLE)
			return idx;
	}
	return -1;
}

AdjustDeagleLimit()
{
	return UpdateDeagleLimit(GetAdjustedDeagleLimit());
}

UpdateDeagleLimit(newLimit)
{
	new deagleIndex = GetDeagleGunmenuIndex();
	if (deagleIndex < 0)
		return 0;

	if (!DatabaseExecuteWrite(
		va_return("UPDATE `Gunmenu` SET `Limit`=%d WHERE `Weapon`=%d", newLimit, WEAPON_DEAGLE),
		"save Desert Eagle limit"))
		return 0;

	GunmenuData[deagleIndex][GunLimit] = newLimit;
	return 1;
}

SetHalfDeagleMode(bool: enabled, replacementWeapon = -1)
{
	new deagleIndex = GetDeagleGunmenuIndex();
	if (deagleIndex < 0)
		return 0;

	new limit = enabled ? GetAdjustedDeagleLimit() : 999;
	if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin Half Deagle update"))
		return 0;
	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %d WHERE Option = 'HalfDeagle'", enabled),
		"save Half Deagle mode"))
	{
		DatabaseExecuteWrite("ROLLBACK", "rollback Half Deagle update");
		return 0;
	}
	if (!DatabaseExecuteWrite(
		va_return("UPDATE `Gunmenu` SET `Limit`=%d WHERE `Weapon`=%d", limit, WEAPON_DEAGLE),
		"save Desert Eagle limit"))
	{
		DatabaseExecuteWrite("ROLLBACK", "rollback Half Deagle update");
		return 0;
	}
	if (replacementWeapon != -1 && !DatabaseExecuteWrite(
		va_return("UPDATE `Gunmenu` SET `Weapon`=%d WHERE `Weapon`=%d", replacementWeapon, WEAPON_DEAGLE),
		"replace Desert Eagle in Gunmenu"))
	{
		DatabaseExecuteWrite("ROLLBACK", "rollback Half Deagle update");
		return 0;
	}
	if (!DatabaseExecuteWrite("COMMIT", "commit Half Deagle update"))
	{
		DatabaseExecuteWrite("ROLLBACK", "rollback Half Deagle update");
		return 0;
	}

	HalfDeagle = enabled;
	GunmenuData[deagleIndex][GunLimit] = limit;
	if (replacementWeapon != -1)
		GunmenuData[deagleIndex][GunID] = replacementWeapon;
	return 1;
}

YCMD: halfdeagle(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Adjusts the Desert Eagle limit based on player count and team balance.");
	
	// Check if there is an active round
	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is active.");

	// Check if the Desert Eagle exists in the Gunmenu
	new deagleExists = false;
	for (new idx = 0; idx < MAX_GUNMENU_GUNS; idx++)
	{
		if (GunmenuData[idx][GunID] == WEAPON_DEAGLE)
		{
			deagleExists = true;
			break;
		}
	}

	// Exit if the Desert Eagle does not exist in the Gunmenu
	if (!deagleExists)
	{
		return SendErrorMessage(playerid, "Deagle is not available in the Gunmenu.");
	}

	new bool: enabled = !HalfDeagle;
	if (!SetHalfDeagleMode(enabled))
		return SendErrorMessage(playerid, "Could not save Half Deagle mode. No changes were applied.");

	LogAdminCommand("halfdeagle", playerid, INVALID_PLAYER_ID);
	va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Half Deagle Mode.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], HalfDeagle ? "{AADC93}enabled" : "{EB8989}disabled");
	return 1;
}
