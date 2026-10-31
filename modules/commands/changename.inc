#if defined _attdef_cmd_changename
	#endinput
#endif
#define _attdef_cmd_changename

YCMD: changename(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "change your user account name.");
	if (!Player[playerid][Logged])
		return SendErrorMessage(playerid, "You must be logged in.");
	if (Player[playerid][Mute])
		return SendErrorMessage(playerid, "Cannot use this command when you're muted.");
	if (isnull(params))
		return SendUsageMessage(playerid, "/changename [New Name]");
	if (!IsValidNickName(params))
		return SendErrorMessage(playerid, "Invalid nickname!");

	switch (SetPlayerName(playerid, params))
	{
		case 0: return SendErrorMessage(playerid, "You're already using that name.");
		case -1: return SendErrorMessage(playerid, "Either Name is too long, already in use or has invalid characters.");
		default:
		{
			//success
			new iString[144];
			format(iString, sizeof(iString), "SELECT * FROM `Players` WHERE `Name` = '%q'", params);
			new DBResult: result = DB_ExecuteQuery(sqliteconnection, iString);
			if (!result)
			{
				SetPlayerName(playerid, Player[playerid][Name]);
				printf("[AttDef] Failed to check whether player %d's requested name is already registered.", playerid);
				return SendErrorMessage(playerid, "Could not verify that name right now; your name was not changed.");
			}
			new num_rows		  = DB_GetRowCount(result);
			DB_FreeResultSet(result);

			if (num_rows > 0) //name in Use in DB.
			{
				SetPlayerName(playerid, Player[playerid][Name]);
				return SendErrorMessage(playerid, "Name already registered!");
			}

			format(iString, sizeof(iString), "UPDATE `Players` SET `Name` = '%q' WHERE `Name` = '%q'", params, Player[playerid][Name]);
			if (!DatabaseExecuteWrite(iString, "save player name change"))
			{
				SetPlayerName(playerid, Player[playerid][Name]);
				return SendErrorMessage(playerid, "Could not save your new name; it was not changed.");
			}

			format(iString, sizeof(iString), COL_PRIM "\xBB "COL_SEC"%s " COL_PRIM "has changed name to "COL_SEC"%s", Player[playerid][Name], params);
			SendClientMessageToAll(-1, iString);
			format(Player[playerid][Name], MAX_PLAYER_NAME, "%s", params);

			new NewName[MAX_PLAYER_NAME];
			NewName = RemoveClanTagFromName(playerid);

			if (strlen(NewName) != 0)
				Player[playerid][NameWithoutTag] = NewName;
			else
				Player[playerid][NameWithoutTag] = Player[playerid][Name];
			return 1;
		}
	}
	return 1; //not needed but compiler complains so yeah..
}
