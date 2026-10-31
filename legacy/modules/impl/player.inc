#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_player
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_player
	#endinput
#endif
#define _attdef_impl_player

/*
	Player related
*/

#include <YSI_Coding\y_hooks>

// Checks player AKAs
GetPlayerAKA(playerid, str[], len)
{
	new IP[16];
	GetPlayerIp(playerid, IP, sizeof(IP));
	new DBResult: dbres = DB_ExecuteQuery(sqliteconnection, va_return("SELECT `Names` FROM `AKAs` WHERE `IP` = '%q'", IP));
	if (!dbres)
	{
		printf("[AttDef] Failed to read AKA data for player %d.", playerid);
		return 0;
	}

	if (DB_GetRowCount(dbres) > 0)
	{
		DB_GetFieldStringByName(dbres, "Names", str, len);
		return DB_FreeResultSet(dbres);
	}
	if (!DB_FreeResultSet(dbres))
	{
		printf("[AttDef] Failed to release AKA query result for player %d.", playerid);
		return 0;
	}
	if (!DatabaseExecuteWrite(
		va_return("INSERT INTO `AKAs` (`IP`, `Names`) VALUES ('%q', '%q')", IP, ""),
		"create AKA record"))
		return 0;
	strcat(str, "NULL", len);
	return 1;
}

MatchAKA(playerid)
{
	new AKAString[256];
	if (!GetPlayerAKA(playerid, AKAString, sizeof AKAString))
		return -1;

	new nameStart, namesLength = strlen(AKAString);
	while (nameStart < namesLength)
	{
		new nameEnd = strfind(AKAString, ",", true, nameStart);
		if (nameEnd == -1)
			nameEnd = namesLength;

		new compare[MAX_PLAYER_NAME];
		strmid(compare, AKAString, nameStart, nameEnd);

		if (nameEnd > nameStart && !strcmp(compare, Player[playerid][Name], true)
			&& strlen(Player[playerid][Name]) > 0)
			return 1;

		if (nameEnd == namesLength)
			break;
		nameStart = nameEnd + 1;
	}
	return 0;
}

CheckPlayerAKA(playerid)
{
	new matchResult = MatchAKA(playerid);
	if (matchResult < 0)
		return 0;
	if (!matchResult)
	{
		new str[16];
		GetPlayerIp(playerid, str, sizeof(str));
		new AKAString[256];
		if (!GetPlayerAKA(playerid, AKAString, sizeof AKAString))
			return 0;
		new add[MAX_PLAYER_NAME + 1];
		format(add, sizeof(add), ",%s", Player[playerid][Name]);

		if (strlen(AKAString) > 0)
			strcat(AKAString, add);
		else
			strcat(AKAString, add[1]);

		new query[640];
		format(query, sizeof(query), "UPDATE `AKAs` SET `Names` = '%q' WHERE `IP` = '%q'", AKAString, str);
		if (!DatabaseExecuteWrite(query, "update AKA record"))
			return 0;
	}
	return 1;
}

// Player name functions
RemoveClanTagFromName(playerid)
{
	new start, end, string[MAX_PLAYER_NAME];
	format(string, MAX_PLAYER_NAME, "%s", Player[playerid][Name]);
	start = strfind(string, "[", true);
	end	  = strfind(string, "]", true);
	if (start >= end)
		return string;

	strdel(string, start, end + 1);
	return string;
}

// Loads nickname and name without the clan tag
InitPlayerNames(playerid)
{
	GetPlayerName(playerid, Player[playerid][Name], MAX_PLAYER_NAME);
	new NewName[MAX_PLAYER_NAME];
	NewName = RemoveClanTagFromName(playerid);

	if (strlen(NewName) != 0)
		Player[playerid][NameWithoutTag] = NewName; // Removes clan tag from the player name and stores the new name in NameWithoutTag variable.
	else
		Player[playerid][NameWithoutTag] = Player[playerid][Name];
}

// Initialize player variables
SetPlayerRoundParticipation(playerid, bool: playing, bool: updateIterator = true)
{
	Player[playerid][Playing] = playing;
	if (!updateIterator)
		return 1;

	if (playing)
	{
		if (!Iter_Contains(PlayersInRound, playerid))
			Iter_Add(PlayersInRound, playerid);
	}
	else if (Iter_Contains(PlayersInRound, playerid))
	{
		Iter_Remove(PlayersInRound, playerid);
	}
	return 1;
}

stock IsTeamAssistRound()
{
	return Current != -1 && AllowStartBase && !RoundPaused && !AllVsAllMode
		&& ((GameType == BASE && BaseStarted) || (GameType == ARENA && ArenaStarted));
}

stock IsAssistTeam(teamid)
{
	return teamid == ATTACKER || teamid == DEFENDER;
}

stock ResetVictimAssistDamage(victimid)
{
	if (victimid < 0 || victimid >= MAX_PLAYERS)
		return 0;

	for (new attackerid = 0; attackerid < MAX_PLAYERS; attackerid++)
		AssistDamage[attackerid][victimid] = 0.0;
	return 1;
}

stock ResetPlayerAssistDamage(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	ResetVictimAssistDamage(playerid);
	for (new victimid = 0; victimid < MAX_PLAYERS; victimid++)
		AssistDamage[playerid][victimid] = 0.0;
	return 1;
}

stock ResetAllAssistDamage()
{
	for (new attackerid = 0; attackerid < MAX_PLAYERS; attackerid++)
	{
		for (new victimid = 0; victimid < MAX_PLAYERS; victimid++)
			AssistDamage[attackerid][victimid] = 0.0;
	}
	return 1;
}

stock TrackAssistDamage(victimid, attackerid, Float: amount)
{
	if (!IsTeamAssistRound() || amount <= 0.0
		|| victimid < 0 || victimid >= MAX_PLAYERS || attackerid < 0 || attackerid >= MAX_PLAYERS
		|| !IsPlayerConnected(victimid) || !IsPlayerConnected(attackerid)
		|| !Player[victimid][Playing] || !Player[attackerid][Playing]
		|| !IsAssistTeam(Player[victimid][Team]) || !IsAssistTeam(Player[attackerid][Team])
		|| IsTeamTheSame(Player[victimid][Team], Player[attackerid][Team]))
		return 0;

	if (AssistDamage[attackerid][victimid] < ASSIST_MIN_DAMAGE)
	{
		AssistDamage[attackerid][victimid] += amount;
		if (AssistDamage[attackerid][victimid] > ASSIST_MIN_DAMAGE)
			AssistDamage[attackerid][victimid] = ASSIST_MIN_DAMAGE;
	}
	return 1;
}

