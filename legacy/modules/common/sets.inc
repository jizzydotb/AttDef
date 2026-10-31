#if defined _attdef_common_sets
	#endinput
#endif
#define _attdef_common_sets

/*
	Base & Arena Sets Configuration & Loading Dialogs Sub-module
*/

public UpdateBaseIDTextDraw(playerid)
{
	if (Current == -1 || (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER)
		|| Player[playerid][InDM] || Player[playerid][InDuel])
	{
		PlayerTextDrawHide(playerid, BaseID_VS[playerid]);
		return 1;
	}

	if (Player[playerid][RoundTDStyle] == 4 && !AllVsAllMode && RoundTeamHPHUDReady[playerid])
	{
		PlayerTextDrawHide(playerid, BaseID_VS[playerid]);
		return 1;
	}

	new iString[32];

	if (Player[playerid][RoundTDStyle] == 3)
	{
		if (GameType == BASE)
			format(iString, sizeof(iString), "~p~AAD~w~ %d", Current);
		else
			format(iString, sizeof(iString), "~p~Arena~w~ %d", Current);
		PlayerTextDrawFont(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawLetterSize(playerid, BaseID_VS[playerid], 0.310000, 1.500000);
		PlayerTextDrawAlignment(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawColor(playerid, BaseID_VS[playerid], -1);
		PlayerTextDrawSetProportional(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawTextSize(playerid, BaseID_VS[playerid], 1280.0, 1280.0);
		PlayerTextDrawBackgroundColor(playerid, BaseID_VS[playerid], 0x000000FF);
		PlayerTextDrawSetSelectable(playerid, BaseID_VS[playerid], 0);
		PlayerTextDrawSetOutline(playerid, BaseID_VS[playerid], 0);
		PlayerTextDrawSetShadow(playerid, BaseID_VS[playerid], 1);
	}
	else if (Player[playerid][RoundTDStyle] == 0) // Modern
	{
		if (GameType == BASE)
			format(iString, sizeof(iString), "Base %d", Current);
		else
			format(iString, sizeof(iString), "Arena %d", Current);
		PlayerTextDrawFont(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawLetterSize(playerid, BaseID_VS[playerid], 0.2251, 1.2883);
		PlayerTextDrawAlignment(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawColor(playerid, BaseID_VS[playerid], -1);
		PlayerTextDrawSetProportional(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawTextSize(playerid, BaseID_VS[playerid], 930.366, 930.000);
		PlayerTextDrawBackgroundColor(playerid, BaseID_VS[playerid], 0x000000FF);
		PlayerTextDrawSetSelectable(playerid, BaseID_VS[playerid], 0);
		PlayerTextDrawSetOutline(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawSetShadow(playerid, BaseID_VS[playerid], 0);
	}
	else if (Player[playerid][RoundTDStyle] == 2) // Simple
	{
		if (GameType == BASE)
			format(iString, sizeof(iString), "Base %d", Current);
		else
			format(iString, sizeof(iString), "Arena %d", Current);
		PlayerTextDrawFont(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawLetterSize(playerid, BaseID_VS[playerid], 0.2251, 1.2883);
		PlayerTextDrawAlignment(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawColor(playerid, BaseID_VS[playerid], -1);
		PlayerTextDrawSetProportional(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawTextSize(playerid, BaseID_VS[playerid], 930.366, 930.000);
		PlayerTextDrawBackgroundColor(playerid, BaseID_VS[playerid], 0x000000FF);
		PlayerTextDrawSetSelectable(playerid, BaseID_VS[playerid], 0);
		PlayerTextDrawSetOutline(playerid, BaseID_VS[playerid], 0);
		PlayerTextDrawSetShadow(playerid, BaseID_VS[playerid], 1);
	}
	else if (GameType == BASE)
	{
		format(iString, sizeof(iString), "%sBase %s(~r~%d%s)", MAIN_TEXT_COLOUR, MAIN_TEXT_COLOUR, Current, MAIN_TEXT_COLOUR);
		PlayerTextDrawLetterSize(playerid, BaseID_VS[playerid], 0.26000, 1.500000);
		PlayerTextDrawColor(playerid, BaseID_VS[playerid], -65281);
		PlayerTextDrawSetOutline(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawSetShadow(playerid, BaseID_VS[playerid], 0);
	}
	else if (GameType == ARENA)
	{
		format(iString, sizeof(iString), "%sArena %s(~r~%d%s)", MAIN_TEXT_COLOUR, MAIN_TEXT_COLOUR, Current, MAIN_TEXT_COLOUR);
		PlayerTextDrawLetterSize(playerid, BaseID_VS[playerid], 0.26000, 1.500000);
		PlayerTextDrawColor(playerid, BaseID_VS[playerid], -65281);
		PlayerTextDrawSetOutline(playerid, BaseID_VS[playerid], 1);
		PlayerTextDrawSetShadow(playerid, BaseID_VS[playerid], 0);
	}
	else
	{
		FixVsTextDraw(playerid);
	}

	PlayerTextDrawSetString(playerid, BaseID_VS[playerid], iString);
	PlayerTextDrawShow(playerid, BaseID_VS[playerid]);
	return 1;
}

forward ShowBaseSetsDialog(playerid);
public ShowBaseSetsDialog(playerid)
{
	new string[2048];
	new tempString[256];
	new baseSets[13][120] = {
		"Wargrounds",
		"OldBulletproofBases",
		"DeadlyCombinations",
		"NoCbugBases",
		"Slaughterfield",
		"NewBulletproofBases",
		"cYsBulletproofBases",
		"AttdefBases",
		"TournamentsBases",
		"BattleGrounds",
		"MrSOldBases",
		"ESL",
		"HazeBases"
	};

	format(string, sizeof(string), "ID\tName\tBases\n");

	for (new i = 0; i < sizeof(baseSets); i++)
	{
		new baseCount = GetBaseCount(baseSets[i]);
		if (!strcmp(baseSets[i], CurrentBaseSet, true))
		{
			format(tempString, sizeof(tempString), "%d\t{AADC93}%s (Active)\t{AADC93}%d Bases\n", i, baseSets[i], baseCount);
		}
		else
		{
			format(tempString, sizeof(tempString), "%d\t{FFFFFF}%s\t%d Bases\n", i, baseSets[i], baseCount);
		}
		strcat(string, tempString, sizeof(string));
	}

	ShowPlayerDialog(playerid, DIALOG_LOAD_BASES, DIALOG_STYLE_TABLIST_HEADERS, "Select Base Pack", string, "Select", "Cancel");
	return 1;
}

forward ShowArenaSetsDialog(playerid);
public ShowArenaSetsDialog(playerid)
{
	new string[1024];
	new tempString[256];
	new arenaSets[2][32] = {
		"Arenas",
		"RomdoArenas"
	};
	new arenaSetNames[2][64] = {
		"AttDef Arenas",
		"Romdo Arenas (GTA League)"
	};

	format(string, sizeof(string), "ID\tName\tArenas\n");

	for (new i = 0; i < sizeof(arenaSets); i++)
	{
		new arenaCount = GetArenaCount(arenaSets[i]);
		if (!strcmp(arenaSets[i], CurrentArenaSet, true))
		{
			format(tempString, sizeof(tempString), "%d\t{AADC93}%s (Active)\t{AADC93}%d Arenas\n", i, arenaSetNames[i], arenaCount);
		}
		else
		{
			format(tempString, sizeof(tempString), "%d\t{FFFFFF}%s\t%d Arenas\n", i, arenaSetNames[i], arenaCount);
		}
		strcat(string, tempString, sizeof(string));
	}

	ShowPlayerDialog(playerid, DIALOG_LOAD_ARENAS, DIALOG_STYLE_TABLIST_HEADERS, "Select Arena Pack", string, "Select", "Cancel");
	return 1;
}

GetArenaCount(const arenaSetName[])
{
	new query[128];
	format(query, sizeof(query), "SELECT COUNT(*) FROM `%s`", arenaSetName);
	new iString[200];

	new DBResult: res = DB_ExecuteQuery(sqliteconnection, query);
	if (res)
	{
		DB_GetFieldStringByName(res, "COUNT(*)", iString, sizeof(iString));
		new arenaCount = strval(iString);
		DB_FreeResultSet(res);
		return arenaCount;
	}
	return 0;
}

stock DoLoadBases(playerid, baseset)
{
	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is active.");

	if (baseset < 0 || baseset > 12)
		return SendErrorMessage(playerid, "Invalid base pack ID.");

	new selectedBaseSet[32];
	switch (baseset)
	{
		case 0: strcat(selectedBaseSet, "Wargrounds");
		case 1: strcat(selectedBaseSet, "OldBulletproofBases");
		case 2: strcat(selectedBaseSet, "DeadlyCombinations");
		case 3: strcat(selectedBaseSet, "NoCbugBases");
		case 4: strcat(selectedBaseSet, "Slaughterfield");
		case 5: strcat(selectedBaseSet, "NewBulletproofBases");
		case 6: strcat(selectedBaseSet, "cYsBulletproofBases");
		case 7: strcat(selectedBaseSet, "AttdefBases");
		case 8: strcat(selectedBaseSet, "TournamentsBases");
		case 9: strcat(selectedBaseSet, "BattleGrounds");
		case 10: strcat(selectedBaseSet, "MrSOldBases");
		case 11: strcat(selectedBaseSet, "ESL");
		case 12: strcat(selectedBaseSet, "HazeBases");
	}

	new DBResult: validation = DB_ExecuteQuery(sqliteconnection, va_return("SELECT ID FROM `%q` LIMIT 1", selectedBaseSet));
	if (!validation)
		return SendErrorMessage(playerid, "Could not read the selected base pack. The current pack was kept.");
	DB_FreeResultSet(validation);

	new oldBaseSet[sizeof(CurrentBaseSet)];
	format(oldBaseSet, sizeof(oldBaseSet), "%s", CurrentBaseSet);
	if (!DatabaseExecuteWrite(
		va_return("UPDATE `Configs` SET `Value`='%q' WHERE `Option`='CurrentBaseSet'", selectedBaseSet),
		"save current base set"))
		return SendErrorMessage(playerid, "Could not save the selected base pack. No changes were applied.");
	format(CurrentBaseSet, sizeof(CurrentBaseSet), "%s", selectedBaseSet);
	if (!LoadBases())
	{
		new configRestored = DatabaseExecuteWrite(
			va_return("UPDATE `Configs` SET `Value`='%q' WHERE `Option`='CurrentBaseSet'", oldBaseSet),
			"restore current base set after load failure");
		format(CurrentBaseSet, sizeof(CurrentBaseSet), "%s", oldBaseSet);
		if (!configRestored)
			printf("[AttDef] Could not restore CurrentBaseSet after loading '%s' failed.", selectedBaseSet);
		return SendErrorMessage(playerid, "Could not load the selected base pack. The previous base data was kept.");
	}
	DestroyMainSpawnText3D();
	CreateMainSpawnText3D();

	va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has loaded base pack "COL_SEC"\"%s\" (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], CurrentBaseSet, baseset);
	return 1;
}

stock DoLoadArenas(playerid, arenaset)
{
	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is active.");

	if (arenaset < 0 || arenaset > 1)
		return SendErrorMessage(playerid, "Invalid arena pack ID.");

	new selectedArenaSet[32];
	switch (arenaset)
	{
		case 0: strcat(selectedArenaSet, "Arenas");
		case 1: strcat(selectedArenaSet, "RomdoArenas");
	}

	new DBResult: validation = DB_ExecuteQuery(sqliteconnection, va_return("SELECT ID FROM `%q` LIMIT 1", selectedArenaSet));
	if (!validation)
		return SendErrorMessage(playerid, "Could not read the selected arena set. The current set was kept.");
	DB_FreeResultSet(validation);

	new oldArenaSet[sizeof(CurrentArenaSet)];
	format(oldArenaSet, sizeof(oldArenaSet), "%s", CurrentArenaSet);
	if (!DatabaseExecuteWrite(
		va_return("UPDATE `Configs` SET `Value`='%q' WHERE `Option`='CurrentArenaSet'", selectedArenaSet),
		"save current arena set"))
		return SendErrorMessage(playerid, "Could not save the selected arena set. No changes were applied.");
	format(CurrentArenaSet, sizeof(CurrentArenaSet), "%s", selectedArenaSet);

	if (!LoadArenas())
	{
		new configRestored = DatabaseExecuteWrite(
			va_return("UPDATE `Configs` SET `Value`='%q' WHERE `Option`='CurrentArenaSet'", oldArenaSet),
			"restore current arena set after load failure");
		format(CurrentArenaSet, sizeof(CurrentArenaSet), "%s", oldArenaSet);
		if (!configRestored)
			printf("[AttDef] Could not restore CurrentArenaSet after loading '%s' failed.", selectedArenaSet);
		return SendErrorMessage(playerid, "Could not load the selected arena set. The previous arena data was kept.");
	}
	DestroyMainSpawnText3D();
	CreateMainSpawnText3D();

	va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has loaded arena pack "COL_SEC"\"%s\" (ID: %d)", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], CurrentArenaSet, arenaset);
	return 1;
}

GetBaseCount(const baseSetName[])
{
	new query[128];
	format(query, sizeof(query), "SELECT COUNT(*) FROM `%s`", baseSetName);
	new iString[200];

	new DBResult: res = DB_ExecuteQuery(sqliteconnection, query);
	if (res)
	{
		DB_GetFieldStringByName(res, "COUNT(*)", iString, sizeof(iString)); // Assuming COUNT(*) is the field name
		new baseCount = strval(iString);
		DB_FreeResultSet(res);
		return baseCount;
	}
	return 0; // Error handling or case where no records were found
}

#define ISSUE_LOW_FPS        1
#define ISSUE_HIGH_FPS       2
#define ISSUE_HIGH_PL        3
#define ISSUE_HIGH_PING      4

