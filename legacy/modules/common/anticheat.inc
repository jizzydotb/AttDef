#if defined _attdef_common_anticheat
	#endinput
#endif
#define _attdef_common_anticheat

/*
	Anti-Cheat (AC Black Diamond), Mod Detection, and Network Monitoring Sub-module
*/

static gModReportTimer[MAX_PLAYERS];

forward OnDeferredModDetectionReport(playerid, generation);
public OnDeferredModDetectionReport(playerid, generation)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	gModReportTimer[playerid] = 0;
	if (!IsPlayerConnected(playerid) || PlayerConnectionGeneration[playerid] != generation)
		return 0;

	UpdateModDetectionMessage(playerid);
	return 1;
}

stock ScheduleModDetectionReport(playerid, delay = 800)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	if (gModReportTimer[playerid])
	{
		KillTimer(gModReportTimer[playerid]);
		gModReportTimer[playerid] = 0;
	}

	gModReportTimer[playerid] = SetTimerEx(
		"OnDeferredModDetectionReport",
		delay,
		false,
		"ii",
		playerid,
		PlayerConnectionGeneration[playerid]
	);
	return gModReportTimer[playerid];
}

public OnSprintHookDetected(playerid)
{
	Player[playerid][UsingSprintHook] = true;
	Player[playerid][SprintHookPStatus] = true;
	ScheduleModDetectionReport(playerid);

	if (!UseSprinthook)
	{
		printf("[AC] SprintHook detected (ID: %d)", playerid);
		SendClientMessage(playerid, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"SprintHook.asi "COL_PRIM"from your GTA folder.");
		Discord_SendAntiCheatAlert(playerid, "SprintHook.asi", "Player connected with disallowed modification `SprintHook.asi`.");
		SchedulePlayerKick(playerid, 500);
	}

	return 1;
}

public OnModsDetected(playerid)
{
	Player[playerid][UsingCleo] = true;
	Player[playerid][CleoPStatus] = true;
	ScheduleModDetectionReport(playerid);

	if (!UseCleo)
	{
		printf("[AC] Cleo detected (ID: %d)", playerid);
		SendClientMessage(playerid, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"Cleo.asi "COL_PRIM"from your GTA folder.");
		Discord_SendAntiCheatAlert(playerid, "Cleo.asi", "Player connected with disallowed modification `Cleo.asi`.");
		SchedulePlayerKick(playerid, 500);
	}

	return 1;
}

public OnSampfuncsDetected(playerid)
{
	Player[playerid][UsingSampfuncs] = true;
	Player[playerid][SampfuncsPStatus] = true;
	ScheduleModDetectionReport(playerid);

	if (!UseSampfuncs)
	{
		printf("[AC] SAMPFUNCS detected (ID: %d)", playerid);
		SendClientMessage(playerid, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"SAMPFUNCS.asi "COL_PRIM"from your GTA folder.");
		Discord_SendAntiCheatAlert(playerid, "SAMPFUNCS.asi", "Player connected with disallowed modification `SAMPFUNCS.asi`.");
		SchedulePlayerKick(playerid, 500);
	}
	return 1;
}

public OnExtraWsDetected(playerid)
{
	Player[playerid][UsingExtraws] = true;
	Player[playerid][ExtrawsPStatus] = true;
	ScheduleModDetectionReport(playerid);

	if (!UseExtraws)
	{
		printf("[AC] ExtraWS detected (ID: %d)", playerid);
		SendClientMessage(playerid, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"ExtraWS.asi "COL_PRIM"from your GTA folder.");
		Discord_SendAntiCheatAlert(playerid, "ExtraWS.asi", "Player connected with disallowed modification `ExtraWS.asi`.");
		SchedulePlayerKick(playerid, 500);
	}
	return 1;
}

