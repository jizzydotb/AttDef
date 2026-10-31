#if defined _attdef_OnVDamageStatusUpdate
	#endinput
#endif
#define _attdef_OnVDamageStatusUpdate

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
	UpdateVehicleHealthLabel(vehicleid, true);
	return 1;
}
