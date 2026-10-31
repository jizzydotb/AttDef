#if defined _attdef_OnPKeyStateChange
	#endinput
#endif
#define _attdef_OnPKeyStateChange

stock bool: IsAntiCBugWeapon(weaponid)
{
	switch (weaponid)
	{
		case 23..25, 27, 29..34, 41: return true;
	}
	return false;
}

stock bool: IsAntiSlideBugWeapon(weaponid)
{
	switch (weaponid)
	{
		case 22, 24, 25, 27, 29, 30, 31, 33, 34, 38, 41: return true;
	}
	return false;
}

stock bool: IsSlideBugInputActive(weaponid, keys, updown)
{
	if (!IsAntiSlideBugWeapon(weaponid))
		return false;

	return (keys & KEY_SPRINT)
		&& (keys & KEY_AIM)
		&& (updown == KEY_UP || updown == KEY_DOWN);
}

stock EnsureAntiBugPlayerState(playerid)
{
	if (AntiBugTrackedGeneration[playerid] == PlayerConnectionGeneration[playerid])
		return 1;

	AntiBugTrackedGeneration[playerid] = PlayerConnectionGeneration[playerid];
	AntiCBugCheckActive[playerid] = false;
	AntiCBugCheckStarted[playerid] = 0;
	AntiCBugCheckWeapon[playerid] = 0;
	AntiCBugCheckAmmo[playerid] = 0;
	AntiSlideBugCrouchTick[playerid] = 0;
	AntiSlideBugTriggered[playerid] = false;
	AntiBugFrozen[playerid] = false;
	AntiBugFreezeGeneration[playerid]++;
	return 1;
}

stock bool: FreezePlayerForAntiBug(playerid, const reason[])
{
	if (AntiBugFrozen[playerid])
		return true;
	if (!IsPlayerControllable(playerid))
		return false;

	new freezeGeneration = ++AntiBugFreezeGeneration[playerid];
	if (!SetTimerEx(
		"EndAntiBugFreeze", ANTIBUG_FREEZE_DURATION_MS, false, "iii",
		playerid, PlayerConnectionGeneration[playerid], freezeGeneration))
	{
		printf("[AttDef] Failed to schedule anti-bug freeze release for player %d.", playerid);
		return false;
	}

	AntiBugFrozen[playerid] = true;
	TogglePlayerControllable(playerid, false);
	va_SendClientMessageToAll(
		-1, "\xBB "COL_ERROR"System froze "COL_SEC"%s "COL_ERROR"for %s.", Player[playerid][Name], reason);
	PlayerPlaySound(playerid, 1190, 0.0, 0.0, 0.0);
	return true;
}

stock bool: CheckAntiSlideBugInput(playerid, weaponid, keys, updown, bool: crouchPressed)
{
	if (!AntiSlideBug || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT
		|| !IsAntiSlideBugWeapon(weaponid))
	{
		AntiSlideBugCrouchTick[playerid] = 0;
		AntiSlideBugTriggered[playerid] = false;
		return false;
	}

	if (crouchPressed)
	{
		AntiSlideBugCrouchTick[playerid] = GetTickCount();
		AntiSlideBugTriggered[playerid] = false;
	}

	if (!AntiSlideBugCrouchTick[playerid])
		return false;

	new currentTick = GetTickCount();
	if (currentTick - AntiSlideBugCrouchTick[playerid] > ANTISLIDEBUG_INPUT_WINDOW_MS)
	{
		AntiSlideBugCrouchTick[playerid] = 0;
		AntiSlideBugTriggered[playerid] = false;
		return false;
	}

	if (AntiSlideBugTriggered[playerid] || !IsSlideBugInputActive(weaponid, keys, updown))
		return false;

	if (!FreezePlayerForAntiBug(playerid, "slidebugging"))
		return false;

	AntiSlideBugTriggered[playerid] = true;
	AntiSlideBugCrouchTick[playerid] = 0;
	return true;
}