public OnOldVersionDetected(playerid)
{
	printf("[AC] Old client version detected (ID: %d)", playerid);
	SendClientMessage(playerid, COL_PRIM_HEX, "\xBB Your client version is not supported by the anti-cheat. Please use "COL_SEC"open.mp launcher "COL_PRIM"or update to "COL_SEC"0.3.7-R2+"COL_PRIM".");
	Discord_SendAntiCheatAlert(playerid, "Unsupported Client Version", "Player connected with an outdated SA-MP client unsupported by the anti-cheat.");
	SchedulePlayerKick(playerid, 500);
	return 1;
}

public OnImprovedDeagleDetected(playerid)
{
	Player[playerid][UsingImprovedDeagle] = true;
	Player[playerid][ImprovedDeaglePStatus] = true;
	ScheduleModDetectionReport(playerid);
	printf("[AC] ImprovedDeagle detected (ID: %d)", playerid);
	SendClientMessage(playerid, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"ImprovedDeagle.asi "COL_PRIM"from your GTA folder.");
	Discord_SendAntiCheatAlert(playerid, "ImprovedDeagle.asi", "Auto C-Bug / rapid-fire cheat (`ImprovedDeagle.asi`) detected.");
	SchedulePlayerKick(playerid, 500);
	return 1;
}

public OnS0beitDetected(playerid)
{
	Player[playerid][UsingSobeit] = true;
	Player[playerid][SobeitPStatus] = true;
	ScheduleModDetectionReport(playerid);
	printf("[AC] s0beit detected (ID: %d)", playerid);
	SendClientMessage(playerid, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"s0beit/cheats "COL_PRIM"from your GTA folder.");
	Discord_SendAntiCheatAlert(playerid, "s0beit", "Multi-hack cheat client (`s0beit`) detected.");
	SchedulePlayerKick(playerid, 500);
	return 1;
}

public OnBypassDetected(playerid)
{
	printf("[AC] Anti-cheat bypass detected (ID: %d)", playerid);
	va_SendClientMessageToAll(-1, COL_PRIM "\xBB (Anti Cheat) "COL_SEC"%s "COL_PRIM"has been kicked for attempting to bypass the anti-cheat.", Player[playerid][Name]);
	SendClientMessage(playerid, COL_PRIM_HEX, "\xBB Anti-cheat bypass detected. Please remove any bypass tools from your GTA folder.");
	Discord_SendAntiCheatAlert(playerid, "Anti-Cheat Bypass", "Attempted to bypass or tamper with AC Black Diamond checks.");
	SchedulePlayerKick(playerid, 500);
	return 1;
}

public OnSilentAimDetected(playerid)
{
	printf("[AC] Silent Aim detected (ID: %d)", playerid);
	va_SendClientMessageToAll(-1, COL_PRIM "\xBB (Anti Cheat) "COL_SEC"%s "COL_PRIM"has been kicked for suspected "COL_SEC"Silent Aim"COL_PRIM".", Player[playerid][Name]);
	SendClientMessage(playerid, COL_PRIM_HEX, "\xBB Silent Aim detected. Please remove cheats from your GTA folder.");
	Discord_SendAntiCheatAlert(playerid, "Silent Aim", "Suspected aimbot (`Silent Aim`) detected during combat.");
	SchedulePlayerKick(playerid, 500);
	return 1;
}

forward OnPlayerKicked(playerid, generation);
public OnPlayerKicked(playerid, generation)
{
	if (!IsPlayerConnected(playerid) || PlayerConnectionGeneration[playerid] != generation)
		return 0;

	Kick(playerid);
	return 1;
}

SchedulePlayerKick(playerid, delay)
{
	if (!IsPlayerConnected(playerid))
		return 0;

	new timer = SetTimerEx("OnPlayerKicked", delay, false, "ii", playerid, PlayerConnectionGeneration[playerid]);
	if (!timer)
	{
		printf("[AttDef] Failed to schedule kick for player %d; kicking immediately.", playerid);
		Kick(playerid);
		return 0;
	}
	return timer;
}


