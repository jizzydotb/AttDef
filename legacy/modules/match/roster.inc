#if defined _attdef_match_roster
	#endinput
#endif
#define _attdef_match_roster

/*
	Match/Round Roster, Substitutions, Swaps & Player State Sub-module
*/

forward SwapBothTeams(generation);
public SwapBothTeams(generation)
{
	if (generation != RoundEndGeneration || Current != -1 || WarModeEnding || !WarMode)
		return 0;

	SwapTeams();
	if (!PreMatchResultsShowing)
		AllowStartBase = true;
	return 1;
}

PreparePlayerForReplacement(playerid)
{
	ClearPlayerDMState(playerid);

	new vehicleid = Player[playerid][LastVehicle];
	if (vehicleid > 0 && IsValidVehicle(vehicleid))
	{
		foreach (new occupant : VehicleDriver(vehicleid))
			RemovePlayerFromVehicle(occupant);
		foreach (new occupant : VehiclePassenger(vehicleid))
			RemovePlayerFromVehicle(occupant);
		DestroyVehicle(vehicleid);
	}
	Player[playerid][LastVehicle] = 0;

	if (Player[playerid][Spectating])
		StopSpectate(playerid);
	return 1;
}

forward OnPlayerReplace(
	ToAddID, ToReplaceID, playerid, addGeneration, replaceGeneration, issuerGeneration, requestGeneration, roundID);
public OnPlayerReplace(
	ToAddID, ToReplaceID, playerid, addGeneration, replaceGeneration, issuerGeneration, requestGeneration, roundID)
{
	if (!IsPlayerConnected(ToAddID) || PlayerConnectionGeneration[ToAddID] != addGeneration
		|| !IsPlayerConnected(ToReplaceID) || PlayerConnectionGeneration[ToReplaceID] != replaceGeneration
		|| !IsPlayerConnected(playerid) || PlayerConnectionGeneration[playerid] != issuerGeneration
		|| Current == -1 || Current != roundID || REPLACE_RequestRound[playerid] != roundID
		|| REPLACE_RequestGeneration[playerid] != requestGeneration || REPLACE_ToAddID[playerid] != ToAddID
		|| REPLACE_ToAddGeneration[playerid] != addGeneration
		|| !Player[ToReplaceID][Playing] || Player[ToAddID][Playing])
		return 0;

	PreparePlayerForReplacement(ToAddID);

	new Float: Pos[4],
			   HP[2],
			   iString[180];

	GetPlayerPos(ToReplaceID, Pos[0], Pos[1], Pos[2]);
	GetPlayerFacingAngle(ToReplaceID, Pos[3]);

	HP[0] = GetHP(ToReplaceID);
	HP[1] = GetAP(ToReplaceID);

	new Weapons[13], Ammo[13];
	for (new i = 0; i < 13; i++) { GetPlayerWeaponData(ToReplaceID, i, Weapons[i], Ammo[i]); }

	SetPlayerRoundParticipation(ToAddID, true);
	Player[ToAddID][WasInBase] = true;

	Player[ToAddID][WasInTeam] = Player[ToAddID][Team];
	Player[ToAddID][Team]	   = Player[ToReplaceID][Team];
	SwitchTeamFix(ToAddID, false, false, true);

	SetSpawnInfo(ToAddID, Player[ToAddID][Team], Skin[Player[ToAddID][Team]], Pos[0], Pos[1], Pos[2] + 1, Pos[3], 0, 0, 0, 0, 0, 0);
	Player[ToAddID][IgnoreSpawn] = true;
	SpawnPlayer(ToAddID);

	SetHPAP(ToAddID, HP[0], HP[1]);

	SetPlayerPos(ToAddID, Pos[0], Pos[1], Pos[2] + 1);
	SetPlayerFacingAngle(ToAddID, Pos[3]);
	SetPlayerInterior(ToAddID, GetPlayerInterior(ToReplaceID));
	SetPlayerVirtualWorld(ToAddID, GetPlayerVirtualWorld(ToReplaceID));
	SetPlayerTeam(ToAddID, NO_TEAM);

	ColorFix(ToAddID);

	for (new i = 0; i < 13; i++) { GivePlayerWeapon(ToAddID, Weapons[i], Ammo[i]); }

	if (Player[ToReplaceID][WeaponPicked] > 0)
	{
		Player[ToAddID][WeaponPicked]	  = Player[ToReplaceID][WeaponPicked];
		Player[ToReplaceID][WeaponPicked] = 0;
	}

	RemovePlayerFromRound(ToReplaceID);

	if (GameType == BASE)
		SetCPForPlayer(ToAddID);

	CreatePlayerTeamBar(ToAddID);
	ShowTeamBarsForPlayer(ToAddID);
	UpdatePlayerTeamBar(ToAddID);

	ShowTeamCornerInfo(ToAddID);
	UpdateTeamCornerInfo();

	UpdateTeamPlayerCount(Player[ToAddID][Team], false);
	if (Player[ToAddID][Team] == ATTACKER || Player[ToAddID][Team] == DEFENDER)
		UpdateTeamHP(Player[ToAddID][Team]);

	if (RoundPaused)
	{
		TogglePlayerControllable(ToAddID, 0);
		ShowPauseTDForAll();
	}

	format(
		iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has replaced "COL_SEC"%s " COL_PRIM "by "COL_SEC"%s", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[ToReplaceID][Name],
		Player[ToAddID][Name]);
	SendClientMessageToAll(-1, iString);

	UpdateRoundKillDmgTDmg(ToAddID);

	RadarFix();
	return 1;
}

