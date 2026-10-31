#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_common
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_common
	#endinput
#endif
#define _attdef_impl_common

/*
	Common stuff
*/

#include "modules/common/anticheat.inc"
#include "modules/common/vehicles.inc"
#include "modules/common/corpses.inc"
#include "modules/common/teams.inc"
#include "modules/common/sets.inc"

public ClearSpectatorState(playerid)
{
	DisablePlayerCheckpoint(playerid);
	SetCameraBehindPlayer(playerid);
	Player[playerid][SpectatingRound] = -1;
	Player[playerid][SpectatingType] = -1;
	Player[playerid][Spectating] = false;
	PlayerTextDrawSetString(playerid, TD_RoundSpec[playerid], "_");
	Player[playerid][SpectatingMessageSent] = false; // Reset the sent message flag
}

new ArenaSpecGangzone[MAX_PLAYERS] = {-1};

forward DeleteArenaSpecGangzone(playerid);
public DeleteArenaSpecGangzone(playerid)
{
	if (ArenaSpecGangzone[playerid] != -1)
	{
		GangZoneDestroy(ArenaSpecGangzone[playerid]);
		ArenaSpecGangzone[playerid] = -1;
	}
	return 1;
}

// Update play time every minute
forward UpdatePlayTime();
public UpdatePlayTime()
{
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i))
		{
			Player[i][PlayTime]++;
		}
	}
	return 1;
}

public GetTeamHealthBarColor(team)
{
	switch (team)
	{
		case ATTACKER:
			return 0x3A5F2BFF;
		case ATTACKER_SUB:
			return 0xA4F2B4FF;
		case DEFENDER:
			return 0x5462B9FF;
		case DEFENDER_SUB:
			return 0x8CE0FCFF;
		case REFEREE:
			return 0xA88400FF;
		default:
			return 0xB4191DFF;
	}
	return 0xB4191DFF;
}

stock UpdatePlayerHealthMoney(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	new totalHealth = 0;
	new pState = GetPlayerState(playerid);
	if (pState == PLAYER_STATE_ONFOOT || pState == PLAYER_STATE_DRIVER || pState == PLAYER_STATE_PASSENGER || pState == PLAYER_STATE_SPAWNED)
	{
		totalHealth = floatround(GetHP(playerid) + GetAP(playerid));
		if (totalHealth < 0)
			totalHealth = 0;
	}

	if (GetPlayerMoney(playerid) != totalHealth)
	{
		ResetPlayerMoney(playerid);
		if (totalHealth > 0)
			GivePlayerMoney(playerid, totalHealth);
	}
	return 1;
}

forward UpdatePlayerHealthBar(playerid);
public UpdatePlayerHealthBar(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	UpdatePlayerHealthMoney(playerid);

	new bool: enabled = PlayerInterface[playerid][INTERFACE_HP] && Player[playerid][Team] != NON
		&& (!Player[playerid][Spectating] || Player[playerid][SpectatingRound] != -1)
		;

	new color = GetTeamHealthBarColor(Player[playerid][Team]);
	SetPlayerHealthBarColor(playerid, color);

	return EnableHealthBarForPlayer(playerid, enabled);
}

CreateMainSpawnText3D()
{
	if (!IsValid3DTextLabel(MainSpawnText3D))
	{
		new lobby_text[256];
		format(lobby_text, sizeof(lobby_text), ""COL_PRIM"Welcome to {FFFFFF}%s Server!\n{FFFFFF}You are in the lobby!\n"COL_PRIM"Att-Def Version: {FFFFFF}%s\n"COL_PRIM"Bases: {FFFFFF}%d "COL_PRIM"- Arenas: {FFFFFF}%d\n{FFFFFF}/help /cmds /settings /changelog /matchtips /credits", ServerName, GM_VERSION, TotalBases, TotalArenas);
		new Float: x = MainSpawn[0], Float: y = MainSpawn[1], Float: z = MainSpawn[2];
		new interior = MainInterior;
		new Float: distance = 150.0;
		MainSpawnText3D = Create3DTextLabel(lobby_text, 0xFFFFFFFF, x, y, z + 0.2, distance, interior, true);
	}
	return 1;
}

DestroyMainSpawnText3D()
{
	if (IsValid3DTextLabel(MainSpawnText3D))
	{
		Delete3DTextLabel(MainSpawnText3D);
		MainSpawnText3D = INVALID_3DTEXT_ID;
	}
	return 1;
}

forward AttachHealthTextLabelToVehicle(vehicleid);
public CheckWinCondition()
{
	if(AutoEnd)
	{
		if (TeamScore[ATTACKER] >= WinningScore || TeamScore[DEFENDER] >= WinningScore)
		{
			ScheduleWarEnd(5000);
			SendClientMessageToAll(-1, COL_PRIM "\xBB Generating End Match Results..");
			SendClientMessageToAll(-1, COL_PRIM "\xBB If you missed the results, type "COL_SEC"/showagain");
		}
	}
}

// Function to destroy the blood object for a specific player
forward DestroyBloodObjectForPlayer(bloodObject, playerid);
public SendTipMessageToAll(bool: calledDuringPause)
{
	if (calledDuringPause)
		TipTimerDuringPause = 0;
	switch (random(16))
	{
		case 0: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Use {FFFFFF}/gunmenu " COL_TIP "to select your weapons.");
		case 1: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Want to differentiate your team-mates on radar by color? Use {FFFFFF}/playermarkers");
		case 2: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Use {FFFFFF}/weaponbinds " COL_TIP "to code your own weapon key binds.");
		case 3: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "You can drop a gun by holding it and typing {FFFFFF}/remgun");
		case 4: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "If you're lagging, you can request netcheck with {FFFFFF}/netcheckme");
		case 5: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Disrespect noobs, use {FFFFFF}/deathdiss " COL_TIP "command.");
		case 6: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Getting distracted by some textdraws? Try {FFFFFF}/hud");
		case 7: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Spectate other players with {FFFFFF}/spec " COL_TIP "command.");
		case 8: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Play in different duel arenas by means of 'custom' parameter in {FFFFFF}/duel " COL_TIP "command.");
		case 9: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Use {FFFFFF}/cmdhelp [Command name] " COL_TIP "to get info about any command.");
		case 10: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Change your hit sounds with {FFFFFF}/sound " COL_TIP "command.");
		case 11: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Change your time and weather with {FFFFFF}/time " COL_TIP "and {FFFFFF}/weather " COL_TIP "commands.");
		case 12: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Press 'Tab' key and click on a player for quicker operations/commands.");
		case 13: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "You can change your skin with /skin while in lobby, DM or duel.");
		case 14: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Switch round textdraws style with {FFFFFF}/style");
		case 15: SendClientMessageToAll(COL_TIP_PRIM_HEX, "Tips: " COL_TIP "Use {FFFFFF}/matchtips " COL_TIP "to display some guidelines you may need while playing!");
	}
	return 1;
}

