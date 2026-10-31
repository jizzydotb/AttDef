#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_database
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_database
	#endinput
#endif
#define _attdef_impl_database

#define DATABASE_SCHEMA_VERSION (8)

/*
	Database loading
*/

#define MAX_PENDING_DATABASE_WRITES 128
#define MAX_DATABASE_WRITE_RETRIES 3
#define DATABASE_WRITE_RETRY_DELAY 10000

new bool: PendingDatabaseWrite[MAX_PENDING_DATABASE_WRITES];
new PendingDatabaseAttempts[MAX_PENDING_DATABASE_WRITES];
new PendingDatabaseQuery[MAX_PENDING_DATABASE_WRITES][512];
new PendingDatabaseOperation[MAX_PENDING_DATABASE_WRITES][64];
new DatabaseWriteRetryTimer;
new DatabaseWriteRetryGeneration;

stock ScheduleDatabaseWriteRetries()
{
	if (DatabaseWriteRetryTimer)
		return 1;

	DatabaseWriteRetryGeneration++;
	if (DatabaseWriteRetryGeneration == 0)
		DatabaseWriteRetryGeneration++;

	DatabaseWriteRetryTimer = SetTimerEx(
		"RetryPendingDatabaseWrites",
		DATABASE_WRITE_RETRY_DELAY,
		false,
		"i",
		DatabaseWriteRetryGeneration);
	if (!DatabaseWriteRetryTimer)
	{
		printf("[AttDef] Failed to schedule retries for pending database writes.");
		return 0;
	}
	return 1;
}

stock QueueDatabaseWriteRetry(const query[], const operation[])
{
	new slot = -1;
	for (new index = 0; index < MAX_PENDING_DATABASE_WRITES; index++)
	{
		if (PendingDatabaseWrite[index] && !strcmp(PendingDatabaseOperation[index], operation))
		{
			slot = index;
			break;
		}
		if (!PendingDatabaseWrite[index] && slot == -1)
			slot = index;
	}

	if (slot == -1)
	{
		printf("[AttDef] Pending database-write queue is full; could not queue operation '%s'.", operation);
		return 0;
	}

	PendingDatabaseWrite[slot] = true;
	PendingDatabaseAttempts[slot] = 0;
	// Coalesce by logical operation so a newer absolute-value update replaces an older one.
	format(PendingDatabaseQuery[slot], sizeof(PendingDatabaseQuery[]), "%s", query);
	format(PendingDatabaseOperation[slot], sizeof(PendingDatabaseOperation[]), "%s", operation);
	return ScheduleDatabaseWriteRetries();
}

stock ClearPendingDatabaseWrite(const operation[])
{
	for (new index = 0; index < MAX_PENDING_DATABASE_WRITES; index++)
	{
		if (PendingDatabaseWrite[index] && !strcmp(PendingDatabaseOperation[index], operation))
		{
			PendingDatabaseWrite[index] = false;
			PendingDatabaseAttempts[index] = 0;
		}
	}
	return 1;
}

forward RetryPendingDatabaseWrites(generation);
public RetryPendingDatabaseWrites(generation)
{
	if (generation != DatabaseWriteRetryGeneration)
		return 0;

	DatabaseWriteRetryTimer = 0;
	new bool: hasRetryableWrites;
	for (new index = 0; index < MAX_PENDING_DATABASE_WRITES; index++)
	{
		if (!PendingDatabaseWrite[index] || PendingDatabaseAttempts[index] >= MAX_DATABASE_WRITE_RETRIES)
			continue;

		if (!sqliteconnection)
		{
			PendingDatabaseAttempts[index]++;
			if (PendingDatabaseAttempts[index] == MAX_DATABASE_WRITE_RETRIES)
			{
				printf(
					"[AttDef] Database write '%s' failed after %d retries; it remains pending in memory.",
					PendingDatabaseOperation[index], MAX_DATABASE_WRITE_RETRIES);
			}
			else
			{
				hasRetryableWrites = true;
			}
			continue;
		}

		PendingDatabaseAttempts[index]++;
		new DBResult: result = DB_ExecuteQuery(sqliteconnection, PendingDatabaseQuery[index]);
		if (result && DB_FreeResultSet(result))
		{
			printf("[AttDef] Recovered pending database write: %s.", PendingDatabaseOperation[index]);
			PendingDatabaseWrite[index] = false;
			PendingDatabaseAttempts[index] = 0;
			continue;
		}

		if (PendingDatabaseAttempts[index] == MAX_DATABASE_WRITE_RETRIES)
		{
			printf(
				"[AttDef] Database write '%s' failed after %d retries; it remains pending in memory.",
				PendingDatabaseOperation[index], MAX_DATABASE_WRITE_RETRIES);
		}
		else
		{
			hasRetryableWrites = true;
		}
	}

	if (hasRetryableWrites)
		ScheduleDatabaseWriteRetries();
	return 1;
}