forward OnPlayerInGameReplace(ToAddID, i, playerid, addGeneration, issuerGeneration, requestGeneration, roundID);
public OnPlayerInGameReplace(ToAddID, i, playerid, addGeneration, issuerGeneration, requestGeneration, roundID)
{
	if (!IsPlayerConnected(ToAddID) || PlayerConnectionGeneration[ToAddID] != addGeneration
		|| !IsPlayerConnected(playerid) || PlayerConnectionGeneration[playerid] != issuerGeneration
		|| Current == -1 || Current != roundID || REPLACE_RequestRound[playerid] != roundID
		|| REPLACE_RequestGeneration[playerid] != requestGeneration || REPLACE_ToAddID[playerid] != ToAddID
		|| REPLACE_ToAddGeneration[playerid] != addGeneration
		|| i < 0 || i >= SAVE_SLOTS || Player[ToAddID][Playing]
		|| !SaveVariables[i][ToBeAdded] || SaveVariables[i][RoundID] != Current)
		return 0;

	PreparePlayerForReplacement(ToAddID);

	SetPlayerRoundParticipation(ToAddID, true);
	Player[ToAddID][WasInBase] = true;

	Player[ToAddID][WasInTeam] = Player[ToAddID][Team];
	Player[ToAddID][Team]	   = SaveVariables[i][pTeam];
	SwitchTeamFix(ToAddID, false, false, true);

	if (!SaveVariables[i][WasCrashedInStart])
	{
		SetSpawnInfo(
			ToAddID, Player[ToAddID][Team], Skin[Player[ToAddID][Team]], SaveVariables[i][pCoords][0], SaveVariables[i][pCoords][1], SaveVariables[i][pCoords][2] + 1,
			SaveVariables[i][pCoords][3], 0, 0, 0, 0, 0, 0);

		SetPlayerInterior(ToAddID, SaveVariables[i][pInterior]);
		SetPlayerVirtualWorld(ToAddID, SaveVariables[i][pVWorld]);
	}
	else if (Current != -1)
	{
		if (GameType == BASE)
		{
			switch (Player[ToAddID][Team])
			{
				case ATTACKER:
					SetSpawnInfo(
						ToAddID, Player[ToAddID][Team], Skin[Player[ToAddID][Team]], BAttackerSpawn[Current][0] + RandomMinMax(-3, 3), BAttackerSpawn[Current][1] + RandomMinMax(-3, 3),
						BAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
				case DEFENDER:
					SetSpawnInfo(
						ToAddID, Player[ToAddID][Team], Skin[Player[ToAddID][Team]], BDefenderSpawn[Current][0] + RandomMinMax(-3, 3), BDefenderSpawn[Current][1] + RandomMinMax(-3, 3),
						BDefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
			}
			SetPlayerInterior(ToAddID, BInterior[Current]);
		}
		else if (GameType == ARENA)
		{
			switch (Player[ToAddID][Team])
			{
				case ATTACKER:
					SetSpawnInfo(
						ToAddID, Player[ToAddID][Team], Skin[Player[ToAddID][Team]], AAttackerSpawn[Current][0] + RandomMinMax(-3, 3), AAttackerSpawn[Current][1] + RandomMinMax(-3, 3),
						AAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
				case DEFENDER:
					SetSpawnInfo(
						ToAddID, Player[ToAddID][Team], Skin[Player[ToAddID][Team]], ADefenderSpawn[Current][0] + RandomMinMax(-3, 3), ADefenderSpawn[Current][1] + RandomMinMax(-3, 3),
						ADefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
			}
			SetPlayerInterior(ToAddID, AInterior[Current]);
		}
		SetPlayerVirtualWorld(ToAddID, 2);
	}

	Player[ToAddID][IgnoreSpawn] = true;
	SpawnPlayer(ToAddID);

	SetHPAP(ToAddID, SaveVariables[i][gHealth], SaveVariables[i][gArmour]);

	SetPlayerTeam(ToAddID, NO_TEAM);

	ColorFix(ToAddID);
	SetCPForPlayer(ToAddID);

	ShowPlayerGunmenu(ToAddID, 500);

	CreatePlayerTeamBar(ToAddID);
	ShowTeamBarsForPlayer(ToAddID);

	ShowTeamCornerInfo(ToAddID);
	UpdateTeamCornerInfo();

	UpdateTeamPlayerCount(Player[ToAddID][Team], false);
	if (Player[ToAddID][Team] == ATTACKER || Player[ToAddID][Team] == DEFENDER)
	{
		UpdatePlayerTeamBar(ToAddID);
		UpdateTeamHP(Player[ToAddID][Team]);
	}

	SaveVariables[i][ToBeAdded] = false;

	new iString[160];

	if (RoundPaused)
	{
		TogglePlayerControllable(ToAddID, 0);
		ShowPauseTDForAll();
	}

	format(
		iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has replaced "COL_SEC"%s " COL_PRIM "by "COL_SEC"%s", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], SaveVariables[i][pName],
		Player[ToAddID][Name]);
	SendClientMessageToAll(-1, iString);

	UpdateRoundKillDmgTDmg(ToAddID);

	RadarFix();
	return 1;
}

RemovePlayerFromRound(playerid, bool: remove_iterator = true, bool: spawn_lobby = true)
{
	SetPlayerRoundParticipation(playerid, false, remove_iterator);

	if (Current != -1)
		CallLocalFunction("OnPlayerLeaveCheckpoint", "i", playerid);

	Player[playerid][WasInBase]	   = false;
	Player[playerid][WasInTeam]	   = NON;
	Player[playerid][ToAddInRound] = false;
	if (Current != -1)
	{
		UpdateAttackersZoneVisibility(playerid);
		if (GameType == BASE)
			UpdateCPZoneVisibility(playerid);
	}
	TogglePlayerControllable(playerid, 1);
	RemovePlayerMapIcon(playerid, 59);

	new remTeam = Player[playerid][Team];
	DeletePlayerTeamBar(playerid);
	HideAllTeamBarsForPlayer(playerid);
	if (remTeam == ATTACKER || remTeam == DEFENDER)
		ReorganizeTeamBars(remTeam);

	HideTeamCornerInfo(playerid);
	UpdateTeamCornerInfo();

	UpdateTeamPlayerCount(Player[playerid][Team], true, playerid);
	UpdateTeamHP(Player[playerid][Team], playerid);

	DisablePlayerCheckpoint(playerid);
	SetPlayerScore(playerid, 0);
	HidePlayerDialog(playerid);

	PlayerTextDrawHide(playerid, AreaCheckTD[playerid]);
	PlayerTextDrawHide(playerid, AreaCheckBG[playerid]);

	ResetPlayerGunmenu(playerid, false);
	PlayerNoLeadTeam(playerid);

	if (spawn_lobby)
		SpawnInLobby(playerid);
	return 1;
}

ResetPlayerRoundVariables(playerid, bool: resetTotals = false)
{
	if (resetTotals)
	{
		Player[playerid][TotalKills]		= 0;
		Player[playerid][TotalDeaths]		= 0;
		Player[playerid][TotalDamage]		= 0.0;
		Player[playerid][RoundPlayed]		= 0;
		Player[playerid][TotalBulletsFired] = 0;
		Player[playerid][TotalshotsHit]		= 0;
	}
	else
	{
		Player[playerid][RoundKills]  = 0;
		Player[playerid][RoundDeaths] = 0;
		Player[playerid][RoundAssists] = 0;
		Player[playerid][RoundDamage] = 0.0;
		Player[playerid][shotsHit]	  = 0;
	}
}

ResetRoundVariablesInSlots()
{
	for (new i = 0; i < SAVE_SLOTS; i++)
	{
		if (SaveVariables[i][pName][0])
		{
			SaveVariables[i][RKills]	 = 0;
			SaveVariables[i][RDeaths]	 = 0;
			SaveVariables[i][RAssists]	 = 0;
			SaveVariables[i][RDamage]	 = 0.0;
			SaveVariables[i][sWasInBase] = false;
		}
	}
	ResetAllAssistDamage();
}

SchedulePlayingPlayersGunmenu(delay)
{
	if (!SetTimerEx(
		"ShowPlayingPlayersGunmenu", delay, false, "ii",
		Current, RoundEndGeneration))
	{
		printf("[AttDef] Failed to schedule gunmenus for the current round.");
		return 0;
	}
	return 1;
}

forward ShowPlayingPlayersGunmenu(roundID, roundGeneration);
public ShowPlayingPlayersGunmenu(roundID, roundGeneration)
{
	if (Current != roundID || RoundEndGeneration != roundGeneration
		|| Current == -1 || AlreadyEndingRound)
		return 0;

	foreach (new i : PlayersInRound)
	{
		if (IsPlayerConnected(i) && Player[i][Playing])
			ShowPlayerGunmenu(i, 0);
	}
	return 1;
}

stock AnnounceTeamRoster(teamid)
{
	new names[144];
	new bool: found = false;
	foreach (new i : Player)
	{
		if (Player[i][Playing] && Player[i][Team] == teamid)
		{
			if (found)
				strcat(names, ", ", sizeof(names));
			strcat(names, Player[i][Name], sizeof(names));
			found = true;
		}
	}

	if (found)
	{
		va_SendClientMessageToAll(
			-1, "\xBB %s%s: " COL_SEC "%s",
			TextColor[teamid],
			teamid == ATTACKER ? "Attackers" : (teamid == DEFENDER ? "Defenders" : "Referees"),
			names
		);
	}
	else if (teamid != REFEREE)
	{
		va_SendClientMessageToAll(
			-1, "\xBB %sNo players in the %s team.",
			TextColor[teamid],
			teamid == ATTACKER ? "Attacker" : "Defender"
		);
	}
}