SendCommandHelpMessage(playerid, const text[], MessageBoxType: messagebox_type = MSGBOX_TYPE_MIDDLE)
{
	#pragma unused messagebox_type
	va_SendClientMessage(playerid, -1, "%s\xBB HELP: {FFFFFF}%s", COL_INFO, text);
	return 1;
}

SendErrorMessage(playerid, const text[], MessageBoxType: messagebox_type = MSGBOX_TYPE_MIDDLE)
{
	#pragma unused messagebox_type
	PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
	return va_SendClientMessage(playerid, -1, "%s\xBB ERROR: {FFFFFF}%s", COL_ERROR, text);
}

SendWarningMessage(playerid, const text[], MessageBoxType: messagebox_type = MSGBOX_TYPE_MIDDLE)
{
	#pragma unused messagebox_type
	PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
	return va_SendClientMessage(playerid, -1, "%s\xBB WARNING: {FFFFFF}%s", COL_WARNING, text);
}

SendUsageMessage(playerid, const text[], MessageBoxType: messagebox_type = MSGBOX_TYPE_MIDDLE)
{
	#pragma unused messagebox_type
	PlayerPlaySound(playerid, 5202, 0.0, 0.0, 0.0);
	return va_SendClientMessage(playerid, -1, "%s\xBB USAGE: {FFFFFF}%s", COL_USAGE, text);
}

GetColor(color)
{
	new string[9];
	format(string, sizeof(string), "{%06x}", ShiftRGBAToRGB(color));
	return string;
}

GetPlayerCorrectMarkerCol(playerid, forplayerid)
{
	if (!Player[forplayerid][PlayerMarkers])
		return GetPlayerColor(playerid);
	if (TeamHasLeader[Player[playerid][Team]] && TeamLeader[Player[playerid][Team]] == playerid)
		return Player[playerid][Team] == ATTACKER ? ATTACKER_TEAM_LEADER : DEFENDER_TEAM_LEADER;
	if (Player[playerid][AskingForHelp])
		return Player[playerid][Team] == ATTACKER ? ATTACKER_ASKING_HELP : DEFENDER_ASKING_HELP;
	return PlayerColors[playerid];
}