// Function to update the mod detection message for all players
forward UpdateModDetectionMessage(playerid);
public UpdateModDetectionMessage(playerid)
{
	if (playerid >= 0 && playerid < MAX_PLAYERS && gModReportTimer[playerid])
	{
		KillTimer(gModReportTimer[playerid]);
		gModReportTimer[playerid] = 0;
	}

	if(Player[playerid][UsingCleo] || Player[playerid][UsingSprintHook] || Player[playerid][UsingSampfuncs] || Player[playerid][UsingExtraws] || Player[playerid][UsingSobeit] || Player[playerid][UsingImprovedDeagle])
	{
		new modMessage[256], modStatus[256];
	
		// Build the status message of detected mods
		format(modMessage, sizeof(modMessage), COL_PRIM "\xBB (Anti Cheat) "COL_SEC"%s "COL_PRIM"is using mods:"COL_SEC" ", Player[playerid][Name]);

		// Check which mods are active and append them to the message
		if (Player[playerid][UsingCleo])
		{
			strcat(modStatus, "Cleo.asi, ");
			Player[playerid][CleoPStatus] = true;
		}
		if (Player[playerid][UsingSprintHook])
		{
			strcat(modStatus, "Sprinthook.asi, ");
			Player[playerid][SprintHookPStatus] = true;
		}
		if (Player[playerid][UsingSampfuncs])
		{
			strcat(modStatus, "Sampfuncs.asi, ");
			Player[playerid][SampfuncsPStatus] = true;
		}	
		if (Player[playerid][UsingExtraws])
		{
			strcat(modStatus, "Extraws.asi, ");
			Player[playerid][ExtrawsPStatus] = true;
		}	
		if (Player[playerid][UsingSobeit])
		{
			strcat(modStatus, "s0beit cheat, ");
			Player[playerid][SobeitPStatus] = true;
		}
		if (Player[playerid][UsingImprovedDeagle])
		{
			strcat(modStatus, "ImprovedDeagle.asi, ");
			Player[playerid][ImprovedDeaglePStatus] = true;
		}	

		// Remove the trailing comma and space if any mod was detected
		if (strlen(modStatus) > 0)
			modStatus[strlen(modStatus) - 2] = EOS; // Strip the last comma and space

		// Append the mod status list to the main message
		strcat(modMessage, modStatus);

		// Broadcast the updated message to all players
		va_SendClientMessageToAll(-1, modMessage);
	}
	return 1;
}

// Clear the spectator state for a player
forward ClearSpectatorState(playerid);

