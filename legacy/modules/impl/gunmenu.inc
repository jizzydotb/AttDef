#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_gunmenu
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_gunmenu
	#endinput
#endif
#define _attdef_impl_gunmenu

/*
	Guns selection script
*/

#include <YSI_Coding\y_hooks>

new GunmenuShowGeneration[MAX_PLAYERS];
new GunmenuTimeoutTimer[MAX_PLAYERS];

hook OnPlayerDisconnect(playerid, reason)
{
	GunmenuShowGeneration[playerid]++;
	if (GunmenuTimeoutTimer[playerid])
	{
		KillTimer(GunmenuTimeoutTimer[playerid]);
		GunmenuTimeoutTimer[playerid] = 0;
	}
	// Reset player weapons on gunmenu
	ResetPlayerGunmenu(playerid, false);
	return 1;
}

LoadGunmenuData()
{
	new str[64], DBResult: res = DB_ExecuteQuery(sqliteconnection, "SELECT * FROM 'Gunmenu' ORDER BY Weapon ASC"),
						   weaponID,
						   idx = 0;

	do {
		DB_GetFieldStringByName(res, "Weapon", str, sizeof(str));
		weaponID				   = strval(str);

		GunmenuData[idx][GunID]	   = weaponID;
		GunmenuData[idx][GunModel] = GetWeaponModelFromID(weaponID);

		DB_GetFieldStringByName(res, "Limit", str, sizeof(str));
		GunmenuData[idx][GunLimit] = strval(str);

		DB_GetFieldStringByName(res, "Ammo", str, sizeof(str));
		GunmenuData[idx][GunAmmo] = strval(str);

		idx++;
	}
	while (DB_SelectNextRow(res) && idx < MAX_GUNMENU_GUNS);

	DB_FreeResultSet(res);
}

ResetGunmenuSelections(team = -1)
{
	switch (team)
	{
		case -1:
		{
			for (new i = 0; i < MAX_TEAMS; i++)
				for (new j = 0; j < MAX_GUNMENU_GUNS; j++)
					GunmenuData[j][TimesSelected][i] = 0;

			for (new playerid = 0; playerid < MAX_PLAYERS; playerid++)
			{
				for (new k = 0; k < MAX_GUNMENU_GUNS; k++)
					GunmenuData[k][HasGun][playerid] = false;
				GunmenuSelectionTeam[playerid] = -1;
			}
		}
		default:
		{
			for (new i = 0; i < MAX_GUNMENU_GUNS; i++) GunmenuData[i][TimesSelected][team] = 0;

			// Only wipe the picks that were counted for this team
			for (new playerid = 0; playerid < MAX_PLAYERS; playerid++)
			{
				if (GunmenuSelectionTeam[playerid] != team)
					continue;

				for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
					GunmenuData[i][HasGun][playerid] = false;
				GunmenuSelectionTeam[playerid] = -1;
			}
		}
	}
}

UpdateGunmenuLimitsTextDraw(updated_team = -1)
{
	#pragma unused updated_team
	return 1;
}

InitGunmenu()
{
	Iter_Init(PlayerGunObjects); // empty the iterator array
	LoadGunmenuData();
	ResetGunmenuSelections();
	UpdateGunmenuLimitsTextDraw();
}


ResetPlayerGunmenu(playerid, bool: showagain)
{
	// Picks are undone on the team they were counted for, not the current one,
	// otherwise a team swap/balance in between corrupts both teams' counters.
	new team = GunmenuSelectionTeam[playerid];
	if (team < 0 || team >= MAX_TEAMS)
		team = Player[playerid][Team];

	for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
	{
		if (GunmenuData[i][HasGun][playerid])
		{
			GunmenuData[i][HasGun][playerid] = false;
			if (GunmenuData[i][TimesSelected][team] > 0)
				GunmenuData[i][TimesSelected][team]--;
		}
	}
	GunmenuSelectionTeam[playerid] = -1;
	ResetPlayerWeapons(playerid);
	format(Player[playerid][PlayerTypeByWeapon], 16, "N/A");
	UpdateGunmenuLimitsTextDraw(team);
	if (showagain)
		ShowPlayerGunmenu(playerid, 0);
}

