#if defined _attdef_match_checkpoint
	#endinput
#endif
#define _attdef_match_checkpoint

/*
	Match/Round Checkpoint, Flares & Gangzones Sub-module
*/

#define ARENA_FLARE_COUNT 3
new ArenaFlare[MAX_PLAYERS][3];

public DestroyFlareObjects(playerid)
{
	// Destroy Flare objects if they exist
	for (new i = 0; i < FLARE_COUNT; i++)
	{
		if (IsValidPlayerObject(playerid, Flare[playerid][i]))
		{
			DestroyPlayerObject(playerid, Flare[playerid][i]);
			Flare[playerid][i] = INVALID_OBJECT_ID;
		}
	}
}

public DestroyArenaFlareObjects(playerid)
{
	// Destroy Arena Flare objects if they exist
	for (new i = 0; i < ARENA_FLARE_COUNT; i++)
	{
		if (IsValidPlayerObject(playerid, ArenaFlare[playerid][i]))
		{
			DestroyPlayerObject(playerid, ArenaFlare[playerid][i]);
			ArenaFlare[playerid][i] = INVALID_OBJECT_ID;
		}
	}
}

// Player objects are local to each client. Keep checkpoint effects in one
// place so every route that activates a CP renders the same flares.
public RefreshCheckpointFlares(playerid)
{
	DestroyFlareObjects(playerid);
	DestroyArenaFlareObjects(playerid);

	if (Current == -1)
		return 0;

	new Float:x, Float:y, Float:z;
	switch (GameType)
	{
		case BASE:
		{
			x = BCPSpawn[Current][0];
			y = BCPSpawn[Current][1];
			z = BCPSpawn[Current][2] - 2.1;
			Flare[playerid][0] = CreatePlayerObject(playerid, 18728, x, y, z, 0.0, 0.0, 270.0, 5000.0);
			Flare[playerid][1] = CreatePlayerObject(playerid, 18717, x, y, z, 0.0, 0.0, 270.0, 5000.0);
		}
		case ARENA:
		{
			if (!CPInArena || AllVsAllMode)
				return 0;

			x = ACPSpawn[Current][0];
			y = ACPSpawn[Current][1];
			z = ACPSpawn[Current][2] - 2.1;
			ArenaFlare[playerid][0] = CreatePlayerObject(playerid, 18728, x, y, z, 0.0, 0.0, 270.0, 5000.0);
			ArenaFlare[playerid][1] = CreatePlayerObject(playerid, 18717, x, y, z, 0.0, 0.0, 270.0, 5000.0);
		}
	}
	return 1;
}

public UpdateAttackersZoneVisibility(playerid)
{
	#pragma unused playerid
	if (AttackersZone == INVALID_GANG_ZONE)
		return 0;

	foreach (new i : Player)
	{
  		if (Player[i][Playing] && Player[i][Team] == ATTACKER)
		{
			GangZoneShowForPlayer(i, AttackersZone, GANGZONE_AttSpawn);
		}
		else
		{
			GangZoneHideForPlayer(i, AttackersZone);
		}	
	}
	return 1;
}

public UpdateCPZoneVisibility(playerid)
{
	#pragma unused playerid
	if (CPZone == INVALID_GANG_ZONE)
		return 0;

	foreach (new i : Player)
	{
		new team = Player[i][Playing] ? Player[i][WasInTeam] : Player[i][Team];
		if (team == NON)
			team = Player[i][Team];
		new bool: defender = team == DEFENDER || team == DEFENDER_SUB;
		new bool: canSee = !Player[i][InDM] && GetPlayerVirtualWorld(i) == 2
			&& ((Player[i][Playing] && defender)
				|| (Player[i][Spectating] && (defender || Player[i][Team] == REFEREE)));

		if (canSee)
			GangZoneShowForPlayer(i, CPZone, GANGZONE_CP);
		else
			GangZoneHideForPlayer(i, CPZone);
	}
	return 1;
}

#include <YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid)
{
	for (new i; i < FLARE_COUNT; i++)
		Flare[playerid][i] = INVALID_OBJECT_ID;
	for (new i; i < ARENA_FLARE_COUNT; i++)
		ArenaFlare[playerid][i] = INVALID_OBJECT_ID;
	return 1;
}
