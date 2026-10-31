#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_match
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_match
	#endinput
#endif
#define _attdef_impl_match

/*
	Round/match related
*/

#define FLARE_COUNT 3
#define ARENA_START_COUNTDOWN (6)
#define ARENA_CAMERA_TICK_MS (33)
#define ARENA_CAMERA_ANGLE_STEP (1.0)

new Flare[MAX_PLAYERS][3];
new BackupRequestEpoch[MAX_PLAYERS];

ClearRoundStartCommandRestrictions()
{
	for (new playerid = 0; playerid < MAX_PLAYERS; playerid++)
	{
		AllowedViewingCommands[playerid] = false;
		BlackListViewingCommands[playerid] = false;
		NormalListViewingCommands[playerid] = false;
	}
	return 1;
}

#include "modules/match/checkpoint.inc"

hook OnPlayerDisconnect(playerid, reason)
{
	// If this player was in round (referees are spectators and do not affect the match)
	if (Player[playerid][Playing] && Player[playerid][Team] != REFEREE)
	{
		// Reset team leading status
		PlayerNoLeadTeam(playerid);

		// Store player data temporarily in case they come back to the game again
		StorePlayerVariables(playerid);

		// If configured to handle player disconnection in round
		switch (gAutoPauseMode)
		{
			case 0: // Do Nothing
			{
				// Nothing
			}
			case 1: // Send Message
			{
				va_SendClientMessageToAll(-1, COL_PRIM "\xBB " COL_SEC "%s " COL_PRIM "disconnected. (HP: %d - %d | Slot ID: %d). Player data saved!", Player[playerid][Name], Player[playerid][pHealth], Player[playerid][pArmour], playerid);
			}
			case 2: // Pause Round
			{
				new totalPlayersAlive = 0;

				// Recalculate the number of alive combatants in the round
				foreach (new i : PlayersInRound)
				{
					if (Player[i][Playing] && Player[i][Team] != REFEREE) // Count only alive combatants
					{
						totalPlayersAlive++;
					}
				}

				// Pause the round if there are more than one player alive
				if (totalPlayersAlive > 1)
				{
					if (!RoundPaused && !RoundUnpausing)
					{
						KillTimer(UnpauseTimer);
						RoundUnpausing = false;
						PauseRound();
						va_SendClientMessageToAll(-1, COL_PRIM "\xBB Round has been paused because " COL_SEC "%s " COL_PRIM "disconnected.", Player[playerid][Name]);
					}
				}
			}
		}
	}
	else
		StorePlayerVariablesMin(playerid);

	// Resets backup request (player can be not in the round anymore and timer was not completed either)
	if (Player[playerid][AskingForHelp])
		EndTeamBackupRequest(playerid);

	// Handle match
	SetPlayerRoundParticipation(playerid, false);
	UpdateTeamPlayerCount(Player[playerid][Team], true, playerid);
	UpdateTeamHP(Player[playerid][Team], playerid);
	// Call OnPlayerLeaveCheckpoint to see if this player was in the checkpoint and fix issues
	if (Current != -1)
		CallLocalFunction("OnPlayerLeaveCheckpoint", "i", playerid);
	return 1;
}

hook OnGameModeInit()
{
	for (new i = 0; i < MAX_TEAMS; ++i) { TeamDeathIconTimer[i] = 0; }
	return 1;
}

#include "modules/match/cameras.inc"

forward StartRoundAutomatically(generation);
public StartRoundAutomatically(generation)
{
	if (generation != AutoRoundStartGeneration)
		return 0;

	AutoRoundStartTimer = 0;
	if (!AutoRoundStarter || Current != -1)
		return 0;

	if (!AllowStartBase)
	{
		ScheduleAutoRoundStart(5);
		return 0;
	}

	new ct[2] = { 0, 0 };
	new totalEligible = 0;
	foreach (new i : Player)
	{
		if (Player[i][InDuel])
			continue;

		if (CanPlay(i))
		{
			totalEligible++;
			switch (Player[i][Team])
			{
				case ATTACKER, ATTACKER_SUB: ct[0]++;
				case DEFENDER, DEFENDER_SUB: ct[1]++;
			}
		}
	}
	if (totalEligible < 2)
	{
		SendClientMessageToAll(-1, COL_PRIM "\xBB Auto round starter error: "COL_SEC"Not enough players to start a round (minimum 2 players required)!");
		SendClientMessageToAll(-1, COL_PRIM"\xBB System will try to start a round again in "COL_SEC"60 seconds!");
		ScheduleAutoRoundStart(60);
		return 0;
	}

	// Never rebalance during a match: teams only swap sides between rounds
	if (!WarMode)
	{
		BalanceTeams();
		SendClientMessageToAll(-1, ""COL_THR"\xBB System has balanced the teams!");
	}
	else if (ct[0] < 1 || ct[1] < 1)
	{
		SendClientMessageToAll(-1, COL_PRIM "\xBB Auto round starter error: "COL_SEC"Both teams must have at least 1 player during match mode!");
		SendClientMessageToAll(-1, COL_PRIM"\xBB System will try to start a round again in "COL_SEC"60 seconds!");
		ScheduleAutoRoundStart(60);
		return 0;
	}

	new roundType = random(2) == 0 ? ARENA : BASE;
	new id = DetermineRandomRound(0, true, roundType);
	if (id < 0)
	{
		roundType = roundType == ARENA ? BASE : ARENA;
		id = DetermineRandomRound(0, true, roundType);
	}
	if (id < 0)
	{
		AllowStartBase = true;
		SendClientMessageToAll(-1, COL_PRIM "\xBB System couldn't start a round because there are no valid bases or arenas in the selected packs.");
		return 0;
	}

	AllowStartBase = false;
	foreach (new i : Player)
	{
		if (CanPlay(i))
		{
			Player[i][PauseCount] = 0;
			Player[i][gPauseWarnings] = 0;
			Player[i][gWasPaused] = 0;
			Player[i][gPauseCheck] = false;
			TogglePlayerControllable(i, 0);
			Player[i][ToAddInRound] = true;
		}
	}

	new str[144];
	if (roundType == ARENA)
	{
		ScheduleRoundStart(ARENA, id, 2000);
		format(str, sizeof(str), COL_PRIM "\xBB System has randomly started Arena "COL_SEC"'%s' (ID: %d)", AName[id], id);
	}
	else
	{
		ScheduleRoundStart(BASE, id, 2000);
		format(str, sizeof(str), COL_PRIM "\xBB System has randomly started Base "COL_SEC"'%s' (ID: %d)", BName[id], id);
	}
	SendClientMessageToAll(-1, str);
	return 1;
}

ScheduleAutoRoundStart(in_seconds)
{
	CancelAutoRoundStart();
	AutoRoundStartTimer = SetTimerEx(
		"StartRoundAutomatically", in_seconds * 1000, false, "i", AutoRoundStartGeneration);
	if (!AutoRoundStartTimer)
	{
		printf("[AttDef] Failed to schedule automatic round start.");
		SendClientMessageToAll(-1, COL_PRIM "\xBB Automatic round start could not be scheduled. Start a round manually.");
		return 0;
	}
	return AutoRoundStartTimer;
}

CancelAutoRoundStart()
{
	if (AutoRoundStartTimer)
	{
		KillTimer(AutoRoundStartTimer);
		AutoRoundStartTimer = 0;
	}

	AutoRoundStartGeneration++;
	if (AutoRoundStartGeneration == 0)
		AutoRoundStartGeneration++;
	return 1;
}

SetRoundAutoStart(in_seconds)
{
	if (!AutoRoundStarter)
		return 0;

	if (in_seconds > 0)
	{
		if (ScheduleAutoRoundStart(in_seconds))
			SendClientMessageToAll(-1, va_return(""COL_PRIM"\xBB System is starting another round in "COL_SEC"%d seconds!", in_seconds));
	}
	else
	{
		CancelAutoRoundStart();
		StartRoundAutomatically(AutoRoundStartGeneration);
	}
	return 1;
}

AbortPendingRoundStart()
{
	AllowStartBase = true;
	foreach (new i : Player)
	{
		if (Player[i][ToAddInRound])
		{
			Player[i][ToAddInRound] = false;
			TogglePlayerControllable(i, 1);
		}
	}
	SendClientMessageToAll(-1, COL_PRIM "\xBB Round start cancelled because the selected map is no longer valid.");
	return 0;
}

// Set default name for all teams
SetTeamDefaultNames()
{
	TeamName[ATTACKER]	   = "Alpha";
	TeamName[DEFENDER]	   = "Beta";
	TeamName[REFEREE]	   = "Referee";
	TeamName[ATTACKER_SUB] = "Alpha Sub";
	TeamName[DEFENDER_SUB] = "Beta Sub";
	return 1;
}

