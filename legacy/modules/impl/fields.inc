#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_fields
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_fields
	#endinput
#endif
#define _attdef_impl_fields

/*
	Fields related: bases, arenas... etc
*/

stock FindFreeBaseSlot()
{
	for (new i = 1; i < MAX_BASES; i++)
		if (!BExist[i])
			return i;

	return TotalBases + 1;
}

stock FindFreeArenaSlot()
{
	for (new i = 1; i < MAX_ARENAS; i++)
		if (!AExist[i])
			return i;

	return TotalArenas + 1;
}

stock ValidateBaseSetRows(const baseSet[])
{
	new DBResult: result = DB_ExecuteQuery(
		sqliteconnection,
		va_return(
			"SELECT ID, Interior, AttSpawn, DefSpawn, CPSpawn, Name, BAttWins, BDefWins, BPlayed FROM `%q` ORDER BY ID ASC",
			baseSet));
	if (!result)
	{
		printf("[AttDef] Failed to validate base set '%s'.", baseSet);
		return 0;
	}
	if (DB_GetRowCount(result) == 0)
	{
		if (!DB_FreeResultSet(result))
			printf("[AttDef] Failed to release the empty base-set validation result.");
		printf("[AttDef] Base set '%s' is empty.", baseSet);
		return 0;
	}

	new bool: seen[MAX_BASES];
	new idString[16], coordinates[128];
	new Float: x, Float: y, Float: z;
	new bool: valid = true;
	for (new i = 0; i < MAX_BASES; i++)
		seen[i] = false;

	do
	{
		DB_GetFieldStringByName(result, "ID", idString, sizeof(idString));
		if (!IsNumeric(idString))
		{
			printf("[AttDef] Base set '%s' contains a non-numeric ID '%s'.", baseSet, idString);
			valid = false;
			break;
		}
		new baseID = strval(idString);
		if (baseID < 0 || baseID >= MAX_BASES || seen[baseID])
		{
			printf("[AttDef] Base set '%s' contains an invalid or duplicate ID (%d).", baseSet, baseID);
			valid = false;
			break;
		}
		seen[baseID] = true;

		DB_GetFieldStringByName(result, "AttSpawn", coordinates, sizeof(coordinates));
		if (sscanf(coordinates, "p<,>fff", x, y, z))
		{
			printf("[AttDef] Base set '%s', ID %d has an invalid AttSpawn.", baseSet, baseID);
			valid = false;
			break;
		}
		DB_GetFieldStringByName(result, "DefSpawn", coordinates, sizeof(coordinates));
		if (sscanf(coordinates, "p<,>fff", x, y, z))
		{
			printf("[AttDef] Base set '%s', ID %d has an invalid DefSpawn.", baseSet, baseID);
			valid = false;
			break;
		}
		DB_GetFieldStringByName(result, "CPSpawn", coordinates, sizeof(coordinates));
		if (sscanf(coordinates, "p<,>fff", x, y, z))
		{
			printf("[AttDef] Base set '%s', ID %d has an invalid CPSpawn.", baseSet, baseID);
			valid = false;
			break;
		}
	}
	while (DB_SelectNextRow(result));

	if (!DB_FreeResultSet(result))
	{
		printf("[AttDef] Failed to release the base-set validation result.");
		return 0;
	}
	if (!valid)
		printf("[AttDef] Base set '%s' is empty or contains malformed map data.", baseSet);
	return valid;
}

