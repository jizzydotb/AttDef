#if defined _attdef_match_arena
	#endinput
#endif
#define _attdef_match_arena

/*
	Match/Round Arena System & All-vs-All Sub-module
*/

forward OnRCStart();
public OnRCStart()
{
	PreStartRoundTimer = 0;
	AutoAFKCheck(); // Checks all players and set idle players to afk mode
	RCArena		= true;
	new ArenaID = 72;		   // That's the ID of the RC arena (interior)
	ClearKillList();		   // Clears the kill-list.
	DestroyUnusedVehicles(); // Destroys vehicles
	DeleteAllDeadBodies();
	GameType = ARENA;
	Current	 = ArenaID; // Current will be the ID of the base that we just started. We do this so that we can use this ID later on (e.g. check /car command for the use).
	ClearKillList();	// Clears the kill-list.
	ServerLastPlayed	 = Current;
	ServerLastPlayedType = 0;
	TotalPausesInRound	 = 0;
	TotalPauseTimeElapsed = 0;

	ElapsedTime			 = 0;

	TeamCapturingCP		 = NON;
	PlayersInCP			 = 0;
	ElapsedTime			 = 0;
	CurrentCPTime		 = ConfigCPTime + 1;
	ResetRoundTextDraws();
	foreach (new i : Player)
	{
		if (!Player[i][Spawned])
			continue;

		if (Player[i][RoundTDStyle] != 1)
			continue;

		if (Player[i][RoundTDStyle] != 1)
			continue;

		TextDrawShowForPlayer(i, timerCircleTD);
	}

	ResetGunmenuSelections();

	new iString[32];
	format(iString, sizeof(iString), "%sRC BF %s(~r~%d%s)", MAIN_TEXT_COLOUR, MAIN_TEXT_COLOUR, Current, MAIN_TEXT_COLOUR);

	CPZone = GangZoneCreate(ACPSpawn[Current][0] - 75, ACPSpawn[Current][1] - 75, ACPSpawn[Current][0] + 75, ACPSpawn[Current][1] + 75);
	GangZoneShowForAll(CPZone, GANGZONE_CP);

	ViewRoundCountdown = 4;
	new viewGeneration = StartRoundViewSequence();

	new cdString[64];
	format(cdString, sizeof(cdString), "~w~RC starting in ~r~~h~%d ~w~seconds", ViewRoundCountdown);
	TextDrawSetString(centerTeamNames, cdString);
	TextDrawSetString(AttDef_RoundTimer, cdString);
	TextDrawSetString(ModernRound_Timer, cdString);
	TextDrawSetString(SimpleRound_Timer, cdString);
	UpdateRoundTeamStatusTimer(" ");
	UpdateRoundTeamStatusMessages();

	foreach (new i : Player)
	{
		Player[i][WasInCP]	   = false;

		if (Player[i][ToAddInRound])
		{
			if (Player[i][Team] != ATTACKER && Player[i][Team] != DEFENDER && Player[i][Team] != REFEREE)
			{
				Player[i][ToAddInRound] = false;
				HideRoundStats(i);
				continue;
			}
			HideEndRoundTextDraw(i);

			if (Player[i][Spectating])
				StopSpectate(i);

			ClearPlayerDMState(i);
			SetPlayerRoundParticipation(i, true);

			SetPlayerVirtualWorld(
				i,
				2); //Set player virtual world to something different that that for lobby and DM so that they don't collide with each other. e.g. You shouldn't be able to see players in lobby or DM while you are in base.
			TogglePlayerControllable(i, 0); //Pause players.

			SetPlayerInterior(i, AInterior[Current]);
			SetPlayerCameraLookAt(i, ACPSpawn[Current][0], ACPSpawn[Current][1], ACPSpawn[Current][2]);
			SetPlayerCameraPos(i, ACPSpawn[Current][0] + 100, ACPSpawn[Current][1], ACPSpawn[Current][2] + 80);
			SetPlayerPos(i, ACPSpawn[Current][0] + 10.0, ACPSpawn[Current][1] + 10.0, ACPSpawn[Current][2]);
		}

		if (Player[i][Team] != REFEREE && (Player[i][ToAddInRound] || Player[i][Playing]))
			ShowRoundStats(i);
		else
			HideRoundStats(i);

		UpdateBaseIDTextDraw(i);
	}

	if (AMin[Current][0] == -9999.0 && AMin[Current][1] == -9999.0
		&& AMax[Current][0] == 9999.0 && AMax[Current][1] == 9999.0)
		ArenaZone = INVALID_GANG_ZONE;
	else
		ArenaZone = GangZoneCreate(AMin[Current][0], AMin[Current][1], AMax[Current][0], AMax[Current][1]);
	CreateBlackoutZone();
	format(iString, sizeof(iString), "game.map RC: %d", Current);
	SendRconCommand(iString);

	if (ArenaZone != INVALID_GANG_ZONE)
		GangZoneShowForAll(ArenaZone, GANGZONE_ARENA);

	ViewRCForPlayers(viewGeneration);

	if (WarMode)
	{
		format(iString, sizeof(iString), "%s: %d - %s: %d", TeamName[ATTACKER], TeamScore[ATTACKER], TeamName[DEFENDER], TeamScore[DEFENDER]);
		SetGameModeText(iString);
		MatchRoundsStarted++;
		if (MatchRoundsStarted > 100)
			MatchRoundsStarted = 1;
		MatchRoundsRecord[MatchRoundsStarted - 1][round__type]		= 1;
		MatchRoundsRecord[MatchRoundsStarted - 1][round__ID]		= ArenaID;
		MatchRoundsRecord[MatchRoundsStarted - 1][round__completed] = false;
	}
}