stock AwardTeamAssists(victimid, killerid)
{
	if (!IsTeamAssistRound() || victimid < 0 || victimid >= MAX_PLAYERS
		|| killerid < 0 || killerid >= MAX_PLAYERS || victimid == killerid
		|| !IsPlayerConnected(victimid) || !IsPlayerConnected(killerid)
		|| !Player[victimid][Playing] || !Player[killerid][Playing]
		|| !IsAssistTeam(Player[victimid][Team]) || !IsAssistTeam(Player[killerid][Team])
		|| IsTeamTheSame(Player[victimid][Team], Player[killerid][Team]))
		return 0;

	new assistsAwarded;
	foreach (new assistantid : Player)
	{
		if (assistantid == victimid || assistantid == killerid || !Player[assistantid][Playing]
			|| !IsAssistTeam(Player[assistantid][Team])
			|| !IsTeamTheSame(Player[assistantid][Team], Player[killerid][Team])
			|| AssistDamage[assistantid][victimid] < ASSIST_MIN_DAMAGE)
			continue;

		Player[assistantid][RoundAssists]++;
		Player[assistantid][ATimeAssists]++;
		UpdateRoundKillDmgTDmg(assistantid);
		SendClientMessage(
			assistantid, -1,
			va_return("%s\xBB Assist: {FFFFFF}You helped eliminate %s.", TextColor[Player[assistantid][Team]], Player[victimid][Name]));
		assistsAwarded++;
	}
	return assistsAwarded;
}

forward DeferredRemoveBuildings(playerid, connectionGeneration);
public DeferredRemoveBuildings(playerid, connectionGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration)
		return 0;

	RemoveUselessObjectsForPlayer(playerid);
	RemoveBuildingForPlayer(playerid, 1227, 0.0, 0.0, 0.0, 6000.0); // green bin
	RemoveBuildingForPlayer(playerid, 1432, 0.0, 0.0, 0.0, 6000.0); // dining table
	RemoveBuildingForPlayer(playerid, 1281, 0.0, 0.0, 0.0, 6000.0); // dining table 2
	RemoveBuildingForPlayer(playerid, 1594, 0.0, 0.0, 0.0, 6000.0); // chairs && table
	RemoveBuildingForPlayer(playerid, 3374, 0.0, 0.0, 0.0, 6000.0); // arena farm hay
	RemoveBuildingForPlayer(playerid, 3851, 0.0, 0.0, 0.0, 6000.0); // wangcars windows
	RemoveBuildingForPlayer(playerid, 3850, 0.0, 0.0, 0.0, 6000.0); // wangcars windows
	RemoveBuildingForPlayer(playerid, 1265, 0.0, 0.0, 0.0, 6000.0); // trash bag 1
	RemoveBuildingForPlayer(playerid, 1450, 0.0, 0.0, 0.0, 6000.0); // boxes
	RemoveBuildingForPlayer(playerid, 1264, 0.0, 0.0, 0.0, 6000.0); // trash bag 2
	RemoveBuildingForPlayer(playerid, 1280, 0.0, 0.0, 0.0, 6000.0); // bench
	RemoveBuildingForPlayer(playerid, 1649, 0.0, 0.0, 0.0, 6000.0); // small glass
	RemoveBuildingForPlayer(playerid, 3857, 0.0, 0.0, 0.0, 6000.0); // smash glasses
	RemoveBuildingForPlayer(playerid, 1342, 0.0, 0.0, 0.0, 6000.0); // stand1
	RemoveBuildingForPlayer(playerid, 1340, 0.0, 0.0, 0.0, 6000.0); // stand2
	RemoveBuildingForPlayer(playerid, 1341, 0.0, 0.0, 0.0, 6000.0); // stand3
	RemoveBuildingForPlayer(playerid, 1570, 0.0, 0.0, 0.0, 6000.0); // stand4
	RemoveBuildingForPlayer(playerid, 1349, 0.0, 0.0, 0.0, 6000.0); // shopping cart
	RemoveBuildingForPlayer(playerid, 1408, 0.0, 0.0, 0.0, 6000.0); // fence
	RemoveBuildingForPlayer(playerid, 1460, 0.0, 0.0, 0.0, 6000.0); // fence2
	RemoveBuildingForPlayer(playerid, 1418, 0.0, 0.0, 0.0, 6000.0); // fence3
	RemoveBuildingForPlayer(playerid, 1446, 0.0, 0.0, 0.0, 6000.0); // fence4
	RemoveBuildingForPlayer(playerid, 3036, -2176.0, 660.0, 50.0, 200.0); // chinatown gate
	RemoveBuildingForPlayer(playerid, 5464, 1902, -1309, 29, 200.0); // jeff constr
	return 1;
}

