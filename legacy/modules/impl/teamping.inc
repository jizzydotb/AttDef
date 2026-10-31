#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_teamping
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_teamping
	#endinput
#endif
#define _attdef_impl_teamping

stock DestroyTeamPingForViewer(pingerid, viewerid)
{
	if (TeamPingObjectActive[pingerid][viewerid])
	{
		DestroyPlayerObject(viewerid, TeamPingObjects[pingerid][viewerid]);
		TeamPingObjectActive[pingerid][viewerid] = false;
	}
	if (TeamPingMapIconActive[pingerid][viewerid])
	{
		RemovePlayerMapIcon(viewerid, TEAM_PING_MAP_ICON_BASE + pingerid);
		TeamPingMapIconActive[pingerid][viewerid] = false;
	}
	return 1;
}

stock DestroyTeamPing(pingerid)
{
	TeamPingGeneration[pingerid]++;
	for (new viewerid = 0; viewerid < MAX_PLAYERS; viewerid++)
		DestroyTeamPingForViewer(pingerid, viewerid);
	return 1;
}

DestroyTeamPingsForPlayer(playerid)
{
	DestroyTeamPing(playerid);
	LastTeamPingTick[playerid] = 0;
	TeamPingDanger[playerid] = false;
	for (new pingerid = 0; pingerid < MAX_PLAYERS; pingerid++)
	{
		if (pingerid != playerid)
			DestroyTeamPingForViewer(pingerid, playerid);
	}
	return 1;
}

public DestroyAllTeamPings()
{
	for (new i = 0; i < MAX_PLAYERS; i++)
		DestroyTeamPingsForPlayer(i);
	return 1;
}

stock bool: IsPlayerInTeamPingRound(playerid)
{
	if (AllVsAllMode || Current < 0 || !Player[playerid][Playing]
		|| Player[playerid][Spectating] || Player[playerid][InDM]
		|| Player[playerid][InDuel] || !Player[playerid][Spawned])
		return false;

	switch (GameType)
	{
		case BASE:
		{
			if (!BaseStarted)
				return false;
		}
		case ARENA:
		{
			if (!ArenaStarted || AllVsAllMode)
				return false;
		}
		default: return false;
	}

	switch (Player[playerid][Team])
	{
		case ATTACKER, DEFENDER, ATTACKER_SUB, DEFENDER_SUB: return true;
	}
	return false;
}

stock bool: IsPlayerTeamPingTeammate(pingerid, viewerid, virtualWorld, interior)
{
	return IsPlayerConnected(viewerid)
		&& !Player[viewerid][InDM]
		&& !Player[viewerid][InDuel]
		&& (Player[viewerid][Playing] || Player[viewerid][Spectating])
		&& IsTeamTheSame(Player[pingerid][Team], Player[viewerid][Team])
		&& GetPlayerVirtualWorld(viewerid) == virtualWorld
		&& GetPlayerInterior(viewerid) == interior;
}

hook OnGameModeInit()
{
	TeamPingCollisionReady = CA_Init() != 0;
	if (!TeamPingCollisionReady)
		printf("[AttDef] Team pings disabled: ColAndreas could not load scriptfiles/colandreas/ColAndreas.cadb.");
	return 1;
}

