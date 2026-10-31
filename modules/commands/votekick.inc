#if defined _attdef_cmd_votekick
	#endinput
#endif
#define _attdef_cmd_votekick

YCMD: votekick(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "start a voting (kick a player from the server)");
	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/votekick [Player Name/ID]");

	return PlayerVoteKick(playerid, pID);
}

PlayerVoteKick(playerid, onplayerid)
{
	if (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER)
		return SendErrorMessage(playerid, "You must be part of one of the following teams: Attacker, Defender.");

	new votetype = VOTE_TYPE_KICK;
	if (VoteData[votetype][PlayerVoted][playerid])
		return SendErrorMessage(playerid, "You've already voted!");
	if (VoteData[votetype][VoteCount] > 0 && VoteData[votetype][VotingOnPlayer] != onplayerid)
		return SendErrorMessage(playerid, va_return("Another kick-voting is in progress (on playerid: %d), please wait!", VoteData[votetype][VotingOnPlayer]));

	if (onplayerid < 0 || onplayerid >= MAX_PLAYERS || !IsPlayerConnected(onplayerid))
		return SendErrorMessage(playerid, "That player is not connected.");
	if (playerid == onplayerid)
		return SendErrorMessage(playerid, "You cannot vote to kick yourself.");

	// Counting players who are able to vote
	new countInTeam[2];
	foreach (new i : Player)
	{
		if (i == onplayerid)
			continue;

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
		VoteData[votetype][VotingOnPlayer]	  = onplayerid;
		foreach (new i : Player)
		{
			VoteData[votetype][PlayerVoted][i] = false;
			PlayerPlaySound(i, 1133, 0.0, 0.0, 0.0);
		}
		VoteData[votetype][PlayerVoted][playerid] = true;
		SendClientMessageToAll(
			-1, va_return(
					""COL_PRIM"\xBB "COL_SEC"%s" COL_PRIM " has started "COL_SEC"kick-voting " COL_PRIM "on "COL_SEC"%s " COL_PRIM "[%d/%d] [/votekick %d]", Player[playerid][Name],
					Player[onplayerid][Name], VoteData[votetype][VoteCount], MaxVotesNeeded, onplayerid));
	}
	else
	{
		VoteData[votetype][VoteCount]++;
		VoteData[votetype][PlayerVoted][playerid] = true;
		SendClientMessageToAll(
			-1, va_return(
					""COL_PRIM"\xBB "COL_SEC"%s" COL_PRIM " has added to "COL_SEC"kick-voting " COL_PRIM "on "COL_SEC"%s " COL_PRIM "[%d/%d] [/votekick %d]", Player[playerid][Name],
					Player[onplayerid][Name], VoteData[votetype][VoteCount], MaxVotesNeeded, onplayerid));
		if (VoteData[votetype][VoteCount] >= MaxVotesNeeded) // Enough people voted!
		{
			SendClientMessageToAll(-1, va_return("" COL_PRIM "\xBB Voting result on "COL_SEC"%s: " COL_PRIM "kicked from the server!", Player[onplayerid][Name]));
			ResetVoting(votetype, false);
			SchedulePlayerKick(onplayerid, GetPlayerPing(onplayerid) + 10);
		}
	}
	return 1;
}
