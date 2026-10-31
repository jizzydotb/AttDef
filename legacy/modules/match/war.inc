#if defined _attdef_match_war
	#endinput
#endif
#define _attdef_match_war

/*
	Match/War Mode Lifecycle Sub-module
*/

ScheduleWarEnd(delay)
{
	if (MatchEndTimer)
		KillTimer(MatchEndTimer);

	WarModeEnding = true;
	MatchEndGeneration++;
	MatchEndTimer = SetTimerEx("WarEnded", delay, false, "i", MatchEndGeneration);
	if (!MatchEndTimer)
	{
		printf("[AttDef] Failed to schedule match end timer; ending the match immediately.");
		return WarEnded(MatchEndGeneration);
	}
	return 1;
}

forward WarEnded(generation);
public WarEnded(generation)
{
	if (generation != MatchEndGeneration || !WarModeEnding)
		return 0;

	MatchEndTimer = 0;
	MatchEndGeneration++;
	FixGamemodeText();
	ClearKillList(); // Clears the kill-list.

	new iString[256], TopString[3][128];

	new winnerid = -1;

	if (TeamScore[ATTACKER] > TeamScore[DEFENDER])
	{
		winnerid = ATTACKER;
	}
	else if (TeamScore[DEFENDER] > TeamScore[ATTACKER])
	{
		winnerid = DEFENDER;
	}

	MatchEnded	  = true;
	HPBars_IsMatchEnd = true;
	WarModeEnding = false;

	new mWinner = (TeamScore[ATTACKER] > TeamScore[DEFENDER]) ? ATTACKER : ((TeamScore[DEFENDER] > TeamScore[ATTACKER]) ? DEFENDER : -1);
	TeamIcons_LastWinner = mWinner;

	CurrentRound  = 0;
	format(iString, sizeof(iString), "SELECT * FROM Configs WHERE Option = 'Total Rounds'");
	new DBResult: res = DB_ExecuteQuery(sqliteconnection, iString);

	DB_GetFieldStringByName(res, "Value", iString, sizeof(iString));
	TotalRounds = strval(iString);
	DB_FreeResultSet(res);

	UpdateRoundsPlayedTextDraw();

	WarMode = false;

	HideMatchScoreBoard();

	new index, p;

	foreach (new i : Player)
	{
		if (Player[i][TotalDamage] > 0.0)
		{
			playerScores[index][player_Score]	= floatround(Player[i][TotalDamage]);
			playerScores[index][player_Team]	= Player[i][Team];
			playerScores[index][player_Kills]	= Player[i][TotalKills];
			playerScores[index][player_Deaths]	= Player[i][TotalDeaths];
			playerScores[index][player_TPlayed] = Player[i][RoundPlayed];
			playerScores[index][player_HP]		= GetHP(i) + GetAP(i);

			new Float: nTotalAccuracy;
			if (Player[i][TotalBulletsFired] == 0)
				nTotalAccuracy = 0.0;
			else
				nTotalAccuracy = floatmul(100.0, floatdiv(Player[i][TotalshotsHit], Player[i][TotalBulletsFired]));
			playerScores[index][player_Acc] = floatround(nTotalAccuracy, floatround_round);

			if (playerScores[index][player_TPlayed] > 0)
				playerScores[index][player_Overall] = (playerScores[index][player_Score] / playerScores[index][player_TPlayed]) + playerScores[index][player_Kills]
													+ playerScores[index][player_TPlayed] + playerScores[index][player_Acc] - playerScores[index][player_Deaths];
			else
				playerScores[index][player_Overall] = 0;

			format(RoundResultNames[index], MAX_PLAYER_NAME, Player[i][NameWithoutTag]);

			printf("overall of %s[%d]: %d", RoundResultNames[index], index, playerScores[index][player_Overall]);

			playerScores[index++][player_ID] = i;
			p++;
		}
		if (Player[i][Spectating])
			StopSpectate(i);

		if (!Player[i][InDuel] && (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER))
			ShowEndRoundTextDraw(i);
	}
	for (new i = 0; i < SAVE_SLOTS; ++i)
	{
		if (SaveVariables[i][pName][0] && SaveVariables[i][TDamage] > 0.0)
		{
			playerScores[index][player_Score]	= floatround(SaveVariables[i][TDamage]);
			playerScores[index][player_Team]	= SaveVariables[i][pTeam];
			playerScores[index][player_Kills]	= SaveVariables[i][TKills];
			playerScores[index][player_Deaths]	= SaveVariables[i][TDeaths];
			playerScores[index][player_TPlayed] = SaveVariables[i][TPlayed];
			playerScores[index][player_HP]		= SaveVariables[i][gHealth] + SaveVariables[i][gArmour];

			new Float: nTotalAccuracy;
			if (SaveVariables[i][tBulletsShot] == 0)
				nTotalAccuracy = 0.0;
			else
				nTotalAccuracy = floatmul(100.0, floatdiv(SaveVariables[i][tshotsHit], SaveVariables[i][tBulletsShot]));
			playerScores[index][player_Acc] = floatround(nTotalAccuracy, floatround_round);

			if (playerScores[index][player_TPlayed] > 0)
				playerScores[index][player_Overall] = (playerScores[index][player_Score] / playerScores[index][player_TPlayed]) + playerScores[index][player_Kills]
													+ playerScores[index][player_TPlayed] + playerScores[index][player_Acc] - playerScores[index][player_Deaths];
			else
				playerScores[index][player_Overall] = 0;

			format(RoundResultNames[index], MAX_PLAYER_NAME, SaveVariables[i][pNameWithoutTag]);

			SaveVariables[i][RKills]  = 0;
			SaveVariables[i][RDeaths] = 0;
			SaveVariables[i][RDamage] = 0.0;

			SaveVariables[i][RoundID] = -1;
			index++;
			p++;
		}
	}

	if (index > 1)
		GetPlayerHighestScores2(playerScores, RoundResultNames, 0, index - 1);

	new topkill, topkillID = -1, Float: topDmg,
				 topDmgID = -1,
				 topAcc,
				 topAccID = -1,
				 topOverall,
				 topOverallID = -1;

	for (new i = 0; i != p; ++i)
	{
		if (playerScores[i][player_Kills] > topkill && playerScores[i][player_Kills] > 0)
		{
			topkill	  = playerScores[i][player_Kills];
			topkillID = i;
		}
		if (playerScores[i][player_Score] > topDmg && playerScores[i][player_Score] > 0)
		{
			topDmg	 = playerScores[i][player_Score];
			topDmgID = i;
		}
		if (playerScores[i][player_Acc] > topAcc && playerScores[i][player_Acc] > 0)
		{
			topAcc	 = playerScores[i][player_Acc];
			topAccID = i;
		}
		if (playerScores[i][player_Overall] > topOverall && playerScores[i][player_Overall] != 0 && playerScores[i][player_Score] > 0 && playerScores[i][player_TPlayed] > 0
			&& (winnerid == -1 || playerScores[i][player_Team] == winnerid))
		{
			topOverall	 = playerScores[i][player_Overall];
			topOverallID = i;
		}

		if (playerScores[i][player_Team] == ATTACKER || playerScores[i][player_Team] == ATTACKER_SUB)
		{
			format(AttList, sizeof(AttList), "%s~w~%s~n~", AttList, RoundResultNames[i]);
			format(AttKills, sizeof(AttKills), "%s~w~%d~n~", AttKills, playerScores[i][player_Kills]);
			format(AttDeaths, sizeof(AttDeaths), "%s~w~%d~n~", AttDeaths, playerScores[i][player_Deaths]);
			format(AttPlayed, sizeof(AttPlayed), "%s~w~%d~n~", AttPlayed, playerScores[i][player_TPlayed]);
			format(AttDamage, sizeof(AttDamage), "%s~w~%d~n~", AttDamage, playerScores[i][player_Score]);
			format(AttAcc, sizeof(AttAcc), "%s~w~%d%%~n~", AttAcc, playerScores[i][player_Acc]);
		}
		else if (playerScores[i][player_Team] == DEFENDER || playerScores[i][player_Team] == DEFENDER_SUB)
		{
			format(DefList, sizeof(DefList), "%s~w~%s~n~", DefList, RoundResultNames[i]);
			format(DefKills, sizeof(DefKills), "%s~w~%d~n~", DefKills, playerScores[i][player_Kills]);
			format(DefDeaths, sizeof(DefDeaths), "%s~w~%d~n~", DefDeaths, playerScores[i][player_Deaths]);
			format(DefPlayed, sizeof(DefPlayed), "%s~w~%d~n~", DefPlayed, playerScores[i][player_TPlayed]);
			format(DefDamage, sizeof(DefDamage), "%s~w~%d~n~", DefDamage, playerScores[i][player_Score]);
			format(DefAcc, sizeof(DefAcc), "%s~w~%d%%~n~", DefAcc, playerScores[i][player_Acc]);
		}
		if (i == 0)
			format(
				TopString[0], 128, "\xBB " COL_WARNING "1st: " COL_SEC "%s " COL_MUTED "(Damage: " COL_SEC "%d " COL_MUTED "| Kills: " COL_SEC "%d" COL_MUTED ")", RoundResultNames[i], playerScores[i][player_Score],
				playerScores[i][player_Kills]);
		else if (i == 1)
			format(
				TopString[1], 128, "\xBB " COL_MUTED "2nd: " COL_SEC "%s " COL_MUTED "(Damage: " COL_SEC "%d " COL_MUTED "| Kills: " COL_SEC "%d" COL_MUTED ")", RoundResultNames[i], playerScores[i][player_Score],
				playerScores[i][player_Kills]);
		else if (i == 2)
			format(
				TopString[2], 128, "\xBB " COL_MUTED "3rd: " COL_SEC "%s " COL_MUTED "(Damage: " COL_SEC "%d " COL_MUTED "| Kills: " COL_SEC "%d" COL_MUTED ")", RoundResultNames[i], playerScores[i][player_Score],
				playerScores[i][player_Kills]);
	}

	iString = "";

	UpdateHPBarsMatchStats(playerScores, RoundResultNames, p, topkillID, topkill, topDmgID, topDmg, topAccID, topAcc);
	new matchWinnerID = (TeamScore[ATTACKER] > TeamScore[DEFENDER]) ? ATTACKER : ((TeamScore[DEFENDER] > TeamScore[ATTACKER]) ? DEFENDER : -1);
	UpdateTeamIconsMatchStats(playerScores, RoundResultNames, p, matchWinnerID, topkillID, topkill, topDmgID, topDmg, topAccID, topAcc);
	UpdateModernMatchStats(playerScores, RoundResultNames, p, matchWinnerID, topkillID, topkill, topDmgID, topDmg, topAccID, topAcc, AttList, AttDamage, AttKills, AttDeaths, DefList, DefDamage, DefKills, DefDeaths);

	ClearChat();
	
	new wMessage[144];
	SendClientMessageToAll(-1, COL_PRIM "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	if (TeamScore[ATTACKER] > TeamScore[DEFENDER])
	{
		winnerid = ATTACKER;
		format(wMessage, sizeof(wMessage), "\xBB %sTeam %s " COL_PRIM "won the match! " COL_MUTED "(Final Score: " COL_SEC "%d " COL_MUTED "vs " COL_SEC "%d" COL_MUTED ")", TextColor[ATTACKER], TeamName[ATTACKER], TeamScore[ATTACKER], TeamScore[DEFENDER]);
		SendClientMessageToAll(-1, wMessage);
	}
	else if (TeamScore[DEFENDER] > TeamScore[ATTACKER])
	{
		winnerid = DEFENDER;
		format(wMessage, sizeof(wMessage), "\xBB %sTeam %s " COL_PRIM "won the match! " COL_MUTED "(Final Score: " COL_SEC "%d " COL_MUTED "vs " COL_SEC "%d" COL_MUTED ")", TextColor[DEFENDER], TeamName[DEFENDER], TeamScore[DEFENDER], TeamScore[ATTACKER]);
		SendClientMessageToAll(-1, wMessage);
	}
	else
	{
		format(wMessage, sizeof(wMessage), "\xBB " COL_PRIM "The match ended in a draw! " COL_MUTED "(%sTeam %s " COL_SEC "%d " COL_MUTED "- " COL_SEC "%d %sTeam %s" COL_MUTED ")", TextColor[ATTACKER], TeamName[ATTACKER], TeamScore[ATTACKER], TeamScore[DEFENDER], TextColor[DEFENDER], TeamName[DEFENDER]);
		SendClientMessageToAll(-1, wMessage);
	}
	if (TopString[0][0]) SendClientMessageToAll(-1, TopString[0]);
	if (TopString[1][0]) SendClientMessageToAll(-1, TopString[1]);
	if (TopString[2][0]) SendClientMessageToAll(-1, TopString[2]);
	SendClientMessageToAll(-1, COL_PRIM "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	SendClientMessageToAll(-1, " ");
	if (topOverallID != -1 && (winnerid != -1 || playerScores[topOverallID][player_Team] == ATTACKER || playerScores[topOverallID][player_Team] == DEFENDER))
		SendClientMessageToAll(-1, va_return("\xBB {FFFFFF}Match MVP: %s%s", TextColor[playerScores[topOverallID][player_Team]], RoundResultNames[topOverallID]));

	format(LastMatchSummary, sizeof(LastMatchSummary), "%s", wMessage);
	LastMatchScoreCount = p;
	format(LastMatchTopString[0], sizeof(LastMatchTopString[]), "%s", TopString[0]);
	format(LastMatchTopString[1], sizeof(LastMatchTopString[]), "%s", TopString[1]);
	format(LastMatchTopString[2], sizeof(LastMatchTopString[]), "%s", TopString[2]);

	CallLocalFunction("OnMatchEndEvent", "dddd", winnerid, TeamScore[ATTACKER], TeamScore[DEFENDER], topOverallID);

	TeamTacPauseUsed[ATTACKER] = false;
	TeamTacPauseUsed[DEFENDER] = false;

	TeamName[ATTACKER]	   = "Alpha";
	TeamName[ATTACKER_SUB] = "Alpha Sub";
	TeamName[DEFENDER]	   = "Beta";
	TeamName[DEFENDER_SUB] = "Beta Sub";

	TeamScore[ATTACKER]	   = 0;
	TeamScore[DEFENDER]	   = 0;

	UpdateTeamScoreTextDraw();
	UpdateRoundsPlayedTextDraw();
	UpdateTeamNameTextDraw();

	foreach (new i : Player)
	{
		ResetPlayerRoundVariables(i, true);
		UpdateRoundKillDmgText(i);
	}

	ClearPlayerVariables();
	ForgetPlayedBases();
	ForgetPlayedArenas();

	AttList	  = "";
	AttKills  = "";
	AttDeaths = "";
	AttPlayed = "";
	AttAcc	  = "";
	AttDamage = "";

	DefList	  = "";
	DefKills  = "";
	DefDeaths = "";
	DefPlayed = "";
	DefAcc	  = "";
	DefDamage = "";
	return 1;
}