// Checks all players and set idle players to afk mode
AutoAFKCheck()
{
	if (WarMode)
		return;

	foreach (new i : Player)
	{
		if (Player[i][ToAddInRound])
			continue;

		if (Player[i][PauseCount] > 60 && !Player[i][IsAFK]) // If they've been away for more than 60 seconds
		{
			new bool: wasPlaying = Player[i][Playing];
			if (wasPlaying)
				RemovePlayerFromRound(i, true, false);
			else if (Player[i][Spectating])
				StopSpectate(i);
			else if (Player[i][InDM])
				QuitDM(i);

			Player[i][Team] = wasPlaying ? REFEREE : NON;
			SetPlayerColor(i, wasPlaying ? REFEREE_COLOR : 0xAAAAAAAA);
			Player[i][IsAFK] = true;
			SpawnInLobby(i);
			TogglePlayerControllable(i, 0);
			printf(
				"[AttDef][AutoAFK] Moved player to lobby: player=%d wasPlaying=%d pauseCount=%d team=%d.",
				i, wasPlaying, Player[i][PauseCount], Player[i][Team]);

			if (wasPlaying)
			{
				va_SendClientMessageToAll(
					-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has been set to AFK mode automatically due to one minute(+) of inactivity!", Player[i][Name]);
			}
		}
	}
}

bool: IsValidWeapon(weaponid) { return weaponid > 1 && weaponid < 19 || weaponid > 21 && weaponid < 47 && weaponid != 40; }

stock bool: IsValidSkin(skinid) { return skinid > 74 && skinid < 265 || skinid >= 0 && skinid < 74 || skinid > 272 && skinid < 300; }

stock GetPlayerSpeed(playerid)
{
	new Float: velocity[4];
	GetPlayerVelocity(playerid, velocity[0], velocity[1], velocity[2]);
	velocity[3] = floatsqroot(floatpower(floatabs(velocity[0]), 2.0) + floatpower(floatabs(velocity[1]), 2.0) + floatpower(floatabs(velocity[2]), 2.0)) * 179.28625;
	return floatround(velocity[3]);
}

ShowPlayerClassSelection(playerid)
{
	HideRoundStats(playerid);
	HideEndRoundTextDraw(playerid);
	HideMatchScoreBoard(playerid);
	TextDrawHideForPlayer(playerid, InitTeamAttackerTD);
	TextDrawHideForPlayer(playerid, InitTeamDefenderTD);
	PlayerRejoinSpawnGeneration[playerid]++;
	new str[320], ct[5];

	foreach (new i : Player)
	{
		if (i == playerid)
			continue;

		switch (Player[i][Team])
		{
			case ATTACKER: ct[0]++;
			case DEFENDER: ct[1]++;
			case REFEREE: ct[2]++;
			case ATTACKER_SUB: ct[3]++;
			case DEFENDER_SUB: ct[4]++;
		}
	}
	format(
		str, sizeof(str),
		"Team\tAccessibility\tMembers Count\n{FFFFFF}Auto-assign\tN/A\tN/A\n%s%s{FFFFFF}\t%s\t%d\n%s%s{FFFFFF}\t%s\t%d\n%s%s{FFFFFF}\t%s\t%d\n%s%s{FFFFFF}\t%s\t%d\n%s%s{FFFFFF}\t%s\t%d",
		TextColor[ATTACKER], TeamName[ATTACKER], strlen(GroupAccessPassword[ATTACKER - 1]) > 0 ? "{FF6666}Locked (password required)" : "{66FF66}Open", ct[0], TextColor[DEFENDER],
		TeamName[DEFENDER], strlen(GroupAccessPassword[DEFENDER - 1]) > 0 ? "{FF6666}Locked (password required)" : "{66FF66}Open", ct[1], TextColor[REFEREE], TeamName[REFEREE],
		strlen(GroupAccessPassword[REFEREE - 1]) > 0 ? "{FF6666}Locked (password required)" : "{66FF66}Open", ct[2], TextColor[ATTACKER_SUB], TeamName[ATTACKER_SUB],
		strlen(GroupAccessPassword[ATTACKER_SUB - 1]) > 0 ? "{FF6666}Locked (password required)" : "{66FF66}Open", ct[3], TextColor[DEFENDER_SUB], TeamName[DEFENDER_SUB],
		strlen(GroupAccessPassword[DEFENDER_SUB - 1]) > 0 ? "{FF6666}Locked (password required)" : "{66FF66}Open", ct[4]);

	ShowPlayerDialog(playerid, DIALOG_TEAM_SELECTION, DIALOG_STYLE_TABLIST_HEADERS, "Team Selection", str, "Spawn", "");
}

forward SpawnConnectedPlayer(playerid, team);
public SpawnConnectedPlayer(playerid, team)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	switch (team)
	{
		case 0: // auto-assign
		{
			if (!WarMode)
				Player[playerid][Team] = GetTeamWithLessPlayers();
			else if (strfind(Player[playerid][Name], TeamName[ATTACKER], true) != -1)
				Player[playerid][Team] = ATTACKER;
			else if (strfind(Player[playerid][Name], TeamName[DEFENDER], true) != -1)
				Player[playerid][Team] = DEFENDER;
			else
				Player[playerid][Team] = GetTeamWithLessPlayers();
		}
		case 1: Player[playerid][Team] = ATTACKER;
		case 2: Player[playerid][Team] = DEFENDER;
		case 3:
		{
			Player[playerid][Team] = REFEREE;
			HideRoundStats(playerid);
			HideEndRoundTextDraw(playerid);
			DestroyPlayerBlackoutZone(playerid);
			DisablePlayerCheckpoint(playerid);
		}
		case 4: Player[playerid][Team] = ATTACKER_SUB;
		case 5: Player[playerid][Team] = DEFENDER_SUB;
	}

	SwitchTeamFix(playerid, false, false, false);
	SetPlayerWeather(playerid, Player[playerid][Weather]);
	SetPlayerTime(playerid, Player[playerid][Time], 0);

	if (PlayerInterface[playerid][INTERFACE_NET])
	{
		PlayerTextDrawShow(playerid, FPSPingPacketBox[playerid]);
		PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);
	}
	else
	{
		PlayerTextDrawHide(playerid, FPSPingPacketBox[playerid]);
		PlayerTextDrawHide(playerid, FPSPingPacket[playerid]);
	}

	if (Current != -1 && (Player[playerid][Team] == ATTACKER || Player[playerid][Team] == DEFENDER))
	{
		ShowRoundStats(playerid);
	}
	else
	{
		HideRoundStats(playerid);
	}

	if (PlayerInterface[playerid][INTERFACE_SPEC])
	{
		PlayerTextDrawShow(playerid, WhoSpecHeader[playerid]);
		PlayerTextDrawShow(playerid, WhoSpec[playerid]);
	}
	else
	{
		PlayerTextDrawHide(playerid, WhoSpecHeader[playerid]);
		PlayerTextDrawHide(playerid, WhoSpec[playerid]);
	}

	if (Current != -1 && (Player[playerid][Team] == ATTACKER || Player[playerid][Team] == DEFENDER) && Player[playerid][RoundTDStyle] == 1 && !Player[playerid][Spectating])
	{
		TextDrawShowForPlayer(playerid, TeamHpLose[0]);
		TextDrawShowForPlayer(playerid, TeamHpLose[1]);
		TextDrawShowForPlayer(playerid, AttHpLose);
		TextDrawShowForPlayer(playerid, DefHpLose);
	}
	else
	{
		TextDrawHideForPlayer(playerid, TeamHpLose[0]);
		TextDrawHideForPlayer(playerid, TeamHpLose[1]);
		TextDrawHideForPlayer(playerid, AttHpLose);
		TextDrawHideForPlayer(playerid, DefHpLose);
	}

	PlayerTextDrawShow(playerid, TD_RoundSpec[playerid]);
	UpdatePlayerHealthBar(playerid);

	if (WarMode)
	{
		TextDrawHideForPlayer(playerid, InitTeamDefenderTD);
		TextDrawHideForPlayer(playerid, InitTeamAttackerTD);
		ShowMatchScoreBoard(playerid);
	}
	else
	{
		HideMatchScoreBoard(playerid);
	}
	Player[playerid][Spawned] = true;
	SpawnInLobby(playerid);

	HideAllTeamBarsForPlayer(playerid);
	UpdateBaseIDTextDraw(playerid);
	LoadPlayerVariables(playerid);
	return 1;
}

ScheduleConnectedPlayerSpawn(playerid, team)
{
	new spawnGeneration = ++PlayerRejoinSpawnGeneration[playerid];
	if (!SetTimerEx(
		"SpawnConnectedPlayerDelayed", 250, false, "iiii", playerid, team,
		PlayerConnectionGeneration[playerid], spawnGeneration))
	{
		PlayerRejoinSpawnGeneration[playerid]++;
		SendErrorMessage(playerid, "Could not schedule your round rejoin. Please try /sync or reconnect.");
		return 0;
	}
	return 1;
}

forward SpawnConnectedPlayerDelayed(playerid, team, connectionGeneration, spawnGeneration);
public SpawnConnectedPlayerDelayed(playerid, team, connectionGeneration, spawnGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration
		|| PlayerRejoinSpawnGeneration[playerid] != spawnGeneration
		|| !Player[playerid][Logged] || Player[playerid][Spawned] || Current == -1
		|| ShouldPlayerBeReadded(playerid) != team)
		return 0;
	return SpawnConnectedPlayer(playerid, team);
}

