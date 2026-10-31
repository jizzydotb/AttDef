#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_match
	#error "The match module header must be included before its blackout implementation."
#endif

#if defined _attdef_impl_blackout
	#endinput
#endif
#define _attdef_impl_blackout

#include <YSI_Coding\y_hooks>

new bool: BlackoutZonesCreated;

hook OnGameModeInit()
{
	for (new i = 0; i < sizeof(BlackoutZone); i++)
		BlackoutZone[i] = INVALID_GANG_ZONE;

	BlackoutZonesCreated = false;
	return 1;
}

public DestroyPlayerBlackoutZone(playerid)
{
	for (new i = 0; i < sizeof(BlackoutZone); i++)
	{
		if (BlackoutZone[i] != INVALID_GANG_ZONE)
			GangZoneHideForPlayer(playerid, BlackoutZone[i]);
	}
	return 1;
}

public CreatePlayerBlackoutZone(playerid)
{
	if (Current == -1 || GameType != ARENA || Player[playerid][Team] == REFEREE)
		return 0;

	if (!BlackoutZonesCreated && !CreateBlackoutZone())
		return 0;

	for (new i = 0; i < sizeof(BlackoutZone); i++)
	{
		if (BlackoutZone[i] != INVALID_GANG_ZONE)
			GangZoneShowForPlayer(playerid, BlackoutZone[i], 0x000000AA);
	}
	return 1;
}

public DestroyBlackoutZones()
{
	BlackoutZonesCreated = false;
	for (new i = 0; i < sizeof(BlackoutZone); i++)
	{
		if (BlackoutZone[i] == INVALID_GANG_ZONE)
			continue;

		GangZoneDestroy(BlackoutZone[i]);
		BlackoutZone[i] = INVALID_GANG_ZONE;
	}
	return 1;
}

stock CreateBlackoutRectangle(zoneIndex, Float: minX, Float: minY, Float: maxX, Float: maxY)
{
	if (minX >= maxX || minY >= maxY)
	{
		BlackoutZone[zoneIndex] = INVALID_GANG_ZONE;
		return 1;
	}

	BlackoutZone[zoneIndex] = GangZoneCreate(minX, minY, maxX, maxY);
	return BlackoutZone[zoneIndex] != INVALID_GANG_ZONE;
}

public CreateBlackoutZone()
{
	if (Current < 0 || Current >= MAX_ARENAS || GameType != ARENA)
		return 0;

	DestroyBlackoutZones();
	new Float: minX = AMin[Current][0];
	new Float: minY = AMin[Current][1];
	new Float: maxX = AMax[Current][0];
	new Float: maxY = AMax[Current][1];
	// Interior arena rows use sentinel bounds and have no outdoor area to blackout.
	if (minX == -9999.0 && minY == -9999.0 && maxX == 9999.0 && maxY == 9999.0)
	{
		BlackoutZonesCreated = true;
		return 1;
	}
	if (minX < MAP_MIN_X || minY < MAP_MIN_Y || maxX > MAP_MAX_X || maxY > MAP_MAX_Y
		|| minX >= maxX || minY >= maxY)
	{
		printf("[AttDef] Cannot create blackout for arena %d: invalid map bounds.", Current);
		return 0;
	}

	if (!CreateBlackoutRectangle(0, MAP_MIN_X, MAP_MIN_Y, MAP_MAX_X, minY)
		|| !CreateBlackoutRectangle(1, MAP_MIN_X, maxY, MAP_MAX_X, MAP_MAX_Y)
		|| !CreateBlackoutRectangle(2, MAP_MIN_X, minY, minX, maxY)
		|| !CreateBlackoutRectangle(3, maxX, minY, MAP_MAX_X, maxY))
	{
		DestroyBlackoutZones();
		printf("[AttDef] Could not allocate all blackout zones for arena %d.", Current);
		return 0;
	}

	for (new i = 0; i < sizeof(BlackoutZone); i++)
	{
		if (BlackoutZone[i] != INVALID_GANG_ZONE)
		{
			foreach (new p : Player)
			{
				if (Player[p][Team] != REFEREE && (Player[p][Playing] || Player[p][ToAddInRound]))
					GangZoneShowForPlayer(p, BlackoutZone[i], 0x000000AA);
				else
					GangZoneHideForPlayer(p, BlackoutZone[i]);
			}
		}
	}
	BlackoutZonesCreated = true;
	return 1;
}
