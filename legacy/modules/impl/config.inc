#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_config
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_config
	#endinput
#endif
#define _attdef_impl_config

/*
	Gamemode configuration
*/

// Functions
// Functions
SetStoredConfigBooleanValue(playerid, const option[], &bool: value, bool: newValue)
{
	if (!DatabaseExecuteWrite(
		va_return("INSERT OR REPLACE INTO Configs (Option, Value) VALUES ('%q', '%d')", option, newValue),
		"save server configuration"))
		return SendErrorMessage(playerid, "Could not save this configuration option. No changes were applied.");

	value = newValue;
	return 1;
}

ToggleStoredConfigValue(playerid, const option[], &bool: value)
{
	return SetStoredConfigBooleanValue(playerid, option, value, !value);
}

SetPlayerPauseMode(playerid, PauseMode: mode)
{
	new description[24];
	switch (mode)
	{
		case PAUSE_MODE_MESSAGE: format(description, sizeof(description), "Send Message");
		case PAUSE_MODE_PAUSE_ROUND: format(description, sizeof(description), "Pause Round");
		case PAUSE_MODE_NONE: format(description, sizeof(description), "Do Nothing");
		default: return SendErrorMessage(playerid, "Invalid pause detection mode.");
	}

	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %d WHERE Option = 'PauseMode'", _:mode),
		"save pause detection mode"))
		return SendErrorMessage(playerid, "Could not save the pause detection mode. No changes were applied.");

	gPauseMode = mode;
	va_SendClientMessageToAll(
		-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has set player pause detection to"COL_SEC" \"%s\"",
		PLAYER_ROLE_NAME(playerid), Player[playerid][Name], description);
	return 1;
}

SetAutoPauseMode(playerid, mode)
{
	new description[24];
	switch (mode)
	{
		case 0: format(description, sizeof(description), "Do Nothing");
		case 1: format(description, sizeof(description), "Send Message");
		case 2: format(description, sizeof(description), "Pause Round");
		default: return SendErrorMessage(playerid, "Invalid auto-pause mode.");
	}

	gAutoPauseMode = mode;

	if (sqliteconnection)
	{
		DatabaseExecuteWrite(
			va_return("INSERT OR REPLACE INTO Configs (Option, Value) VALUES ('AutoPause', '%d')", mode),
			"save auto-pause mode");
	}

	va_SendClientMessageToAll(
		-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has set player disconnect action to"COL_SEC" \"%s\"",
		PLAYER_ROLE_NAME(playerid), Player[playerid][Name], description);
	return 1;
}

SetPlayerNetMode(playerid, action)
{
	new description[32];
	switch (action)
	{
		case 0: format(description, sizeof(description), "Send Message");
		case 1: format(description, sizeof(description), "Remove from Round");
		case 2: format(description, sizeof(description), "Kick from the server");
		case 3: format(description, sizeof(description), "Pause Round");
		default: return SendErrorMessage(playerid, "Invalid netcheck action.");
	}

	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %d WHERE Option = 'NetMode'", action),
		"save netcheck action"))
		return SendErrorMessage(playerid, "Could not save the netcheck action. No changes were applied.");

	gBadNetAction = action;
	va_SendClientMessageToAll(
		-1, COL_PRIM "%s " COL_SEC "%s " COL_PRIM "has set exceeding netcheck limit to " COL_SEC "\"%s\"",
		PLAYER_ROLE_NAME(playerid), Player[playerid][Name], description);
	return 1;
}

PersistTeamSkin(team, skinid)
{
	new option[32];
	switch (team)
	{
		case ATTACKER: format(option, sizeof(option), "Attacker Skin");
		case DEFENDER: format(option, sizeof(option), "Defender Skin");
		case REFEREE: format(option, sizeof(option), "Referee Skin");
		case ATTACKER_SUB: format(option, sizeof(option), "Attacker Sub Skin");
		case DEFENDER_SUB: format(option, sizeof(option), "Defender Sub Skin");
		default: return 0;
	}

	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %d WHERE Option = '%q'", skinid, option),
		"save team skin"))
		return 0;

	Skin[team] = skinid;
	return 1;
}

SetDefaultTeamSkins()
{
	if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin default team skin update"))
		return 0;

	if (!DatabaseExecuteWrite("UPDATE Configs SET Value = 170 WHERE Option = 'Attacker Skin'", "save default attacker skin")
		|| !DatabaseExecuteWrite("UPDATE Configs SET Value = 45 WHERE Option = 'Attacker Sub Skin'", "save default attacker substitute skin")
		|| !DatabaseExecuteWrite("UPDATE Configs SET Value = 176 WHERE Option = 'Defender Skin'", "save default defender skin")
		|| !DatabaseExecuteWrite("UPDATE Configs SET Value = 154 WHERE Option = 'Defender Sub Skin'", "save default defender substitute skin")
		|| !DatabaseExecuteWrite("UPDATE Configs SET Value = 155 WHERE Option = 'Referee Skin'", "save default referee skin"))
	{
		DatabaseExecuteWrite("ROLLBACK", "rollback default team skin update");
		return 0;
	}

	if (!DatabaseExecuteWrite("COMMIT", "commit default team skin update"))
	{
		DatabaseExecuteWrite("ROLLBACK", "rollback default team skin update");
		return 0;
	}

	Skin[ATTACKER] = 170;
	Skin[ATTACKER_SUB] = 45;
	Skin[DEFENDER] = 176;
	Skin[DEFENDER_SUB] = 154;
	Skin[REFEREE] = 155;
	return 1;
}