SpawnInLobby(playerid)
{
	HideAllTeamBarsForPlayer(playerid);
	DisablePlayerCheckpoint(playerid);
	DestroyPlayerBlackoutZone(playerid);
	DestroyFlareObjects(playerid);
	DestroyArenaFlareObjects(playerid);
	SetSpawnInfo(playerid, NO_TEAM, Skin[Player[playerid][Team]], MainSpawn[0] + RandomMinMax(-3, 3), MainSpawn[1] + RandomMinMax(-3, 3), MainSpawn[2] + 2, MainSpawn[3], 0, 0, 0, 0, 0, 0);
	if (GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
		TogglePlayerSpectating(playerid, false);
	else
		SpawnPlayer(playerid);
}

ShowPlayerDeathMessage(killerid, playerid)
{
	if (!Player[killerid][HasDeathQuote])
		return;
	MessageBox(
		playerid, MSGBOX_TYPE_MIDDLE, va_return("%s~h~%s ~w~is dissing you", TDC[Player[killerid][Team]], Player[killerid][Name]), va_return("%s", DeathMessageStr[killerid]),
		3000);
}

RecountPlayersOnCP()
{
	PlayersInCP = 0;

	if (GameType == BASE || GameType == ARENA && CPInArena)
	{
		foreach (new i : PlayersInRound)
		{
			Player[i][WasInCP] = false;
			if (IsPlayerInCheckpoint(i))
				CallLocalFunction("OnPlayerEnterCheckpoint", "i", i);
		}
	}

	if (PlayersInCP != 0)
		return PlayersInCP;

	TeamCapturingCP = NON;
	CurrentCPTime	= ConfigCPTime + 1;
	HideRoundTeamHPCPHUDForAll();
	TextDrawHideForAll(EN_CheckPoint);
	TextDrawHideForAll(CheckpointWarningBox);
	TextDrawHideForAll(ModernRound_CPBoard);
	TextDrawHideForAll(ModernRound_CPText);
	TextDrawHideForAll(ModernRound_CPPercent);
	TextDrawHideForAll(SimpleRound_CPLine);
	TextDrawHideForAll(SimpleRound_CPText);
	TextDrawHideForAll(SimpleRound_CPPercent);
	TextDrawColor(timerCircleTD, 0x00000033);
	GangZoneStopFlashForAll(CPZone);
	foreach (new i : Player)
	{
		if (!Player[i][Spawned])
			continue;

		if (Player[i][RoundTDStyle] != 1)
			continue;

		TextDrawShowForPlayer(i, timerCircleTD);
	}
	return PlayersInCP;
}

RemoveVendingMachinesForPlayer(playerid)
{
	RemoveBuildingForPlayer(playerid, 955, 0.0, 0.0, 0.0, 20000.0);	 // CJ_EXT_SPRUNK
	RemoveBuildingForPlayer(playerid, 956, 0.0, 0.0, 0.0, 20000.0);	 // CJ_EXT_CANDY
	RemoveBuildingForPlayer(playerid, 1209, 0.0, 0.0, 0.0, 20000.0); // vendmach
	RemoveBuildingForPlayer(playerid, 1302, 0.0, 0.0, 0.0, 20000.0); // vendmachfd
	RemoveBuildingForPlayer(playerid, 1775, 0.0, 0.0, 0.0, 20000.0); // CJ_SPRUNK1
	RemoveBuildingForPlayer(playerid, 1776, 0.0, 0.0, 0.0, 20000.0); // CJ_CANDYVENDOR
	RemoveBuildingForPlayer(playerid, 1977, 0.0, 0.0, 0.0, 20000.0); // vendin3
}

RemoveUselessObjectsForPlayer(playerid)
{
	RemoveVendingMachinesForPlayer(playerid);
	RemoveBuildingForPlayer(playerid, 1220, 0.0, 0.0, 0.0, 6000.0); // boxes that make you lose hp
	RemoveBuildingForPlayer(playerid, 1221, 0.0, 0.0, 0.0, 6000.0); // boxes that make you lose hp
	RemoveBuildingForPlayer(playerid, 1230, 0.0, 0.0, 0.0, 6000.0); // boxes that make you lose hp
	RemoveBuildingForPlayer(playerid, 1299, 0.0, 0.0, 0.0, 6000.0); // boxes that make you lose hp
	RemoveBuildingForPlayer(playerid, 1421, 0.0, 0.0, 0.0, 6000.0); // boxes that make you lose hp
	RemoveBuildingForPlayer(playerid, 1448, 0.0, 0.0, 0.0, 6000.0); // boxes that make you lose hp
	RemoveBuildingForPlayer(playerid, 1449, 0.0, 0.0, 0.0, 6000.0); // boxes that make you lose hp
	RemoveBuildingForPlayer(playerid, 1450, 0.0, 0.0, 0.0, 6000.0); // boxes that make you lose hp
	RemoveBuildingForPlayer(playerid, 1440, 0.0, 0.0, 0.0, 6000.0);
	RemoveBuildingForPlayer(playerid, 1421, 0.0, 0.0, 0.0, 6000.0);
	RemoveBuildingForPlayer(playerid, 1438, 0.0, 0.0, 0.0, 6000.0);
	RemoveBuildingForPlayer(playerid, 1338, 0.0, 0.0, 0.0, 6000.0);
	RemoveBuildingForPlayer(playerid, 1219, 0.0, 0.0, 0.0, 6000.0);
	RemoveBuildingForPlayer(playerid, 1676, 0.0, 0.0, 0.0, 6000.0); // exploding gas tank
	RemoveBuildingForPlayer(playerid, 1686, 0.0, 0.0, 0.0, 6000.0);
}
/*
randomExFloat(Float:min, Float:max)
{
	new rand = random(floatround(max-min, floatround_round))+floatround(min, floatround_round);
	return rand;
}
*/
/*
Function: LogAdminCommand
cmd[]: the entered command
adminid: the admin who enters the command
playerid: the player who the command hits (use INVALID_PLAYER_ID to ignore this)
*/
LogAdminCommand(const cmd[], adminid, playerid)
{
	new File: log = fopen("admin_command_log.txt", io_append);

	if (log)
	{
		new Year, Month, Day, Hours, Minutes, Seconds;
		getdate(Year, Month, Day);
		gettime(Hours, Minutes, Seconds);
		if (playerid != INVALID_PLAYER_ID)
			fwrite(
				log, va_return(
						 "[%02d/%02d/%d][%02d:%02d:%02d] %s [%d] has used the command (/%s) at %s [%d]. \r\n", Day, Month, Year, Hours, Minutes, Seconds, Player[adminid][Name],
						 adminid, cmd, Player[playerid][Name], playerid));
		else
			fwrite(
				log,
				va_return("[%02d/%02d/%d][%02d:%02d:%02d] %s [%d] has used the command (/%s). \r\n", Day, Month, Year, Hours, Minutes, Seconds, Player[adminid][Name], adminid, cmd));
		fclose(log);
	}
	return 1;
}

ClearAdminCommandLog()
{
	new File: log = fopen("admin_command_log.txt", io_write);
	fwrite(log, "");
	fclose(log);
}

forward HideTargetInfo(playerid, generation);
public HideTargetInfo(playerid, generation)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != generation)
		return 0;

	TargetInfoData[playerid][TargetInfoTimerGeneration]++;
	TargetInfoData[playerid][TargetInfoTimer] = 0;
	TextDrawHideForPlayer(playerid, TargetInfo_CardBG);
	TextDrawHideForPlayer(playerid, TargetInfo_ArmourSlotBG);
	TextDrawHideForPlayer(playerid, TargetInfo_HealthSlotBG);
	PlayerTextDrawHide(playerid, TargetInfoData[playerid][TargetInfo_Name]);
	PlayerTextDrawHide(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar]);
	PlayerTextDrawHide(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent]);
	PlayerTextDrawHide(playerid, TargetInfoData[playerid][TargetInfo_HealthBar]);
	PlayerTextDrawHide(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent]);
	PlayerTextDrawHide(playerid, TargetInfoData[playerid][TargetInfo_Net]);
	return 1;
}

