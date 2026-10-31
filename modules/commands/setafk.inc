#if defined _attdef_cmd_setafk
	#endinput
#endif
#define _attdef_cmd_setafk

YCMD: setafk(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "force someone to go into AFK mode.");
	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/setafk [Player Name/ID]");

	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player is not connected.");
	if (Player[pID][Playing])
		return SendErrorMessage(playerid, "You cannot switch this player to AFK mode as he's playing");
	if (Player[pID][InDuel])
		return SendErrorMessage(playerid, "That player is in a duel");
	if (Player[pID][Spectating])
		StopSpectate(pID);
	if (Player[pID][InDM])
		QuitDM(pID);


	Player[pID][Team] = NON;
	SetPlayerColor(pID, 0xAAAAAAAA);
	TogglePlayerControllable(pID, 0);
	Player[pID][IsAFK] = true;

	LogAdminCommand("setafk", playerid, pID);
	UpdatePlayerHealthBar(pID);

	// Adjusted messaging
	if (pID == playerid) // The command is executed by the player changing their own level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set himself to AFK mode.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	}
	else // The command is executed by an admin changing another player's level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has set "COL_SEC"%s " COL_PRIM "to AFK mode.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name]);
	}

}
