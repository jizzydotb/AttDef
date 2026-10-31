#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_duel
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_duel
	#endinput
#endif
#define _attdef_impl_duel

#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_va>

#define DUEL_ROUND_DELAY 2500
#define DUEL_MAP_MIN_X -3000.0
#define DUEL_MAP_MIN_Y -3000.0
#define DUEL_MAP_MAX_X 3000.0
#define DUEL_MAP_MAX_Y 3000.0

new DuelBlackoutZone[MAX_PLAYERS][4];
new DuelSessionCounter;

stock SendDuelMessageToNonPlaying(const fmat[], va_args<>)
{
	new message[144];
	va_format(message, sizeof(message), fmat, va_start<1>);
	foreach (new i : Player)
	{
		if (!Player[i][Playing])
		{
			SendClientMessage(i, -1, message);
		}
	}
	return 1;
}

stock Float:GetDuelFacingAngle(Float:x1, Float:y1, Float:x2, Float:y2)
{
	new Float: angle = -(90.0 - atan2(y1 - y2, x1 - x2));
	if (angle < 0.0)
		angle += 360.0;
	else if (angle >= 360.0)
		angle -= 360.0;
	return angle;
}

hook OnPlayerDisconnect(playerid, reason)
{
	foreach (new i : Player)
	{
		if (i != playerid && !Player[i][InDuel] && Player[i][challengerid] == playerid)
		{
			Player[i][challengerid] = -1;
			Player[i][DuelRequestedByGeneration] = 0;
			SendErrorMessage(i, "The player who invited you has disconnected.");
		}
	}

	if (!Player[playerid][InDuel])
		return 1;

	new winner = Player[playerid][challengerid];
	if (winner < 0 || winner >= MAX_PLAYERS || !IsPlayerConnected(winner) || !Player[winner][InDuel])
	{
		Player[playerid][InDuel] = false;
		Player[playerid][challengerid] = -1;
		return 1;
	}

	SendDuelMessageToNonPlaying(
		"{BD93DC}\xBB Duel: {FFFFFF}%s {93DCAC}left the duel. %s %d-%d %s", Player[playerid][Name], Player[winner][Name],
		Player[winner][DuelScore], Player[playerid][DuelScore], Player[playerid][Name]);
	FinishDuel(winner, playerid, true, playerid);
	return 1;
}

ResetDuelZones(dueller1, dueller2)
{
	new zone = Player[dueller1][Duel_GangZone];
	if (zone != INVALID_GANG_ZONE)
		GangZoneDestroy(zone);
	zone = Player[dueller2][Duel_GangZone];
	if (zone != INVALID_GANG_ZONE && zone != Player[dueller1][Duel_GangZone])
		GangZoneDestroy(zone);

	for (new i = 0; i < 4; i++)
	{
		zone = DuelBlackoutZone[dueller1][i];
		if (zone != INVALID_GANG_ZONE)
			GangZoneDestroy(zone);
		zone = DuelBlackoutZone[dueller2][i];
		if (zone != INVALID_GANG_ZONE && zone != DuelBlackoutZone[dueller1][i])
			GangZoneDestroy(zone);
		DuelBlackoutZone[dueller1][i] = INVALID_GANG_ZONE;
		DuelBlackoutZone[dueller2][i] = INVALID_GANG_ZONE;
	}
	Player[dueller1][Duel_GangZone] = INVALID_GANG_ZONE;
	Player[dueller2][Duel_GangZone] = INVALID_GANG_ZONE;
}

ResetDuellers(dueller1, dueller2, bool: respawn = true, skipplayerid = INVALID_PLAYER_ID)
{
	ResetDuelZones(dueller1, dueller2);

	for (new index = 0; index < 2; index++)
	{
		new playerid = (index == 0) ? dueller1 : dueller2;
		Player[playerid][InDuel] = false;
		Player[playerid][Team] = Player[playerid][TeamBeforeDuel];
		Player[playerid][challengerid] = -1;
		Player[playerid][DuelRequestedByGeneration] = 0;
		Player[playerid][DuelSessionID] = 0;
		Player[playerid][DuelArenaID] = -1;
		Player[playerid][DuelRequestedArena] = -1;
		Player[playerid][duelweap1] = 0;
		Player[playerid][duelweap2] = 0;
		Player[playerid][DuelRound] = 0;
		Player[playerid][DuelTarget] = 0;
		Player[playerid][DuelScore] = 0;
		Player[playerid][DuelRoundPending] = false;
		Player[playerid][DuelMode] = DUEL_MODE_FIRST_TO;
		Player[playerid][DuelSide] = NON;

		if (playerid == skipplayerid || !IsPlayerConnected(playerid))
			continue;

		SwitchTeamFix(playerid, false, false);
		ResetPlayerWorldBounds(playerid);
		if (respawn)
			SpawnInLobby(playerid);
		if (Current != -1 && GameType == ARENA)
		{
			for (new i = 0; i < 4; i++)
			{
				if (BlackoutZone[i] != INVALID_GANG_ZONE)
					GangZoneShowForPlayer(playerid, BlackoutZone[i], 0x000000AA);
			}
		}
	}
}

