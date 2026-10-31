#if defined _attdef_common_vehicles
	#endinput
#endif
#define _attdef_common_vehicles

/*
	Vehicle Health Labels, Component Legality & Helpers Sub-module
*/

public AttachHealthTextLabelToVehicle(vehicleid)
{
	if (!IsValidVehicle(vehicleid))
		return 0;

	new Float: vehicleHealth;
	new Float: posX, Float: posY, Float: posZ;
	new labelText[32];
	GetVehicleHealth(vehicleid, vehicleHealth);

	if (IsValid3DTextLabel(VehicleHealthText3D[vehicleid]))
	{
		if (vehicleHealth != lastVehicleHealth[vehicleid])
		{
			format(labelText, sizeof(labelText), "Health: %.0f%%", vehicleHealth / 10);
			Update3DTextLabelText(VehicleHealthText3D[vehicleid], 0xDC143CFF, labelText);
			lastVehicleHealth[vehicleid] = vehicleHealth;
		}
		return 1;
	}

	GetVehiclePos(vehicleid, posX, posY, posZ);
	format(labelText, sizeof(labelText), "Health: %.0f%%", vehicleHealth / 10);

	VehicleHealthText3D[vehicleid] = Create3DTextLabel(labelText, 0xDC143CFF, posX, posY, posZ + 1.0, 25.0, GetVehicleVirtualWorld(vehicleid), true);
	if (IsValid3DTextLabel(VehicleHealthText3D[vehicleid]))
	{
		Attach3DTextLabelToVehicle(VehicleHealthText3D[vehicleid], vehicleid, 0.0, 0.0, 0.0);
		lastVehicleHealth[vehicleid] = vehicleHealth;
		Iter_Add(VehicleHealthLabels, vehicleid);
	}

	return IsValid3DTextLabel(VehicleHealthText3D[vehicleid]);
}

UpdateVehicleHealthLabel(vehicleid, bool: force = false)
{
	new Float:vehHealth;
	new labelText[128];
	new currentTick = GetTickCount();

	if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
		return 0;

	if (!IsValidVehicle(vehicleid))
	{
		if (IsValid3DTextLabel(VehicleHealthText3D[vehicleid]))
			Delete3DTextLabel(VehicleHealthText3D[vehicleid]);
		VehicleHealthText3D[vehicleid] = INVALID_3DTEXT_ID;
		lastVehicleHealth[vehicleid] = 0.0;
		lastVehicleHealthPollTick[vehicleid] = 0;
		Iter_Remove(VehicleHealthLabels, vehicleid);
		return 0;
	}

	if (!force && currentTick - lastVehicleHealthPollTick[vehicleid] < 50)
		return 1;
	lastVehicleHealthPollTick[vehicleid] = currentTick;

	GetVehicleHealth(vehicleid, vehHealth);
	new bool:createdLabel = !IsValid3DTextLabel(VehicleHealthText3D[vehicleid]);
	if (createdLabel)
		AttachHealthTextLabelToVehicle(vehicleid);

	if (IsValid3DTextLabel(VehicleHealthText3D[vehicleid]) && vehHealth == lastVehicleHealth[vehicleid])
		return 1;

	if (IsValid3DTextLabel(VehicleHealthText3D[vehicleid]))
	{
		lastVehicleHealth[vehicleid] = vehHealth;
		format(labelText, sizeof(labelText), "Health: %.0f%%", vehHealth / 10);
		Update3DTextLabelText(VehicleHealthText3D[vehicleid], 0xDC143CFF, labelText);
	}

	return 1;
}

forward UpdateVehicleHealthLabels();
public UpdateVehicleHealthLabels()
{
	if (!VehicleHealthTextdraw)
		return 1;

	foreach (new vehicleid : Vehicle)
		UpdateVehicleHealthLabel(vehicleid, true);
	return 1;
}

forward UpdateExistingVehicleHealthLabels();
public UpdateExistingVehicleHealthLabels()
{
	foreach (new vehicleid : VehicleHealthLabels)
		UpdateVehicleHealthLabel(vehicleid);
	return 1;
}

forward DestroyVehicleHealthLabels();
public DestroyVehicleHealthLabels()
{
	for (new i = 0; i < MAX_VEHICLES; i++)
	{
		if (IsValid3DTextLabel(VehicleHealthText3D[i]))
		{
			Delete3DTextLabel(VehicleHealthText3D[i]);
			VehicleHealthText3D[i] = INVALID_3DTEXT_ID;
		}
	}
	Iter_Clear(VehicleHealthLabels);
	return 1;
}


forward UpdateBaseIDTextDraw(playerid);

public RespawnInVehicleAfterComeBack(playerid, vehicleid, seatid, connectionGeneration, rejoinGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration
		|| PlayerVehicleRejoinGeneration[playerid] != rejoinGeneration
		|| !Player[playerid][Playing] || Current == -1 || !IsValidVehicle(vehicleid))
		return 0;

	new ct = 0;
	foreach (new k : Player)
	{
		if (GetPlayerVehicleID(k) == vehicleid && GetPlayerVehicleSeat(k) == seatid)
			ct++;
	}
	if (ct == 0)
		PutPlayerInVehicle(playerid, vehicleid, seatid);
	return 1;
}


