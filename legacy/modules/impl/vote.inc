#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_vote
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_vote
	#endinput
#endif
#define _attdef_impl_vote

/*
	Vote system
*/

forward ResetVoting(votetype, bool: message);
public ResetVoting(votetype, bool: message)
{
	if (votetype < 0 || votetype >= MAX_VOTE_TYPES)
		return 0;

	CancelVoteExpiration(votetype);
	if (votetype == VOTE_TYPE_NETCHECK)
	{
		new onplayerid					 = VoteData[votetype][VotingOnPlayer];
		if (onplayerid >= 0 && onplayerid < MAX_PLAYERS && IsPlayerConnected(onplayerid)
			&& PlayerConnectionGeneration[onplayerid] == VoteData[votetype][VotingOnPlayerGeneration])
		{
			Player[onplayerid][TempNetcheck] = false;
			TempNetcheckGeneration[onplayerid]++;
		}
		VoteData[votetype][VotingOnPlayer] = INVALID_PLAYER_ID;
		VoteData[votetype][VotingOnPlayerGeneration] = 0;
	}
	VoteData[votetype][VoteCount] = 0;
	foreach (new i : Player)
	{
		VoteData[votetype][PlayerVoted][i] = false;
	}
	if (message)
		SendClientMessageToAll(
			-1, va_return(""COL_PRIM"\xBB %s-voting " COL_PRIM "has been reset due to an error or vote timeout "COL_SEC"(%d secs)!", VoteTypeNames[votetype], VOTE_TIMEOUT_INTERVAL / 1000));
	return 1;
}

forward ResetVotingTimed(votetype, bool: message, generation);
public ResetVotingTimed(votetype, bool: message, generation)
{
	if (votetype < 0 || votetype >= MAX_VOTE_TYPES
		|| VoteExpirationGeneration[votetype] != generation)
		return 0;

	return ResetVoting(votetype, message);
}

ScheduleVoteExpiration(votetype)
{
	if (votetype < 0 || votetype >= MAX_VOTE_TYPES)
		return 0;

	CancelVoteExpiration(votetype);
	VoteData[votetype][VotingExpireTimer] = SetTimerEx(
		"ResetVotingTimed", VOTE_TIMEOUT_INTERVAL, false, "ibi",
		votetype, true, VoteExpirationGeneration[votetype]);
	if (!VoteData[votetype][VotingExpireTimer])
	{
		VoteExpirationGeneration[votetype]++;
		printf("[AttDef] Failed to schedule expiration for vote type %d.", votetype);
		return 0;
	}
	return 1;
}

CancelVoteExpiration(votetype)
{
	if (votetype < 0 || votetype >= MAX_VOTE_TYPES)
		return 0;

	if (VoteData[votetype][VotingExpireTimer])
		KillTimer(VoteData[votetype][VotingExpireTimer]);
	VoteData[votetype][VotingExpireTimer] = 0;
	VoteExpirationGeneration[votetype]++;
	return 1;
}

forward ResetPlayerActiveVotes(playerid);
public ResetPlayerActiveVotes(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	for (new votetype = 0; votetype < MAX_VOTE_TYPES; votetype++)
	{
		if (VoteData[votetype][VoteCount] > 0 && VoteData[votetype][VotingOnPlayer] == playerid)
		{
			ResetVoting(votetype, false);
			continue;
		}

		if (!VoteData[votetype][PlayerVoted][playerid])
			continue;

		VoteData[votetype][PlayerVoted][playerid] = false;
		if (VoteData[votetype][VoteCount] > 0)
			VoteData[votetype][VoteCount]--;
		if (VoteData[votetype][VoteCount] == 0)
			ResetVoting(votetype, false);
	}
	return 1;
}

InitPlayerVoting(playerid)
{
	for (new i = 0; i < MAX_VOTE_TYPES; i++) { VoteData[i][PlayerVoted][playerid] = false; }
}

InitVoting()
{
	for (new i = 0; i < MAX_VOTE_TYPES; i++) { VoteData[i][VoteCount] = 0; }
}