stock ValidateArenaSetRows(const arenaSet[])
{
	new DBResult: result = DB_ExecuteQuery(
		sqliteconnection,
		va_return(
			"SELECT ID, Interior, AttSpawn, DefSpawn, CPSpawn, Max, Min, Name, AAttWins, ADefWins, APlayed FROM `%q` ORDER BY ID ASC",
			arenaSet));
	if (!result)
	{
		printf("[AttDef] Failed to validate arena set '%s'.", arenaSet);
		return 0;
	}
	if (DB_GetRowCount(result) == 0)
	{
		if (!DB_FreeResultSet(result))
			printf("[AttDef] Failed to release the empty arena-set validation result.");
		printf("[AttDef] Arena set '%s' is empty.", arenaSet);
		return 0;
	}

	new bool: seen[MAX_ARENAS];
	new idString[16], interiorString[16], coordinates[128];
	new Float: x, Float: y, Float: z;
	new Float: minX, Float: minY, Float: maxX, Float: maxY;
	new bool: valid = true;
	for (new i = 0; i < MAX_ARENAS; i++)
		seen[i] = false;

	do
	{
		DB_GetFieldStringByName(result, "ID", idString, sizeof(idString));
		if (!IsNumeric(idString))
		{
			printf("[AttDef] Arena set '%s' contains a non-numeric ID '%s'.", arenaSet, idString);
			valid = false;
			break;
		}
		new arenaID = strval(idString);
		if (arenaID < 0 || arenaID >= MAX_ARENAS || seen[arenaID])
		{
			printf("[AttDef] Arena set '%s' contains an invalid or duplicate ID (%d).", arenaSet, arenaID);
			valid = false;
			break;
		}
		seen[arenaID] = true;

		DB_GetFieldStringByName(result, "Interior", interiorString, sizeof(interiorString));
		if (!IsNumeric(interiorString) || strval(interiorString) < 0)
		{
			printf("[AttDef] Arena set '%s', ID %d has an invalid Interior value.", arenaSet, arenaID);
			valid = false;
			break;
		}
		new interiorID = strval(interiorString);

		DB_GetFieldStringByName(result, "AttSpawn", coordinates, sizeof(coordinates));
		if (sscanf(coordinates, "p<,>fff", x, y, z))
		{
			printf("[AttDef] Arena set '%s', ID %d has an invalid AttSpawn.", arenaSet, arenaID);
			valid = false;
			break;
		}
		DB_GetFieldStringByName(result, "DefSpawn", coordinates, sizeof(coordinates));
		if (sscanf(coordinates, "p<,>fff", x, y, z))
		{
			printf("[AttDef] Arena set '%s', ID %d has an invalid DefSpawn.", arenaSet, arenaID);
			valid = false;
			break;
		}
		DB_GetFieldStringByName(result, "CPSpawn", coordinates, sizeof(coordinates));
		if (sscanf(coordinates, "p<,>fff", x, y, z))
		{
			printf("[AttDef] Arena set '%s', ID %d has an invalid CPSpawn.", arenaSet, arenaID);
			valid = false;
			break;
		}
		DB_GetFieldStringByName(result, "Max", coordinates, sizeof(coordinates));
		if (sscanf(coordinates, "p<,>ff", maxX, maxY))
		{
			printf("[AttDef] Arena set '%s', ID %d has invalid Max bounds.", arenaSet, arenaID);
			valid = false;
			break;
		}
		DB_GetFieldStringByName(result, "Min", coordinates, sizeof(coordinates));
		if (sscanf(coordinates, "p<,>ff", minX, minY))
		{
			printf("[AttDef] Arena set '%s', ID %d has invalid Min bounds.", arenaSet, arenaID);
			valid = false;
			break;
		}

		new bool: interiorBounds = interiorID > 0
			&& ((minX == -9999.0 && minY == -9999.0 && maxX == 9999.0 && maxY == 9999.0)
				|| (minX == 9999.0 && minY == 9999.0 && maxX == 9999.0 && maxY == 9999.0));
		if (!interiorBounds && (minX >= maxX || minY >= maxY))
		{
			printf("[AttDef] Arena set '%s', ID %d has inverted or empty bounds.", arenaSet, arenaID);
			valid = false;
			break;
		}
		if (!interiorBounds && (minX < MAP_MIN_X || minY < MAP_MIN_Y
			|| maxX > MAP_MAX_X || maxY > MAP_MAX_Y))
		{
			printf("[AttDef] Arena set '%s', ID %d has bounds outside the supported map area.", arenaSet, arenaID);
			valid = false;
			break;
		}
	}
	while (DB_SelectNextRow(result));

	if (!DB_FreeResultSet(result))
	{
		printf("[AttDef] Failed to release the arena-set validation result.");
		return 0;
	}
	if (!valid)
		printf("[AttDef] Arena set '%s' is empty or contains malformed map data.", arenaSet);
	return valid;
}

