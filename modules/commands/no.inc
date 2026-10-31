#if defined _attdef_cmd_no
	#endinput
#endif
#define _attdef_cmd_no

YCMD: no(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "deny a duel request.");
	if (Player[playerid][InDuel])
		return SendErrorMessage(playerid, "You are in a duel anyway");

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

	Player[playerid][challengerid] = -1;
	Player[playerid][DuelRequestedByGeneration] = 0;

	SendDuelMessageToNonPlaying(COL_PRIM "\xBB Duel: "COL_SEC"%s "COL_PRIM"denied the duel challenge by "COL_SEC"%s", Player[playerid][Name],
		Player[pID][Name]);
	return 1;
}