forward HideTargetInfoTimed(playerid, generation, timerGeneration);
public HideTargetInfoTimed(playerid, generation, timerGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != generation
		|| TargetInfoData[playerid][TargetInfoTimerGeneration] != timerGeneration)
		return 0;

	return HideTargetInfo(playerid, generation);
}

ShowTargetInfo(playerid, targetid, bool: force = false)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| targetid < 0 || targetid >= MAX_PLAYERS || !IsPlayerConnected(targetid))
		return 0;

	new currentTick = GetTickCount();
	if (!force && targetid == TargetInfoData[playerid][TargetInfoLastTarget] && currentTick - TargetInfoData[playerid][TargetInfoLastTick] < 500)
		return 1;

	if (TargetInfoData[playerid][TargetInfoTimer])
		KillTimer(TargetInfoData[playerid][TargetInfoTimer]);
	new timerGeneration = ++TargetInfoData[playerid][TargetInfoTimerGeneration];
	TargetInfoData[playerid][TargetInfoTimer] = SetTimerEx(
		"HideTargetInfoTimed", 5000, false, "iii", playerid, PlayerConnectionGeneration[playerid], timerGeneration);
	if (!TargetInfoData[playerid][TargetInfoTimer])
	{
		printf("[AttDef] Failed to schedule target-info cleanup for player %d.", playerid);
		HideTargetInfo(playerid, PlayerConnectionGeneration[playerid]);
		return 0;
	}

	// 1. Show Global textdraws
	TextDrawShowForPlayer(playerid, TargetInfo_CardBG);
	TextDrawShowForPlayer(playerid, TargetInfo_ArmourSlotBG);
	TextDrawShowForPlayer(playerid, TargetInfo_HealthSlotBG);

	// Determine team colors dynamically based on target's team:
	new teamColorCode[12];
	new hpBoxColor;
	switch (Player[targetid][Team])
	{
		case ATTACKER:
		{
			teamColorCode = "~g~~h~";
			hpBoxColor = 0x3A5F2B95;
		}
		case ATTACKER_SUB:
		{
			teamColorCode = "~g~~h~";
			hpBoxColor = 0xA4F2B495;
		}
		case DEFENDER:
		{
			teamColorCode = "~b~~h~";
			hpBoxColor = 0x5462B995;
		}
		case DEFENDER_SUB:
		{
			teamColorCode = "~b~~h~~h~";
			hpBoxColor = 0x8CE0FC95;
		}
		case REFEREE:
		{
			teamColorCode = "~y~~h~";
			hpBoxColor = 0xF7D07095;
		}
		default:
		{
			teamColorCode = "~w~";
			hpBoxColor = 0x47893B95;
		}
	}

	// 2. Target Name & ID: "Name [ID: X]"
	new nameStr[MAX_PLAYER_NAME + 24];
	format(nameStr, sizeof(nameStr), "~w~%s %s[ID: %d]", Player[targetid][Name], teamColorCode, targetid);
	PlayerTextDrawSetString(playerid, TargetInfoData[playerid][TargetInfo_Name], nameStr);
	PlayerTextDrawShow(playerid, TargetInfoData[playerid][TargetInfo_Name]);

	// 3. Armour Bar & Percent
	new ap = GetAP(targetid);
	if (ap < 0) ap = 0;
	if (ap > 100) ap = 100;
	if (ap > 0)
	{
		new Float: apSpan = (float(ap) / 100.0) * 73.415;
		PlayerTextDrawTextSize(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], 49.576 + apSpan, 930.000);
		PlayerTextDrawShow(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar]);

		new apStr[12];
		format(apStr, sizeof(apStr), "%d", ap);
		PlayerTextDrawSetString(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent], apStr);
		PlayerTextDrawColor(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent], -1);
		PlayerTextDrawShow(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent]);
	}
	else
	{
		PlayerTextDrawHide(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar]);
		PlayerTextDrawHide(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent]);
	}

	// 4. Health Bar & Percent
	new hp = GetHP(targetid);
	if (hp < 0) hp = 0;
	if (hp > 100) hp = 100;
	if (hp > 0)
	{
		new Float: hpSpan = (float(hp) / 100.0) * 73.415;
		PlayerTextDrawTextSize(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], 49.576 + hpSpan, 930.000);
		PlayerTextDrawBoxColor(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], hpBoxColor);
		PlayerTextDrawColor(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], -1);
		PlayerTextDrawShow(playerid, TargetInfoData[playerid][TargetInfo_HealthBar]);

		new hpStr[12];
		format(hpStr, sizeof(hpStr), "%d", hp);
		PlayerTextDrawSetString(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], hpStr);
		PlayerTextDrawShow(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent]);
	}
	else
	{
		PlayerTextDrawHide(playerid, TargetInfoData[playerid][TargetInfo_HealthBar]);
		PlayerTextDrawSetString(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], "0");
		PlayerTextDrawShow(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent]);
	}

	// 5. Net info: Ping, FPS, PL
	new netStr[96];
	format(netStr, sizeof(netStr), "~w~Ping: %s%d   ~w~FPS: %s%d   ~w~PL: %s%.1f%%",
		teamColorCode, GetPlayerPing(targetid),
		teamColorCode, GetPlayerFPS(targetid),
		teamColorCode, NetStats_PacketLossPercent(targetid));
	PlayerTextDrawSetString(playerid, TargetInfoData[playerid][TargetInfo_Net], netStr);
	PlayerTextDrawShow(playerid, TargetInfoData[playerid][TargetInfo_Net]);

	TargetInfoData[playerid][TargetInfoLastTick]   = currentTick;
	TargetInfoData[playerid][TargetInfoLastTarget] = targetid;
	return 1;
}

