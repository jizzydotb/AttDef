#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_spectate
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_spectate
	#endinput
#endif
#define _attdef_impl_spectate

/*
	Spectator mode
*/

#include <YSI_Coding\y_hooks>

hook OnPlayerDisconnect(playerid, reason)
{
	// Handle spectate when a player disconnects
	StopSpectate(playerid);
	HandleSpectatedPlayerDisconnect(playerid);
	return 1;
}

UpdateSpectateInfoForPlayer(spectatorid)
{
	if (spectatorid < 0 || spectatorid >= MAX_PLAYERS || !IsPlayerConnected(spectatorid)
		|| !Player[spectatorid][Spectating])
		return 0;

	new specid = Player[spectatorid][IsSpectatingID];
	if (specid < 0 || specid >= MAX_PLAYERS || !IsPlayerConnected(specid)
		|| PlayerSpectatedConnectionGeneration[spectatorid] != PlayerConnectionGeneration[specid])
		return 0;

	// Determine team color code for spectated player
	new teamColorCode[12];
	switch (Player[specid][Team])
	{
		case ATTACKER:
			teamColorCode = "~g~~h~";
		case ATTACKER_SUB:
			teamColorCode = "~g~~h~";
		case DEFENDER:
			teamColorCode = "~b~~h~";
		case DEFENDER_SUB:
			teamColorCode = "~b~~h~~h~";
		case REFEREE:
			teamColorCode = "~y~~h~";
		default:
			teamColorCode = "~w~";
	}

	// 0: Player Name (centered)
	new nameStr[MAX_PLAYER_NAME + 16];
	format(nameStr, sizeof(nameStr), "%s%s", teamColorCode, Player[specid][Name]);
	PlayerTextDrawSetString(spectatorid, SpecText[spectatorid][0], nameStr);

	// 1 & 2: Health active bar & percentage
	new hp = GetHP(specid);
	if (hp < 0) hp = 0;
	if (hp > 100) hp = 100;
	if (hp > 0)
	{
		new Float: hpSpan = (float(hp) / 100.0) * 56.787;
		PlayerTextDrawTextSize(spectatorid, SpecText[spectatorid][1], 39.140 + hpSpan, 930.000);
		PlayerTextDrawBoxColor(spectatorid, SpecText[spectatorid][1], GetTeamHealthBarColor(Player[specid][Team]));
		PlayerTextDrawShow(spectatorid, SpecText[spectatorid][1]);

		new hpValStr[12];
		format(hpValStr, sizeof(hpValStr), "%d", hp);
		PlayerTextDrawSetString(spectatorid, SpecText[spectatorid][2], hpValStr);
		PlayerTextDrawShow(spectatorid, SpecText[spectatorid][2]);
	}
	else
	{
		PlayerTextDrawHide(spectatorid, SpecText[spectatorid][1]);
		PlayerTextDrawHide(spectatorid, SpecText[spectatorid][2]);
	}

	// 3 & 4: Armour active bar & percentage
	new ap = GetAP(specid);
	if (ap < 0) ap = 0;
	if (ap > 100) ap = 100;
	if (ap > 0)
	{
		new Float: apSpan = (float(ap) / 100.0) * 56.787;
		PlayerTextDrawTextSize(spectatorid, SpecText[spectatorid][3], 39.140 + apSpan, 930.000);
		PlayerTextDrawShow(spectatorid, SpecText[spectatorid][3]);

		new apValStr[12];
		format(apValStr, sizeof(apValStr), "%d", ap);
		PlayerTextDrawSetString(spectatorid, SpecText[spectatorid][4], apValStr);
		PlayerTextDrawShow(spectatorid, SpecText[spectatorid][4]);
	}
	else
	{
		PlayerTextDrawHide(spectatorid, SpecText[spectatorid][3]);
		PlayerTextDrawHide(spectatorid, SpecText[spectatorid][4]);
	}

	// 5: Net stats (FPS - Ping - Packet)
	new netStr[96];
	format(
		netStr, sizeof(netStr),
		"%sFPS: ~w~%d %s- Ping: ~w~%d %s- Packet: ~w~%.2f%%",
		teamColorCode, GetPlayerFPS(specid), teamColorCode, GetPlayerPing(specid), teamColorCode, NetStats_PacketLossPercent(specid));
	PlayerTextDrawSetString(spectatorid, SpecText[spectatorid][5], netStr);

	// Round Header: "=- Round -="
	new roundHeaderStr[32];
	format(roundHeaderStr, sizeof(roundHeaderStr), "%s=- Round -=", teamColorCode);
	PlayerTextDrawSetString(spectatorid, Spec_RoundHeader[spectatorid], roundHeaderStr);
	PlayerTextDrawShow(spectatorid, Spec_RoundHeader[spectatorid]);

	// 6: Stats (Kills, Damage, Total, Weapons)
	new statsStr[160];
	format(
		statsStr, sizeof(statsStr),
		"%sKills: ~w~%d~n~%sDamage: ~w~%d~n~%sTotal: ~w~%d~n~%sWeapons: ~w~%s",
		teamColorCode, Player[specid][RoundKills], teamColorCode, floatround(Player[specid][RoundDamage]),
		teamColorCode, floatround(Player[specid][TotalDamage]), teamColorCode, ListWeapsSpec(specid));
	PlayerTextDrawSetString(spectatorid, SpecText[spectatorid][6], statsStr);
	return 1;
}

