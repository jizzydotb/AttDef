#if defined _attdef_cmd_voteendmatch
	#endinput
#endif
#define _attdef_cmd_voteendmatch

YCMD: voteendmatch(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "start a voting (end match)");
	return PlayerVoteEndMatch(playerid);
}

PlayerVoteEndMatch(playerid)
{
	if (!WarMode)
		return SendErrorMessage(playerid, "Match mode is already disabled!");
	if (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER)
		return SendErrorMessage(playerid, "You must be part of one of the following teams: Attacker, Defender.");

	new votetype = VOTE_TYPE_ENDMATCH;
	if (VoteData[votetype][PlayerVoted][playerid])
		return SendErrorMessage(playerid, "You've already voted!");

	// Counting players who are able to vote
	new countInTeam[2];
	foreach (new i : Player)
	{
		switch (Player[i][Team])
		{
			case ATTACKER: countInTeam[0]++;
			case DEFENDER: countInTeam[1]++;
		}
	}
	if (countInTeam[0] == 0 || countInTeam[1] == 0)
		return SendErrorMessage(playerid, "One team has no active players, so we cannot count this vote.");

	new MaxVotesNeeded = countInTeam[0] + countInTeam[1];
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
					""COL_SEC"\xBB %s" COL_PRIM " has started "COL_SEC"End Match-voting " COL_PRIM "[%d/%d] [/voteendmatch]", Player[playerid][Name], VoteData[votetype][VoteCount],
					MaxVotesNeeded));
	}
	else
	{
		VoteData[votetype][VoteCount]++;
		VoteData[votetype][PlayerVoted][playerid] = true;
		SendClientMessageToAll(
			-1, va_return(
					""COL_PRIM"\xBB "COL_SEC"%s" COL_PRIM " has added to "COL_SEC"End Match-voting " COL_PRIM "[%d/%d] [/voteendmatch]", Player[playerid][Name], VoteData[votetype][VoteCount],
					MaxVotesNeeded));
		if (VoteData[votetype][VoteCount] >= MaxVotesNeeded) // Enough people voted!
		{
			SendClientMessageToAll(-1, "" COL_PRIM "\xBB End Match-voting result: "COL_SEC"match has been set to end!");
			ResetVoting(votetype, false);
			if (Current != -1)
			{
				ResetPauseTimers();
				Current = -1;
				if (RoundPaused)
					TextDrawHideForAll(PauseTD);

				RoundPaused			   = false;
				TeamCapturingCP		   = NON;
				PlayersInCP			   = 0;

				PlayersAlive[ATTACKER] = 0;
				PlayersAlive[DEFENDER] = 0;

				PlayersDead[ATTACKER]  = 0;
				PlayersDead[DEFENDER]  = 0;

				RoundUnpausing		   = false;

				foreach (new i : Player)
				{
					if (Player[i][InDuel] || Player[i][IsAFK] || !Player[i][Spawned])
						continue;

					SetPlayerRoundParticipation(i, false);
					Player[i][WasInCP] = false;
					if (Player[i][Spectating])
						StopSpectate(i);
					Player[i][WasInBase]	= false;
					Player[i][WasInTeam]	= NON;
					Player[i][WeaponPicked] = 0;
					Player[i][TimesSpawned] = 0;

					TogglePlayerControllable(i, 0);
					RemovePlayerMapIcon(i, 59);

					SpawnPlayer(i);

					DisablePlayerCheckpoint(i);
					SetPlayerScore(i, 0);
					HidePlayerDialog(i);

					PlayerTextDrawHide(i, AreaCheckTD[i]);
					PlayerTextDrawHide(i, AreaCheckBG[i]);
				}

				foreach (new i : Player)
				{
					HideRoundStats(i);
				}
				TextDrawHideForAll(EN_CheckPoint);
				TextDrawHideForAll(CheckpointWarningBox);

				ResetGunmenuSelections();

				BaseStarted	 = false;
				ArenaStarted = false;

				SendRconCommand("game.map Lobby");
				FixGamemodeText();
				DeleteAllTeamBars();
				DeleteAllDeadBodies();

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

				ResetTeamLeaders();
				Iter_Clear(PlayersInRound);
			}
			ScheduleWarEnd(4000);
		}
	}
	return 1;
}