forward HideAnnForAll();
public HideAnnForAll()
{
	AnnTimer = 0;
	TextDrawHideForAll(AnnTD);
}

bool: CanPlay(playerid)
{
	if (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER)
		return false;

	if (Player[playerid][InDuel] || Player[playerid][IsAFK] || !Player[playerid][Spawned])
		return false; // can not play

	return true; // can play
}

bool: IsValidSound(soundid)
{
	for (new i = 0; i < sizeof(ValidSounds); ++i)
	{
		if (ValidSounds[i] == soundid)
			return true;
	}
	return false;
}

GetWeaponID(const weaponname[])
{
	for (new i = 0; i < sizeof(g_WeaponName); ++i)
	{
		if (strfind(g_WeaponName[i], weaponname, true) != -1)
			return i;
	}
	return -1;
}

GetWeaponModelFromID(weaponid)
{
	new model = -1;
	switch (weaponid)
	{
		case 0, 1: model = 331;
		case 2: model = 333;
		case 3: model = 334;
		case 4: model = 335;
		case 5: model = 336;
		case 6: model = 337;
		case 7: model = 338;
		case 8: model = 339;
		case 9: model = 341;
		case 10: model = 321;
		case 11: model = 322;
		case 12: model = 323;
		case 13: model = 324;
		case 14: model = 325;
		case 15: model = 326;
		case 16: model = 342;
		case 17: model = 343;
		case 18: model = 344;
		case 22: model = 346;
		case 23: model = 347;
		case 24: model = 348;
		case 25: model = 349;
		case 26: model = 350;
		case 27: model = 351;
		case 28: model = 352;
		case 29: model = 353;
		case 30: model = 355;
		case 31: model = 356;
		case 32: model = 372;
		case 33: model = 357;
		case 34: model = 358;
		case 35: model = 359;
		case 36: model = 360;
		case 37: model = 361;
		case 38: model = 362;
		case 39: model = 363;
		case 41: model = 365;
		case 42: model = 366;
		case 46: model = 371;
	}
	return model;
}

OnPlayerAmmoUpdate(playerid)
{
	if (Player[playerid][Playing])
	{
		new weapons;
		new Ammo, TotalShots, Float: accuracy;

		for (new k = 2; k < 8; ++k)
		{
			GetPlayerWeaponData(playerid, k, weapons, Ammo);
			if (Ammo > 10)
				TotalShots += (9999 - Ammo);
		}

		accuracy = TotalShots == 0 ? 0.0 : floatmul(100.0, floatdiv(Player[playerid][shotsHit], TotalShots));

		Player[playerid][TotalBulletsFired] += TotalShots;
		Player[playerid][TotalshotsHit] += Player[playerid][shotsHit];
		Player[playerid][Accuracy] = accuracy;
		Player[playerid][TotalAccuracy] += accuracy;
	}
}

stock bool: DoesPlayerHaveWeaponInSlot(playerid, slot)
{
	new wep, ammo;
	GetPlayerWeaponData(playerid, slot, wep, ammo);
	return wep != 0 && ammo > 0;
}

bool: DoesPlayerHaveWeapon(playerid, weaponid)
{
	new wep, ammo;
	GetPlayerWeaponData(playerid, GetWeaponSlot(weaponid), wep, ammo);
	return wep == weaponid && ammo > 0;
}

GetCardinalPoint(Float: degree)
{
	// this function return a string that contain the Cardinal point of a heading direction

	// Create string and initialize it to hold the direction text
	new CardinalPoint[11];

	/*
		From SA:MP wiki

		"Angles are reversed in GTA:SA; 90 degrees would be
		East in the real world, but in GTA:SA 90 degrees is
		in fact West. North and South are still 0/360 and 180
	*/
	degree = 360.0 - degree; // correct conversion

	// each cardinal point cover 45degree (45 X 8 = 360)
	if (337.5 <= degree <= 360)
		strcat(CardinalPoint, "North");
	else if (0 <= degree <= 22.5)
		strcat(CardinalPoint, "North");
	else if (22.5 <= degree <= 67.5)
		strcat(CardinalPoint, "North East");
	else if (67.5 <= degree <= 112.5)
		strcat(CardinalPoint, "East");
	else if (112.5 <= degree <= 157.5)
		strcat(CardinalPoint, "South East");
	else if (157.5 <= degree <= 202.5)
		strcat(CardinalPoint, "South");
	else if (202.5 <= degree <= 247.5)
		strcat(CardinalPoint, "South West");
	else if (247.5 <= degree <= 292.5)
		strcat(CardinalPoint, "West");
	else if (292.5 <= degree <= 337.5)
		strcat(CardinalPoint, "North West");
	else
		strcat(CardinalPoint, "Unknown"); // error

	return CardinalPoint; // we return our string
}

bool: IsPlayerInArea(playerid, Float: minx, Float: maxx, Float: miny, Float: maxy)
{
	new Float: x,
		Float: y,
		Float: z;
	GetPlayerPos(playerid, x, y, z);
	return x > minx && x < maxx && y > miny && y < maxy;
}

/*
GetPlayerHighestScores(array[][rankingEnum], left, right)
{
	new
		tempLeft = left,
		tempRight = right,
		pivot = array[(left + right) / 2][player_Score],
		tempVar
	;
	while(tempLeft <= tempRight)
	{
		while(array[tempLeft][player_Score] > pivot) tempLeft++;
		while(array[tempRight][player_Score] < pivot) tempRight--;

		if(tempLeft <= tempRight)
		{
			tempVar = array[tempLeft][player_Score], array[tempLeft][player_Score] = array[tempRight][player_Score], array[tempRight][player_Score] = tempVar;
			tempVar = array[tempLeft][player_ID], array[tempLeft][player_ID] = array[tempRight][player_ID], array[tempRight][player_ID] = tempVar;
//			format(tempVarStr, sizeof(tempVarStr), array[tempLeft][player_Name]); format(array[tempLeft][player_Name], MAX_PLAYER_NAME, array[tempRight][player_Name]); format(array[tempRight][player_Name], MAX_PLAYER_NAME, tempVarStr);
			//tempVarStr = array[tempLeft][player_Name], array[tempLeft][player_Name] = array[tempRight][player_Name], array[tempRight][player_Name] = tempVarStr;
			tempVar = array[tempLeft][player_Kills], array[tempLeft][player_Kills] = array[tempRight][player_Kills], array[tempRight][player_Kills] = tempVar;
			tempVar = array[tempLeft][player_Deaths], array[tempLeft][player_Deaths] = array[tempRight][player_Deaths], array[tempRight][player_Deaths] = tempVar;
			tempVar = array[tempLeft][player_Team], array[tempLeft][player_Team] = array[tempRight][player_Team], array[tempRight][player_Team] = tempVar;
			tempLeft++, tempRight--;
		}
	}
	if(left < tempRight) GetPlayerHighestScores(array, left, tempRight);
	if(tempLeft < right) GetPlayerHighestScores(array, tempLeft, right);
}*/