stock bool: GetTeamPingPosition(playerid, WEAPON: weaponid, &Float: x, &Float: y, &Float: z)
{
	new Float: cameraX, Float: cameraY, Float: cameraZ;
	new Float: directionX, Float: directionY, Float: directionZ;
	if (!GetPlayerCameraPos(playerid, cameraX, cameraY, cameraZ)
		|| !GetPlayerCameraFrontVector(playerid, directionX, directionY, directionZ))
		return false;

	new Float: directionLength = floatsqroot(
		directionX * directionX + directionY * directionY + directionZ * directionZ);
	if (directionLength < 0.001)
		return false;

	directionX /= directionLength;
	directionY /= directionLength;
	directionZ /= directionLength;

	if (GetPlayerCameraMode(playerid) == CAM_MODE_AIMWEAPON)
	{
		new Float: aspectRatio = GetPlayerCameraAspectRatio(playerid);
		new Float: rightX = directionY;
		new Float: rightY = -directionX;
		new Float: rightLength = floatsqroot(rightX * rightX + rightY * rightY);
		if (aspectRatio <= 0.0 || rightLength < 0.001)
			return false;

		rightX /= rightLength;
		rightY /= rightLength;
		new Float: upX = rightY * directionZ;
		new Float: upY = -rightX * directionZ;
		new Float: upZ = rightX * directionY - rightY * directionX;
		new Float: cameraFov = GetPlayerCameraZoom(playerid);
		if (cameraFov < 35.0 || cameraFov > 70.0)
			return false;
		new Float: tanHalfFov = floattan(cameraFov * 0.5, degrees);

		// GTA SA derives the aim ray from its crosshair offset and current camera FOV.
		new Float: verticalOffset = tanHalfFov * ((0.5 - TEAM_PING_CROSSHAIR_Y) * 2.0) / aspectRatio;
		new Float: horizontalOffset = tanHalfFov * ((TEAM_PING_CROSSHAIR_X - 0.5) * 2.0);
		directionX += upX * verticalOffset + rightX * horizontalOffset;
		directionY += upY * verticalOffset + rightY * horizontalOffset;
		directionZ += upZ * verticalOffset;

		directionLength = floatsqroot(
			directionX * directionX + directionY * directionY + directionZ * directionZ);
		if (directionLength < 0.001)
			return false;

		directionX /= directionLength;
		directionY /= directionLength;
		directionZ /= directionLength;
	}

	new Float: weaponRange = GetWeaponMaxRange(weaponid);
	if (weaponRange <= 0.0)
		return false;

	new Float: endX = cameraX + directionX * weaponRange;
	new Float: endY = cameraY + directionY * weaponRange;
	new Float: endZ = cameraZ + directionZ * weaponRange;
	new Float: hitX, Float: hitY, Float: hitZ;
	new hitModel = CA_RayCastLine(
		cameraX, cameraY, cameraZ, endX, endY, endZ,
		hitX, hitY, hitZ);

	if (hitModel != 0)
	{
		x = hitX;
		y = hitY;
		z = hitZ;
	}
	else
	{
		x = endX;
		y = endY;
		z = endZ;
	}
	return true;
}

