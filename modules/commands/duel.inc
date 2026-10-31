#if defined _attdef_cmd_duel
	#endinput
#endif
#define _attdef_cmd_duel

YCMD: duel(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "send a duel request to a specific player.");
	if (Player[playerid][InDuel])
		return SendErrorMessage(playerid, "You are already dueling someone.");

	new invitedid, Weapon1[23], Weapon2[23], modeText[8], arenaid;
	if (sscanf(params, "rs[*]s[*]S(9)[8]I(-1)", invitedid, sizeof(Weapon1), Weapon1, sizeof(Weapon2), Weapon2, modeText, arenaid))
	{
		SendUsageMessage(playerid, "/duel [Player Name/ID] [Weapon 1] [Weapon 2] [Optional: target|bo5] [Optional: arena ID]");
		return SendClientMessage(playerid, -1, "" COL_PRIM "\xBB "COL_THR"Default is first to 9 in a random loaded arena. Use bo5 for first to 3.");
	}
	if (invitedid == INVALID_PLAYER_ID || !IsPlayerConnected(invitedid))
		return SendErrorMessage(playerid, "That player isn't connected.");
	if (Player[invitedid][Playing])
		return SendErrorMessage(playerid, "That player is in a round.");
	if (Player[playerid][Playing])
		return SendErrorMessage(playerid, "You can't duel while being in a round.");
	if (Player[playerid][InDM])
		return SendErrorMessage(playerid, "Leave the deathmatch before challenging someone to a duel.");
	if (Player[invitedid][InDM])
		return SendErrorMessage(playerid, "That player must leave the deathmatch before dueling.");
	if (Player[invitedid][InDuel])
		return SendErrorMessage(playerid, "That player is already dueling someone.");
	if (Player[invitedid][challengerid] != -1)
		return SendErrorMessage(playerid, "That player already has a pending duel request.");
	if (invitedid == playerid)
		return SendErrorMessage(playerid, "Can't duel with yourself.");

	if (Iter_IsEmpty(CurrentArenas))
		return SendErrorMessage(playerid, "There are no loaded arenas available for duels.");
	if (arenaid != -1 && !Iter_Contains(CurrentArenas, arenaid))
		return SendErrorMessage(playerid, "That arena ID is not loaded.");

	new WeaponID1 = GetWeaponID(Weapon1);
	if (!IsValidWeapon(WeaponID1))
		return SendErrorMessage(playerid, "Invalid Weapon Name.");

	new WeaponID2 = GetWeaponID(Weapon2);
	if (!IsValidWeapon(WeaponID2))
		return SendErrorMessage(playerid, "That weapon is not allowed in duels.");

	new target, mode = DUEL_MODE_FIRST_TO;
	if (!strcmp(modeText, "bo5", true) || !strcmp(modeText, "best5", true))
	{
		mode = DUEL_MODE_BEST_OF_FIVE;
		target = 3;
	}
	else
	{
		if (sscanf(modeText, "i", target) || target < 1 || target > DUEL_MAX_TARGET)
			return SendErrorMessage(playerid, "Choose a target from 1 to 99, or use bo5.");
	}
	return ProcessDuelRequest(playerid, invitedid, WeaponID1, WeaponID2, arenaid, target, mode);
}
