#if defined _attdef_common_corpses
	#endinput
#endif
#define _attdef_common_corpses

/*
	Player Death Effects, Blood Splatters & Corpse Actors Sub-module
*/

public DestroyBloodObjectForPlayer(bloodObject, playerid)
{
	return DestroyPlayerObject(playerid, bloodObject);  // Destroy the blood object only for the specified player
}





forward SendTipMessageToAll(bool: calledDuringPause);

CreatePlayerBloodEffect(playerid, Float: x, Float: y, Float: z)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (DeathBloodObject[playerid] != INVALID_OBJECT_ID)
	{
		if (IsValidObject(DeathBloodObject[playerid]))
			DestroyObject(DeathBloodObject[playerid]);
		DeathBloodObject[playerid] = INVALID_OBJECT_ID;
	}

	new obj = CreateObject(DEATH_BLOOD_EFFECT_MODEL, x, y, z - 0.8, 0.0, 0.0, 0.0);
	if (obj != INVALID_OBJECT_ID)
	{
		SetObjectNoCameraCollision(obj);
		DeathBloodObject[playerid] = obj;
		if (!SetTimerEx("DestroyPlayerBloodEffect", DEATH_BLOOD_EFFECT_DURATION, false, "ii", playerid, obj))
		{
			DestroyObject(obj);
			DeathBloodObject[playerid] = INVALID_OBJECT_ID;
			return 0;
		}
	}
	return 1;
}

public DestroyPlayerBloodEffect(playerid, objectid)
{
	if (objectid != INVALID_OBJECT_ID && IsValidObject(objectid))
	{
		DestroyObject(objectid);
	}
	if (playerid >= 0 && playerid < MAX_PLAYERS && DeathBloodObject[playerid] == objectid)
	{
		DeathBloodObject[playerid] = INVALID_OBJECT_ID;
	}
	return 1;
}

public OnDeadBodyLanded(playerid, connectionGeneration, flareGen, actorid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (!DeadBodies)
		return 0;

	if (PlayerConnectionGeneration[playerid] != connectionGeneration
		|| DeadBodyFlareGeneration[playerid] != flareGen)
		return 0;

	new Float: fx = DeadBodyFlarePos[playerid][0];
	new Float: fy = DeadBodyFlarePos[playerid][1];
	new Float: fz = DeadBodyFlarePos[playerid][2];

	if (actorid != INVALID_ACTOR_ID && IsValidActor(actorid))
	{
		new Float: ax, Float: ay, Float: az;
		GetActorPos(actorid, ax, ay, az);
		fx = ax;
		fy = ay;
		fz = az;
	}

	if (TeamPingCollisionReady)
	{
		new Float: gx, Float: gy, Float: gz;
		if (CA_RayCastLine(fx, fy, fz + 1.0, fx, fy, fz - 3.5, gx, gy, gz))
		{
			fz = gz;
		}
	}

	CreateDeadBodySmokeFlare(playerid, fx, fy, fz);
	return 1;
}

CreateDeadBodySmokeFlare(playerid, Float: x, Float: y, Float: z)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (DeadBodyFlareObject[playerid] != INVALID_OBJECT_ID)
	{
		if (IsValidObject(DeadBodyFlareObject[playerid]))
			DestroyObject(DeadBodyFlareObject[playerid]);
		DeadBodyFlareObject[playerid] = INVALID_OBJECT_ID;
	}

	new obj = CreateObject(DEAD_BODY_FLARE_MODEL, x, y, z - 2.1, 0.0, 0.0, 0.0);
	if (obj != INVALID_OBJECT_ID)
	{
		SetObjectNoCameraCollision(obj);
		DeadBodyFlareObject[playerid] = obj;
		if (!SetTimerEx("DestroyDeadBodyFlare", DEAD_BODY_FLARE_DURATION, false, "ii", playerid, obj))
		{
			DestroyObject(obj);
			DeadBodyFlareObject[playerid] = INVALID_OBJECT_ID;
			return 0;
		}
	}
	return 1;
}

public DestroyDeadBodyFlare(playerid, objectid)
{
	if (objectid != INVALID_OBJECT_ID && IsValidObject(objectid))
	{
		DestroyObject(objectid);
	}
	if (playerid >= 0 && playerid < MAX_PLAYERS && DeadBodyFlareObject[playerid] == objectid)
	{
		DeadBodyFlareObject[playerid] = INVALID_OBJECT_ID;
	}
	return 1;
}

