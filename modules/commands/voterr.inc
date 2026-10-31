#if defined _attdef_cmd_voterr
	#endinput
#endif
#define _attdef_cmd_voterr

YCMD: voterr(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "start a voting (restart round)");
	return PlayerVoteRestartRound(playerid);
}

PlayerVoteRestartRound(playerid)
{
	if (Current == -1)
		return SendErrorMessage(playerid, "There's no round in progress!");
	if (!AllowStartBase)
		return SendErrorMessage(playerid, "Please wait.");
	if (!Player[playerid][Playing])
		return SendErrorMessage(playerid, "You must be playing to be able to vote on this!");

	new votetype = VOTE_TYPE_RESTARTROUND;
	if (VoteData[votetype][PlayerVoted][playerid])
		return SendErrorMessage(playerid, "You've already voted!");

	// Counting players who are able to vote
	new MaxVotesNeeded = Iter_Count(PlayersInRound);
	if (VoteData[votetype][VoteCount] < 1) // First vote?
	{
		if (!ScheduleVoteExpiration(votetype))
			return SendErrorMessage(playerid, "Could not start the vote because its timeout timer failed.");
		VoteData[votetype][VoteCount]		  = 1;
		foreach (new i : Player)
		{
			VoteData[votetype][PlayerVoted][i] = false;
			PlayerPlaySound(i, 1133, 0.0, 0.0, 0.0);
		}
		VoteData[votetype][PlayerVoted][playerid] = true;
		SendClientMessageToAll(
			-1, va_return(
					""COL_PRIM"\xBB "COL_SEC"%s" COL_PRIM " has started "COL_SEC"restart round-voting " COL_PRIM "[%d/%d] [/voterr]", Player[playerid][Name], VoteData[votetype][VoteCount],
					MaxVotesNeeded));
	}
	else
	{
		VoteData[votetype][VoteCount]++;
		VoteData[votetype][PlayerVoted][playerid] = true;
		SendClientMessageToAll(
			-1, va_return(
					""COL_PRIM"\xBB "COL_SEC"%s" COL_PRIM " has added to "COL_SEC"restart round-voting " COL_PRIM "[%d/%d] [/voterr]", Player[playerid][Name], VoteData[votetype][VoteCount],
					MaxVotesNeeded));
		if (VoteData[votetype][VoteCount] >= MaxVotesNeeded) // Enough people voted!
		{
			AllowStartBase = false;
			SendClientMessageToAll(-1, "" COL_PRIM "\xBB Restart round-voting result: "COL_SEC"round has been set to restart!");
			ResetVoting(votetype, false);
			ResetPauseTimers();
			if (RoundPaused)
			{
				TextDrawHideForAll(PauseTD);
				RoundPaused = false;
			}
			RoundUnpausing = false;

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
					Player[i][WasInTeam]	= NON;

					Player[i][WasInBase]	= false;
					Player[i][WeaponPicked] = 0;
					Player[i][TimesSpawned] = 0;

					Player[i][gPauseWarnings] = 0;
					Player[i][gWasPaused] = 0;

					HidePlayerDialog(i);
					DisablePlayerCheckpoint(i);
					RemovePlayerMapIcon(i, 59);

					PlayerTextDrawHide(i, AreaCheckTD[i]);
					PlayerTextDrawHide(i, AreaCheckBG[i]);
					TogglePlayerControllable(i, 0);
					Player[i][ToAddInRound] = true;
				}
				HideRoundStats(i);
			}

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
			foreach (new i : Player)
			{
				DestroyFlareObjects(i);
				DestroyArenaFlareObjects(i);
			}

			TextDrawHideForAll(EN_CheckPoint);
			TextDrawHideForAll(CheckpointWarningBox);
		}
	}
	return 1;
}