forward ViewRCForPlayers(generation);
public ViewRCForPlayers(generation)
{
	if (generation != RoundViewGeneration || !RCArena || Current != 72 || GameType != ARENA)
		return 0;

	if (ViewRoundCountdown == 0)
	{
		if (ViewRoundTimer)
		{
			KillTimer(ViewRoundTimer);
			ViewRoundTimer = 0;
		}
		ClearRoundStartCommandRestrictions();
		SpawnPlayersInRC();
		ResetTeamLeaders();
		TextDrawSetString(centerTeamNames, " ");
		TextDrawSetString(AttDef_RoundTimer, " ");
		TextDrawSetString(ModernRound_Timer, " ");
		TextDrawSetString(SimpleRound_Timer, " ");
		if (Current != -1)
		{
			UpdateRoundTeamStatusTimer(" ");
			UpdateRoundTeamStatusMessages();
		}
		return 1;
	}

	if (!ViewRoundTimer)
	{
		ViewRoundTimer = SetTimerEx("ViewRCForPlayers", 1000, true, "i", generation);
		if (!ViewRoundTimer)
		{
			printf("[AttDef] Failed to schedule RC round countdown; starting players immediately.");
			ViewRoundCountdown = 0;
			return ViewRCForPlayers(generation);
		}
	}
	new iString[64];
	format(iString, sizeof(iString), "~w~RC starting in ~r~~h~%d ~w~seconds", ViewRoundCountdown);
	TextDrawSetString(centerTeamNames, iString);
	TextDrawSetString(AttDef_RoundTimer, iString);
	TextDrawSetString(ModernRound_Timer, iString);
	TextDrawSetString(SimpleRound_Timer, iString);
	UpdateRoundTeamStatusTimer(" ");
	UpdateRoundTeamStatusMessages();

	foreach (new i : Player)
	{
		if (Player[i][ToAddInRound])
		{
			AllowedViewingCommands[i] = true;
			PlayerPlaySound(i, 1139, 0.0, 0.0, 0.0);
			switch (ViewRoundCountdown)
			{
				case 4:
				{
					SetPlayerCameraLookAt(i, ACPSpawn[Current][0], ACPSpawn[Current][1], ACPSpawn[Current][2]);
					SetPlayerCameraPos(i, ACPSpawn[Current][0], ACPSpawn[Current][1] + 100, ACPSpawn[Current][2] + 80);
				}
				case 3:
				{
					InterpolateCameraPos(
						i, ACPSpawn[Current][0], ACPSpawn[Current][1] + 100, ACPSpawn[Current][2] + 80, ACPSpawn[Current][0] - 100, ACPSpawn[Current][1], ACPSpawn[Current][2] + 80,
						1000, CAMERA_MOVE);
					InterpolateCameraLookAt(
						i, ACPSpawn[Current][0], ACPSpawn[Current][1], ACPSpawn[Current][2], ACPSpawn[Current][0], ACPSpawn[Current][1], ACPSpawn[Current][2], 1000, CAMERA_MOVE);
				}
				case 2:
				{
					InterpolateCameraPos(
						i, ACPSpawn[Current][0] - 100, ACPSpawn[Current][1], ACPSpawn[Current][2] + 80, ACPSpawn[Current][0], ACPSpawn[Current][1] - 100, ACPSpawn[Current][2] + 80,
						1000, CAMERA_MOVE);
					InterpolateCameraLookAt(
						i, ACPSpawn[Current][0], ACPSpawn[Current][1], ACPSpawn[Current][2], ACPSpawn[Current][0], ACPSpawn[Current][1], ACPSpawn[Current][2], 1000, CAMERA_MOVE);
				}
				case 1:
				{
					InterpolateCameraPos(
						i, ACPSpawn[Current][0], ACPSpawn[Current][1] - 100, ACPSpawn[Current][2] + 80, ACPSpawn[Current][0] + 100, ACPSpawn[Current][1], ACPSpawn[Current][2] + 80,
						1000, CAMERA_MOVE);
					InterpolateCameraLookAt(
						i, ACPSpawn[Current][0], ACPSpawn[Current][1], ACPSpawn[Current][2], ACPSpawn[Current][0], ACPSpawn[Current][1], ACPSpawn[Current][2], 1000, CAMERA_MOVE);
				}
			}
		}
	}

	foreach (new i : Player)
	{
		if (Player[i][Team])
		{
			BlackListViewingCommands[i] = true;
			NormalListViewingCommands[i] = true;
		}
	}

	ViewRoundCountdown--;
	return 1;
}

