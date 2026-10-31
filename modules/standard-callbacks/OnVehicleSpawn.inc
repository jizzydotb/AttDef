#if defined _attdef_OnVehicleSpawn
	#endinput
#endif
#define _attdef_OnVehicleSpawn

public OnVehicleSpawn(vehicleid)
{
	UpdateVehicleHealthLabel(vehicleid, true);
	return 1;
}