DestroyPlayerDeathEffects(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return;

	DeadBodyFlareGeneration[playerid]++;

	if (DeathBloodObject[playerid] != INVALID_OBJECT_ID)
	{
		if (IsValidObject(DeathBloodObject[playerid]))
			DestroyObject(DeathBloodObject[playerid]);
		DeathBloodObject[playerid] = INVALID_OBJECT_ID;
	}
	if (DeadBodyFlareObject[playerid] != INVALID_OBJECT_ID)
	{
		if (IsValidObject(DeadBodyFlareObject[playerid]))
			DestroyObject(DeadBodyFlareObject[playerid]);
		DeadBodyFlareObject[playerid] = INVALID_OBJECT_ID;
	}
}

DeleteAllDeathBloodEffects()
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		if (DeathBloodObject[i] != INVALID_OBJECT_ID)
		{
			if (IsValidObject(DeathBloodObject[i]))
				DestroyObject(DeathBloodObject[i]);
			DeathBloodObject[i] = INVALID_OBJECT_ID;
		}
	}
}

CreateDeadBody(playerid, killerid, reason, Float: distance = 0.0, Float: x = 0.0, Float: y = 0.0, Float: z = 0.0)
{
	if (!DeadBodies)
		return;

	if (x == 0.0 && y == 0.0 && z == 0.0)
		GetPlayerPos(playerid, x, y, z);
	if (distance == 0.0 && killerid != INVALID_PLAYER_ID)
		distance = GetPlayerDistanceFromPoint(killerid, x, y, z);

	new Float: angle;
	GetPlayerFacingAngle(playerid, angle);
	new actorid = CreateActor(GetPlayerSkin(playerid), x, y, z, angle);
	SetActorInvulnerable(actorid, false);
	SetActorHealth(actorid, 0.0);
	new worldid = GetPlayerVirtualWorld(playerid);
	SetActorVirtualWorld(actorid, worldid);
	Iter_Add(DeadBodyActors, actorid);
	new victimTeam = Player[playerid][Team];
	new victimColor[10];
	if (victimTeam >= 0 && victimTeam < MAX_TEAMS)
		format(victimColor, sizeof(victimColor), "%s", TextColor[victimTeam]);
	else
		format(victimColor, sizeof(victimColor), "{FFFFFF}");

	new str[256];
	if (killerid != INVALID_PLAYER_ID && killerid >= 0 && killerid < MAX_PLAYERS && IsPlayerConnected(killerid))
	{
		new killerTeam = Player[killerid][Team];
		new killerColor[10];
		if (killerTeam >= 0 && killerTeam < MAX_TEAMS)
			format(killerColor, sizeof(killerColor), "%s", TextColor[killerTeam]);
		else
			format(killerColor, sizeof(killerColor), "{FFFFFF}");

		format(str, sizeof(str),
			"{FFFFFF}Dead Body of %s%s\n{FFFFFF}Eliminated by %s%s\n{FFFFFF}Weapon: {FFFFFF}%s {CCCCCC}(%.1fm)",
			victimColor, Player[playerid][Name],
			killerColor, Player[killerid][Name],
			ReturnWeaponName(reason), distance);
	}
	else
	{
		format(str, sizeof(str),
			"{FFFFFF}Dead Body of %s%s\n{FFFFFF}Cause of Death: {FFFFFF}%s",
			victimColor, Player[playerid][Name],
			ReturnWeaponName(reason));
	}

	new Text3D: textlabel = Create3DTextLabel(str, -1, x, y, z - 0.3, 20.0, worldid, true);
	Iter_Add(DeadBodyLabels, _: textlabel);

	DeadBodyFlarePos[playerid][0] = x;
	DeadBodyFlarePos[playerid][1] = y;
	DeadBodyFlarePos[playerid][2] = z;
	new flareGen = ++DeadBodyFlareGeneration[playerid];
	if (!SetTimerEx(
		"OnDeadBodyLanded", DEAD_BODY_LAND_DELAY, false, "iiii",
		playerid, PlayerConnectionGeneration[playerid], flareGen, actorid))
	{
		CreateDeadBodySmokeFlare(playerid, x, y, z);
	}
}

DeleteAllDeadBodies()
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		DeadBodyFlareGeneration[i]++;
		if (DeadBodyFlareObject[i] != INVALID_OBJECT_ID)
		{
			if (IsValidObject(DeadBodyFlareObject[i]))
				DestroyObject(DeadBodyFlareObject[i]);
			DeadBodyFlareObject[i] = INVALID_OBJECT_ID;
		}
	}
	DeleteAllDeathBloodEffects();

	if (!DeadBodies)
		return;

	if (Iter_Count(DeadBodyLabels) != 0)
	{
		foreach (new i : DeadBodyLabels)
			Delete3DTextLabel(Text3D: i);

		Iter_Clear(DeadBodyLabels);
	}
	if (Iter_Count(DeadBodyActors) != 0)
	{
		foreach (new i : DeadBodyActors)
			DestroyActor(i);

		Iter_Clear(DeadBodyActors);
	}
}