stock ValidateDMRows()
{
	new DBResult: result = DB_ExecuteQuery(
		sqliteconnection,
		"SELECT ID, Interior, Wep1, Wep2, Wep3, Wep4, Wep5, Spawn FROM DMs ORDER BY ID ASC");
	if (!result)
	{
		printf("[AttDef] Failed to validate the DM list.");
		return 0;
	}
	if (DB_GetRowCount(result) == 0)
	{
		if (!DB_FreeResultSet(result))
			printf("[AttDef] Failed to release the empty DM validation result.");
		return 1;
	}

	new bool: seen[MAX_DMS];
	new idString[16], spawn[128];
	new Float: x, Float: y, Float: z, Float: angle;
	new bool: valid = true;
	for (new i = 0; i < MAX_DMS; i++)
		seen[i] = false;

	do
	{
		DB_GetFieldStringByName(result, "ID", idString, sizeof(idString));
		if (!IsNumeric(idString))
		{
			printf("[AttDef] The DM list contains a non-numeric ID '%s'.", idString);
			valid = false;
			break;
		}
		new dmID = strval(idString);
		if (dmID < 0 || dmID >= MAX_DMS || seen[dmID])
		{
			printf("[AttDef] The DM list contains an invalid or duplicate ID (%d).", dmID);
			valid = false;
			break;
		}
		seen[dmID] = true;

		DB_GetFieldStringByName(result, "Spawn", spawn, sizeof(spawn));
		new separatorCount;
		for (new index = 0; spawn[index] != EOS; index++)
		{
			if (spawn[index] == ',')
				separatorCount++;
		}
		if (separatorCount == 2)
		{
			if (sscanf(spawn, "p<,>fff", x, y, z))
			{
				printf("[AttDef] DM %d has an invalid Spawn tuple.", dmID);
				valid = false;
				break;
			}
			angle = 0.0;
		}
		else if (separatorCount != 3 || sscanf(spawn, "p<,>ffff", x, y, z, angle))
		{
			printf("[AttDef] DM %d has an invalid Spawn tuple.", dmID);
			valid = false;
			break;
		}
	}
	while (DB_SelectNextRow(result));

	if (!DB_FreeResultSet(result))
	{
		printf("[AttDef] Failed to release the DM validation result.");
		return 0;
	}
	if (!valid)
		printf("[AttDef] The DM list is empty or contains malformed data.");
	return valid;
}

