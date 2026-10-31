#if defined _attdef_OnPDamageDone
	#endinput
#endif
#define _attdef_OnPDamageDone

stock ClearDamageIndicator(playerid)
{
	if (w0Timer[playerid])
	{
		KillTimer(w0Timer[playerid]);
		w0Timer[playerid] = 0;
	}

	if (w0[playerid] != INVALID_OBJECT_ID && IsValidObject(w0[playerid]))
		DestroyObject(w0[playerid]);
	w0[playerid] = INVALID_OBJECT_ID;
}

public hidew0(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	w0Timer[playerid] = 0;
	if (w0[playerid] != INVALID_OBJECT_ID && IsValidObject(w0[playerid]))
		DestroyObject(w0[playerid]);
	w0[playerid] = INVALID_OBJECT_ID;
	return 1;
}

/*
This is used to make sure a head-shot is legit and successful
then announce it. This function has return values as well.
Returns 0 on failure (illegal/rejected headshots) and returns 1 on success.
*/
static HandleHeadshot(playerid, issuerid, WEAPON:weaponid)
{
	#pragma unused playerid, issuerid, weaponid
	return 1;
}

public OnPlayerDamageDone(playerid, Float: amount, issuerid, WEAPON:weapon, bodypart)
{
	if (!IsPlayerConnected(playerid))
		return 0;

	new bool: issuerConnected = issuerid >= 0 && issuerid < MAX_PLAYERS && IsPlayerConnected(issuerid);
	if (issuerConnected)
	{
		TrackAssistDamage(playerid, issuerid, amount);

		// Detect headshots
		if (bodypart == 9)
			HandleHeadshot(playerid, issuerid, weapon);

		// Show target player info for the shooter (HP, PL, Ping and many other things)
		ShowTargetInfo(issuerid, playerid, true);

		CreateFloatingDamageText(issuerid, playerid, amount);

		//SetPlayerDamageBubble(playerid, issuerid, amount, weapon);

		if (Player[issuerid][Playing] && Player[playerid][Playing])
		{
			Player[issuerid][shotsHit]++;
			Player[issuerid][RoundDamage] += amount;
			Player[issuerid][TotalDamage] += amount;
			UpdateRoundKillDmgTDmg(issuerid);
		}
	}
	UpdatePlayerHPAPVariables(playerid, Player[playerid][Playing]);
	UpdatePlayerHealthBar(playerid);

	if (amount > 0.0)
	{
		ClearDamageIndicator(playerid);

		new modelid = Player[playerid][pHealth] <= 0 || Player[playerid][pArmour] <= 0 ? 1240 : 1242;
		w0[playerid] = CreateObject(modelid, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, DRAW_DISTANCE);
		if (w0[playerid] != INVALID_OBJECT_ID)
		{
			AttachObjectToPlayer(w0[playerid], playerid, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0);
			w0Timer[playerid] = SetTimerEx("hidew0", 1000, false, "i", playerid);
			if (!w0Timer[playerid])
			{
				printf("[AttDef] Failed to schedule damage-indicator cleanup for player %d.", playerid);
				ClearDamageIndicator(playerid);
			}
		}
		else
		{
			printf("[AttDef] Failed to create damage indicator for player %d.", playerid);
		}
	}

	// If there's a round running
	if (Current != -1)
	{
		if (Player[playerid][Playing])
		{
			TriggerPlayerTeamBarFlash(playerid);
			UpdatePlayerTeamBar(playerid);
		}
		if (issuerConnected && Player[issuerid][Playing])
			UpdatePlayerTeamBar(issuerid);

		// Update teammates information textdraw
		UpdateTeamCornerInfo();
		
		// Update damaged team information textdraws
		if (!AllVsAllMode && Player[playerid][Playing]
			&& (Player[playerid][Team] == ATTACKER || Player[playerid][Team] == DEFENDER))
		{
			UpdateTeamHP(Player[playerid][Team]);
		}

		if (AllVsAllMode)
		{
			UpdateAllVsAllTextDraws();
		}
		// Show team lost hp textdraws
		else if (Player[playerid][Playing])
		{
			switch (Player[playerid][Team])
			{
				case ATTACKER:
				{
					va_TextDrawSetString(AttHpLose, "~r~~h~%s", Player[playerid][NameWithoutTag]);

					TempDamage[ATTACKER] += amount;
					va_TextDrawSetString(TeamHpLose[0], "~r~-%d", floatround(TempDamage[ATTACKER]));
					va_TextDrawSetString(ModernRound_AttDmgText, "-%d", floatround(TempDamage[ATTACKER]));
					va_TextDrawSetString(SimpleRound_AttDmgText, "-%d", floatround(TempDamage[ATTACKER]));
					foreach (new p : Player)
					{
						if (Player[p][Team] == REFEREE)
							continue;
						if (Player[p][RoundTDStyle] == 0)
						{
							TextDrawShowForPlayer(p, ModernRound_AttDmgBoard);
							TextDrawShowForPlayer(p, ModernRound_AttDmgText);
						}
						else if (Player[p][RoundTDStyle] == 2)
						{
							TextDrawShowForPlayer(p, SimpleRound_AttDmgText);
						}
					}
					UpdateRoundTeamStatusHUD();

					ModernTextDrawFlash(ATTACKER);

					if (AttHpTimer)
						KillTimer(AttHpTimer);
					AttHpTimer = SetTimerEx("HideHpTextForAtt", HIDE_DAMAGE_TEXTDRAW_AFTER, false, "i", ++AttHpTimerGeneration);
					if (!AttHpTimer)
					{
						AttHpTimerGeneration++;
						printf("[AttDef] Failed to schedule attacker damage-text cleanup.");
						TextDrawSetString(TeamHpLose[0], " ");
						TextDrawSetString(AttHpLose, " ");
						TempDamage[ATTACKER] = 0.0;
						StopModernTextDrawFlash(ATTACKER);
						UpdateRoundTeamStatusHUD();
					}
				}
				case DEFENDER:
				{
					va_TextDrawSetString(DefHpLose, "~b~~h~%s", Player[playerid][NameWithoutTag]);

					TempDamage[DEFENDER] += amount;
					va_TextDrawSetString(TeamHpLose[1], "~b~-%d", floatround(TempDamage[DEFENDER]));
					va_TextDrawSetString(ModernRound_DefDmgText, "-%d", floatround(TempDamage[DEFENDER]));
					va_TextDrawSetString(SimpleRound_DefDmgText, "-%d", floatround(TempDamage[DEFENDER]));
					foreach (new p : Player)
					{
						if (Player[p][Team] == REFEREE)
							continue;
						if (Player[p][RoundTDStyle] == 0)
						{
							TextDrawShowForPlayer(p, ModernRound_DefDmgBoard);
							TextDrawShowForPlayer(p, ModernRound_DefDmgText);
						}
						else if (Player[p][RoundTDStyle] == 2)
						{
							TextDrawShowForPlayer(p, SimpleRound_DefDmgText);
						}
					}
					UpdateRoundTeamStatusHUD();

					ModernTextDrawFlash(DEFENDER);

					if (DefHpTimer)
						KillTimer(DefHpTimer);
					DefHpTimer = SetTimerEx("HideHpTextForDef", HIDE_DAMAGE_TEXTDRAW_AFTER, false, "i", ++DefHpTimerGeneration);
					if (!DefHpTimer)
					{
						DefHpTimerGeneration++;
						printf("[AttDef] Failed to schedule defender damage-text cleanup.");
						TextDrawSetString(TeamHpLose[1], " ");
						TextDrawSetString(DefHpLose, " ");
						TempDamage[DEFENDER] = 0.0;
						StopModernTextDrawFlash(DEFENDER);
						UpdateRoundTeamStatusHUD();
					}
				}
			}
		}
	}
	return 1;
}