SpawnPlayersInRC()
{
	DisableRoundSync();
	FallProtection = true;
	ClearChat();
	SendClientMessageToAll(-1, va_return(COL_PRIM "RC Battlefield started: {FFFFFF}%s (ID: %d)", AName[Current], Current));
	foreach (new i : Player)
	{
		if (Player[i][ToAddInRound])
		{
			if (Player[i][Spectating])
				StopSpectate(i);

			if (!Player[i][FakePacketRenovation] && Player[i][NetCheck] && NetStats_GetConnectedTime(i) < 300000)
			{
				ScheduleFakePacketRenovation(i, 30000, false, NetStats_PacketLossPercent(i));
			}

			SetPlayerRoundParticipation(i, true);
			Player[i][WasInBase] = true;
			Player[i][WasInTeam] = Player[i][Team];

			ResetPlayerRoundVariables(i);

			PlayerPlaySound(i, 1057, 0, 0, 0);
			SetPlayerVirtualWorld(i, 2);
			SetPlayerInterior(i, AInterior[Current]);

			SetHPAP(i, RoundHP, RoundAR);

			CreatePlayerTeamBar(i);

			ShowTeamCornerInfo(i);

			switch (Player[i][Team])
			{
				case ATTACKER:
				{
					if (AInterior[Current] == 0)
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], AAttackerSpawn[Current][0] + RandomMinMax(-3, 3), AAttackerSpawn[Current][1] + RandomMinMax(-3, 3),
							AAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
					else
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], AAttackerSpawn[Current][0] + RandomMinMax(-1, 1), AAttackerSpawn[Current][1] + RandomMinMax(-1, 1),
							AAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);

					Player[i][IgnoreSpawn] = true;
					SetPlayerColor(i, ATTACKER_PLAYING);
					SpawnPlayer(i);
					SetPlayerTeam(i, NO_TEAM);
				}
				case DEFENDER:
				{
					if (AInterior[Current] == 0)
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], ADefenderSpawn[Current][0] + RandomMinMax(-3, 3), ADefenderSpawn[Current][1] + RandomMinMax(-3, 3),
							ADefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
					else
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], ADefenderSpawn[Current][0] + RandomMinMax(-1, 1), ADefenderSpawn[Current][1] + RandomMinMax(-1, 1),
							ADefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);

					Player[i][IgnoreSpawn] = true;
					SetPlayerColor(i, DEFENDER_PLAYING);
					SpawnPlayer(i);
					SetPlayerTeam(i, NO_TEAM);
				}
			}
			SetCameraBehindPlayer(i);
			if (!RoundPaused)
				TogglePlayerControllable(i, 1);
			else
				TogglePlayerControllable(i, 0);
			Player[i][ToAddInRound] = false;

			UpdateRoundKillDmgTDmg(i);
		}
	}

	UpdateTeamCornerInfo();

	RoundSeconds	 = ConfigRoundTime * 60;

	if (CheckTeamElimination() != 0)
	{
		AllowStartBase = true;
		ArenaStarted   = true;
		foreach (new i : Player)
		{
			ShowRoundStats(i);
		}
		ShowTeamBarsForAll();
		ResetRoundTeamHPMaximums();
		UpdateTeamPlayerCount(ATTACKER, false);
		UpdateTeamPlayerCount(DEFENDER, false);
		UpdateTeamHP(ATTACKER);
		UpdateTeamHP(DEFENDER);
		UpdateTeamNamesTextdraw();
		if (!RestartGameLoop())
			return 0;
		RadarFix();
		foreach (new i : PlayersInRound)
		{
			ResetPlayerWeapons(i);
			GivePlayerWeapon(i, WEAPON_M4, 9999);
			GivePlayerWeapon(i, WEAPON_SHOTGSPA, 9999);
			GivePlayerWeapon(i, WEAPON_DEAGLE, 9999);
		}
		ScheduleRoundSyncEnable(1050);
	}
	return 1;
}

forward OnArenaStart(ArenaID);
public OnArenaStart(ArenaID)
{
	PreStartRoundTimer = 0;
	if (Current != -1)
		return 0;
	if (ArenaID < 0 || ArenaID >= MAX_ARENAS || !Iter_Contains(CurrentArenas, ArenaID))
		return AbortPendingRoundStart();

	AutoAFKCheck();			   // Checks all players and set idle players to afk mode
	ClearKillList();		   // Clears the kill-list.
	DestroyUnusedVehicles(); // Destroys vehicles
	DeleteAllDeadBodies();
	GameType = ARENA;
	RCArena	 = false;
	Current	 = ArenaID; // Current will be the ID of the base that we just started. We do this so that we can use this ID later on (e.g. check /car command for the use).
	ClearKillList();	// Clears the kill-list.
	ServerLastPlayed	 = Current;
	ServerLastPlayedType = 0;
	TotalPausesInRound	 = 0;
	TotalPauseTimeElapsed = 0;

	ElapsedTime			 = 0;

	TeamCapturingCP		 = NON;
	PlayersInCP			 = 0;
	ElapsedTime			 = 0;
	CurrentCPTime		 = ConfigCPTime + 1;
	ResetRoundTextDraws();
	foreach (new i : Player)
	{
		if (!Player[i][Spawned])
			continue;

		if (Player[i][RoundTDStyle] != 1)
			continue;

		TextDrawShowForPlayer(i, timerCircleTD);
	}

	ResetGunmenuSelections();

	new iString[32];
	new bool: isInteriorArena = AMin[Current][0] == -9999.0 && AMin[Current][1] == -9999.0;
	new Float: cameraTargetX = isInteriorArena
		? ACPSpawn[Current][0]
		: (AMin[Current][0] + AMax[Current][0]) / 2.0;
	new Float: cameraTargetY = isInteriorArena
		? ACPSpawn[Current][1]
		: (AMin[Current][1] + AMax[Current][1]) / 2.0;
	format(iString, sizeof(iString), "%sArena %s(~r~%d%s)", MAIN_TEXT_COLOUR, MAIN_TEXT_COLOUR, Current, MAIN_TEXT_COLOUR);

	if (CPZone != INVALID_GANG_ZONE)
	{
		GangZoneDestroy(CPZone);
		CPZone = INVALID_GANG_ZONE;
	}
	if (CPInArena)
	{
		CPZone = GangZoneCreate(ACPSpawn[Current][0] - 75, ACPSpawn[Current][1] - 75, ACPSpawn[Current][0] + 75, ACPSpawn[Current][1] + 75);
	}

	ViewRoundCountdown = ARENA_START_COUNTDOWN;
	new viewGeneration = StartRoundViewSequence();

	new cdString[64];
	format(cdString, sizeof(cdString), "~w~Arena starting in ~r~~h~%d ~w~seconds", ViewRoundCountdown);
	TextDrawSetString(centerTeamNames, cdString);
	TextDrawSetString(AttDef_RoundTimer, cdString);
	TextDrawSetString(ModernRound_Timer, cdString);
	TextDrawSetString(SimpleRound_Timer, cdString);
	UpdateRoundTeamStatusTimer(" ");
	UpdateRoundTeamStatusMessages();

	foreach (new i : Player)
	{
		Player[i][WasInCP]	   = false;

		if (Player[i][ToAddInRound])
		{
			if (!AllVsAllMode && Player[i][Team] != ATTACKER && Player[i][Team] != DEFENDER && Player[i][Team] != REFEREE)
			{
				Player[i][ToAddInRound] = false;
				HideRoundStats(i);
				continue;
			}
			HideEndRoundTextDraw(i);

			if (Player[i][Spectating])
				StopSpectate(i);

			ClearPlayerDMState(i);
			SetPlayerRoundParticipation(i, true);

			SetPlayerVirtualWorld(
				i,
				2); //Set player virtual world to something different that that for lobby and DM so that they don't collide with each other. e.g. You shouldn't be able to see players in lobby or DM while you are in base.
			TogglePlayerControllable(i, 0); //Pause players.

			SetPlayerInterior(i, AInterior[Current]);
			SetPlayerCameraLookAt(i, cameraTargetX, cameraTargetY, ACPSpawn[Current][2]);
			SetPlayerCameraPos(i, cameraTargetX, cameraTargetY + 60.0, ACPSpawn[Current][2] + 80.0);
			SetPlayerPos(i, ACPSpawn[Current][0] + 10.0, ACPSpawn[Current][1] + 10.0, ACPSpawn[Current][2]);
			SetCPForPlayer(i);
		}

		if (Player[i][Team] != REFEREE && (Player[i][ToAddInRound] || Player[i][Playing]))
			ShowRoundStats(i);
		else
			HideRoundStats(i);

		UpdateBaseIDTextDraw(i);
	}

	if (AMin[Current][0] == -9999.0 && AMin[Current][1] == -9999.0
		&& AMax[Current][0] == 9999.0 && AMax[Current][1] == 9999.0)
		ArenaZone = INVALID_GANG_ZONE;
	else
		ArenaZone = GangZoneCreate(AMin[Current][0], AMin[Current][1], AMax[Current][0], AMax[Current][1]);
	CreateBlackoutZone();
	format(iString, sizeof(iString), "game.map Arena: %d", Current);
	SendRconCommand(iString);

	if (ArenaZone != INVALID_GANG_ZONE)
		GangZoneShowForAll(ArenaZone, GANGZONE_ARENA);

	ViewArenaForPlayers(viewGeneration);

	if (WarMode)
	{
		format(iString, sizeof(iString), "%s: %d - %s: %d", TeamName[ATTACKER], TeamScore[ATTACKER], TeamName[DEFENDER], TeamScore[DEFENDER]);
		SetGameModeText(iString);
		MatchRoundsStarted++;
		if (MatchRoundsStarted > 100)
			MatchRoundsStarted = 1;
		MatchRoundsRecord[MatchRoundsStarted - 1][round__type]		= 1;
		MatchRoundsRecord[MatchRoundsStarted - 1][round__ID]		= ArenaID;
		MatchRoundsRecord[MatchRoundsStarted - 1][round__completed] = false;
	}
	return 1;
}