forward EndAntiBugFreeze(playerid, connectionGeneration, freezeGeneration);
public EndAntiBugFreeze(playerid, connectionGeneration, freezeGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration
		|| AntiBugFreezeGeneration[playerid] != freezeGeneration)
		return 0;

	AntiBugFrozen[playerid] = false;
	if (RoundPaused || RoundUnpausing || Player[playerid][OnGunmenu]
		|| Player[playerid][Spectating] || Player[playerid][IsFrozen]
		|| Player[playerid][ToAddInRound]
		|| GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
		return 1;

	TogglePlayerControllable(playerid, true);
	return 1;
}

stock bool: CheckAntiCBugAttempt(playerid, keys)
{
	if (!AntiCBugCheckActive[playerid])
		return false;

	new currentTick = GetTickCount();
	if (currentTick - AntiCBugCheckStarted[playerid] > ANTICBUG_CHECK_WINDOW_MS
		|| GetPlayerState(playerid) != PLAYER_STATE_ONFOOT
		|| GetPlayerWeapon(playerid) != AntiCBugCheckWeapon[playerid])
	{
		AntiCBugCheckActive[playerid] = false;
		return false;
	}

	if ((keys & KEY_CROUCH) && !((keys & KEY_FIRE) || (keys & KEY_HANDBRAKE))
		&& GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_DUCK
		&& AntiCBugCheckAmmo[playerid] > GetPlayerAmmo(playerid))
	{
		AntiCBugCheckActive[playerid] = false;
		FreezePlayerForAntiBug(playerid, "C-bugging");
		return true;
	}
	return false;
}

stock bool: ShouldTrackAntiCBugAttempt(playerid, newkeys, oldkeys)
{
	new currentKeys, ud, lr;
	GetPlayerKeys(playerid, currentKeys, ud, lr);
	new bool: notMoving = !ud && !lr;
	new bool: fireWithoutSprintOrJump = (newkeys & KEY_FIRE)
		&& !((newkeys & KEY_SPRINT) || (newkeys & KEY_JUMP));
	new bool: fireWhileStationary = notMoving && (newkeys & KEY_FIRE);
	new bool: fireAfterCrouch = (newkeys & KEY_FIRE) && (oldkeys & KEY_CROUCH)
		&& !((oldkeys & KEY_FIRE) || (newkeys & KEY_HANDBRAKE));
	new bool: crouchAfterFire = (oldkeys & KEY_FIRE) && (newkeys & KEY_CROUCH)
		&& !((newkeys & KEY_FIRE) || (newkeys & KEY_HANDBRAKE));

	return fireWithoutSprintOrJump || fireWhileStationary || fireAfterCrouch || crouchAfterFire;
}

