#if defined _attdef_cmd_vote
	#endinput
#endif
#define _attdef_cmd_vote

#define VOTE_TYPE_NONE 0
#define VOTE_TYPE_BASE 1
#define VOTE_TYPE_ARENA 2
#define VOTE_TYPE_AVA 3

#define MAX_VOTE_OPTIONS 10
#define VOTE_SESSION_DURATION 25

enum eVoteOptionData
{
	eVote_Type,
	eVote_MapID,
	eVote_Votes
};

new VoteOptions[MAX_VOTE_OPTIONS][eVoteOptionData];
new TotalVoteOptions = 0;
new PlayerVotedOption[MAX_PLAYERS] = { -1, ... };
new VoteTimer = 0;
new VoteTimerGeneration = 0;

forward CheckVoteStatus(timerGeneration);
forward ResetPlayerVotesOnDisconnect(playerid);
forward ResetVoteSession();

stock GetEligibleVoters()
{
	new count = 0;
	foreach (new i : Player)
	{
		if (CanPlay(i) && (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER || Player[i][Team] == ATTACKER_SUB || Player[i][Team] == DEFENDER_SUB))
			count++;
	}
	return count;
}

stock GetMajorityThreshold(eligible)
{
	if (eligible <= 1)
		return 1;
	if (eligible == 2)
		return 2;
	return (eligible / 2) + 1;
}

stock GetVoteOptionName(optIndex, output[], maxlen = sizeof(output))
{
	if (optIndex < 0 || optIndex >= TotalVoteOptions)
	{
		format(output, maxlen, "Unknown");
		return 0;
	}

	new type = VoteOptions[optIndex][eVote_Type];
	new id = VoteOptions[optIndex][eVote_MapID];

	switch (type)
	{
		case VOTE_TYPE_BASE:
			format(output, maxlen, "Base '%s' (ID: %d)", BName[id], id);
		case VOTE_TYPE_ARENA:
			format(output, maxlen, "Arena '%s' (ID: %d)", AName[id], id);
		case VOTE_TYPE_AVA:
			format(output, maxlen, "All-vs-All '%s' (ID: %d)", AName[id], id);
		default:
			format(output, maxlen, "Unknown");
	}
	return 1;
}

stock StartVoteTimer(durationSeconds)
{
	if (VoteTimer)
	{
		KillTimer(VoteTimer);
		VoteTimer = 0;
	}
	new gen = ++VoteTimerGeneration;
	VoteTimer = SetTimerEx("CheckVoteStatus", durationSeconds * 1000, false, "i", gen);
	return VoteTimer != 0;
}

public ResetVoteSession()
{
	if (VoteTimer)
	{
		KillTimer(VoteTimer);
		VoteTimer = 0;
	}
	VoteTimerGeneration++;
	TotalVoteOptions = 0;
	for (new i = 0; i < MAX_VOTE_OPTIONS; i++)
	{
		VoteOptions[i][eVote_Type] = 0;
		VoteOptions[i][eVote_MapID] = -1;
		VoteOptions[i][eVote_Votes] = 0;
	}
	foreach (new i : Player)
	{
		PlayerVotedOption[i] = -1;
	}
	return 1;
}

stock StartBaseNew(baseID)
{
	AllVsAllMode = false;
	GameType = BASE;
	AllowStartBase = false;
	ScheduleRoundStart(BASE, baseID, 2000);
	SendClientMessageToAll(-1, va_return("{93d8dc}\xBB (VOTE) {d0f5f7}Base '{93d8dc}%s{d0f5f7}' (ID: %d) has been voted to start!", BName[baseID], baseID));

	foreach (new i : Player)
	{
		if (CanPlay(i))
		{
			if ((Player[i][SpectatingType] == ARENA || Player[i][SpectatingType] == BASE) && (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER))
			{
				StopSpectate(i);
			}

			Player[i][gPauseWarnings] = 0;
			Player[i][gWasPaused] = 0;
			TogglePlayerControllable(i, 0);
			Player[i][ToAddInRound] = true;
			Player[i][gPauseCheck] = false;
		}
	}
}