// Basically checks if a team lacks players, the other team should win
CheckTeamElimination()
{
	// If round is paused or there's no active round at all, we should stop the process
	if (RoundPaused || Current == -1 || AllVsAllMode)
		return -1;

	// Reset teams player count to 0
	PlayersAlive[ATTACKER] = 0;
	PlayersAlive[DEFENDER] = 0;
	// Loop through all players who are currently in round
	foreach (new i : PlayersInRound)
	{
		switch (Player[i][Team]) // increase team player count
		{
			case ATTACKER: PlayersAlive[ATTACKER]++;
			case DEFENDER: PlayersAlive[DEFENDER]++;
		}
	}

#if !defined ROUND_NEVER_END
	// If both teams have no players
	if (PlayersAlive[ATTACKER] == 0 && PlayersAlive[DEFENDER] == 0)
	{
		EndRound(4); // Nobody wins
		return 0;
	}
	// If attacker team has no players, defenders win
	else if (PlayersAlive[ATTACKER] == 0)
	{
		EndRound(2); // Defenders Win
		return 0;
	}
	// If defender team has no players, attackers win
	else if (PlayersAlive[DEFENDER] == 0)
	{
		EndRound(3); // Attackers Win
		return 0;
	}
#endif
	return 1;
}

/*
Summary:
Corrects the player count for a team and update textdraws then check for elimination if asked

Parameters:
teamid: the team to check
elimination_check: a bool to check for elimination
exception: the id of the player who we should ignore while looping and checking. -1 to disable
*/
UpdateTeamPlayerCount(teamid, bool: elimination_check, exception = -1)
{
	// If there's no active round, we should stop the process
	if (Current == -1 || !ArenaStarted && !BaseStarted)
		return;

	// Reset team player count variable
	PlayersAlive[teamid] = 0;
	PlayersDead[teamid] = 0;
	foreach (new i : Player)
	{
		if (i == exception)
			continue;
		if (Player[i][WasInBase] && Player[i][WasInTeam] == teamid)
		{
			if (Player[i][Playing])
				PlayersAlive[teamid]++;
			else
				PlayersDead[teamid]++;
		}
	}
	if (exception >= 0 && exception < MAX_PLAYERS && IsPlayerConnected(exception))
	{
		if (Player[exception][WasInBase] && Player[exception][WasInTeam] == teamid && !Player[exception][Playing])
		{
			PlayersDead[teamid]++;
		}
	}

	if (!AllVsAllMode && (teamid == ATTACKER || teamid == DEFENDER))
	{
		new enemyTeam = (teamid == ATTACKER) ? DEFENDER : ATTACKER;
		if (PlayersAlive[teamid] == 1)
		{
			new survivor = INVALID_PLAYER_ID;
			foreach (new i : PlayersInRound)
			{
				if (i != exception && Player[i][Team] == teamid)
				{
					survivor = i;
					break;
				}
			}
			if (survivor != INVALID_PLAYER_ID && PlayersAlive[enemyTeam] >= 2 && RoundClutchEnemies[teamid] < PlayersAlive[enemyTeam])
			{
				RoundClutchPlayer[teamid] = survivor;
				RoundClutchEnemies[teamid] = PlayersAlive[enemyTeam];
			}
		}
		else if (PlayersAlive[teamid] > 1)
		{
			RoundClutchPlayer[teamid] = INVALID_PLAYER_ID;
			RoundClutchEnemies[teamid] = 0;
		}
	}

	if (!AllVsAllMode)
	{
		// Update round textdraws
		new str[256];
		switch (teamid)
		{
			case ATTACKER:
			{
				format(str, sizeof str, "~w~%d__(%d)", PlayersAlive[ATTACKER], TeamHP[ATTACKER]);
				TextDrawSetString(leftTeamData, str);
				format(str, sizeof str, "%s%d ~w~[%d]", TeamDamageFlashing[ATTACKER] ? "~g~~h~~h~" : TDC[ATTACKER], TeamHP[ATTACKER], PlayersAlive[ATTACKER]);
				TextDrawSetString(ModernRound_AttHP, str);
				TextDrawSetString(SimpleRound_AttHP, str);
			}
			case DEFENDER:
			{
				format(str, sizeof str, "~w~(%d)__%d", TeamHP[DEFENDER], PlayersAlive[DEFENDER]);
				TextDrawSetString(rightTeamData, str);
				format(str, sizeof str, "~w~[%d] %s%d", PlayersAlive[DEFENDER], TeamDamageFlashing[DEFENDER] ? "~b~~h~~h~~h~" : TDC[DEFENDER], TeamHP[DEFENDER]);
				TextDrawSetString(ModernRound_DefHP, str);
				TextDrawSetString(SimpleRound_DefHP, str);
			}
		}

		// Update attdef style round textdraws
		format(
			str, sizeof(str),
			"~g~~h~%s  ~g~~h~~h~%d   ~w~(~g~~h~~h~%d~w~)			   	                    			   	            ~b~~h~%s  ~b~~h~~h~%d   ~w~(~b~~h~~h~%d~w~)~n~",
			TeamName[ATTACKER], PlayersAlive[ATTACKER], TeamHP[ATTACKER], TeamName[DEFENDER], PlayersAlive[DEFENDER], TeamHP[DEFENDER]);
		TextDrawSetString(AttDef_RoundStats, str);
	}
	UpdateRoundTeamStatusTextDraws();
	UpdateRoundTeamStatusHUD();

// If we're asked to check team/round elimination
#if !defined ROUND_NEVER_END
	if (elimination_check && !AllVsAllMode)
	{
		// If round is not paused
		if (!RoundPaused)
		{
			// Handle lack of players (or elimination)
			if (PlayersAlive[ATTACKER] == 0)
				EndRound(2); // Defenders Win
			else if (PlayersAlive[DEFENDER] == 0)
				EndRound(3); // Attackers Win
		}
	}
#endif
}

ResetRoundTeamHPMaximums()
{
	TeamDamageFlashing[ATTACKER] = false;
	TeamDamageFlashing[DEFENDER] = false;
	TeamHPMax[ATTACKER] = 0;
	TeamHPMax[DEFENDER] = 0;
	RoundClutchPlayer[ATTACKER] = INVALID_PLAYER_ID;
	RoundClutchPlayer[DEFENDER] = INVALID_PLAYER_ID;
	RoundClutchEnemies[ATTACKER] = 0;
	RoundClutchEnemies[DEFENDER] = 0;
}

// This updates round textdraws with correct HP info for each team
// teamid: team to check
// exception: the id of the player who we should ignore while looping and checking. -1 to disable
UpdateTeamHP(teamid, exception = -1)
{
	// If there's no active round, this call is a waste
	if (Current == -1 || !ArenaStarted && !BaseStarted)
		return;

	if (AllVsAllMode)
		return;

	// Rebuild team HP and its capacity from the current round roster.
	TeamHP[teamid] = 0;
	TeamHPMax[teamid] = 0;
	foreach (new i : PlayersInRound)
	{
		if (i == exception)
			continue;

		// If he isn't in the specified team
		if (Player[i][Team] != teamid)
			continue; // Skip this index

		TeamHPMax[teamid] += RoundHP + RoundAR;
		TeamHP[teamid] += GetHP(i) + GetAP(i);
	}
	if (TeamHP[teamid] > TeamHPMax[teamid])
		TeamHPMax[teamid] = TeamHP[teamid];
	UpdateRoundTeamStatusHUD();
	// Update round textdraws with correct info
	new str[256];
	switch (teamid)
	{
		case ATTACKER:
		{
			format(str, sizeof str, "~w~%d__(%d)", PlayersAlive[ATTACKER], TeamHP[ATTACKER]);
			TextDrawSetString(leftTeamData, str);
			format(str, sizeof str, "%s%d ~w~[%d]", TeamDamageFlashing[ATTACKER] ? "~g~~h~~h~" : TDC[ATTACKER], TeamHP[ATTACKER], PlayersAlive[ATTACKER]);
			TextDrawSetString(ModernRound_AttHP, str);
			TextDrawSetString(SimpleRound_AttHP, str);
		}
		case DEFENDER:
		{
			format(str, sizeof str, "~w~(%d)__%d", TeamHP[DEFENDER], PlayersAlive[DEFENDER]);
			TextDrawSetString(rightTeamData, str);
			format(str, sizeof str, "~w~[%d] %s%d", PlayersAlive[DEFENDER], TeamDamageFlashing[DEFENDER] ? "~b~~h~~h~~h~" : TDC[DEFENDER], TeamHP[DEFENDER]);
			TextDrawSetString(ModernRound_DefHP, str);
			TextDrawSetString(SimpleRound_DefHP, str);
		}
	}

	// Update attdef style round textdraws
	format(
		str, sizeof(str),
		"~r~~h~%s  ~r~~h~~h~%d   ~w~(~r~~h~~h~%d~w~)			   	                    			   	            ~b~~h~%s  ~b~~h~~h~%d   ~w~(~b~~h~~h~%d~w~)~n~",
		TeamName[ATTACKER], PlayersAlive[ATTACKER], TeamHP[ATTACKER], TeamName[DEFENDER], PlayersAlive[DEFENDER], TeamHP[DEFENDER]);
	TextDrawSetString(AttDef_RoundStats, str);
}

