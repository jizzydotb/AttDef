#if defined _attdef_match_base
	#endinput
#endif
#define _attdef_match_base

/*
	Match/Round Base System Sub-module
*/

forward OnBaseStart(BaseID);
public OnBaseStart(BaseID)
{
	PreStartRoundTimer = 0;
	if (Current != -1)
		return 0;
	if (BaseID < 0 || BaseID >= MAX_BASES || !Iter_Contains(CurrentBases, BaseID))
		return AbortPendingRoundStart();

	AutoAFKCheck();			 // Checks all players and set idle players to afk mode
	ClearKillList();		 // Clears the kill-list.
	DestroyUnusedVehicles(); // Destroys vehicles
	DeleteAllDeadBodies();
	GameType = BASE;
	RCArena	 = false;
	Current	 = BaseID; // Current will be the ID of the base that we just started. We do this so that we can use this ID later on (e.g. check /car command for the use).
	ClearKillList();   // Clears the kill-list.
	ServerLastPlayed	 = Current;
	ServerLastPlayedType = 1;
	TotalPausesInRound	 = 0;
	TotalPauseTimeElapsed = 0;

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
	format(iString, sizeof(iString), "%sBase %s(~r~%d%s)", MAIN_TEXT_COLOUR, MAIN_TEXT_COLOUR, Current, MAIN_TEXT_COLOUR);

	ViewRoundCountdown		  = 9;
	new viewGeneration = StartRoundViewSequence();

	new cdString[64];
	format(cdString, sizeof(cdString), "~w~Base starting in ~r~~h~%d ~w~seconds", ViewRoundCountdown);
	TextDrawSetString(centerTeamNames, cdString);
	TextDrawSetString(AttDef_RoundTimer, cdString);
	TextDrawSetString(ModernRound_Timer, cdString);
	TextDrawSetString(SimpleRound_Timer, cdString);
	UpdateRoundTeamStatusTimer(" ");
	UpdateRoundTeamStatusMessages();

	CPZone			  = GangZoneCreate(BCPSpawn[Current][0] - 75, BCPSpawn[Current][1] - 75, BCPSpawn[Current][0] + 75, BCPSpawn[Current][1] + 75);
	UpdateCPZoneVisibility(INVALID_PLAYER_ID);

	foreach (new i : Player)
	{
		Player[i][WasInCP] = false;

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

			if (Player[i][InDM] || Player[i][DMReadd] != -1)
			{
				ClearPlayerDMState(i);
			}

			SetPlayerRoundParticipation(i, true);

			// Set player virtual world to something different that that for lobby and DM so that they don't collide with each other. e.g. You shouldn't be able to see players in lobby or DM while you are in base.
			SetPlayerVirtualWorld(i, 2);
			SetPlayerInterior(i, BInterior[Current]);
			TogglePlayerControllable(i, 0); //Pause players.
			SetPlayerCameraLookAt(i, BCPSpawn[Current][0], BCPSpawn[Current][1], BCPSpawn[Current][2]);
			SetPlayerCameraPos(i, BCPSpawn[Current][0], BCPSpawn[Current][1], BCPSpawn[Current][2]);
			SetPlayerPos(i, BCPSpawn[Current][0] + 10, BCPSpawn[Current][1] + 10, BCPSpawn[Current][2]);
			SetCPForPlayer(i); // Set checkpoint location and size.
		}

		if (Player[i][Team] != REFEREE && (Player[i][ToAddInRound] || Player[i][Playing]))
			ShowRoundStats(i);
		else
			HideRoundStats(i);

		UpdateBaseIDTextDraw(i);
	}
	UpdateCPZoneVisibility(INVALID_PLAYER_ID);
	StartBaseCameraMovement();
	ViewBaseForPlayers(viewGeneration);

	format(iString, sizeof(iString), "game.map Base: %d", Current); // Sets the map name in the server browser.
	SendRconCommand(iString);

	if (WarMode)
	{
		format(iString, sizeof(iString), "%s: %d - %s: %d", TeamName[ATTACKER], TeamScore[ATTACKER], TeamName[DEFENDER], TeamScore[DEFENDER]);
		SetGameModeText(iString);
		MatchRoundsStarted++;
		if (MatchRoundsStarted > 100)
			MatchRoundsStarted = 1;
		MatchRoundsRecord[MatchRoundsStarted - 1][round__type]		= 0;
		MatchRoundsRecord[MatchRoundsStarted - 1][round__ID]		= BaseID;
		MatchRoundsRecord[MatchRoundsStarted - 1][round__completed] = false;
	}
	return 1;
}