stock StartArenaNew(arenaID)
{
	AllVsAllMode = false;
	AllowStartBase = false;
	GameType = ARENA;

	ScheduleRoundStart(ARENA, arenaID, 2000);
	SendClientMessageToAll(-1, va_return("{93d8dc}\xBB (VOTE) {d0f5f7}Arena '{93d8dc}%s{d0f5f7}' (ID: %d) has been voted to start!", AName[arenaID], arenaID));

	foreach (new i : Player)
	{
		if (CanPlay(i))
		{
			if ((Player[i][SpectatingType] == ARENA || Player[i][SpectatingType] == BASE) && (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER))
			{
				StopSpectate(i);
			}

			Player[i][gPauseWarnings] = 0;
			Player[i][gWasPaused] = 0;
			TogglePlayerControllable(i, 0);
			Player[i][ToAddInRound] = true;
			Player[i][gPauseCheck] = false;
		}
	}
}

stock StartAllVsAllNew(arenaID)
{
	AllowStartBase = false;
	AllVsAllMode = true;
	GameType = ARENA;

	ScheduleRoundStart(ARENA, arenaID, 2000);
	SendClientMessageToAll(-1, va_return("{93d8dc}\xBB (VOTE) {d0f5f7}All-vs-All in Arena '{93d8dc}%s{d0f5f7}' (ID: %d) has been voted to start!", AName[arenaID], arenaID));

	foreach (new i : Player)
	{
		if (CanPlay(i))
		{
			if ((Player[i][SpectatingType] == ARENA || Player[i][SpectatingType] == BASE) && (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER))
			{
				StopSpectate(i);
			}

			Player[i][gPauseWarnings] = 0;
			Player[i][gWasPaused] = 0;
			TogglePlayerControllable(i, 0);
			Player[i][ToAddInRound] = true;
			Player[i][gPauseCheck] = false;
		}
	}
}

stock ExecuteVoteWinner(optIndex)
{
	if (optIndex < 0 || optIndex >= TotalVoteOptions)
	{
		ResetVoteSession();
		return 0;
	}

	new type = VoteOptions[optIndex][eVote_Type];
	new id = VoteOptions[optIndex][eVote_MapID];

	ResetVoteSession();

	switch (type)
	{
		case VOTE_TYPE_BASE:
			StartBaseNew(id);
		case VOTE_TYPE_ARENA:
			StartArenaNew(id);
		case VOTE_TYPE_AVA:
			StartAllVsAllNew(id);
	}
	return 1;
}

public CheckVoteStatus(timerGeneration)
{
	if (VoteTimerGeneration != timerGeneration)
		return 0;

	VoteTimer = 0;
	if (TotalVoteOptions == 0)
		return 0;

	new maxVotes = 0;
	for (new i = 0; i < TotalVoteOptions; i++)
	{
		if (VoteOptions[i][eVote_Votes] > maxVotes)
			maxVotes = VoteOptions[i][eVote_Votes];
	}

	if (maxVotes <= 0)
	{
		SendClientMessageToAll(-1, "{93d8dc}\xBB (VOTE) {d0f5f7}Voting ended with insufficient votes.");
		ResetVoteSession();
		return 1;
	}

	new tiedOptions[MAX_VOTE_OPTIONS];
	new tiedCount = 0;
	for (new i = 0; i < TotalVoteOptions; i++)
	{
		if (VoteOptions[i][eVote_Votes] == maxVotes)
		{
			tiedOptions[tiedCount++] = i;
		}
	}

	new winnerOpt = (tiedCount == 1) ? tiedOptions[0] : tiedOptions[random(tiedCount)];

	if (tiedCount > 1)
	{
		SendClientMessageToAll(-1, "{93d8dc}\xBB (VOTE) {d0f5f7}Vote was tied! A winner was randomly chosen among the top options.");
	}

	ExecuteVoteWinner(winnerOpt);
	return 1;
}