ShowAntiCheatConfigDialog(playerid1)
{
	inline InlineAntiCheatConfig(playerid, dialogid, response, listitem, string: inputtext[])
	{
		#pragma unused dialogid, inputtext
		if (!response)
		{
			ShowConfigDialog(playerid);
			return 1;
		}
		switch (listitem)
		{
			case 0:
			{
				if (!ToggleStoredConfigValue(playerid, "UseCleo", UseCleo))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Use of Cleo "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], UseCleo ? "{AADC93}allowed" : "{EB8989}disallowed");
				ShowAntiCheatConfigDialog(playerid);
				if (!UseCleo)
				{
					for (new i = 0; i < MAX_PLAYERS; i++)
					{
						if (IsPlayerConnected(i) && (Player[i][UsingCleo] || Player[i][CleoPStatus]))
						{
							UpdateModDetectionMessage(i);
							Discord_SendAntiCheatAlert(i, "CLEO Modification", "Kicked when CLEO usage was disallowed in server config.");
							SendClientMessage(i, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"Cleo.asi "COL_PRIM"from your GTA folder.");
							SchedulePlayerKick(i, 500);
						}
					}
				}
			}
			case 1:
			{
				if (!ToggleStoredConfigValue(playerid, "UseSampfuncs", UseSampfuncs))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Use of Sampfuncs "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], UseSampfuncs ? "{AADC93}allowed" : "{EB8989}disallowed");
				ShowAntiCheatConfigDialog(playerid);
				if (!UseSampfuncs)
				{
					for (new i = 0; i < MAX_PLAYERS; i++)
					{
						if (IsPlayerConnected(i) && (Player[i][UsingSampfuncs] || Player[i][SampfuncsPStatus]))
						{
							UpdateModDetectionMessage(i);
							Discord_SendAntiCheatAlert(i, "SAMPFUNCS.asi", "Kicked when SAMPFUNCS usage was disallowed in server config.");
							SendClientMessage(i, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"SAMPFUNCS.asi "COL_PRIM"from your GTA folder.");
							SchedulePlayerKick(i, 500);
						}
					}
				}
			}
			case 2:
			{
				if (!ToggleStoredConfigValue(playerid, "UseExtraws", UseExtraws))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Use of Extraws "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], UseExtraws ? "{AADC93}allowed" : "{EB8989}disallowed");
				ShowAntiCheatConfigDialog(playerid);
				if (!UseExtraws)
				{
					for (new i = 0; i < MAX_PLAYERS; i++)
					{
						if (IsPlayerConnected(i) && (Player[i][UsingExtraws] || Player[i][ExtrawsPStatus]))
						{
							UpdateModDetectionMessage(i);
							Discord_SendAntiCheatAlert(i, "ExtraWS.asi", "Kicked when ExtraWS usage was disallowed in server config.");
							SendClientMessage(i, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"ExtraWS.asi "COL_PRIM"from your GTA folder.");
							SchedulePlayerKick(i, 500);
						}
					}
				}
			}
			case 3:
			{
				if (!ToggleStoredConfigValue(playerid, "UseSprinthook", UseSprinthook))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Use of SprintHook "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], UseSprinthook ? "{AADC93}allowed" : "{EB8989}disallowed");
				ShowAntiCheatConfigDialog(playerid);
				if (!UseSprinthook)
				{
					for (new i = 0; i < MAX_PLAYERS; i++)
					{
						if (IsPlayerConnected(i) && (Player[i][UsingSprintHook] || Player[i][SprintHookPStatus]))
						{
							UpdateModDetectionMessage(i);
							Discord_SendAntiCheatAlert(i, "SprintHook.asi", "Kicked when SprintHook usage was disallowed in server config.");
							SendClientMessage(i, COL_PRIM_HEX, "\xBB Please remove "COL_SEC"SprintHook.asi "COL_PRIM"from your GTA folder.");
							SchedulePlayerKick(i, 500);
						}
					}
				}
			}
			case 4:
			{
				if (!ToggleStoredConfigValue(playerid, "AntiMacros", AntiMacros))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Anti-macros system "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], AntiMacros ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowAntiCheatConfigDialog(playerid);
			}
			case 5:
			{
				if (!ToggleStoredConfigValue(playerid, "AntiCBug", AntiCBug))
					return 1;
				va_SendClientMessageToAll(
					-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Anti-C-bug "COL_PRIM"option as %s",
					PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], AntiCBug ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowAntiCheatConfigDialog(playerid);
			}
			case 6:
			{
				if (!ToggleStoredConfigValue(playerid, "AntiSlideBug", AntiSlideBug))
					return 1;
				va_SendClientMessageToAll(
					-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Anti-slidebug "COL_PRIM"option as %s",
					PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], AntiSlideBug ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowAntiCheatConfigDialog(playerid);
			}
			case 7:
			{
				if (!ToggleStoredConfigValue(playerid, "SyncAbuse", SyncAbuse))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Sync abuse "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], SyncAbuse ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowAntiCheatConfigDialog(playerid);
			}
		}
		return 1;
	}

	new string[1024];
	strcat(string, COL_PRIM "Option\t"COL_PRIM"Status\t"COL_PRIM"Command\n");
	strcat(string, "Cleo Usage\t");
	strcat(string, UseCleo ? "{AADC93}Allowed\t" : "{EB8989}Disallowed\t");
	strcat(string, "/cleousage\n");
	strcat(string, "Sampfuncs Usage\t");
	strcat(string, UseSampfuncs ? "{AADC93}Allowed\t" : "{EB8989}Disallowed\t");
	strcat(string, "/sampfuncsusage\n");
	strcat(string, "ExtraWS Usage\t");
	strcat(string, UseExtraws ? "{AADC93}Allowed\t" : "{EB8989}Disallowed\t");
	strcat(string, "/extrawsusage\n");
	strcat(string, "SprintHook Usage\t");
	strcat(string, UseSprinthook ? "{AADC93}Allowed\t" : "{EB8989}Disallowed\t");
	strcat(string, "/sprinthookusage\n");
	strcat(string, "Anti-Macros\t");
	strcat(string, AntiMacros ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/antimacros\n");
	strcat(string, "Anti-C-Bug\t");
	strcat(string, AntiCBug ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/anticbug\n");
	strcat(string, "Anti-Slidebug\t");
	strcat(string, AntiSlideBug ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "-\n");
	strcat(string, "Sync Abuse\t");
	strcat(string, SyncAbuse ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/syncabuse\n");

	Dialog_ShowCallback(playerid1, using inline InlineAntiCheatConfig, DIALOG_STYLE_TABLIST_HEADERS, "Anti-Cheat & Mods Configuration", string, "Toggle", "Back");
	return 1;
}

ShowConfigDialog(playerid1)
{


	inline InlineConfig(playerid, dialogid, response, listitem, string: inputtext[])
	{
		#pragma unused dialogid, inputtext
		if (!response)
			return 1;
		switch (listitem)
		{
			case 0: ShowPlayerDialog(playerid, DIALOG_ATT_NAME, DIALOG_STYLE_INPUT, "Attacker Team Name", "Enter Attacker " "Team Name Below:", "Next", "Close");
			case 1:
			{
				new iString[256];
				format(iString, sizeof(iString), "%sAttacker\n%sDefender\n%sReferee\n%sAttacker Sub\n%sDefender Sub",
					TextColor[ATTACKER], TextColor[DEFENDER], TextColor[REFEREE], TextColor[ATTACKER_SUB], TextColor[DEFENDER_SUB]);
				ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_TEAM_SKIN, DIALOG_STYLE_LIST, "Select Team", iString, "OK", "Back");
			}
			case 2: ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_AAD, DIALOG_STYLE_LIST, "Main Config", "Health\nArmour\nRound Time\nCP Time", "OK", "Back");
			case 3: ShowAntiCheatConfigDialog(playerid);
			case 4: SendRconCommand("gmx");
			case 5: ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_MAX_PING, DIALOG_STYLE_INPUT, "Maximun Ping", "Set the max ping:", "OK", "Back");
			case 6: ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_MAX_PACKET, DIALOG_STYLE_INPUT, "Maximun Packet", "Set the max packetloss:", "OK", "Back");
			case 7: ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_MIN_FPS, DIALOG_STYLE_INPUT, "Minimum FPS", "Set the minimum FPS:", "OK", "Back");
			case 8: ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_MAX_FPS, DIALOG_STYLE_INPUT, "Maximun FPS", "Set the maximun FPS:", "OK", "Back");
			case 9:
			{
				new string[144];
				strcat(string, strlen(GroupAccessPassword[0]) > 0 ? "{EB8989}" : "{AADC93}");
				strcat(string, TeamName[ATTACKER]);
				strcat(string, strlen(GroupAccessPassword[1]) > 0 ? "\n{EB8989}" : "\n{AADC93}");
				strcat(string, TeamName[DEFENDER]);
				strcat(string, strlen(GroupAccessPassword[2]) > 0 ? "\n{EB8989}Referee" : "\n{AADC93}Referee");
				strcat(string, strlen(GroupAccessPassword[3]) > 0 ? "\n{EB8989}" : "\n{AADC93}");
				strcat(string, TeamName[ATTACKER]);
				strcat(string, " Sub");
				strcat(string, strlen(GroupAccessPassword[4]) > 0 ? "\n{EB8989}" : "\n{AADC93}");
				strcat(string, TeamName[DEFENDER]);
				strcat(string, " Sub");
				ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_GA, DIALOG_STYLE_LIST, "Config Settings", string, "OK", "Cancel");
				return 1;
			}
			case 10:
			{
				if (!ServerLocked)
				{
					ShowPlayerDialog(playerid, DIALOG_SERVER_PASS, DIALOG_STYLE_INPUT, "Server Password", "Enter server password below:", "Ok", "Close");
				}
				else
				{
					SendRconCommand("password 0");
					ServerLocked = false;
					PermLocked = false;
				}
			}
			case 11:
			{
				AntiSpam = !AntiSpam;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"anti-spam "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], AntiSpam ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 12:
			{
				AutoBal = !AutoBal;
				va_SendClientMessageToAll(-1, "%s "COL_SEC"%s "COL_PRIM "has set "COL_SEC"auto-balance in non match mode "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], AutoBal ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 13:
			{
				ShowPlayerDialog(playerid, DIALOG_CONFIG_AUTOPAUSE, DIALOG_STYLE_LIST, COL_PRIM "Player Disconnect Action", "Do Nothing\nSend Message\nPause Round", "Select", "Back");
			}
			case 14:
			{
				LobbyGuns = !LobbyGuns;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"guns in lobby "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], LobbyGuns ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 15:
			{
				if (Current != -1)
					return SendErrorMessage(playerid, "Can't use this while a round is in progress.");
				if (!ToggleStoredConfigValue(playerid, "CPInArena", CPInArena))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Checkpoint in arena "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], CPInArena ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 16:
			{
				if (Current != -1)
					return SendErrorMessage(playerid, "Can't use this while a round is in progress.");

				new deagleExists = false;
				for (new idx = 0; idx < MAX_GUNMENU_GUNS; idx++)
				{
					if (GunmenuData[idx][GunID] == WEAPON_DEAGLE)
					{
						deagleExists = true;
						break;
					}
				}

				if (!deagleExists)
				{
					SendErrorMessage(playerid, "Deagle is not available in the Gunmenu.");
					return 1;
				}

				new bool: enabled = !HalfDeagle;
				if (!SetHalfDeagleMode(enabled))
					return SendErrorMessage(playerid, "Could not save Half Deagle mode. No changes were applied.");

				va_SendClientMessageToAll(
					-1,
					""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"Half Deagle Mode",
					PLAYER_ROLE_NAME(playerid1),
					Player[playerid][Name],
					HalfDeagle ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 17:
			{
				if (!ToggleStoredConfigValue(playerid, "DeadBodies", DeadBodies))
					return 1;
				if (!DeadBodies)
					DeleteAllDeadBodies();
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Dead bodies "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], DeadBodies ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 18:
			{
				if (Current != -1)
					return SendErrorMessage(playerid, "Can't use this while a round is in progress.");
				if (!ToggleStoredConfigValue(playerid, "Aircraft", Aircraft))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has "COL_SEC"%s "COL_PRIM"Aircraft Spawning during rounds.", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], Aircraft ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 19:
			{
				if (Current != -1)
					return SendErrorMessage(playerid, "Can't do this while a round is in progress.");
				if (!ToggleStoredConfigValue(playerid, "ShowHPBars", ShowHPBars))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"HP Bars system "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], ShowHPBars ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 20:
			{
				if (!ToggleStoredConfigValue(playerid, "GunmenuRestrictions", GunmenuRestrictions))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Gunmenu Restrictions "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], GunmenuRestrictions ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 21:
			{
				if (!ToggleStoredConfigValue(playerid, "MeleeAllowed", MeleeAllowed))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Melee Weapons "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], MeleeAllowed ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 22:
			{
				if (!ToggleStoredConfigValue(playerid, "AutoRoundStarter", AutoRoundStarter))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Auto round starter "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], AutoRoundStarter ? "{AADC93}enabled" : "{EB8989}disabled");
				if (AutoRoundStarter)
				{
					if (Current == -1)
						SetRoundAutoStart(10);
				}
				else
				{
					CancelAutoRoundStart();
				}
				ShowConfigDialog(playerid);
			}
			case 23:
			{
				if (!ToggleStoredConfigValue(playerid, "RDeathMsg", RandomDeathMessages))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Random death messages "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], RandomDeathMessages ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 24:
			{
				if (!ToggleStoredConfigValue(playerid, "SightseeingInCS", SightseeingInClassSelection))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Sightseeing in class selection "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], SightseeingInClassSelection ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 25:
			{
				if (!ToggleStoredConfigValue(playerid, "VehicleHealthTD", VehicleHealthTextdraw))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Vehicle health info "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], VehicleHealthTextdraw ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 26:
			{
				if (!ToggleStoredConfigValue(playerid, "ScreenFlash", ScreenFlashing))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Screen flashing system "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], ScreenFlashing ? "{AADC93}enabled" : "{EB8989}disabled");
				ShowConfigDialog(playerid);
			}
			case 27:
			{
				if (!ToggleStoredConfigValue(playerid, "DefVehicles", DefendersWithVehicles))
					return 1;
				va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has set "COL_SEC"Defenders with vehicles "COL_PRIM"option as %s", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], DefendersWithVehicles ? "{AADC93}allowed" : "{EB8989}disallowed");
				ShowConfigDialog(playerid);
			}
			case 28: ShowPlayerDialog(playerid, DIALOG_CONFIG_NETMODE, DIALOG_STYLE_LIST, COL_PRIM "Netmode Settings", "Send Message\nRemove from Round\nKick from the server\nPause Round", "Select", "Back");
			case 29: ShowPlayerDialog(playerid, DIALOG_CONFIG_PAUSEMODE, DIALOG_STYLE_LIST, COL_PRIM "Pause Mode", "Send Message\nPause Round\nDo Nothing", "Select", "Back");
			case 30: ShowPlayerDialog(playerid, DIALOG_CONFIG_ADDMETIME, DIALOG_STYLE_INPUT, "Set Addme Time", "Set the time in seconds:", "OK", "Back");
			case 31: ShowPlayerDialog(playerid, DIALOG_CONFIG_GUNMENUTIME, DIALOG_STYLE_INPUT, "Set Gunmenu Time", "Set the time in seconds:", "OK", "Back");
			case 32:
			{
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/autoend");
				ShowConfigDialog(playerid);
			}
			case 33:
			{
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/noswitch");
				ShowConfigDialog(playerid);
			}
			case 34:
			{
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/vlobby");
				ShowConfigDialog(playerid);
			}
			case 35:
			{
				if (ShouldNerfDeagle)
				{
					CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/deaglenerf off");
					ShowConfigDialog(playerid);
				}
				else
				{
					ShowPlayerDialog(playerid, DIALOG_CONFIG_DEAGLE_NERF, DIALOG_STYLE_INPUT, "Deagle Nerf Damage", "Enter custom damage for Deagle (Default: 35.0, Min: 0.5, Max: 46.2):", "Set", "Cancel");
				}
			}
			case 36:
			{
				if (ShouldNerfSniper)
				{
					CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/snipernerf off");
					ShowConfigDialog(playerid);
				}
				else
				{
					ShowPlayerDialog(playerid, DIALOG_CONFIG_SNIPER_NERF, DIALOG_STYLE_INPUT, "Sniper Nerf Damage", "Enter custom damage for Sniper (Default: 25.0, Min: 0.5, Max: 41.25):", "Set", "Cancel");
				}
			}
			case 37:
			{
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/togglearmour");
				ShowConfigDialog(playerid);
			}
			case 38:
			{
				ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_ARMOUR_PING, DIALOG_STYLE_INPUT, "Armour Object Minimum Ping", "Set the minimum ping required to display the armour object:", "Set", "Back");
			}
			case 39:
			{
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/skinlabels");
			}
			case 40:
			{
				ShowBaseSetsDialog(playerid);
			}
			case 41:
			{
				ShowArenaSetsDialog(playerid);
			}
			case 42:
			{
				ToggleFloatingDamage(playerid);
				ShowConfigDialog(playerid);
			}
		}
		return 1;
	}

	new string[4800];
	new header[228];
	// Format the header properly
	format(header, sizeof(header), "Server Configuration");

	strcat(string, COL_PRIM "Option\t"COL_PRIM"Value\t"COL_PRIM"Command\n");
	strcat(string, COL_PRIM "> Team Names\t{FFFFFF}-\t/teamname\n");
	strcat(string, COL_PRIM "> Team Skins\t{FFFFFF}-\t/teamskin\n");
	strcat(string, COL_PRIM "> AD Settings\t{FFFFFF}-\t-\n");
	strcat(string, COL_PRIM "> Anti-Cheat & Mods\t{FFFFFF}Configure\t-\n");
	strcat(string, COL_PRIM "> Restart Server\t{FFFFFF}-\t/gmx\n");

	new tmp[256];

	format(tmp, sizeof(tmp), "Maximun Ping\t%d\t/maxping\n", Max_Ping);
	strcat(string, tmp);

	format(tmp, sizeof(tmp), "Maximun Packet\t%.2f\t/maxpacket\n", Float:Max_Packetloss);
	strcat(string, tmp);

	format(tmp, sizeof(tmp), "Minimun FPS\t%d\t/minfps\n", Min_FPS);
	strcat(string, tmp);

	format(tmp, sizeof(tmp), "Maximun FPS\t%d\t/maxfps\n", Max_FPS);
	strcat(string, tmp);

	new bool: isrestricted;
	for (new i = 0; i < sizeof(GroupAccessPassword); i++)
	{
		if (strlen(GroupAccessPassword[i]) == 0)
			continue;
		isrestricted = true;
		break;
	}
	strcat(string, "Group Access\t");
	strcat(string, isrestricted ? "{EB8989}Restricted\n" : "{AADC93}Unrestricted\n");
	strcat(string, "Server Password\t");
	strcat(string, ServerLocked ? "{EB8989}Locked\t" : "{AADC93}Unlocked\t");
	strcat(string, "/serverpassword\n");
	strcat(string, "Anti-Spam\t");
	strcat(string, AntiSpam ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/antispam\n");
	strcat(string, "Auto-Balance\t");
	strcat(string, AutoBal ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/autobalance\n");
	strcat(string, "Auto-Pause\t");
	strcat(string, (gAutoPauseMode == 1) ? "Send Message\t" : (gAutoPauseMode == 2) ? "Pause Round\t" : "Do Nothing\t");
	strcat(string, "/autopause\n");
	strcat(string, "Lobby Guns\t");
	strcat(string, LobbyGuns ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/lobbyguns\n");
	strcat(string, "CP in Arena\t");
	strcat(string, CPInArena ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/cparena\n");
	strcat(string, "Half Deagle Mode\t");
	strcat(string, HalfDeagle ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/halfdeagle\n");
	strcat(string, "Dead Bodies\t");
	strcat(string, DeadBodies ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/deadbodies\n");
	strcat(string, "Aircraft Spawning\t");
	strcat(string, Aircraft ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/aircraft\n");
	strcat(string, "Show HP Bars\t");
	strcat(string, ShowHPBars ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/hpbars\n");
	strcat(string, "Gunmenu Restrictions\t");
	strcat(string, GunmenuRestrictions ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "-\n");
	strcat(string, "Melee Allowed\t");
	strcat(string, MeleeAllowed ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "-\n");
	strcat(string, "Auto Round Starter\t");
	strcat(string, AutoRoundStarter ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "-\n");
	strcat(string, "Random Death Messages\t");
	strcat(string, RandomDeathMessages ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "-\n");
	strcat(string, "Sightseeing In Class\t");
	strcat(string, SightseeingInClassSelection ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "-\n");
	strcat(string, "Vehicle Health 3D TextLabel\t");
	strcat(string, VehicleHealthTextdraw ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "-\n");
	strcat(string, "Screen Flashing\t");
	strcat(string, ScreenFlashing ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "-\n");
	strcat(string, "Defenders with Vehicles\t");
	strcat(string, DefendersWithVehicles ? "{AADC93}Allowed\t" : "{EB8989}Disallowed\t");
	strcat(string, "/defendersvehicles\n");
	strcat(string, "Player Netcheck Fail\t");
	strcat(string, (gBadNetAction == 0) ? "Send Message\t" : (gBadNetAction == 1) ? "Remove From Round\t" : (gBadNetAction == 2) ? "Kick Player\t" : (gBadNetAction == 3) ? "Pause Round\t" : "Unknown Action\t");
	strcat(string, "/netmode\n");
	strcat(string, "Player Pause\t");
	strcat(string, (gPauseMode == PAUSE_MODE_MESSAGE) ? "Send Message\t" : (gPauseMode == PAUSE_MODE_PAUSE_ROUND) ? "Pause Round\t" : (gPauseMode == PAUSE_MODE_NONE) ? "Do Nothing\t" : "Unknown Action\t");
	strcat(string, "/pausemode\n");

	format(tmp, sizeof(tmp), "Addme Time\t%ds\t/addmetime\n", gAddmeTime);
	strcat(string, tmp);
	format(tmp, sizeof(tmp), "Gunmenu Time\t%ds\t/gunmenutime\n", gGunmenuTime);
	strcat(string, tmp);
	strcat(string, "Auto-End Match\t");
	strcat(string, AutoEnd ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/autoend\n");
	strcat(string, "No-Switch in Match\t");
	strcat(string, NoSwitch ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/noswitch\n");
	strcat(string, "Lobby Vehicles\t");
	strcat(string, LobbyVehicles ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/vlobby\n");
	strcat(string, "Deagle Nerf\t");
	if (ShouldNerfDeagle)
	{
		format(tmp, sizeof(tmp), "{AADC93}%.1f DMG\t/deaglenerf\n", DeagleNerfValue);
		strcat(string, tmp);
	}
	else
	{
		strcat(string, "{EB8989}Disabled\t/deaglenerf\n");
	}
	strcat(string, "Sniper Nerf\t");
	if (ShouldNerfSniper)
	{
		format(tmp, sizeof(tmp), "{AADC93}%.1f DMG\t/snipernerf\n", SniperNerfValue);
		strcat(string, tmp);
	}
	else
	{
		strcat(string, "{EB8989}Disabled\t/snipernerf\n");
	}
	strcat(string, "Armour Object Ping\t");
	strcat(string, ArmourToggle ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/togglearmour\n");
	format(tmp, sizeof(tmp), "Armour Min Ping\t%d ms\t/setarmourping\n", ArmourPingMin);
	strcat(string, tmp);
	strcat(string, "Skin Labels\t{FFFFFF}Configure\t/skinlabels\n");
	format(tmp, sizeof(tmp), "Base Pack\t{FFFFFF}%s\t/loadbases\n", CurrentBaseSet);
	strcat(string, tmp);
	format(tmp, sizeof(tmp), "Arena Pack\t{FFFFFF}%s\t/loadarenas\n", CurrentArenaSet);
	strcat(string, tmp);
	strcat(string, "Floating Dmg\t");
	strcat(string, FloatingDamage ? "{AADC93}Enabled\t" : "{EB8989}Disabled\t");
	strcat(string, "/dmg\n");

	Dialog_ShowCallback(playerid1, using inline InlineConfig, DIALOG_STYLE_TABLIST_HEADERS, header, string, "OK", "Cancel");
	return 1;
}




#define REQUIRED_CONFIG_OPTION_COUNT 45

new const RequiredConfigOptions[REQUIRED_CONFIG_OPTION_COUNT][32] =
{
	"Attacker Skin",
	"Attacker Sub Skin",
	"Defender Skin",
	"Defender Sub Skin",
	"Referee Skin",
	"Server Weather",
	"Server Time",
	"CP Time",
	"Round Time",
	"Total Rounds",
	"Maximum Packetloss",
	"Maximum Ping",
	"Minimum FPS",
	"Maximum FPS",
	"Main Spawn",
	"RoundHPAR",
	"ChatColor",
	"CPInArena",
	"HalfDeagle",
	"AntiMacros",
	"DeadBodies",
	"Aircraft",
	"ShowHPBars",
	"MeleeAllowed",
	"CurrentBaseSet",
	"AutoRoundStarter",
	"GameSpeed",
	"RDeathMsg",
	"SightseeingInCS",
	"VehicleHealthTD",
	"DefVehicles",
	"SyncAbuse",
	"ScreenFlash",
	"Server Name",
	"UseCleo",
	"UseSampfuncs",
	"UseExtraws",
	"UseSprinthook",
	"CurrentArenaSet",
	"NetMode",
	"PauseMode",
	"AddmeTime",
	"GunmenuTime",
	"AntiCBug",
	"AntiSlideBug"
};

stock FindRequiredConfigOption(const option[])
{
	for (new index = 0; index < REQUIRED_CONFIG_OPTION_COUNT; index++)
		if (!strcmp(option, RequiredConfigOptions[index], true))
			return index;
	return -1;
}

stock bool: ValidateRequiredConfigValue(const option[], const value[])
{
	if (!strcmp(option, "Main Spawn", true))
	{
		new Float: x, Float: y, Float: z, Float: angle, interior;
		if (sscanf(value, "p<,>ffffd", x, y, z, angle, interior))
		{
			printf("[AttDef] Configuration option '%s' must contain four coordinates and an interior ID.", option);
			return false;
		}
		return true;
	}
	if (!strcmp(option, "RoundHPAR", true))
	{
		new health, armour;
		if (sscanf(value, "p<,>dd", armour, health))
		{
			printf("[AttDef] Configuration option '%s' must contain armour and health values.", option);
			return false;
		}
		return true;
	}
	if (!strcmp(option, "Maximum Packetloss", true))
	{
		new Float: packetLoss;
		if (sscanf(value, "f", packetLoss) || packetLoss <= 0.0 || packetLoss > 10.0)
		{
			printf("[AttDef] Configuration option '%s' must be a number greater than 0 and no greater than 10.", option);
			return false;
		}
		return true;
	}
	if (!strcmp(option, "CurrentBaseSet", true)
		|| !strcmp(option, "CurrentArenaSet", true)
		|| !strcmp(option, "ChatColor", true)
		|| !strcmp(option, "Server Name", true))
		return true;
	if (!IsNumeric(value))
	{
		printf("[AttDef] Configuration option '%s' must contain an integer value.", option);
		return false;
	}

	new numericValue = strval(value);
	if (!strcmp(option, "CPInArena", true)
		|| !strcmp(option, "HalfDeagle", true)
		|| !strcmp(option, "AntiMacros", true)
		|| !strcmp(option, "AntiCBug", true)
		|| !strcmp(option, "AntiSlideBug", true)
		|| !strcmp(option, "DeadBodies", true)
		|| !strcmp(option, "Aircraft", true)
		|| !strcmp(option, "ShowHPBars", true)
		|| !strcmp(option, "MeleeAllowed", true)
		|| !strcmp(option, "AutoRoundStarter", true)
		|| !strcmp(option, "RDeathMsg", true)
		|| !strcmp(option, "SightseeingInCS", true)
		|| !strcmp(option, "VehicleHealthTD", true)
		|| !strcmp(option, "DefVehicles", true)
		|| !strcmp(option, "SyncAbuse", true)
		|| !strcmp(option, "ScreenFlash", true)
		|| !strcmp(option, "UseCleo", true)
		|| !strcmp(option, "UseSampfuncs", true)
		|| !strcmp(option, "UseExtraws", true)
		|| !strcmp(option, "UseSprinthook", true))
	{
		if (numericValue != 0 && numericValue != 1)
		{
			printf("[AttDef] Configuration option '%s' must be 0 or 1.", option);
			return false;
		}
	}
	else if (!strcmp(option, "NetMode", true))
	{
		if (numericValue < 0 || numericValue > 3)
		{
			printf("[AttDef] Configuration option '%s' must be between 0 and 3.", option);
			return false;
		}
	}
	else if (!strcmp(option, "PauseMode", true))
	{
		if (numericValue < 0 || numericValue > 2)
		{
			printf("[AttDef] Configuration option '%s' must be between 0 and 2.", option);
			return false;
		}
	}
	return true;
}

stock ValidateConfigRows()
{
	new DBResult: duplicates = DB_ExecuteQuery(
		sqliteconnection,
		"SELECT Option FROM Configs GROUP BY Option HAVING COUNT(*) > 1");
	if (!duplicates)
	{
		printf("[AttDef] Failed to check for duplicate configuration options.");
		return 0;
	}
	new bool: hasDuplicates = DB_GetRowCount(duplicates) > 0;
	if (!DB_FreeResultSet(duplicates))
	{
		printf("[AttDef] Failed to release the duplicate-configuration query result.");
		return 0;
	}
	if (hasDuplicates)
	{
		printf("[AttDef] Warning: Duplicate options found in Configs table. Auto-cleaning...");
		DatabaseExecuteWrite(
			"DELETE FROM `Configs` WHERE rowid NOT IN (SELECT MAX(rowid) FROM `Configs` GROUP BY `Option`)",
			"clean duplicate config options");
	}

	new DBResult: result = DB_ExecuteQuery(sqliteconnection, "SELECT Option, Value FROM Configs");
	if (!result)
	{
		printf("[AttDef] Failed to validate the server configuration.");
		return 0;
	}
	if (DB_GetRowCount(result) < REQUIRED_CONFIG_OPTION_COUNT)
	{
		if (!DB_FreeResultSet(result))
			printf("[AttDef] Failed to release the incomplete configuration result.");
		printf("[AttDef] The Configs table must contain all %d required options.", REQUIRED_CONFIG_OPTION_COUNT);
		return 0;
	}

	new bool: seen[REQUIRED_CONFIG_OPTION_COUNT];
	new option[128], value[128];
	new bool: valid = true;
	for (new index = 0; index < REQUIRED_CONFIG_OPTION_COUNT; index++)
		seen[index] = false;

	do
	{
		DB_GetFieldStringByName(result, "Option", option, sizeof(option));
		new index = FindRequiredConfigOption(option);
		if (index == -1)
			continue;
		DB_GetFieldStringByName(result, "Value", value, sizeof(value));
		if (isnull(value))
		{
			printf("[AttDef] Required configuration option '%s' has an empty value.", option);
			valid = false;
			break;
		}
		if (!ValidateRequiredConfigValue(option, value))
		{
			valid = false;
			break;
		}
		if (seen[index])
		{
			printf("[AttDef] The Configs table contains duplicate required option '%s'.", option);
			valid = false;
			break;
		}
		seen[index] = true;
	}
	while (DB_SelectNextRow(result));

	if (!DB_FreeResultSet(result))
	{
		printf("[AttDef] Failed to release the configuration validation result.");
		return 0;
	}
	if (!valid)
		return 0;

	for (new index = 0; index < REQUIRED_CONFIG_OPTION_COUNT; index++)
	{
		if (!seen[index])
		{
			printf("[AttDef] The Configs table is missing required option '%s'.", RequiredConfigOptions[index]);
			return 0;
		}
	}
	return 1;
}

stock ApplyServerConfigValue(const option[], const value[])
{
	if (!strcmp(option, "Attacker Skin", true)) Skin[ATTACKER] = strval(value);
	else if (!strcmp(option, "Attacker Sub Skin", true)) Skin[ATTACKER_SUB] = strval(value);
	else if (!strcmp(option, "Defender Skin", true)) Skin[DEFENDER] = strval(value);
	else if (!strcmp(option, "Defender Sub Skin", true)) Skin[DEFENDER_SUB] = strval(value);
	else if (!strcmp(option, "Referee Skin", true)) Skin[REFEREE] = strval(value);
	else if (!strcmp(option, "Server Weather", true)) MainWeather = strval(value);
	else if (!strcmp(option, "Server Time", true)) MainTime = strval(value);
	else if (!strcmp(option, "CP Time", true)) ConfigCPTime = strval(value);
	else if (!strcmp(option, "Round Time", true)) ConfigRoundTime = strval(value);
	else if (!strcmp(option, "Total Rounds", true)) TotalRounds = strval(value);
	else if (!strcmp(option, "Maximum Packetloss", true)) Max_Packetloss = floatstr(value);
	else if (!strcmp(option, "Maximum Ping", true)) Max_Ping = strval(value);
	else if (!strcmp(option, "Minimum FPS", true)) Min_FPS = strval(value);
	else if (!strcmp(option, "Maximum FPS", true)) Max_FPS = strval(value);
	else if (!strcmp(option, "Main Spawn", true))
		sscanf(value, "p<,>ffffd", MainSpawn[0], MainSpawn[1], MainSpawn[2], MainSpawn[3], MainInterior);
	else if (!strcmp(option, "RoundHPAR", true))
		sscanf(value, "p<,>dd", RoundAR, RoundHP);
	else if (!strcmp(option, "MainBackgroundColour", true) || !strcmp(option, "MainTextColour", true)) return 1;
	else if (!strcmp(option, "ChatColor", true)) format(ColScheme, sizeof(ColScheme), "{%s}", value);
	else if (!strcmp(option, "CPInArena", true)) CPInArena = strval(value) == 1;
	else if (!strcmp(option, "HalfDeagle", true)) HalfDeagle = strval(value) == 1;
	else if (!strcmp(option, "AntiMacros", true)) AntiMacros = strval(value) == 1;
	else if (!strcmp(option, "AntiCBug", true)) AntiCBug = strval(value) == 1;
	else if (!strcmp(option, "AntiSlideBug", true)) AntiSlideBug = strval(value) == 1;
	else if (!strcmp(option, "DeadBodies", true)) DeadBodies = strval(value) == 1;
	else if (!strcmp(option, "Aircraft", true)) Aircraft = strval(value) == 1;
	else if (!strcmp(option, "ShowHPBars", true)) ShowHPBars = strval(value) == 1;
	else if (!strcmp(option, "GunmenuRestrictions", true)) GunmenuRestrictions = strval(value) == 1;
	else if (!strcmp(option, "MeleeAllowed", true)) MeleeAllowed = strval(value) == 1;
	else if (!strcmp(option, "CurrentBaseSet", true)) format(CurrentBaseSet, sizeof(CurrentBaseSet), "%s", value);
	else if (!strcmp(option, "AutoRoundStarter", true)) AutoRoundStarter = strval(value) == 1;
	else if (!strcmp(option, "GameSpeed", true)) GAME_SPEED = strval(value);
	else if (!strcmp(option, "RDeathMsg", true)) RandomDeathMessages = strval(value) == 1;
	else if (!strcmp(option, "SightseeingInCS", true)) SightseeingInClassSelection = strval(value) == 1;
	else if (!strcmp(option, "VehicleHealthTD", true)) VehicleHealthTextdraw = strval(value) == 1;
	else if (!strcmp(option, "DefVehicles", true)) DefendersWithVehicles = strval(value) == 1;
	else if (!strcmp(option, "SyncAbuse", true)) SyncAbuse = strval(value) == 1;
	else if (!strcmp(option, "ScreenFlash", true)) ScreenFlashing = strval(value) == 1;
	else if (!strcmp(option, "Server Name", true)) format(ServerName, sizeof(ServerName), "%s", value);
	else if (!strcmp(option, "UseCleo", true)) UseCleo = strval(value) == 1;
	else if (!strcmp(option, "UseSampfuncs", true)) UseSampfuncs = strval(value) == 1;
	else if (!strcmp(option, "UseExtraws", true)) UseExtraws = strval(value) == 1;
	else if (!strcmp(option, "UseSprinthook", true)) UseSprinthook = strval(value) == 1;
	else if (!strcmp(option, "CurrentArenaSet", true)) format(CurrentArenaSet, sizeof(CurrentArenaSet), "%s", value);
	else if (!strcmp(option, "NetMode", true)) gBadNetAction = strval(value);
	else if (!strcmp(option, "PauseMode", true)) gPauseMode = PauseMode:strval(value);
	else if (!strcmp(option, "AutoPause", true))
	{
		gAutoPauseMode = strval(value);
	}
	else if (!strcmp(option, "FloatingDamage", true)) FloatingDamage = strval(value) == 1;
	else if (!strcmp(option, "ArmourToggle", true)) ArmourToggle = strval(value) == 1;
	else if (!strcmp(option, "ArmourPingMin", true)) ArmourPingMin = strval(value);
	else if (!strcmp(option, "NoSwitch", true)) NoSwitch = strval(value) == 1;
	else if (!strcmp(option, "AutoEnd", true)) AutoEnd = strval(value) == 1;
	else if (!strcmp(option, "SkinLabelsPL", true)) GlobalTextLabelOption[TEXTLABEL_PL] = strval(value);
	else if (!strcmp(option, "SkinLabelsPing", true)) GlobalTextLabelOption[TEXTLABEL_Ping] = strval(value);
	else if (!strcmp(option, "SkinLabelsFPS", true)) GlobalTextLabelOption[TEXTLABEL_FPS] = strval(value);
	else if (!strcmp(option, "AddmeTime", true)) gAddmeTime = strval(value);
	else if (!strcmp(option, "GunmenuTime", true)) gGunmenuTime = strval(value);
	else return 0;
	return 1;
}

LoadConfig()
{
	if (!sqliteconnection)
	{
		printf("[AttDef] Cannot load configuration: SQLite database is not open.");
		return 0;
	}

	if (!ValidateConfigRows())
		return 0;

	new DBResult: res = DB_ExecuteQuery(sqliteconnection, "SELECT Option, Value FROM Configs");
	if (!res)
	{
		printf("[AttDef] Failed to read the server configuration.");
		return 0;
	}
	if (DB_GetRowCount(res) < REQUIRED_CONFIG_OPTION_COUNT)
	{
		if (!DB_FreeResultSet(res))
			printf("[AttDef] Failed to release the incomplete configuration result.");
		printf("[AttDef] The Configs table changed while its values were being loaded.");
		return 0;
	}

	Skin[NON] = 0;
	new option[128], value[128], loadedOptions;
	if (DB_GetRowCount(res) > 0)
	{
		do
		{
			DB_GetFieldStringByName(res, "Option", option, sizeof(option));
			DB_GetFieldStringByName(res, "Value", value, sizeof(value));
			if (ApplyServerConfigValue(option, value))
			{
				if (FindRequiredConfigOption(option) != -1)
					loadedOptions++;
			}
			else
				printf("[AttDef] Ignoring unrecognized configuration option '%s'.", option);
		}
		while (DB_SelectNextRow(res));
	}

	if (!DB_FreeResultSet(res))
		printf("[AttDef] Failed to release the configuration result after applying validated values.");
	if (loadedOptions != REQUIRED_CONFIG_OPTION_COUNT)
	{
		printf(
			"[AttDef] The Configs table is incomplete: loaded %d of %d required options.",
			loadedOptions, REQUIRED_CONFIG_OPTION_COUNT);
		return 0;
	}

	print("Server Config Loaded.");
	return 1;
}
