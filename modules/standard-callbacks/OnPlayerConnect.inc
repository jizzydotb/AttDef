#if defined _attdef_OnPConnect
	#endinput
#endif
#define _attdef_OnPConnect

hook GOOD_OnPlayerConnect(playerid)
{
	// Cleans chat
	for (new i = 0; i < 20; i++) SendClientMessage(playerid, -1, " ");

	SendClientMessage(playerid, -1, "{93DCAC}~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	va_SendClientMessage(playerid, -1, "{93DCAC}\xBB Welcome to {FFFFFF}%s Server | {FFFFFF}%s Version: {93DCAC}%s", ServerName, GM_NAME, GM_VERSION);
	SendClientMessage(playerid, -1, "{93DCAC}\xBB Silent ASI Loader: {FFFFFF}shorturl.at/szCR0 {FFFFFF}| {93DCAC}Sniper Bug Fix: {FFFFFF}shorturl.at/bgpH1");

	SendClientMessage(playerid, -1, "{93DCAC}\xBB {FFFFFF}Use {93DCAC}/vote {FFFFFF}to start or vote for a round | {93DCAC}/cmds {FFFFFF}to see commands");
	SendClientMessage(playerid, -1, "{93DCAC}\xBB {FFFFFF}Use {93DCAC}/lastmatches {FFFFFF}to view match history | {93DCAC}/style {FFFFFF}to customize your HUD");
	SendClientMessage(playerid, -1, "{93DCAC}~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	return 1;
}

hook OnPlayerConnect(playerid)
{
	EnablePlayerCameraTarget(playerid, true);
	RemovePlayerMapIcon(playerid, 33);

	// Initialize the new player
	PlayerConnectionGeneration[playerid]++;
	if (PlayerConnectionGeneration[playerid] == 0)
		PlayerConnectionGeneration[playerid]++;
	CallLocalFunction("ResetPlayerActiveVotes", "i", playerid);
	InitPlayer(playerid);
	AllowedViewingCommands[playerid] = false;
	BlackListViewingCommands[playerid] = false;
	NormalListViewingCommands[playerid] = false;
	w0[playerid] = INVALID_OBJECT_ID;
	w0Timer[playerid] = 0;
	CallLocalFunction("ResetPlayerVotesOnDisconnect", "i", playerid);
	CallLocalFunction("GOOD_OnPlayerConnect", "i", playerid);
	CheckPlayerAKA(playerid);
	GetPlayerName(playerid, gGeoReadyName[playerid], MAX_PLAYER_NAME);
	GetPlayerIp(playerid, gGeoReadyIP[playerid], sizeof(gGeoReadyIP[]));
	Player[playerid][gPauseWarnings] = 0;
	Player[playerid][gWasPaused] = 0;
	Player[playerid][gPauseRemovedFromCP] = false;
	Player[playerid][gPauseLabelVisible] = false;
	Player[playerid][gLastUpdate] = GetTickCount();
	Player[playerid][gPauseCheck] = false;
	return 1;
}