InitPlayer(playerid)
{
	ResetPlayerAssistDamage(playerid);
	DeathBloodObject[playerid]                 = INVALID_OBJECT_ID;
	DeadBodyFlareObject[playerid]              = INVALID_OBJECT_ID;
	//added by zyph
	Player[playerid][DmPosX]				   = -1.0;
	Player[playerid][DmPosY]				   = -1.0;
	Player[playerid][DmPosZ]				   = -1.0;
	Player[playerid][DmAngle]				   = -1.0;
	//end added by zyph
	Player[playerid][Level]					   = 0;
	UpdatePlayerAdminGroup(playerid);
	Player[playerid][Lvl] 					   = 0;
	Player[playerid][Exp] 					   = 0;
	Player[playerid][Weather]				   = MainWeather;
	Player[playerid][Time]					   = MainTime;
	Player[playerid][Team]					   = NON;
	Player[playerid][NetCheckTimeoutTimestamp] = -1;
	Player[playerid][DMReadd]				   = -1;
	Player[playerid][ChatChannel]			   = -1;
	Player[playerid][RoundKills]			   = 0;
	Player[playerid][RoundDeaths]			   = 0;
	Player[playerid][RoundAssists]			   = 0;
	Player[playerid][RoundDamage]			   = 0.0;
	Player[playerid][WeaponPicked]			   = 0;
	Player[playerid][TotalKills]			   = 0;
	Player[playerid][TotalDeaths]			   = 0;
	Player[playerid][TotalDamage]			   = 0.0;
	Player[playerid][ATimeDeaths]              = 0;
	Player[playerid][ATimeKills]               = 0;
	Player[playerid][ATimeAssists]             = 0;
	Player[playerid][ATimeDMG]                 = 0;
	Player[playerid][ClutchesWon]              = 0;
	Player[playerid][AKillDGL]                 = 0;
	Player[playerid][AKillPNCH]                = 0;
	Player[playerid][AKillM]                   = 0;
	Player[playerid][AKillSHT]                 = 0;
	Player[playerid][AKillSP]                  = 0;
	Player[playerid][AKillSN]                  = 0;
	Player[playerid][AKillAK]                  = 0;
	Player[playerid][ACKick]				   = 0;
	Player[playerid][IsSpectatingID]		   = -1;
	Player[playerid][OutOfArena]			   = MAX_ZONE_LIMIT_WARNINGS;
	Player[playerid][FPSKick]				   = 0;
	Player[playerid][HighFPSKick]			   = 0;
	Player[playerid][PingKick]				   = 0;
	Player[playerid][PacketKick]			   = 0;
	Player[playerid][LastVehicle]			   = 0;
	Player[playerid][LastEnterVehicleAttempt]  = 0;
	Player[playerid][TimesSpawned]			   = 0;
	Player[playerid][VWorld]				   = 1;
	Player[playerid][lastChat]				   = GetTickCount();
	Player[playerid][LastPauseRequest]		   = 0;
	Player[playerid][LastAskLeader]			   = 0;
	Player[playerid][LastTimeSynced]		   = GetTickCount();
	Player[playerid][RoundPlayed]			   = 0;
	Player[playerid][shotsHit]				   = 0;
	Player[playerid][TotalBulletsFired]		   = 0;
	Player[playerid][TotalshotsHit]			   = 0;
	Player[playerid][RconTry]				   = 0;
	Player[playerid][NetCheck]				   = true;
	Player[playerid][FPSCheck]				   = true;
	Player[playerid][PingCheck]				   = true;
	Player[playerid][PLCheck]				   = true;
	Player[playerid][SpectatingRound]		   = -1;
	Player[playerid][SpectatingType]		   = -1;
	InitPlayerSpectate(playerid);
	ForgetSpectator(playerid);
	Player[playerid][HitBy]				 = INVALID_PLAYER_ID;
	Player[playerid][HitWith]			 = 47;
	Player[playerid][HitSound]			 = 17802;
	Player[playerid][GetHitSound]		 = 1131;
	Player[playerid][TipsEnabled]        = 0;
	Player[playerid][IsFrozen]           = false;
	Player[playerid][AlreadyDying]       = false;
	format(Player[playerid][PlayerTypeByWeapon], 16, "None");
	Player[playerid][LastEditWepLimit]	 = -1;
	Player[playerid][LastEditWeaponSlot] = -1;
	Player[playerid][challengerid]		 = -1;
	Player[playerid][DuelRequestedByGeneration] = 0;
	Player[playerid][duelweap1]			 = 0;
	Player[playerid][duelweap2]			 = 0;
	Player[playerid][DuelsWon]			 = 0;
	Player[playerid][DuelsLost]			 = 0;
	Player[playerid][LastMsgr]			 = -1;
	Player[playerid][blockedid]			 = -1;
	Player[playerid][FightStyle]		 = 4;
	SetPlayerFightingStyle(playerid, Player[playerid][FightStyle]);
	TargetInfoData[playerid][TargetInfoLastTarget] = INVALID_PLAYER_ID;
	TargetInfoData[playerid][TargetInfoLastTick] = GetTickCount();
	TargetInfoData[playerid][TargetInfoTimer] = 0;
	ChangingSkinOfTeam[playerid]			  = -1;
	Player[playerid][RequestedClass]		  = NON;
	Player[playerid][GunmenuModdingType]	  = 0;
	Player[playerid][GunmenuModdingIndex]	  = 0;
	Player[playerid][RoundTDStyle]			  = 0;
	Player[playerid][PreAllVsAllStyle]		  = 0;

	Player[playerid][Logged]				  = false;
	Player[playerid][IgnoreSpawn]			  = false;
	Player[playerid][InDM]					  = false;
	Player[playerid][InDuel]				  = false;
	SetPlayerRoundParticipation(playerid, false);
	Player[playerid][WasInCP]		   = false;
	Player[playerid][Spectating]	   = false;
	Player[playerid][BeingSpeced]	   = false;
	Player[playerid][WasInBase]		   = false;
	Player[playerid][WasInTeam]		   = NON;
	Player[playerid][TextDrawOnScreen] = false;
	Player[playerid][Spawned]		   = false;
	Player[playerid][IsAFK]			   = false;
	Player[playerid][AskingForHelp]	   = false;
	Player[playerid][Mute]			   = false;
	Player[playerid][ToAddInRound]	   = false;
	InitPlayerInterface(playerid);
	Player[playerid][blockedall]			   = false;
	Player[playerid][OnGunmenu]				   = false;
	Player[playerid][GunmenuStyle]			   = GUNMENU_STYLE_DIALOG;
	Player[playerid][TempNetcheck]			   = false;
	TempNetcheckGeneration[playerid]++;
	Player[playerid][HasDeathQuote]			   = false;
	Player[playerid][HadParachuteBeforeFreeze] = false;

	ResetPlayerMoney(playerid);
	SetPlayerColor(playerid, 0xAAAAAAAA);
	SetTimerEx("DeferredRemoveBuildings", 350, false, "ii", playerid, PlayerConnectionGeneration[playerid]);

	LoadPlayerTextDraws(playerid);

	for (new i = 0; i < MAX_PLAYERS; i++) TeamBarShownForPlayer[playerid][i] = false;

	ShowTeamCornerInfo(playerid);

	CreatePlayerLabels(playerid);
	Player[playerid][DamageBubbleTick] = 0;
	Player[playerid][ShowFloatingDamage] = true;

	InitPlayerMessageBox(playerid);
	InitPlayerNames(playerid);
	InitPlayerWeaponBind(playerid);
	InitPlayerVisualDamage(playerid);
	InitPlayerVoting(playerid);
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	DestroyPlayerLabels(playerid);
	DestroyPlayerFloatingLabels(playerid);
	Iter_Remove(PlayersInVehicles, playerid);
	return 1;
}

CreatePlayerLabels(playerid)
{
	Player[playerid][InfoLabel] = Create3DTextLabel(" ", -1, 0, 0, 0, DRAW_DISTANCE - 3.2, 0, true);
	Attach3DTextLabelToPlayer(Player[playerid][InfoLabel], playerid, 0, 0, -0.7);

	Player[playerid][DamageLabel] = Create3DTextLabel(" ", -1, 0, 0, 0, DRAW_DISTANCE - 3.2, 0, true);
	Attach3DTextLabelToPlayer(Player[playerid][DamageLabel], playerid, 0, 0, 2.0);

	Player[playerid][PauseLabel] = Create3DTextLabel(" ", 0xFFFFFFFF, 0, 0, 0, DRAW_DISTANCE - 3.2, 0, true);
	Attach3DTextLabelToPlayer(Player[playerid][PauseLabel], playerid, 0, 0, 0.4);
}

DestroyPlayerLabels(playerid)
{
	Delete3DTextLabel(Player[playerid][InfoLabel]);
	Delete3DTextLabel(Player[playerid][DamageLabel]);
	Delete3DTextLabel(Player[playerid][PauseLabel]);
}