stock HandleNetworkIssue(playerid)
{
	new pFPS = GetPlayerFPS(playerid);
	new Float: pPL = NetStats_PacketLossPercent(playerid);
	new pPing = GetPlayerPing(playerid);
	
	switch (gBadNetAction)
	{
		case 0:
		{
			if (Player[playerid][FPSKick] >= MAX_LOWFPS_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB (Netcheck) "COL_SEC"%s " COL_PRIM "considering removing or netcheck. (Reason: "COL_SEC"Low FPS %d/%d"COL_PRIM")", Player[playerid][Name], pFPS, Min_FPS);
			}
			
			if (Player[playerid][HighFPSKick] >= MAX_HIGHFPS_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB (Netcheck) "COL_SEC"%s " COL_PRIM "considering removing or netcheck. (Reason: "COL_SEC"High FPS %d/%d"COL_PRIM")", Player[playerid][Name], pFPS, Max_FPS);
			}
			
			if (Player[playerid][PacketKick] >= MAX_PACKETLOSS_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB (Netcheck) "COL_SEC"%s " COL_PRIM "considering removing or netcheck. (Reason: "COL_SEC"High Packet Loss %.2f/%.2f"COL_PRIM")", Player[playerid][Name], pPL, Max_Packetloss);
			}
			if (Player[playerid][PingKick] >= MAX_HIGHPING_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB (Netcheck) "COL_SEC"%s " COL_PRIM "considering removing or netcheck. (Reason: "COL_SEC"High Ping %d/%d"COL_PRIM")", Player[playerid][Name], pPing, Max_Ping);
			}
		}
		case 1:
		{
			if (Player[playerid][FPSKick] >= MAX_LOWFPS_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has been removed from the round. (Reason: "COL_SEC"Low FPS %d/%d"COL_PRIM")", Player[playerid][Name], pFPS, Min_FPS);
			}
			
			if (Player[playerid][HighFPSKick] >= MAX_HIGHFPS_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has been removed from the round. (Reason: "COL_SEC"High FPS %d/%d"COL_PRIM")", Player[playerid][Name], pFPS, Max_FPS);
			}
			
			if (Player[playerid][PacketKick] >= MAX_PACKETLOSS_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has been removed from the round. (Reason: "COL_SEC"High Packet Loss %.2f/%.2f"COL_PRIM")", Player[playerid][Name], pPL, Max_Packetloss);
			}
			if (Player[playerid][PingKick] >= MAX_HIGHPING_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has been removed from the round. (Reason: "COL_SEC"High Ping %d/%d"COL_PRIM")", Player[playerid][Name], pPing, Max_Ping);
			}

			RemovePlayerFromRound(playerid);
		}
		case 2:
		{
			if (Player[playerid][FPSKick] >= MAX_LOWFPS_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has been kicked. (Reason: "COL_SEC"Low FPS %d/%d"COL_PRIM")", Player[playerid][Name], pFPS, Min_FPS);
			}
			
			if (Player[playerid][HighFPSKick] >= MAX_HIGHFPS_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has been kicked. (Reason: "COL_SEC"High FPS %d/%d"COL_PRIM")", Player[playerid][Name], pFPS, Max_FPS);
			}
			
			if (Player[playerid][PacketKick] >= MAX_PACKETLOSS_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has been kicked. (Reason: "COL_SEC"High Packet Loss %.2f/%.2f"COL_PRIM")", Player[playerid][Name], pPL, Max_Packetloss);
			}
			if (Player[playerid][PingKick] >= MAX_HIGHPING_WARNINGS)
			{
				va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has been kicked. (Reason: "COL_SEC"High Ping %d/%d"COL_PRIM")", Player[playerid][Name], pPing, Max_Ping);
			}
			SchedulePlayerKick(playerid, 350);
		}
		case 3:
		{
			if(!RoundPaused)
			{
				if (Player[playerid][FPSKick] >= MAX_LOWFPS_WARNINGS)
				{
					va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has caused the round to be paused. (Reason: "COL_SEC"Low FPS %d/%d"COL_PRIM")", Player[playerid][Name], pFPS, Min_FPS);
				}
			
				if (Player[playerid][HighFPSKick] >= MAX_HIGHFPS_WARNINGS)
				{
					va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has caused the round to be paused. (Reason: "COL_SEC"High FPS %d/%d"COL_PRIM")", Player[playerid][Name], pFPS, Max_FPS);
				}
			
				if (Player[playerid][PacketKick] >= MAX_PACKETLOSS_WARNINGS)
				{
					va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has caused the round to be paused. (Reason: "COL_SEC"High Packet Loss %.2f/%.2f"COL_PRIM")", Player[playerid][Name], pPL, Max_Packetloss);
				}
				if (Player[playerid][PingKick] >= MAX_HIGHPING_WARNINGS)
				{
					va_SendClientMessageToAll(-1, ""COL_PRIM"\xBB "COL_SEC"%s " COL_PRIM "has caused the round to be paused. (Reason: "COL_SEC"High Ping %d/%d"COL_PRIM")", Player[playerid][Name], pPing, Max_Ping);
				}
				PauseRound(); // Function to pause the round
			}
		}
	}
}

forward CheckWinCondition(); 