new Float: ArenaCameraAngle[MAX_PLAYERS];

forward ArenaCameraMovementTick(generation);
public ArenaCameraMovementTick(generation)
{
	if (generation != ArenaCameraGeneration)
		return 0;

	if (Current < 0 || Current >= MAX_ARENAS || GameType != ARENA)
	{
		StopArenaCameraMovement();
		return 0;
	}

	new bool:isInterior = (AMin[Current][0] == -9999.0 && AMin[Current][1] == -9999.0);
	new Float:centerX = (AMin[Current][0] + AMax[Current][0]) / 2.0;
	new Float:centerY = (AMin[Current][1] + AMax[Current][1]) / 2.0;
	new Float:targetX = isInterior ? ACPSpawn[Current][0] : centerX;
	new Float:targetY = isInterior ? ACPSpawn[Current][1] : centerY;

	foreach (new i : Player)
	{
		if (!Player[i][ToAddInRound])
			continue;

		new Float:angle = ArenaCameraAngle[i];
		if (angle > 180.0)
			angle = 180.0;

		new Float:progress = angle / 180.0;
		new Float: cameraHeight = 80.0 - (78.0 * progress);
		new Float:radius = 60.0 - (58.0 * progress);

		new Float:cameraX = targetX + radius * floatsin(angle, degrees);
		new Float:cameraY = targetY + radius * floatcos(angle, degrees);

		SetPlayerCameraPos(i, cameraX, cameraY, ACPSpawn[Current][2] + cameraHeight);
		SetPlayerCameraLookAt(i, targetX, targetY, ACPSpawn[Current][2]);

		ArenaCameraAngle[i] = angle + ARENA_CAMERA_ANGLE_STEP;
		if (ArenaCameraAngle[i] > 180.0)
			ArenaCameraAngle[i] = 180.0;
	}
	return 1;
}

forward ViewArenaForPlayers(generation);
public ViewArenaForPlayers(generation)
{
	if (generation != RoundViewGeneration || Current < 0 || GameType != ARENA || RCArena)
		return 0;

	if (ViewRoundCountdown == 0)
	{
		if (ViewRoundTimer)
		{
			KillTimer(ViewRoundTimer);
			ViewRoundTimer = 0;
		}
		StopArenaCameraMovement();
		ClearRoundStartCommandRestrictions();
		SpawnPlayersInArena();
		ResetTeamLeaders();
		TextDrawSetString(centerTeamNames, " ");
		TextDrawSetString(AttDef_RoundTimer, " ");
		TextDrawSetString(ModernRound_Timer, " ");
		TextDrawSetString(SimpleRound_Timer, " ");
		if (Current != -1)
		{
			UpdateRoundTeamStatusTimer(" ");
			UpdateRoundTeamStatusMessages();
		}
		return 1;
	}

	if (!ViewRoundTimer)
	{
		ViewRoundTimer = SetTimerEx("ViewArenaForPlayers", 1000, true, "i", generation);
		if (!ViewRoundTimer)
		{
			printf("[AttDef] Failed to schedule arena round countdown; starting players immediately.");
			ViewRoundCountdown = 0;
			return ViewArenaForPlayers(generation);
		}
	}
	if (!ArenaCameraMovementTimer)
	{
		foreach (new i : Player)
		{
			if (!Player[i][ToAddInRound])
				continue;
			ArenaCameraAngle[i] = 0.0;
		}

		ArenaCameraGeneration++;
		if (ArenaCameraGeneration == 0)
			ArenaCameraGeneration++;
		new cameraGeneration = ArenaCameraGeneration;
		ArenaCameraMovementTick(cameraGeneration);
		ArenaCameraMovementTimer = SetTimerEx(
			"ArenaCameraMovementTick", ARENA_CAMERA_TICK_MS, true, "i", cameraGeneration);
		if (!ArenaCameraMovementTimer)
			printf("[AttDef] Failed to schedule the arena camera movement.");
	}
	new iString[64];
	format(iString, sizeof(iString), "~w~Arena starting in ~r~~h~%d ~w~seconds", ViewRoundCountdown);
	TextDrawSetString(centerTeamNames, iString);
	TextDrawSetString(AttDef_RoundTimer, iString);
	TextDrawSetString(ModernRound_Timer, iString);
	TextDrawSetString(SimpleRound_Timer, iString);
	UpdateRoundTeamStatusTimer(" ");
	UpdateRoundTeamStatusMessages();

	foreach (new i : Player)
	{
		if (Player[i][ToAddInRound])
		{
			AllowedViewingCommands[i] = true;
			PlayerPlaySound(i, 1139, 0.0, 0.0, 0.0);
		}
	}

	foreach (new i : Player)
	{
		if (Player[i][Team])
		{
			BlackListViewingCommands[i] = true;
			NormalListViewingCommands[i] = true;
		}
	}

	ViewRoundCountdown--;
	return 1;
}

