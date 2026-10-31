#if defined _attdef_cmd_giveweapon
	#endinput
#endif
#define _attdef_cmd_giveweapon

YCMD: giveweapon(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "give a weapon to a specific player.");
	new pID, Weapon[50], Ammo;
	if (sscanf(params, "rs[*]I(250)", pID, sizeof(Weapon), Weapon, Ammo))
		return SendUsageMessage(playerid, "/giveweapon [Player Name/ID] [Weapon Name] [Optional: Ammo]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isn't connected.");



	Ammo = clamp(Ammo, 10, 9999);
	new WeaponID = GetWeaponID(Weapon);
	if (!IsValidWeapon(WeaponID))
		return SendErrorMessage(playerid, "Invalid Weapon Name.");

	GivePlayerWeapon(pID, WeaponID, Ammo);

	LogAdminCommand("giveweapon", playerid, pID);

	// Adjusted messaging
	if (pID == playerid)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"equipped a "COL_SEC"%s", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], ReturnWeaponName(WeaponID));
	}
	else
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "gave "COL_SEC"%s " COL_PRIM "a "COL_SEC"%s" COL_PRIM ".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name],
		Player[pID][Name], ReturnWeaponName(WeaponID));
	}

}