CheckPlayerNet(playerid)
{
	new pFPS = GetPlayerFPS(playerid);
	new Float: pPL = NetStats_PacketLossPercent(playerid);
	new pPing = GetPlayerPing(playerid);
	if (pFPS < Min_FPS && Player[playerid][PauseCount] < 5 && Player[playerid][FPSCheck])
	{
		if (Player[playerid][FPSKick] < MAX_LOWFPS_WARNINGS)
		{
			Player[playerid][FPSKick]++;
			va_SendClientMessage(playerid, -1, "{EB8989}\xBB Netcheck: {FFFFFF}Your FPS is too low {EB8989}(%d/%d) {FFFFFF}[%d/" #MAX_LOWFPS_WARNINGS "] - use {BD93DC}/netcheckme", pFPS, Min_FPS, Player[playerid][FPSKick]);

			if (Player[playerid][FPSKick] == MAX_LOWFPS_WARNINGS)
			{
				HandleNetworkIssue(playerid);
			}
		}
	}

	if (pFPS > Max_FPS && Player[playerid][FPSCheck])
	{
		if (Player[playerid][HighFPSKick] < MAX_HIGHFPS_WARNINGS)
		{
			Player[playerid][HighFPSKick]++;
			va_SendClientMessage(playerid, -1, "{EB8989}\xBB Netcheck: {FFFFFF}Your FPS is too high {EB8989}(%d/%d) {FFFFFF}[%d/" #MAX_HIGHFPS_WARNINGS "] - use {BD93DC}/netcheckme", pFPS, Max_FPS, Player[playerid][HighFPSKick]);

			if (Player[playerid][HighFPSKick] == MAX_HIGHFPS_WARNINGS)
			{
				HandleNetworkIssue(playerid);
			}
		}
	}

	if (pPL >= Max_Packetloss && !Player[playerid][FakePacketRenovation] && Player[playerid][PLCheck])
	{
		if (Player[playerid][PacketKick] < MAX_PACKETLOSS_WARNINGS)
		{
			Player[playerid][PacketKick]++;
			va_SendClientMessage(playerid, -1, "{EB8989}\xBB Netcheck: {FFFFFF}Your packet loss is too high {EB8989}(%.1f%%/%.1f%%) {FFFFFF}[%d/" #MAX_PACKETLOSS_WARNINGS "] - use {BD93DC}/netcheckme", pPL, Max_Packetloss, Player[playerid][PacketKick]);

			if (Player[playerid][PacketKick] == MAX_PACKETLOSS_WARNINGS)
			{
				HandleNetworkIssue(playerid);
			}
		}
	}

	if (pPing >= Max_Ping && Player[playerid][PingCheck])
	{
		if (Player[playerid][PingKick] < MAX_HIGHPING_WARNINGS)
		{
			Player[playerid][PingKick]++;
			va_SendClientMessage(playerid, -1, "{EB8989}\xBB Netcheck: {FFFFFF}Your ping is too high {EB8989}(%d/%d ms) {FFFFFF}[%d/" #MAX_HIGHPING_WARNINGS "] - use {BD93DC}/netcheckme", pPing, Max_Ping, Player[playerid][PingKick]);

			if (Player[playerid][PingKick] == MAX_HIGHPING_WARNINGS)
			{
				HandleNetworkIssue(playerid);
			}
		}
	}
}


public AimbotCheck(playerid)
{
	new weaponid = 0, ammo = 0;
	GetPlayerWeaponData(playerid, 0, weaponid, ammo);
	if (ammo == 1000)
	{
		va_SendClientMessageToAll(-1, COL_PRIM "\xBB System has banned "COL_SEC"%s "COL_PRIM"(Reason: "COL_SEC"Aimbot"COL_PRIM")", Player[playerid][Name]);
		BanEx(playerid, "(SYSTEM BAN) Aimbot");
		return 1;
	}
	return 0;
}