GetAllVsAllSpawnPoint(playerid, &Float:x, &Float:y, &Float:z)
{
	if (Current < 0 || Current >= MAX_ARENAS || !AExist[Current])
		return 0;

	new bool: isInterior = (AInterior[Current] > 0)
		|| (AMin[Current][0] == -9999.0 && AMin[Current][1] == -9999.0);

	// 1. For exterior arenas with ColAndreas collision ready, generate a truly random point inside bounds
	if (!isInterior && TeamPingCollisionReady && AMax[Current][0] > AMin[Current][0] && AMax[Current][1] > AMin[Current][1])
	{
		new Float: minX = AMin[Current][0];
		new Float: maxX = AMax[Current][0];
		new Float: minY = AMin[Current][1];
		new Float: maxY = AMax[Current][1];

		// Margin to keep players safely inside arena boundaries without triggering zone warnings
		new Float: marginX = (maxX - minX > 20.0) ? 6.0 : 1.5;
		new Float: marginY = (maxY - minY > 20.0) ? 6.0 : 1.5;
		minX += marginX;
		maxX -= marginX;
		minY += marginY;
		maxY -= marginY;

		// Reference Z elevation from arena spawns
		new Float: attZ = AAttackerSpawn[Current][2];
		new Float: defZ = ADefenderSpawn[Current][2];
		new Float: minRefZ = (attZ < defZ) ? attZ : defZ;
		new Float: maxRefZ = (attZ > defZ) ? attZ : defZ;
		if (CPInArena)
		{
			if (ACPSpawn[Current][2] < minRefZ) minRefZ = ACPSpawn[Current][2];
			if (ACPSpawn[Current][2] > maxRefZ) maxRefZ = ACPSpawn[Current][2];
		}

		// First pass: try up to 25 random points at least 9.0m away from any alive player
		// Second pass: try up to 15 random points if arena is crowded
		for (new pass = 0; pass < 2; pass++)
		{
			new maxAttempts = (pass == 0) ? 25 : 15;
			for (new attempt = 0; attempt < maxAttempts; attempt++)
			{
				new Float: candX = minX + float(random(floatround((maxX - minX) * 100.0))) / 100.0;
				new Float: candY = minY + float(random(floatround((maxY - minY) * 100.0))) / 100.0;

				new Float: hitX, Float: hitY, Float: hitZ;
				// Raycast vertically downwards from above play area down to ground
				new hitModel = CA_RayCastLine(candX, candY, maxRefZ + 28.0, candX, candY, minRefZ - 4.0, hitX, hitY, hitZ);
				if (hitModel == 0)
					continue; // Void, water, or outside collision data

				// Prevent spawning on top of skyscraper/building roofs or under the map
				if (hitZ < minRefZ - 3.0 || hitZ > maxRefZ + 6.0)
					continue;

				// Check vertical clearance: player body height from hitZ+0.2 to hitZ+1.85
				new Float: cx, Float: cy, Float: cz;
				if (CA_RayCastLine(hitX, hitY, hitZ + 0.2, hitX, hitY, hitZ + 1.85, cx, cy, cz) != 0)
					continue; // Ceiling, low roof, or inside an object

				// Check horizontal clearance (player radius ~0.55m at waist height)
				if (CA_RayCastLine(hitX, hitY, hitZ + 0.9, hitX + 0.55, hitY, hitZ + 0.9, cx, cy, cz) != 0
				 || CA_RayCastLine(hitX, hitY, hitZ + 0.9, hitX - 0.55, hitY, hitZ + 0.9, cx, cy, cz) != 0
				 || CA_RayCastLine(hitX, hitY, hitZ + 0.9, hitX, hitY + 0.55, hitZ + 0.9, cx, cy, cz) != 0
				 || CA_RayCastLine(hitX, hitY, hitZ + 0.9, hitX, hitY - 0.55, hitZ + 0.9, cx, cy, cz) != 0)
					continue; // Inside or touching a wall/fence

				// Pass 0: check distance to other alive players
				if (pass == 0)
				{
					new bool: tooClose = false;
					foreach (new p : PlayersInRound)
					{
						if (p == playerid || !Player[p][Playing])
							continue;
						new pstate = GetPlayerState(p);
						if (pstate == PLAYER_STATE_WASTED || pstate == PLAYER_STATE_SPECTATING)
							continue;
						if (IsPlayerInRangeOfPoint(p, 9.0, hitX, hitY, hitZ))
						{
							tooClose = true;
							break;
						}
					}
					if (tooClose)
						continue;
				}

				// Valid surface inside arena found!
				x = hitX;
				y = hitY;
				z = hitZ + 0.35;
				return 1;
			}
		}
	}

	// 2. Fallback / Interior arenas: pick among authored arena points with safe random jitter
	new Float: spawnX[3], Float: spawnY[3], Float: spawnZ[3];
	spawnX[0] = AAttackerSpawn[Current][0];
	spawnY[0] = AAttackerSpawn[Current][1];
	spawnZ[0] = AAttackerSpawn[Current][2];
	spawnX[1] = ADefenderSpawn[Current][0];
	spawnY[1] = ADefenderSpawn[Current][1];
	spawnZ[1] = ADefenderSpawn[Current][2];
	spawnX[2] = ACPSpawn[Current][0];
	spawnY[2] = ACPSpawn[Current][1];
	spawnZ[2] = ACPSpawn[Current][2];

	new pointCount = CPInArena ? 3 : 2;
	new spawnCrowd[3] = {0, 0, 0};

	foreach (new i : PlayersInRound)
	{
		if (i == playerid || !Player[i][Playing])
			continue;
		new pstate = GetPlayerState(i);
		if (pstate == PLAYER_STATE_WASTED || pstate == PLAYER_STATE_SPECTATING)
			continue;

		for (new point = 0; point < pointCount; point++)
		{
			if (IsPlayerInRangeOfPoint(i, 10.0, spawnX[point], spawnY[point], spawnZ[point]))
				spawnCrowd[point]++;
		}
	}

	new leastCrowded = MAX_PLAYERS + 1;
	new candidates[3], candidateCount = 0;
	for (new point = 0; point < pointCount; point++)
	{
		if (spawnCrowd[point] < leastCrowded)
		{
			leastCrowded = spawnCrowd[point];
			candidateCount = 0;
		}
		if (spawnCrowd[point] == leastCrowded)
			candidates[candidateCount++] = point;
	}

	new selected = (candidateCount > 0) ? candidates[random(candidateCount)] : 0;
	new Float: baseAnchorX = spawnX[selected];
	new Float: baseAnchorY = spawnY[selected];
	new Float: baseAnchorZ = spawnZ[selected];

	// Add random radial jitter around anchor (1.0m to 4.0m)
	new Float: angle = float(random(360));
	new Float: dist = 1.0 + float(random(30)) / 10.0;
	new Float: jx = baseAnchorX + dist * floatcos(angle, degrees);
	new Float: jy = baseAnchorY + dist * floatsin(angle, degrees);

	if (!isInterior && TeamPingCollisionReady)
	{
		new Float: hx, Float: hy, Float: hz;
		if (CA_RayCastLine(jx, jy, baseAnchorZ + 8.0, jx, jy, baseAnchorZ - 3.0, hx, hy, hz) != 0)
		{
			new Float: cx, Float: cy, Float: cz;
			if (CA_RayCastLine(hx, hy, hz + 0.2, hx, hy, hz + 1.8, cx, cy, cz) == 0)
			{
				x = hx;
				y = hy;
				z = hz + 0.35;
				return 1;
			}
		}
	}

	x = jx;
	y = jy;
	z = baseAnchorZ + 0.5;
	return 1;
}

