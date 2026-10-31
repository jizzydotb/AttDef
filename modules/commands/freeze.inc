#if defined _attdef_cmd_freeze
	#endinput
#endif
#define _attdef_cmd_freeze

YCMD: freeze(playerid, params[], help)
{
	//if(Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be a higher admin level to do that.");
	if (help)
		return SendCommandHelpMessage(playerid, "freeze a player.");
	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/freeze [Player Name/ID]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isnt connected.");

	TogglePlayerControllable(pID, 0);

	LogAdminCommand("freeze", playerid, pID);



	// Adjusted messaging
	if (pID == playerid) // The command is executed by the player changing their own level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"froze in place.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	}
	else // The command is executed by an admin changing another player's level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has frozen "COL_SEC"%s"COL_PRIM".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name]);
	}

}

forward DelayedFreeze(pID, connectionGeneration, freezeGeneration);
public DelayedFreeze(pID, connectionGeneration, freezeGeneration)
{
	if (pID < 0 || pID >= MAX_PLAYERS || !IsPlayerConnected(pID)
		|| PlayerConnectionGeneration[pID] != connectionGeneration
		|| DelayedFreezeGeneration[pID] != freezeGeneration)
		return 0;

	TogglePlayerControllable(pID, 0);
	return 1;
}
