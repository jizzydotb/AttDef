#if defined _attdef_OnPDisconnect
	#endinput
#endif
#define _attdef_OnPDisconnect

public OnPlayerDisconnect(playerid, reason)
{
	ClearDamageIndicator(playerid);
	DestroyPlayerFloatingLabels(playerid);
	DestroyPlayerDeathEffects(playerid);
	AllowedViewingCommands[playerid] = false;
	BlackListViewingCommands[playerid] = false;
	NormalListViewingCommands[playerid] = false;
	DestroyTeamPingsForPlayer(playerid);
	PlayerRejoinSpawnGeneration[playerid]++;
	PlayerVehicleRejoinGeneration[playerid]++;
	DelayedFreezeGeneration[playerid]++;
	CallLocalFunction("ResetPlayerActiveVotes", "i", playerid);
	CallLocalFunction("ResetPlayerVotesOnDisconnect", "i", playerid);

	g_discordVerifyCode[playerid] = 0;
	g_discordVerifyExpire[playerid] = 0;
	g_discordVerifyAccount[playerid][0] = EOS;

	if (Player[playerid][WasInCP])
		CallLocalFunction("OnPlayerLeaveCheckpoint", "i", playerid);

	RoundTeamHPHUDReady[playerid] = false;
	RoundTeamHPHUDShown[playerid] = false;
	RoundTeamHPCPReady[playerid] = false;
	RoundTeamHPCPShown[playerid] = false;

	if (AllVsAllMode)
	{
		Player[playerid][RoundTDStyle] = Player[playerid][PreAllVsAllStyle];
		UpdateAllVsAllTextDraws();
	}

	// Send public disconnect messages
	static const disconnectReasons[][] = {
		"(crash/timeout)",
		"(Leaving)",
		"(Kick/Ban)"
	};

	if (!Player[playerid][Playing])
	{
		va_SendClientMessageToAll(
			-1, "{CCCCCC}\xBB Disconnected: {FFFFFF}%s {CCCCCC}has left the server. {FF4954}%s",
			Player[playerid][Name], (reason >= 0 && reason < sizeof(disconnectReasons) ? disconnectReasons[reason] : "(Unknown)")
		);
	}
	else
	{
		va_SendClientMessageToAll(
			-1, "{CCCCCC}\xBB Disconnected: {FFFFFF}%s {CCCCCC}has left the server. {FF4954}%s {CCCCCC}- (HP: %d - %d)",
			Player[playerid][Name], (reason >= 0 && reason < sizeof(disconnectReasons) ? disconnectReasons[reason] : "(Unknown)"), Player[playerid][pHealth], Player[playerid][pArmour]
		);
	}

	// Check player spawned vehicle
	new vehicleid = Player[playerid][LastVehicle];
	if (vehicleid)
	{
		foreach (new i : VehicleDriver(vehicleid))
		{
			RemovePlayerFromVehicle(i);
		}
		foreach (new i : VehiclePassenger(vehicleid))
		{
			RemovePlayerFromVehicle(i);
		}
		DestroyVehicle(vehicleid);
		Player[playerid][LastVehicle] = 0;
	}
	
	// Fixes the x Vs y textdraws with current team player count
	FixVsTextDraw(playerid);

	if (HalfDeagle && Current == -1)
	{
		AdjustDeagleLimit();
	}
	StopTipsTimers(playerid);

	/*To End the loop of Using mods*/
	Player[playerid][UsingCleo] = false;
	Player[playerid][UsingSprintHook] = false;
	Player[playerid][UsingSampfuncs] = false;
	Player[playerid][UsingExtraws] = false;
	Player[playerid][UsingSobeit] = false;
	Player[playerid][UsingImprovedDeagle] = false;

	/*To End the loop of mods status*/
	Player[playerid][CleoPStatus] = false;
	Player[playerid][SprintHookPStatus] = false;
	Player[playerid][SampfuncsPStatus] = false;
	Player[playerid][ExtrawsPStatus] = false;
	Player[playerid][SobeitPStatus] = false;
	Player[playerid][ImprovedDeaglePStatus] = false;

	Player[playerid][gPauseWarnings] = 0;
	Player[playerid][gWasPaused] = 0;
	Player[playerid][gPauseRemovedFromCP] = false;
	Player[playerid][gPauseLabelVisible] = false;
	Player[playerid][gPauseCheck] = false;
	ResetPlayerAssistDamage(playerid);

	return 1;
}