LoadBases()
{
	if (!sqliteconnection)
	{
		printf("[AttDef] Cannot load bases: SQLite database is not open.");
		return 0;
	}

	if (!ValidateBaseSetRows(CurrentBaseSet))
		return 0;

	new iString[64];
	new DBResult: res = DB_ExecuteQuery(sqliteconnection, va_return("SELECT * FROM `%q` ORDER BY ID ASC", CurrentBaseSet));
	if (!res)
	{
		printf("[AttDef] Failed to reopen base set '%s'; keeping the current base state.", CurrentBaseSet);
		return 0;
	}
	if (DB_GetRowCount(res) == 0)
	{
		if (!DB_FreeResultSet(res))
			printf("[AttDef] Failed to release the empty base-set result.");
		printf("[AttDef] Base set '%s' became empty while it was loading.", CurrentBaseSet);
		return 0;
	}

	TotalBases = 0;
	for (new i = 0; i < MAX_BASES; i++) BExist[i] = false;

	Iter_Clear(CurrentBases);
	if (DB_GetRowCount(res) > 0)
	{
		do {
			DB_GetFieldStringByName(res, "ID", iString, sizeof(iString));
			new i = strval(iString);
			if (i < 0 || i >= MAX_BASES)
			{
				printf("ERROR: Skipping base with invalid ID %d in set '%s'.", i, CurrentBaseSet);
				continue;
			}
			Iter_Add(CurrentBases, i);

			DB_GetFieldStringByName(res, "Interior", iString, sizeof(iString));
			BInterior[i] = strval(iString);

			DB_GetFieldStringByName(res, "AttSpawn", iString, sizeof(iString));
			sscanf(iString, "p<,>fff", BAttackerSpawn[i][0], BAttackerSpawn[i][1], BAttackerSpawn[i][2]);

			DB_GetFieldStringByName(res, "DefSpawn", iString, sizeof(iString));
			sscanf(iString, "p<,>fff", BDefenderSpawn[i][0], BDefenderSpawn[i][1], BDefenderSpawn[i][2]);

			DB_GetFieldStringByName(res, "CPSpawn", iString, sizeof(iString));
			sscanf(iString, "p<,>fff", BCPSpawn[i][0], BCPSpawn[i][1], BCPSpawn[i][2]);

			DB_GetFieldStringByName(res, "Name", BName[i], MAX_MAP_FIELD_NAME);

			DB_GetFieldStringByName(res, "BAttWins", iString, sizeof(iString));
			BAttWins[i] = strval(iString);

			DB_GetFieldStringByName(res, "BDefWins", iString, sizeof(iString));
			BDefWins[i] = strval(iString);

			DB_GetFieldStringByName(res, "BPlayed", iString, sizeof(iString));
			BPlayed[i] = strval(iString);

			TotalBases++;
			BExist[i] = true;
		}
		while (DB_SelectNextRow(res));
	}

	if (!DB_FreeResultSet(res))
		printf("[AttDef] Failed to release the base-set query result after loading valid data.");

	ForgetPlayedBases();
	new totalBases = Iter_Count(CurrentBases);
	SendClientMessageToAll(-1, va_return("{A4DACF}\xBB Bases Loaded: {FFFFFF}\"%d Bases\" {A4DACF}| Current Base Pack: {FFFFFF}\"%s\"", totalBases, CurrentBaseSet));
	printf("Bases Loaded: %d (from table: %s)", totalBases, CurrentBaseSet);
	return 1;
}

LoadArenas()
{
	if (!sqliteconnection)
	{
		printf("[AttDef] Cannot load arenas: SQLite database is not open.");
		return 0;
	}

	if (!ValidateArenaSetRows(CurrentArenaSet))
		return 0;

	new iString[64];
	new DBResult: res = DB_ExecuteQuery(sqliteconnection, va_return("SELECT * FROM `%q` ORDER BY ID ASC", CurrentArenaSet));
	if (!res)
	{
		printf("[AttDef] Failed to reopen arena set '%s'; keeping the current arena state.", CurrentArenaSet);
		return 0;
	}
	if (DB_GetRowCount(res) == 0)
	{
		if (!DB_FreeResultSet(res))
			printf("[AttDef] Failed to release the empty arena-set result.");
		printf("[AttDef] Arena set '%s' became empty while it was loading.", CurrentArenaSet);
		return 0;
	}

	TotalArenas = 0;
	for (new i = 0; i < MAX_ARENAS; i++) AExist[i] = false;

	Iter_Clear(CurrentArenas);
	if (DB_GetRowCount(res) > 0)
	{
		do {
			DB_GetFieldStringByName(res, "ID", iString, sizeof(iString));
			new i = strval(iString);
			if (i < 0 || i >= MAX_ARENAS)
			{
				printf("ERROR: Skipping arena with invalid ID %d in set '%s'.", i, CurrentArenaSet);
				continue;
			}
			Iter_Add(CurrentArenas, i);

			DB_GetFieldStringByName(res, "Interior", iString, sizeof(iString));
			AInterior[i] = strval(iString);

			DB_GetFieldStringByName(res, "AttSpawn", iString, sizeof(iString));
			sscanf(iString, "p<,>fff", AAttackerSpawn[i][0], AAttackerSpawn[i][1], AAttackerSpawn[i][2]);

			DB_GetFieldStringByName(res, "DefSpawn", iString, sizeof(iString));
			sscanf(iString, "p<,>fff", ADefenderSpawn[i][0], ADefenderSpawn[i][1], ADefenderSpawn[i][2]);

			DB_GetFieldStringByName(res, "CPSpawn", iString, sizeof(iString));
			sscanf(iString, "p<,>fff", ACPSpawn[i][0], ACPSpawn[i][1], ACPSpawn[i][2]);

			DB_GetFieldStringByName(res, "Max", iString, sizeof(iString));
			sscanf(iString, "p<,>ff", AMax[i][0], AMax[i][1]);

			DB_GetFieldStringByName(res, "Min", iString, sizeof(iString));
			sscanf(iString, "p<,>ff", AMin[i][0], AMin[i][1]);

			if (AInterior[i] > 0
				&& AMin[i][0] == 9999.0 && AMin[i][1] == 9999.0
				&& AMax[i][0] == 9999.0 && AMax[i][1] == 9999.0)
			{
				AMin[i][0] = -9999.0;
				AMin[i][1] = -9999.0;
			}

			DB_GetFieldStringByName(res, "Name", AName[i], MAX_MAP_FIELD_NAME);

			DB_GetFieldStringByName(res, "AAttWins", iString, sizeof(iString));
			AAttWins[i] = strval(iString);

			DB_GetFieldStringByName(res, "ADefWins", iString, sizeof(iString));
			ADefWins[i] = strval(iString);

			DB_GetFieldStringByName(res, "APlayed", iString, sizeof(iString));
			APlayed[i] = strval(iString);

			TotalArenas++;
			AExist[i] = true;
		}
		while (DB_SelectNextRow(res));
	}

	if (!DB_FreeResultSet(res))
		printf("[AttDef] Failed to release the arena-set query result after loading valid data.");

	ForgetPlayedArenas();
	new totalArenas = Iter_Count(CurrentArenas);
	SendClientMessageToAll(-1, va_return(COL_PRIM "\xBB Arenas Loaded: "COL_SEC"%d " COL_PRIM "| Current arena set: "COL_SEC"%s", totalArenas, CurrentArenaSet));
	printf("Arenas Loaded: %d (from table: %s)", totalArenas, CurrentArenaSet);
	return 1;
}