stock ProcessAntiBugPlayerUpdate(playerid)
{
	EnsureAntiBugPlayerState(playerid);

	if (AntiCBug)
	{
		new keys, ud, lr;
		GetPlayerKeys(playerid, keys, ud, lr);
		if (CheckAntiCBugAttempt(playerid, keys))
			return 1;
	}
	else
	{
		AntiCBugCheckActive[playerid] = false;
	}

	new keys, updown, leftright;
	GetPlayerKeys(playerid, keys, updown, leftright);
	#pragma unused leftright
	CheckAntiSlideBugInput(playerid, GetPlayerWeapon(playerid), keys, updown, false);
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	EnsureAntiBugPlayerState(playerid);
	if ((newkeys & KEY_WALK) && !(oldkeys & KEY_WALK)
		&& (newkeys & KEY_AIM) && CreateTeamPing(playerid))
		return 1;

	new keys, updown, leftright;
	GetPlayerKeys(playerid, keys, updown, leftright);
	#pragma unused leftright
	CheckAntiSlideBugInput(
		playerid, GetPlayerWeapon(playerid), newkeys, updown,
		(newkeys & KEY_CROUCH) && !(oldkeys & KEY_CROUCH));

	if (AntiCBug)
	{
		if (CheckAntiCBugAttempt(playerid, newkeys))
			return 1;

		new weaponid = GetPlayerWeapon(playerid);
		if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT
			&& IsAntiCBugWeapon(weaponid)
			&& ShouldTrackAntiCBugAttempt(playerid, newkeys, oldkeys))
		{
			AntiCBugCheckActive[playerid] = true;
			AntiCBugCheckStarted[playerid] = GetTickCount();
			AntiCBugCheckWeapon[playerid] = weaponid;
			AntiCBugCheckAmmo[playerid] = GetPlayerAmmo(playerid);
		}
	}
	else
	{
		AntiCBugCheckActive[playerid] = false;
	}

	new currentTick = GetTickCount();
	new vehicleid = GetPlayerVehicleID(playerid);
	// - High priority key functions
	if (PRESSED(KEY_JUMP | KEY_HANDBRAKE) && !vehicleid)
	{
		switch (GetWeaponSlot(GetPlayerWeapon(playerid)))
		{
			case 0, 1, 8, 11:
			{
				SyncPlayer(playerid);
				return 1;
			}
		}
	}
	if (AntiMacros && CheckPlayerSprintMacro(playerid, newkeys, oldkeys))
		return 1;

	if (PRESSED(KEY_FIRE)) // key fire
	{
		if (Player[playerid][TextDrawOnScreen] && (LastMatchEndTime + 2) - gettime() <= 0)
		{
			HideEndRoundTextDraw(playerid);
			return 1;
		}
	}
	if (Player[playerid][Spectating] && Player[playerid][SpectatingRound] == -1)
	{
		if (PRESSED(KEY_FIRE))
		{
			if (Current != -1)
				SpectateNextTeamPlayer(playerid);
			else
				SpectateNextPlayer(playerid);
		}
		else if (PRESSED(KEY_HANDBRAKE))
		{
			if (Current != -1)
				SpectatePreviousTeamPlayer(playerid);
			else
				SpectatePreviousPlayer(playerid);
		}
		return 1;
	}
	if (Current != -1)
	{
		if (Player[playerid][Playing])
		{
			if (PRESSED(KEY_NO) && AllowStartBase)
			{
				if (PlayerRequestBackup(playerid))
					return 1;
			}
			// Lead team
			if (PRESSED(KEY_CTRL_BACK) && AllowStartBase)
			{
				if (vehicleid)
					return 1;

				if (currentTick - Player[playerid][LastAskLeader] < 5000)
				{
					SendErrorMessage(playerid, "Please wait.");
					return 0;
				}
				new team = Player[playerid][Team];
				if (TeamHasLeader[team] != true)
				{
					PlayerLeadTeam(playerid, false, true);
				}
				else
				{
					if (TeamLeader[team] == playerid) // off
					{
						PlayerNoLeadTeam(playerid);
					}
					else
						SendErrorMessage(playerid, "Your team already has a leader!");
				}
				Player[playerid][LastAskLeader] = currentTick;
				return 1;
			}
		}

		// Pause/unpause or ask for pause/unpause
		if (PRESSED(KEY_YES))
		{

			if (Player[playerid][Level] > 0)
			{
				// Arena players become active only after its camera countdown. Permit
				// an admin queued for that countdown to pause it just like a base.
				if (!Player[playerid][Spectating] && !Player[playerid][Playing]
					&& !(Current != -1 && Player[playerid][ToAddInRound] && ViewRoundCountdown > 0))
					return 1;

				switch (RoundPaused)
				{
					case true:
					{
						new iString[144];
						if (currentTick - PausePressed < 3000)
							return SendErrorMessage(playerid, "Please Wait.");
						if (RoundUnpausing == true)
							return 1;

						PauseCountdown = 4;
						UnpauseRound();

						format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has unpaused the current round.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
						SendClientMessageToAll(-1, iString);
						return 1;
					}
					case false:
					{
						new iString[144];
						if (RoundUnpausing == true)
							return SendErrorMessage(playerid, "Round is unpausing, please wait.");

						PausePressed = currentTick;

						PauseRound();

						format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has paused the current round.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
						SendClientMessageToAll(-1, iString);
						return 1;
					}
				}
			}
			else
			{
				if (!Player[playerid][Playing])
					return 1;

				if (currentTick - Player[playerid][LastPauseRequest] < 10000)
					return SendErrorMessage(playerid, "You have to wait 10 seconds before asking for a pause again.");

				foreach (new i : Player)
					PlayerPlaySound(i, 1133, 0.0, 0.0, 0.0);
				Player[playerid][LastPauseRequest] = currentTick;
				SendClientMessageToAll(-1, va_return(""COL_SEC"%s " COL_PRIM "is asking for an %spause!", Player[playerid][Name], RoundPaused ? "un" : ""));
				return 1;
			}
		}
	}

	// - Low priority key functions

	if (CheckKeysForWeaponBind(playerid, newkeys, oldkeys))
		return 1;

	if (vehicleid && PRESSED(KEY_FIRE) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER
		&& IsValidVehicle(vehicleid) && VehicleCanHaveComponent(GetVehicleModel(vehicleid), 1010)
		&& GetVehicleComponentInSlot(vehicleid, CARMODTYPE_NITRO) != 1010)
	{
		AddVehicleComponent(vehicleid, 1010);
		return 1;
	}

	if (Player[playerid][SpectatingRound] != -1)
	{
		new iString[128];
		new playerWeapon = GetPlayerWeapon(playerid);

		// Only allow switching if the player is unarmed (weapon 0 is fist)
		if (playerWeapon == 0)
		{
			switch (Player[playerid][SpectatingType])
			{
				case BASE:
				{
					if (PRESSED(KEY_FIRE))
					{
						new searching = 0;
						new i = Player[playerid][SpectatingRound] + 1;
						while (searching < 2)
						{
							if (i >= MAX_BASES) i = 1;
							if (BExist[i] == true)
							{
								Player[playerid][SpectatingRound] = i;
								DisablePlayerCheckpoint(playerid);
								DeleteArenaSpecGangzone(playerid);
								SetPlayerInterior(playerid, BInterior[i]);
								SetPlayerCheckpoint(playerid, BCPSpawn[i][0], BCPSpawn[i][1], BCPSpawn[i][2], 2);
								SetPlayerPos(playerid, BCPSpawn[i][0], BCPSpawn[i][1], BCPSpawn[i][2]);
								va_PlayerTextDrawSetString(playerid, TD_RoundSpec[playerid], "%sBase ~n~%s%s (ID: ~r~%d%s)", MAIN_TEXT_COLOUR, MAIN_TEXT_COLOUR, BName[i], i, MAIN_TEXT_COLOUR);
								va_SendClientMessage(playerid, -1, COL_PRIM "\xBB Viewing Base "COL_SEC"'%s' (ID: %d)", BName[i], i);
								return 1;
							}
							i++;
							if (i >= MAX_BASES) 
							{
								i = 1;
								searching++;
							}
						}
					}
					else if (PRESSED(KEY_HANDBRAKE))
					{
						new searching = 0;
						new i = Player[playerid][SpectatingRound] - 1;
						while (searching < 2)
						{
							if (i < 1) i = MAX_BASES - 1;
							if (BExist[i] == true)
							{
								Player[playerid][SpectatingRound] = i;
								DisablePlayerCheckpoint(playerid);
								DeleteArenaSpecGangzone(playerid);
								SetPlayerInterior(playerid, BInterior[i]);
								SetPlayerPos(playerid, BCPSpawn[i][0], BCPSpawn[i][1], BCPSpawn[i][2]);
								SetPlayerCheckpoint(playerid, BCPSpawn[i][0], BCPSpawn[i][1], BCPSpawn[i][2], 2);
								format(iString, sizeof(iString), "%sBase ~n~%s%s (ID: ~r~%d%s)", MAIN_TEXT_COLOUR, MAIN_TEXT_COLOUR, BName[i], i, MAIN_TEXT_COLOUR);
								PlayerTextDrawSetString(playerid, TD_RoundSpec[playerid], iString);
								va_SendClientMessage(playerid, -1, COL_PRIM "\xBB Viewing Base "COL_SEC"'%s' (ID: %d)", BName[i], i);
								return 1;
							}
							i--;
							if (i < 1)
							{
								i = MAX_BASES - 1;
								searching++;
							}
						}
					}
					return 1;
				}
				case ARENA:
				{
					if (PRESSED(KEY_FIRE))
					{
						new searching = 0;
						new i = Player[playerid][SpectatingRound] + 1;
						while (searching < 2)
						{
							if (i >= MAX_ARENAS) i = 0;
							if (AExist[i] == true)
							{
								Player[playerid][SpectatingRound] = i;
								DisablePlayerCheckpoint(playerid);
								DeleteArenaSpecGangzone(playerid);
								SetPlayerInterior(playerid, AInterior[i]);
								SetPlayerPos(
									playerid, ACPSpawn[Player[playerid][SpectatingRound]][0], ACPSpawn[Player[playerid][SpectatingRound]][1],
									ACPSpawn[Player[playerid][SpectatingRound]][2]);
								SetPlayerCheckpoint(playerid, ACPSpawn[Player[playerid][SpectatingRound]][0], ACPSpawn[Player[playerid][SpectatingRound]][1], ACPSpawn[Player[playerid][SpectatingRound]][2], 2);
								va_PlayerTextDrawSetString(playerid, TD_RoundSpec[playerid], "%sArena ~n~%s%s (ID: ~r~%d%s)", MAIN_TEXT_COLOUR, MAIN_TEXT_COLOUR, AName[i], i, MAIN_TEXT_COLOUR);
								va_SendClientMessage(playerid, -1, COL_PRIM "\xBB Viewing Arena "COL_SEC"'%s' (ID: %d)", AName[i], i);
								
								new Float:min_x = AMin[i][0];
								new Float:min_y = AMin[i][1];
								new Float:max_x = AMax[i][0];
								new Float:max_y = AMax[i][1];
								ArenaSpecGangzone[playerid] = GangZoneCreate(min_x, min_y, max_x, max_y);
								GangZoneShowForPlayer(playerid, ArenaSpecGangzone[playerid], 0xFFFF00AA);
								return 1;
							}
							i++;
							if (i >= MAX_ARENAS)
							{
								i = 0;
								searching++;
							}
						}
					}
					else if (PRESSED(KEY_HANDBRAKE))
					{
						new searching = 0;
						new i = Player[playerid][SpectatingRound] - 1;
						while (searching < 2)
						{
							if (i < 0) i = MAX_ARENAS - 1;
							if (AExist[i] == true)
							{
								Player[playerid][SpectatingRound] = i;
								DisablePlayerCheckpoint(playerid);
								DeleteArenaSpecGangzone(playerid);
								SetPlayerInterior(playerid, AInterior[i]);
								SetPlayerPos(
									playerid, ACPSpawn[Player[playerid][SpectatingRound]][0], ACPSpawn[Player[playerid][SpectatingRound]][1],
									ACPSpawn[Player[playerid][SpectatingRound]][2]);
								SetPlayerCheckpoint(playerid, ACPSpawn[Player[playerid][SpectatingRound]][0], ACPSpawn[Player[playerid][SpectatingRound]][1], ACPSpawn[Player[playerid][SpectatingRound]][2], 2);
								va_PlayerTextDrawSetString(playerid, TD_RoundSpec[playerid], "%sArena ~n~%s%s (ID: ~r~%d%s)", MAIN_TEXT_COLOUR, MAIN_TEXT_COLOUR, AName[i], i, MAIN_TEXT_COLOUR);
								va_SendClientMessage(playerid, -1, COL_PRIM "\xBB Viewing Arena "COL_SEC"'%s' (ID: %d)", AName[i], i);

								new Float:min_x = AMin[i][0];
								new Float:min_y = AMin[i][1];
								new Float:max_x = AMax[i][0];
								new Float:max_y = AMax[i][1];
								ArenaSpecGangzone[playerid] = GangZoneCreate(min_x, min_y, max_x, max_y);
								GangZoneShowForPlayer(playerid, ArenaSpecGangzone[playerid], 0xFFFF00AA);
								return 1;
							}
							i--;
							if (i < 0)
							{
								i = MAX_ARENAS - 1;
								searching++;
							}
						}
					}
					return 1;
				}
			}
		}
		else if (PRESSED(KEY_FIRE) || PRESSED(KEY_HANDBRAKE))
		{
			va_SendClientMessage(playerid, -1, COL_PRIM "\xBB You cannot change base or arena while holding a weapon.");
		}
		return 1;
	}
	if (Current == -1)
	{
		if (PRESSED(KEY_NO) && GetPlayerVehicleID(playerid) == 0)
		{
			if (ServerLastPlayed == -1 && ServerLastPlayedType == -1)
			{
				return SendErrorMessage(playerid, "No game has been played yet. Results are not available.");
			}
			else
			{
				ShowEndRoundTextDraw(playerid);
				return 1;
			}
		}
	}
	return 1;
}