LoginPlayer(playerid, DBResult: res)
{
	new iString[256];
	new IP[16];
	GetPlayerIp(playerid, IP, sizeof(IP));
	new day, month, year;
	getdate(year, month, day);
	if (!DatabaseExecuteWrite(
		va_return(
			"UPDATE `Players` SET `IP` = '%q', `LastSeen_Day` = %d, `LastSeen_Month` = %d, `LastSeen_Year` = %d WHERE `Name` = '%q'",
			IP, day, month, year, Player[playerid][Name]),
		"save player IP and last-seen date"))
		return 0;

	// Load level
	DB_GetFieldStringByName(res, "Level", iString, sizeof(iString));
	Player[playerid][Level] = strval(iString);
	if (IsPlayerAdmin(playerid) && Player[playerid][Level] < 5)
	{
		Player[playerid][Level] = 5;
		DatabaseExecuteWrite(
			va_return("UPDATE `Players` SET `Level` = 5 WHERE `Name` = '%q'", Player[playerid][Name]),
			"save RCON admin level on login");
	}
	UpdatePlayerAdminGroup(playerid);

	// Load Weather
	DB_GetFieldStringByName(res, "Weather", iString, sizeof(iString));
	Player[playerid][Weather] = strval(iString);

	// Load time
	DB_GetFieldStringByName(res, "Time", iString, sizeof(iString));
	Player[playerid][Time] = strval(iString);

	// Load ChatChannel
	DB_GetFieldStringByName(res, "ChatChannel", iString, sizeof(iString));
	Player[playerid][ChatChannel] = strval(iString);

	// Load NetCheck
	DB_GetFieldStringByName(res, "NetCheck", iString, sizeof(iString));
	Player[playerid][NetCheck] = bool: strval(iString);

	// Load HitSound
	DB_GetFieldStringByName(res, "HitSound", iString, sizeof(iString));
	Player[playerid][HitSound] = strval(iString);

	// Load GetHitSound
	DB_GetFieldStringByName(res, "GetHitSound", iString, sizeof(iString));
	Player[playerid][GetHitSound] = strval(iString);

	// Load Duels won
	DB_GetFieldStringByName(res, "DWon", iString, sizeof(iString));
	Player[playerid][DuelsWon] = strval(iString);

	// Load Duels Lost
	DB_GetFieldStringByName(res, "DLost", iString, sizeof(iString));
	Player[playerid][DuelsLost] = strval(iString);

	// Load ShowSpecs
	DB_GetFieldStringByName(res, "InterfaceSpec", iString, sizeof(iString));
	PlayerInterface[playerid][INTERFACE_SPEC] = strval(iString) != 0;

	// Load Fighting Style
	DB_GetFieldStringByName(res, "FightStyle", iString, sizeof(iString));
	Player[playerid][FightStyle] = strval(iString);
	SetPlayerFightingStyle(playerid, Player[playerid][FightStyle]);

	// Load Death Messages
	DB_GetFieldStringByName(res, "DeathMessage", iString, sizeof(iString));
	DeathMessageStr[playerid][0] = EOS;
	strcat(DeathMessageStr[playerid], iString, 64);
	if (strcmp("NO_DEATH_MESSAGE", DeathMessageStr[playerid], false) != 0)
		Player[playerid][HasDeathQuote] = true;

	// Load Show net textdraws
	DB_GetFieldStringByName(res, "InterfaceNet", iString, sizeof(iString));
	PlayerInterface[playerid][INTERFACE_NET] = strval(iString) != 0;

	// Load weapon key binds
	new fieldName[12], value;
	for (new i = 0; i < MAX_WEAPON_BINDS; i++)
	{
		format(fieldName, sizeof fieldName, "WeaponBind%d", i);
		DB_GetFieldStringByName(res, fieldName, iString, sizeof(iString));
		value = strval(iString);
		if (value != -1)
			WeaponBindData[playerid][BindKeyWeapon][i] = value;
	}

	// Load Gunmenu Style
	DB_GetFieldStringByName(res, "GunmenuStyle", iString, sizeof(iString));
	Player[playerid][GunmenuStyle] = strval(iString);

	// Load Weapon Binding
	DB_GetFieldStringByName(res, "WeaponBinding", iString, sizeof(iString));
	Player[playerid][WeaponBinding] = strval(iString) != 0;

	// Load player markers setting
	DB_GetFieldStringByName(res, "PlayerMarkers", iString, sizeof(iString));
	Player[playerid][PlayerMarkers] = strval(iString) != 0;

	// Load round textdraws style
	DB_GetFieldStringByName(res, "RoundTDStyle", iString, sizeof(iString));
	Player[playerid][RoundTDStyle] = strval(iString);
	if (Player[playerid][RoundTDStyle] < 0 || Player[playerid][RoundTDStyle] > 4)
		Player[playerid][RoundTDStyle] = 0;
	Player[playerid][PreAllVsAllStyle] = Player[playerid][RoundTDStyle];

	// Load Tips Message Status
	DB_GetFieldStringByName(res, "TipsEnabled", iString, sizeof(iString));
	Player[playerid][TipsEnabled] = strval(iString);

	// Load lvl
	DB_GetFieldStringByName(res, "Lvl", iString, sizeof(iString));
	new level = strval(iString);

	// Load exp
	DB_GetFieldStringByName(res, "Exp", iString, sizeof(iString));
	new exp = strval(iString);
	if (!NormalizePlayerProgress(playerid, level, exp))
		return 0;
	Player[playerid][Lvl] = level;
	Player[playerid][Exp] = exp;

	// Load Show HP textdraws
	DB_GetFieldStringByName(res, "InterfaceHP", iString, sizeof(iString));
	PlayerInterface[playerid][INTERFACE_HP] = strval(iString) != 0;

	// Load all time kills
	DB_GetFieldStringByName(res, "TKills", iString, sizeof(iString));
	Player[playerid][ATimeKills] = strval(iString);

	// Load all time deaths
	DB_GetFieldStringByName(res, "TDeaths", iString, sizeof(iString));
	Player[playerid][ATimeDeaths] = strval(iString);

	// Load all time assists
	DB_GetFieldStringByName(res, "TAssists", iString, sizeof(iString));
	Player[playerid][ATimeAssists] = strval(iString);

	// Load all time damage
	DB_GetFieldStringByName(res, "TTDMG", iString, sizeof(iString));
	Player[playerid][ATimeDMG] = strval(iString);

	// Load weapon kills and clutches
	DB_GetFieldStringByName(res, "AKWD", iString, sizeof(iString));
	Player[playerid][AKillDGL] = strval(iString);
	DB_GetFieldStringByName(res, "AKWP", iString, sizeof(iString));
	Player[playerid][AKillPNCH] = strval(iString);
	DB_GetFieldStringByName(res, "AKWM", iString, sizeof(iString));
	Player[playerid][AKillM] = strval(iString);
	DB_GetFieldStringByName(res, "AKWSN", iString, sizeof(iString));
	Player[playerid][AKillSN] = strval(iString);
	DB_GetFieldStringByName(res, "AKWS", iString, sizeof(iString));
	Player[playerid][AKillSP] = strval(iString);
	DB_GetFieldStringByName(res, "AKWSHT", iString, sizeof(iString));
	Player[playerid][AKillSHT] = strval(iString);
	DB_GetFieldStringByName(res, "AKWAK", iString, sizeof(iString));
	Player[playerid][AKillAK] = strval(iString);
	DB_GetFieldStringByName(res, "ClutchesWon", iString, sizeof(iString));
	Player[playerid][ClutchesWon] = strval(iString);

	// Load Player Skin
	DB_GetFieldStringByName(res, "pSkin", iString, sizeof(iString));
	Player[playerid][pSkin] = strval(iString);

	Player[playerid][Logged] = true;
	UpdateRoundKillDmgText(playerid);
	return 1;
}