HidePlayerGunmenu(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (GunmenuTimeoutTimer[playerid])
	{
		KillTimer(GunmenuTimeoutTimer[playerid]);
		GunmenuTimeoutTimer[playerid] = 0;
	}
	GunmenuShowGeneration[playerid]++;
	if (!Player[playerid][OnGunmenu])
		return 1;
	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		FreezeV_GunmenuClose(vehicleid);
	}
	SetCameraBehindPlayer(playerid);
	SetPlayerArmedWeapon(playerid, 0);
	if (!RoundPaused)
		TogglePlayerControllable(playerid, 1);
	else
		TogglePlayerControllable(playerid, 0);
	Player[playerid][OnGunmenu] = false;
	return 1;
}

bool: DoesPlayerHaveGunmenuSlot(playerid, slot)
{
	for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
	{
		if (GunmenuData[i][HasGun][playerid] && GetWeaponSlot(GunmenuData[i][GunID]) == slot)
			return true;
	}
	return false;
}

bool: DoesPlayerHaveGunmenuWeapon(playerid, weaponid)
{
	for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
	{
		if (GunmenuData[i][HasGun][playerid] && GunmenuData[i][GunID] == weaponid)
			return true;
	}
	return false;
}

bool: IsGunSelectionOkay(playerid, weaponid, index)
{
	// Check whether this weapon slot is already selected in the current gunmenu session
	if (GunmenuData[index][HasGun][playerid] || DoesPlayerHaveGunmenuSlot(playerid, GetWeaponSlot(weaponid)))
	{
		SendErrorMessage(playerid, "You've already got a weapon in this slot.");
		return false;
	}
	new team = Player[playerid][Team]; // Player team
	// Each game type has its own rules and settings so we have to do some more checks...
	switch (GameType)
	{
		case BASE:
		{
			// Check if limit exceeded
			if (GunmenuData[index][TimesSelected][team] >= GunmenuData[index][GunLimit])
			{
				SendErrorMessage(
					playerid, va_return("This weapon is no longer available because limit has exceeded. [%d/%d]", GunmenuData[index][GunLimit], GunmenuData[index][GunLimit]));
				return false;
			}
			if (GunmenuRestrictions) // If gunmenu selection restriction is present
			{
				// Make sure players won't have Spas and Sniper/Deagle/M4
				if (weaponid == WEAPON_SHOTGSPA)
				{
					if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_SNIPER))
					{
						SendErrorMessage(playerid, "You cannot get Spas and Sniper!");
						return false;
					}
					else if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_DEAGLE))
					{
						SendErrorMessage(playerid, "You cannot get Spas and Deagle!");
						return false;
					}
					else if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_M4))
					{
						SendErrorMessage(playerid, "You cannot get Spas and M4!");
						return false;
					}
				}
				// Make sure players won't have Sniper and Spas/Deagle/M4
				else if (weaponid == WEAPON_SNIPER)
				{
					if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_SHOTGSPA))
					{
						SendErrorMessage(playerid, "You cannot get Spas and Sniper!");
						return false;
					}
					else if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_DEAGLE))
					{
						SendErrorMessage(playerid, "You cannot get Sniper and Deagle!");
						return false;
					}
					else if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_M4))
					{
						SendErrorMessage(playerid, "You cannot get Sniper and M4!");
						return false;
					}
				}
				// Make sure players won't have Deagle and Spas/Sniper/M4
				else if (weaponid == WEAPON_DEAGLE)
				{
					if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_SHOTGSPA))
					{
						SendErrorMessage(playerid, "You cannot get Spas and Deagle!");
						return false;
					}
					else if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_SNIPER))
					{
						SendErrorMessage(playerid, "You cannot get Deagle and Sniper!");
						return false;
					}
					else if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_M4))
					{
						SendErrorMessage(playerid, "You cannot get Deagle and M4!");
						return false;
					}
				}
				// Make sure players won't have M4 and Spas/Deagle/Sniper/Rifle
				else if (weaponid == WEAPON_M4)
				{
					if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_SHOTGSPA))
					{
						SendErrorMessage(playerid, "You cannot get M4 and Spas!");
						return false;
					}
					else if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_DEAGLE))
					{
						SendErrorMessage(playerid, "You cannot get M4 and Deagle!");
						return false;
					}
					else if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_SNIPER))
					{
						SendErrorMessage(playerid, "You cannot get M4 and Sniper!");
						return false;
					}
					else if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_RIFLE))
					{
						SendErrorMessage(playerid, "You cannot get Rifle and M4!");
						return false;
					}
				}
				// Make sure players won't have Rifle and M4
				else if (weaponid == WEAPON_RIFLE)
				{
					if (DoesPlayerHaveGunmenuWeapon(playerid, WEAPON_M4))
					{
						SendErrorMessage(playerid, "You cannot get Rifle and M4!");
						return false;
					}
				}
			}
		}
		case ARENA:
		{
			// Check if limit exceeded
			if (GunmenuData[index][GunLimit] <= 0)
			{
				SendErrorMessage(playerid, "This weapon is disabled.");
				return false;
			}
			// Make sure players won't have Spas in Arenas
			if (weaponid == WEAPON_SHOTGSPA)
			{
				SendErrorMessage(playerid, "Spas is not allowed in Arenas!");
				return false;
			}
		}
	}
	return true;
}