GetPlayerHighestScores2(array[][rankingEnum], names[][MAX_PLAYER_NAME], left, right)
{
	new tempLeft = left, tempRight = right, pivot = array[(left + right) / 2][player_Score], tempVar, tempVarStr[MAX_PLAYER_NAME];
	while (tempLeft <= tempRight)
	{
		while (array[tempLeft][player_Score] > pivot) tempLeft++;
		while (array[tempRight][player_Score] < pivot) tempRight--;

		if (tempLeft <= tempRight)
		{
			tempVar = array[tempLeft][player_Score], array[tempLeft][player_Score] = array[tempRight][player_Score], array[tempRight][player_Score] = tempVar;
			tempVar = array[tempLeft][player_ID], array[tempLeft][player_ID] = array[tempRight][player_ID], array[tempRight][player_ID] = tempVar;
			tempVarStr = names[tempLeft], names[tempLeft] = names[tempRight], names[tempRight] = tempVarStr;
			tempVar = array[tempLeft][player_Kills], array[tempLeft][player_Kills] = array[tempRight][player_Kills], array[tempRight][player_Kills] = tempVar;
			tempVar = array[tempLeft][player_Deaths], array[tempLeft][player_Deaths] = array[tempRight][player_Deaths], array[tempRight][player_Deaths] = tempVar;
			tempVar = array[tempLeft][player_Team], array[tempLeft][player_Team] = array[tempRight][player_Team], array[tempRight][player_Team] = tempVar;
			tempVar = array[tempLeft][player_TPlayed], array[tempLeft][player_TPlayed] = array[tempRight][player_TPlayed], array[tempRight][player_TPlayed] = tempVar;
			tempVar = array[tempLeft][player_HP], array[tempLeft][player_HP] = array[tempRight][player_HP], array[tempRight][player_HP] = tempVar;
			tempVar = array[tempLeft][player_Acc], array[tempLeft][player_Acc] = array[tempRight][player_Acc], array[tempRight][player_Acc] = tempVar;
			tempVar = array[tempLeft][player_Overall], array[tempLeft][player_Overall] = array[tempRight][player_Overall], array[tempRight][player_Overall] = tempVar;
			tempLeft++, tempRight--;
		}
	}
	if (left < tempRight)
		GetPlayerHighestScores2(array, names, left, tempRight);
	if (tempLeft < right)
		GetPlayerHighestScores2(array, names, tempLeft, right);
}

forward SetCPForPlayer(playerid);
public SetCPForPlayer(playerid)
{
	switch (GameType)
	{
		case BASE:
		{
			SetPlayerCheckpoint(playerid, BCPSpawn[Current][0], BCPSpawn[Current][1], BCPSpawn[Current][2], 2);
			UpdateCPZoneVisibility(playerid);
			RefreshCheckpointFlares(playerid);
		}
		case ARENA:
		{
			if (!CPInArena)
			{
				DestroyArenaFlareObjects(playerid);
				return 0;
			}
			SetPlayerCheckpoint(playerid, ACPSpawn[Current][0], ACPSpawn[Current][1], ACPSpawn[Current][2], 2);
			GangZoneShowForPlayer(playerid, CPZone, GANGZONE_CPARENA);
			RefreshCheckpointFlares(playerid);
		}
	}
	return 1;
}

forward RespawnInVehicleAfterComeBack(playerid, vehicleid, seatid, connectionGeneration, rejoinGeneration);
bool: IsTeamTheSame(team1, team2)
{
	if (team1 == team2)
		return true;
	else if (team1 == DEFENDER && team2 == DEFENDER_SUB || team1 == DEFENDER_SUB && team2 == DEFENDER)
		return true;
	else if (team1 == ATTACKER && team2 == ATTACKER_SUB || team1 == ATTACKER_SUB && team2 == ATTACKER)
		return true;

	return false;
}

ResetPlayerWorldBounds(playerid) { ClearPlayerWorldBounds(playerid); }

ColorFix(playerid)
{
	if (Player[playerid][Playing])
	{
		switch (Player[playerid][Team])
		{
			case ATTACKER: SetPlayerColor(playerid, ATTACKER_PLAYING);
			case DEFENDER: SetPlayerColor(playerid, DEFENDER_PLAYING);
			case REFEREE: SetPlayerColor(playerid, REFEREE_COLOR);
		}

		new team = Player[playerid][Team];
		if (TeamHasLeader[team] && TeamLeader[team] == playerid)
			PlayerLeadTeam(playerid, true, false);

		UpdatePlayerHealthBar(playerid);
		return;
	}

	switch (Player[playerid][Team])
	{
		case ATTACKER: SetPlayerColor(playerid, ATTACKER_NOT_PLAYING);
		case DEFENDER: SetPlayerColor(playerid, DEFENDER_NOT_PLAYING);
		case REFEREE: SetPlayerColor(playerid, REFEREE_COLOR);
		case ATTACKER_SUB: SetPlayerColor(playerid, ATTACKER_SUB_COLOR);
		case DEFENDER_SUB: SetPlayerColor(playerid, DEFENDER_SUB_COLOR);
	}

	UpdatePlayerHealthBar(playerid);
}

RadarFix(playerid = INVALID_PLAYER_ID)
{
	if (playerid == INVALID_PLAYER_ID) // if markers need a global update
	{
		foreach (new i : Player)
		{
			foreach (new x : Player)
			{
				CallLocalFunction("OnPlayerStreamIn", "ii", i, x);
			}
		}
		return;
	}

	// if only one player's color is changed and needs updating
	foreach (new i : Player)
	{
		CallLocalFunction("OnPlayerStreamIn", "ii", playerid, i);
	}
}

ClearChat()
{
	for (new i = 0; i < 20; i++) SendClientMessageToAll(-1, " ");
}

