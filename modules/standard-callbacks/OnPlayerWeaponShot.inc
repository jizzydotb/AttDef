#if defined _attdef_OnPWeaponShot
	#endinput
#endif
#define _attdef_OnPWeaponShot

public OnPlayerWeaponShot(playerid, WEAPON: weaponid, BULLET_HIT_TYPE: hittype, hitid, Float: fX, Float: fY, Float: fZ)
{
	#pragma unused weaponid, hittype, hitid, fX, fY, fZ

	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 1;

	EnsureAntiBugPlayerState(playerid);
	if (AntiBugFrozen[playerid])
		return 0;
	return 1;
}