stock NormalizePlayerProgress(playerid, &level, &exp)
{
	new originalLevel = level;
	new originalExp = exp;

	if (level < 0)
		level = 0;
	else if (level > 19)
		level = 19;

	if (exp < 0)
		exp = 0;

	for (new i = 0; i < 20 && exp >= 100 + level * 100; i++)
	{
		exp -= 100 + level * 100;
		if (level == 19)
		{
			exp = 0;
			break;
		}
		level++;
	}

	if (level != originalLevel || exp != originalExp)
	{
		if (!DatabaseExecuteWrite(
			va_return("UPDATE `Players` SET `Lvl`=%d, `Exp`=%d WHERE `Name`='%q'", level, exp, Player[playerid][Name]),
			"normalize player progress"))
			return 0;
	}
	return 1;
}

StorePlayerVariablesMin(playerid)
{
	for (new i = 0; i < SAVE_SLOTS; i++)
	{
		if (strlen(SaveVariables[i][pName]) >= 2)
			continue;

		SaveVariables[i][pTeam]		 = Player[playerid][Team];
		SaveVariables[i][RKills]	 = Player[playerid][RoundKills];
		SaveVariables[i][RDeaths]	 = Player[playerid][RoundDeaths];
		SaveVariables[i][RAssists]	 = Player[playerid][RoundAssists];
		SaveVariables[i][RDamage]	 = Player[playerid][RoundDamage];
		SaveVariables[i][TKills]	 = Player[playerid][TotalKills];
		SaveVariables[i][TDeaths]	 = Player[playerid][TotalDeaths];
		SaveVariables[i][TDamage]	 = Player[playerid][TotalDamage];
		SaveVariables[i][ATKills]    = Player[playerid][ATimeKills];
		SaveVariables[i][ATDeaths]   = Player[playerid][ATimeDeaths];
		SaveVariables[i][ATAssists]	 = Player[playerid][ATimeAssists];
		SaveVariables[i][ATDMG]	 	 = Player[playerid][ATimeDMG];
		SaveVariables[i][ATClutches] = Player[playerid][ClutchesWon];
		SaveVariables[i][TPlayed]	 = Player[playerid][RoundPlayed];
		SaveVariables[i][sWasInBase] = Player[playerid][WasInBase];

		OnPlayerAmmoUpdate(playerid);
		SaveVariables[i][iAccuracy]	   = floatround(Player[playerid][Accuracy], floatround_round);
		SaveVariables[i][tshotsHit]	   = Player[playerid][TotalshotsHit];
		SaveVariables[i][tBulletsShot] = Player[playerid][TotalBulletsFired];

		SaveVariables[i][gHealth]	   = 0;
		SaveVariables[i][gArmour]	   = 0;

		SaveVariables[i][RoundID]	   = Current;
		SaveVariables[i][ToBeAdded]	   = false;
		SaveVariables[i][CheckScore]   = true;

		new weapID, weapAmmo = 0;
		GetPlayerWeaponData(playerid, 8, weapID, weapAmmo);
		if (weapID == WEAPON_GRENADE)
			SaveVariables[i][SavedGrenadeAmmo] = weapAmmo;
		else
			SaveVariables[i][SavedGrenadeAmmo] = 0;

		format(SaveVariables[i][pName], 24, Player[playerid][Name]);
		format(SaveVariables[i][pNameWithoutTag], 24, Player[playerid][NameWithoutTag]);

		if (GetPlayerWeapon(playerid) == WEAPON_PARACHUTE)
			SaveVariables[i][HadParachute] = 1;
		else
			SaveVariables[i][HadParachute] = 0;

		if (IsPlayerInAnyVehicle(playerid))
		{
			SaveVariables[i][pVehicleID] = GetPlayerVehicleID(playerid);
			SaveVariables[i][pSeatID]	 = GetPlayerVehicleSeat(playerid);
		}
		else
		{
			SaveVariables[i][pVehicleID] = -1;
			SaveVariables[i][pSeatID]	 = -1;
		}
		break;
	}
	return 1;
}