// This timer is called frequently to update textdraws for spectators so they can see correct info about who they're spectating
forward UpdateInfoForSpectators();
public UpdateInfoForSpectators()
{
	if (Iter_Count(AllSpectators) == 0)
		return 0;

	foreach (new i : AllSpectators)
	{
		if (IsPlayerConnected(i) && Player[i][Spectating])
			UpdateSpectateInfoForPlayer(i);
	}
	return 1;
}

// This initializes the system
InitSpectate()
{
	Iter_Init(PlayerSpectators);										 // empty the iterator array
	if (!SetTimer("UpdateInfoForSpectators", UPDATE_SPECINFO_INTERVAL, true))
	{
		printf("[AttDef] Failed to start the spectator information update timer.");
		return 0;
	}
	return 1;
}

// This will empty the iteration array that holds the IDs of the spectators of this player
// This will be used to initialize spectate mode for a player when they connect to the server
InitPlayerSpectate(playerid)
{
	Iter_Clear(PlayerSpectators[playerid]);
	Iter_Remove(AllSpectators, playerid);
	PlayerSpectatedConnectionGeneration[playerid] = 0;
}

HandleSpectatedPlayerDisconnect(playerid)
{
	if (!Player[playerid][BeingSpeced])
		return 0;

	foreach (new i : AllSpectators)
	{
		if (Player[i][IsSpectatingID] == playerid)
		{
			if (Player[i][Team] == REFEREE)
				SpectateAnyPlayer(i, false, true, playerid);
			else if (Current != -1)
				SpectateAnyPlayer(i, true, true, playerid);
			else
				SpectateAnyPlayer(i, false, true, playerid);
		}
	}
	return 1;
}

// Returns a list of the weapons which playerid got
ListWeapsSpec(playerid)
{
	new str[128], wep, ammo, count;
	for (new slt = 0; slt < 13; slt++)
	{
		GetPlayerWeaponData(playerid, slt, wep, ammo); // fetch the weapon data in this slot
		if (ammo > 0 && IsValidWeapon(wep))
		{
			if (count > 0)
				format(str, sizeof(str), "%s %s", str, ReturnWeaponName(wep));
			else
				format(str, sizeof(str), "%s", ReturnWeaponName(wep));
			count++;
		}
	}
	if (!count)
		format(str, sizeof(str), "None");
	return str;
}

// Add this player to the spectators iteration array
RememberSpectator(playerid)
{
	if (!Iter_Contains(AllSpectators, playerid))
		Iter_Add(AllSpectators, playerid);
}