stock GiveDefaultRoundWeapons(playerid)
{
	new team = Player[playerid][Team];
	if (team != ATTACKER && team != DEFENDER)
		return 0;

	if (!DoesPlayerHaveGunmenuSlot(playerid, GetWeaponSlot(WEAPON_DEAGLE)))
	{
		for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
		{
			if (GunmenuData[i][GunID] == WEAPON_DEAGLE)
			{
				if (GunmenuData[i][TimesSelected][team] < GunmenuData[i][GunLimit])
				{
					GivePlayerGunmenuWeapon(playerid, i);
					break;
				}
			}
		}
	}
	if (!DoesPlayerHaveGunmenuSlot(playerid, GetWeaponSlot(WEAPON_SHOTGUN)))
	{
		for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
		{
			if (GunmenuData[i][GunID] == WEAPON_SHOTGUN)
			{
				if (GunmenuData[i][TimesSelected][team] < GunmenuData[i][GunLimit])
				{
					GivePlayerGunmenuWeapon(playerid, i);
					break;
				}
			}
		}
	}
	new ct = 0;
	for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
	{
		if (GunmenuData[i][HasGun][playerid])
			ct++;
	}
	if (ct < 2 && !DoesPlayerHaveGunmenuSlot(playerid, GetWeaponSlot(WEAPON_M4)))
	{
		for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
		{
			if (GunmenuData[i][GunID] == WEAPON_M4)
			{
				if (GunmenuData[i][TimesSelected][team] < GunmenuData[i][GunLimit])
				{
					GivePlayerGunmenuWeapon(playerid, i);
					break;
				}
			}
		}
	}
	if (GetPlayerWeapon(playerid) == 0)
	{
		GivePlayerWeapon(playerid, WEAPON_DEAGLE, 999);
		GivePlayerWeapon(playerid, WEAPON_SHOTGUN, 999);
	}
	return 1;
}

forward OnGunmenuTimeout(playerid, showGen);
public OnGunmenuTimeout(playerid, showGen)
{
	GunmenuTimeoutTimer[playerid] = 0;
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;
	if (GunmenuShowGeneration[playerid] != showGen || !Player[playerid][OnGunmenu])
		return 0;
	if (Current == -1 || !Player[playerid][Playing])
	{
		HidePlayerGunmenu(playerid);
		HidePlayerDialog(playerid);
		return 0;
	}

	new ct = 0;
	for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
	{
		if (GunmenuData[i][HasGun][playerid])
			ct++;
	}

	if (ct < 2)
	{
		GiveDefaultRoundWeapons(playerid);
		SendErrorMessage(playerid, "Gunmenu time expired! Default weapons equipped.");
	}

	HidePlayerDialog(playerid);
	OnPlayerFinishGunmenu(playerid);
	return 1;
}