// This is a handler for any game running (base, arena, etc...)
FinishAllVsAll()
{
	new winnerid = INVALID_PLAYER_ID;
	new highestKills = -1;
	new bool:tie = false;
	foreach (new i : PlayersInRound)
	{
		if (Player[i][RoundKills] > highestKills)
		{
			highestKills = Player[i][RoundKills];
			winnerid = i;
			tie = false;
		}
		else if (Player[i][RoundKills] == highestKills)
			tie = true;
	}

	if (winnerid == INVALID_PLAYER_ID || tie)
		SendClientMessageToAll(-1, "{FFFFFF}\xBB All-vs-All ended in a tie.");
	else
		SendClientMessageToAll(-1, va_return("{FFFFFF}\xBB All-vs-All winner: "COL_PRIM"%s {FFFFFF}with "COL_PRIM"%d {FFFFFF}kills.", Player[winnerid][Name], highestKills));
	return EndRound(4);
}

AllVsAllGameLoop()
{
	if (!ArenaStarted || RoundPaused)
		return 1;

	new contestants = 0;
	foreach (new i : PlayersInRound)
	{
		if (!IsPlayerConnected(i) || !Player[i][Playing])
			continue;

		contestants++;
		if (Player[i][NetCheck] && !Player[i][TempNetcheck])
			CheckPlayerNet(i);

		if (!IsPlayerInArea(i, AMin[Current][0], AMax[Current][0], AMin[Current][1], AMax[Current][1]))
		{
			Player[i][OutOfArena]--;
			PlayerTextDrawSetString(i, AreaCheckTD[i], va_return("Stay in Arena ~r~~h~%d~w~", Player[i][OutOfArena]));
			PlayerTextDrawShow(i, AreaCheckTD[i]);
			PlayerTextDrawShow(i, AreaCheckBG[i]);
			if (Player[i][OutOfArena] == 0)
			{
				Player[i][OutOfArena] = MAX_ZONE_LIMIT_WARNINGS;
				RemovePlayerFromRound(i);
				contestants--;
			}
		}
		else if (Player[i][OutOfArena] != MAX_ZONE_LIMIT_WARNINGS)
		{
			Player[i][OutOfArena] = MAX_ZONE_LIMIT_WARNINGS;
			PlayerTextDrawHide(i, AreaCheckTD[i]);
			PlayerTextDrawHide(i, AreaCheckBG[i]);
		}
	}
	if (!contestants)
	{
		SendClientMessageToAll(-1, "{FF6666}All-vs-All ended because no participants remain.");
		return EndRound(4);
	}

	RoundSeconds--;
	if (RoundSeconds <= 0)
		return FinishAllVsAll();

	ElapsedTime++;
	new str[16];
	format(str, sizeof(str), "~w~%d:%02d", RoundSeconds / 60, RoundSeconds % 60);
	TextDrawSetString(timerCenterTD, str);
	TextDrawSetString(AttDef_RoundTimer, str);
	TextDrawSetString(ModernRound_Timer, str);
	TextDrawSetString(SimpleRound_Timer, str);
	UpdateRoundTeamStatusTimer(str);
	UpdateAllVsAllTextDraws();
	return 1;
}

forward GameLoopCallback();
public GameLoopCallback()
{
	if (Current == -1)
	{
		// If there's no active round, we kill this timer so the loop isn't called again
		KillGameLoop();
		return 0;
	}
	switch (GameType)
	{
		case BASE:
		{
			foreach (new i : PlayersInRound) // Loop through all players in round
			{
				// Plays sound when CP is being taken
				if (PlayersInCP > 0 && !RoundPaused)
					PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
				// Net check
				if (Player[i][NetCheck] && !Player[i][TempNetcheck])
					CheckPlayerNet(i);
			}
			if (!BaseStarted)
				return 1;
			if (RoundPaused)
				return 1;

			// Handle round time
			RoundSeconds--;
			if (RoundSeconds <= 0)
				return EndRound(1); // Defenders Win
			// Increase elapsed time
			ElapsedTime++;
			// Draw round time (and CP time if it's being taken)
			switch (PlayersInCP)
			{
				case 0:
				{
					new str[16];
					format(str, sizeof(str), "~w~%d:%02d", RoundSeconds / 60, RoundSeconds % 60);
					TextDrawSetString(timerCenterTD, str);
					TextDrawSetString(AttDef_RoundTimer, str);
					TextDrawSetString(ModernRound_Timer, str);
					TextDrawSetString(SimpleRound_Timer, str);
					UpdateRoundTeamStatusTimer(str);
					UpdateModernAndSimpleCheckpointHUD();
				}
				default:
				{
					new str[32];
					CurrentCPTime--;
					format(str, sizeof(str), "~w~%d:%02d / %s%d", RoundSeconds / 60, RoundSeconds % 60, TDC[ATTACKER], CurrentCPTime);
					TextDrawSetString(timerCenterTD, str);
					TextDrawSetString(AttDef_RoundTimer, str);
					TextDrawSetString(ModernRound_Timer, str);
					TextDrawSetString(SimpleRound_Timer, str);
					new roundTime[16];
					format(roundTime, sizeof(roundTime), "~w~%d:%02d", RoundSeconds / 60, RoundSeconds % 60);
					UpdateRoundTeamStatusTimer(roundTime);

					UpdateModernAndSimpleCheckpointHUD();

					if (CurrentCPTime == 0)
						return EndRound(0); // Attackers Win

					if (CurrentCPTime <= 10)
					{
						if (CurrentCPTime % 2 == 0 && ScreenFlashing)
							TextDrawShowForAll(CheckpointWarningBox);
						else
							TextDrawHideForAll(CheckpointWarningBox);
					}
				}
			}
		}
		case ARENA:
		{
			if (AllVsAllMode)
				return AllVsAllGameLoop();

			foreach (new i : PlayersInRound) // Loop through all players in round
			{
				// Plays sound when CP is being taken
				if (PlayersInCP > 0 && !RoundPaused)
					PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
				// Net check
				if (Player[i][NetCheck] && !Player[i][TempNetcheck])
					CheckPlayerNet(i);
				// Perform zone limit check if arena is in progress
				if (!ArenaStarted)
					continue;
				if (!RoundPaused && !IsPlayerInArea(i, AMin[Current][0], AMax[Current][0], AMin[Current][1], AMax[Current][1]))
				{
					Player[i][OutOfArena]--;

					PlayerTextDrawSetString(i, AreaCheckTD[i], va_return("Stay in Arena ~r~~h~%d~w~", Player[i][OutOfArena]));
					PlayerTextDrawShow(i, AreaCheckTD[i]);
					PlayerTextDrawShow(i, AreaCheckBG[i]);

					if (Player[i][OutOfArena] == 0)
					{
						va_SendClientMessageToAll(
							-1,
							"{FFFFFF}%s " COL_PRIM "has been removed for not staying in Arena. {FFFFFF}(%d | %d)", Player[i][Name], GetHP(i), GetAP(i));

						Player[i][OutOfArena] = MAX_ZONE_LIMIT_WARNINGS;

						PlayerTextDrawHide(i, AreaCheckTD[i]);
						PlayerTextDrawHide(i, AreaCheckBG[i]);
						RemovePlayerFromRound(i);
						break;
					}
				}
				else if (Player[i][OutOfArena] != MAX_ZONE_LIMIT_WARNINGS)
				{
					Player[i][OutOfArena] = MAX_ZONE_LIMIT_WARNINGS;
					PlayerTextDrawHide(i, AreaCheckTD[i]);
					PlayerTextDrawHide(i, AreaCheckBG[i]);
				}
			}
			if (!ArenaStarted)
				return 1;
			if (RoundPaused)
				return 1;
			RoundSeconds--;
			if (RoundSeconds <= 0)
			{
				if (TeamHP[ATTACKER] < TeamHP[DEFENDER])
					EndRound(2);
				else if (TeamHP[DEFENDER] < TeamHP[ATTACKER])
					EndRound(3);
				else if (floatround(TeamHP[ATTACKER]) == floatround(TeamHP[DEFENDER]))
					EndRound(4); // No one win
				return 1;
			}
			ElapsedTime++;
			// Draw round time (and CP time if it's being taken)
			switch (PlayersInCP)
			{
				case 0:
				{
					new str[16];
					format(str, sizeof(str), "~w~%d:%02d", RoundSeconds / 60, RoundSeconds % 60);
					TextDrawSetString(timerCenterTD, str);
					TextDrawSetString(AttDef_RoundTimer, str);
					TextDrawSetString(ModernRound_Timer, str);
					TextDrawSetString(SimpleRound_Timer, str);
					new roundTime[16];
					format(roundTime, sizeof(roundTime), "~w~%d:%02d", RoundSeconds / 60, RoundSeconds % 60);
					UpdateRoundTeamStatusTimer(roundTime);
					UpdateModernAndSimpleCheckpointHUD();
				}
				default:
				{
					CurrentCPTime--;
					new str[32];
					format(str, sizeof(str), "~w~%d:%02d / %s%d", RoundSeconds / 60, RoundSeconds % 60, TDC[TeamCapturingCP], CurrentCPTime);
					TextDrawSetString(timerCenterTD, str);
					TextDrawSetString(AttDef_RoundTimer, str);
					TextDrawSetString(ModernRound_Timer, str);
					TextDrawSetString(SimpleRound_Timer, str);
					new roundTime[16];
					format(roundTime, sizeof(roundTime), "~w~%d:%02d", RoundSeconds / 60, RoundSeconds % 60);
					UpdateRoundTeamStatusTimer(roundTime);

					UpdateModernAndSimpleCheckpointHUD();

					if (CurrentCPTime == 0)
					{
						switch (TeamCapturingCP)
						{
							case ATTACKER: return EndRound(5);
							case DEFENDER: return EndRound(6);
							default: return EndRound(4);
						}
					}
				}
			}
		}
	}
	UpdateRoundTeamStatusMessages();
	return 1;
}

