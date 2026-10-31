#if defined _attdef_cmd_rr
	#endinput
#endif
#define _attdef_cmd_rr

YCMD: rr(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "restart a round.");
	if (Current == -1)
		return SendErrorMessage(playerid, "Round is not active.");
	if (!AllowStartBase)
		return SendErrorMessage(playerid, "Please wait.");

	AllowStartBase = false;
	if (RoundPaused)
		TextDrawHideForAll(PauseTD);
	RoundPaused	   = false;
	RoundUnpausing = false;

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

	DeleteAllDeadBodies();
	DestroyBlackoutZones();

	foreach (new i : Player)
	{
		DestroyFlareObjects(i);
		DestroyArenaFlareObjects(i);
	}

	ResetBackupRequests();
	ResetPauseTimers();



	new iString[128];
	format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has ordered a round restart. Restarting round...", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	SendClientMessageToAll(-1, iString);

	ResetGunmenuSelections();

	if (GameType == BASE)
	{
		BaseStarted = false;
		ScheduleRoundStart(BASE, Current, 4000);
	}
	else if (GameType == ARENA)
	{
		ArenaStarted = false;
		ScheduleRoundStart(ARENA, Current, 4000);
	}

	foreach (new i : Player)
	{
		if (Player[i][OnGunmenu])
			HidePlayerGunmenu(i);
		if (CanPlay(i))
		{
			if (Player[i][Spectating])
				StopSpectate(i);
			Player[i][WasInCP]		= false;

			Player[i][WasInBase]	= false;
			Player[i][WasInTeam]	= NON;
			Player[i][WeaponPicked] = 0;
			Player[i][TimesSpawned] = 0;

			Player[i][gPauseCheck]  = true;
			Player[i][gPauseWarnings] = 0;
			Player[i][gWasPaused]    = 0;

			HidePlayerDialog(i);
			DisablePlayerCheckpoint(i);
			RemovePlayerMapIcon(i, 59);

			PlayerTextDrawHide(i, AreaCheckTD[i]);
			PlayerTextDrawHide(i, AreaCheckBG[i]);
			TogglePlayerControllable(i, 0);
			Player[i][ToAddInRound] = true;
		}
	}

	foreach (new i : Player)
	{
		HideRoundStats(i);
	}
	TextDrawHideForAll(EN_CheckPoint);
	TextDrawHideForAll(CheckpointWarningBox);
	return 1;
}
