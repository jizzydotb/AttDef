#if defined vehicle_freeze_included_
	#endinput
#endif
#define vehicle_freeze_included_

#include <YSI_Coding\y_hooks>

static enum (<<= 1)
{
	FREEZE_PAUSE = 1,
	FREEZE_GUNMENU
}

static Iterator: s_RoundVehicles<MAX_VEHICLES>;
static Iterator: s_FrozenVehicles<MAX_VEHICLES>;
static s_FreezeFlags[MAX_VEHICLES char] = { 0, ... };
static s_FreezeTimers[MAX_VEHICLES]		= { 0, ... };
static s_VehicleFreezeGeneration[MAX_VEHICLES];
static Float: s_VehicleSpeed[MAX_VEHICLES][3];

stock FreezeV_CreateVehicle(bool: isForRound, vehicletype, Float: x, Float: y, Float: z, Float: rotation, color1, color2, respawn_delay, addsiren = 0)
{
	new ret = CreateVehicle(vehicletype, x, y, z, rotation, color1, color2, respawn_delay, addsiren);
	if (ret && ret != INVALID_VEHICLE_ID)
	{
		if (s_FreezeTimers[ret])
		{
			KillTimer(s_FreezeTimers[ret]);
			s_FreezeTimers[ret] = 0;
		}
		Iter_Remove(s_FrozenVehicles, ret);
		Iter_Remove(s_RoundVehicles, ret);
		s_FreezeFlags { ret } = 0;
		s_VehicleFreezeGeneration[ret]++;
		if (isForRound)
		{
			Iter_Add(s_RoundVehicles, ret);
		}
	}
	return ret;
}

hook FreezeV_Timer(vehicleid, generation, Float: x, Float: y, Float: z)
{
	if (vehicleid < 1 || vehicleid >= MAX_VEHICLES || generation != s_VehicleFreezeGeneration[vehicleid])
		return 0;
	if (!IsValidVehicle(vehicleid))
	{
		if (s_FreezeTimers[vehicleid])
			KillTimer(s_FreezeTimers[vehicleid]);
		s_FreezeTimers[vehicleid] = 0;
		s_FreezeFlags { vehicleid } = 0;
		Iter_Remove(s_FrozenVehicles, vehicleid);
		Iter_Remove(s_RoundVehicles, vehicleid);
		return 0;
	}

	SetVehiclePos(vehicleid, x, y, z);
	return 1;
}

static stock FreezeV_AddFlag(vehicleid, flag)
{
	if (vehicleid < 1 || vehicleid >= MAX_VEHICLES || !IsValidVehicle(vehicleid))
		return 0;

	if (s_FreezeFlags { vehicleid } == 0)
	{
		new Float: x,
			Float: y,
			Float: z;
		GetVehiclePos(vehicleid, x, y, z);
		GetVehicleVelocity(vehicleid, s_VehicleSpeed[vehicleid][0], s_VehicleSpeed[vehicleid][1], s_VehicleSpeed[vehicleid][2]);
		s_FreezeTimers[vehicleid] = SetTimerEx(
			"FreezeV_Timer", 50, true, "iifff", vehicleid,
			s_VehicleFreezeGeneration[vehicleid], x, y, z);
		if (!s_FreezeTimers[vehicleid])
		{
			printf("[AttDef] Failed to start freeze timer for vehicle %d.", vehicleid);
			return 0;
		}
		Iter_Add(s_FrozenVehicles, vehicleid);
	}
	s_FreezeFlags { vehicleid } |= flag;
	return 1;
}

static stock FreezeV_RemoveFlag(vehicleid, flag)
{
	if (s_FreezeFlags { vehicleid } && (s_FreezeFlags { vehicleid } &= ~flag) == 0)
	{
		if (s_FreezeTimers[vehicleid])
			KillTimer(s_FreezeTimers[vehicleid]);
		s_FreezeTimers[vehicleid] = 0;
		if (IsValidVehicle(vehicleid))
			SetVehicleVelocity(vehicleid, s_VehicleSpeed[vehicleid][0], s_VehicleSpeed[vehicleid][1], s_VehicleSpeed[vehicleid][2]);
		Iter_Remove(s_FrozenVehicles, vehicleid);
	}
	return 1;
}

stock FreezeV_PauseRound()
{
	foreach (new vehicleid : s_RoundVehicles) { FreezeV_AddFlag(vehicleid, FREEZE_PAUSE); }
}

stock FreezeV_UnpauseRound()
{
	foreach (new vehicleid : s_FrozenVehicles) { FreezeV_RemoveFlag(vehicleid, FREEZE_PAUSE); }
}

stock FreezeV_GunmenuOpen(vehicleid) { FreezeV_AddFlag(vehicleid, FREEZE_GUNMENU); }

stock FreezeV_GunmenuClose(vehicleid) { FreezeV_RemoveFlag(vehicleid, FREEZE_GUNMENU); }

stock FreezeV_DestroyAll()
{
	foreach (new vehicleid : s_FrozenVehicles)
	{
		FreezeV_RemoveFlag(vehicleid, FREEZE_PAUSE | FREEZE_GUNMENU);
	}
	Iter_Clear(s_FrozenVehicles);
	foreach (new vehicleid : s_RoundVehicles) { DestroyVehicle(vehicleid); }
	Iter_Clear(s_RoundVehicles);
}