GetVehicleModelID(const vehiclename[])
{
	for (new i = 0; i < 211; i++)
	{
		if (strfind(aVehicleNames[i], vehiclename, true) != -1)
			return i + 400;
	}
	return -1;
}

stock bool: IsAirVehicle(vehicleid)
{
	if (vehicleid == 0 || vehicleid == INVALID_VEHICLE_ID)
		return false;

	switch (GetVehicleModel(vehicleid))
	{
		case 417, 425, 447, 460, 469, 476, 487, 488, 497, 511, 512, 513, 519, 520, 548, 553, 563, 577, 592, 593:
		{
			return true;
		}
	}
	return false;
}


bool: IsVehicleComponentLegal(vehiclemodel, componentid)
{
	// These is the only case with componentids > 1191 (saves ~1kb in the array)
	if (vehiclemodel == 576 && (componentid == 1192 || componentid == 1193))
		return true;

	new s_LegalMods[][] = { { 54273792, 0, 16776704, 7, 0, 0 },
							{ 35268602, 0, 16776704, 7, 245760, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 37431173, 0, 16776704, 7, 0, 0 },
							{ 45893379, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 62531466, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 42862474, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 36767556, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 36177722, 0, 16776704, 7, 0, 0 },
							{ 45958913, 0, 16776704, 7, 0, 0 },
							{ 37365632, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 36177786, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 41560010, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 42084234, 0, 16776704, 7, 245760, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 37619648, 0, 16776704, 7, 0, 0 },
							{ 57685808, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 52242293, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 46024584, 0, 16776704, 7, 245760, 0 },
							{ 33621873, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 43651022, 0, 16776704, 7, 49152, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 54011648, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 37717909, 0, 16776704, 7, 0, 0 },
							{ 43976588, 0, 16776704, 7, 245760, 0 },
							{ 43395050, 0, 16776704, 7, 245760, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 37144450, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 43917258, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, -67107785, 0, 35389440 },
							{ 33556224, 0, 16776704, 67002375, 0, 0 },
							{ 33556224, 0, 16776704, 7047, 1, 31457280 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 60688338, 0, 16776704, 7, 245760, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 37537536, 0, 16776704, 7, 196608, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 59639766, 0, 16776704, 7, 245760, 0 },
							{ 37553929, 0, 16776704, 7, 49152, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 43917194, 0, 16776704, 7, 245760, 0 },
							{ 43779962, 0, 16776704, 7, 245760, 0 },
							{ 45942636, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, -512, 7, 0, 504 },
							{ 33556224, 0, 16777214, 7, -1073741824, 8199 },
							{ -33552640, 3, 16776704, 7, 15360, 1536 },
							{ -838859008, -8388608, 16776705, 7, 1006632960, 0 },
							{ 33556224, 1020, 16776704, 7, 3932160, 6144 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 8380416, 16776704, 7, 62914560, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 71, 62, 1006632960 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 7168, 16776704, 15, 0, 245760 },
							{ 33556224, 0, 16776704, 7, 960, -1073741824 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 43386818, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 51849201, 0, 16776704, 7, 196608, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 0, 0, 0, 0, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 39200752, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 60688322, 0, 16776704, 7, 245760, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 },
							{ 33556224, 0, 16776704, 7, 0, 0 } };
	if (1000 <= componentid <= 1191 && 400 <= vehiclemodel <= 611)
	{
		componentid -= 1000;
		vehiclemodel -= 400;

		return (s_LegalMods[vehiclemodel][componentid >>> 5] & 1 << (componentid & 31)) || false;
	}
	return false;
}


stock DestroyDynamicVehicles(whatvehicles)
{
	switch (whatvehicles)
	{
		case 0: // all
		{
			foreach (new i : DynamicVehicle)
			{
				RemoveVehicle(i, false);
				Iter_Remove(DynamicVehicle, i);
			}
		}
		case 1: // unoccupied only
		{
			new bool: foundDriver;
			foreach (new i : DynamicVehicle)
			{
				foundDriver = false;
				foreach (new j : Player)
				{
					if (GetPlayerVehicleID(j) == i && GetPlayerState(j) == PLAYER_STATE_DRIVER)
					{
						foundDriver = true;
						break;
					}
				}
				if (!foundDriver)
				{
					RemoveVehicle(i, false);
					Iter_Remove(DynamicVehicle, i);
				}
			}
		}
		case 2: // occupied only
		{
			new bool: foundDriver;
			foreach (new i : DynamicVehicle)
			{
				foundDriver = false;
				foreach (new j : Player)
				{
					if (GetPlayerVehicleID(j) == i && GetPlayerState(j) == PLAYER_STATE_DRIVER)
					{
						foundDriver = true;
						break;
					}
				}
				if (foundDriver)
				{
					RemoveVehicle(i, false);
					Iter_Remove(DynamicVehicle, i);
				}
			}
		}
	}
	return 1;
}

forward AimbotCheck(playerid);