ScheduleRoundStart(roundType, roundID, delay)
{
	if (roundType != BASE && roundType != ARENA)
		return 0;

	CancelAutoRoundStart();
	CallLocalFunction("ResetVoteSession", "");
	CallLocalFunction("DestroyAllTeamPings", "");

	if (PreStartRoundTimer)
		KillTimer(PreStartRoundTimer);

	PendingRoundStartGeneration++;
	if (PendingRoundStartGeneration == 0)
		PendingRoundStartGeneration++;

	PreStartRoundTimer = SetTimerEx(
		"StartPendingRound", delay, false, "iii", roundType, roundID, PendingRoundStartGeneration);
	if (!PreStartRoundTimer)
	{
		PendingRoundStartGeneration++;
		printf("[AttDef] Failed to schedule round start (type %d, id %d); starting immediately.", roundType, roundID);
		if (roundType == BASE)
			return OnBaseStart(roundID);
		return OnArenaStart(roundID);
	}
	return PreStartRoundTimer;
}

CancelPendingRoundStart()
{
	if (PreStartRoundTimer)
	{
		KillTimer(PreStartRoundTimer);
		PreStartRoundTimer = 0;
	}

	PendingRoundStartGeneration++;
	if (PendingRoundStartGeneration == 0)
		PendingRoundStartGeneration++;
	return 1;
}

forward StartPendingRound(roundType, roundID, generation);
public StartPendingRound(roundType, roundID, generation)
{
	if (generation != PendingRoundStartGeneration || !PreStartRoundTimer)
		return 0;

	PreStartRoundTimer = 0;
	switch (roundType)
	{
		case BASE: return OnBaseStart(roundID);
		case ARENA: return OnArenaStart(roundID);
	}
	return 0;
}

forward ViewBaseForPlayers(generation);
public ViewBaseForPlayers(generation)
{
	if (generation != RoundViewGeneration || Current < 0 || GameType != BASE)
		return 0;

	if (ViewRoundCountdown == 0)
	{
		if (ViewRoundTimer)
		{
			KillTimer(ViewRoundTimer);
			ViewRoundTimer = 0;
		}
		StopBaseCameraMovement();
		ClearRoundStartCommandRestrictions();
		SpawnPlayersInBase();
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
		ViewRoundTimer = SetTimerEx("ViewBaseForPlayers", 1000, true, "i", generation);
		if (!ViewRoundTimer)
		{
			printf("[AttDef] Failed to schedule base round countdown; starting players immediately.");
			ViewRoundCountdown = 0;
			return ViewBaseForPlayers(generation);
		}
	}
	new iString[64];
	format(iString, sizeof(iString), "~w~Base starting in ~r~~h~%d ~w~seconds", ViewRoundCountdown);
	TextDrawSetString(centerTeamNames, iString);
	TextDrawSetString(AttDef_RoundTimer, iString);
	TextDrawSetString(ModernRound_Timer, iString);
	TextDrawSetString(SimpleRound_Timer, iString);

	foreach (new i : Player)
	{
		if (!Player[i][ToAddInRound])
			continue;

		PlayerPlaySound(i, 1139, 0.0, 0.0, 0.0);
	}

	UpdateRoundTeamStatusTimer(" ");
	UpdateRoundTeamStatusMessages();

	foreach (new i : Player)
	{
		if (Player[i][ToAddInRound])
		{
			AllowedViewingCommands[i] = true;
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

forward EnableVehicleCollisions(playerid, connectionGeneration, roundID, roundGeneration);
public EnableVehicleCollisions(playerid, connectionGeneration, roundID, roundGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration || !Player[playerid][Playing]
		|| Current != roundID || RoundEndGeneration != roundGeneration)
		return 0;

	if(Current != - 1)
	{
		DisableRemoteVehicleCollisions(playerid, false);
		SendClientMessage(playerid, -1, ""COL_PRIM"\xBB Vehicle collisions "COL_SEC"have been enabled!");
	}
	return 1;
}

SpawnPlayersInBase()
{
	BaseStarted = true;
	DisableRoundSync();
	FallProtection = true;
	ClearChat();
	new Bmessage[144];
	format(Bmessage, sizeof(Bmessage), "\xBB " COL_PRIM "Base %d " COL_MUTED "(" COL_SEC "%s" COL_MUTED ") " COL_PRIM "has started.", Current, BName[Current]);
	SendClientMessageToAll(-1, Bmessage);
	SendClientMessageToAll(-1, va_return("%s\xBB Played: %d {FFFFFF}%sAttackers: %d {FFFFFF}%sDefenders: %d", TextColor[REFEREE], BPlayed[Current], TextColor[ATTACKER], BAttWins[Current], TextColor[DEFENDER], BDefWins[Current]));

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
			SetCameraBehindPlayer(i);

			SetHPAP(i, RoundHP, RoundAR);

			CreatePlayerTeamBar(i);

			ShowTeamCornerInfo(i);

			//Set player virtual world to something different that that for lobby and DM so that they don't collide with each other. e.g. You shouldn't be able to see players in lobby or DM while you are in base.
			SetPlayerVirtualWorld(i, 2);
			SetPlayerInterior(i, BInterior[Current]);

			switch (Player[i][Team])
			{
				case ATTACKER:
				{
					if (BInterior[Current] == 0)
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], BAttackerSpawn[Current][0] + RandomMinMax(-3, 3), BAttackerSpawn[Current][1] + RandomMinMax(-3, 3),
							BAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
					else
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], BAttackerSpawn[Current][0] + RandomMinMax(-1, 1), BAttackerSpawn[Current][1] + RandomMinMax(-1, 1),
							BAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);

					Player[i][IgnoreSpawn] = true;
					SetCPForPlayer(i);
					SetPlayerColor(i, ATTACKER_PLAYING);
					SpawnPlayer(i);
					SetPlayerMapIcon(i, 59, BAttackerSpawn[Current][0], BAttackerSpawn[Current][1], BAttackerSpawn[Current][2], 31, 0, MAPICON_GLOBAL);
					SetPlayerTeam(i, NO_TEAM);

					DisableRemoteVehicleCollisions(i, true);
					if (!SetTimerEx(
						"EnableVehicleCollisions", 15000, false, "iiii", i,
						PlayerConnectionGeneration[i], Current, RoundEndGeneration))
					{
						printf("[AttDef] Failed to schedule vehicle-collision restoration for player %d.", i);
						DisableRemoteVehicleCollisions(i, false);
					}
					SendClientMessage(i, -1, ""COL_PRIM"\xBB Vehicle collisions "COL_SEC"have been disabled "COL_PRIM"for "COL_SEC"15 seconds.");
				}
				case DEFENDER:
				{
					if (BInterior[Current] == 0)
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], BDefenderSpawn[Current][0] + RandomMinMax(-3, 3), BDefenderSpawn[Current][1] + RandomMinMax(-3, 3),
							BDefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
					else
						SetSpawnInfo(
							i, Player[i][Team], Skin[Player[i][Team]], BDefenderSpawn[Current][0] + RandomMinMax(-1, 1), BDefenderSpawn[Current][1] + RandomMinMax(-1, 1),
							BDefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);

					Player[i][IgnoreSpawn] = true;
					SetCPForPlayer(i);
					SetPlayerColor(i, DEFENDER_PLAYING);
					SpawnPlayer(i);
					SetPlayerTeam(i, NO_TEAM);
				}
			}

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
		BaseStarted	   = true;
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