UpdateDuellersStats(winner, loser)
{
	SendDuelMessageToNonPlaying(
		"{BD93DC}\xBB Duel: {FFFFFF}Stats: %s [W: %d / L: %d] & %s [W: %d / L: %d]",
		Player[winner][NameWithoutTag], Player[winner][DuelsWon], Player[winner][DuelsLost],
		Player[loser][NameWithoutTag], Player[loser][DuelsWon], Player[loser][DuelsLost]);
	if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin duel-stat transaction"))
		return 0;

	if (!DatabaseExecuteWrite(
		va_return("UPDATE Players SET DLost = %d WHERE Name = '%q'", Player[loser][DuelsLost], Player[loser][Name]),
		"save duel loss"))
	{
		DatabaseExecuteWrite("ROLLBACK", "rollback duel-stat transaction");
		return 0;
	}
	if (!DatabaseExecuteWrite(
		va_return("UPDATE Players SET DWon = %d WHERE Name = '%q'", Player[winner][DuelsWon], Player[winner][Name]),
		"save duel win"))
	{
		DatabaseExecuteWrite("ROLLBACK", "rollback duel-stat transaction");
		return 0;
	}
	if (!DatabaseExecuteWrite("COMMIT", "commit duel-stat transaction"))
	{
		DatabaseExecuteWrite("ROLLBACK", "rollback duel-stat transaction");
		return 0;
	}
	return 1;
}

ProcessDuelRequest(duel_host, duel_guest, weapon1, weapon2, arena, target, mode)
{
	Player[duel_guest][challengerid] = duel_host;
	Player[duel_guest][DuelRequestedByGeneration] = PlayerConnectionGeneration[duel_host];
	Player[duel_guest][duelweap1] = weapon1;
	Player[duel_guest][duelweap2] = weapon2;
	Player[duel_guest][DuelRequestedArena] = arena;
	Player[duel_guest][DuelTarget] = target;
	Player[duel_guest][DuelMode] = mode;

	SendClientMessage(duel_guest, -1, "{BD93DC}\xBB Duel: {FFFFFF}Type {AADC93}/yes {FFFFFF}to accept or {EB8989}/no {FFFFFF}to decline the duel invitation.");
	PlayerPlaySound(duel_guest, 1137, 0.0, 0.0, 0.0);

	new arenaText[64];
	if (arena == -1)
		format(arenaText, sizeof(arenaText), "random arena");
	else
		format(arenaText, sizeof(arenaText), "%s (ID %d)", AName[arena], arena);

	new modeText[32];
	if (mode == DUEL_MODE_BEST_OF_FIVE)
		format(modeText, sizeof(modeText), "BO5");
	else
		format(modeText, sizeof(modeText), "first to %d", target);

	return SendDuelMessageToNonPlaying(
		"{BD93DC}\xBB Duel: {FFFFFF}%s {93DCAC}challenged {FFFFFF}%s {93DCAC}to a duel ({FFFFFF}%s{93DCAC}, {FFFFFF}%s{93DCAC}) [Weapons: {FFFFFF}%s - %s{93DCAC}]",
		Player[duel_host][Name], Player[duel_guest][Name],
		modeText, arenaText, ReturnWeaponName(weapon1), ReturnWeaponName(weapon2));
}