SpawnAllVsAllPlayer(playerid)
{
	new Float:x, Float:y, Float:z;
	if (!GetAllVsAllSpawnPoint(playerid, x, y, z))
	{
		x = AAttackerSpawn[Current][0];
		y = AAttackerSpawn[Current][1];
		z = AAttackerSpawn[Current][2] + 0.5;
	}

	SetPlayerVirtualWorld(playerid, 2);
	SetPlayerInterior(playerid, AInterior[Current]);
	SetSpawnInfo(playerid, NO_TEAM, Skin[Player[playerid][Team]], x, y, z, 0.0, 0, 0, 0, 0, 0, 0);
	SetPlayerTeam(playerid, NO_TEAM);
	Player[playerid][IgnoreSpawn] = true;
	SpawnPlayer(playerid);

	SetPlayerVirtualWorld(playerid, 2);
	SetPlayerInterior(playerid, AInterior[Current]);
	SetPlayerPos(playerid, x, y, z);
	SetHPAP(playerid, RoundHP, RoundAR);
	SetCameraBehindPlayer(playerid);
	TogglePlayerControllable(playerid, !RoundPaused);

	if (Player[playerid][WeaponPicked])
		RestorePlayerGunmenuWeapons(playerid);
	else
		ShowPlayerGunmenu(playerid, 500);
	return 1;
}

forward RespawnAllVsAllPlayer(playerid, connectionGeneration, deathGeneration);
public RespawnAllVsAllPlayer(playerid, connectionGeneration, deathGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration)
		return 0;
	if (!AllVsAllMode || GameType != ARENA || !ArenaStarted || Current == -1 || !Player[playerid][Playing])
		return 0;

	// If player already respawned and is alive in arena, avoid double spawning
	if (GetPlayerState(playerid) == PLAYER_STATE_SPAWNED && (GetHP(playerid) + GetAP(playerid)) > 0)
		return 1;

	return SpawnAllVsAllPlayer(playerid);
}

SpawnPlayersInAllVsAll()
{
	DisableRoundSync();
	FallProtection = true;
	ClearChat();
	SendClientMessageToAll(-1, va_return("{FFFFFF}\xBB All-vs-All "COL_PRIM"'%s' {FFFFFF}| ID: "COL_PRIM"%d {FFFFFF}| Info: "COL_PRIM"Most kills when time expires wins.", AName[Current], Current));

	foreach (new i : Player)
	{
		HideRoundStats(i);
		Player[i][PreAllVsAllStyle] = Player[i][RoundTDStyle];
		Player[i][RoundTDStyle] = 2; // Simple style

		if (!Player[i][ToAddInRound])
			continue;

		if (Player[i][Spectating])
			StopSpectate(i);
		SetPlayerRoundParticipation(i, true);
		Player[i][WasInBase] = true;
		Player[i][WasInTeam] = Player[i][Team];
		Player[i][ToAddInRound] = false;
		ResetPlayerRoundVariables(i);
		Player[i][WeaponPicked] = 0;
		ResetPlayerGunmenu(i, false);
		SpawnAllVsAllPlayer(i);
		UpdateRoundKillDmgTDmg(i);
	}

	RoundSeconds = ConfigRoundTime * 60;
	AllowStartBase = true;
	ArenaStarted = true;
	ShowTeamBarsForAll();
	if (Iter_Count(PlayersInRound) < 2)
	{
		SendClientMessageToAll(-1, "{FF6666}All-vs-All needs at least two players.");
		return EndRound(4);
	}

	new timerStr[16];
	format(timerStr, sizeof(timerStr), "~w~%d:%02d", RoundSeconds / 60, RoundSeconds % 60);
	TextDrawSetString(timerCenterTD, timerStr);
	TextDrawSetString(AttDef_RoundTimer, timerStr);
	TextDrawSetString(ModernRound_Timer, timerStr);
	TextDrawSetString(SimpleRound_Timer, timerStr);

	UpdateAllVsAllTextDraws();

	foreach (new i : Player)
	{
		if (Player[i][Playing] || Player[i][Spectating])
		{
			ShowRoundStats(i);
		}
	}

	if (!RestartGameLoop())
		return 0;
	RadarFix();
	ScheduleRoundSyncEnable(1050);
	return 1;
}