StorePlayerVariables(playerid)
{
	for (new i = 0; i < SAVE_SLOTS; i++)
	{
		if (strlen(SaveVariables[i][pName]) >= 2)
			continue;

		format(SaveVariables[i][pName], 24, Player[playerid][Name]);
		format(SaveVariables[i][pNameWithoutTag], 24, Player[playerid][NameWithoutTag]);

		GetPlayerPos(playerid, SaveVariables[i][pCoords][0], SaveVariables[i][pCoords][1], SaveVariables[i][pCoords][2]);
		GetPlayerFacingAngle(playerid, SaveVariables[i][pCoords][3]);

		if (!Player[playerid][ToAddInRound])
		{
			SaveVariables[i][gHealth] = GetHP(playerid);
			SaveVariables[i][gArmour] = GetAP(playerid);
		}
		else
		{
			SaveVariables[i][gArmour] = 100;
			SaveVariables[i][gHealth] = 100;
		}

		SaveVariables[i][pTeam]		 = Player[playerid][Team];
		SaveVariables[i][pInterior]	 = GetPlayerInterior(playerid);
		SaveVariables[i][pVWorld]	 = GetPlayerVirtualWorld(playerid);

		SaveVariables[i][RKills]	 = Player[playerid][RoundKills];
		SaveVariables[i][RDeaths]	 = Player[playerid][RoundDeaths];
		SaveVariables[i][RAssists]	 = Player[playerid][RoundAssists];
		SaveVariables[i][RDamage]	 = Player[playerid][RoundDamage];
		SaveVariables[i][TKills]	 = Player[playerid][TotalKills];
		SaveVariables[i][TDeaths]	 = Player[playerid][TotalDeaths];
		SaveVariables[i][TDamage]	 = Player[playerid][TotalDamage];
		SaveVariables[i][ATKills]    = Player[playerid][ATimeKills];
		SaveVariables[i][ATDeaths]   = Player[playerid][ATimeDeaths];
		SaveVariables[i][ATAssists]	 = Player[playerid][ATimeAssists];
		SaveVariables[i][ATDMG]	 	 = Player[playerid][ATimeDMG];
		SaveVariables[i][ATClutches] = Player[playerid][ClutchesWon];
		SaveVariables[i][TPlayed]	 = Player[playerid][RoundPlayed];
		SaveVariables[i][sWasInBase] = Player[playerid][WasInBase];

		OnPlayerAmmoUpdate(playerid);
		SaveVariables[i][iAccuracy]	   = floatround(Player[playerid][Accuracy], floatround_round);
		SaveVariables[i][tshotsHit]	   = Player[playerid][TotalshotsHit];
		SaveVariables[i][tBulletsShot] = Player[playerid][TotalBulletsFired];

		for (new j = 0; j < MAX_GUNMENU_GUNS; j++) { SaveVariables[i][HadGunmenuGun][j] = GunmenuData[j][HasGun][playerid]; }

		SaveVariables[i][RoundID]	 = Current;
		SaveVariables[i][ToBeAdded]	 = true;
		SaveVariables[i][CheckScore] = true;

		new weapID, weapAmmo = 0;
		GetPlayerWeaponData(playerid, 8, weapID, weapAmmo);
		if (weapID == WEAPON_GRENADE)
			SaveVariables[i][SavedGrenadeAmmo] = weapAmmo;
		else
			SaveVariables[i][SavedGrenadeAmmo] = 0;

		if (GetPlayerWeapon(playerid) == WEAPON_PARACHUTE)
			SaveVariables[i][HadParachute] = 1;
		else
			SaveVariables[i][HadParachute] = 0;

		if (IsPlayerInAnyVehicle(playerid))
		{
			SaveVariables[i][pVehicleID] = GetPlayerVehicleID(playerid);
			SaveVariables[i][pSeatID]	 = GetPlayerVehicleSeat(playerid);
		}
		else
		{
			SaveVariables[i][pVehicleID] = -1;
			SaveVariables[i][pSeatID]	 = -1;
		}

		if (Current != -1 && (!AllowStartBase || ElapsedTime < 20) && (Player[playerid][ToAddInRound] || Player[playerid][Playing]))
			SaveVariables[i][WasCrashedInStart] = true;

		break;
	}
}

// Note: this function returns team id if the player is supposed to be re-added to round, otherwise -1
ShouldPlayerBeReadded(playerid)
{
	for (new i = 0; i < SAVE_SLOTS; i++)
	{
		if (strlen(SaveVariables[i][pName]) >= 2 && strcmp(SaveVariables[i][pName], Player[playerid][Name], true) == 0)
		{
			if (SaveVariables[i][RoundID] != Current || Current == -1 || !SaveVariables[i][ToBeAdded])
				return -1;
			else
				return SaveVariables[i][pTeam];
		}
	}
	return -1;
}

