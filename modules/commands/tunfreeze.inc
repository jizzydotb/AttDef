#if defined _attdef_cmd_tunfreeze
	#endinput
#endif
#define _attdef_cmd_tunfreeze

YCMD: tunfreeze(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "unfreeze a player for 500ms.");
	new pID;
	if (sscanf(params, "r", pID))
		return SendUsageMessage(playerid, "/tunfreeze [Player Name/ID]");

	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isnt connected.");

	TogglePlayerControllable(pID, 1);
	new timer = SetTimerEx(
		"DelayedFreeze", 500, false, "iii", pID,
		PlayerConnectionGeneration[pID], ++DelayedFreezeGeneration[pID]);
	if (!timer)
	{
		TogglePlayerControllable(pID, 0);
		return SendErrorMessage(playerid, "Could not schedule the delayed freeze; the player remains frozen.");
	}

	LogAdminCommand("tunfreeze", playerid, pID);



	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has temporarily unfrozen "COL_SEC"%s"COL_PRIM" for 500ms.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name]);
}