GetDuelArenaBounds(arenaid, &Float: minx, &Float: miny, &Float: maxx, &Float: maxy)
{
	minx = AMin[arenaid][0];
	miny = AMin[arenaid][1];
	maxx = AMax[arenaid][0];
	maxy = AMax[arenaid][1];

	if (minx >= maxx || miny >= maxy || minx <= -9000.0 || miny <= -9000.0 || maxx >= 9000.0 || maxy >= 9000.0)
	{
		minx = AAttackerSpawn[arenaid][0];
		miny = AAttackerSpawn[arenaid][1];
		maxx = ADefenderSpawn[arenaid][0];
		maxy = ADefenderSpawn[arenaid][1];
		if (maxx < minx) { new Float: swap = minx; minx = maxx; maxx = swap; }
		if (maxy < miny) { new Float: swap = miny; miny = maxy; maxy = swap; }
		minx -= 75.0;
		miny -= 75.0;
		maxx += 75.0;
		maxy += 75.0;
	}

	if (minx < DUEL_MAP_MIN_X) minx = DUEL_MAP_MIN_X;
	if (miny < DUEL_MAP_MIN_Y) miny = DUEL_MAP_MIN_Y;
	if (maxx > DUEL_MAP_MAX_X) maxx = DUEL_MAP_MAX_X;
	if (maxy > DUEL_MAP_MAX_Y) maxy = DUEL_MAP_MAX_Y;
}

CreateDuelZones(dueller1, dueller2)
{
	new arenaid = Player[dueller1][DuelArenaID];
	new Float: minx, Float: miny, Float: maxx, Float: maxy;
	GetDuelArenaBounds(arenaid, minx, miny, maxx, maxy);

	new zone = GangZoneCreate(minx, miny, maxx, maxy);
	Player[dueller1][Duel_GangZone] = zone;
	Player[dueller2][Duel_GangZone] = zone;
	if (zone != INVALID_GANG_ZONE)
	{
		GangZoneShowForPlayer(dueller1, zone, 0xFFA50066);
		GangZoneShowForPlayer(dueller2, zone, 0xFFA50066);
	}

	if (miny > DUEL_MAP_MIN_Y)
		DuelBlackoutZone[dueller1][0] = GangZoneCreate(DUEL_MAP_MIN_X, DUEL_MAP_MIN_Y, DUEL_MAP_MAX_X, miny);
	if (maxy < DUEL_MAP_MAX_Y)
		DuelBlackoutZone[dueller1][1] = GangZoneCreate(DUEL_MAP_MIN_X, maxy, DUEL_MAP_MAX_X, DUEL_MAP_MAX_Y);
	if (minx > DUEL_MAP_MIN_X)
		DuelBlackoutZone[dueller1][2] = GangZoneCreate(DUEL_MAP_MIN_X, miny, minx, maxy);
	if (maxx < DUEL_MAP_MAX_X)
		DuelBlackoutZone[dueller1][3] = GangZoneCreate(maxx, miny, DUEL_MAP_MAX_X, maxy);

	new bool: blackoutIncomplete;
	for (new i = 0; i < 4; i++)
	{
		zone = DuelBlackoutZone[dueller1][i];
		DuelBlackoutZone[dueller2][i] = zone;
		if (zone != INVALID_GANG_ZONE)
		{
			GangZoneShowForPlayer(dueller1, zone, 0x000000AA);
			GangZoneShowForPlayer(dueller2, zone, 0x000000AA);
		}
		else if ((i == 0 && miny > DUEL_MAP_MIN_Y) || (i == 1 && maxy < DUEL_MAP_MAX_Y)
			|| (i == 2 && minx > DUEL_MAP_MIN_X) || (i == 3 && maxx < DUEL_MAP_MAX_X))
			blackoutIncomplete = true;
	}

	if (blackoutIncomplete)
	{
		SendErrorMessage(dueller1, "The duel blackout could not be fully created.");
		SendErrorMessage(dueller2, "The duel blackout could not be fully created.");
	}

	SetPlayerWorldBounds(dueller1, maxx, minx, maxy, miny);
	SetPlayerWorldBounds(dueller2, maxx, minx, maxy, miny);
}

GiveDuelWeapons(playerid, weapon1, weapon2)
{
	ResetPlayerWeapons(playerid);
	GivePlayerWeapon(playerid, weapon1, 9999);
	if (weapon2 != weapon1)
		GivePlayerWeapon(playerid, weapon2, 9999);
}