// Remove this player from the spectators iteration array
// Warning: can't be used in a loop over AllSpectators
ForgetSpectator(playerid)
{
	if (Iter_Contains(AllSpectators, playerid))
		Iter_Remove(AllSpectators, playerid);
}

// Update/fix the textdraw that shows who are spectating you
UpdatePlayerSpectators(playerid)
{
	// Iter_Count(PlayerSpectators[playerid]); ==  how many are spectating this guy?
	if (Iter_Count(PlayerSpectators[playerid]) == 0) // if none are spectating
	{
		PlayerTextDrawSetString(playerid, WhoSpecHeader[playerid], " ");
		PlayerTextDrawSetString(playerid, WhoSpec[playerid], " ");
		Player[playerid][BeingSpeced] = false;
		return 0;
	}
	else
	{
		new str[SPECTATORS_STRING_SIZE];
		new totalAdded;

		new const specTeamColor[][] = {
			"~r~~h~", // ATTACKER
			"~b~~h~", // DEFENDER
			"~r~~h~"  // REFEREE / NON / OTHER
		};

		foreach (new i : PlayerSpectators[playerid]) // Loop through the array that contains this player's spectators
		{
			// Make sure we don't show more players than MAX_PLAYER_SPECTATORS. Otherwise string will go beyond 255th character and cause crashes.
			if (totalAdded >= MAX_PLAYER_SPECTATORS)
				break;

			new tIdx = 2;
			if (Player[i][Team] == ATTACKER) tIdx = 0;
			else if (Player[i][Team] == DEFENDER) tIdx = 1;

			// Format string with spectator name
			if (totalAdded == 0)
				format(str, sizeof str, "~w~- %s%s", specTeamColor[tIdx], Player[i][Name]);
			else
				format(str, sizeof str, "%s~n~~w~- %s%s", str, specTeamColor[tIdx], Player[i][Name]);

			totalAdded++;
		}
		foreach (new i : PlayerSpectators[playerid])
		{
			PlayerTextDrawSetString(i, WhoSpecHeader[i], "Spectators:");
			PlayerTextDrawSetString(i, WhoSpec[i], str);
			if (PlayerInterface[i][INTERFACE_SPEC])
			{
				PlayerTextDrawShow(i, WhoSpecHeader[i]);
				PlayerTextDrawShow(i, WhoSpec[i]);
			}
		}
		PlayerTextDrawSetString(playerid, WhoSpecHeader[playerid], "Spectating you:");
		PlayerTextDrawSetString(playerid, WhoSpec[playerid], str);
		if (PlayerInterface[playerid][INTERFACE_SPEC])
		{
			PlayerTextDrawShow(playerid, WhoSpecHeader[playerid]);
			PlayerTextDrawShow(playerid, WhoSpec[playerid]);
		}
		Player[playerid][BeingSpeced] = true;
	}
	return 1;
}

