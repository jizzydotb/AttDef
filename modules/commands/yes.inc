#if defined _attdef_cmd_yes
	#endinput
#endif
#define _attdef_cmd_yes

YCMD: yes(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "accept a duel request.");
	if (Player[playerid][InDuel])
		return SendErrorMessage(playerid, "You are already in a duel.");

	new pID = Player[playerid][challengerid];
	if (Player[playerid][challengerid] == -1)
		return SendErrorMessage(playerid, "No one has invited you to a duel.");
	if (pID < 0 || pID >= MAX_PLAYERS || !IsPlayerConnected(pID)
		|| PlayerConnectionGeneration[pID] != Player[playerid][DuelRequestedByGeneration])
	{
		Player[playerid][challengerid] = -1;
		Player[playerid][DuelRequestedByGeneration] = 0;
		return SendErrorMessage(playerid, "That player isn't connected.");
	}
	if (Player[pID][Playing])
	{
		Player[playerid][challengerid] = -1;
		Player[playerid][DuelRequestedByGeneration] = 0;
		return SendErrorMessage(playerid, "That player is in a round.");
	}
	if (Player[pID][InDM])
	{
		Player[playerid][challengerid] = -1;
		Player[playerid][DuelRequestedByGeneration] = 0;
		return SendErrorMessage(playerid, "That player is in a deathmatch.");
	}
	if (Player[pID][InDuel])
	{
		Player[playerid][challengerid] = -1;
		Player[playerid][DuelRequestedByGeneration] = 0;
		return SendErrorMessage(playerid, "That player is already dueling someone else.");
	}
	if (Player[playerid][Playing])
	{
		Player[playerid][challengerid] = -1;
		Player[playerid][DuelRequestedByGeneration] = 0;
		return SendErrorMessage(playerid, "You can't duel while being in a round.");
	}
	if (Player[playerid][InDM])
	{
		Player[playerid][challengerid] = -1;
		Player[playerid][DuelRequestedByGeneration] = 0;
		return SendErrorMessage(playerid, "Leave the deathmatch before accepting a duel.");
	}

	new arenaid = Player[playerid][DuelRequestedArena];
	if (arenaid == -1)
	{
		if (Iter_IsEmpty(CurrentArenas))
		{
			Player[playerid][challengerid] = -1;
			Player[playerid][DuelRequestedByGeneration] = 0;
			return SendErrorMessage(playerid, "There are no loaded arenas available for duels.");
		}
		arenaid = Iter_Random(CurrentArenas);
	}
	else if (!Iter_Contains(CurrentArenas, arenaid))
	{
		Player[playerid][challengerid] = -1;
		Player[playerid][DuelRequestedByGeneration] = 0;
		return SendErrorMessage(playerid, "The selected duel arena is no longer available.");
	}

	StartDuel(
		pID, playerid, Player[playerid][duelweap1], Player[playerid][duelweap2], arenaid, Player[playerid][DuelTarget],
		Player[playerid][DuelMode]);

	return SendDuelMessageToNonPlaying(COL_PRIM "\xBB Duel: "COL_SEC"%s "COL_PRIM"accepted the duel challenge by "COL_SEC"%s", Player[playerid][Name],
		Player[pID][Name]);
}