ShowGunmenuDialog(playerid1)
{
	inline InlineShowGunmenu(playerid, dialogid, response, listitem, string: inputtext[])
	{
		#pragma unused dialogid, inputtext
		if (listitem < 0 || listitem >= MAX_GUNMENU_GUNS)
			return 1;

		if (!Player[playerid][OnGunmenu])
		{
			if (Current != -1 && Player[playerid][Playing])
				Player[playerid][OnGunmenu] = true;
			else
				return 1;
		}

		if (!response)
		{
			ResetPlayerGunmenu(playerid, true);
			va_SendClientMessage(playerid, -1, "\xBB " COL_PRIM "Inventory: " COL_SEC "Your weapon inventory has been reset.");
			return 1;
		}
		// Clicked Pick
		if (IsGunSelectionOkay(playerid, GunmenuData[listitem][GunID], listitem))
		{
			GivePlayerGunmenuWeapon(playerid, listitem);
			PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
		}
		else
			ShowGunmenuDialog(playerid);
	}

	new str[800];
	new team = Player[playerid1][Team];
	new primaryColor[10], secondaryColor[10];
	if (team == ATTACKER)
	{
		format(primaryColor, sizeof(primaryColor), TextColor[ATTACKER]);
		format(secondaryColor, sizeof(secondaryColor), TextColor[ATTACKER_SUB]);
	}
	else if (team == DEFENDER)
	{
		format(primaryColor, sizeof(primaryColor), TextColor[DEFENDER]);
		format(secondaryColor, sizeof(secondaryColor), TextColor[DEFENDER_SUB]);
	}
	else
	{
		format(primaryColor, sizeof(primaryColor), COL_PRIM);
		format(secondaryColor, sizeof(secondaryColor), COL_THR);
	}

	switch (GameType)
	{
		case BASE:
		{
			format(str, sizeof str, "{FFFFFF}Weapon\t{FFFFFF}Ammo\t{FFFFFF}Limit");

			for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
			{
				if (GunmenuData[i][HasGun][playerid1])
				{
					format(
						str, sizeof str, "%s\n{FFFFFF}%s\t{FFFFFF}%d\t{FFFFFF}%d / %d", str, ReturnWeaponName(GunmenuData[i][GunID]), GunmenuData[i][GunAmmo],
						GunmenuData[i][TimesSelected][team], GunmenuData[i][GunLimit]);
				}
				else if (GunmenuData[i][TimesSelected][team] < GunmenuData[i][GunLimit])
				{
					new rowColor[10];
					if (i % 2 == 0)
						format(rowColor, sizeof(rowColor), "%s", primaryColor);
					else
						format(rowColor, sizeof(rowColor), "%s", secondaryColor);

					format(
						str, sizeof str, "%s\n%s%s\t%s%d\t%s%d / %d", str, rowColor, ReturnWeaponName(GunmenuData[i][GunID]), rowColor, GunmenuData[i][GunAmmo],
						rowColor, GunmenuData[i][TimesSelected][team], GunmenuData[i][GunLimit]);
				}
				else
				{
					format(
						str, sizeof str, "%s\n{696969}%s\t{696969}%d\t{696969}%d / %d", str, ReturnWeaponName(GunmenuData[i][GunID]), GunmenuData[i][GunAmmo],
						GunmenuData[i][TimesSelected][team], GunmenuData[i][GunLimit]);
				}
			}
		}
		case ARENA:
		{
			format(str, sizeof str, "{FFFFFF}Weapon\t{FFFFFF}Times Picked");

			for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
			{
				if (GunmenuData[i][HasGun][playerid1])
					format(str, sizeof str, "%s\n{FFFFFF}%s\t{FFFFFF}%d", str, ReturnWeaponName(GunmenuData[i][GunID]), GunmenuData[i][TimesSelected][team]);
				else if (GunmenuData[i][GunLimit] <= 0)
					format(str, sizeof str, "%s\n{696969}%s\t{696969}%d", str, ReturnWeaponName(GunmenuData[i][GunID]), GunmenuData[i][TimesSelected][team]);
				else
				{
					new rowColor[10];
					if (i % 2 == 0)
						format(rowColor, sizeof(rowColor), "%s", primaryColor);
					else
						format(rowColor, sizeof(rowColor), "%s", secondaryColor);

					format(str, sizeof str, "%s\n%s%s\t%s%d", str, rowColor, ReturnWeaponName(GunmenuData[i][GunID]), rowColor, GunmenuData[i][TimesSelected][team]);
				}
			}
		}
	}
	Dialog_ShowCallback(playerid1, using inline InlineShowGunmenu, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Select your weapons:", str, "Pick", "Reset");
	return 1;
}

forward ShowPlayerGunmenu(playerid, delay);
public ShowPlayerGunmenu(playerid, delay)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	GunmenuShowGeneration[playerid]++;
	if (Player[playerid][OnGunmenu])
		HidePlayerGunmenu(playerid);

	if (delay > 0)
	{
		if (!SetTimerEx(
			"ShowPlayerGunmenuDelayed", delay, false, "iii",
			playerid, PlayerConnectionGeneration[playerid], GunmenuShowGeneration[playerid]))
		{
			printf("[AttDef] Failed to schedule gunmenu for player %d.", playerid);
			return 0;
		}
		return 1;
	}
	if (GunmenuTimeoutTimer[playerid])
	{
		KillTimer(GunmenuTimeoutTimer[playerid]);
		GunmenuTimeoutTimer[playerid] = 0;
	}
	if (Current != -1 && Player[playerid][Playing] && !AllVsAllMode)
	{
		new remaining = (gGunmenuTime > ElapsedTime) ? (gGunmenuTime - ElapsedTime) : 10;
		if (remaining < 10)
			remaining = 10;
		GunmenuTimeoutTimer[playerid] = SetTimerEx(
			"OnGunmenuTimeout", remaining * 1000, false, "ii",
			playerid, GunmenuShowGeneration[playerid]);
	}

	Player[playerid][OnGunmenu] = true; // Indicate that they're on the gunmenu selection mode
	ResetPlayerGunmenu(playerid, false);
	new veh = GetPlayerVehicleID(playerid);
	if (veh && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		FreezeV_GunmenuOpen(veh);
	}
	ShowGunmenuDialog(playerid);
	return 1;
}

