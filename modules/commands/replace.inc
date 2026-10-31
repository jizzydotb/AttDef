#if defined _attdef_cmd_replace
	#endinput
#endif
#define _attdef_cmd_replace

YCMD: replace(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "replace players whether they're connected to the server or not.");
	if (Current == -1)
		return SendErrorMessage(playerid, "Round is not active.");

	REPLACE_RequestGeneration[playerid]++;
	REPLACE_RequestRound[playerid] = Current;
	LogAdminCommand("replace", playerid, INVALID_PLAYER_ID);

	foreach (new i : Player)
	{
		if (Player[i][InDuel] || Player[i][IsAFK] || !Player[i][Spawned])
			continue;

		DynDialog_AddItem(playerid, i, Player[i][Name]);
	}
	DynDialog_Show(playerid, using public ReplaceFirstDialog<iiis>, DIALOG_STYLE_LIST, COL_PRIM "Player to add", "Process", "Cancel", 15);
	return 1;
}

hook ReplaceFirstDialog(playerid, response, item_value, string: inputtext[])
{
	if (!response)
		return 1;
	if (item_value < 0 || item_value >= MAX_PLAYERS || !IsPlayerConnected(item_value))
		return SendErrorMessage(playerid, "The selected player is no longer connected.");
	
	REPLACE_ToAddID[playerid] = item_value;
	REPLACE_ToAddGeneration[playerid] = PlayerConnectionGeneration[item_value];
	REPLACE_RequestRound[playerid] = Current;
	REPLACE_RequestGeneration[playerid]++;
	foreach (new i : Player)
	{
		if (!Player[i][Playing])
			continue;

		DynDialog_AddItem(playerid, i, Player[i][Name]); // item_value = playerid = [0, MAX_PLAYERS - 1] for online players
	}
	for (new i = 0; i < SAVE_SLOTS; i++)
	{
		if (SaveVariables[i][pName][0] && SaveVariables[i][RoundID] == Current && SaveVariables[i][ToBeAdded] == true)
		{
			DynDialog_AddItem(playerid, -i - 1, SaveVariables[i][pName]); // item_value = -slot - 1 = [-SAVE_SLOTS, -1] for offline players
		}
	}
	DynDialog_Show(playerid, using public ReplaceSecondDialog<iiis>, DIALOG_STYLE_LIST, COL_PRIM "Player to replace", "Process", "Cancel", 15);
	return 1;
}

hook ReplaceSecondDialog(playerid, response, item_value, string: inputtext[])
{
	if (!response)
		return 1;
	new ToReplaceID = item_value;
	if (ToReplaceID >= 0)
	{
		new ToAddID = REPLACE_ToAddID[playerid];
		if (!IsPlayerConnected(ToAddID)
			|| PlayerConnectionGeneration[ToAddID] != REPLACE_ToAddGeneration[playerid]
			|| Current != REPLACE_RequestRound[playerid])
			return SendErrorMessage(playerid, "The replacement selection is no longer valid.");

		if (Player[ToAddID][InDuel] == true)
			return SendErrorMessage(playerid, "That player is in a duel."); //duel

		if (!SetTimerEx(
			"OnPlayerReplace", 500, false, "iiiiiiii", ToAddID, ToReplaceID, playerid,
			PlayerConnectionGeneration[ToAddID], PlayerConnectionGeneration[ToReplaceID],
			PlayerConnectionGeneration[playerid], REPLACE_RequestGeneration[playerid], REPLACE_RequestRound[playerid]))
			return SendErrorMessage(playerid, "Could not schedule the replacement. Please try again.");
	}
	else
	{
		ToReplaceID = -ToReplaceID - 1; // reversing the transformation we did to the item_value
		new ToAddID = REPLACE_ToAddID[playerid];
		if (!IsPlayerConnected(ToAddID)
			|| PlayerConnectionGeneration[ToAddID] != REPLACE_ToAddGeneration[playerid]
			|| Current != REPLACE_RequestRound[playerid])
			return SendErrorMessage(playerid, "The replacement selection is no longer valid.");

		if (Player[ToAddID][InDuel] == true)
			return SendErrorMessage(playerid, "That player is in a duel."); //duel
		if (!SetTimerEx(
			"OnPlayerInGameReplace", 500, false, "iiiiiii", ToAddID, ToReplaceID, playerid,
			PlayerConnectionGeneration[ToAddID], PlayerConnectionGeneration[playerid],
			REPLACE_RequestGeneration[playerid], REPLACE_RequestRound[playerid]))
			return SendErrorMessage(playerid, "Could not schedule the replacement. Please try again.");
	}
	return 1;
}