SpawnPlayersInArena()
{
	if (AllVsAllMode)
		return SpawnPlayersInAllVsAll();

	ArenaStarted = true;
	DisableRoundSync();
	FallProtection = true;
	ClearChat();
	new Amessage[144];
	format(Amessage, sizeof(Amessage), "\xBB " COL_PRIM "Arena %d " COL_MUTED "(" COL_SEC "%s" COL_MUTED ") " COL_PRIM "has started.", Current, AName[Current]);
	SendClientMessageToAll(-1, Amessage);
	SendClientMessageToAll(-1, va_return("%s\xBB Played: %d {FFFFFF}%sAttackers: %d {FFFFFF}%sDefenders: %d", TextColor[REFEREE], APlayed[Current], TextColor[ATTACKER], AAttWins[Current], TextColor[DEFENDER], ADefWins[Current]));

	foreach (new i : Player)
	{
		if (Player[i][ToAddInRound] && (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER || Player[i][Team] == REFEREE))
		{
			SetPlayerRoundParticipation(i, true);
		}
	}

	AnnounceTeamRoster(ATTACKER);
	AnnounceTeamRoster(DEFENDER);
	AnnounceTeamRoster(REFEREE);

	foreach (new i : Player)
	{
		if (Player[i][ToAddInRound])
		{
			ClearPlayerDMState(i);

			if (Player[i][Spectating])
				StopSpectate(i);

			if (!Player[i][FakePacketRenovation] && Player[i][NetCheck] && NetStats_GetConnectedTime(i) < 300000)
			{
				ScheduleFakePacketRenovation(i, 30000, false, NetStats_PacketLossPercent(i));
			}

			SetPlayerRoundParticipation(i, true);
			Player[i][WasInBase] = true;
			Player[i][WasInTeam] = Player[i][Team];

			ResetPlayerRoundVariables(i);

			PlayerPlaySound(i, 1057, 0, 0, 0);
			SetPlayerVirtualWorld(i, 2);
			SetPlayerInterior(i, AInterior[Current]);

			SetHPAP(i, RoundHP, RoundAR);

			CreatePlayerTeamBar(i);

			ShowTeamCornerInfo(i);

			SetCPForPlayer(i);
			Player[i][OutOfArena] = MAX_ZONE_LIMIT_WARNINGS;

			switch (Player[i][Team])
			{
				case ATTACKER:
				{
					if (AInterior[Current] == 0)
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], AAttackerSpawn[Current][0] + RandomMinMax(-3, 3), AAttackerSpawn[Current][1] + RandomMinMax(-3, 3),
							AAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
					else
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], AAttackerSpawn[Current][0] + RandomMinMax(-1, 1), AAttackerSpawn[Current][1] + RandomMinMax(-1, 1),
							AAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);

					Player[i][IgnoreSpawn] = true;
					SetPlayerColor(i, ATTACKER_PLAYING);
					SpawnPlayer(i);
					SetPlayerTeam(i, NO_TEAM);
				}
				case DEFENDER:
				{
					if (AInterior[Current] == 0)
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], ADefenderSpawn[Current][0] + RandomMinMax(-3, 3), ADefenderSpawn[Current][1] + RandomMinMax(-3, 3),
							ADefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
					else
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], ADefenderSpawn[Current][0] + RandomMinMax(-1, 1), ADefenderSpawn[Current][1] + RandomMinMax(-1, 1),
							ADefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);

					Player[i][IgnoreSpawn] = true;
					SetPlayerColor(i, DEFENDER_PLAYING);
					SpawnPlayer(i);
					SetPlayerTeam(i, NO_TEAM);
				}
			}
			SetCameraBehindPlayer(i);
			SetHPAP(i, RoundHP, RoundAR);
			if (!RoundPaused)
				TogglePlayerControllable(i, 1);
			else
				TogglePlayerControllable(i, 0);
			Player[i][ToAddInRound] = false;

			UpdateRoundKillDmgTDmg(i);
			ShowRoundStats(i, false);
		}
	}
	ResetRoundVariablesInSlots();

	UpdateTeamCornerInfo();

	RoundSeconds	 = ConfigRoundTime * 60;

	if (CheckTeamElimination() != 0)
	{
		AllowStartBase = true;
		ArenaStarted   = true;
		foreach (new i : Player)
		{
			ShowRoundStats(i);
		}
		ShowTeamBarsForAll();
		ResetRoundTeamHPMaximums();
		UpdateTeamPlayerCount(ATTACKER, false);
		UpdateTeamPlayerCount(DEFENDER, false);
		UpdateTeamHP(ATTACKER);
		UpdateTeamHP(DEFENDER);
		UpdateTeamNamesTextdraw();
		if (!RestartGameLoop())
			return 0;
		RadarFix();
		SchedulePlayingPlayersGunmenu(1000);
		ScheduleRoundSyncEnable(1050);
	}
	return 1;
}