forward ShowPlayerGunmenuDelayed(playerid, connectionGeneration, showGeneration);
public ShowPlayerGunmenuDelayed(playerid, connectionGeneration, showGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration
		|| GunmenuShowGeneration[playerid] != showGeneration)
		return 0;
	return ShowPlayerGunmenu(playerid, 0);
}

OnGunObjectMovedToPlayer(playerid, objectid, index)
{
	SetPlayerArmedWeapon(playerid, GunmenuData[index][GunID]);
	new tmpWeapID, tmpAmmo, slot = GetWeaponSlot(GunmenuData[index][GunID]);
	new str[256];
	format(str, sizeof str, "~w~");
	for (new i = 1; i < 13; i++)
	{
		if (i == slot)
		{
			tmpWeapID = GunmenuData[index][GunID];
			tmpAmmo	  = GunmenuData[index][GunAmmo];
		}
		else
			GetPlayerWeaponData(playerid, i, tmpWeapID, tmpAmmo);

		if (tmpWeapID == 0)
			continue;

		if (tmpWeapID == GunmenuData[index][GunID])
			format(str, sizeof str, "%s~g~~h~~h~%s ~h~(newly added)~w~~n~", str, ReturnWeaponName(tmpWeapID));
		else
			format(str, sizeof str, "%s%s~n~", str, ReturnWeaponName(tmpWeapID));
	}
	va_SendClientMessage(playerid, -1, "\xBB " COL_PRIM "Inventory: " COL_SEC "%s " COL_PRIM "added to your weapons.", ReturnWeaponName(GunmenuData[index][GunID]));
	if (IsBulletWeapon(GunmenuData[index][GunID]))
	{
		ApplyAnimation(playerid, "PYTHON", "python_reload", 4.1, 0, 0, 0, 0, 0, 1);
		PlayerPlaySound(playerid, 36401, 0.0, 0.0, 0.0);
	}
	Iter_Remove(PlayerGunObjects[playerid], objectid);
	DestroyPlayerObject(playerid, objectid);
	DestroyPlayerObject(playerid, GunmenuData[index][GunFlamePlayerObject][playerid]);
	return 1;
}

