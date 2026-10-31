#if defined _attdef_OnVehicleStreamIn
	#endinput
#endif
#define _attdef_OnVehicleStreamIn

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	#pragma unused forplayerid
	UpdateVehicleHealthLabel(vehicleid, true);
	return 1;
}
