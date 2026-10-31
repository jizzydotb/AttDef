#if defined _attdef_match_pause
	#endinput
#endif
#define _attdef_match_pause

/*
	Match/Round Pause & Stopwatch Sub-module
*/

public ResetPauseTimers()
{
	PauseTimerGeneration++;
	PauseTimeElapsed = 0;
	TotalPauseTimeElapsed = 0;
	TotalPausesInRound = 0;
	TacticalPauseRemaining = 0;

	if (UnpauseTimer)
	{
		KillTimer(UnpauseTimer);
		UnpauseTimer = 0;
	}
	if (PauseStopWatchTimer)
	{
		KillTimer(PauseStopWatchTimer);
		PauseStopWatchTimer = 0;
	}
	if (TipTimerDuringPause)
	{
		KillTimer(TipTimerDuringPause);
		TipTimerDuringPause = 0;
	}
	PauseCountdown = 0;
	RoundUnpausing = false;
	return 1;
}

forward UnpauseRoundTimed(generation);
public UnpauseRoundTimed(generation)
{
	if (generation != PauseTimerGeneration || !RoundUnpausing)
		return 0;
	return ProcessUnpauseRound(generation);
}

forward UnpauseRound();
public UnpauseRound()
{
	if (RoundUnpausing)
		return 0;

	new generation = ++PauseTimerGeneration;
	return ProcessUnpauseRound(generation);
}

ProcessUnpauseRound(generation)
{
	if (Current == -1 || !RoundPaused)
	{
		UnpauseTimer = 0;
		RoundUnpausing = false;
		return 1;
	}
	UnpauseTimer = 0;

	if (PauseStopWatchTimer)
	{
		KillTimer(PauseStopWatchTimer);
		PauseStopWatchTimer = 0;
	}
	TacticalPauseRemaining = 0;

	RoundUnpausing = true;
	PauseCountdown--;

	if (PauseCountdown > 0)
	{
		foreach (new i : PlayersInRound)
			PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);

		new iString[48];
		format(iString, sizeof(iString), "~r~Round resuming in ~r~~h~%d", PauseCountdown);
		TextDrawSetString(PauseTD, iString);
		ShowPauseTDForAll();
		UnpauseTimer = SetTimerEx("UnpauseRoundTimed", 1000, false, "i", generation);
		if (!UnpauseTimer)
		{
			printf("[AttDef] Failed to schedule unpause countdown; resuming round immediately.");
			PauseCountdown = 1;
			return ProcessUnpauseRound(generation);
		}
	}
	else
	{
		foreach (new i : PlayersInRound)
		{
			PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
			if (!Player[i][OnGunmenu] && ViewRoundCountdown <= 0)
				TogglePlayerControllable(i, 1);
			else
				TogglePlayerControllable(i, 0);

			if (!Player[i][ToGiveParachute])
				SetPlayerArmedWeapon(i, 0);
			else
			{
				GivePlayerWeapon(i, WEAPON_PARACHUTE, 1);
				SetPlayerArmedWeapon(i, WEAPON_PARACHUTE);
				Player[i][ToGiveParachute] = false;
			}
		}
		FreezeV_UnpauseRound();

		RoundPaused	   = false;
		RoundUnpausing = false;
		TextDrawHideForAll(PauseTD);
		RecountPlayersOnCP();
		if (!AllVsAllMode)
			CheckTeamElimination();

		if (TipTimerDuringPause)
		{
			KillTimer(TipTimerDuringPause);
			TipTimerDuringPause = 0;
		}
	}
	return 1;
}

forward PauseStopWatch();
public PauseStopWatch()
{
	PauseTimeElapsed++;
	TotalPauseTimeElapsed++;
	if (TacticalPauseRemaining > 0)
	{
		TacticalPauseRemaining--;
		va_TextDrawSetString(PauseTD, "~y~~h~Tactical Timeout!~n~~w~Resuming in: ~r~~h~%02d:%02d", TacticalPauseRemaining / 60, TacticalPauseRemaining % 60);
		if (TacticalPauseRemaining == 0 && RoundPaused && !RoundUnpausing)
		{
			SendClientMessageToAll(-1, COL_PRIM "\xBB Tactical timeout has ended. Resuming the current round!");
			PauseCountdown = 4;
			UnpauseRound();
		}
		return 1;
	}
	va_TextDrawSetString(
		PauseTD,
		"~r~Round is paused!~n~Time ~r~~h~%02d:%02d~n~~r~Total Time ~r~~h~%02d:%02d~n~~r~Count ~r~~h~%d",
		PauseTimeElapsed / 60, PauseTimeElapsed % 60,
		TotalPauseTimeElapsed / 60, TotalPauseTimeElapsed % 60,
		TotalPausesInRound
	);
	return 1;
}

PauseRound()
{
	if (Current == -1 || RoundPaused)
		return 0;

	PauseTimerGeneration++;
	RoundUnpausing = false;
	if (UnpauseTimer)
	{
		KillTimer(UnpauseTimer);
		UnpauseTimer = 0;
	}
	FreezeV_PauseRound();

	if (TipTimerDuringPause)
		KillTimer(TipTimerDuringPause);

	TipTimerDuringPause = SetTimerEx("SendTipMessageToAll", 15000, false, "b", true);
	if (!TipTimerDuringPause)
		printf("[AttDef] Failed to schedule paused-round tip.");

	if (!PauseStopWatchTimer)
	{
		TotalPausesInRound++;
		PauseTimeElapsed = 0;
		PauseStopWatchTimer = SetTimer("PauseStopWatch", 1000, true);
		if (!PauseStopWatchTimer)
		{
			printf("[AttDef] Failed to start paused-round stopwatch.");
			PauseStopWatch();
		}
	}

	// Make sure this bix red textdraw won't stay on players screen because it will get annoying
	TextDrawHideForAll(CheckpointWarningBox);

	foreach (new i : PlayersInRound)
	{
		TogglePlayerControllable(i, 0);
		if (GetPlayerWeapon(i) == WEAPON_PARACHUTE)
			Player[i][ToGiveParachute] = true;
	}
	va_TextDrawSetString(
		PauseTD,
		"~r~Round is paused!~n~Time ~r~~h~%02d:%02d~n~~r~Total Time ~r~~h~%02d:%02d~n~~r~Count ~r~~h~%d",
		PauseTimeElapsed / 60, PauseTimeElapsed % 60,
		TotalPauseTimeElapsed / 60, TotalPauseTimeElapsed % 60,
		TotalPausesInRound
	);
	ShowPauseTDForAll();
	RoundPaused = true;
	return 1;
}