SpectatePlayer(playerid, specid)
{
	if (playerid == specid)
		return 0;

	if (!IsPlayerConnected(playerid) || !IsPlayerConnected(specid))
		return 0;

	if (TargetInfoData[playerid][TargetInfoTimer])
	{
		KillTimer(TargetInfoData[playerid][TargetInfoTimer]);
		HideTargetInfo(playerid, PlayerConnectionGeneration[playerid]); // hides the target info textdraws
	}
	new OldSpecID = -1;
	new OldSpecConnectionGeneration;
	if (Player[playerid][BeingSpeced]) // If anyone is spectating this player, we gotta turn off spectate for them
	{
		foreach (new i : AllSpectators)
		{
			if (Player[i][IsSpectatingID] == playerid)
			{
				Iter_Remove(AllSpectators, i);
				StopSpectate(i, false);
			}
		}
	}
	else if (Player[playerid][Spectating] && specid != Player[playerid][IsSpectatingID])
	{
		OldSpecID = Player[playerid][IsSpectatingID];
		OldSpecConnectionGeneration = PlayerSpectatedConnectionGeneration[playerid];
	}

	RememberSpectator(playerid); // Remember that this player is a spectator

	if (!Player[playerid][Spectating]) // If he's specing for the first time (i.e not from next/previous spectate functions)
	{
		// Spectation info textdraws
		TextDrawShowForPlayer(playerid, Spec_CardBG);
		TextDrawShowForPlayer(playerid, Spec_HealthLabel);
		TextDrawShowForPlayer(playerid, Spec_HealthBarBG);
		TextDrawShowForPlayer(playerid, Spec_ArmourLabel);
		TextDrawShowForPlayer(playerid, Spec_ArmourBarBG);
		PlayerTextDrawShow(playerid, Spec_RoundHeader[playerid]);
		PlayerTextDrawShow(playerid, SpecText[playerid][0]);
		PlayerTextDrawShow(playerid, SpecText[playerid][5]);
		PlayerTextDrawShow(playerid, SpecText[playerid][6]);

		// Hide round HUD, stats and damage textdraws
		HideRoundKillDmgTextDraws(playerid);
		HideRoundStats(playerid);
		HideTeamBarsForPlayer(playerid);
		HideTeamCornerInfo(playerid);
		TextDrawHideForPlayer(playerid, AttHpLose);
		TextDrawHideForPlayer(playerid, DefHpLose);
		TextDrawHideForPlayer(playerid, TeamHpLose[0]);
		TextDrawHideForPlayer(playerid, TeamHpLose[1]);
	}

	// Update variables
	Player[playerid][IsSpectatingID] = specid;
	Player[playerid][Spectating]	 = true;
	Player[specid][BeingSpeced]		 = true;
	PlayerSpectatedConnectionGeneration[playerid] = PlayerConnectionGeneration[specid];

	// Set up spectating mode
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(specid));
	SetPlayerInterior(playerid, GetPlayerInterior(specid));
	TogglePlayerSpectating(playerid, 1);
	if (GetPlayerVehicleID(specid))
		PlayerSpectateVehicle(playerid, GetPlayerVehicleID(specid));
	else
		PlayerSpectatePlayer(playerid, specid);

	if (OldSpecID >= 0 && OldSpecID < MAX_PLAYERS && IsPlayerConnected(OldSpecID)
		&& PlayerConnectionGeneration[OldSpecID] == OldSpecConnectionGeneration)
	{
		RadarFix(OldSpecID);
		Iter_Remove(PlayerSpectators[OldSpecID], playerid); // State that this player is no longer a spectator of 'OldSpecID'
		UpdatePlayerSpectators(OldSpecID);					// Update who-specing textdraws
	}
	Iter_Add(PlayerSpectators[specid], playerid); // State that this player is one of the spectators of 'specid'
	UpdatePlayerSpectators(specid);				  // Update who-specing

	// Update textdraws that show spectated player's info
	UpdateSpectateInfoForPlayer(playerid);

	RadarFix(playerid);
	RadarFix(specid);
	SpectatorRemovalGeneration[playerid]++;
	if (ShouldPlayerSeeTeamBars(playerid))
		ShowTeamBarsForPlayer(playerid);
	else
		HideAllTeamBarsForPlayer(playerid);
	return 1;
}