stock SetupDuellerSpawn(playerid, bool: forcePos = true, bool: freeze = false)
{
	if (!IsPlayerConnected(playerid) || !Player[playerid][InDuel])
		return 0;

	new arenaid = Player[playerid][DuelArenaID];
	if (!Iter_Contains(CurrentArenas, arenaid))
		return 0;

	new side = Player[playerid][DuelSide];
	new Float: x, Float: y, Float: z;
	new Float: otherX, Float: otherY;
	if (side == ATTACKER)
	{
		x = AAttackerSpawn[arenaid][0];
		y = AAttackerSpawn[arenaid][1];
		z = AAttackerSpawn[arenaid][2] + 0.5;
		otherX = ADefenderSpawn[arenaid][0];
		otherY = ADefenderSpawn[arenaid][1];
	}
	else
	{
		x = ADefenderSpawn[arenaid][0];
		y = ADefenderSpawn[arenaid][1];
		z = ADefenderSpawn[arenaid][2] + 0.5;
		otherX = AAttackerSpawn[arenaid][0];
		otherY = AAttackerSpawn[arenaid][1];
	}

	new Float: facingAngle = GetDuelFacingAngle(x, y, otherX, otherY);
	new duelSkin = Player[playerid][pSkin];
	if (duelSkin < 0 || duelSkin >= MAX_SKINS || duelSkin == 74)
		duelSkin = Skin[side];

	new virtualworld = 10000 + (Player[playerid][DuelSessionID] % 50000);
	SetPlayerVirtualWorld(playerid, virtualworld);
	SetPlayerInterior(playerid, AInterior[arenaid]);
	SetPlayerTeam(playerid, NO_TEAM);
	SetSpawnInfo(playerid, NO_TEAM, duelSkin, x, y, z, facingAngle, 0, 0, 0, 0, 0, 0);

	if (forcePos)
	{
		SetPlayerPos(playerid, x, y, z);
		SetPlayerFacingAngle(playerid, facingAngle);
		SetCameraBehindPlayer(playerid);
	}
	SetPlayerSkin(playerid, duelSkin);

	new Float: minx, Float: miny, Float: maxx, Float: maxy;
	GetDuelArenaBounds(arenaid, minx, miny, maxx, maxy);
	SetPlayerWorldBounds(playerid, maxx, minx, maxy, miny);

	if (freeze)
		TogglePlayerControllable(playerid, false);

	return 1;
}

StartDuelRound(dueller1, dueller2)
{
	if (!IsPlayerConnected(dueller1) || !IsPlayerConnected(dueller2)
		|| !Player[dueller1][InDuel] || !Player[dueller2][InDuel]
		|| Player[dueller1][DuelSessionID] != Player[dueller2][DuelSessionID])
		return 0;

	new arenaid = Player[dueller1][DuelArenaID];
	if (!Iter_Contains(CurrentArenas, arenaid))
	{
		SendErrorMessage(dueller1, "The selected duel arena is no longer available.");
		SendErrorMessage(dueller2, "The selected duel arena is no longer available.");
		ResetDuellers(dueller1, dueller2);
		return 0;
	}

	Player[dueller1][DuelRound]++;
	Player[dueller2][DuelRound] = Player[dueller1][DuelRound];
	Player[dueller1][DuelRoundPending] = false;
	Player[dueller2][DuelRoundPending] = false;

	for (new index = 0; index < 2; index++)
	{
		new playerid = (index == 0) ? dueller1 : dueller2;
		new side = Player[playerid][DuelSide];

		Player[playerid][IgnoreSpawn] = true;
		SpawnPlayer(playerid);
		SetupDuellerSpawn(playerid, true, false);

		SetHPAP(playerid, 100.0, 100.0);
		GiveDuelWeapons(playerid, Player[playerid][duelweap1], Player[playerid][duelweap2]);
		SetPlayerColor(playerid, side == ATTACKER ? ATTACKER_PLAYING : DEFENDER_PLAYING);
		if (Player[playerid][Duel_GangZone] != INVALID_GANG_ZONE)
			GangZoneShowForPlayer(playerid, Player[playerid][Duel_GangZone], 0xFFA50066);
		for (new zoneIndex = 0; zoneIndex < 4; zoneIndex++)
		{
			new zone = DuelBlackoutZone[playerid][zoneIndex];
			if (zone != INVALID_GANG_ZONE)
				GangZoneShowForPlayer(playerid, zone, 0x000000AA);
		}
		TogglePlayerControllable(playerid, true);
		RadarFix(playerid);
		PlayerPlaySound(playerid, 3200, 0.0, 0.0, 0.0);
	}

	new score[96];
	format(
		score, sizeof(score), "{BD93DC}\xBB Duel: {FFFFFF}Round %d | %s %d - %d %s", Player[dueller1][DuelRound], Player[dueller1][Name],
		Player[dueller1][DuelScore], Player[dueller2][DuelScore], Player[dueller2][Name]);
	SendClientMessage(dueller1, -1, score);
	SendClientMessage(dueller2, -1, score);
	return 1;
}