GivePlayerGunmenuWeapon(playerid, index)
{
	new weaponid = GunmenuData[index][GunID], team = Player[playerid][Team];

	// Never count the same pick twice nor allow more than 2 picks
	if (GunmenuData[index][HasGun][playerid])
		return 0;
	new picks = 0;
	for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
	{
		if (GunmenuData[i][HasGun][playerid])
			picks++;
	}
	if (picks >= 2)
		return 0;

	// Picks from a different team (stale data) are discarded before counting new ones
	if (GunmenuSelectionTeam[playerid] != -1 && GunmenuSelectionTeam[playerid] != team)
		ResetPlayerGunmenu(playerid, false);
	GunmenuSelectionTeam[playerid] = team;

	if (weaponid != WEAPON_FIST && DoesPlayerHaveWeapon(playerid, weaponid))
		RemovePlayerWeapon(playerid, weaponid);

	GunmenuData[index][HasGun][playerid] = true; // Indicate that they have this weapon now from gunmenu
	GunmenuData[index][TimesSelected][team]++;	 // Indicate how many times this weapon is selected in this team (by increasing 1 everytime a player selects)
	if (weaponid != WEAPON_FIST)
		GivePlayerWeapon(playerid, weaponid, GunmenuData[index][GunAmmo]);
	SetPlayerArmedWeapon(playerid, 0);

	switch (weaponid)
	{
		case WEAPON_DEAGLE: format(Player[playerid][PlayerTypeByWeapon], 16, "Deagler");
		case WEAPON_SHOTGSPA: format(Player[playerid][PlayerTypeByWeapon], 16, "Spasser");
		case WEAPON_M4: format(Player[playerid][PlayerTypeByWeapon], 16, "M4~er");
		case WEAPON_SNIPER: format(Player[playerid][PlayerTypeByWeapon], 16, "Sniper");
		case WEAPON_AK47: format(Player[playerid][PlayerTypeByWeapon], 16, "AK~er");
		case WEAPON_MP5: format(Player[playerid][PlayerTypeByWeapon], 16, "MP5~er");
		case WEAPON_SHOTGUN: format(Player[playerid][PlayerTypeByWeapon], 16, "Shotgunner");
		case WEAPON_RIFLE: format(Player[playerid][PlayerTypeByWeapon], 16, "Rifler");
		case WEAPON_SILENCED: format(Player[playerid][PlayerTypeByWeapon], 16, "Silenced");
	}

	// Update limits for other players (for both styles: dialog and objects)
	UpdateGunmenuLimitsTextDraw(Player[playerid][Team]);
	// Count how many weapons they've got so far
	new ct = 0;
	for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
	{
		if (GunmenuData[i][HasGun][playerid])
			ct++;
	}
	// Check if they've selected enough weapons
	if (ct >= 2)
		OnPlayerFinishGunmenu(playerid);
	else
		ShowGunmenuDialog(playerid);
	return 1;
}

// Reapply an already chosen inventory after an All-vs-All respawn. This does
// not touch HasGun/TimesSelected, so respawning never reopens or resets menu.
RestorePlayerGunmenuWeapons(playerid)
{
	ResetPlayerWeapons(playerid);
	for (new i; i < MAX_GUNMENU_GUNS; i++)
	{
		if (GunmenuData[i][HasGun][playerid] && GunmenuData[i][GunID] != WEAPON_FIST)
			GivePlayerWeapon(playerid, GunmenuData[i][GunID], GunmenuData[i][GunAmmo]);
	}
	SetPlayerArmedWeapon(playerid, WEAPON_UNARMED);
	return 1;
}

OnPlayerFinishGunmenu(playerid)
{
	new bool: allVsAll = AllVsAllMode && GameType == ARENA;
	if (allVsAll && Player[playerid][Playing])
		Player[playerid][WeaponPicked] = 1;
	HidePlayerGunmenu(playerid);

	new team = Player[playerid][Team];
	if (Current != -1 && Player[playerid][Playing] && (GameType == BASE || GameType == ARENA))
	{
		new str[200];
		format(str, sizeof str, "%s%s has chosen: {FFFFFF}", TextColor[team], Player[playerid][Name]);
		new count, weaponid;
		for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
		{
			if (GunmenuData[i][HasGun][playerid])
			{
				weaponid = GunmenuData[i][GunID];
				format(str, sizeof str, "%s%s%s(%d)",
					str, count > 0 ? " / " : "",
					weaponid == 0 ? "Fist" : ReturnWeaponName(weaponid),
					GunmenuData[i][TimesSelected][team]);
				count++;
			}
		}

		if (count > 0)
		{
			foreach (new i : Player)
			{
				if ((allVsAll && Player[i][Playing]) || (!allVsAll && IsTeamTheSame(team, Player[i][Team])))
					SendClientMessage(i, -1, str);
			}
		}
	}
	return 1;
}

forward ShowPlayerMeleeWeaponsMenu(playerid);
public ShowPlayerMeleeWeaponsMenu(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	new str[256], weap;
	for (new i = 0; i != sizeof MeleeWeaponsArray_ID; ++i)
	{
		weap = MeleeWeaponsArray_ID[i];
		format(str, sizeof str, "%s\n%s%s", str, DoesPlayerHaveWeapon(playerid, weap) ? COL_ERROR : COL_THR, ReturnWeaponName(weap));
	}
	ShowPlayerDialog(playerid, DIALOG_GUNMENU_MELEE, DIALOG_STYLE_LIST, "Melee weapons", str, "Get", "Close");
	return 1;
}

forward ShowPlayerMeleeWeaponsMenuDelayed(playerid, generation);
public ShowPlayerMeleeWeaponsMenuDelayed(playerid, generation)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != generation)
		return 0;
	return ShowPlayerMeleeWeaponsMenu(playerid);
}