stock bool: CreateTeamPing(playerid)
{
	if (AllVsAllMode || !IsPlayerInTeamPingRound(playerid)
		|| GetPlayerState(playerid) != PLAYER_STATE_ONFOOT
		|| Player[playerid][OnGunmenu] || AntiBugFrozen[playerid])
		return false;

	new currentTick = GetTickCount();
	new bool: isDangerPing = false;
	if (LastTeamPingTick[playerid] != 0 && (currentTick - LastTeamPingTick[playerid]) < TEAM_PING_COOLDOWN_MS)
	{
		if (!TeamPingDanger[playerid] && (currentTick - LastTeamPingTick[playerid]) <= TEAM_PING_DOUBLE_TAP_MS)
		{
			isDangerPing = true;
		}
		else
		{
			new remaining = (TEAM_PING_COOLDOWN_MS - (currentTick - LastTeamPingTick[playerid])) / 1000 + 1;
			va_SendClientMessage(playerid, COL_ERROR_HEX, "\xBB {FFFFFF}You must wait %d second%s before sending another team ping.", remaining, remaining == 1 ? "" : "s");
			return false;
		}
	}

	if (!TeamPingCollisionReady)
	{
		SendErrorMessage(playerid, "Team pings are unavailable because the collision map could not be loaded.");
		return false;
	}

	new weaponid = GetPlayerWeapon(playerid);
	if (!IsBulletWeapon(WEAPON: weaponid))
		return false;

	new Float: pingX, Float: pingY, Float: pingZ;
	if (!GetTeamPingPosition(playerid, WEAPON: weaponid, pingX, pingY, pingZ))
	{
		SendErrorMessage(playerid, "Could not determine the ping location. Please try again.");
		return false;
	}

	new virtualWorld = GetPlayerVirtualWorld(playerid);
	new interior = GetPlayerInterior(playerid);
	new objectModel = isDangerPing
		? TEAM_PING_ATTACKER_OBJECT_MODEL
		: (Player[playerid][Team] == ATTACKER || Player[playerid][Team] == ATTACKER_SUB
			? TEAM_PING_ATTACKER_OBJECT_MODEL : TEAM_PING_DEFENDER_OBJECT_MODEL);
	new pingColor = isDangerPing
		? 0xFF2B2BFF
		: (Player[playerid][Team] == ATTACKER || Player[playerid][Team] == ATTACKER_SUB
			? ATTACKER_PLAYING : DEFENDER_PLAYING);
	new mapIconType = isDangerPing ? TEAM_PING_DANGER_MAP_ICON_TYPE : TEAM_PING_MAP_ICON_TYPE;

	DestroyTeamPing(playerid);
	TeamPingOwnerConnectionGeneration[playerid] = PlayerConnectionGeneration[playerid];
	new pingGeneration = TeamPingGeneration[playerid];
	new recipients = 0;

	for (new viewerid = 0; viewerid < MAX_PLAYERS; viewerid++)
	{
		if (!IsPlayerTeamPingTeammate(playerid, viewerid, virtualWorld, interior))
			continue;

		TeamPingObjects[playerid][viewerid] = CreatePlayerObject(
			viewerid, objectModel, pingX, pingY, pingZ, 0.0, 0.0, 0.0, 250.0);
		if (TeamPingObjects[playerid][viewerid] == INVALID_OBJECT_ID)
		{
			printf("[AttDef] Failed to create team ping object for player %d (viewer %d).", playerid, viewerid);
			DestroyTeamPing(playerid);
			SendErrorMessage(playerid, "Could not create the team ping. Please try again.");
			return false;
		}
		TeamPingObjectActive[playerid][viewerid] = true;

		if (!SetPlayerMapIcon(
			viewerid, TEAM_PING_MAP_ICON_BASE + playerid, pingX, pingY, pingZ,
			mapIconType, pingColor, MAPICON_GLOBAL))
		{
			printf("[AttDef] Failed to create team ping map icon for player %d (viewer %d).", playerid, viewerid);
			DestroyTeamPing(playerid);
			SendErrorMessage(playerid, "Could not create the team ping. Please try again.");
			return false;
		}
		TeamPingMapIconActive[playerid][viewerid] = true;
		recipients++;
	}

	if (!recipients)
	{
		DestroyTeamPing(playerid);
		SendErrorMessage(playerid, "No teammates are currently available to receive your ping.");
		return false;
	}

	if (!SetTimerEx(
		"ExpireTeamPing", TEAM_PING_DURATION_MS, false, "iii",
		playerid, TeamPingOwnerConnectionGeneration[playerid], pingGeneration))
	{
		printf("[AttDef] Failed to schedule team ping expiration for player %d.", playerid);
		DestroyTeamPing(playerid);
		SendErrorMessage(playerid, "Could not schedule the team ping. Please try again.");
		return false;
	}

	LastTeamPingTick[playerid] = currentTick;
	TeamPingDanger[playerid] = isDangerPing;

	// Notify teammates about the ping
	new team = Player[playerid][Team];
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		if (!IsPlayerConnected(i))
			continue;
		if (!(Player[i][Playing] || Player[i][Spectating]))
			continue;
		if (!IsTeamTheSame(Player[i][Team], team))
			continue;

		if (isDangerPing)
		{
			PlayerPlaySound(i, 6400, 0.0, 0.0, 0.0);
			va_SendClientMessage(i, -1, "%s\xBB {FF3B3B}[DANGER] %s%s {FFFFFF}spotted an enemy on the map!", TextColor[team], TextColor[team], Player[playerid][Name]);
		}
		else
		{
			PlayerPlaySound(i, 6400, 0.0, 0.0, 0.0);
			va_SendClientMessage(i, -1, "%s\xBB %s pinged a location on the map!", TextColor[team], Player[playerid][Name]);
		}
	}

	return true;
}

forward ExpireTeamPing(pingerid, connectionGeneration, pingGeneration);
public ExpireTeamPing(pingerid, connectionGeneration, pingGeneration)
{
	if (pingerid < 0 || pingerid >= MAX_PLAYERS
		|| !IsPlayerConnected(pingerid)
		|| PlayerConnectionGeneration[pingerid] != connectionGeneration
		|| TeamPingOwnerConnectionGeneration[pingerid] != connectionGeneration
		|| TeamPingGeneration[pingerid] != pingGeneration)
		return 0;

	DestroyTeamPing(pingerid);
	return 1;
}