StopSpectate(playerid, bool: remove_iterator = true)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (!Player[playerid][Spectating])
		return 0;

	if (Player[playerid][SpectatingRound] != -1)
	{
		Player[playerid][SpectatingRound] = -1;
		Player[playerid][SpectatingType] = -1;
		PlayerTextDrawSetString(playerid, TD_RoundSpec[playerid], "_");
		ClearSpectatorState(playerid);
		DisablePlayerCheckpoint(playerid);
		DeleteArenaSpecGangzone(playerid);
		Player[playerid][Spectating] = false;
		SpawnInLobby(playerid);
		SetPlayerInterior(playerid, MainInterior);
		return 1;
	}
	if (Player[playerid][BeingSpeced])
	{
		foreach (new i : AllSpectators)
		{
			if (i == playerid) // Absence of this causes a stupid error on crashdetect "stack heap size". Endless loop?
				continue;

			if (Player[i][IsSpectatingID] == playerid)
			{
				Iter_Remove(AllSpectators, i);
				StopSpectate(i, false);
			}
		}
	}
	new specid					  = Player[playerid][IsSpectatingID];
	new specConnectionGeneration = PlayerSpectatedConnectionGeneration[playerid];

	// Reset variables
	if (remove_iterator)
		ForgetSpectator(playerid);

	Player[playerid][Spectating]	 = false;
	Player[playerid][SpectatingType] = -1;
	Player[playerid][IsSpectatingID] = -1;
	Player[playerid][BeingSpeced]	 = false;
	PlayerSpectatedConnectionGeneration[playerid] = 0;
	// Hide spec textdraws
	TextDrawHideForPlayer(playerid, Spec_CardBG);
	TextDrawHideForPlayer(playerid, Spec_HealthLabel);
	TextDrawHideForPlayer(playerid, Spec_HealthBarBG);
	TextDrawHideForPlayer(playerid, Spec_ArmourLabel);
	TextDrawHideForPlayer(playerid, Spec_ArmourBarBG);
	PlayerTextDrawHide(playerid, Spec_RoundHeader[playerid]);
	for (new i; i < 7; i++) PlayerTextDrawHide(playerid, SpecText[playerid][i]);
	// Hide team damage textdraws
	HideTeamBarsForPlayer(playerid);
	HideTeamCornerInfo(playerid);
	// Reset text
	PlayerTextDrawSetString(playerid, WhoSpecHeader[playerid], " ");
	PlayerTextDrawSetString(playerid, WhoSpec[playerid], " ");
	// Reshow HUD textdraws
	ShowRoundKillDmgTextDraws(playerid);
	if (PlayerInterface[playerid][INTERFACE_NET])
	{
		PlayerTextDrawShow(playerid, FPSPingPacketBox[playerid]);
		PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);
	}
	UpdateBaseIDTextDraw(playerid);
	UpdatePlayerHealthBar(playerid);

	// Cancel spectating mode
	// This will be called if he was spectating normally, not /freecam mode
	if (!Player[playerid][IgnoreSpawn])
		SpawnInLobby(playerid);
	else
	{
		// Freecam mode cancel
		new Float: camPos[3];
		GetPlayerCameraPos(playerid, camPos[0], camPos[1], camPos[2]);
		SetPlayerPos(playerid, camPos[0], camPos[1], camPos[2]);
		SwitchTeamFix(playerid, false, true);
	}
	SetPlayerInterior(playerid, MainInterior);
	SetPlayerVirtualWorld(playerid, 0);
	if (CPZone != INVALID_GANG_ZONE)
		GangZoneHideForPlayer(playerid, CPZone);
	DisablePlayerCheckpoint(playerid);
	DestroyFlareObjects(playerid);
	DestroyArenaFlareObjects(playerid);
	SetCameraBehindPlayer(playerid);

	if (specid >= 0 && specid < MAX_PLAYERS && IsPlayerConnected(specid)
		&& PlayerConnectionGeneration[specid] == specConnectionGeneration)
	{
		Iter_Remove(PlayerSpectators[specid], playerid); // State that this player is no longer a spectator of 'specid'
		UpdatePlayerSpectators(specid);
	}
	return 1;
}

SpectateNextTeamPlayer(playerid, bool: stop_spectate = false)
{
	// Calling Iter_Next inside loop comparison stage makes the starting index equal to the next player after spectator
	// Loop through all connected players ignoring players with IDs lower than spectator's
	for (new i = Player[playerid][IsSpectatingID]; (i = Iter_Next(Player, i)) != Iter_End(Player);)
	{
		if (Player[i][Team] == NON || GetPlayerState(i) == PLAYER_STATE_WASTED)
			continue; // Skip this player if they're dead/wasted or not spawned yet

		if (!Player[i][Playing])
			continue; // Skip this player if they're not playing

		if (Player[playerid][Team] != REFEREE && !IsTeamTheSame(Player[i][Team], Player[playerid][Team]))
			continue; // Skip this player if he's in a different team than spectator's team

		if (SpectatePlayer(playerid, i) == 1)
			return; // Stop the loop AS WELL AS the function process when the spectate process is a success
	}
	// This will be reached if no available players to be spectated are found
	if (stop_spectate)
	{
		StopSpectate(playerid); // Exit spectate mode
		return;
	}
	SpectateAnyPlayer(playerid, Player[playerid][Team] != REFEREE, true);
}