ClearKillList()
{
	for (new i = 0; i < 5; i++) { SendDeathMessage(255, 50, 255); }
}

DestroyUnusedVehicles() { FreezeV_DestroyAll(); }

forward ForceRespectate(playerid, generation);
public ForceRespectate(playerid, generation)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != generation)
		return 0;

	foreach (new i : PlayerSpectators[playerid])
		SpectatePlayer(i, playerid);
	return 1;
}

SyncPlayer(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	// Restrictions
	new currentTick = GetTickCount();
	if (currentTick - Player[playerid][LastTimeSynced] < 1000)
		return 1;
	if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)
		return 1;
	if (GBUG_IsPerformingEntering(playerid))
		return 1;
	if ((RoundPaused || !AllowSync) && Player[playerid][Playing])
		return 1;
	if (Player[playerid][OnGunmenu])
		return SendErrorMessage(playerid, "Can't sync while in gunmenu");
	if (Player[playerid][IsAFK] || !IsPlayerControllable(playerid))
		return SendErrorMessage(playerid, "Can't sync while frozen or in AFK mode");

	Player[playerid][LastTimeSynced] = currentTick;
	// Set spectators to re-spectate
	if (Player[playerid][BeingSpeced])
	{
		if (!SetTimerEx("ForceRespectate", GetPlayerPing(playerid) + 10, false, "ii", playerid, PlayerConnectionGeneration[playerid]))
		{
			printf("[AttDef] Failed to schedule spectator restoration after sync for player %d.", playerid);
			ForceRespectate(playerid, PlayerConnectionGeneration[playerid]);
		}
	}

	ResyncPlayer(playerid);
	UpdatePlayerHealthBar(playerid);
	SendClientMessage(playerid, -1, "\xBB Sync successful!");
	return 1;
}

FixVsTextDraw(exception = -1)
{
	if (Current == -1)
	{
		new str[32];
		new ct[2];
		foreach (new i : Player)
		{
			if (i == exception)
				continue;

			switch (Player[i][Team])
			{
				case ATTACKER: ct[0]++;
				case DEFENDER: ct[1]++;
			}
		}
		//format(str, sizeof(str), "~r~%d  %svs  ~b~~h~%d", ct[0], MAIN_TEXT_COLOUR, ct[1]);
		foreach (new i : Player)
			PlayerTextDrawSetString(i, BaseID_VS[i], str);
	}
	return 1;
}

Float: GetDistanceBetweenPlayers(playerid, toplayerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || toplayerid < 0 || toplayerid >= MAX_PLAYERS
		|| !IsPlayerConnected(playerid) || !IsPlayerConnected(toplayerid))
		return -1.0;

	new Float: Pos[2][3];
	GetPlayerPos(playerid, Pos[0][0], Pos[0][1], Pos[0][2]);
	GetPlayerPos(toplayerid, Pos[1][0], Pos[1][1], Pos[1][2]);
	return floatsqroot(
		floatpower(floatabs(floatsub(Pos[1][0], Pos[0][0])), 2) + floatpower(floatabs(floatsub(Pos[1][1], Pos[0][1])), 2)
		+ floatpower(floatabs(floatsub(Pos[1][2], Pos[0][2])), 2));
}

SpawnInDM(playerid, DMID)
{
	HideAllTeamBarsForPlayer(playerid);
	SetPlayerDMState(playerid, DMID);

	ResetPlayerWeapons(playerid); // Reset all player weapons

	if (Player[playerid][DmPosX] == -1.0)
		SetSpawnInfo(
			playerid, NO_TEAM, Skin[Player[playerid][Team]], DMSpawn[DMID][0] + RandomMinMax(-1, 1), DMSpawn[DMID][1] + RandomMinMax(-1, 1), DMSpawn[DMID][2], DMSpawn[DMID][3], DMWeapons[DMID][0],
			9999, DMWeapons[DMID][1], 9999, DMWeapons[DMID][2], 9999);
	else
		SetSpawnInfo(
			playerid, NO_TEAM, Skin[Player[playerid][Team]], Player[playerid][DmPosX], Player[playerid][DmPosY], DMInterior[DMID] ? Player[playerid][DmPosZ] + 1 : Player[playerid][DmPosZ], Player[playerid][DmAngle],
			DMWeapons[DMID][0], 9999, DMWeapons[DMID][1], 9999, DMWeapons[DMID][2], 9999);

	SetPlayerInterior(playerid, DMInterior[DMID]);
	Player[playerid][IgnoreSpawn] = true; // Make sure you ignore OnPlayerSpawn, else you will just spawn in
										  // lobby (because u are about to use SpawnPlayer).
	SpawnPlayer(playerid);				  // Spawns players, in this case we have SetSpawnInfo (but still
										  // you need to make sure OnPlayerSpawn is ignored);

	SetPlayerVirtualWorld(
		playerid,
		Player[playerid][VWorld]); // Put player in a different virtual world so that if you
								   // create a DM in your lobby and you join the DM, you
								   // won't be able to see other players in the lobby.
	SetHPAP(playerid, 100.0, 100.0);
}

stock SetPlayerDMState(playerid, dmID)
{
	Player[playerid][InDM] = true;
	Player[playerid][DMReadd] = dmID;
	return 1;
}

stock ClearPlayerDMState(playerid)
{
	Player[playerid][InDM] = false;
	Player[playerid][DMReadd] = -1;
	return 1;
}

QuitDM(playerid)
{
	if (Player[playerid][Playing])
		return 1;
	if (!Player[playerid][InDM] && Player[playerid][DMReadd] == -1)
		return 1;

	Player[playerid][DmPosX]  = -1.0;
	Player[playerid][DmPosY]  = -1.0;
	Player[playerid][DmPosZ]  = -1.0;
	Player[playerid][DmAngle] = -1.0;

	ClearPlayerDMState(playerid);
	SpawnInLobby(playerid);
	return 1;
}

bool: IsSafeGametext(const text[])
{
	new cnt = 0;
	for (new i = 0, j = strlen(text); i < j; i++)
	{
		if (text[i] == '~')
			cnt++;
	}
	return bool:IsEven(cnt);
}

// Function written by Slice to help fix vehicle bad modding (tweaked it a little bit)
SendAdminMessage(const message[])
{
	foreach (new i : Player)
	{
		if (Player[i][Level] > 0 || IsPlayerAdmin(i))
		{
			SendClientMessage(i, 0xA85C60AA, message);
		}
	}
}

public GetPlayerTeamID(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;
	return Player[playerid][Team];
}
