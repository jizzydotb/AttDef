#if defined _attdef_OnPDeath
	#endinput
#endif
#define _attdef_OnPDeath


forward PlayerCounterTextDrawHide(killerid, connectionGeneration, timerGeneration);
public PlayerCounterTextDrawHide(killerid, connectionGeneration, timerGeneration)
{
	if (killerid < 0 || killerid >= MAX_PLAYERS || !IsPlayerConnected(killerid)
		|| PlayerConnectionGeneration[killerid] != connectionGeneration
		|| PlayerCounterTextDrawGeneration[killerid] != timerGeneration)
		return 0;
	return PlayerTextDrawHide(killerid, PointCounterTD[killerid]);
}

public OnPlayerDeath(playerid, killerid, reason)
{
	new deathGeneration = ++PlayerDeathFlowGeneration[playerid];
	new bool: spectateAfterDeath = false;
	DelayedFreezeGeneration[playerid]++;
	new KillerConnected = killerid >= 0 && killerid < MAX_PLAYERS
		&& killerid != playerid && IsPlayerConnected(killerid);

	new Float: deathX,
		Float: deathY,
		Float: deathZ;
	GetPlayerPos(playerid, deathX, deathY, deathZ);
	CreatePlayerBloodEffect(playerid, deathX, deathY, deathZ);
	if (!KillerConnected)
	{
		if (Player[playerid][Playing])
		{
			SendDeathMessage(INVALID_PLAYER_ID, playerid, reason);
			Player[playerid][RoundDeaths]++;
			Player[playerid][TotalDeaths]++;
			Player[playerid][ATimeDeaths]++;
			UpdateRoundKillDmgTDmg(playerid);

			va_SendClientMessageToAll(-1, "%s\xBB {FFFFFF}%s has died by %s.", TextColor[Player[playerid][Team]], Player[playerid][Name], ReturnWeaponName(reason));
			MessageBox(playerid, MSGBOX_TYPE_TOP, "wasted", "_", 3000, false);
			OnPlayerAmmoUpdate(playerid);
		}
		else if (Current == -1)
			SendDeathMessage(INVALID_PLAYER_ID, playerid, reason);
	}
	else
	{
		if (!RandomDeathMessages)
		{
			new victimMsg[128], killerMsg[128];
			format(victimMsg, sizeof(victimMsg), "~w~Killed By %s%s", TDC[Player[killerid][Team]], Player[killerid][Name]);
			format(killerMsg, sizeof(killerMsg), "~w~You Killed %s%s", TDC[Player[playerid][Team]], Player[playerid][Name]);
			ShowPlayerDeathNotice(playerid, 1, victimMsg);
			ShowPlayerDeathNotice(killerid, 0, killerMsg);
		}
		else
		{
			switch (reason)
			{
				case WEAPON_KNIFE:
				{
					new victimMsg[128], killerMsg[128];
					format(victimMsg, sizeof(victimMsg), "~rp~%s%s knifed you", Player[killerid][Name], MAIN_TEXT_COLOUR);
					format(killerMsg, sizeof(killerMsg), "%sYou knifed ~r~%s", MAIN_TEXT_COLOUR, Player[playerid][Name]);
					ShowPlayerDeathNotice(playerid, 1, victimMsg);
					ShowPlayerDeathNotice(killerid, 0, killerMsg);
				}
				case WEAPON_GRENADE:
				{
					new victimMsg[128], killerMsg[128];
					format(victimMsg, sizeof(victimMsg), "~r~%s%s bombed you", Player[killerid][Name], MAIN_TEXT_COLOUR);
					format(killerMsg, sizeof(killerMsg), "%sYou bombed ~r~%s", MAIN_TEXT_COLOUR, Player[playerid][Name]);
					ShowPlayerDeathNotice(playerid, 1, victimMsg);
					ShowPlayerDeathNotice(killerid, 0, killerMsg);
				}
				default:
				{
					switch (random(4))
					{
						case 0:
						{
							new victimMsg[128], killerMsg[128];
							format(victimMsg, sizeof(victimMsg), "~r~%s%s defeated you", Player[killerid][Name], MAIN_TEXT_COLOUR);
							format(killerMsg, sizeof(killerMsg), "%sYou defeated ~r~%s", MAIN_TEXT_COLOUR, Player[playerid][Name]);
							ShowPlayerDeathNotice(playerid, 1, victimMsg);
							ShowPlayerDeathNotice(killerid, 0, killerMsg);
						}
						case 1:
						{
							new victimMsg[128], killerMsg[128];
							format(victimMsg, sizeof(victimMsg), "~r~%s%s owned you", Player[killerid][Name], MAIN_TEXT_COLOUR);
							format(killerMsg, sizeof(killerMsg), "%sYou owned ~r~%s", MAIN_TEXT_COLOUR, Player[playerid][Name]);
							ShowPlayerDeathNotice(playerid, 1, victimMsg);
							ShowPlayerDeathNotice(killerid, 0, killerMsg);
						}
						case 2:
						{
							new victimMsg[128], killerMsg[128];
							format(victimMsg, sizeof(victimMsg), "~r~%s%s murdered you", Player[killerid][Name], MAIN_TEXT_COLOUR);
							format(killerMsg, sizeof(killerMsg), "%sYou murdered ~r~%s", MAIN_TEXT_COLOUR, Player[playerid][Name]);
							ShowPlayerDeathNotice(playerid, 1, victimMsg);
							ShowPlayerDeathNotice(killerid, 0, killerMsg);
						}
						case 3:
						{
							new victimMsg[128], killerMsg[128];
							format(victimMsg, sizeof(victimMsg), "~r~%s%s sent you to cemetery", Player[killerid][Name], MAIN_TEXT_COLOUR);
							format(killerMsg, sizeof(killerMsg), "%sYou sent ~r~%s%s to cemetery", MAIN_TEXT_COLOUR, Player[playerid][Name], MAIN_TEXT_COLOUR);
							ShowPlayerDeathNotice(playerid, 1, victimMsg);
							ShowPlayerDeathNotice(killerid, 0, killerMsg);
						}
					}
				}
			}
		}

		new killerTextGeneration = ++PlayerDeathMessageGeneration[killerid];
		new playerTextGeneration = killerid == playerid
			? killerTextGeneration
			: ++PlayerDeathMessageGeneration[playerid];
		if (!SetTimerEx(
			"DeathMessageF", 4000, false, "iiiiii", killerid, playerid,
			PlayerConnectionGeneration[killerid], PlayerConnectionGeneration[playerid],
			killerTextGeneration, playerTextGeneration))
		{
			printf("[AttDef] Failed to schedule death-message cleanup for players %d and %d.", killerid, playerid);
			HidePlayerDeathNotice(killerid);
			HidePlayerDeathNotice(playerid);
		}

		if (Player[playerid][Playing] && Player[killerid][Playing] && AllowStartBase)
		{
			ShowPlayerDeathMessage(killerid, playerid);
			SendDeathMessage(killerid, playerid, reason);

			Player[killerid][RoundKills]++;
			Player[killerid][TotalKills]++;
			Player[killerid][ATimeKills]++;
			Player[playerid][RoundDeaths]++;
			Player[playerid][TotalDeaths]++;
			Player[playerid][ATimeDeaths]++;
			AwardTeamAssists(playerid, killerid);

			new expToAdd = 0, ptsStr[8];
			switch (reason)
			{
				case WEAPON_DEAGLE:   { Player[killerid][AKillDGL]++; expToAdd = 5;  format(ptsStr, sizeof(ptsStr), "+5"); }
				case WEAPON_M4:       { Player[killerid][AKillM]++;   expToAdd = 5;  format(ptsStr, sizeof(ptsStr), "+5"); }
				case WEAPON_SNIPER:   { Player[killerid][AKillSN]++;  expToAdd = 10; format(ptsStr, sizeof(ptsStr), "+10"); }
				case WEAPON_SHOTGSPA: { Player[killerid][AKillSP]++;  expToAdd = 5;  format(ptsStr, sizeof(ptsStr), "+5"); }
				case WEAPON_SHOTGUN:  { Player[killerid][AKillSHT]++; expToAdd = 5;  format(ptsStr, sizeof(ptsStr), "+5"); }
				case WEAPON_RIFLE:    { Player[killerid][AKillPNCH]++; expToAdd = 10; format(ptsStr, sizeof(ptsStr), "+10"); }
				case WEAPON_AK47:     { Player[killerid][AKillAK]++;  expToAdd = 8;  format(ptsStr, sizeof(ptsStr), "+8"); }
				case WEAPON_MP5:      { expToAdd = 10; format(ptsStr, sizeof(ptsStr), "+10"); }
				case WEAPON_SILENCED: { expToAdd = 15; format(ptsStr, sizeof(ptsStr), "+15"); }
				case WEAPON_FIST:     { expToAdd = 20; format(ptsStr, sizeof(ptsStr), "+20"); }
			}
			if (expToAdd > 0)
			{
				Player[killerid][Exp] += expToAdd;
				PlayerTextDrawHide(killerid, PointCounterTD[killerid]);
				PlayerTextDrawSetString(killerid, PointCounterTD[killerid], ptsStr);
				PlayerTextDrawShow(killerid, PointCounterTD[killerid]);
				new timerGeneration = ++PlayerCounterTextDrawGeneration[killerid];
				if (!SetTimerEx(
					"PlayerCounterTextDrawHide", 2000, false, "iii", killerid,
					PlayerConnectionGeneration[killerid], timerGeneration))
				{
					printf("[AttDef] Failed to schedule point-counter cleanup for player %d.", killerid);
					PlayerTextDrawHide(killerid, PointCounterTD[killerid]);
				}
			}

			new str[160];
			new deathVerbs[][] = {
				"Defeated","Overwhelmed","Crushed","Destroyed","Terminated",
				"Obliterated","Annihilated","Vanquished","Eliminated",
				"Eradicated","Exterminated","Annexed","Incapacitated",
				"Neutralized","Erased","Liquidated","Exorcised","Extinguished",
				"Demolished","Snapped", "Killed"
			};
			if (GameType == BASE)
			{
				format(
					str, sizeof(str), "%s\xBB %s '%s' %s <%s> (Weap: %s | HP: %d - %d | Dist: %.1f ft) \xAB", TextColor[Player[killerid][Team]], Player[killerid][Name], deathVerbs[random(sizeof(deathVerbs))],
					Player[playerid][Name], Player[playerid][PlayerTypeByWeapon], ReturnWeaponName(reason), GetHP(killerid), GetAP(killerid), GetDistanceBetweenPlayers(killerid, playerid));
			}
			else
			{
				format(
					str, sizeof(str), "%s\xBB %s '%s' %s (Weap: %s | HP: %d - %d | Dist: %.1f ft) \xAB", TextColor[Player[killerid][Team]], Player[killerid][Name], deathVerbs[random(sizeof(deathVerbs))],
					Player[playerid][Name], ReturnWeaponName(reason), GetHP(killerid), GetAP(killerid), GetDistanceBetweenPlayers(killerid, playerid));
			}
			SendClientMessageToAll(-1, str);
			if (!AllVsAllMode && Player[killerid][RoundKills] >= 3)
			{
				if (Player[killerid][RoundKills] == 3)
					va_SendClientMessageToAll(-1, "%s\xBB Kill Streak: %s {FFFFFF}is on a "COL_PRIM"Triple Kill {FFFFFF}streak! (3 kills)", TextColor[Player[killerid][Team]], Player[killerid][Name]);
				else if (Player[killerid][RoundKills] == 4)
					va_SendClientMessageToAll(-1, "%s\xBB Kill Streak: %s {FFFFFF}is on an "COL_PRIM"Ultra Kill {FFFFFF}streak! (4 kills)", TextColor[Player[killerid][Team]], Player[killerid][Name]);
				else
					va_SendClientMessageToAll(-1, "%s\xBB Rampage: %s {FFFFFF}is "COL_PRIM"UNSTOPPABLE{FFFFFF}! (%d kills in this round)", TextColor[Player[killerid][Team]], Player[killerid][Name], Player[killerid][RoundKills]);
			}
			UpdateRoundKillDmgTDmg(killerid);
			if (killerid != playerid)
				UpdateRoundKillDmgTDmg(playerid);
			UpdateLvl(killerid);

			OnPlayerAmmoUpdate(playerid);
		}
		else
		{
			if (Current == -1)
				SendDeathMessage(killerid, playerid, reason);
			if (Player[killerid][InDM])
			{
				SetHPAP(killerid, 100.0, 100.0);

				Player[playerid][VWorld] = GetPlayerVirtualWorld(killerid);
			}
		}
	}
	ResetVictimAssistDamage(playerid);

	// All-vs-All is score/time based: keep the player in the round and respawn
	// them instead of entering the regular elimination/death-camera flow.
	if (Player[playerid][Playing] && AllVsAllMode && GameType == ARENA)
	{
		UpdateAllVsAllTextDraws();
		if (Player[playerid][OutOfArena] != MAX_ZONE_LIMIT_WARNINGS)
			PlayerTextDrawHide(playerid, AreaCheckTD[playerid]);
		if (!SetTimerEx(
			"RespawnAllVsAllPlayer", 1250, false, "iii", playerid,
			PlayerConnectionGeneration[playerid], deathGeneration))
		{
			printf("[AttDef] Failed to schedule All-vs-All respawn for player %d; respawning immediately.", playerid);
			return RespawnAllVsAllPlayer(playerid, PlayerConnectionGeneration[playerid], deathGeneration);
		}
		return 1;
	}

	if (Player[playerid][Playing])
	{
		CreateDeadBody(playerid, killerid, reason, 0.0, deathX, deathY, deathZ);
		PlayerNoLeadTeam(playerid);
		if (!SetTimerEx(
			"SpectateAnyPlayerT", GetPlayerPing(playerid) + 30, false, "iii", playerid,
			PlayerConnectionGeneration[playerid], deathGeneration))
		{
			printf("[AttDef] Failed to schedule post-death spectating for player %d.", playerid);
			spectateAfterDeath = true;
		}
		// Create an icon on radar where the player died
		CreateTeamDeathMapIcon(Player[playerid][Team], deathX, deathY, deathZ);
	}
	else if (Player[playerid][InDuel])
		ProcessDuellerDeath(playerid, killerid, reason);

	// Hide arena out of bound warning textdraws if they're shown
	if (Player[playerid][OutOfArena] != MAX_ZONE_LIMIT_WARNINGS)
	{
		PlayerTextDrawHide(playerid, AreaCheckTD[playerid]);
		PlayerTextDrawHide(playerid, AreaCheckBG[playerid]);
	}
	Player[playerid][InDM]	  = false;
	SetPlayerRoundParticipation(playerid, false);
	DeletePlayerTeamBar(playerid);
	ReorganizeTeamBars(Player[playerid][Team]);
	UpdateTeamCornerInfo();
	UpdateTeamPlayerCount(Player[playerid][Team], true, playerid);
	UpdateTeamHP(Player[playerid][Team], playerid);
	// Handle spectate
	foreach (new i : AllSpectators)
	{
		if (Player[i][IsSpectatingID] == playerid)
		{
			if (Player[i][Team] == REFEREE)
				SpectateAnyPlayer(i, false, true, playerid);
			else
				SpectateAnyPlayer(i, true, true, playerid);
		}
	}

	// If they're selecting from gunmenu
	if (Player[playerid][OnGunmenu])
	{
		// Hide it!!!
		HidePlayerGunmenu(playerid);
	}

	UpdatePlayerHealthMoney(playerid);

	// Reset player gunmenu selections
	ResetPlayerGunmenu(playerid, false);

	// Call OnPlayerLeaveCheckpoint to see if player was in CP and fix issues
	CallLocalFunction("OnPlayerLeaveCheckpoint", "i", playerid);

	// If he's spectating, stop it
	if (Player[playerid][Spectating])
		StopSpectate(playerid);

	// If they're in a DM
	if (Player[playerid][DMReadd] >= 0)
	{
		GetPlayerPos(playerid, Player[playerid][DmPosX], Player[playerid][DmPosY], Player[playerid][DmPosZ]);
		GetPlayerFacingAngle(playerid, Player[playerid][DmAngle]);
		// Re-spawn them there
		SpawnInDM(playerid, Player[playerid][DMReadd]);
	}
	else if (!Player[playerid][InDuel]) // Spawn player in a lobby
		SpawnInLobby(playerid);
	if (spectateAfterDeath && Current != -1)
		SpectateAnyPlayer(playerid, true, true);
	return 1;
}
