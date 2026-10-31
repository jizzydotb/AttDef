#if defined _attdef_OnPUpdate
	#endinput
#endif
#define _attdef_OnPUpdate

static const PARACHUTE_SLOT = 11;

public OnPlayerUpdate(playerid)
{
	// Player is sending updates, hence they're not paused
	Player[playerid][PauseCount] = 0;
	Player[playerid][gLastUpdate] = GetTickCount();

	if (IsPlayerAdmin(playerid) && Player[playerid][Level] < 5)
	{
		Player[playerid][Level] = 5;
		UpdatePlayerAdminGroup(playerid);
		if (Player[playerid][Logged])
		{
			DatabaseExecuteWrite(
				va_return("UPDATE Players SET Level = 5 WHERE Name = '%q'", Player[playerid][Name]),
				"save RCON admin level");
		}
	}

	ProcessAntiBugPlayerUpdate(playerid);

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid != INVALID_VEHICLE_ID && vehicleid != 0 && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		UpdateVehicleHealthLabel(vehicleid);

	//remove parachute functions
	new weaponid, ammo;
	GetPlayerWeaponData(playerid, PARACHUTE_SLOT, weaponid, ammo);
	if (ammo && weaponid == WEAPON_PARACHUTE)
	{
		switch (GetPlayerAnimationIndex(playerid))
		{
			case 971: // PARA_open
			{
				RemovePlayerWeapon(playerid, weaponid);
				SetPlayerVelocity(playerid, 0, 0, 0);
				ClearAnimations(playerid);
			}
			case 1231: // run_player
			{
				RemovePlayerWeapon(playerid, weaponid);
			}
		}
	}
	return 1;
}