public ResetPlayerVotesOnDisconnect(playerid)
{
	if (playerid >= 0 && playerid < MAX_PLAYERS)
	{
		new opt = PlayerVotedOption[playerid];
		if (opt >= 0 && opt < TotalVoteOptions)
		{
			if (VoteOptions[opt][eVote_Votes] > 0)
				VoteOptions[opt][eVote_Votes]--;
		}
		PlayerVotedOption[playerid] = -1;
	}
	return 1;
}

YCMD: vote(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "vote for starting a base, arena or all-vs-all.");
	if (Current != -1)
		return SendErrorMessage(playerid, "A round is in progress, please wait for it to end.");
	if (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER && Player[playerid][Team] != ATTACKER_SUB && Player[playerid][Team] != DEFENDER_SUB)
		return SendErrorMessage(playerid, "You must be either in attacker or defender team to vote.");
	if (WarMode)
		return SendErrorMessage(playerid, "Cannot do this when match mode is enabled.");

	new Params[2][64];
	sscanf(params, "s[64]S( )[64]", Params[0], Params[1]);

	new eligible = GetEligibleVoters();
	new majority = GetMajorityThreshold(eligible);

	if (isnull(Params[0]) || !strcmp(Params[0], "status", true) || !strcmp(Params[0], "list", true))
	{
		if (TotalVoteOptions == 0)
			return SendUsageMessage(playerid, "/vote [base | arena | ava] [optional ID]");

		SendClientMessage(playerid, -1, "{93d8dc}\xBB (VOTE) {d0f5f7}Current Voting Standings:");
		for (new i = 0; i < TotalVoteOptions; i++)
		{
			new optName[64];
			GetVoteOptionName(i, optName, sizeof(optName));
			va_SendClientMessage(playerid, -1, "{93d8dc}\xBB [%d] {d0f5f7}%s {93d8dc}- Votes: {d0f5f7}%d", i + 1, optName, VoteOptions[i][eVote_Votes]);
		}
		va_SendClientMessage(playerid, -1, "{93d8dc}\xBB {d0f5f7}%d votes {93d8dc}needed to win instantly. Use {d0f5f7}/vote [base/arena/ava] [id] {93d8dc}to cast/change vote.", majority);
		return 1;
	}

	new voteType = VOTE_TYPE_NONE;
	switch (YHash(Params[0], false))
	{
		case _I<base>, _I<b>:
			voteType = VOTE_TYPE_BASE;
		case _I<arena>, _I<a>:
			voteType = VOTE_TYPE_ARENA;
		case _I<allvsall>, _I<allvs>, _I<ava>, _I<all>:
			voteType = VOTE_TYPE_AVA;
		default:
			return SendUsageMessage(playerid, "/vote [base | arena | ava] [optional ID]");
	}

	new voteID = -1;
	if (!isnull(Params[1]) && IsNumeric(Params[1]))
	{
		voteID = strval(Params[1]);
	}
	else
	{
		// Check if an option of this type already exists to add vote to it
		for (new i = 0; i < TotalVoteOptions; i++)
		{
			if (VoteOptions[i][eVote_Type] == voteType)
			{
				voteID = VoteOptions[i][eVote_MapID];
				break;
			}
		}

		if (voteID == -1)
		{
			if (voteType == VOTE_TYPE_BASE)
				voteID = DetermineRandomRound(2, false, BASE);
			else
			{
				voteID = DetermineRandomRound(2, false, ARENA);
				if (voteID < 0)
					voteID = DetermineRandomRound(0, false, ARENA);
			}
		}
	}

	// Validation
	if (voteType == VOTE_TYPE_BASE)
	{
		if (voteID == -1)
			return SendErrorMessage(playerid, "No bases available.");
		if (voteID < 0 || voteID >= MAX_BASES || !Iter_Contains(CurrentBases, voteID))
			return SendErrorMessage(playerid, "Invalid base ID.");
	}
	else
	{
		if (voteID == -1)
			return SendErrorMessage(playerid, "No arenas available.");
		if (voteID < 0 || voteID >= MAX_ARENAS || !Iter_Contains(CurrentArenas, voteID))
			return SendErrorMessage(playerid, "Invalid arena ID.");
	}

	// Find or create option
	new optIndex = -1;
	for (new i = 0; i < TotalVoteOptions; i++)
	{
		if (VoteOptions[i][eVote_Type] == voteType && VoteOptions[i][eVote_MapID] == voteID)
		{
			optIndex = i;
			break;
		}
	}

	if (optIndex == -1)
	{
		if (TotalVoteOptions >= MAX_VOTE_OPTIONS)
			return SendErrorMessage(playerid, "Maximum voting options reached.");

		optIndex = TotalVoteOptions++;
		VoteOptions[optIndex][eVote_Type] = voteType;
		VoteOptions[optIndex][eVote_MapID] = voteID;
		VoteOptions[optIndex][eVote_Votes] = 0;
	}

	if (PlayerVotedOption[playerid] == optIndex)
		return SendErrorMessage(playerid, "You have already voted for this option! Use /vote to check standings.");

	new optName[64];
	GetVoteOptionName(optIndex, optName, sizeof(optName));

	new bool:isChange = (PlayerVotedOption[playerid] != -1);
	if (isChange)
	{
		new oldOpt = PlayerVotedOption[playerid];
		if (oldOpt >= 0 && oldOpt < TotalVoteOptions)
		{
			if (VoteOptions[oldOpt][eVote_Votes] > 0)
				VoteOptions[oldOpt][eVote_Votes]--;
		}
		VoteOptions[optIndex][eVote_Votes]++;
		PlayerVotedOption[playerid] = optIndex;

		va_SendClientMessageToAll(-1, "{93d8dc}\xBB (VOTE) {d0f5f7}%s {93d8dc}changed their vote to {d0f5f7}%s {93d8dc}(Votes: {d0f5f7}%d/%d {93d8dc}to win instantly)",
			Player[playerid][Name], optName, VoteOptions[optIndex][eVote_Votes], majority);
	}
	else
	{
		VoteOptions[optIndex][eVote_Votes]++;
		PlayerVotedOption[playerid] = optIndex;

		if (VoteTimer == 0)
		{
			if (!StartVoteTimer(VOTE_SESSION_DURATION))
			{
				ResetVoteSession();
				return SendErrorMessage(playerid, "Could not start the vote timer. Please try again.");
			}
			SendClientMessageToAll(-1, va_return("{93d8dc}\xBB (VOTE) {d0f5f7}Voting session started! {93d8dc}Time remaining: {d0f5f7}%ds. {93d8dc}Use {d0f5f7}/vote [base/arena/ava] [id]", VOTE_SESSION_DURATION));
		}

		va_SendClientMessageToAll(-1, "{93d8dc}\xBB (VOTE) {d0f5f7}%s {93d8dc}voted for {d0f5f7}%s {93d8dc}(Votes: {d0f5f7}%d/%d {93d8dc}to win instantly)",
			Player[playerid][Name], optName, VoteOptions[optIndex][eVote_Votes], majority);
	}

	// Check instant win
	if (VoteOptions[optIndex][eVote_Votes] >= majority)
	{
		SendClientMessageToAll(-1, va_return("{93d8dc}\xBB (VOTE) {d0f5f7}%s {93d8dc}has reached the required majority (%d votes) to win!", optName, majority));
		ExecuteVoteWinner(optIndex);
	}

	return 1;
}