LoadPlayerVariables(playerid)
{
	new iString[160];

	for (new i = 0; i < SAVE_SLOTS; i++)
	{
		if (strlen(SaveVariables[i][pName]) >= 2 && strcmp(SaveVariables[i][pName], Player[playerid][Name], true) == 0)
		{
			if (SaveVariables[i][RoundID] != Current || Current == -1)
			{
				Player[playerid][TotalKills]		= SaveVariables[i][TKills];
				Player[playerid][TotalDeaths]		= SaveVariables[i][TDeaths];
				Player[playerid][TotalDamage]		= SaveVariables[i][TDamage];
				Player[playerid][ATimeKills]		= SaveVariables[i][ATKills];
				Player[playerid][ATimeDeaths]		= SaveVariables[i][ATDeaths];
				Player[playerid][ATimeAssists]		= SaveVariables[i][ATAssists];
				Player[playerid][ATimeDMG]			= SaveVariables[i][ATDMG];
				Player[playerid][ClutchesWon]		= SaveVariables[i][ATClutches];
				Player[playerid][RoundPlayed]		= SaveVariables[i][TPlayed];
				Player[playerid][Accuracy]			= SaveVariables[i][iAccuracy];
				Player[playerid][TotalshotsHit]		= SaveVariables[i][tshotsHit];
				Player[playerid][TotalBulletsFired] = SaveVariables[i][tBulletsShot];

				ResetSaveVariables(i);

				if (Player[playerid][Team] == ATTACKER)
				{
					format(teamRole, sizeof(teamRole), "(Attackers)");
				}
				else if (Player[playerid][Team] == ATTACKER_SUB)
				{
					format(teamRole, sizeof(teamRole), "(Attacker Sub)");
				}	
				else if (Player[playerid][Team] == DEFENDER)
				{
					format(teamRole, sizeof(teamRole), "(Defenders)");
				}
				else if (Player[playerid][Team] == DEFENDER_SUB)
				{
					format(teamRole, sizeof(teamRole), "(Defender Sub)");
				}
				else if (Player[playerid][Team] == REFEREE)
				{
					format(teamRole, sizeof(teamRole), "(Spectators)");
				}

				format(iString, sizeof(iString), "%s\xBB %s spawned as %s %s", TextColor[Player[playerid][Team]], Player[playerid][Name], TeamName[Player[playerid][Team]], teamRole);
				SendClientMessageToAll(-1, iString);
				UpdatePlayerHealthBar(playerid);

				UpdateRoundKillDmgTDmg(playerid);
				return 1;
			}
			else if (!SaveVariables[i][ToBeAdded])
			{
				Player[playerid][TotalKills]		= SaveVariables[i][TKills];
				Player[playerid][TotalDeaths]		= SaveVariables[i][TDeaths];
				Player[playerid][TotalDamage]		= SaveVariables[i][TDamage];
				Player[playerid][ATimeKills]		= SaveVariables[i][ATKills];
				Player[playerid][ATimeDeaths]		= SaveVariables[i][ATDeaths];
				Player[playerid][ATimeAssists]		= SaveVariables[i][ATAssists];
				Player[playerid][ATimeDMG]			= SaveVariables[i][ATDMG];
				Player[playerid][ClutchesWon]		= SaveVariables[i][ATClutches];
				Player[playerid][RoundPlayed]		= SaveVariables[i][TPlayed];
				Player[playerid][Accuracy]			= SaveVariables[i][iAccuracy];
				Player[playerid][TotalshotsHit]		= SaveVariables[i][tshotsHit];
				Player[playerid][TotalBulletsFired] = SaveVariables[i][tBulletsShot];
				Player[playerid][RoundKills]		= SaveVariables[i][RKills];
				Player[playerid][RoundDeaths]		= SaveVariables[i][RDeaths];
				Player[playerid][RoundAssists]		= SaveVariables[i][RAssists];
				Player[playerid][RoundDamage]		= SaveVariables[i][RDamage];
				Player[playerid][WasInBase]			= SaveVariables[i][sWasInBase];

				ResetSaveVariables(i);

				if (Player[playerid][Team] == ATTACKER)
				{
					format(teamRole, sizeof(teamRole), "(Attackers)");
				}
				else if (Player[playerid][Team] == ATTACKER_SUB)
				{
					format(teamRole, sizeof(teamRole), "(Attacker Sub)");
				}	
				else if (Player[playerid][Team] == DEFENDER)
				{
					format(teamRole, sizeof(teamRole), "(Defenders)");
				}
				else if (Player[playerid][Team] == DEFENDER_SUB)
				{
					format(teamRole, sizeof(teamRole), "(Defender Sub)");
				}
				else if (Player[playerid][Team] == REFEREE)
				{
					format(teamRole, sizeof(teamRole), "(Spectators)");
				}

				format(iString, sizeof(iString), "%s\xBB %s spawned as %s %s", TextColor[Player[playerid][Team]], Player[playerid][Name], TeamName[Player[playerid][Team]], teamRole);
				SendClientMessageToAll(-1, iString);
				UpdatePlayerHealthBar(playerid);

				UpdateRoundKillDmgTDmg(playerid);
				return 1;
			}

			SetPlayerRoundParticipation(playerid, true);
			Player[playerid][WasInBase] = true;

			Player[playerid][Team]				= SaveVariables[i][pTeam];
			Player[playerid][WasInTeam]			= Player[playerid][Team];

			Player[playerid][RoundKills]		= SaveVariables[i][RKills];
			Player[playerid][RoundDeaths]		= SaveVariables[i][RDeaths];
			Player[playerid][RoundAssists]		= SaveVariables[i][RAssists];
			Player[playerid][RoundDamage]		= SaveVariables[i][RDamage];
			Player[playerid][TotalKills]		= SaveVariables[i][TKills];
			Player[playerid][TotalDeaths]		= SaveVariables[i][TDeaths];
			Player[playerid][TotalDamage]		= SaveVariables[i][TDamage];
			Player[playerid][ATimeKills]		= SaveVariables[i][ATKills];
			Player[playerid][ATimeDeaths]		= SaveVariables[i][ATDeaths];
			Player[playerid][ATimeAssists]		= SaveVariables[i][ATAssists];
			Player[playerid][ATimeDMG]			= SaveVariables[i][ATDMG];
			Player[playerid][ClutchesWon]		= SaveVariables[i][ATClutches];
			Player[playerid][RoundPlayed]		= SaveVariables[i][TPlayed];
			Player[playerid][Accuracy]			= SaveVariables[i][iAccuracy];
			Player[playerid][TotalshotsHit]		= SaveVariables[i][tshotsHit];
			Player[playerid][TotalBulletsFired] = SaveVariables[i][tBulletsShot];

			if (!SaveVariables[i][WasCrashedInStart])
			{
				SetSpawnInfo(
					playerid, Player[playerid][Team], Skin[Player[playerid][Team]], SaveVariables[i][pCoords][0], SaveVariables[i][pCoords][1], SaveVariables[i][pCoords][2] + 1,
					SaveVariables[i][pCoords][3], 0, 0, 0, 0, 0, 0);

				SetPlayerInterior(playerid, SaveVariables[i][pInterior]);
				SetPlayerVirtualWorld(playerid, SaveVariables[i][pVWorld]);
			}
			else if (Current != -1)
			{
				if (GameType == BASE)
				{
					switch (Player[playerid][Team])
					{
						case ATTACKER:
							SetSpawnInfo(
								playerid, Player[playerid][Team], Skin[Player[playerid][Team]], BAttackerSpawn[Current][0] + RandomMinMax(-3, 3), BAttackerSpawn[Current][1] + RandomMinMax(-3, 3),
								BAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
						case DEFENDER:
							SetSpawnInfo(
								playerid, Player[playerid][Team], Skin[Player[playerid][Team]], BDefenderSpawn[Current][0] + RandomMinMax(-3, 3), BDefenderSpawn[Current][1] + RandomMinMax(-3, 3),
								BDefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
					}
					SetPlayerInterior(playerid, BInterior[Current]);
				}
				else if (GameType == ARENA)
				{
					switch (Player[playerid][Team])
					{
						case ATTACKER:
							SetSpawnInfo(
								playerid, Player[playerid][Team], Skin[Player[playerid][Team]], AAttackerSpawn[Current][0] + RandomMinMax(-3, 3), AAttackerSpawn[Current][1] + RandomMinMax(-3, 3),
								AAttackerSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
						case DEFENDER:
							SetSpawnInfo(
								playerid, Player[playerid][Team], Skin[Player[playerid][Team]], ADefenderSpawn[Current][0] + RandomMinMax(-3, 3), ADefenderSpawn[Current][1] + RandomMinMax(-3, 3),
								ADefenderSpawn[Current][2] + 0.5, 0, 0, 0, 0, 0, 0, 0);
					}
					SetPlayerInterior(playerid, AInterior[Current]);
				}
				SetPlayerVirtualWorld(playerid, 2);
			}

			Player[playerid][IgnoreSpawn] = true;
			SpawnPlayer(playerid);

			ShowTeamBarsForPlayer(playerid);
			CreatePlayerTeamBar(playerid);

			ShowTeamCornerInfo(playerid);
			UpdateTeamCornerInfo();

			UpdateTeamPlayerCount(Player[playerid][Team], false);
			SetHPAP(playerid, SaveVariables[i][gHealth], SaveVariables[i][gArmour]);
			UpdateTeamHP(Player[playerid][Team]);

			ColorFix(playerid);
			SetPlayerTeam(playerid, NO_TEAM);
			RadarFix();
			SetCPForPlayer(playerid);

			if (GameType == BASE)
			{
				new team = Player[playerid][Team];
				new ct	 = 0;
				for (new j = 0; j < MAX_GUNMENU_GUNS; j++)
				{
					if (SaveVariables[i][HadGunmenuGun][j])
					{
						if (GunmenuData[j][TimesSelected][team] >= GunmenuData[j][GunLimit])
						{
							ct = -1;
							ResetPlayerGunmenu(playerid, false);
							ShowPlayerGunmenu(playerid, 500);
							SendErrorMessage(
								playerid, va_return("This weapon is no longer available because limit has exceeded. [%d/%d]", GunmenuData[j][GunLimit], GunmenuData[j][GunLimit]));
							break;
						}
						else
						{
							ct++;
							GivePlayerWeapon(playerid, GunmenuData[j][GunID], GunmenuData[j][GunAmmo]);
							GunmenuData[j][HasGun][playerid] = true;
							GunmenuData[j][TimesSelected][team]++;
							GunmenuSelectionTeam[playerid] = team;
						}
					}
				}
				if (ct == 0)
				{
					ResetPlayerGunmenu(playerid, false);
					ShowPlayerGunmenu(playerid, 500);
				}
				if (SaveVariables[i][HadParachute] == 1)
				{
					GivePlayerWeapon(playerid, WEAPON_PARACHUTE, 1);
					SetPlayerArmedWeapon(playerid, WEAPON_PARACHUTE);
				}
				else
					SetPlayerArmedWeapon(playerid, 0);

				if (SaveVariables[i][pVehicleID] != -1)
				{
					new vehicleRejoinGeneration = ++PlayerVehicleRejoinGeneration[playerid];
					if (!SetTimerEx(
						"RespawnInVehicleAfterComeBack", 500, false, "iiiii",
						playerid, SaveVariables[i][pVehicleID], SaveVariables[i][pSeatID],
						PlayerConnectionGeneration[playerid], vehicleRejoinGeneration))
					{
						PlayerVehicleRejoinGeneration[playerid]++;
						printf("[AttDef] Failed to schedule vehicle restoration for player %d.", playerid);
					}
				}
			}
			else if (GameType == ARENA)
			{
				new team = Player[playerid][Team];
				for (new j = 0; j < MAX_GUNMENU_GUNS; j++)
				{
					if (SaveVariables[i][HadGunmenuGun][j])
					{
						GivePlayerWeapon(playerid, GunmenuData[j][GunID], GunmenuData[j][GunAmmo]);
						GunmenuData[j][HasGun][playerid] = true;
						GunmenuData[j][TimesSelected][team]++;
						GunmenuSelectionTeam[playerid] = team;
					}
				}
				GangZoneShowForPlayer(playerid, ArenaZone, GANGZONE_ARENA);
			}
			if (RoundPaused)
			{
				TogglePlayerControllable(playerid, 0);
				ShowPauseTDForAll();
			}
			else
				TogglePlayerControllable(playerid, 1);

			format(iString, sizeof(iString), ""COL_PRIM"\xBB Player "COL_SEC"%s "COL_PRIM"has been re-added. " COL_PRIM "Data retrieved successfully.", Player[playerid][Name]);
			SendClientMessageToAll(-1, iString);

			if (!Player[playerid][FakePacketRenovation] && Player[playerid][NetCheck] && NetStats_GetConnectedTime(playerid) < 300000)
			{
				ScheduleFakePacketRenovation(playerid, 30000, false, NetStats_PacketLossPercent(playerid));
			}

			UpdateRoundKillDmgTDmg(playerid);

			SaveVariables[i][PauseWait] = false;

			ResetSaveVariables(i);
			return 1;
		}
	}
	if (Player[playerid][Team] == ATTACKER)
	{
		format(teamRole, sizeof(teamRole), "(Attackers)");
	}
	else if (Player[playerid][Team] == ATTACKER_SUB)
	{
		format(teamRole, sizeof(teamRole), "(Attacker Sub)");
	}	
	else if (Player[playerid][Team] == DEFENDER)
	{
		format(teamRole, sizeof(teamRole), "(Defenders)");
	}
	else if (Player[playerid][Team] == DEFENDER_SUB)
	{
		format(teamRole, sizeof(teamRole), "(Defender Sub)");
	}
	else if (Player[playerid][Team] == REFEREE)
	{
		format(teamRole, sizeof(teamRole), "(Spectators)");
	}

	format(iString, sizeof(iString), "%s\xBB %s spawned as %s %s", TextColor[Player[playerid][Team]], Player[playerid][Name], TeamName[Player[playerid][Team]], teamRole);
	SendClientMessageToAll(-1, iString);
	UpdatePlayerHealthBar(playerid);
	UpdateRoundKillDmgText(playerid);
	return 1;
}

ResetSaveVariables(index)
{
	SaveVariables[index][pName][0] = EOS;

	SaveVariables[index][pCoords][0]		= 0.0;
	SaveVariables[index][pCoords][1]		= 0.0;
	SaveVariables[index][pCoords][2]		= 0.0;
	SaveVariables[index][pCoords][3]		= 0.0;

	SaveVariables[index][gHealth]			= 0;
	SaveVariables[index][gArmour]			= 0;
	SaveVariables[index][pInterior]			= 0;
	SaveVariables[index][pVWorld]			= 0;
	SaveVariables[index][pTeam]				= 0;
	SaveVariables[index][RKills]			= 0;
	SaveVariables[index][RDeaths]			= 0;
	SaveVariables[index][RAssists]			= 0;
	SaveVariables[index][RDamage]			= 0.0;
	SaveVariables[index][TKills]			= 0;
	SaveVariables[index][TDeaths]			= 0;
	SaveVariables[index][TDamage]			= 0.0;
	SaveVariables[index][ATKills]			= 0;
	SaveVariables[index][ATDeaths]			= 0;
	SaveVariables[index][ATAssists]			= 0;
	SaveVariables[index][ATDMG]				= 0;
	SaveVariables[index][ATClutches]		= 0;
	SaveVariables[index][sWasInBase]		= false;
	SaveVariables[index][RoundID]			= -1;
	SaveVariables[index][WasCrashedInStart] = false;
	SaveVariables[index][ToBeAdded]			= false;
	SaveVariables[index][CheckScore]		= false;
	SaveVariables[index][PauseWait]			= false;
	SaveVariables[index][pVehicleID]		= -1;
	SaveVariables[index][pSeatID]			= -1;
	SaveVariables[index][HadParachute]		= 0;
	SaveVariables[index][SavedGrenadeAmmo]	= 0;
	for (new i = 0; i < MAX_GUNMENU_GUNS; i++) SaveVariables[index][HadGunmenuGun][i] = false;
}

ClearPlayerVariables()
{
	for (new i = 0; i < SAVE_SLOTS; i++) { ResetSaveVariables(i); }
}

stock GetNextLevelExp(level)
{
	if (level >= 0 && level <= 19)
	{
		return 100 + (level * 100);
	}
	return 0;
}

stock UpdateLvl(playerid)
{
	new iString[128];
	new exp = Player[playerid][Exp];
	new level = Player[playerid][Lvl];

	if (level < 0)
		level = 0;
	else if (level > 19)
		level = 19;
	if (exp < 0)
		exp = 0;

	for (new i = 0; i < 20 && exp >= GetNextLevelExp(level); i++)
	{
		exp -= GetNextLevelExp(level);
		if (level == 19)
		{
			exp = 0;
			break;
		}
		level++;
	}

	if (Player[playerid][Lvl] != level)
	{
		Player[playerid][Lvl] = level;
		format(iString, sizeof(iString), "\xBB Congratulations! You've reached level %d!", level);
		SendClientMessage(playerid, -1, iString);
	}

	Player[playerid][Exp] = exp;
	format(iString, sizeof(iString), "UPDATE `Players` SET `Lvl`=%d, `Exp`=%d WHERE `Name`='%q'", level, exp, Player[playerid][Name]);
	if (!DatabaseExecuteWrite(iString, "save player level and experience"))
		return 0;

	return 1;
}