SpectatePreviousTeamPlayer(playerid, bool: stop_spectate = false)
{
	// Calling Iter_Next inside loop comparison stage makes the starting index equal to the previous player before spectator
	// Loop through all connected players (descending loop: from highest ID to lowest)
	for (new i = Player[playerid][IsSpectatingID]; (i = Iter_Prev(Player, i)) != Iter_Begin(Player);)
	{
		if (Player[i][Team] == NON || GetPlayerState(i) == PLAYER_STATE_WASTED)
			continue; // Skip this player if they're dead/wasted or not spawned yet

		if (!Player[i][Playing])
			continue; // Skip this player if they're not playing

		if (Player[playerid][Team] != REFEREE && !IsTeamTheSame(Player[i][Team], Player[playerid][Team]))
			continue; // Skip this player if he's in a different team than spectator's team

		if (SpectatePlayer(playerid, i) == 1)
			return; // Stop the loop AS WELL AS the function process when the spectate process is a success
	}
	// This will be reached if no available players to be spectated are found
	if (stop_spectate)
	{
		StopSpectate(playerid); // Exit spectate mode
		return;
	}
	SpectateAnyPlayer(playerid, Player[playerid][Team] != REFEREE, false);
}

SpectateNextPlayer(playerid, bool: stop_spectate = false)
{
	// Calling Iter_Next inside loop comparison stage makes the starting index equal to the next player after spectator
	// Loop through all connected players ignoring players with IDs lower than spectator's
	for (new i = Player[playerid][IsSpectatingID]; (i = Iter_Next(Player, i)) != Iter_End(Player);)
	{
		if (Player[i][Team] == NON || GetPlayerState(i) == PLAYER_STATE_WASTED)
			continue; // Skip this player if they're dead/wasted or not spawned yet

		if (Player[i][Spectating])
			continue; // Skip this player if they're spectating someone else

		if (SpectatePlayer(playerid, i) == 1)
			return; // Stop the loop AS WELL AS the function process when the spectate process is a success
	}
	// This will be reached if no available players to be spectated are found
	if (stop_spectate)
	{
		StopSpectate(playerid); // Exit spectate mode
		return;
	}
	SpectateAnyPlayer(playerid, false, true);
}

SpectatePreviousPlayer(playerid, bool: stop_spectate = false)
{
	// Calling Iter_Next inside loop comparison stage makes the starting index equal to the previous player before spectator
	// Loop through all connected players (descending loop: from highest ID to lowest)
	for (new i = Player[playerid][IsSpectatingID]; (i = Iter_Prev(Player, i)) != Iter_Begin(Player);)
	{
		if (Player[i][Team] == NON || GetPlayerState(i) == PLAYER_STATE_WASTED)
			continue; // Skip this player if they're dead/wasted or not spawned yet

		if (Player[i][Spectating])
			continue; // Skip this player if they're spectating someone else

		if (SpectatePlayer(playerid, i) == 1)
			return; // Stop the loop AS WELL AS the function process when the spectate process is a success
	}
	// This will be reached if no available players to be spectated are found
	if (stop_spectate)
	{
		StopSpectate(playerid); // Exit spectate mode
		return;
	}
	SpectateAnyPlayer(playerid, false, false);
}

// Called by timer from OnPlayerDeath
forward SpectateAnyPlayerT(playerid, connectionGeneration, deathGeneration);
public SpectateAnyPlayerT(playerid, connectionGeneration, deathGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration
		|| PlayerDeathFlowGeneration[playerid] != deathGeneration
		|| Current == -1)
		return 0;
	SpectateAnyPlayer(playerid, true, true);
	return 1;
}

