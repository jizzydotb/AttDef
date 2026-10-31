#if defined _attdef_cmd_voteunpause
	#endinput
#endif
#define _attdef_cmd_voteunpause

YCMD: voteunpause(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "start a voting (pause round)");
	return PlayerVoteUnpause(playerid);
}

PlayerVoteUnpause(playerid)
{
	if (Current == -1)
		return SendErrorMessage(playerid, "There's no round in progress!");
	if (!Player[playerid][Playing])
		return SendErrorMessage(playerid, "You must be playing to be able to vote on this!");

	new votetype = VOTE_TYPE_UNPAUSE;
	if (VoteData[votetype][PlayerVoted][playerid])
		return SendErrorMessage(playerid, "You've already voted!");
	if (!RoundPaused)
		return SendErrorMessage(playerid, "Round is already running and is not paused.");

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
					""COL_PRIM"\xBB "COL_SEC"%s" COL_PRIM " has started "COL_SEC"unpause-voting " COL_PRIM "[%d/%d] [/voteunpause or press Y]", Player[playerid][Name],
					VoteData[votetype][VoteCount], MaxVotesNeeded));
	}
	else
	{
		VoteData[votetype][VoteCount]++;
		VoteData[votetype][PlayerVoted][playerid] = true;
		SendClientMessageToAll(
			-1, va_return(
					""COL_PRIM"\xBB "COL_SEC"%s" COL_PRIM " has added to "COL_SEC"unpause-voting " COL_PRIM "[%d/%d] [/voteunpause or press Y]", Player[playerid][Name],
					VoteData[votetype][VoteCount], MaxVotesNeeded));
		if (VoteData[votetype][VoteCount] >= MaxVotesNeeded) // Enough people voted!
		{
			ResetVoting(votetype, false);
			if (RoundUnpausing)
				return SendClientMessageToAll(-1, "" COL_PRIM "\xBB Unpause-voting result: "COL_SEC"round is already being unpaused!");

			PauseCountdown = 4;
			UnpauseRound();
			SendClientMessageToAll(-1, "" COL_PRIM "\xBB Unpause-voting result: "COL_SEC"round has been unpaused!");
		}
	}
	return 1;
}