StartGameLoop()
{
	if (GameLoop)
		return 1;
	GameLoop = SetTimer("GameLoopCallback", GAME_SPEED, true);
	if (!GameLoop)
	{
		printf("[AttDef] Failed to start the active round game loop.");
		return 0;
	}
	return 1;
}

KillGameLoop()
{
	if (!GameLoop)
		return;
	KillTimer(GameLoop);
	GameLoop = 0;
}

RestartGameLoop()
{
	if (Current == -1)
		return 0;
	KillGameLoop();
	if (StartGameLoop())
		return 1;

	SendClientMessageToAll(-1, COL_PRIM "\xBB The round was stopped because its game timer could not be started.");
	EndRound(4);
	return 0;
}

// Delete arenas that are already played from memory (so they can be started again from random arena command)
ForgetPlayedArenas()
{
	foreach (new i : CurrentArenas) IsArenaPlayed[i] = false;
}

// Delete bases that are already played from memory (so they can be started again from random base command)
ForgetPlayedBases()
{
	foreach (new i : CurrentBases) IsBasePlayed[i] = false;
}

/*
	DOES: Generate a random ID to start a round

	PARAMETERS:
	mode: 0 for int and ext, 1 interior only, 2 exterior only
	ignore: false to skip already played bases, true to ignore that
	type: ARENA for arena, BASE for base, ...

	RETURN:
	Random ID of mode to start

	NOTE:
	If there are no maps created, returns -1.
	If there are maps created but no criteria met, returns -2.
*/
DetermineRandomRound(mode = 0, bool: ignore = false, type = ARENA)
{
	switch (type)
	{
		case ARENA:
		{
			if (Iter_IsEmpty(CurrentArenas))
				return -1;

			new timesSearched,			 // How many times we searched for a map
				id,						 // id of the map to start
				completeLoops;			 // lets avoid an infinite loop if a base/arena set doesn't meet the criteria
			new bool: extOrIntAllowed;	 // either an int or ext was requested

			// Will loop until a map is found valid to be played.
			for (timesSearched = 1;; timesSearched++)
			{
				id = Iter_Random(CurrentArenas); // Generate a random ID
				extOrIntAllowed = mode == 0 ? true : mode == 1 ? AInterior[id] != 0 : AInterior[id] == 0;
				if (extOrIntAllowed && (ignore || !IsArenaPlayed[id]))
				{
					// Mark it as played
					IsArenaPlayed[id] = true;
					return id;
				}
				// if all the maps are already played
				if (timesSearched >= Iter_Count(CurrentArenas))
				{
					// We clear them and restart search
					timesSearched = 0;
					if (++completeLoops == 2)
						return -2;
					ForgetPlayedArenas();
				}
			}
		}
		case BASE:
		{
			if (Iter_IsEmpty(CurrentBases))
				return -1;

			new timesSearched,			 // How many times we searched for a map
				id,						 // id of the map to start
				completeLoops;			 // lets avoid an infinite loop if a base/arena set doesn't meet the criteria
			new bool: extOrIntAllowed;	 // either an int or ext was requested

			// Will loop until a map is found valid to be played.
			for (timesSearched = 1;; timesSearched++)
			{
				id = Iter_Random(CurrentBases); // Generate a random ID
				extOrIntAllowed = mode == 0 ? true : mode == 1 ? BInterior[id] != 0 : BInterior[id] == 0;
				if (extOrIntAllowed && (ignore || !IsBasePlayed[id]))
				{
					// Mark it as played
					IsBasePlayed[id] = true;
					return id;
				}
				// if all the maps are already played
				if (timesSearched >= Iter_Count(CurrentBases))
				{
					// We clear them and restart search
					timesSearched = 0;
					if (++completeLoops == 2)
						return -2;
					ForgetPlayedBases();
				}
			}
		}
	}
	return -1;
}

#include "modules/match/pause.inc"

PlayerLeadTeam(playerid, bool: force, bool: message = true)
{
	new team = Player[playerid][Team];

	if (!force && TeamHasLeader[team])
		return 0;

	TeamLeader[team]	= playerid;
	TeamHasLeader[team] = true;
	if (message)
	{
		switch (team)
		{
			case ATTACKER:
			{
				foreach (new i : Player)
				{
					if (Player[i][Team] == ATTACKER || Player[i][Team] == ATTACKER_SUB)
						SendClientMessage(i, -1, va_return("%s\xBB %s is now leading the team.", TextColor[team], Player[playerid][Name]));
					if (i != playerid && GetPlayerColor(i) == ATTACKER_TEAM_LEADER)
						ColorFix(i);
				}
			}
			case DEFENDER:
			{
				foreach (new i : Player)
				{
					if (Player[i][Team] == DEFENDER || Player[i][Team] == DEFENDER_SUB)
						SendClientMessage(i, -1, va_return("%s\xBB %s is now leading the team.", TextColor[team], Player[playerid][Name]));
					if (i != playerid && GetPlayerColor(i) == DEFENDER_TEAM_LEADER)
						ColorFix(i);
				}
			}
		}
	}
	SetPlayerColor(playerid, (team == ATTACKER ? ATTACKER_TEAM_LEADER : DEFENDER_TEAM_LEADER));
	RadarFix(playerid);
	return 1;
}

PlayerNoLeadTeam(playerid)
{
	new team = Player[playerid][Team];

	if (!TeamHasLeader[team] || TeamLeader[team] != playerid)
		return 1;

	TeamLeader[team]	= INVALID_PLAYER_ID;
	TeamHasLeader[team] = false;
	switch (team)
	{
		case ATTACKER:
		{
			foreach (new i : Player)
			{
				if (Player[i][Team] == ATTACKER || Player[i][Team] == ATTACKER_SUB)
					SendClientMessage(i, -1, va_return("%s\xBB %s is no longer leading the team.", TextColor[team], Player[playerid][Name]));
			}
			EndLeaderRequests(playerid);
		}
		case DEFENDER:
		{
			foreach (new i : Player)
			{
				if (Player[i][Team] == DEFENDER || Player[i][Team] == DEFENDER_SUB)
					SendClientMessage(i, -1, va_return("%s\xBB %s is no longer leading the team.", TextColor[team], Player[playerid][Name]));
			}
			EndLeaderRequests(playerid);
		}
	}
	return 1;
}

ResetTeamLeaders()
{
	new playerid;
	for (new team = 0; team < MAX_TEAMS; team++)
	{
		if (!TeamHasLeader[team])
			continue;

		if (IsPlayerConnected(TeamLeader[team]))
		{
			playerid			= TeamLeader[team];
			TeamLeader[team]	= INVALID_PLAYER_ID;
			TeamHasLeader[team] = false;
			ColorFix(playerid);
			RadarFix(playerid);
		}
		else
		{
			TeamLeader[team]	= INVALID_PLAYER_ID;
			TeamHasLeader[team] = false;
		}
	}
	return 1;
}

ResetBackupRequests()
{
	foreach (new i : Player)
	{
		if (Player[i][AskingForHelp])
			EndTeamBackupRequest(i);
	}
}