DisableRoundSync()
{
	AllowSync = false;
	RoundSyncGeneration++;
	return 1;
}

ScheduleRoundSyncEnable(delay)
{
	new generation = ++RoundSyncGeneration;
	if (!SetTimerEx("Round_EnableSync", delay, false, "i", generation))
	{
		RoundSyncGeneration++;
		printf("[AttDef] Failed to schedule round sync enable.");
		if (Current != -1 && !AlreadyEndingRound)
			AllowSync = true;
		return 0;
	}
	return 1;
}

forward Round_EnableSync(generation);
public Round_EnableSync(generation)
{
	if (generation != RoundSyncGeneration || Current == -1 || AlreadyEndingRound)
		return 0;
	AllowSync = true;
	return 1;
}

ScheduleDelayedRoundEntry(playerid, roundType)
{
	new entryEpoch = ++PlayerRoundEntryEpoch[playerid];
	new callbackName[32];
	if (roundType == BASE)
		format(callbackName, sizeof callbackName, "AddPlayerToBaseDelayed");
	else
		format(callbackName, sizeof callbackName, "AddPlayerToArenaDelayed");

	if (!SetTimerEx(
		callbackName, 500, false, "iiiiii", playerid,
		PlayerConnectionGeneration[playerid], entryEpoch, Current, roundType, RoundEndGeneration))
	{
		PlayerRoundEntryEpoch[playerid]++;
		SendErrorMessage(playerid, "Could not schedule your round entry. Please try again.");
		printf("[AttDef] Failed to schedule delayed round entry for player %d.", playerid);
		return 0;
	}
	return 1;
}

