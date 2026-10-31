#if defined _attdef_OnPSpawn
	#endinput
#endif
#define _attdef_OnPSpawn

public OnPlayerSpawn(playerid)
{
	ResetVictimAssistDamage(playerid);
	PlayerDeathFlowGeneration[playerid]++;
	PlayerRejoinSpawnGeneration[playerid]++;
	PlayerVehicleRejoinGeneration[playerid]++;
	DelayedFreezeGeneration[playerid]++;
	PlayerPlaySound(playerid, 1188, 0.0, 0.0, 0.0);
	SetPlayerDrunkLevel(playerid, 0);
	HideMessageBox(playerid, MSGBOX_TYPE_TOP);
	HideMessageBox(playerid, MSGBOX_TYPE_MIDDLE);
	HideMessageBox(playerid, MSGBOX_TYPE_BOTTOM);
	if (Player[playerid][IgnoreSpawn])
	{
		// This spawn call should be ignored (used for many things .. e.g the SyncPlayer function)
		Player[playerid][IgnoreSpawn] = false;
		if (Current != -1 && Player[playerid][RoundTDStyle] == 4 && Player[playerid][Team] != REFEREE && (Player[playerid][Playing] || Player[playerid][Spectating] || Player[playerid][ToAddInRound]))
			ShowRoundStats(playerid, false);
		return 1;
	}

	if (AllVsAllMode && GameType == ARENA && ArenaStarted && Current != -1 && Player[playerid][Playing])
	{
		SpawnAllVsAllPlayer(playerid);
		return 1;
	}

	// If they're selecting from gunmenu
	if (Player[playerid][OnGunmenu])
	{
		// Hide it!!!
		HidePlayerGunmenu(playerid);
	}

	if (Player[playerid][IsFrozen])
	{
		Player[playerid][IsFrozen] = false;
	}

	if (Player[playerid][InDuel])
	{
		SetupDuellerSpawn(playerid, true, Player[playerid][DuelRoundPending]);
		return 1;
	}

	// If this player is just spawning regularly
	if (!Player[playerid][Playing] && !Player[playerid][InDM] && !Player[playerid][InDuel])
	{
		// Adjust player HP
		SetHPAP(playerid, 100.0, 100.0);

		// Unarm players from any weapons
		ResetPlayerWeapons(playerid);
		// 'playerid' is unique which means playerids can never match, hence we use that to initialize player's team
		// and workaround SAMP's built-in anti team-shooting (i.e players in the same team cannot harm each other)
		SetPlayerTeam(playerid, NO_TEAM);
		SetPlayerScore(playerid, 0);
		UpdatePlayerHealthMoney(playerid);

		// Initialize player spawn camera and position
		SetPlayerInterior(playerid, MainInterior);
		SetPlayerVirtualWorld(playerid, 0);
		SetCameraBehindPlayer(playerid);

		ColorFix(playerid); // Fixes player color based on their team.
		RadarFix();
	}

	// Fixes the x Vs y textdraws with current team player count
	FixVsTextDraw();

	new remainingTime = gAddmeTime - ElapsedTime;

	// If there's no round running, hide the round stats textdraws
	if (Current == -1)
		HideRoundStats(playerid);
	else
	{
		if (Player[playerid][RoundTDStyle] == 4 && Player[playerid][Team] != REFEREE && (Player[playerid][Playing] || Player[playerid][Spectating] || Player[playerid][ToAddInRound]))
			ShowRoundStats(playerid);

		if (ElapsedTime <= gAddmeTime && !Player[playerid][Playing] && !WarMode && (Player[playerid][Team] == ATTACKER || Player[playerid][Team] == DEFENDER))
			va_SendClientMessage(playerid, -1, COL_PRIM "\xBB You can use " COL_SEC "/addme " COL_PRIM "for the next " COL_SEC "%d seconds " COL_PRIM "to add yourself to the current round.", remainingTime);
	}
	UpdatePlayerHealthMoney(playerid);
	return 1;
}