forward EndTeamBackupRequest(playerid);
public EndTeamBackupRequest(playerid)
{
	if (!Player[playerid][AskingForHelp])
		return 1;
	BackupRequestEpoch[playerid]++;

	new iconid						= Player[playerid][Team] == DEFENDER ? DEFENDER_BACKUP_ICON : ATTACKER_BACKUP_ICON;

	new team						= Player[playerid][Team];
	TeamHelp[team]					= false;
	Player[playerid][AskingForHelp] = false;

	foreach (new i : Player)
	{
		RemovePlayerMapIcon(i, iconid);
	}

	if (TeamHasLeader[team] && TeamLeader[team] == playerid)
		PlayerLeadTeam(playerid, true, false);
	else
	{
		ColorFix(playerid);
		RadarFix(playerid);
	}

	UpdatePlayerTeamBar(playerid);
	return 1;
}

forward EndTeamBackupRequestTimed(playerid, generation, requestEpoch);
public EndTeamBackupRequestTimed(playerid, generation, requestEpoch)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != generation
		|| BackupRequestEpoch[playerid] != requestEpoch || !Player[playerid][AskingForHelp])
		return 0;
	return EndTeamBackupRequest(playerid);
}

EndLeaderRequests(playerid)
{
	new iconid						= Player[playerid][Team] == DEFENDER ? DEFENDER_BACKUP_ICON : ATTACKER_BACKUP_ICON;

	new team						= Player[playerid][Team];
	TeamHelp[team]					= false;
	Player[playerid][AskingForHelp] = false;

	foreach (new i : Player)
	{
		RemovePlayerMapIcon(i, iconid);
	}

	ColorFix(playerid);
	RadarFix(playerid);

	UpdatePlayerTeamBar(playerid);
	return 1;
}

PlayerRequestBackup(playerid)
{
	new team = Player[playerid][Team];

	// Check if there's someone in their team already requesting backup
	if (TeamHelp[team])
		return 0; // fail

	// Calculate the player's total HP
	new totHP = GetHP(playerid) + GetAP(playerid);

	// Get the player's facing angle
	new Float: facingAngle;
	GetPlayerFacingAngle(playerid, facingAngle);

	// Get player's position
	new Float: x,
		Float: y,
		Float: z;
	GetPlayerPos(playerid, x, y, z);

	// Find how many players are in range
	new enemiesCt = 0;
	foreach (new i : PlayersInRound)
	{
		// in enemy team?
		if (Player[i][Team] != team)
			if (IsPlayerInRangeOfPoint(i, 35.0, x, y, z)) // Check if enemy is in range
				enemiesCt++;
	}
	// Inform the player that their backup request is a success
	SendClientMessage(playerid, -1, va_return("{FF4954}\xBB You have requested for backup. (Enemies: %d)", enemiesCt));

	// Get map icon id based off the team which the player is in
	new iconID = team == DEFENDER ? DEFENDER_BACKUP_ICON : ATTACKER_BACKUP_ICON;

	// Set map icon for the player in the location where they requested backup
	SetPlayerMapIcon(playerid, iconID, x, y, z, 19, 0, MAPICON_GLOBAL);

	// Create a variable to hold request backup message
	new str[144];

	// Format backup request message to send it later
	format(
		str, sizeof(str), "{%s}[HELP] %s {FFFFFF}needs backup [Heading %s | HP %d | Enemies: %d]", team == ATTACKER ? "FF6666" : "9999FF", Player[playerid][Name],
		GetCardinalPoint(facingAngle), totHP, enemiesCt);

	// This is for players who use colored markers (via /playermarkers)
	new specialStr[144];

	// Send backup request message to all teammates
	foreach (new i : PlayersInRound)
	{
		if (i == playerid)
			continue;

		if (Player[i][Team] == team)
		{
			if (!Player[i][PlayerMarkers])
				SendClientMessage(i, -1, str);
			else
			{
				format(
					specialStr, sizeof specialStr, "%s[HELP] %s {FFFFFF}needs backup [Heading %s | HP %d | Enemies: %d]", GetColor(GetPlayerCorrectMarkerCol(playerid, i)),
					Player[playerid][Name], GetCardinalPoint(facingAngle), totHP, enemiesCt);
				SendClientMessage(i, -1, specialStr);
			}

			PlayerPlaySound(i, 1137, 0.0, 0.0, 0.0);
			SetPlayerMapIcon(i, iconID, x, y, z, 19, 0, MAPICON_GLOBAL);
		}
	}

	// Set the player's color
	SetPlayerColor(playerid, team == ATTACKER ? ATTACKER_ASKING_HELP : DEFENDER_ASKING_HELP);

	// Set a timer to end backup request later
	new requestEpoch = ++BackupRequestEpoch[playerid];

	// Set values to tell that this team has someone requesting for backup now
	TeamHelp[team]					= true;
	Player[playerid][AskingForHelp] = true;
	if (!SetTimerEx("EndTeamBackupRequestTimed", 7000, false, "iii", playerid, PlayerConnectionGeneration[playerid], requestEpoch))
	{
		printf("[AttDef] Failed to schedule backup-request cleanup for player %d.", playerid);
		EndTeamBackupRequest(playerid);
		return 0;
	}

	// Fix the players radar
	RadarFix(playerid);

	// Update the team bar so that its color changes
	UpdatePlayerTeamBar(playerid);
	return 1; // success
}

forward RemoveTeamDeathMapIcon(teamid, iconid, timerGeneration);
public RemoveTeamDeathMapIcon(teamid, iconid, timerGeneration)
{
	if (teamid < 0 || teamid >= MAX_TEAMS || TeamDeathIconGeneration[teamid] != timerGeneration)
		return 0;

	TeamDeathIconTimer[teamid] = 0;
	foreach (new i : Player)
	{
		RemovePlayerMapIcon(i, iconid);
	}
	return 1;
}

CreateTeamDeathMapIcon(teamid, Float: x, Float: y, Float: z)
{
	if (teamid == ATTACKER_SUB)
		teamid = ATTACKER;
	else if (teamid == DEFENDER_SUB)
		teamid = DEFENDER;
	else if (teamid != ATTACKER && teamid != DEFENDER)
		return 0;

	new iconID = teamid == DEFENDER ? DEFENDER_DEATH_ICON : ATTACKER_DEATH_ICON;

	if (TeamDeathIconTimer[teamid])
		KillTimer(TeamDeathIconTimer[teamid]);
	TeamDeathIconTimer[teamid] = SetTimerEx(
		"RemoveTeamDeathMapIcon", 5000, false, "iii", teamid, iconID, ++TeamDeathIconGeneration[teamid]);
	if (!TeamDeathIconTimer[teamid])
	{
		TeamDeathIconGeneration[teamid]++;
		printf("[AttDef] Failed to schedule death map-icon cleanup for team %d.", teamid);
		foreach (new i : PlayersInRound)
		{
			if (IsTeamTheSame(Player[i][Team], teamid))
				RemovePlayerMapIcon(i, iconID);
		}
		return 0;
	}
	foreach (new i : PlayersInRound)
	{
		if (IsTeamTheSame(Player[i][Team], teamid))
			SetPlayerMapIcon(i, iconID, x, y, z, 23, 0, MAPICON_GLOBAL);
	}
	return 1;
}

forward HideHpTextForAtt(timerGeneration);
public HideHpTextForAtt(timerGeneration)
{
	if (AttHpTimerGeneration != timerGeneration)
		return 0;

	AttHpTimer = 0;
	TempDamage[ATTACKER] = 0.0;
	TextDrawSetString(TeamHpLose[0], " ");
	TextDrawSetString(AttHpLose, " ");
	TextDrawSetString(ModernRound_AttDmgText, " ");
	TextDrawSetString(SimpleRound_AttDmgText, " ");
	TextDrawHideForAll(ModernRound_AttDmgBoard);
	TextDrawHideForAll(ModernRound_AttDmgText);
	TextDrawHideForAll(SimpleRound_AttDmgText);
	StopModernTextDrawFlash(ATTACKER);
	UpdateRoundTeamStatusHUD();
	return 1;
}

forward HideHpTextForDef(timerGeneration);
public HideHpTextForDef(timerGeneration)
{
	if (DefHpTimerGeneration != timerGeneration)
		return 0;

	DefHpTimer = 0;
	TempDamage[DEFENDER] = 0.0;
	TextDrawSetString(TeamHpLose[1], " ");
	TextDrawSetString(DefHpLose, " ");
	TextDrawSetString(ModernRound_DefDmgText, " ");
	TextDrawSetString(SimpleRound_DefDmgText, " ");
	TextDrawHideForAll(ModernRound_DefDmgBoard);
	TextDrawHideForAll(ModernRound_DefDmgText);
	TextDrawHideForAll(SimpleRound_DefDmgText);
	StopModernTextDrawFlash(DEFENDER);
	UpdateRoundTeamStatusHUD();
	return 1;
}

