#if defined _FPS_utils_included
	#endinput
#endif
#define _FPS_utils_included

#include <YSI_Coding\y_hooks>

static const FPS_DRUNK_LEVEL = 2000;
static s_FPS[MAX_PLAYERS] = { 0, ... };
static s_LastDrunkLevel[MAX_PLAYERS] = { 0, ... };
static s_LastFPSSampleTick[MAX_PLAYERS] = { 0, ... };
static bool:s_HasFPSSample[MAX_PLAYERS] = { false, ... };

stock ResetPlayerFPS(playerid)
{
	s_FPS[playerid] = 0;
	s_LastDrunkLevel[playerid] = 0;
	s_LastFPSSampleTick[playerid] = 0;
	s_HasFPSSample[playerid] = false;
	return 1;
}

stock StartPlayerFPSTracking(playerid)
{
	ResetPlayerFPS(playerid);
	SetPlayerDrunkLevel(playerid, FPS_DRUNK_LEVEL);
	s_LastDrunkLevel[playerid] = FPS_DRUNK_LEVEL;
	s_LastFPSSampleTick[playerid] = GetTickCount();
	s_HasFPSSample[playerid] = true;
	return 1;
}

hook OnPlayerConnect(playerid)
{
	return StartPlayerFPSTracking(playerid);
}

hook OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	return ResetPlayerFPS(playerid);
}

stock UpdatePlayerFPS(playerid)
{
	new currentDrunkLevel = GetPlayerDrunkLevel(playerid);
	new currentTick = GetTickCount();

	if (!s_HasFPSSample[playerid])
	{
		s_LastDrunkLevel[playerid] = currentDrunkLevel;
		s_LastFPSSampleTick[playerid] = currentTick;
		s_HasFPSSample[playerid] = true;
		return 1;
	}

	if (currentDrunkLevel < 100)
	{
		// Keep the level measurable while remaining below the drunk-effect threshold.
		SetPlayerDrunkLevel(playerid, FPS_DRUNK_LEVEL);
		s_LastDrunkLevel[playerid] = FPS_DRUNK_LEVEL;
		s_LastFPSSampleTick[playerid] = currentTick;
		return 1;
	}

	new elapsed = currentTick - s_LastFPSSampleTick[playerid];
	if (currentDrunkLevel > s_LastDrunkLevel[playerid])
	{
		s_LastDrunkLevel[playerid] = currentDrunkLevel;
		s_LastFPSSampleTick[playerid] = currentTick;
		return 1;
	}

	if (currentDrunkLevel < s_LastDrunkLevel[playerid] && elapsed > 0)
	{
		new Float: measuredFPS = float(s_LastDrunkLevel[playerid] - currentDrunkLevel) * 1000.0 / float(elapsed);
		s_FPS[playerid] = floatround(measuredFPS);
		if (s_FPS[playerid] > 1000)
			s_FPS[playerid] = 1000;

		s_LastDrunkLevel[playerid] = currentDrunkLevel;
		s_LastFPSSampleTick[playerid] = currentTick;
	}
	return 1;
}

stock GetPlayerFPS(playerid)
{
	return s_FPS[playerid];
}
