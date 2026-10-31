#if defined _attdef_OnPRequestClass
	#endinput
#endif
#define _attdef_OnPRequestClass

forward InlineLogin(playerid, dialogid, response, listitem, string: inputtext[]);
public InlineLogin(playerid, dialogid, response, listitem, string: inputtext[])
{
#pragma unused dialogid, listitem
	if (response)
	{
		if (isnull(inputtext))
		{
			Dialog_ShowCallback(
				playerid, using public InlineLogin<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Login Dialog", "{FFFFFF}Type your password below to log in:", "Login", "Leave");
			return 1;
		}

		new iString[BCRYPT_HASH_LENGTH];
		new DBResult: res = DB_ExecuteQuery(sqliteconnection, va_return("SELECT `Password` FROM `Players` WHERE `Name` = '%q'", Player[playerid][Name]));
		if (!res)
			return SendErrorMessage(playerid, "Could not read the account from the database.");
		if (!DB_GetRowCount(res))
		{
			DB_FreeResultSet(res);
			Dialog_ShowCallback(
				playerid, using public InlineRegister<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Registration Dialog", "{FFFFFF}This account no longer exists. Register a new password:",
				"Register", "Leave");
			return 1;
		}
		DB_GetFieldStringByName(res, "Password", iString);
		DB_FreeResultSet(res);
		bcrypt_verify(playerid, "OnLoginPassHash", inputtext, iString, "i", PlayerConnectionGeneration[playerid]);
	}
	else
	{
		new iString[128];
		format(iString, sizeof(iString), ""COL_PRIM"\xAB "COL_SEC"%s "COL_PRIM"has been kicked from the server for not logging in.", Player[playerid][Name]);
		SendClientMessageToAll(-1, iString);

		SchedulePlayerKick(playerid, GetPlayerPing(playerid) + 10);
	}
	return 1;
}

forward InlineRegister(playerid, dialogid, response, listitem, string: inputtext[]);
public InlineRegister(playerid, dialogid, response, listitem, string: inputtext[])
{
#pragma unused dialogid, listitem
	if (response)
	{
		if (isnull(inputtext))
		{
			Dialog_ShowCallback(
				playerid, using public InlineRegister<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Registration Dialog", "{FFFFFF}Type your password below to register:",
				"Register", "Leave");
			return 1;
		}

		bcrypt_hash(playerid, "OnRegisterPassHash", inputtext, BCRYPT_COST, "i", PlayerConnectionGeneration[playerid]);
	}
	else
	{
		new iString[128];
		format(iString, sizeof(iString), ""COL_PRIM"\xAB "COL_SEC"%s " COL_PRIM "has been kicked from the server for not registering.", Player[playerid][Name]);
		SendClientMessageToAll(-1, iString);

		SchedulePlayerKick(playerid, GetPlayerPing(playerid) + 10);
	}
	return 1;
}

forward OnLoginPassHash(playerid, bool: success, generation);
public OnLoginPassHash(playerid, bool: success, generation)
{
	if (!IsPlayerConnected(playerid) || PlayerConnectionGeneration[playerid] != generation)
		return 1;

	if (!success)
	{
		SendErrorMessage(playerid, "Wrong Password. Please try again.");
		Dialog_ShowCallback(
			playerid, using public InlineLogin<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Login Dialog", "{FFFFFF}Type your password below to log in:", "Login", "Leave");
		return 1;
	}
	new DBResult: res = DB_ExecuteQuery(sqliteconnection, va_return("SELECT * FROM `Players` WHERE `Name` = '%q'", Player[playerid][Name]));
	if (!res)
	{
		SendErrorMessage(playerid, "Could not read the account from the database.");
		Dialog_ShowCallback(
			playerid, using public InlineLogin<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Login Dialog", "{FFFFFF}Type your password below to log in:", "Login", "Leave");
		return 1;
	}
	if (!DB_GetRowCount(res))
	{
		DB_FreeResultSet(res);
		SendErrorMessage(playerid, "This account no longer exists. Please try logging in again.");
		Dialog_ShowCallback(
			playerid, using public InlineLogin<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Login Dialog", "{FFFFFF}Type your password below to log in:", "Login", "Leave");
		return 1;
	}
	if (!LoginPlayer(playerid, res))
	{
		DB_FreeResultSet(res);
		SendErrorMessage(playerid, "Could not finish loading your account. Please try logging in again.");
		Dialog_ShowCallback(
			playerid, using public InlineLogin<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Login Dialog", "{FFFFFF}Type your password below to log in:", "Login", "Leave");
		return 1;
	}
	DB_FreeResultSet(res);

	va_SendClientMessage(playerid, -1, "{BD93DC}\xBB Level: {FFFFFF}%d {BD93DC}| Exp: {FFFFFF}%d/%d", Player[playerid][Lvl], Player[playerid][Exp], GetNextLevelExp(Player[playerid][Lvl]));
	if (Player[playerid][TipsEnabled])
	{
		StartTipsTimers(playerid);
	}
	new teamid = ShouldPlayerBeReadded(playerid);
	if (teamid != -1)
	{
		ScheduleConnectedPlayerSpawn(playerid, teamid);
	}
	else
	{
		PlayerRejoinSpawnGeneration[playerid]++;
		ShowPlayerClassSelection(playerid);
	}
	return 1;
}
forward OnRegisterPassHash(playerid, generation);
public OnRegisterPassHash(playerid, generation)
{
	if (!IsPlayerConnected(playerid) || PlayerConnectionGeneration[playerid] != generation)
		return 1;

	new dest[BCRYPT_HASH_LENGTH];
	bcrypt_get_hash(dest);

	new DBResult: existing = DB_ExecuteQuery(
		sqliteconnection, va_return("SELECT `Name` FROM `Players` WHERE `Name` = '%q'", Player[playerid][Name]));
	if (!existing)
		return SendErrorMessage(playerid, "Could not check whether the account already exists.");
	if (DB_GetRowCount(existing) > 0)
	{
		DB_FreeResultSet(existing);
		Dialog_ShowCallback(
			playerid, using public InlineLogin<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Login Dialog", "{FFFFFF}An account with this name now exists. Enter its password:",
			"Login", "Leave");
		return 1;
	}
	DB_FreeResultSet(existing);

	new IP[16];
	GetPlayerIp(playerid, IP, sizeof(IP));
	new day, month, year;
	getdate(year, month, day);
	new query[240];
	format(
		query, sizeof(query), "INSERT INTO Players (Name, Password, IP, LastSeen_Day, LastSeen_Month, LastSeen_Year) VALUES('%q', '%q', '%q', %d, %d, %d)",
		Player[playerid][Name], dest, IP, day, month, year);
	if (!DatabaseExecuteWrite(query, "register player account"))
		return SendErrorMessage(playerid, "Could not create the account. Please try again.");

	Player[playerid][Level]		  = IsPlayerAdmin(playerid) ? 5 : 0;
	if (Player[playerid][Level] == 5)
	{
		DatabaseExecuteWrite(
			va_return("UPDATE `Players` SET `Level` = 5 WHERE `Name` = '%q'", Player[playerid][Name]),
			"save RCON admin level on register");
	}
	Player[playerid][Weather]	  = MainWeather;
	Player[playerid][Time]		  = MainTime;
	Player[playerid][Logged]	  = true;
	Player[playerid][ChatChannel] = -1;
	Player[playerid][NetCheck]	  = true;
	Player[playerid][DuelsWon]	  = 0;
	Player[playerid][DuelsLost]	  = 0;
	Player[playerid][Lvl] 		  = 0;
	Player[playerid][Exp]         = 0;
	Player[playerid][ATimeDeaths] = 0;
	Player[playerid][ATimeKills]  = 0;
	Player[playerid][ATimeAssists] = 0;
	Player[playerid][ATimeDMG]    = 0;
	Player[playerid][pSkin]		  = -1;
	UpdatePlayerAdminGroup(playerid);

	SendClientMessage(playerid, -1, "{BD93DC}\xBB Your Account has successfully registered");
	va_SendClientMessage(playerid, -1, "{BD93DC}\xBB Level: {FFFFFF}%d {BD93DC}| Exp: {FFFFFF}%d/%d", Player[playerid][Lvl], Player[playerid][Exp], GetNextLevelExp(Player[playerid][Lvl]));
	if (Player[playerid][TipsEnabled])
	{
		StartTipsTimers(playerid);
	}

	new teamid = ShouldPlayerBeReadded(playerid);
	if (teamid != -1)
	{
		ScheduleConnectedPlayerSpawn(playerid, teamid);
	}
	else
	{
		PlayerRejoinSpawnGeneration[playerid]++;
		ShowPlayerClassSelection(playerid);
	}
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	// Initialize class selection mode
	Player[playerid][Team] = NON;
	SetPlayerColor(playerid, 0xAAAAAAAA);
	Player[playerid][Spawned] = false;
	UpdatePlayerHealthBar(playerid);
	PlayerTextDrawHide(playerid, FPSPingPacketBox[playerid]);
	PlayerTextDrawHide(playerid, FPSPingPacket[playerid]);

	if (GetPlayerState(playerid) != PLAYER_STATE_SPECTATING)
		TogglePlayerSpectating(playerid, true);

	SetPlayerTime(playerid, 0, 0);
	SetPlayerInterior(playerid, 0);
	switch (random(4))
	{
		case 0:
		{
			PlayerPlaySound(playerid, 162, 0.0, 0.0, 0.0);
		}
		case 1:
		{
			PlayerPlaySound(playerid, 178, 0.0, 0.0, 0.0);
		}
		case 2:
		{
			PlayerPlaySound(playerid, 176, 0.0, 0.0, 0.0);
		}
		case 3:
		{
			PlayerPlaySound(playerid, 1068, 0.0, 0.0, 0.0);
		}
	}

	if (!SightseeingInClassSelection)
	{
		switch (random(3))
		{
			case 0:
			{
				InterpolateCameraPos(playerid, MainSpawn[0], MainSpawn[1], MainSpawn[2] + 25.0, MainSpawn[0] - 7.0, MainSpawn[1] + 7.0, MainSpawn[2] + 5.0, 15000, CAMERA_MOVE);
				InterpolateCameraLookAt(playerid, MainSpawn[0], MainSpawn[1], MainSpawn[2] + 27.0, MainSpawn[0], MainSpawn[1], MainSpawn[2], 7000, CAMERA_MOVE);
			}
			case 1:
			{
				InterpolateCameraPos(
					playerid, MainSpawn[0] + 5.0, MainSpawn[1] + 5.0, MainSpawn[2] + 2.0, MainSpawn[0] - 7.0, MainSpawn[1] + 7.0, MainSpawn[2] + 25.0, 15000, CAMERA_MOVE);
				InterpolateCameraLookAt(playerid, MainSpawn[0], MainSpawn[1], MainSpawn[2] + 10.0, MainSpawn[0], MainSpawn[1], MainSpawn[2], 7000, CAMERA_MOVE);
			}
			case 2:
			{
				InterpolateCameraPos(
					playerid, MainSpawn[0] + 10.0, MainSpawn[1] - 10.0, MainSpawn[2] + 10.0, MainSpawn[0] - 5.0, MainSpawn[1] + 5.0, MainSpawn[2] + 5.0, 15000, CAMERA_MOVE);
				InterpolateCameraLookAt(playerid, MainSpawn[0], MainSpawn[1], MainSpawn[2] + 5.0, MainSpawn[0], MainSpawn[1], MainSpawn[2], 10000, CAMERA_MOVE);
			}
		}
	}
	else
	{
		switch (random(7))
		{
			case 0:
			{
				// Vinewood Sign
				InterpolateCameraPos(playerid, 1334.0220, -783.3859, 87.6606, 1407.5430, -896.9464, 87.6606, 20000, CAMERA_MOVE);
				InterpolateCameraLookAt(playerid, 1415.7408, -806.5591, 85.0399, 1415.7408, -806.5591, 85.0399, 20000, CAMERA_MOVE);
			}
			case 1:
			{
				// Vinewood Sign 2
				InterpolateCameraPos(playerid, 1476.7277, -874.3438, 110.0, 1476.7277, -900.000, 70.0, 5000, CAMERA_MOVE);
				InterpolateCameraLookAt(playerid, 1415.2177, -807.9233, 200.0, 1415.2177, -807.9233, 85.0623, 5000, CAMERA_MOVE);
			}
			case 2:
			{
				// Desert Mountains
				InterpolateCameraPos(playerid, -365.5211, 1938.2665, 86.0535, -228.2556, 1821.5653, 96.6716, 15000, CAMERA_MOVE);
				InterpolateCameraLookAt(playerid, -327.0362, 1943.1190, 112.4141, -206.0446, 1895.2479, 91.2241, 15000, CAMERA_MOVE);
			}
			case 3:
			{
				// Streets of San Fierro
				InterpolateCameraPos(playerid, -2078.7246, 731.2352, 69.4141, -1714.5399, 731.2352, 69.4141, 45000, CAMERA_MOVE);
				InterpolateCameraLookAt(playerid, -1971.8036, 731.0178, 45.2969, -1607.8036, 731.0178, 45.2969, 45000, CAMERA_MOVE);
			}
			case 4:
			{
				// LS Beach
				InterpolateCameraPos(playerid, 340.3344, -1801.2339, 10.6959, 207.3543, -1801.2339, 10.6959, 80000, CAMERA_MOVE);
				InterpolateCameraLookAt(playerid, 289.9604, -1766.6553, 4.5469, 159.9604, -1766.6553, 4.5469, 80000, CAMERA_MOVE);
			}
			case 5:
			{
				// SF Bridge
				InterpolateCameraPos(playerid, -2630.2266, 1459.0537, 65.6484, -2596.2339, 2039.0321, 263.0035, 20000, CAMERA_MOVE);
				InterpolateCameraLookAt(playerid, -2678.6890, 1589.8137, 129.3078, -2713.4839, 1757.8318, 98.4932, 20000, CAMERA_MOVE);
			}
			case 6:
			{
				// LV Stadium
				InterpolateCameraPos(playerid, 1328.3080, 2116.9485, 11.0156, 1287.4218, 2097.1223, 55.1216, 20000, CAMERA_MOVE);
				InterpolateCameraLookAt(playerid, 1334.9221, 2077.7285, 26.6737, 1381.2794, 2184.0823, 11.0234, 20000, CAMERA_MOVE);
			}
		}
	}
	// Login player
	if (!Player[playerid][Logged])
	{
		new Query[128];
		format(Query, sizeof(Query), "SELECT Name FROM Players WHERE Name = '%q'", Player[playerid][Name]);
		new DBResult: result = DB_ExecuteQuery(sqliteconnection, Query);
		if (!result)
			return SendErrorMessage(playerid, "Could not check the account in the database.");

		if (!DB_GetRowCount(result))
		{
			Dialog_ShowCallback(
				playerid, using public InlineRegister<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Registration Dialog", "{FFFFFF}Type your password below to register:",
				"Register", "Leave");
			return DB_FreeResultSet(result);
		}
		DB_FreeResultSet(result);
		// Get IP
		new IP[16];
		GetPlayerIp(playerid, IP, sizeof(IP));

		// Construct query to check if the player with the same name and IP has connected before to this server
		format(Query, sizeof(Query), "SELECT * FROM `Players` WHERE `Name` = '%q' AND `IP` = '%q'", Player[playerid][Name], IP);

		// execute
		new DBResult: res = DB_ExecuteQuery(sqliteconnection, Query);
		if (!res)
			return SendErrorMessage(playerid, "Could not check automatic login in the database.");

		// If result returns any registered users with the same name and IP that have connected to this server before, log them in
		if (DB_GetRowCount(res))
		{
			if (!LoginPlayer(playerid, res))
			{
				DB_FreeResultSet(res);
				SendErrorMessage(playerid, "Could not finish loading your account. Please log in manually.");
				Dialog_ShowCallback(
					playerid, using public InlineLogin<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Login Dialog", "{FFFFFF}Type your password below to log in:", "Login", "Leave");
				return 1;
			}
			DB_FreeResultSet(res);

			va_SendClientMessage(playerid, -1, "{BD93DC}\xBB Welcome back %s You're automatically logged!", Player[playerid][Name]);
			va_SendClientMessage(playerid, -1, "{BD93DC}\xBB Level: {FFFFFF}%d {BD93DC}| Exp: {FFFFFF}%d/%d", Player[playerid][Lvl], Player[playerid][Exp], GetNextLevelExp(Player[playerid][Lvl]));
			if (Player[playerid][TipsEnabled])
			{
				StartTipsTimers(playerid);
			}
			new teamid = ShouldPlayerBeReadded(playerid);
			if (teamid != -1)
			{
				ScheduleConnectedPlayerSpawn(playerid, teamid);
			}
			else
			{
				PlayerRejoinSpawnGeneration[playerid]++;
				ShowPlayerClassSelection(playerid);
			}
			return 1;
		}
		DB_FreeResultSet(res);
		Dialog_ShowCallback(
			playerid, using public InlineLogin<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Login Dialog", "{FFFFFF}Type your password below to log in:", "Login", "Leave");
		return 1;
	}

	new teamid = ShouldPlayerBeReadded(playerid);
	if (teamid != -1)
	{
		ScheduleConnectedPlayerSpawn(playerid, teamid);
	}
	else
	{
		PlayerRejoinSpawnGeneration[playerid]++;
		ShowPlayerClassSelection(playerid);
	}
	return 1;
}