forward DeathMessageF(killerid, playerid, killerGeneration, playerGeneration, killerTextGeneration, playerTextGeneration);
public DeathMessageF(killerid, playerid, killerGeneration, playerGeneration, killerTextGeneration, playerTextGeneration)
{
	if (killerid >= 0 && killerid < MAX_PLAYERS && IsPlayerConnected(killerid)
		&& PlayerConnectionGeneration[killerid] == killerGeneration
		&& PlayerDeathMessageGeneration[killerid] == killerTextGeneration)
		HidePlayerDeathNotice(killerid);
	if (playerid >= 0 && playerid < MAX_PLAYERS && IsPlayerConnected(playerid)
		&& PlayerConnectionGeneration[playerid] == playerGeneration
		&& PlayerDeathMessageGeneration[playerid] == playerTextGeneration)
		HidePlayerDeathNotice(playerid);
	return 1;
}

#include "modules/match/roster.inc"

#include "modules/match/arena.inc"
#include "modules/match/base.inc"


forward NotEndingRound(generation);
public NotEndingRound(generation)
{
	if (generation != RoundEndGeneration)
		return 0;

	AlreadyEndingRound = false;
	return 1;
}

ScheduleRoundUnlock()
{
	new timer = SetTimerEx("NotEndingRound", 3000, false, "i", RoundEndGeneration);
	if (!timer)
	{
		AlreadyEndingRound = false;
		printf("[AttDef] Failed to schedule round-end unlock timer.");
	}
	return timer;
}