forward StartDuelRoundTimer(dueller1, dueller2, sessionid, dueller1generation, dueller2generation);
public StartDuelRoundTimer(dueller1, dueller2, sessionid, dueller1generation, dueller2generation)
{
	if (dueller1 < 0 || dueller1 >= MAX_PLAYERS || dueller2 < 0 || dueller2 >= MAX_PLAYERS)
		return 0;
	if (!IsPlayerConnected(dueller1) || !IsPlayerConnected(dueller2)
		|| PlayerConnectionGeneration[dueller1] != dueller1generation
		|| PlayerConnectionGeneration[dueller2] != dueller2generation
		|| !Player[dueller1][InDuel] || !Player[dueller2][InDuel]
		|| Player[dueller1][DuelSessionID] != sessionid || Player[dueller2][DuelSessionID] != sessionid
		|| Player[dueller1][challengerid] != dueller2 || Player[dueller2][challengerid] != dueller1)
		return 0;
	return StartDuelRound(dueller1, dueller2);
}

StartDuel(dueller1, dueller2, weapon1, weapon2, arenaid, target, mode)
{
	if (Player[dueller1][Spectating])
		StopSpectate(dueller1);
	if (Player[dueller2][Spectating])
		StopSpectate(dueller2);

	DuelSessionCounter++;
	if (DuelSessionCounter <= 0)
		DuelSessionCounter = 1;

	Player[dueller1][TeamBeforeDuel] = Player[dueller1][Team];
	Player[dueller2][TeamBeforeDuel] = Player[dueller2][Team];
	Player[dueller1][Team] = REFEREE;
	Player[dueller2][Team] = REFEREE;
	Player[dueller1][ToAddInRound] = false;
	Player[dueller2][ToAddInRound] = false;
	Player[dueller1][InDuel] = true;
	Player[dueller2][InDuel] = true;
	Player[dueller1][challengerid] = dueller2;
	Player[dueller2][challengerid] = dueller1;
	Player[dueller1][DuelRequestedByGeneration] = 0;
	Player[dueller2][DuelRequestedByGeneration] = 0;
	Player[dueller1][duelweap1] = weapon1;
	Player[dueller1][duelweap2] = weapon2;
	Player[dueller2][duelweap1] = weapon1;
	Player[dueller2][duelweap2] = weapon2;
	Player[dueller1][DuelArenaID] = arenaid;
	Player[dueller2][DuelArenaID] = arenaid;
	Player[dueller1][DuelTarget] = target;
	Player[dueller2][DuelTarget] = target;
	Player[dueller1][DuelMode] = mode;
	Player[dueller2][DuelMode] = mode;
	Player[dueller1][DuelScore] = 0;
	Player[dueller2][DuelScore] = 0;
	Player[dueller1][DuelRound] = 0;
	Player[dueller2][DuelRound] = 0;
	Player[dueller1][DuelRoundPending] = false;
	Player[dueller2][DuelRoundPending] = false;
	Player[dueller1][DuelSide] = ATTACKER;
	Player[dueller2][DuelSide] = DEFENDER;
	Player[dueller1][DuelSessionID] = DuelSessionCounter;
	Player[dueller2][DuelSessionID] = DuelSessionCounter;
	Player[dueller1][Duel_GangZone] = INVALID_GANG_ZONE;
	Player[dueller2][Duel_GangZone] = INVALID_GANG_ZONE;

	for (new i = 0; i < 4; i++)
	{
		DuelBlackoutZone[dueller1][i] = INVALID_GANG_ZONE;
		DuelBlackoutZone[dueller2][i] = INVALID_GANG_ZONE;
	}

	SetPlayerColor(dueller1, ATTACKER_PLAYING);
	SetPlayerColor(dueller2, DEFENDER_PLAYING);
	DestroyPlayerBlackoutZone(dueller1);
	DestroyPlayerBlackoutZone(dueller2);
	CreateDuelZones(dueller1, dueller2);

	new modeText[32];
	if (mode == DUEL_MODE_BEST_OF_FIVE)
		format(modeText, sizeof(modeText), "Best of five (first to 3)");
	else
		format(modeText, sizeof(modeText), "First to %d", target);
	SendDuelMessageToNonPlaying(
		"{BD93DC}\xBB Duel: {FFFFFF}%s {93DCAC}vs {FFFFFF}%s {93DCAC}| Arena: {FFFFFF}%s {93DCAC}(ID %d) | {FFFFFF}%s",
		Player[dueller1][Name], Player[dueller2][Name], AName[arenaid], arenaid, modeText);
	return StartDuelRound(dueller1, dueller2);
}

