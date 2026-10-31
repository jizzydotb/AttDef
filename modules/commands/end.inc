#if defined _attdef_cmd_end
	#endinput
#endif
#define _attdef_cmd_end

YCMD: end(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 3 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "kill the currently active round.");
	if (AlreadyEndingRound)
		return SendErrorMessage(playerid, "Please wait.");
	if (Current == -1 && AllowStartBase)
		return SendErrorMessage(playerid, "Round is not active.");

	ResetPauseTimers();

	CancelPendingRoundStart();
	CancelRoundViewSequence();
	TextDrawSetString(centerTeamNames, " ");
	TextDrawSetString(AttDef_RoundTimer, " ");
	TextDrawSetString(ModernRound_Timer, " ");
	TextDrawSetString(SimpleRound_Timer, " ");

	new bool: wasAllVsAll = AllVsAllMode;
	Current = -1;
	AllVsAllMode = false;
	AllowStartBase = true;
	if (RoundPaused)
		TextDrawHideForAll(PauseTD);

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

	DestroyBlackoutZones();
	StopBaseCameraMovement();
	StopArenaCameraMovement();

	foreach (new i : Player)
	{
		DestroyFlareObjects(i);
		DestroyArenaFlareObjects(i);
	}

	RoundPaused			   = false;
	FallProtection 		   = false;
	TeamCapturingCP		   = NON;
	PlayersInCP			   = 0;

	PlayersAlive[ATTACKER] = 0;
	PlayersAlive[DEFENDER] = 0;

	PlayersDead[ATTACKER]  = 0;
	PlayersDead[DEFENDER]  = 0;

	RoundUnpausing		   = false;

	foreach (new i : Player)
	{
		if (Player[i][InDM] || Player[i][InDuel] || Player[i][IsAFK] || !Player[i][Spawned])
			continue;

		SetPlayerRoundParticipation(i, false);
		Player[i][ToAddInRound] = false;
		Player[i][WasInCP] = false;
		Player[i][gPauseCheck] = false;
		Player[i][gPauseWarnings] = 0;
		Player[i][gWasPaused] = 0;

		if (Player[i][Spectating])
			StopSpectate(i);
		Player[i][WasInBase]	= false;
		Player[i][WasInTeam]	= NON;
		Player[i][WeaponPicked] = 0;
		Player[i][TimesSpawned] = 0;

		HidePlayerGunmenu(i);
		RemovePlayerMapIcon(i, 59);

		SpawnInLobby(i);
		TogglePlayerControllable(i, 1);

		DisablePlayerCheckpoint(i);
		SetPlayerScore(i, 0);
		HidePlayerDialog(i);

		PlayerTextDrawHide(i, AreaCheckTD[i]);
		PlayerTextDrawHide(i, AreaCheckBG[i]);
	}

	foreach (new i : Player)
	{
		HideRoundStats(i);
		UpdateRoundKillDmgText(i);
	}
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
	TextDrawHideForAll(EN_CheckPoint);
	TextDrawHideForAll(CheckpointWarningBox);

	ResetGunmenuSelections();
	ResetBackupRequests();

	BaseStarted	 = false;
	ArenaStarted = false;

	SendRconCommand("game.map Lobby");
	//FixGamemodeText();

	DeleteAllTeamBars();
	HideTeamCornerInfoForAll();

	DeleteAllDeadBodies();

	ResetTeamLeaders();
	Iter_Clear(PlayersInRound);

	foreach (new i : Player)
	{
		AllowedViewingCommands[i] = false;
		BlackListViewingCommands[i] = false;
		NormalListViewingCommands[i] = false;
	}

	if (AutoRoundStarter)
		SetRoundAutoStart(20);

	LogAdminCommand("end", playerid, INVALID_PLAYER_ID);



	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"ended the round.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
}