forward AddPlayerToArena(playerid);
public AddPlayerToArena(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	PlayerRoundEntryEpoch[playerid]++;

	if (AllVsAllMode)
	{
		if (Player[playerid][Spectating])
		{
			StopSpectate(playerid);
			return ScheduleDelayedRoundEntry(playerid, ARENA);
		}

		HideRoundStats(playerid);
		Player[playerid][PreAllVsAllStyle] = Player[playerid][RoundTDStyle];
		Player[playerid][RoundTDStyle] = 2; // Simple style

		SetPlayerRoundParticipation(playerid, true);
		Player[playerid][WasInBase] = true;
		Player[playerid][WasInTeam] = Player[playerid][Team];
		Player[playerid][ToAddInRound] = false;
		ResetPlayerRoundVariables(playerid);
		Player[playerid][WeaponPicked] = 0;
		ResetPlayerGunmenu(playerid, false);
		SpawnAllVsAllPlayer(playerid);
		UpdateRoundKillDmgTDmg(playerid);
		ShowRoundStats(playerid);
		UpdateAllVsAllTextDraws();
		return 1;
	}

	if (Player[playerid][Spectating])
	{
		StopSpectate(playerid);
		return ScheduleDelayedRoundEntry(playerid, ARENA);
	}

	ClearPlayerDMState(playerid);

	if (Player[playerid][LastVehicle])
	{
		foreach (new i : VehicleDriver(Player[playerid][LastVehicle]))
		{
			RemovePlayerFromVehicle(i);
		}
		foreach (new i : VehiclePassenger(Player[playerid][LastVehicle]))
		{
			RemovePlayerFromVehicle(i);
		}
		DestroyVehicle(Player[playerid][LastVehicle]);
		Player[playerid][LastVehicle] = 0;
	}

	if (!Player[playerid][FakePacketRenovation] && Player[playerid][NetCheck] && NetStats_GetConnectedTime(playerid) < 300000)
	{
		ScheduleFakePacketRenovation(playerid, 30000, false, NetStats_PacketLossPercent(playerid));
	}

	SetPlayerRoundParticipation(playerid, true);
	Player[playerid][WasInBase] = true;
	Player[playerid][WasInTeam] = Player[playerid][Team];
	UpdateTeamPlayerCount(Player[playerid][Team], false);
	Player[playerid][ToAddInRound] = false;
	ResetPlayerRoundVariables(playerid);

	PlayerPlaySound(playerid, 1057, 0, 0, 0);
	SetCameraBehindPlayer(playerid);

	ShowTeamCornerInfo(playerid);
	UpdateTeamCornerInfo();

	SetPlayerVirtualWorld(playerid, 2);
	SetPlayerInterior(playerid, AInterior[Current]);
	if (!RCArena)
		SetCPForPlayer(playerid);

	switch (Player[playerid][Team])
	{
		case ATTACKER:
		{
			SetSpawnInfo(
				playerid, Player[playerid][Team], Skin[Player[playerid][Team]], AAttackerSpawn[Current][0] + RandomMinMax(-1, 1), AAttackerSpawn[Current][1] + RandomMinMax(-1, 1),
				AAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
			SetPlayerMapIcon(playerid, 59, AAttackerSpawn[Current][0], AAttackerSpawn[Current][1], AAttackerSpawn[Current][2], 31, 0, MAPICON_GLOBAL);

			Player[playerid][IgnoreSpawn] = true;
			SpawnPlayer(playerid);
			SetPlayerColor(playerid, ATTACKER_PLAYING);

			SetPlayerTeam(playerid, NO_TEAM);
		}
		case DEFENDER:
		{
			SetSpawnInfo(
				playerid, Player[playerid][Team], Skin[Player[playerid][Team]], ADefenderSpawn[Current][0] + RandomMinMax(-1, 1), ADefenderSpawn[Current][1] + RandomMinMax(-1, 1),
				ADefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);

			Player[playerid][IgnoreSpawn] = true;
			SpawnPlayer(playerid);
			SetPlayerColor(playerid, DEFENDER_PLAYING);
			SetPlayerTeam(playerid, NO_TEAM);
		}
		case REFEREE:
		{
			SetSpawnInfo(
				playerid, Player[playerid][Team], Skin[Player[playerid][Team]], ACPSpawn[Current][0] + RandomMinMax(-1, 1), ACPSpawn[Current][1] + RandomMinMax(-1, 1), ACPSpawn[Current][2] + 0.5, 0,
				0, 0, 0, 0, 0, 0);
			Player[playerid][IgnoreSpawn] = true;
			SpawnPlayer(playerid);
			SetPlayerColor(playerid, REFEREE_COLOR);
			SetPlayerTeam(playerid, NO_TEAM);
			HideRoundStats(playerid);
		}
	}

	if (!RoundPaused)
		TogglePlayerControllable(playerid, 1);
	else
		TogglePlayerControllable(playerid, 0);

	SetHPAP(playerid, RoundHP, RoundAR);
	if (!CreatePlayerBlackoutZone(playerid))
		printf("[AttDef] Failed to show arena blackout zones for player %d.", playerid);

	if (Player[playerid][Team] == ATTACKER || Player[playerid][Team] == DEFENDER)
	{
		ReorganizeTeamBars(Player[playerid][Team]);
		UpdatePlayerTeamBar(playerid);
		UpdateTeamHP(Player[playerid][Team]);
		ShowTeamBarsForPlayer(playerid);
	}

	if (!RCArena)
		ShowPlayerGunmenu(playerid, 500);
	else
	{
		ResetPlayerWeapons(playerid);
		GivePlayerWeapon(playerid, WEAPON_M4, 9999);
		GivePlayerWeapon(playerid, WEAPON_SHOTGSPA, 9999);
		GivePlayerWeapon(playerid, WEAPON_DEAGLE, 9999);
	}

	if (Player[playerid][Team] != REFEREE)
		UpdateRoundKillDmgTDmg(playerid);

	RadarFix();
	return 1;
}

forward AddPlayerToArenaDelayed(playerid, generation, entryEpoch, roundID, roundType, roundGeneration);
public AddPlayerToArenaDelayed(playerid, generation, entryEpoch, roundID, roundType, roundGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != generation || PlayerRoundEntryEpoch[playerid] != entryEpoch
		|| Current != roundID || GameType != roundType || roundType != ARENA
		|| RoundEndGeneration != roundGeneration)
		return 0;
	return AddPlayerToArena(playerid);
}