ScheduleSpectatorRemoval(playerid)
{
	new removalGeneration = ++SpectatorRemovalGeneration[playerid];
	if (!SetTimerEx(
		"RemoveSpectatorSafely", 500, false, "iii",
		playerid, PlayerConnectionGeneration[playerid], removalGeneration))
	{
		printf("[AttDef] Failed to schedule spectator cleanup for player %d.", playerid);
		Iter_Remove(AllSpectators, playerid);
		return 0;
	}
	return 1;
}

forward RemoveSpectatorSafely(playerid, connectionGeneration, removalGeneration);
public RemoveSpectatorSafely(playerid, connectionGeneration, removalGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration
		|| SpectatorRemovalGeneration[playerid] != removalGeneration
		|| Player[playerid][Spectating])
		return 0;

	Iter_Remove(AllSpectators, playerid);
	return 1;
}

SpectateAnyPlayer(playerid, bool: teammatch, bool: ascending, exception = -1)
{
	if (teammatch)
	{
		if (ascending)
		{
			foreach (new i : PlayersInRound)
			{
				if (i == exception)
					continue;
				if (!IsTeamTheSame(Player[playerid][Team], Player[i][Team]))
					continue;
				if (SpectatePlayer(playerid, i) == 1)
					return; // Stop the loop AS WELL AS the function process when the spectate process is a success
			}
		}
		else
		{
			foreach (new i : Reverse(PlayersInRound))
			{
				if (i == exception)
					continue;
				if (!IsTeamTheSame(Player[playerid][Team], Player[i][Team]))
					continue;
				if (SpectatePlayer(playerid, i) == 1)
					return; // Stop the loop AS WELL AS the function process when the spectate process is a success
			}
		}
		// This will be reached if no available players to be spectated are found
		StopSpectate(playerid, false); // Exit spectate mode
		ScheduleSpectatorRemoval(playerid);
	}
	else if (ascending)
	{
		switch (Current)
		{
			case -1:
			{
				foreach (new i : Player)
				{
					if (i == exception)
						continue;
					if (Player[i][Team] == NON || GetPlayerState(i) == PLAYER_STATE_WASTED)
						continue; // Skip this player if they're dead/wasted or not spawned yet
					if (SpectatePlayer(playerid, i) == 1)
						return; // Stop the loop AS WELL AS the function process when the spectate process is a success
				}
			}
			default:
			{
				foreach (new i : PlayersInRound)
				{
					if (i == exception)
						continue;
					if (Player[i][Team] == NON || GetPlayerState(i) == PLAYER_STATE_WASTED)
						continue; // Skip this player if they're dead/wasted or not spawned yet
					if (SpectatePlayer(playerid, i) == 1)
						return; // Stop the loop AS WELL AS the function process when the spectate process is a success
				}
			}
		}
	}
	else
	{
		switch (Current)
		{
			case -1:
			{
				foreach (new i : Reverse(Player))
				{
					if (i == exception)
						continue;
					if (Player[i][Team] == NON || GetPlayerState(i) == PLAYER_STATE_WASTED)
						continue; // Skip this player if they're dead/wasted or not spawned yet
					if (SpectatePlayer(playerid, i) == 1)
						return; // Stop the loop AS WELL AS the function process when the spectate process is a success
				}
			}
			default:
			{
				foreach (new i : Reverse(PlayersInRound))
				{
					if (i == exception)
						continue;
					if (Player[i][Team] == NON || GetPlayerState(i) == PLAYER_STATE_WASTED)
						continue; // Skip this player if they're dead/wasted or not spawned yet
					if (SpectatePlayer(playerid, i) == 1)
						return; // Stop the loop AS WELL AS the function process when the spectate process is a success
				}
			}
		}
	}
	// This will be reached if no available players to be spectated are found
	StopSpectate(playerid, false); // Exit spectate mode
	ScheduleSpectatorRemoval(playerid);
}