forward AddPlayerToBase(playerid);
public AddPlayerToBase(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	PlayerRoundEntryEpoch[playerid]++;

	if (Player[playerid][Spectating])
	{
		StopSpectate(playerid);
		return ScheduleDelayedRoundEntry(playerid, BASE);
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

	if (!Player[playerid][WasInBase])
		ResetPlayerRoundVariables(playerid);

	if (!Player[playerid][FakePacketRenovation] && Player[playerid][NetCheck] && NetStats_GetConnectedTime(playerid) < 300000)
	{
		ScheduleFakePacketRenovation(playerid, 30000, false, NetStats_PacketLossPercent(playerid));
	}

	SetPlayerRoundParticipation(playerid, true);
	Player[playerid][WasInBase] = true;
	Player[playerid][WasInTeam] = Player[playerid][Team];
	UpdateTeamPlayerCount(Player[playerid][Team], false);

	PlayerPlaySound(playerid, 1057, 0, 0, 0);
	SetCameraBehindPlayer(playerid);

	ShowTeamCornerInfo(playerid);
	UpdateTeamCornerInfo();

	//Set player virtual world to something different that that for lobby and DM so that they don't collide with each other. e.g. You shouldn't be able to see players in lobby or DM while you are in base.
	SetPlayerVirtualWorld(playerid, 2);
	SetPlayerInterior(playerid, BInterior[Current]);

	switch (Player[playerid][Team])
	{
		case ATTACKER:
		{
			SetSpawnInfo(
				playerid, Player[playerid][Team], Skin[Player[playerid][Team]], BAttackerSpawn[Current][0] + RandomMinMax(-1, 1), BAttackerSpawn[Current][1] + RandomMinMax(-1, 1),
				BAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
			Player[playerid][IgnoreSpawn] = true;
			SpawnPlayer(playerid);
			SetCPForPlayer(playerid);
			SetPlayerColor(playerid, ATTACKER_PLAYING);
			SetPlayerMapIcon(playerid, 59, BAttackerSpawn[Current][0], BAttackerSpawn[Current][1], BAttackerSpawn[Current][2], 31, 0, MAPICON_GLOBAL);
			SetPlayerTeam(playerid, NO_TEAM);
		}
		case DEFENDER:
		{
			SetSpawnInfo(
				playerid, Player[playerid][Team], Skin[Player[playerid][Team]], BDefenderSpawn[Current][0] + RandomMinMax(-1, 1), BDefenderSpawn[Current][1] + RandomMinMax(-1, 1),
				BDefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
			Player[playerid][IgnoreSpawn] = true;
			SpawnPlayer(playerid);
			SetCPForPlayer(playerid);
			SetPlayerColor(playerid, DEFENDER_PLAYING);
			SetPlayerTeam(playerid, NO_TEAM);
		}
		case REFEREE:
		{
			SetSpawnInfo(
				playerid, Player[playerid][Team], Skin[Player[playerid][Team]], BCPSpawn[Current][0] + RandomMinMax(-1, 1), BCPSpawn[Current][1] + RandomMinMax(-1, 1), BCPSpawn[Current][2] + 0.5, 0,
				0, 0, 0, 0, 0, 0);
			Player[playerid][IgnoreSpawn] = true;
			SpawnPlayer(playerid);
			SetCPForPlayer(playerid);
			SetPlayerColor(playerid, REFEREE_COLOR);
			SetPlayerTeam(playerid, NO_TEAM);
			HideRoundStats(playerid);
		}
	}

	SetHPAP(playerid, RoundHP, RoundAR);
	if (Player[playerid][Team] == ATTACKER || Player[playerid][Team] == DEFENDER)
	{
		ReorganizeTeamBars(Player[playerid][Team]);
		UpdatePlayerTeamBar(playerid);
		UpdateTeamHP(Player[playerid][Team]);
		ShowTeamBarsForPlayer(playerid);
	}

	if (!RoundPaused)
		TogglePlayerControllable(playerid, 1);
	else
		TogglePlayerControllable(playerid, 0);

	Player[playerid][ToAddInRound] = false;

	ShowPlayerGunmenu(playerid, 500);

	if (Player[playerid][Team] != REFEREE)
		UpdateRoundKillDmgTDmg(playerid);

	RadarFix();
	return 1;
}

forward AddPlayerToBaseDelayed(playerid, generation, entryEpoch, roundID, roundType, roundGeneration);
public AddPlayerToBaseDelayed(playerid, generation, entryEpoch, roundID, roundType, roundGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != generation || PlayerRoundEntryEpoch[playerid] != entryEpoch
		|| Current != roundID || GameType != roundType || roundType != BASE
		|| RoundEndGeneration != roundGeneration)
		return 0;
	return AddPlayerToBase(playerid);
}
