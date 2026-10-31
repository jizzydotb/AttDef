#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_weaponbinds
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_weaponbinds
	#endinput
#endif
#define _attdef_impl_weaponbinds

/*
	Weapon key binds system (Dialog Based)
*/

InitPlayerWeaponBind(playerid)
{
	// Load default weapon key binds
	WeaponBindData[playerid][BindKeyWeapon][0] = WEAPON_SNIPER;
	WeaponBindData[playerid][BindKeyWeapon][1] = WEAPON_DEAGLE;
	WeaponBindData[playerid][BindKeyWeapon][2] = WEAPON_SHOTGUN;
	WeaponBindData[playerid][BindKeyWeapon][3] = WEAPON_SHOTGSPA;
	WeaponBindData[playerid][BindKeyWeapon][4] = WEAPON_M4;
}

// Shows the weapon key bind coder dialog
ShowPlayerWeaponBindDialog(playerid)
{
	new str[WEAPON_BIND_DIALOG_STRING_SIZE];
	format(str, sizeof str, "Key combination\tWeapon\n");
	format(str, sizeof str, Player[playerid][WeaponBinding] ? "%s{33CC00}[Active] Click to Disable\t \n" : "%s{CC0000}[Inactive] Click to Enable\t \n", str);
	for (new i = 0; i < MAX_WEAPON_BINDS; i++)
	{
		format(str, sizeof str, "%s%s+%s\t%s\n", str, WEAPON_BIND_PRIMARY_KEY_TEXT_ALT, WeaponBindKeyText_ALT[i], ReturnWeaponName(WeaponBindData[playerid][BindKeyWeapon][i]));
	}
	return ShowPlayerDialog(playerid, DIALOG_WEAPONBIND_MAIN, DIALOG_STYLE_TABLIST_HEADERS, "Weapon Binds", str, "Modify", "Close");
}

// Checks keys to see if a bind key combination exists in it
// Usually placed under OnPlayerKeyStateChange
CheckKeysForWeaponBind(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys
	if (!Player[playerid][WeaponBinding])
		return false;

	if (GetPlayerVehicleID(playerid))
		return false;

	if (!HOLDING(WEAPON_BIND_PRIMARY_KEY_VALUE))
		return false;

	for (new i = 0; i != MAX_WEAPON_BINDS; i++)
	{
		if (PRESSED(WeaponBindKeyValue[i]))
		{
			SetPlayerArmedWeapon(playerid, WeaponBindData[playerid][BindKeyWeapon][i]);
			return true;
		}
	}
	return false;
}