LoadDMs()
{
	if (!sqliteconnection)
	{
		printf("[AttDef] Cannot load DMs: SQLite database is not open.");
		return 0;
	}

	if (!ValidateDMRows())
		return 0;

	new iString[64];
	new DBResult: res = DB_ExecuteQuery(sqliteconnection, "SELECT * FROM DMs ORDER BY ID ASC");
	if (!res)
	{
		printf("[AttDef] Failed to reopen the DM list; keeping the current DM state.");
		return 0;
	}
	Iter_Clear(CurrentDMs);
	for (new i = 0; i < MAX_DMS; i++) DMExist[i] = false;
	if (DB_GetRowCount(res) > 0)
	{
		do {
			DB_GetFieldStringByName(res, "ID", iString, sizeof(iString));
			new i = strval(iString);
			if (i < 0 || i >= MAX_DMS)
			{
				printf("ERROR: Skipping DM with invalid ID %d.", i);
				continue;
			}
			Iter_Add(CurrentDMs, i);
			DMExist[i] = true;

			DB_GetFieldStringByName(res, "Interior", iString, sizeof(iString));
			DMInterior[i] = strval(iString);

			for (new j = 0; j < 5; ++j)
			{
				new fieldName[10], weapon[128];
				format(fieldName, sizeof(fieldName), "Wep%d", j + 1);
				DB_GetFieldStringByName(res, fieldName, weapon, sizeof(weapon));
				DMWeapons[i][j] = strval(weapon);
			}

			DB_GetFieldStringByName(res, "Spawn", iString, sizeof(iString));
			// Older DM rows may omit the heading; preserve their zero-angle default on reload.
			DMSpawn[i][3] = 0.0;
			sscanf(iString, "p<,>ffff", DMSpawn[i][0], DMSpawn[i][1], DMSpawn[i][2], DMSpawn[i][3]);
		}
		while (DB_SelectNextRow(res));
	}

	if (!DB_FreeResultSet(res))
		printf("[AttDef] Failed to release the DM query result after loading valid data.");

	new totalDMs = Iter_Count(CurrentDMs);
	printf("DMs Loaded: %d", totalDMs);
	return 1;
}