DatabaseExecuteWrite(const query[], const operation[], bool: retrySafe = false)
{
	if (!sqliteconnection)
	{
		printf("[AttDef] Cannot %s: SQLite database is not open.", operation);
		if (retrySafe)
			QueueDatabaseWriteRetry(query, operation);
		return 0;
	}

	new DBResult: result = DB_ExecuteQuery(sqliteconnection, query);
	if (!result)
	{
		printf("[AttDef] Failed to %s.", operation);
		if (retrySafe)
			QueueDatabaseWriteRetry(query, operation);
		return 0;
	}
	if (!DB_FreeResultSet(result))
	{
		printf("[AttDef] Failed to release the query result after attempting to %s.", operation);
		if (retrySafe)
			QueueDatabaseWriteRetry(query, operation);
		return 0;
	}
	if (retrySafe)
		ClearPendingDatabaseWrite(operation);
	return 1;
}

RunDatabaseMigrations()
{
	if (!DatabaseExecuteWrite(
		"CREATE TABLE IF NOT EXISTS `SchemaMigrations` (`Version` INTEGER NOT NULL PRIMARY KEY, `AppliedAt` TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP)",
		"create schema migration history"))
		return 0;

	new DBResult: result = DB_ExecuteQuery(sqliteconnection, "SELECT COALESCE(MAX(`Version`), 0) AS `Version` FROM `SchemaMigrations`");
	if (!result)
	{
		printf("[AttDef] Could not read the database schema version.");
		return 0;
	}

	new versionString[16];
	DB_GetFieldStringByName(result, "Version", versionString, sizeof(versionString));
	new currentVersion = strval(versionString);
	if (!DB_FreeResultSet(result))
	{
		printf("[AttDef] Could not release the database schema version result.");
		return 0;
	}
	if (currentVersion > DATABASE_SCHEMA_VERSION)
	{
		printf(
			"[AttDef] Database schema version %d is newer than this gamemode supports (maximum %d).",
			currentVersion, DATABASE_SCHEMA_VERSION);
		return 0;
	}

	if (currentVersion < 1)
	{
		if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin schema migration 1"))
			return 0;

		if (!DatabaseExecuteWrite("CREATE INDEX IF NOT EXISTS `idx_players_name` ON `Players` (`Name`)", "create player name index")
			|| !DatabaseExecuteWrite("CREATE INDEX IF NOT EXISTS `idx_akas_ip` ON `AKAs` (`IP`)", "create AKA IP index")
			|| !DatabaseExecuteWrite("INSERT INTO `SchemaMigrations` (`Version`) VALUES (1)", "record schema migration 1"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 1");
			return 0;
		}

		if (!DatabaseExecuteWrite("COMMIT", "commit schema migration 1"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 1");
			return 0;
		}
		currentVersion = 1;
	}

	if (currentVersion < 2)
	{
		if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin schema migration 2"))
			return 0;

		if (!DatabaseExecuteWrite(
			"CREATE TABLE IF NOT EXISTS `CasualTeamScores` (`ID` INTEGER NOT NULL PRIMARY KEY CHECK (`ID` = 1), `AttackerWins` INTEGER NOT NULL DEFAULT 0 CHECK (`AttackerWins` >= 0), `DefenderWins` INTEGER NOT NULL DEFAULT 0 CHECK (`DefenderWins` >= 0))",
			"create casual team score storage")
			|| !DatabaseExecuteWrite(
				"INSERT OR IGNORE INTO `CasualTeamScores` (`ID`, `AttackerWins`, `DefenderWins`) VALUES (1, 0, 0)",
				"initialize casual team score storage")
			|| !DatabaseExecuteWrite("INSERT INTO `SchemaMigrations` (`Version`) VALUES (2)", "record schema migration 2"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 2");
			return 0;
		}

		if (!DatabaseExecuteWrite("COMMIT", "commit schema migration 2"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 2");
			return 0;
		}
		currentVersion = 2;
	}

	if (currentVersion < 3)
	{
		if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin schema migration 3"))
			return 0;

		if (!DatabaseExecuteWrite(
			"INSERT INTO `Configs` (`Option`, `Value`) SELECT 'AntiCBug', 0 WHERE NOT EXISTS (SELECT 1 FROM `Configs` WHERE `Option` = 'AntiCBug') UNION ALL SELECT 'AntiSlideBug', 0 WHERE NOT EXISTS (SELECT 1 FROM `Configs` WHERE `Option` = 'AntiSlideBug')",
			"add anti-bug configuration options")
			|| !DatabaseExecuteWrite("INSERT INTO `SchemaMigrations` (`Version`) VALUES (3)", "record schema migration 3"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 3");
			return 0;
		}

		if (!DatabaseExecuteWrite("COMMIT", "commit schema migration 3"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 3");
			return 0;
		}
		currentVersion = 3;
	}

	if (currentVersion < 4)
	{
		if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin schema migration 4"))
			return 0;

		if (!DatabaseExecuteWrite(
			"ALTER TABLE `Players` ADD COLUMN `TAssists` INTEGER NOT NULL DEFAULT 0",
			"add persistent player assists")
			|| !DatabaseExecuteWrite("INSERT INTO `SchemaMigrations` (`Version`) VALUES (4)", "record schema migration 4"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 4");
			return 0;
		}

		if (!DatabaseExecuteWrite("COMMIT", "commit schema migration 4"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 4");
			return 0;
		}
		currentVersion = 4;
	}

	if (currentVersion < 5)
	{
		if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin schema migration 5"))
			return 0;

		if (!DatabaseExecuteWrite(
			"ALTER TABLE `Players` ADD COLUMN `ClutchesWon` INTEGER NOT NULL DEFAULT 0",
			"add persistent player clutches won")
			|| !DatabaseExecuteWrite(
				"CREATE TABLE IF NOT EXISTS `MatchHistory` (`ID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `PlayedAt` TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP, `TeamA` TEXT NOT NULL, `TeamB` TEXT NOT NULL, `ScoreA` INTEGER NOT NULL DEFAULT 0, `ScoreB` INTEGER NOT NULL DEFAULT 0, `Winner` TEXT NOT NULL, `MVP` TEXT NOT NULL, `Top1` TEXT NOT NULL, `Top2` TEXT NOT NULL, `Top3` TEXT NOT NULL, `RoundsPlayed` INTEGER NOT NULL DEFAULT 0)",
				"create match history table")
			|| !DatabaseExecuteWrite("INSERT INTO `SchemaMigrations` (`Version`) VALUES (5)", "record schema migration 5"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 5");
			return 0;
		}

		if (!DatabaseExecuteWrite("COMMIT", "commit schema migration 5"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 5");
			return 0;
		}
		currentVersion = 5;
	}

	if (currentVersion < 6)
	{
		if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin schema migration 6"))
			return 0;

		if (!DatabaseExecuteWrite(
			"DELETE FROM `Configs` WHERE rowid NOT IN (SELECT MAX(rowid) FROM `Configs` GROUP BY `Option`)",
			"clean duplicate config options")
			|| !DatabaseExecuteWrite(
				"CREATE UNIQUE INDEX IF NOT EXISTS `idx_configs_option` ON `Configs` (`Option`)",
				"create unique index on config options")
			|| !DatabaseExecuteWrite("INSERT INTO `SchemaMigrations` (`Version`) VALUES (6)", "record schema migration 6"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 6");
			return 0;
		}

		if (!DatabaseExecuteWrite("COMMIT", "commit schema migration 6"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 6");
			return 0;
		}
		currentVersion = 6;
	}

	if (currentVersion < 7)
	{
		if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin schema migration 7"))
			return 0;

		if (!DatabaseExecuteWrite(
			"CREATE TABLE IF NOT EXISTS DiscordLinks (DiscordID TEXT NOT NULL PRIMARY KEY, AccountName TEXT NOT NULL UNIQUE, LinkedAt TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP)",
			"create discord links table")
			|| !DatabaseExecuteWrite("CREATE INDEX IF NOT EXISTS idx_discord_links_name ON DiscordLinks (AccountName)", "create discord links name index")
			|| !DatabaseExecuteWrite("INSERT INTO SchemaMigrations (Version) VALUES (7)", "record schema migration 7"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 7");
			return 0;
		}

		if (!DatabaseExecuteWrite("COMMIT", "commit schema migration 7"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 7");
			return 0;
		}
		currentVersion = 7;
	}

	if (currentVersion < 8)
	{
		if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin schema migration 8"))
			return 0;

		if (!DatabaseExecuteWrite(
			"DELETE FROM `Commands` WHERE rowid NOT IN (SELECT MAX(rowid) FROM `Commands` GROUP BY `Command`)",
			"clean duplicate command entries")
			|| !DatabaseExecuteWrite(
				"CREATE UNIQUE INDEX IF NOT EXISTS `idx_commands_command` ON `Commands` (`Command`)",
				"create unique index on commands")
			|| !DatabaseExecuteWrite(
				"INSERT OR IGNORE INTO `Commands` (`Command`, `Level`) VALUES " \
				"('stats', 0), ('top', 0), ('matchhistory', 0), ('vote', 0), ('tips', 0), " \
				"('demo', 0), ('linkdiscord', 0), ('discord', 0), " \
				"('dmg', 0), ('swapme', 0), ('tacpause', 0), ('timeout', 0)",
				"seed modern player command access levels")
			|| !DatabaseExecuteWrite(
				"INSERT OR REPLACE INTO `Commands` (`Command`, `Level`) VALUES " \
				"('pswap', 1), ('eject', 1), ('rava', 1), " \
				"('addmetime', 2), ('gunmenutime', 2), " \
				"('aircraft', 3), ('antimacros', 3), ('autoend', 3), ('autopause', 3), ('dcmode', 3), " \
				"('cparena', 3), ('deadbodies', 3), ('defendersvehicles', 3), ('halfdeagle', 3), ('hpbars', 3), " \
				"('maxfps', 3), ('netmode', 3), ('noswitch', 3), ('pausemode', 3), " \
				"('posttop', 3), ('serverpassword', 3), ('sp', 3), ('setarmourping', 3), " \
				"('skinlabels', 3), ('skinslabels', 3), ('snipernerf', 3), ('syncabuse', 3), " \
				"('togglearmour', 3), ('vlobby', 3), ('weapondmg', 3), " \
				"('cleousage', 4), ('extrawsusage', 4), ('sampfuncsusage', 4), ('sprinthookusage', 4), " \
				"('customtitle', 4), ('speedcap', 4)",
				"seed modern admin command access levels")
			|| !DatabaseExecuteWrite("INSERT INTO `SchemaMigrations` (`Version`) VALUES (8)", "record schema migration 8"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 8");
			return 0;
		}

		if (!DatabaseExecuteWrite("COMMIT", "commit schema migration 8"))
		{
			DatabaseExecuteWrite("ROLLBACK", "rollback schema migration 8");
			return 0;
		}
		currentVersion = 8;
	}

	printf("[AttDef] Database schema version %d is ready.", currentVersion);
	return 1;
}

LoadCasualTeamScores()
{
	new DBResult: result = DB_ExecuteQuery(
		sqliteconnection,
		"SELECT `AttackerWins`, `DefenderWins` FROM `CasualTeamScores` WHERE `ID` = 1");
	if (!result)
	{
		printf("[AttDef] Failed to load casual team scores.");
		return 0;
	}
	if (DB_GetRowCount(result) != 1)
	{
		DB_FreeResultSet(result);
		printf("[AttDef] Expected exactly one casual team score row; refusing to replace the in-memory scores.");
		return 0;
	}

	new attackerWinsString[16], defenderWinsString[16];
	DB_GetFieldStringByName(result, "AttackerWins", attackerWinsString, sizeof(attackerWinsString));
	DB_GetFieldStringByName(result, "DefenderWins", defenderWinsString, sizeof(defenderWinsString));
	new attackerWins = strval(attackerWinsString);
	new defenderWins = strval(defenderWinsString);
	if (!DB_FreeResultSet(result))
	{
		printf("[AttDef] Failed to release the casual team score result.");
		return 0;
	}
	if (attackerWins < 0 || defenderWins < 0)
	{
		printf("[AttDef] Casual team scores in the database cannot be negative.");
		return 0;
	}

	TeamRoundWins[ATTACKER] = attackerWins;
	TeamRoundWins[DEFENDER] = defenderWins;
	return 1;
}

SaveCasualTeamScores(attackerWins, defenderWins)
{
	return DatabaseExecuteWrite(
		va_return(
			"UPDATE `CasualTeamScores` SET `AttackerWins` = %d, `DefenderWins` = %d WHERE `ID` = 1",
			attackerWins, defenderWins),
		"save casual team scores",
		true);
}

SetDatabaseToReload(playerid = INVALID_PLAYER_ID)
{
	new DB: reloadedConnection = DB_Open("AttDefDatabase.db");
	if (!reloadedConnection)
	{
		printf("[AttDef] Failed to reopen SQLite database; keeping the current connection.");
		if (playerid != INVALID_PLAYER_ID)
			return SendErrorMessage(playerid, "Could not reopen the database. The current connection was kept.");
		return 0;
	}

	new DB: previousConnection = sqliteconnection;
	sqliteconnection = reloadedConnection;
	new DBResult: walResult = DB_ExecuteQuery(sqliteconnection, "PRAGMA journal_mode = WAL");
	if (walResult)
		DB_FreeResultSet(walResult);
	DatabaseExecuteWrite("PRAGMA synchronous = NORMAL", "set sqlite synchronous pragma");
	DatabaseExecuteWrite("PRAGMA temp_store = MEMORY", "set sqlite temp_store pragma");
	if (!RunDatabaseMigrations())
	{
		sqliteconnection = previousConnection;
		DB_Close(reloadedConnection);
		printf("[AttDef] Database reload rejected because its schema could not be migrated safely.");
		if (playerid != INVALID_PLAYER_ID)
			return SendErrorMessage(playerid, "The database reload was rejected; the current connection was kept.");
		return 0;
	}
	if (previousConnection)
		DB_Close(previousConnection);

	if (playerid != INVALID_PLAYER_ID)
	{

		va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has set the database to reload.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	}
	return SendClientMessageToAll(-1, COL_PRIM "\xBB SQLite database has been reloaded successfully.");
}

LoadDatabase()
{
	sqliteconnection = DB_Open("AttDefDatabase.db");
	if (!sqliteconnection)
	{
		printf("Warning: database is not loaded. Make sure 'AttDefDatabase.db' file is inside the '/scriptfiles' directory and restart. Visit %s for further help!", GM_WEBSITE);
		return 0;
	}
	new DBResult: walResult = DB_ExecuteQuery(sqliteconnection, "PRAGMA journal_mode = WAL");
	if (walResult)
		DB_FreeResultSet(walResult);
	DatabaseExecuteWrite("PRAGMA synchronous = NORMAL", "set sqlite synchronous pragma");
	DatabaseExecuteWrite("PRAGMA temp_store = MEMORY", "set sqlite temp_store pragma");
	if (!RunDatabaseMigrations())
	{
		printf("[AttDef] Database initialization stopped because its schema could not be migrated safely.");
		return 0;
	}
	if (!LoadCasualTeamScores())
	{
		printf("[AttDef] Database initialization stopped because casual team scores could not be loaded safely.");
		return 0;
	}
	if (!LoadConfig())
	{
		printf("[AttDef] Database initialization stopped because server configuration could not be loaded.");
		return 0;
	}
	TeamName[ATTACKER] = "Alpha";
	TeamName[DEFENDER] = "Beta";
	LoadTextDraws(); // Loads all global textdraws
	UpdateTeamScoreTextDraw();
	if (!LoadBases() || !LoadArenas() || !LoadDMs())
	{
		printf("[AttDef] Database initialization stopped because one or more map sets could not be loaded.");
		return 0;
	}
	LoadCommands();	 // Load commands with level
	return 1;
}

OptimizeDatabase()
{
	return DatabaseExecuteWrite("PRAGMA optimize", "optimize the database");
}

#include <YSI_Coding\y_hooks>

hook OnMatchEndEvent(winnerid, scoreAtt, scoreDef, topOverallID)
{
	new winnerName[32], mvpName[MAX_PLAYER_NAME];
	if (winnerid == ATTACKER)
		format(winnerName, sizeof(winnerName), "Team %s", TeamName[ATTACKER]);
	else if (winnerid == DEFENDER)
		format(winnerName, sizeof(winnerName), "Team %s", TeamName[DEFENDER]);
	else
		format(winnerName, sizeof(winnerName), "Draw");

	if (topOverallID != -1 && topOverallID < LastMatchScoreCount)
		format(mvpName, sizeof(mvpName), "%s", RoundResultNames[topOverallID]);
	else
		format(mvpName, sizeof(mvpName), "None");

	DatabaseExecuteWrite(
		va_return(
			"INSERT INTO `MatchHistory` (`TeamA`, `TeamB`, `ScoreA`, `ScoreB`, `Winner`, `MVP`, `Top1`, `Top2`, `Top3`, `RoundsPlayed`) VALUES ('%q', '%q', %d, %d, '%q', '%q', '%q', '%q', '%q', %d)",
			TeamName[ATTACKER], TeamName[DEFENDER], scoreAtt, scoreDef,
			winnerName, mvpName, LastMatchTopString[0], LastMatchTopString[1], LastMatchTopString[2], CurrentRound
		),
		"save completed match history",
		true
	);
	return 1;
}