FinishDuel(winner, loser, bool: respawn = true, skipplayerid = INVALID_PLAYER_ID)
{
	if (winner < 0 || winner >= MAX_PLAYERS || loser < 0 || loser >= MAX_PLAYERS)
		return 0;

	Player[winner][DuelsWon]++;
	Player[loser][DuelsLost]++;
	SendDuelMessageToNonPlaying(
		"{BD93DC}\xBB Duel: {FFFFFF}%s {93DCAC}won against {FFFFFF}%s [Score: {FFFFFF}%d-%d{93DCAC}]",
		Player[winner][Name], Player[loser][Name], Player[winner][DuelScore], Player[loser][DuelScore]);
	ResetDuellers(winner, loser, respawn, skipplayerid);
	UpdateDuellersStats(winner, loser);

	CallLocalFunction("OnDuelEndEvent", "dddd", winner, loser, Player[winner][DuelScore], Player[loser][DuelScore]);

	return 1;
}

PlayerDuelQuit(playerid)
{
	new winner = Player[playerid][challengerid];
	if (winner < 0 || winner >= MAX_PLAYERS || !IsPlayerConnected(winner) || !Player[winner][InDuel])
	{
		Player[playerid][InDuel] = false;
		Player[playerid][challengerid] = -1;
		return 0;
	}

	SendDuelMessageToNonPlaying(
		"{BD93DC}\xBB Duel: {FFFFFF}%s {93DCAC}forfeited the duel. %s %d-%d %s",
		Player[playerid][Name], Player[winner][Name], Player[winner][DuelScore], Player[playerid][DuelScore], Player[playerid][Name]);
	return FinishDuel(winner, playerid);
}

ProcessDuellerDeath(playerid, killerid, reason)
{
	if (!Player[playerid][InDuel] || Player[playerid][DuelRoundPending])
		return 0;

	new opponent = Player[playerid][challengerid];
	if (opponent < 0 || opponent >= MAX_PLAYERS || !IsPlayerConnected(opponent) || !Player[opponent][InDuel]
		|| Player[playerid][DuelSessionID] == 0
		|| Player[opponent][DuelSessionID] != Player[playerid][DuelSessionID]
		|| Player[opponent][challengerid] != playerid)
		return 0;

	Player[playerid][DuelRoundPending] = true;
	Player[opponent][DuelRoundPending] = true;
	new winner = (killerid == opponent) ? killerid : opponent;
	Player[winner][DuelScore]++;

	new score[142];
	format(
		score, sizeof(score), "{BD93DC}\xBB Duel: {FFFFFF}%s {93DCAC}won round %d [Score: {FFFFFF}%d-%d{93DCAC} - Weapon: {FFFFFF}%s{93DCAC}]",
		Player[winner][Name], Player[playerid][DuelRound],
		Player[winner][DuelScore], Player[playerid][DuelScore], ReturnWeaponName(reason));
	SendClientMessage(winner, -1, score);
	SendClientMessage(playerid, -1, score);

	if (Player[winner][DuelScore] >= Player[winner][DuelTarget])
		return FinishDuel(winner, playerid);

	TogglePlayerControllable(opponent, false);
	Player[opponent][DuelSide] = (Player[opponent][DuelSide] == ATTACKER) ? DEFENDER : ATTACKER;
	Player[playerid][DuelSide] = (Player[playerid][DuelSide] == ATTACKER) ? DEFENDER : ATTACKER;

	SetupDuellerSpawn(playerid, false, true);

	return SetTimerEx(
		"StartDuelRoundTimer", DUEL_ROUND_DELAY, false, "iiiii", opponent, playerid,
		Player[opponent][DuelSessionID], PlayerConnectionGeneration[opponent], PlayerConnectionGeneration[playerid]);
}