EndRound(WinID) //WinID: 0 = CP, 1 = RoundTime, 2 = NoAttackersLeft, 3 = NoDefendersLeft, 4 = Nobody, 5 = Arena:Attackers took cp, 6 = Arena:Defenders took cp
{
	if (AlreadyEndingRound)
		return 0;
	if (Current == -1)
	{
		printf("[AttDef] Ignoring EndRound(%d): there is no active round.", WinID);
		return 0;
	}
	if (WinID < 0 || WinID > 6)
	{
		printf("[AttDef] Ignoring EndRound with invalid result ID %d.", WinID);
		return 0;
	}

	new bool: wasAllVsAll = AllVsAllMode;
	AlreadyEndingRound = true;
	CancelRoundViewSequence();
	RoundEndGeneration++;
	ResetPauseTimers();

	switch (GameType)
	{
		case BASE: BaseStarted = false;
		case ARENA: ArenaStarted = false;
	}

	if (WarMode)
		if (MatchRoundsRecord[MatchRoundsStarted - 1][round__ID] == Current)
			MatchRoundsRecord[MatchRoundsStarted - 1][round__completed] = true;

	ElapsedTime			   = 0;
	TeamCapturingCP		   = NON;
	PlayersInCP			   = 0;
	MatchEnded			   = false;
	FallProtection		   = false;
	RoundPaused			   = false;
	RoundUnpausing		   = false;
	LastMatchEndTime	   = gettime();

	if (WinID == 0 || WinID == 3 || WinID == 5)
		TeamIcons_LastWinner = ATTACKER;
	else if (WinID == 1 || WinID == 2 || WinID == 6)
		TeamIcons_LastWinner = DEFENDER;
	else
		TeamIcons_LastWinner = -1;

	PlayersAlive[ATTACKER] = 0;
	PlayersAlive[DEFENDER] = 0;

	PlayersDead[ATTACKER]  = 0;
	PlayersDead[DEFENDER]  = 0;

	if (CPZone != INVALID_GANG_ZONE)
	{
		GangZoneDestroy(CPZone);
		CPZone = INVALID_GANG_ZONE;
	}
	if (ArenaZone != INVALID_GANG_ZONE)
	{
		GangZoneDestroy(ArenaZone);
		ArenaZone = INVALID_GANG_ZONE;
	}
	if (AttackersZone != INVALID_GANG_ZONE)
	{
		GangZoneDestroy(AttackersZone);
		AttackersZone = INVALID_GANG_ZONE;
	}

	foreach (new i : Player)
	{
		AllowedViewingCommands[i] = false;
		BlackListViewingCommands[i] = false;
		NormalListViewingCommands[i] = false;
		DestroyFlareObjects(i);
		DestroyArenaFlareObjects(i);
	}

	DestroyBlackoutZones();
	StopBaseCameraMovement();
	ResetBackupRequests();

	new iString[256], TopString[3][128];

	TextDrawHideForAll(EN_CheckPoint);
	TextDrawHideForAll(ModernRound_CPBoard);
	TextDrawHideForAll(ModernRound_CPText);
	TextDrawHideForAll(ModernRound_CPPercent);
	TextDrawHideForAll(ModernRound_AttDmgBoard);
	TextDrawHideForAll(ModernRound_AttDmgText);
	TextDrawHideForAll(ModernRound_DefDmgBoard);
	TextDrawHideForAll(ModernRound_DefDmgText);
	TextDrawHideForAll(SimpleRound_CPLine);
	TextDrawHideForAll(SimpleRound_CPText);
	TextDrawHideForAll(SimpleRound_CPPercent);
	TextDrawHideForAll(SimpleRound_AttDmgText);
	TextDrawHideForAll(SimpleRound_DefDmgText);
	TextDrawHideForAll(CheckpointWarningBox);

	new winnerid = -1;
	if (WinID == 0 || WinID == 3 || WinID == 5)
	{
		format(iString, sizeof(iString), "~n~~r~~h~%s ~w~Won The Round", TeamName[ATTACKER]);
		winnerid = ATTACKER;
		if (WarMode)
			TeamScore[ATTACKER]++;
		else
		{
			TeamRoundWins[ATTACKER]++;
			if (!SaveCasualTeamScores(TeamRoundWins[ATTACKER], TeamRoundWins[DEFENDER]))
				printf("[AttDef] Casual score is updated in memory but was not persisted; check the database retry log.");
		}
	}
	else if (WinID == 1 || WinID == 2 || WinID == 6)
	{
		format(iString, sizeof(iString), "~n~~b~~h~%s ~w~Won The Round", TeamName[DEFENDER]);
		winnerid = DEFENDER;
		if (WarMode)
			TeamScore[DEFENDER]++;
		else
		{
			TeamRoundWins[DEFENDER]++;
			if (!SaveCasualTeamScores(TeamRoundWins[ATTACKER], TeamRoundWins[DEFENDER]))
				printf("[AttDef] Casual score is updated in memory but was not persisted; check the database retry log.");
		}
	}
	else if (WinID == 4)
		iString = AllVsAllMode ? "~n~~w~All-vs-All Finished" : "~n~~w~No One Won, Same Team HPs";

	if (WarMode && MatchRoundsStarted > 0)
	{
		MatchRoundsRecord[MatchRoundsStarted - 1][round__winner] = winnerid;
		if (winnerid == ATTACKER)
			format(MatchRoundsRecord[MatchRoundsStarted - 1][round__winnerName], 16, "%s", TeamName[ATTACKER]);
		else if (winnerid == DEFENDER)
			format(MatchRoundsRecord[MatchRoundsStarted - 1][round__winnerName], 16, "%s", TeamName[DEFENDER]);
		else
			MatchRoundsRecord[MatchRoundsStarted - 1][round__winnerName][0] = EOS;
	}

	new mapResultStatus = MAP_RESULT_PERSISTED;
	if (GameType == ARENA && !RCArena)
	{
		mapResultStatus = RecordMapRoundResult(Current, true, winnerid);
	}
	else if (GameType == BASE)
	{
		mapResultStatus = RecordMapRoundResult(Current, false, winnerid);
	}
	if (mapResultStatus == MAP_RESULT_REJECTED)
	{
		printf("[AttDef] Rejected statistics for invalid map result (type %d, map %d, winner %d).", GameType, Current, winnerid);
	}
	else if (mapResultStatus == MAP_RESULT_PENDING_PERSISTENCE)
	{
		printf("[AttDef] Map %d result remains in memory but its statistics were not persisted.", Current);
	}


	UpdateTeamScoreTextDraw();
	UpdateTeamNameTextDraw();

	if (WarMode)
	{
		CurrentRound++;
		UpdateRoundsPlayedTextDraw();
	}

	SendRconCommand("game.map Lobby");
	//FixGamemodeText();

	new index, p;

	new Float: ahpleft,
		Float: dhpleft,
			   aalive,
			   dalive;

	foreach (new i : Player)
	{
		HideRoundStats(i);
		if (Player[i][Spawned])
		{
			if (Player[i][WasInBase])
			{
				if (Player[i][Playing])
				{
					OnPlayerAmmoUpdate(i);

					if (Player[i][Team] == ATTACKER)
					{
						ahpleft = ahpleft + (GetHP(i) + GetAP(i));
						aalive++;
					}
					else if (Player[i][Team] == DEFENDER)
					{
						dhpleft = dhpleft + (GetHP(i) + GetAP(i));
						dalive++;
					}
				}
				playerScores[index][player_Score]  = floatround(Player[i][RoundDamage]);
				playerScores[index][player_Team]   = Player[i][Team];
				playerScores[index][player_Kills]  = Player[i][RoundKills];
				playerScores[index][player_Deaths] = Player[i][RoundDeaths];
				playerScores[index][player_HP]	   = GetHP(i) + GetAP(i);
				playerScores[index][player_Acc]	   = floatround(Player[i][Accuracy], floatround_round);

				if (playerScores[index][player_Deaths] > 0)
					playerScores[index][player_Overall] = playerScores[index][player_Score] + playerScores[index][player_Kills] + playerScores[index][player_Acc];
				else
					playerScores[index][player_Overall]
						= playerScores[index][player_Score] + playerScores[index][player_HP] + playerScores[index][player_Kills] + playerScores[index][player_Acc];

				format(RoundResultNames[index], MAX_PLAYER_NAME, Player[i][NameWithoutTag]);

				playerScores[index++][player_ID] = i;
				p++;

				Player[i][RoundPlayed]++;
			}
			Player[i][WasInCP]		= false;
			Player[i][ToAddInRound] = false;
			Player[i][gPauseCheck] = false;
			Player[i][gPauseWarnings] = 0;
			Player[i][gWasPaused] = 0;

			if (Player[i][Spectating])
				StopSpectate(i);

			Player[i][WasInBase]	= false;
			Player[i][WasInTeam]	= NON;
			Player[i][WeaponPicked] = 0;
			Player[i][TimesSpawned] = 0;
			RemovePlayerMapIcon(i, 59);

			PlayerTextDrawHide(i, AreaCheckTD[i]);
			PlayerTextDrawHide(i, AreaCheckBG[i]);

			if (!Player[i][InDuel] && !Player[i][InDM])
			{
				SetHPAP(i, 100.0, 100.0);
			}


			if (Player[i][Playing])
			{
				SetPlayerRoundParticipation(i, false);
				SpawnInLobby(i);
				HidePlayerDeathNotice(i);
			}

			if (!Player[i][InDuel] && !Player[i][InDM] && (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER))
				ShowEndRoundTextDraw(i);
			DisablePlayerCheckpoint(i);
			SetPlayerScore(i, 0);
			if (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER)
			{
				HidePlayerDialog(i);
				HidePlayerGunmenu(i);
			}
		}
	}

	for (new i = 0; i < SAVE_SLOTS; ++i)
	{
		if (SaveVariables[i][pName][0] && Current == SaveVariables[i][RoundID] && SaveVariables[i][CheckScore])
		{
			playerScores[index][player_Score]  = floatround(SaveVariables[i][RDamage]);
			playerScores[index][player_Team]   = SaveVariables[i][pTeam];
			playerScores[index][player_Kills]  = SaveVariables[i][RKills];
			playerScores[index][player_Deaths] = SaveVariables[i][RDeaths];
			playerScores[index][player_HP]	   = SaveVariables[i][gHealth] + SaveVariables[i][gArmour];
			playerScores[index][player_Acc]	   = SaveVariables[i][iAccuracy];

			if (playerScores[index][player_Deaths] > 0)
				playerScores[index][player_Overall] = playerScores[index][player_Score] + playerScores[index][player_Kills] + playerScores[index][player_Acc];
			else
				playerScores[index][player_Overall]
					= playerScores[index][player_Score] + playerScores[index][player_HP] + playerScores[index][player_Kills] + playerScores[index][player_Acc];

			format(RoundResultNames[index], MAX_PLAYER_NAME, SaveVariables[i][pNameWithoutTag]);

			SaveVariables[i][RKills]	 = 0;
			SaveVariables[i][RDeaths]	 = 0;
			SaveVariables[i][RDamage]	 = 0.0;

			SaveVariables[i][RoundID]	 = -1;
			SaveVariables[i][CheckScore] = false;

			index++;
			p++;
		}
	}

	Current = -1;
	AllVsAllMode = false;
	if (wasAllVsAll)
	{
		RadarFix();
		foreach (new i : Player)
		{
			Player[i][RoundTDStyle] = Player[i][PreAllVsAllStyle];
			UpdateRoundKillDmgText(i);
		}
		UpdateTeamNamesTextdraw();
		TextDrawSetString(SimpleRound_AttHP, "_");
		TextDrawSetString(SimpleRound_DefHP, "_");
	}

	if (index > 1)
		GetPlayerHighestScores2(playerScores, RoundResultNames, 0, index - 1);

	new topkill, topkillID = -1, Float: topDmg,
				 topDmgID = -1,
				 topAcc,
				 topAccID = -1,
				 topOverall,
				 topOverallID = -1;

	for (new i = 0; i != p; ++i)
	{
		if (playerScores[i][player_Kills] > topkill && playerScores[i][player_Kills] > 0)
		{
			topkill	  = playerScores[i][player_Kills];
			topkillID = i;
		}
		if (playerScores[i][player_Score] > topDmg && playerScores[i][player_Score] > 0)
		{
			topDmg	 = playerScores[i][player_Score];
			topDmgID = i;
		}
		if (playerScores[i][player_Acc] > topAcc && playerScores[i][player_Acc] > 0)
		{
			topAcc	 = playerScores[i][player_Acc];
			topAccID = i;
		}
		if (playerScores[i][player_Overall] > topOverall && playerScores[i][player_Score] > 0 && playerScores[i][player_Team] == winnerid)
		{
			topOverall	 = playerScores[i][player_Overall];
			topOverallID = i;
		}

		if (playerScores[i][player_Team] == ATTACKER || playerScores[i][player_Team] == ATTACKER_SUB)
		{
			format(AttList, sizeof(AttList), "%s~w~%s~n~", AttList, RoundResultNames[i]);
			format(AttKills, sizeof(AttKills), "%s~w~%d~n~", AttKills, playerScores[i][player_Kills]);
			if (playerScores[i][player_Deaths] > 0)
				format(AttDeaths, sizeof(AttDeaths), "%s~w~Dead~n~", AttDeaths);
			else
				format(AttDeaths, sizeof(AttDeaths), "%s~w~%d~n~", AttDeaths, playerScores[i][player_HP]);
			format(AttDamage, sizeof(AttDamage), "%s~w~%d~n~", AttDamage, playerScores[i][player_Score]);
			format(AttAcc, sizeof(AttAcc), "%s~w~%d%%~n~", AttAcc, playerScores[i][player_Acc]);
		}
		else if (playerScores[i][player_Team] == DEFENDER || playerScores[i][player_Team] == DEFENDER_SUB)
		{
			format(DefList, sizeof(DefList), "%s~w~%s~n~", DefList, RoundResultNames[i]);
			format(DefKills, sizeof(DefKills), "%s~w~%d~n~", DefKills, playerScores[i][player_Kills]);
			if (playerScores[i][player_Deaths] > 0)
				format(DefDeaths, sizeof(DefDeaths), "%s~w~Dead~n~", DefDeaths);
			else
				format(DefDeaths, sizeof(DefDeaths), "%s~w~%d~n~", DefDeaths, playerScores[i][player_HP]);

			format(DefDamage, sizeof(DefDamage), "%s~w~%d~n~", DefDamage, playerScores[i][player_Score]);
			format(DefAcc, sizeof(DefAcc), "%s~w~%d%%~n~", DefAcc, playerScores[i][player_Acc]);
		}

		if (i == 0)
			format(
				TopString[0], 128, "\xBB " COL_WARNING "1st: " COL_SEC "%s " COL_MUTED "(Damage: " COL_SEC "%d " COL_MUTED "| Kills: " COL_SEC "%d" COL_MUTED ")", RoundResultNames[i], playerScores[i][player_Score],
				playerScores[i][player_Kills]);
		else if (i == 1)
			format(
				TopString[1], 128, "\xBB " COL_MUTED "2nd: " COL_SEC "%s " COL_MUTED "(Damage: " COL_SEC "%d " COL_MUTED "| Kills: " COL_SEC "%d" COL_MUTED ")", RoundResultNames[i], playerScores[i][player_Score],
				playerScores[i][player_Kills]);
		else if (i == 2)
			format(
				TopString[2], 128, "\xBB " COL_MUTED "3rd: " COL_SEC "%s " COL_MUTED "(Damage: " COL_SEC "%d " COL_MUTED "| Kills: " COL_SEC "%d" COL_MUTED ")", RoundResultNames[i], playerScores[i][player_Score],
				playerScores[i][player_Kills]);
	}

	iString = "";

	UpdateHPBarsEndRoundStats(playerScores, RoundResultNames, p, winnerid, ahpleft, dhpleft, aalive, dalive, topkillID, topkill, topDmgID, topDmg, topAccID, topAcc);
	UpdateTeamIconsEndRoundStats(playerScores, RoundResultNames, p, winnerid, WinID, ahpleft, dhpleft, aalive, dalive, topkillID, topkill, topDmgID, topDmg, topAccID, topAcc);
	UpdateModernEndRoundStats(playerScores, RoundResultNames, p, winnerid, WinID, ahpleft, dhpleft, aalive, dalive, topkillID, topkill, topDmgID, topDmg, topAccID, topAcc, AttList, AttDamage, AttKills, AttDeaths, DefList, DefDamage, DefKills, DefDeaths);

	iString = "";

	new rMessage[144];
	ClearChat();

	SendClientMessageToAll(-1, COL_PRIM "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	switch (WinID)
	{
		case 0, 5: // Attacker Checkpoint Capture
		{
			format(rMessage, sizeof(rMessage), "\xBB %sTeam %s {FFFFFF}won %s %d by %sCheckpoint Capture " COL_MUTED "| HP Remaining: %.0f", TextColor[ATTACKER], TeamName[ATTACKER], (GameType == BASE ? "Base" : "Arena"), ServerLastPlayed, TextColor[ATTACKER], ahpleft);
			SendClientMessageToAll(-1, rMessage);
		}
		case 3: // Attacker Elimination
		{
			format(rMessage, sizeof(rMessage), "\xBB %sTeam %s {FFFFFF}won %s %d by %sElimination " COL_MUTED "| HP Remaining: %.0f", TextColor[ATTACKER], TeamName[ATTACKER], (GameType == BASE ? "Base" : "Arena"), ServerLastPlayed, TextColor[ATTACKER], ahpleft);
			SendClientMessageToAll(-1, rMessage);
		}
		case 2: // Defender Elimination
		{
			format(rMessage, sizeof(rMessage), "\xBB %sTeam %s {FFFFFF}won %s %d by %sElimination " COL_MUTED "| HP Remaining: %.0f", TextColor[DEFENDER], TeamName[DEFENDER], (GameType == BASE ? "Base" : "Arena"), ServerLastPlayed, TextColor[DEFENDER], dhpleft);
			SendClientMessageToAll(-1, rMessage);
		}
		case 6: // Defender Checkpoint Capture
		{
			format(rMessage, sizeof(rMessage), "\xBB %sTeam %s {FFFFFF}won %s %d by %sCheckpoint Capture " COL_MUTED "| HP Remaining: %.0f", TextColor[DEFENDER], TeamName[DEFENDER], (GameType == BASE ? "Base" : "Arena"), ServerLastPlayed, TextColor[DEFENDER], dhpleft);
			SendClientMessageToAll(-1, rMessage);
		}
		case 4: // Everyone died / Draw
		{
			if (wasAllVsAll)
			{
				new avaWinner = -1, avaKills = -1, bool: avaTie = false;
				for (new j = 0; j < p; j++)
				{
					if (playerScores[j][player_Kills] > avaKills)
					{
						avaKills = playerScores[j][player_Kills];
						avaWinner = j;
						avaTie = false;
					}
					else if (playerScores[j][player_Kills] == avaKills)
					{
						avaTie = true;
					}
					}
				if (avaWinner != -1 && !avaTie && avaKills > 0)
				{
					format(rMessage, sizeof(rMessage), "\xBB " COL_PRIM "All-vs-All on " COL_SEC "%s %d " COL_PRIM "Winner: " COL_SEC "%s " COL_PRIM "(Kills: " COL_SEC "%d" COL_PRIM ")", (GameType == BASE ? "Base" : "Arena"), ServerLastPlayed, RoundResultNames[avaWinner], avaKills);
				}
				else
				{
					format(rMessage, sizeof(rMessage), "\xBB " COL_PRIM "All-vs-All on " COL_SEC "%s %d " COL_PRIM "ended in a tie.", (GameType == BASE ? "Base" : "Arena"), ServerLastPlayed);
				}
			}
			else
			{
				format(rMessage, sizeof(rMessage), "\xBB " COL_PRIM "Round Draw! Everyone died on " COL_SEC "%s %d" COL_PRIM ", no team scored a point.", (GameType == BASE ? "Base" : "Arena"), ServerLastPlayed);
			}
			SendClientMessageToAll(-1, rMessage);
		}
		case 1: // Time expired / Defender wins
		{
			format(rMessage, sizeof(rMessage), "\xBB %sTeam %s {FFFFFF}won %s %d by %sTime Out " COL_MUTED "| HP Remaining: %.0f", TextColor[DEFENDER], TeamName[DEFENDER], (GameType == BASE ? "Base" : "Arena"), ServerLastPlayed, TextColor[DEFENDER], dhpleft);
			SendClientMessageToAll(-1, rMessage);
		}
	}

	if (TopString[0][0]) SendClientMessageToAll(-1, TopString[0]);
	if (TopString[1][0]) SendClientMessageToAll(-1, TopString[1]);
	if (TopString[2][0]) SendClientMessageToAll(-1, TopString[2]);
	SendClientMessageToAll(-1, COL_PRIM "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	SendClientMessageToAll(-1, " ");
	if (topOverallID != -1 && winnerid != -1)
		SendClientMessageToAll(-1, va_return("\xBB {FFFFFF}Round MVP: %s%s", TextColor[playerScores[topOverallID][player_Team]], RoundResultNames[topOverallID]));

	format(LastRoundSummary, sizeof(LastRoundSummary), "%s", rMessage);
	LastRoundScoreCount = p;

	CallLocalFunction("OnRoundEndEvent", "ddd", winnerid, WinID, topOverallID);

	if (!wasAllVsAll && (winnerid == ATTACKER || winnerid == DEFENDER)
		&& RoundClutchEnemies[winnerid] >= 2
		&& RoundClutchPlayer[winnerid] != INVALID_PLAYER_ID
		&& IsPlayerConnected(RoundClutchPlayer[winnerid]))
	{
		new clutcher = RoundClutchPlayer[winnerid];
		Player[clutcher][ClutchesWon]++;
		UpdateRoundKillDmgTDmg(clutcher);
		SendClientMessageToAll(
			-1,
			va_return("\xBB " COL_PRIM "Round Clutch: %s%s " COL_SEC "clutched a " COL_PRIM "1v%d " COL_SEC "situation!", TextColor[winnerid], Player[clutcher][Name], RoundClutchEnemies[winnerid])
		);
	}
	RoundClutchPlayer[ATTACKER] = INVALID_PLAYER_ID;
	RoundClutchPlayer[DEFENDER] = INVALID_PLAYER_ID;
	RoundClutchEnemies[ATTACKER] = 0;
	RoundClutchEnemies[DEFENDER] = 0;

	AllowStartBase = false;

	if (WarMode)
	{
		if (!SetTimerEx("SwapBothTeams", 2500, false, "i", RoundEndGeneration))
		{
			printf("[AttDef] Failed to schedule post-round team swap.");
			SwapBothTeams(RoundEndGeneration);
		}
	}
	else if (AutoBal)
	{
		if (!SetTimerEx("DoAutoBalance", 2500, false, "i", RoundEndGeneration))
		{
			printf("[AttDef] Failed to schedule post-round auto-balance.");
			DoAutoBalance(RoundEndGeneration);
		}
	}
	else if (!SetTimerEx("DontAutoBalance", 2500, false, "i", RoundEndGeneration))
	{
		printf("[AttDef] Failed to schedule post-round team balance.");
		DontAutoBalance(RoundEndGeneration);
	}

	if (HalfDeagle)
	{
		AdjustDeagleLimit();
	}

	if(AutoEnd)
	{
		CheckWinCondition();
	}

	if (CurrentRound >= TotalRounds && CurrentRound != 0)
	{
		ScheduleWarEnd(5000);
		SendClientMessageToAll(-1, "\xBB " COL_PRIM "Generating final match results... " COL_MUTED "Type " COL_SEC "/showagain " COL_MUTED "to view them again.");
	}
	AttList	  = "";
	AttKills  = "";
	AttDeaths = "";
	AttDamage = "";
	AttAcc	  = "";
	DefList	  = "";
	DefKills  = "";
	DefDeaths = "";
	DefDamage = "";
	DefAcc	  = "";

	DeleteAllTeamBars();
	HideTeamCornerInfoForAll();

	DeleteAllDeadBodies();
	ResetTeamLeaders();
	Iter_Clear(PlayersInRound);

	if (AutoRoundStarter)
		SetRoundAutoStart(20);

	ScheduleRoundUnlock();

	switch (random(3))
	{
		case 2: SetTimerEx("SendTipMessageToAll", 7000, false, "b", false);
	}
	return 1;
}

#include "modules/match/war.inc"
