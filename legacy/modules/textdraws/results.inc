#if defined _attdef_td_results
	#endinput
#endif
#define _attdef_td_results

/*
	Round & Match Results Textdraws Sub-module (Modern, HPBars, TeamIcons)
*/

ResultTextdrawsCreate()
{
	Modern_RoundInfo[0] = TextDrawCreate(490.000, 319.530, "Base (id 37) - Elimination win");
	TextDrawLetterSize(Modern_RoundInfo[0], 0.1883, 0.8481);
	TextDrawAlignment(Modern_RoundInfo[0], 3);
	TextDrawColour(Modern_RoundInfo[0], -1);
	TextDrawSetShadow(Modern_RoundInfo[0], 0);
	TextDrawSetOutline(Modern_RoundInfo[0], 0);
	TextDrawBackgroundColour(Modern_RoundInfo[0], 0x000000FF);
	TextDrawFont(Modern_RoundInfo[0], 1);
	TextDrawSetProportional(Modern_RoundInfo[0], true);

	Modern_RoundInfo[1] = TextDrawCreate(490.000, 326.914, "Time elapsed: 2:52");
	TextDrawLetterSize(Modern_RoundInfo[1], 0.1883, 0.8481);
	TextDrawAlignment(Modern_RoundInfo[1], 3);
	TextDrawColour(Modern_RoundInfo[1], -1);
	TextDrawSetShadow(Modern_RoundInfo[1], 0);
	TextDrawSetOutline(Modern_RoundInfo[1], 0);
	TextDrawBackgroundColour(Modern_RoundInfo[1], 0x000000FF);
	TextDrawFont(Modern_RoundInfo[1], 1);
	TextDrawSetProportional(Modern_RoundInfo[1], true);

	Modern_RoundInfo[2] = TextDrawCreate(490.000, 334.463, "Time left: 3:38");
	TextDrawLetterSize(Modern_RoundInfo[2], 0.1883, 0.8481);
	TextDrawAlignment(Modern_RoundInfo[2], 3);
	TextDrawColour(Modern_RoundInfo[2], -1);
	TextDrawSetShadow(Modern_RoundInfo[2], 0);
	TextDrawSetOutline(Modern_RoundInfo[2], 0);
	TextDrawBackgroundColour(Modern_RoundInfo[2], 0x000000FF);
	TextDrawFont(Modern_RoundInfo[2], 1);
	TextDrawSetProportional(Modern_RoundInfo[2], true);

	Modern_AttColorBar = TextDrawCreate(146.000, 100.900, "_");
	TextDrawLetterSize(Modern_AttColorBar, 0.1224, 3.0551);
	TextDrawTextSize(Modern_AttColorBar, 316.000, 930.000);
	TextDrawAlignment(Modern_AttColorBar, 1);
	TextDrawColour(Modern_AttColorBar, -1);
	TextDrawUseBox(Modern_AttColorBar, true);
	TextDrawBoxColour(Modern_AttColorBar, 0x2ED955AA);
	TextDrawSetShadow(Modern_AttColorBar, 0);
	TextDrawSetOutline(Modern_AttColorBar, 0);
	TextDrawBackgroundColour(Modern_AttColorBar, 0x000000FF);
	TextDrawFont(Modern_AttColorBar, 1);
	TextDrawSetProportional(Modern_AttColorBar, true);

	Modern_DefColorBar = TextDrawCreate(324.000, 100.900, "_");
	TextDrawLetterSize(Modern_DefColorBar, 0.1224, 3.0551);
	TextDrawTextSize(Modern_DefColorBar, 494.000, 930.000);
	TextDrawAlignment(Modern_DefColorBar, 1);
	TextDrawColour(Modern_DefColorBar, -1);
	TextDrawUseBox(Modern_DefColorBar, true);
	TextDrawBoxColour(Modern_DefColorBar, 0x3D82FFAA);
	TextDrawSetShadow(Modern_DefColorBar, 0);
	TextDrawSetOutline(Modern_DefColorBar, 0);
	TextDrawBackgroundColour(Modern_DefColorBar, 0x000000FF);
	TextDrawFont(Modern_DefColorBar, 1);
	TextDrawSetProportional(Modern_DefColorBar, true);

	Modern_AttBoard = TextDrawCreate(146.000, 131.714, "_");
	TextDrawLetterSize(Modern_AttBoard, 0.0146, 16.9951);
	TextDrawTextSize(Modern_AttBoard, 316.000, 0.000);
	TextDrawAlignment(Modern_AttBoard, 1);
	TextDrawColour(Modern_AttBoard, -1);
	TextDrawUseBox(Modern_AttBoard, true);
	TextDrawBoxColour(Modern_AttBoard, 0x00000092);
	TextDrawSetShadow(Modern_AttBoard, 0);
	TextDrawSetOutline(Modern_AttBoard, 0);
	TextDrawBackgroundColour(Modern_AttBoard, 0x000000FF);
	TextDrawFont(Modern_AttBoard, 1);
	TextDrawSetProportional(Modern_AttBoard, true);

	Modern_DefBoard = TextDrawCreate(324.000, 131.714, "_");
	TextDrawLetterSize(Modern_DefBoard, 0.0146, 16.9951);
	TextDrawTextSize(Modern_DefBoard, 494.000, 0.000);
	TextDrawAlignment(Modern_DefBoard, 1);
	TextDrawColour(Modern_DefBoard, -1);
	TextDrawUseBox(Modern_DefBoard, true);
	TextDrawBoxColour(Modern_DefBoard, 0x00000092);
	TextDrawSetShadow(Modern_DefBoard, 0);
	TextDrawSetOutline(Modern_DefBoard, 0);
	TextDrawBackgroundColour(Modern_DefBoard, 0x000000FF);
	TextDrawFont(Modern_DefBoard, 1);
	TextDrawSetProportional(Modern_DefBoard, true);

	Modern_BottomBoard = TextDrawCreate(146.000, 295.256, "_");
	TextDrawLetterSize(Modern_BottomBoard, 0.0146, 5.8170);
	TextDrawTextSize(Modern_BottomBoard, 494.000, 0.000);
	TextDrawAlignment(Modern_BottomBoard, 1);
	TextDrawColour(Modern_BottomBoard, -1);
	TextDrawUseBox(Modern_BottomBoard, true);
	TextDrawBoxColour(Modern_BottomBoard, 0x00000092);
	TextDrawSetShadow(Modern_BottomBoard, 0);
	TextDrawSetOutline(Modern_BottomBoard, 0);
	TextDrawBackgroundColour(Modern_BottomBoard, 0x000000FF);
	TextDrawFont(Modern_BottomBoard, 1);
	TextDrawSetProportional(Modern_BottomBoard, true);

	Modern_AttHeaderName = TextDrawCreate(149.185, 129.731, "Name");
	TextDrawLetterSize(Modern_AttHeaderName, 0.1881, 0.8849);
	TextDrawTextSize(Modern_AttHeaderName, 640.133, 930.000);
	TextDrawAlignment(Modern_AttHeaderName, 1);
	TextDrawColour(Modern_AttHeaderName, -1);
	TextDrawSetShadow(Modern_AttHeaderName, 0);
	TextDrawSetOutline(Modern_AttHeaderName, 0);
	TextDrawBackgroundColour(Modern_AttHeaderName, 0x000000FF);
	TextDrawFont(Modern_AttHeaderName, 1);
	TextDrawSetProportional(Modern_AttHeaderName, true);

	Modern_AttHeaderDmg = TextDrawCreate(249.585, 129.690, "Dmg");
	TextDrawLetterSize(Modern_AttHeaderDmg, 0.1881, 0.9034);
	TextDrawTextSize(Modern_AttHeaderDmg, 740.400, 930.000);
	TextDrawAlignment(Modern_AttHeaderDmg, 1);
	TextDrawColour(Modern_AttHeaderDmg, -1);
	TextDrawSetShadow(Modern_AttHeaderDmg, 0);
	TextDrawSetOutline(Modern_AttHeaderDmg, 0);
	TextDrawBackgroundColour(Modern_AttHeaderDmg, 0x000000FF);
	TextDrawFont(Modern_AttHeaderDmg, 1);
	TextDrawSetProportional(Modern_AttHeaderDmg, true);

	Modern_AttHeaderKills = TextDrawCreate(272.385, 129.690, "Kills");
	TextDrawLetterSize(Modern_AttHeaderKills, 0.1881, 0.9034);
	TextDrawTextSize(Modern_AttHeaderKills, 763.200, 930.000);
	TextDrawAlignment(Modern_AttHeaderKills, 1);
	TextDrawColour(Modern_AttHeaderKills, -1);
	TextDrawSetShadow(Modern_AttHeaderKills, 0);
	TextDrawSetOutline(Modern_AttHeaderKills, 0);
	TextDrawBackgroundColour(Modern_AttHeaderKills, 0x000000FF);
	TextDrawFont(Modern_AttHeaderKills, 1);
	TextDrawSetProportional(Modern_AttHeaderKills, true);

	Modern_AttHeaderHealth = TextDrawCreate(293.852, 129.690, "Health");
	TextDrawLetterSize(Modern_AttHeaderHealth, 0.1881, 0.9034);
	TextDrawTextSize(Modern_AttHeaderHealth, 784.667, 930.000);
	TextDrawAlignment(Modern_AttHeaderHealth, 1);
	TextDrawColour(Modern_AttHeaderHealth, -1);
	TextDrawSetShadow(Modern_AttHeaderHealth, 0);
	TextDrawSetOutline(Modern_AttHeaderHealth, 0);
	TextDrawBackgroundColour(Modern_AttHeaderHealth, 0x000000FF);
	TextDrawFont(Modern_AttHeaderHealth, 1);
	TextDrawSetProportional(Modern_AttHeaderHealth, true);

	Modern_TeamWhoWon = TextDrawCreate(273.531, 294.793, "_");
	TextDrawLetterSize(Modern_TeamWhoWon, 0.3819, 1.6882);
	TextDrawTextSize(Modern_TeamWhoWon, 641.390, 930.000);
	TextDrawAlignment(Modern_TeamWhoWon, 1);
	TextDrawColour(Modern_TeamWhoWon, -1);
	TextDrawSetShadow(Modern_TeamWhoWon, 0);
	TextDrawSetOutline(Modern_TeamWhoWon, 0);
	TextDrawBackgroundColour(Modern_TeamWhoWon, 0x000000FF);
	TextDrawFont(Modern_TeamWhoWon, 1);
	TextDrawSetProportional(Modern_TeamWhoWon, true);

	Modern_MVPDamage = TextDrawCreate(149.177, 319.591, "_");
	TextDrawLetterSize(Modern_MVPDamage, 0.1873, 0.8754);
	TextDrawTextSize(Modern_MVPDamage, 634.533, 930.000);
	TextDrawAlignment(Modern_MVPDamage, 1);
	TextDrawColour(Modern_MVPDamage, -1);
	TextDrawSetShadow(Modern_MVPDamage, 0);
	TextDrawSetOutline(Modern_MVPDamage, 0);
	TextDrawBackgroundColour(Modern_MVPDamage, 0x000000FF);
	TextDrawFont(Modern_MVPDamage, 1);
	TextDrawSetProportional(Modern_MVPDamage, true);

	Modern_MVPLeastDmg = TextDrawCreate(149.311, 326.560, "_");
	TextDrawLetterSize(Modern_MVPLeastDmg, 0.1873, 0.8754);
	TextDrawTextSize(Modern_MVPLeastDmg, 634.667, 930.000);
	TextDrawAlignment(Modern_MVPLeastDmg, 1);
	TextDrawColour(Modern_MVPLeastDmg, -1);
	TextDrawSetShadow(Modern_MVPLeastDmg, 0);
	TextDrawSetOutline(Modern_MVPLeastDmg, 0);
	TextDrawBackgroundColour(Modern_MVPLeastDmg, 0x000000FF);
	TextDrawFont(Modern_MVPLeastDmg, 1);
	TextDrawSetProportional(Modern_MVPLeastDmg, true);

	Modern_MVPKills = TextDrawCreate(149.244, 334.192, "_");
	TextDrawLetterSize(Modern_MVPKills, 0.1873, 0.8754);
	TextDrawTextSize(Modern_MVPKills, 634.600, 930.000);
	TextDrawAlignment(Modern_MVPKills, 1);
	TextDrawColour(Modern_MVPKills, -1);
	TextDrawSetShadow(Modern_MVPKills, 0);
	TextDrawSetOutline(Modern_MVPKills, 0);
	TextDrawBackgroundColour(Modern_MVPKills, 0x000000FF);
	TextDrawFont(Modern_MVPKills, 1);
	TextDrawSetProportional(Modern_MVPKills, true);

	Modern_DefHeaderName = TextDrawCreate(327.464, 129.958, "Name");
	TextDrawLetterSize(Modern_DefHeaderName, 0.1881, 0.8849);
	TextDrawTextSize(Modern_DefHeaderName, 818.412, 930.000);
	TextDrawAlignment(Modern_DefHeaderName, 1);
	TextDrawColour(Modern_DefHeaderName, -1);
	TextDrawSetShadow(Modern_DefHeaderName, 0);
	TextDrawSetOutline(Modern_DefHeaderName, 0);
	TextDrawBackgroundColour(Modern_DefHeaderName, 0x000000FF);
	TextDrawFont(Modern_DefHeaderName, 1);
	TextDrawSetProportional(Modern_DefHeaderName, true);

	Modern_DefHeaderDmg = TextDrawCreate(427.864, 129.916, "Dmg");
	TextDrawLetterSize(Modern_DefHeaderDmg, 0.1881, 0.9034);
	TextDrawTextSize(Modern_DefHeaderDmg, 918.678, 930.000);
	TextDrawAlignment(Modern_DefHeaderDmg, 1);
	TextDrawColour(Modern_DefHeaderDmg, -1);
	TextDrawSetShadow(Modern_DefHeaderDmg, 0);
	TextDrawSetOutline(Modern_DefHeaderDmg, 0);
	TextDrawBackgroundColour(Modern_DefHeaderDmg, 0x000000FF);
	TextDrawFont(Modern_DefHeaderDmg, 1);
	TextDrawSetProportional(Modern_DefHeaderDmg, true);

	Modern_DefHeaderKills = TextDrawCreate(450.664, 129.916, "Kills");
	TextDrawLetterSize(Modern_DefHeaderKills, 0.1881, 0.9034);
	TextDrawTextSize(Modern_DefHeaderKills, 941.478, 930.000);
	TextDrawAlignment(Modern_DefHeaderKills, 1);
	TextDrawColour(Modern_DefHeaderKills, -1);
	TextDrawSetShadow(Modern_DefHeaderKills, 0);
	TextDrawSetOutline(Modern_DefHeaderKills, 0);
	TextDrawBackgroundColour(Modern_DefHeaderKills, 0x000000FF);
	TextDrawFont(Modern_DefHeaderKills, 1);
	TextDrawSetProportional(Modern_DefHeaderKills, true);

	Modern_DefHeaderHealth = TextDrawCreate(472.130, 129.916, "Health");
	TextDrawLetterSize(Modern_DefHeaderHealth, 0.1881, 0.9034);
	TextDrawTextSize(Modern_DefHeaderHealth, 962.945, 930.000);
	TextDrawAlignment(Modern_DefHeaderHealth, 1);
	TextDrawColour(Modern_DefHeaderHealth, -1);
	TextDrawSetShadow(Modern_DefHeaderHealth, 0);
	TextDrawSetOutline(Modern_DefHeaderHealth, 0);
	TextDrawBackgroundColour(Modern_DefHeaderHealth, 0x000000FF);
	TextDrawFont(Modern_DefHeaderHealth, 1);
	TextDrawSetProportional(Modern_DefHeaderHealth, true);

	Modern_AttNames = TextDrawCreate(149.245, 137.270, "_");
	TextDrawLetterSize(Modern_AttNames, 0.1905, 0.8665);
	TextDrawTextSize(Modern_AttNames, 640.767, 930.000);
	TextDrawAlignment(Modern_AttNames, 1);
	TextDrawColour(Modern_AttNames, -1);
	TextDrawSetShadow(Modern_AttNames, 0);
	TextDrawSetOutline(Modern_AttNames, 0);
	TextDrawBackgroundColour(Modern_AttNames, 0x000000FF);
	TextDrawFont(Modern_AttNames, 1);
	TextDrawSetProportional(Modern_AttNames, true);

	Modern_AttDamage = TextDrawCreate(250.301, 137.187, "_");
	TextDrawLetterSize(Modern_AttDamage, 0.1905, 0.8665);
	TextDrawTextSize(Modern_AttDamage, 741.822, 930.000);
	TextDrawAlignment(Modern_AttDamage, 1);
	TextDrawColour(Modern_AttDamage, -1);
	TextDrawSetShadow(Modern_AttDamage, 0);
	TextDrawSetOutline(Modern_AttDamage, 0);
	TextDrawBackgroundColour(Modern_AttDamage, 0x000000FF);
	TextDrawFont(Modern_AttDamage, 1);
	TextDrawSetProportional(Modern_AttDamage, true);

	Modern_AttKills = TextDrawCreate(276.767, 137.021, "_");
	TextDrawLetterSize(Modern_AttKills, 0.1905, 0.8665);
	TextDrawTextSize(Modern_AttKills, 768.289, 930.000);
	TextDrawAlignment(Modern_AttKills, 1);
	TextDrawColour(Modern_AttKills, -1);
	TextDrawSetShadow(Modern_AttKills, 0);
	TextDrawSetOutline(Modern_AttKills, 0);
	TextDrawBackgroundColour(Modern_AttKills, 0x000000FF);
	TextDrawFont(Modern_AttKills, 1);
	TextDrawSetProportional(Modern_AttKills, true);

	Modern_AttHealth = TextDrawCreate(296.234, 136.938, "_");
	TextDrawLetterSize(Modern_AttHealth, 0.1905, 0.8665);
	TextDrawTextSize(Modern_AttHealth, 787.755, 930.000);
	TextDrawAlignment(Modern_AttHealth, 1);
	TextDrawColour(Modern_AttHealth, -1);
	TextDrawSetShadow(Modern_AttHealth, 0);
	TextDrawSetOutline(Modern_AttHealth, 0);
	TextDrawBackgroundColour(Modern_AttHealth, 0x000000FF);
	TextDrawFont(Modern_AttHealth, 1);
	TextDrawSetProportional(Modern_AttHealth, true);

	Modern_DefNames = TextDrawCreate(327.471, 137.146, "_");
	TextDrawLetterSize(Modern_DefNames, 0.1905, 0.8665);
	TextDrawTextSize(Modern_DefNames, 818.992, 930.000);
	TextDrawAlignment(Modern_DefNames, 1);
	TextDrawColour(Modern_DefNames, -1);
	TextDrawSetShadow(Modern_DefNames, 0);
	TextDrawSetOutline(Modern_DefNames, 0);
	TextDrawBackgroundColour(Modern_DefNames, 0x000000FF);
	TextDrawFont(Modern_DefNames, 1);
	TextDrawSetProportional(Modern_DefNames, true);

	Modern_DefDamage = TextDrawCreate(428.959, 137.063, "_");
	TextDrawLetterSize(Modern_DefDamage, 0.1905, 0.8665);
	TextDrawTextSize(Modern_DefDamage, 920.481, 930.000);
	TextDrawAlignment(Modern_DefDamage, 1);
	TextDrawColour(Modern_DefDamage, -1);
	TextDrawSetShadow(Modern_DefDamage, 0);
	TextDrawSetOutline(Modern_DefDamage, 0);
	TextDrawBackgroundColour(Modern_DefDamage, 0x000000FF);
	TextDrawFont(Modern_DefDamage, 1);
	TextDrawSetProportional(Modern_DefDamage, true);

	Modern_DefKills = TextDrawCreate(454.993, 136.897, "_");
	TextDrawLetterSize(Modern_DefKills, 0.1905, 0.8665);
	TextDrawTextSize(Modern_DefKills, 946.514, 930.000);
	TextDrawAlignment(Modern_DefKills, 1);
	TextDrawColour(Modern_DefKills, -1);
	TextDrawSetShadow(Modern_DefKills, 0);
	TextDrawSetOutline(Modern_DefKills, 0);
	TextDrawBackgroundColour(Modern_DefKills, 0x000000FF);
	TextDrawFont(Modern_DefKills, 1);
	TextDrawSetProportional(Modern_DefKills, true);

	Modern_DefHealth = TextDrawCreate(477.759, 136.731, "_");
	TextDrawLetterSize(Modern_DefHealth, 0.1905, 0.8665);
	TextDrawTextSize(Modern_DefHealth, 969.281, 930.000);
	TextDrawAlignment(Modern_DefHealth, 1);
	TextDrawColour(Modern_DefHealth, -1);
	TextDrawSetShadow(Modern_DefHealth, 0);
	TextDrawSetOutline(Modern_DefHealth, 0);
	TextDrawBackgroundColour(Modern_DefHealth, 0x000000FF);
	TextDrawFont(Modern_DefHealth, 1);
	TextDrawSetProportional(Modern_DefHealth, true);

	Modern_AttTeamName = TextDrawCreate(231.000, 107.130, "_");
	TextDrawLetterSize(Modern_AttTeamName, 0.3872, 1.6961);
	TextDrawAlignment(Modern_AttTeamName, 2);
	TextDrawColour(Modern_AttTeamName, -1);
	TextDrawSetShadow(Modern_AttTeamName, 0);
	TextDrawSetOutline(Modern_AttTeamName, 0);
	TextDrawBackgroundColour(Modern_AttTeamName, 0x000000FF);
	TextDrawFont(Modern_AttTeamName, 1);
	TextDrawSetProportional(Modern_AttTeamName, true);

	Modern_DefTeamName = TextDrawCreate(409.000, 107.130, "_");
	TextDrawLetterSize(Modern_DefTeamName, 0.3872, 1.6961);
	TextDrawAlignment(Modern_DefTeamName, 2);
	TextDrawColour(Modern_DefTeamName, -1);
	TextDrawSetShadow(Modern_DefTeamName, 0);
	TextDrawSetOutline(Modern_DefTeamName, 0);
	TextDrawBackgroundColour(Modern_DefTeamName, 0x000000FF);
	TextDrawFont(Modern_DefTeamName, 1);
	TextDrawSetProportional(Modern_DefTeamName, true);
	return 1;
}

public UpdateModernEndRoundStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, winnerid, WinID, Float: ahpleft, Float: dhpleft, aalive, dalive, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc, const attList[], const attDmg[], const attKills[], const attHP[], const defList[], const defDmg[], const defKills[], const defHP[])
{
	TextDrawSetString(Modern_AttTeamName, TeamName[ATTACKER][0] ? TeamName[ATTACKER] : "Attacker");
	TextDrawSetString(Modern_DefTeamName, TeamName[DEFENDER][0] ? TeamName[DEFENDER] : "Defender");

	TextDrawSetString(Modern_AttHeaderName, "Name");
	TextDrawSetString(Modern_AttHeaderDmg, "Dmg");
	TextDrawSetString(Modern_AttHeaderKills, "Kills");
	TextDrawSetString(Modern_AttHeaderHealth, "Health");

	TextDrawSetString(Modern_DefHeaderName, "Name");
	TextDrawSetString(Modern_DefHeaderDmg, "Dmg");
	TextDrawSetString(Modern_DefHeaderKills, "Kills");
	TextDrawSetString(Modern_DefHeaderHealth, "Health");

	TextDrawSetString(Modern_AttNames, attList[0] ? attList : " ");
	TextDrawSetString(Modern_AttDamage, attDmg[0] ? attDmg : " ");
	TextDrawSetString(Modern_AttKills, attKills[0] ? attKills : " ");
	TextDrawSetString(Modern_AttHealth, attHP[0] ? attHP : " ");

	TextDrawSetString(Modern_DefNames, defList[0] ? defList : " ");
	TextDrawSetString(Modern_DefDamage, defDmg[0] ? defDmg : " ");
	TextDrawSetString(Modern_DefKills, defKills[0] ? defKills : " ");
	TextDrawSetString(Modern_DefHealth, defHP[0] ? defHP : " ");

	if (winnerid == ATTACKER)
	{
		va_TextDrawSetString(Modern_TeamWhoWon, "%s%s ~w~have won", TDC[ATTACKER], TeamName[ATTACKER]);
	}
	else if (winnerid == DEFENDER)
	{
		va_TextDrawSetString(Modern_TeamWhoWon, "%s%s ~w~have won", TDC[DEFENDER], TeamName[DEFENDER]);
	}
	else
	{
		TextDrawSetString(Modern_TeamWhoWon, "~w~Round Draw");
	}

	if (topDmgID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topDmgID][player_Team] == ATTACKER || array[topDmgID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		va_TextDrawSetString(Modern_MVPDamage, "~w~Most damage: %s%s ~w~(%.0f)", teamCol, names[topDmgID], topDmg);
	}
	else
	{
		TextDrawSetString(Modern_MVPDamage, "~w~Most damage: None");
	}

	new leastDmgID = -1, leastDmgVal = 999999;
	for (new i = 0; i < p; i++)
	{
		if (array[i][player_Deaths] == 0 && array[i][player_HP] > 0)
		{
			new dmgTaken = 200 - array[i][player_HP];
			if (dmgTaken < leastDmgVal)
			{
				leastDmgVal = dmgTaken;
				leastDmgID = i;
			}
		}
	}
	if (leastDmgID >= 0)
	{
		new teamCol[8];
		teamCol = (array[leastDmgID][player_Team] == ATTACKER || array[leastDmgID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		va_TextDrawSetString(Modern_MVPLeastDmg, "~w~Least damaged: %s%s ~w~(%d)", teamCol, names[leastDmgID], leastDmgVal);
	}
	else if (topAccID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topAccID][player_Team] == ATTACKER || array[topAccID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		va_TextDrawSetString(Modern_MVPLeastDmg, "~w~Top accuracy: %s%s ~w~(%d%%)", teamCol, names[topAccID], topAcc);
	}
	else
	{
		TextDrawSetString(Modern_MVPLeastDmg, "~w~Least damaged: None");
	}

	if (topkillID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topkillID][player_Team] == ATTACKER || array[topkillID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		va_TextDrawSetString(Modern_MVPKills, "~w~Most kills: %s%s ~w~(%d)", teamCol, names[topkillID], topkill);
	}
	else
	{
		TextDrawSetString(Modern_MVPKills, "~w~Most kills: None");
	}

	new winReason[32];
	switch (WinID)
	{
		case 0, 5, 6: winReason = "Checkpoint win";
		case 1: winReason = "Round time expired";
		case 2, 3: winReason = "Elimination win";
		case 4: winReason = "Round draw";
		default: winReason = "Round finished";
	}
	va_TextDrawSetString(Modern_RoundInfo[0], "%s (id %d) - %s", (GameType == BASE ? "Base" : "Arena"), ServerLastPlayed, winReason);

	new elapsed = (ConfigRoundTime * 60) - RoundSeconds;
	if (elapsed < 0) elapsed = 0;
	va_TextDrawSetString(Modern_RoundInfo[1], "Time elapsed: %d:%02d", elapsed / 60, elapsed % 60);

	new tLeft = RoundSeconds;
	if (tLeft < 0) tLeft = 0;
	va_TextDrawSetString(Modern_RoundInfo[2], "Time left: %d:%02d", tLeft / 60, tLeft % 60);
	return 1;
}

public UpdateModernMatchStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, matchWinnerID, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc, const attList[], const attDmg[], const attKills[], const attDeaths[], const defList[], const defDmg[], const defKills[], const defDeaths[])
{
	TextDrawSetString(Modern_AttTeamName, TeamName[ATTACKER][0] ? TeamName[ATTACKER] : "Attacker");
	TextDrawSetString(Modern_DefTeamName, TeamName[DEFENDER][0] ? TeamName[DEFENDER] : "Defender");

	TextDrawSetString(Modern_AttHeaderName, "Name");
	TextDrawSetString(Modern_AttHeaderDmg, "Dmg");
	TextDrawSetString(Modern_AttHeaderKills, "Kills");
	TextDrawSetString(Modern_AttHeaderHealth, "Deaths");

	TextDrawSetString(Modern_DefHeaderName, "Name");
	TextDrawSetString(Modern_DefHeaderDmg, "Dmg");
	TextDrawSetString(Modern_DefHeaderKills, "Kills");
	TextDrawSetString(Modern_DefHeaderHealth, "Deaths");

	TextDrawSetString(Modern_AttNames, attList[0] ? attList : " ");
	TextDrawSetString(Modern_AttDamage, attDmg[0] ? attDmg : " ");
	TextDrawSetString(Modern_AttKills, attKills[0] ? attKills : " ");
	TextDrawSetString(Modern_AttHealth, attDeaths[0] ? attDeaths : " ");

	TextDrawSetString(Modern_DefNames, defList[0] ? defList : " ");
	TextDrawSetString(Modern_DefDamage, defDmg[0] ? defDmg : " ");
	TextDrawSetString(Modern_DefKills, defKills[0] ? defKills : " ");
	TextDrawSetString(Modern_DefHealth, defDeaths[0] ? defDeaths : " ");

	if (matchWinnerID == ATTACKER)
	{
		va_TextDrawSetString(Modern_TeamWhoWon, "%s%s ~w~won the match", TDC[ATTACKER], TeamName[ATTACKER]);
	}
	else if (matchWinnerID == DEFENDER)
	{
		va_TextDrawSetString(Modern_TeamWhoWon, "%s%s ~w~won the match", TDC[DEFENDER], TeamName[DEFENDER]);
	}
	else
	{
		TextDrawSetString(Modern_TeamWhoWon, "~w~Match Draw");
	}

	if (topDmgID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topDmgID][player_Team] == ATTACKER || array[topDmgID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		va_TextDrawSetString(Modern_MVPDamage, "~w~Most damage: %s%s ~w~(%.0f)", teamCol, names[topDmgID], topDmg);
	}
	else
	{
		TextDrawSetString(Modern_MVPDamage, "~w~Most damage: None");
	}

	if (topAccID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topAccID][player_Team] == ATTACKER || array[topAccID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		va_TextDrawSetString(Modern_MVPLeastDmg, "~w~Top accuracy: %s%s ~w~(%d%%)", teamCol, names[topAccID], topAcc);
	}
	else
	{
		TextDrawSetString(Modern_MVPLeastDmg, "~w~Top accuracy: None");
	}

	if (topkillID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topkillID][player_Team] == ATTACKER || array[topkillID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		va_TextDrawSetString(Modern_MVPKills, "~w~Most kills: %s%s ~w~(%d)", teamCol, names[topkillID], topkill);
	}
	else
	{
		TextDrawSetString(Modern_MVPKills, "~w~Most kills: None");
	}

	va_TextDrawSetString(Modern_RoundInfo[0], "Final Score: %d - %d", TeamScore[ATTACKER], TeamScore[DEFENDER]);
	va_TextDrawSetString(Modern_RoundInfo[1], "Rounds played: %d", CurrentRound);
	TextDrawSetString(Modern_RoundInfo[2], "Match completed");
	return 1;
}

public HPBars_ResultTextdrawsCreate()
{
	HPBars_TopBox = TextDrawCreate(126.891, 133.959, "_");
	TextDrawLetterSize(HPBars_TopBox, 0.0863, 2.9063);
	TextDrawTextSize(HPBars_TopBox, 476.783, 574.000);
	TextDrawAlignment(HPBars_TopBox, 1);
	TextDrawColor(HPBars_TopBox, -1);
	TextDrawUseBox(HPBars_TopBox, 1);
	TextDrawBoxColor(HPBars_TopBox, 0x000000A3);
	TextDrawSetShadow(HPBars_TopBox, 1);
	TextDrawSetOutline(HPBars_TopBox, 0);
	TextDrawBackgroundColor(HPBars_TopBox, 0x000000FF);
	TextDrawFont(HPBars_TopBox, 1);
	TextDrawSetProportional(HPBars_TopBox, 1);

	HPBars_TopBanner = TextDrawCreate(301.837, 139.500, "_");
	TextDrawLetterSize(HPBars_TopBanner, 0.2709, 1.4597);
	TextDrawAlignment(HPBars_TopBanner, 2);
	TextDrawColor(HPBars_TopBanner, -1);
	TextDrawSetShadow(HPBars_TopBanner, 0);
	TextDrawSetOutline(HPBars_TopBanner, 1);
	TextDrawBackgroundColor(HPBars_TopBanner, 0x000000FF);
	TextDrawFont(HPBars_TopBanner, 1);
	TextDrawSetProportional(HPBars_TopBanner, 1);

	HPBars_Blackboard = TextDrawCreate(126.961, 186.237, "_");
	TextDrawLetterSize(HPBars_Blackboard, 0.2766, 17.3265);
	TextDrawTextSize(HPBars_Blackboard, 476.874, 930.000);
	TextDrawAlignment(HPBars_Blackboard, 1);
	TextDrawColor(HPBars_Blackboard, -1);
	TextDrawUseBox(HPBars_Blackboard, 1);
	TextDrawBoxColor(HPBars_Blackboard, 0x000000CC);
	TextDrawSetShadow(HPBars_Blackboard, 1);
	TextDrawSetOutline(HPBars_Blackboard, 0);
	TextDrawBackgroundColor(HPBars_Blackboard, 0x000000FF);
	TextDrawFont(HPBars_Blackboard, 1);
	TextDrawSetProportional(HPBars_Blackboard, 1);

	HPBars_AttBox = TextDrawCreate(126.888, 170.441, "_");
	TextDrawLetterSize(HPBars_AttBox, 0.2597, 1.4047);
	TextDrawTextSize(HPBars_AttBox, 301.890, 930.000);
	TextDrawAlignment(HPBars_AttBox, 1);
	TextDrawColor(HPBars_AttBox, -1);
	TextDrawUseBox(HPBars_AttBox, 1);
	TextDrawBoxColor(HPBars_AttBox, 0x043106FF);
	TextDrawSetShadow(HPBars_AttBox, 1);
	TextDrawSetOutline(HPBars_AttBox, 0);
	TextDrawBackgroundColor(HPBars_AttBox, 0x000000FF);
	TextDrawFont(HPBars_AttBox, 1);
	TextDrawSetProportional(HPBars_AttBox, 1);

	HPBars_DefBox = TextDrawCreate(301.890, 170.441, "_");
	TextDrawLetterSize(HPBars_DefBox, 0.2597, 1.4047);
	TextDrawTextSize(HPBars_DefBox, 476.892, 930.000);
	TextDrawAlignment(HPBars_DefBox, 1);
	TextDrawColor(HPBars_DefBox, -1);
	TextDrawUseBox(HPBars_DefBox, 1);
	TextDrawBoxColor(HPBars_DefBox, 0x0E133DFF);
	TextDrawSetShadow(HPBars_DefBox, 1);
	TextDrawSetOutline(HPBars_DefBox, 0);
	TextDrawBackgroundColor(HPBars_DefBox, 0x000000FF);
	TextDrawFont(HPBars_DefBox, 1);
	TextDrawSetProportional(HPBars_DefBox, 1);

	HPBars_HeaderShadow = TextDrawCreate(126.961, 170.386, "_");
	TextDrawLetterSize(HPBars_HeaderShadow, 0.0863, 0.6952);
	TextDrawTextSize(HPBars_HeaderShadow, 476.921, 574.000);
	TextDrawAlignment(HPBars_HeaderShadow, 1);
	TextDrawColor(HPBars_HeaderShadow, -1);
	TextDrawUseBox(HPBars_HeaderShadow, 1);
	TextDrawBoxColor(HPBars_HeaderShadow, 0x00000064);
	TextDrawSetShadow(HPBars_HeaderShadow, 1);
	TextDrawSetOutline(HPBars_HeaderShadow, 0);
	TextDrawBackgroundColor(HPBars_HeaderShadow, 0x000000FF);
	TextDrawFont(HPBars_HeaderShadow, 1);
	TextDrawSetProportional(HPBars_HeaderShadow, 1);

	HPBars_LeftHeader = TextDrawCreate(132.128, 173.000, "Name                   Kill   HP   Acc  Dmg");
	TextDrawLetterSize(HPBars_LeftHeader, 0.2149, 1.0419);
	TextDrawTextSize(HPBars_LeftHeader, 643.975, 930.000);
	TextDrawAlignment(HPBars_LeftHeader, 1);
	TextDrawColor(HPBars_LeftHeader, -1);
	TextDrawSetShadow(HPBars_LeftHeader, 0);
	TextDrawSetOutline(HPBars_LeftHeader, 1);
	TextDrawBackgroundColor(HPBars_LeftHeader, 0x000000FF);
	TextDrawFont(HPBars_LeftHeader, 1);
	TextDrawSetProportional(HPBars_LeftHeader, 1);

	HPBars_RightHeader = TextDrawCreate(310.456, 173.000, "Name                   Kill   HP   Acc  Dmg");
	TextDrawLetterSize(HPBars_RightHeader, 0.2149, 1.0419);
	TextDrawTextSize(HPBars_RightHeader, 822.269, 930.000);
	TextDrawAlignment(HPBars_RightHeader, 1);
	TextDrawColor(HPBars_RightHeader, -1);
	TextDrawSetShadow(HPBars_RightHeader, 0);
	TextDrawSetOutline(HPBars_RightHeader, 1);
	TextDrawBackgroundColor(HPBars_RightHeader, 0x000000FF);
	TextDrawFont(HPBars_RightHeader, 1);
	TextDrawSetProportional(HPBars_RightHeader, 1);

	HPBars_BottomBox = TextDrawCreate(197.604, 350.216, "_");
	TextDrawLetterSize(HPBars_BottomBox, 0.0863, 3.4779);
	TextDrawTextSize(HPBars_BottomBox, 409.730, 574.000);
	TextDrawAlignment(HPBars_BottomBox, 1);
	TextDrawColor(HPBars_BottomBox, -1);
	TextDrawUseBox(HPBars_BottomBox, 1);
	TextDrawBoxColor(HPBars_BottomBox, 0x000000CC);
	TextDrawSetShadow(HPBars_BottomBox, 1);
	TextDrawSetOutline(HPBars_BottomBox, 0);
	TextDrawBackgroundColor(HPBars_BottomBox, 0x000000FF);
	TextDrawFont(HPBars_BottomBox, 1);
	TextDrawSetProportional(HPBars_BottomBox, 1);

	HPBars_MVPKills = TextDrawCreate(303.667, 350.861, "_");
	TextDrawLetterSize(HPBars_MVPKills, 0.2182, 1.0140);
	TextDrawAlignment(HPBars_MVPKills, 2);
	TextDrawColor(HPBars_MVPKills, -1);
	TextDrawSetShadow(HPBars_MVPKills, 0);
	TextDrawSetOutline(HPBars_MVPKills, 1);
	TextDrawBackgroundColor(HPBars_MVPKills, 0x000000FF);
	TextDrawFont(HPBars_MVPKills, 1);
	TextDrawSetProportional(HPBars_MVPKills, 1);

	HPBars_MVPDamage = TextDrawCreate(303.667, 360.230, "_");
	TextDrawLetterSize(HPBars_MVPDamage, 0.2090, 0.9587);
	TextDrawAlignment(HPBars_MVPDamage, 2);
	TextDrawColor(HPBars_MVPDamage, -1);
	TextDrawSetShadow(HPBars_MVPDamage, 0);
	TextDrawSetOutline(HPBars_MVPDamage, 1);
	TextDrawBackgroundColor(HPBars_MVPDamage, 0x000000FF);
	TextDrawFont(HPBars_MVPDamage, 1);
	TextDrawSetProportional(HPBars_MVPDamage, 1);

	HPBars_MVPAcc = TextDrawCreate(303.667, 370.007, "_");
	TextDrawLetterSize(HPBars_MVPAcc, 0.2090, 0.9587);
	TextDrawAlignment(HPBars_MVPAcc, 2);
	TextDrawColor(HPBars_MVPAcc, -1);
	TextDrawSetShadow(HPBars_MVPAcc, 0);
	TextDrawSetOutline(HPBars_MVPAcc, 1);
	TextDrawBackgroundColor(HPBars_MVPAcc, 0x000000FF);
	TextDrawFont(HPBars_MVPAcc, 1);
	TextDrawSetProportional(HPBars_MVPAcc, 1);

	// Player Stats Columns
	// Left Names
	HPBars_LeftNames = TextDrawCreate(132.202, 187.000, "_");
	TextDrawLetterSize(HPBars_LeftNames, 0.2247, 0.9641);
	TextDrawTextSize(HPBars_LeftNames, 1279.711, 1280.000);
	TextDrawAlignment(HPBars_LeftNames, 1);
	TextDrawColor(HPBars_LeftNames, -1);
	TextDrawSetShadow(HPBars_LeftNames, 1);
	TextDrawSetOutline(HPBars_LeftNames, 0);
	TextDrawBackgroundColor(HPBars_LeftNames, 0x000000FF);
	TextDrawFont(HPBars_LeftNames, 1);
	TextDrawSetProportional(HPBars_LeftNames, 1);

	// Left Round Columns
	HPBars_RoundLeftKills = TextDrawCreate(222.867, 187.000, "_");
	TextDrawLetterSize(HPBars_RoundLeftKills, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_RoundLeftKills, 2);
	TextDrawColor(HPBars_RoundLeftKills, -1);
	TextDrawSetShadow(HPBars_RoundLeftKills, 1);
	TextDrawSetOutline(HPBars_RoundLeftKills, 0);
	TextDrawBackgroundColor(HPBars_RoundLeftKills, 0x000000FF);
	TextDrawFont(HPBars_RoundLeftKills, 1);
	TextDrawSetProportional(HPBars_RoundLeftKills, 1);

	HPBars_RoundLeftHP = TextDrawCreate(243.575, 187.000, "_");
	TextDrawLetterSize(HPBars_RoundLeftHP, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_RoundLeftHP, 2);
	TextDrawColor(HPBars_RoundLeftHP, -1);
	TextDrawSetShadow(HPBars_RoundLeftHP, 1);
	TextDrawSetOutline(HPBars_RoundLeftHP, 0);
	TextDrawBackgroundColor(HPBars_RoundLeftHP, 0x000000FF);
	TextDrawFont(HPBars_RoundLeftHP, 1);
	TextDrawSetProportional(HPBars_RoundLeftHP, 1);

	HPBars_RoundLeftAcc = TextDrawCreate(265.641, 187.000, "_");
	TextDrawLetterSize(HPBars_RoundLeftAcc, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_RoundLeftAcc, 2);
	TextDrawColor(HPBars_RoundLeftAcc, -1);
	TextDrawSetShadow(HPBars_RoundLeftAcc, 1);
	TextDrawSetOutline(HPBars_RoundLeftAcc, 0);
	TextDrawBackgroundColor(HPBars_RoundLeftAcc, 0x000000FF);
	TextDrawFont(HPBars_RoundLeftAcc, 1);
	TextDrawSetProportional(HPBars_RoundLeftAcc, 1);

	HPBars_RoundLeftDmg = TextDrawCreate(285.630, 187.000, "_");
	TextDrawLetterSize(HPBars_RoundLeftDmg, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_RoundLeftDmg, 2);
	TextDrawColor(HPBars_RoundLeftDmg, -1);
	TextDrawSetShadow(HPBars_RoundLeftDmg, 1);
	TextDrawSetOutline(HPBars_RoundLeftDmg, 0);
	TextDrawBackgroundColor(HPBars_RoundLeftDmg, 0x000000FF);
	TextDrawFont(HPBars_RoundLeftDmg, 1);
	TextDrawSetProportional(HPBars_RoundLeftDmg, 1);

	// Left Match Columns
	HPBars_MatchLeftK = TextDrawCreate(215.000, 186.798, "_");
	TextDrawLetterSize(HPBars_MatchLeftK, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_MatchLeftK, 2);
	TextDrawColor(HPBars_MatchLeftK, -1);
	TextDrawSetShadow(HPBars_MatchLeftK, 1);
	TextDrawSetOutline(HPBars_MatchLeftK, 0);
	TextDrawBackgroundColor(HPBars_MatchLeftK, 0x000000FF);
	TextDrawFont(HPBars_MatchLeftK, 1);
	TextDrawSetProportional(HPBars_MatchLeftK, 1);

	HPBars_MatchLeftD = TextDrawCreate(229.000, 186.798, "_");
	TextDrawLetterSize(HPBars_MatchLeftD, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_MatchLeftD, 2);
	TextDrawColor(HPBars_MatchLeftD, -1);
	TextDrawSetShadow(HPBars_MatchLeftD, 1);
	TextDrawSetOutline(HPBars_MatchLeftD, 0);
	TextDrawBackgroundColor(HPBars_MatchLeftD, 0x000000FF);
	TextDrawFont(HPBars_MatchLeftD, 1);
	TextDrawSetProportional(HPBars_MatchLeftD, 1);

	HPBars_MatchLeftP = TextDrawCreate(243.000, 186.798, "_");
	TextDrawLetterSize(HPBars_MatchLeftP, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_MatchLeftP, 2);
	TextDrawColor(HPBars_MatchLeftP, -1);
	TextDrawSetShadow(HPBars_MatchLeftP, 1);
	TextDrawSetOutline(HPBars_MatchLeftP, 0);
	TextDrawBackgroundColor(HPBars_MatchLeftP, 0x000000FF);
	TextDrawFont(HPBars_MatchLeftP, 1);
	TextDrawSetProportional(HPBars_MatchLeftP, 1);

	HPBars_MatchLeftAcc = TextDrawCreate(260.000, 186.798, "_");
	TextDrawLetterSize(HPBars_MatchLeftAcc, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_MatchLeftAcc, 2);
	TextDrawColor(HPBars_MatchLeftAcc, -1);
	TextDrawSetShadow(HPBars_MatchLeftAcc, 1);
	TextDrawSetOutline(HPBars_MatchLeftAcc, 0);
	TextDrawBackgroundColor(HPBars_MatchLeftAcc, 0x000000FF);
	TextDrawFont(HPBars_MatchLeftAcc, 1);
	TextDrawSetProportional(HPBars_MatchLeftAcc, 1);

	HPBars_MatchLeftDmg = TextDrawCreate(283.000, 186.798, "_");
	TextDrawLetterSize(HPBars_MatchLeftDmg, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_MatchLeftDmg, 2);
	TextDrawColor(HPBars_MatchLeftDmg, -1);
	TextDrawSetShadow(HPBars_MatchLeftDmg, 1);
	TextDrawSetOutline(HPBars_MatchLeftDmg, 0);
	TextDrawBackgroundColor(HPBars_MatchLeftDmg, 0x000000FF);
	TextDrawFont(HPBars_MatchLeftDmg, 1);
	TextDrawSetProportional(HPBars_MatchLeftDmg, 1);

	// Right Names
	HPBars_RightNames = TextDrawCreate(310.663, 187.000, "_");
	TextDrawLetterSize(HPBars_RightNames, 0.2247, 0.9641);
	TextDrawTextSize(HPBars_RightNames, 1458.139, 1280.000);
	TextDrawAlignment(HPBars_RightNames, 1);
	TextDrawColor(HPBars_RightNames, -1);
	TextDrawSetShadow(HPBars_RightNames, 1);
	TextDrawSetOutline(HPBars_RightNames, 0);
	TextDrawBackgroundColor(HPBars_RightNames, 0x000000FF);
	TextDrawFont(HPBars_RightNames, 1);
	TextDrawSetProportional(HPBars_RightNames, 1);

	// Right Round Columns
	HPBars_RoundRightKills = TextDrawCreate(401.194, 187.000, "_");
	TextDrawLetterSize(HPBars_RoundRightKills, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_RoundRightKills, 2);
	TextDrawColor(HPBars_RoundRightKills, -1);
	TextDrawSetShadow(HPBars_RoundRightKills, 1);
	TextDrawSetOutline(HPBars_RoundRightKills, 0);
	TextDrawBackgroundColor(HPBars_RoundRightKills, 0x000000FF);
	TextDrawFont(HPBars_RoundRightKills, 1);
	TextDrawSetProportional(HPBars_RoundRightKills, 1);

	HPBars_RoundRightHP = TextDrawCreate(421.769, 187.000, "_");
	TextDrawLetterSize(HPBars_RoundRightHP, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_RoundRightHP, 2);
	TextDrawColor(HPBars_RoundRightHP, -1);
	TextDrawSetShadow(HPBars_RoundRightHP, 1);
	TextDrawSetOutline(HPBars_RoundRightHP, 0);
	TextDrawBackgroundColor(HPBars_RoundRightHP, 0x000000FF);
	TextDrawFont(HPBars_RoundRightHP, 1);
	TextDrawSetProportional(HPBars_RoundRightHP, 1);

	HPBars_RoundRightAcc = TextDrawCreate(443.902, 187.000, "_");
	TextDrawLetterSize(HPBars_RoundRightAcc, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_RoundRightAcc, 2);
	TextDrawColor(HPBars_RoundRightAcc, -1);
	TextDrawSetShadow(HPBars_RoundRightAcc, 1);
	TextDrawSetOutline(HPBars_RoundRightAcc, 0);
	TextDrawBackgroundColor(HPBars_RoundRightAcc, 0x000000FF);
	TextDrawFont(HPBars_RoundRightAcc, 1);
	TextDrawSetProportional(HPBars_RoundRightAcc, 1);

	HPBars_RoundRightDmg = TextDrawCreate(463.958, 187.000, "_");
	TextDrawLetterSize(HPBars_RoundRightDmg, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_RoundRightDmg, 2);
	TextDrawColor(HPBars_RoundRightDmg, -1);
	TextDrawSetShadow(HPBars_RoundRightDmg, 1);
	TextDrawSetOutline(HPBars_RoundRightDmg, 0);
	TextDrawBackgroundColor(HPBars_RoundRightDmg, 0x000000FF);
	TextDrawFont(HPBars_RoundRightDmg, 1);
	TextDrawSetProportional(HPBars_RoundRightDmg, 1);

	// Right Match Columns
	HPBars_MatchRightK = TextDrawCreate(391.000, 186.798, "_");
	TextDrawLetterSize(HPBars_MatchRightK, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_MatchRightK, 2);
	TextDrawColor(HPBars_MatchRightK, -1);
	TextDrawSetShadow(HPBars_MatchRightK, 1);
	TextDrawSetOutline(HPBars_MatchRightK, 0);
	TextDrawBackgroundColor(HPBars_MatchRightK, 0x000000FF);
	TextDrawFont(HPBars_MatchRightK, 1);
	TextDrawSetProportional(HPBars_MatchRightK, 1);

	HPBars_MatchRightD = TextDrawCreate(405.000, 186.798, "_");
	TextDrawLetterSize(HPBars_MatchRightD, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_MatchRightD, 2);
	TextDrawColor(HPBars_MatchRightD, -1);
	TextDrawSetShadow(HPBars_MatchRightD, 1);
	TextDrawSetOutline(HPBars_MatchRightD, 0);
	TextDrawBackgroundColor(HPBars_MatchRightD, 0x000000FF);
	TextDrawFont(HPBars_MatchRightD, 1);
	TextDrawSetProportional(HPBars_MatchRightD, 1);

	HPBars_MatchRightP = TextDrawCreate(419.000, 186.798, "_");
	TextDrawLetterSize(HPBars_MatchRightP, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_MatchRightP, 2);
	TextDrawColor(HPBars_MatchRightP, -1);
	TextDrawSetShadow(HPBars_MatchRightP, 1);
	TextDrawSetOutline(HPBars_MatchRightP, 0);
	TextDrawBackgroundColor(HPBars_MatchRightP, 0x000000FF);
	TextDrawFont(HPBars_MatchRightP, 1);
	TextDrawSetProportional(HPBars_MatchRightP, 1);

	HPBars_MatchRightAcc = TextDrawCreate(436.000, 186.798, "_");
	TextDrawLetterSize(HPBars_MatchRightAcc, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_MatchRightAcc, 2);
	TextDrawColor(HPBars_MatchRightAcc, -1);
	TextDrawSetShadow(HPBars_MatchRightAcc, 1);
	TextDrawSetOutline(HPBars_MatchRightAcc, 0);
	TextDrawBackgroundColor(HPBars_MatchRightAcc, 0x000000FF);
	TextDrawFont(HPBars_MatchRightAcc, 1);
	TextDrawSetProportional(HPBars_MatchRightAcc, 1);

	HPBars_MatchRightDmg = TextDrawCreate(459.000, 186.798, "_");
	TextDrawLetterSize(HPBars_MatchRightDmg, 0.2247, 0.9641);
	TextDrawAlignment(HPBars_MatchRightDmg, 2);
	TextDrawColor(HPBars_MatchRightDmg, -1);
	TextDrawSetShadow(HPBars_MatchRightDmg, 1);
	TextDrawSetOutline(HPBars_MatchRightDmg, 0);
	TextDrawBackgroundColor(HPBars_MatchRightDmg, 0x000000FF);
	TextDrawFont(HPBars_MatchRightDmg, 1);
	TextDrawSetProportional(HPBars_MatchRightDmg, 1);

	// Match History Panel (Right)
	HPBars_HistBox = TextDrawCreate(485.755, 186.403, "_");
	TextDrawLetterSize(HPBars_HistBox, 0.0863, 17.3300);
	TextDrawTextSize(HPBars_HistBox, 629.839, 574.000);
	TextDrawAlignment(HPBars_HistBox, 1);
	TextDrawColor(HPBars_HistBox, -1);
	TextDrawUseBox(HPBars_HistBox, 1);
	TextDrawBoxColor(HPBars_HistBox, 0x000000CC);
	TextDrawSetShadow(HPBars_HistBox, 1);
	TextDrawSetOutline(HPBars_HistBox, 0);
	TextDrawBackgroundColor(HPBars_HistBox, 0x000000FF);
	TextDrawFont(HPBars_HistBox, 1);
	TextDrawSetProportional(HPBars_HistBox, 1);

	HPBars_HistHeaderBox = TextDrawCreate(485.917, 170.383, "_");
	TextDrawLetterSize(HPBars_HistHeaderBox, 0.2597, 1.3970);
	TextDrawTextSize(HPBars_HistHeaderBox, 629.706, 930.000);
	TextDrawAlignment(HPBars_HistHeaderBox, 1);
	TextDrawColor(HPBars_HistHeaderBox, -1);
	TextDrawUseBox(HPBars_HistHeaderBox, 1);
	TextDrawBoxColor(HPBars_HistHeaderBox, 0x505400FF);
	TextDrawSetShadow(HPBars_HistHeaderBox, 1);
	TextDrawSetOutline(HPBars_HistHeaderBox, 0);
	TextDrawBackgroundColor(HPBars_HistHeaderBox, 0x000000FF);
	TextDrawFont(HPBars_HistHeaderBox, 1);
	TextDrawSetProportional(HPBars_HistHeaderBox, 1);

	HPBars_HistHeaderShadow = TextDrawCreate(485.917, 169.711, "_");
	TextDrawLetterSize(HPBars_HistHeaderShadow, 0.0863, 0.7778);
	TextDrawTextSize(HPBars_HistHeaderShadow, 629.829, 574.000);
	TextDrawAlignment(HPBars_HistHeaderShadow, 1);
	TextDrawColor(HPBars_HistHeaderShadow, -1);
	TextDrawUseBox(HPBars_HistHeaderShadow, 1);
	TextDrawBoxColor(HPBars_HistHeaderShadow, 0x00000064);
	TextDrawSetShadow(HPBars_HistHeaderShadow, 1);
	TextDrawSetOutline(HPBars_HistHeaderShadow, 0);
	TextDrawBackgroundColor(HPBars_HistHeaderShadow, 0x000000FF);
	TextDrawFont(HPBars_HistHeaderShadow, 1);
	TextDrawSetProportional(HPBars_HistHeaderShadow, 1);

	HPBars_HistHeader = TextDrawCreate(490.365, 172.068, "Round        Map               Winner");
	TextDrawLetterSize(HPBars_HistHeader, 0.2031, 1.0788);
	TextDrawTextSize(HPBars_HistHeader, 1002.212, 930.000);
	TextDrawAlignment(HPBars_HistHeader, 1);
	TextDrawColor(HPBars_HistHeader, -1);
	TextDrawSetShadow(HPBars_HistHeader, 0);
	TextDrawSetOutline(HPBars_HistHeader, 1);
	TextDrawBackgroundColor(HPBars_HistHeader, 0x000000FF);
	TextDrawFont(HPBars_HistHeader, 1);
	TextDrawSetProportional(HPBars_HistHeader, 1);

	HPBars_HistRound = TextDrawCreate(492.117, 186.456, "_");
	TextDrawLetterSize(HPBars_HistRound, 0.2101, 1.0788);
	TextDrawAlignment(HPBars_HistRound, 1);
	TextDrawColor(HPBars_HistRound, -1);
	TextDrawSetShadow(HPBars_HistRound, 1);
	TextDrawSetOutline(HPBars_HistRound, 0);
	TextDrawBackgroundColor(HPBars_HistRound, 0x000000FF);
	TextDrawFont(HPBars_HistRound, 1);
	TextDrawSetProportional(HPBars_HistRound, 1);

	HPBars_HistMap = TextDrawCreate(535.000, 186.456, "_");
	TextDrawLetterSize(HPBars_HistMap, 0.2101, 1.0788);
	TextDrawAlignment(HPBars_HistMap, 1);
	TextDrawColor(HPBars_HistMap, -1);
	TextDrawSetShadow(HPBars_HistMap, 1);
	TextDrawSetOutline(HPBars_HistMap, 0);
	TextDrawBackgroundColor(HPBars_HistMap, 0x000000FF);
	TextDrawFont(HPBars_HistMap, 1);
	TextDrawSetProportional(HPBars_HistMap, 1);

	HPBars_HistWinner = TextDrawCreate(595.000, 186.456, "_");
	TextDrawLetterSize(HPBars_HistWinner, 0.2101, 1.0788);
	TextDrawAlignment(HPBars_HistWinner, 1);
	TextDrawColor(HPBars_HistWinner, -1);
	TextDrawSetShadow(HPBars_HistWinner, 1);
	TextDrawSetOutline(HPBars_HistWinner, 0);
	TextDrawBackgroundColor(HPBars_HistWinner, 0x000000FF);
	TextDrawFont(HPBars_HistWinner, 1);
	TextDrawSetProportional(HPBars_HistWinner, 1);
	return 1;
}

public ShowHPBarsEndRoundTextDraw(playerid)
{
	TextDrawShowForPlayer(playerid, HPBars_TopBox);
	TextDrawShowForPlayer(playerid, HPBars_TopBanner);
	TextDrawShowForPlayer(playerid, HPBars_Blackboard);
	TextDrawShowForPlayer(playerid, HPBars_AttBox);
	TextDrawShowForPlayer(playerid, HPBars_DefBox);
	TextDrawShowForPlayer(playerid, HPBars_HeaderShadow);
	TextDrawShowForPlayer(playerid, HPBars_LeftHeader);
	TextDrawShowForPlayer(playerid, HPBars_RightHeader);
	TextDrawShowForPlayer(playerid, HPBars_BottomBox);
	TextDrawShowForPlayer(playerid, HPBars_MVPKills);
	TextDrawShowForPlayer(playerid, HPBars_MVPDamage);
	TextDrawShowForPlayer(playerid, HPBars_MVPAcc);
	TextDrawShowForPlayer(playerid, HPBars_LeftNames);
	TextDrawShowForPlayer(playerid, HPBars_RightNames);

	if (!HPBars_IsMatchEnd)
	{
		TextDrawShowForPlayer(playerid, HPBars_RoundLeftKills);
		TextDrawShowForPlayer(playerid, HPBars_RoundLeftHP);
		TextDrawShowForPlayer(playerid, HPBars_RoundLeftAcc);
		TextDrawShowForPlayer(playerid, HPBars_RoundLeftDmg);
		TextDrawShowForPlayer(playerid, HPBars_RoundRightKills);
		TextDrawShowForPlayer(playerid, HPBars_RoundRightHP);
		TextDrawShowForPlayer(playerid, HPBars_RoundRightAcc);
		TextDrawShowForPlayer(playerid, HPBars_RoundRightDmg);

		TextDrawHideForPlayer(playerid, HPBars_MatchLeftK);
		TextDrawHideForPlayer(playerid, HPBars_MatchLeftD);
		TextDrawHideForPlayer(playerid, HPBars_MatchLeftP);
		TextDrawHideForPlayer(playerid, HPBars_MatchLeftAcc);
		TextDrawHideForPlayer(playerid, HPBars_MatchLeftDmg);
		TextDrawHideForPlayer(playerid, HPBars_MatchRightK);
		TextDrawHideForPlayer(playerid, HPBars_MatchRightD);
		TextDrawHideForPlayer(playerid, HPBars_MatchRightP);
		TextDrawHideForPlayer(playerid, HPBars_MatchRightAcc);
		TextDrawHideForPlayer(playerid, HPBars_MatchRightDmg);

		TextDrawHideForPlayer(playerid, HPBars_HistBox);
		TextDrawHideForPlayer(playerid, HPBars_HistHeaderShadow);
		TextDrawHideForPlayer(playerid, HPBars_HistHeaderBox);
		TextDrawHideForPlayer(playerid, HPBars_HistHeader);
		TextDrawHideForPlayer(playerid, HPBars_HistRound);
		TextDrawHideForPlayer(playerid, HPBars_HistMap);
		TextDrawHideForPlayer(playerid, HPBars_HistWinner);
	}
	else
	{
		TextDrawHideForPlayer(playerid, HPBars_RoundLeftKills);
		TextDrawHideForPlayer(playerid, HPBars_RoundLeftHP);
		TextDrawHideForPlayer(playerid, HPBars_RoundLeftAcc);
		TextDrawHideForPlayer(playerid, HPBars_RoundLeftDmg);
		TextDrawHideForPlayer(playerid, HPBars_RoundRightKills);
		TextDrawHideForPlayer(playerid, HPBars_RoundRightHP);
		TextDrawHideForPlayer(playerid, HPBars_RoundRightAcc);
		TextDrawHideForPlayer(playerid, HPBars_RoundRightDmg);

		TextDrawShowForPlayer(playerid, HPBars_MatchLeftK);
		TextDrawShowForPlayer(playerid, HPBars_MatchLeftD);
		TextDrawShowForPlayer(playerid, HPBars_MatchLeftP);
		TextDrawShowForPlayer(playerid, HPBars_MatchLeftAcc);
		TextDrawShowForPlayer(playerid, HPBars_MatchLeftDmg);
		TextDrawShowForPlayer(playerid, HPBars_MatchRightK);
		TextDrawShowForPlayer(playerid, HPBars_MatchRightD);
		TextDrawShowForPlayer(playerid, HPBars_MatchRightP);
		TextDrawShowForPlayer(playerid, HPBars_MatchRightAcc);
		TextDrawShowForPlayer(playerid, HPBars_MatchRightDmg);

		TextDrawShowForPlayer(playerid, HPBars_HistBox);
		TextDrawShowForPlayer(playerid, HPBars_HistHeaderShadow);
		TextDrawShowForPlayer(playerid, HPBars_HistHeaderBox);
		TextDrawShowForPlayer(playerid, HPBars_HistHeader);
		TextDrawShowForPlayer(playerid, HPBars_HistRound);
		TextDrawShowForPlayer(playerid, HPBars_HistMap);
		TextDrawShowForPlayer(playerid, HPBars_HistWinner);
	}
	return 1;
}

public HideHPBarsEndRoundTextDraw(playerid)
{
	TextDrawHideForPlayer(playerid, HPBars_TopBox);
	TextDrawHideForPlayer(playerid, HPBars_TopBanner);
	TextDrawHideForPlayer(playerid, HPBars_Blackboard);
	TextDrawHideForPlayer(playerid, HPBars_AttBox);
	TextDrawHideForPlayer(playerid, HPBars_DefBox);
	TextDrawHideForPlayer(playerid, HPBars_HeaderShadow);
	TextDrawHideForPlayer(playerid, HPBars_LeftHeader);
	TextDrawHideForPlayer(playerid, HPBars_RightHeader);
	TextDrawHideForPlayer(playerid, HPBars_BottomBox);
	TextDrawHideForPlayer(playerid, HPBars_MVPKills);
	TextDrawHideForPlayer(playerid, HPBars_MVPDamage);
	TextDrawHideForPlayer(playerid, HPBars_MVPAcc);
	TextDrawHideForPlayer(playerid, HPBars_LeftNames);
	TextDrawHideForPlayer(playerid, HPBars_RightNames);

	TextDrawHideForPlayer(playerid, HPBars_RoundLeftKills);
	TextDrawHideForPlayer(playerid, HPBars_RoundLeftHP);
	TextDrawHideForPlayer(playerid, HPBars_RoundLeftAcc);
	TextDrawHideForPlayer(playerid, HPBars_RoundLeftDmg);
	TextDrawHideForPlayer(playerid, HPBars_RoundRightKills);
	TextDrawHideForPlayer(playerid, HPBars_RoundRightHP);
	TextDrawHideForPlayer(playerid, HPBars_RoundRightAcc);
	TextDrawHideForPlayer(playerid, HPBars_RoundRightDmg);

	TextDrawHideForPlayer(playerid, HPBars_MatchLeftK);
	TextDrawHideForPlayer(playerid, HPBars_MatchLeftD);
	TextDrawHideForPlayer(playerid, HPBars_MatchLeftP);
	TextDrawHideForPlayer(playerid, HPBars_MatchLeftAcc);
	TextDrawHideForPlayer(playerid, HPBars_MatchLeftDmg);
	TextDrawHideForPlayer(playerid, HPBars_MatchRightK);
	TextDrawHideForPlayer(playerid, HPBars_MatchRightD);
	TextDrawHideForPlayer(playerid, HPBars_MatchRightP);
	TextDrawHideForPlayer(playerid, HPBars_MatchRightAcc);
	TextDrawHideForPlayer(playerid, HPBars_MatchRightDmg);

	TextDrawHideForPlayer(playerid, HPBars_HistBox);
	TextDrawHideForPlayer(playerid, HPBars_HistHeaderShadow);
	TextDrawHideForPlayer(playerid, HPBars_HistHeaderBox);
	TextDrawHideForPlayer(playerid, HPBars_HistHeader);
	TextDrawHideForPlayer(playerid, HPBars_HistRound);
	TextDrawHideForPlayer(playerid, HPBars_HistMap);
	TextDrawHideForPlayer(playerid, HPBars_HistWinner);
	return 1;
}

public UpdateHPBarsEndRoundStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, winnerid, Float: ahpleft, Float: dhpleft, aalive, dalive, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc)
{
	HPBars_IsMatchEnd = false;
	TextDrawSetString(HPBars_LeftHeader, "Name                   Kill   HP   Acc  Dmg");
	TextDrawSetString(HPBars_RightHeader, "Name                   Kill   HP   Acc  Dmg");

	new banner[128];
	if (winnerid == ATTACKER)
	{
		format(banner, sizeof(banner), "%s%s ~w~have won the round with ~r~~h~%.0f health ~w~and ~g~~h~%d ~w~player%s alive",
			TDC[ATTACKER], TeamName[ATTACKER], ahpleft, aalive, aalive == 1 ? "" : "s");
	}
	else if (winnerid == DEFENDER)
	{
		format(banner, sizeof(banner), "%s%s ~w~have won the round with ~r~~h~%.0f health ~w~and ~b~~h~%d ~w~player%s alive",
			TDC[DEFENDER], TeamName[DEFENDER], dhpleft, dalive, dalive == 1 ? "" : "s");
	}
	else
	{
		format(banner, sizeof(banner), "~w~No one won the round");
	}
	TextDrawSetString(HPBars_TopBanner, banner);

	new mvpStr[96];
	if (topkillID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topkillID][player_Team] == ATTACKER || array[topkillID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		format(mvpStr, sizeof(mvpStr), "~w~Most Kills %s%s ~w~(~r~~h~%d~w~)", teamCol, names[topkillID], topkill);
	}
	else
	{
		format(mvpStr, sizeof(mvpStr), "~w~Most Kills None");
	}
	TextDrawSetString(HPBars_MVPKills, mvpStr);

	if (topDmgID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topDmgID][player_Team] == ATTACKER || array[topDmgID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		format(mvpStr, sizeof(mvpStr), "~w~Most Damage %s%s ~w~(~r~~h~%.0f~w~)", teamCol, names[topDmgID], topDmg);
	}
	else
	{
		format(mvpStr, sizeof(mvpStr), "~w~Most Damage None");
	}
	TextDrawSetString(HPBars_MVPDamage, mvpStr);

	if (topAccID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topAccID][player_Team] == ATTACKER || array[topAccID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		format(mvpStr, sizeof(mvpStr), "~w~Top Acc %s%s ~w~(~r~~h~%d%%~w~)", teamCol, names[topAccID], topAcc);
	}
	else
	{
		format(mvpStr, sizeof(mvpStr), "~w~Top Acc None");
	}
	TextDrawSetString(HPBars_MVPAcc, mvpStr);

	new lNames[512], lKills[256], lHP[256], lAcc[256], lDmg[256];
	new rNames[512], rKills[256], rHP[256], rAcc[256], rDmg[256];

	for (new i = 0; i != p; ++i)
	{
		if (array[i][player_Team] == ATTACKER || array[i][player_Team] == ATTACKER_SUB)
		{
			format(lNames, sizeof(lNames), "%s~g~~h~%s~n~", lNames, names[i]);
			format(lKills, sizeof(lKills), "%s~r~~h~%d~n~", lKills, array[i][player_Kills]);
			if (array[i][player_Deaths] > 0)
				format(lHP, sizeof(lHP), "%s~r~~h~Dead~n~", lHP);
			else
				format(lHP, sizeof(lHP), "%s~r~~h~%d~n~", lHP, array[i][player_HP]);
			format(lAcc, sizeof(lAcc), "%s~r~~h~%d%%~n~", lAcc, array[i][player_Acc]);
			format(lDmg, sizeof(lDmg), "%s~r~~h~%d~n~", lDmg, array[i][player_Score]);
		}
		else if (array[i][player_Team] == DEFENDER || array[i][player_Team] == DEFENDER_SUB)
		{
			format(rNames, sizeof(rNames), "%s~b~~h~%s~n~", rNames, names[i]);
			format(rKills, sizeof(rKills), "%s~r~~h~%d~n~", rKills, array[i][player_Kills]);
			if (array[i][player_Deaths] > 0)
				format(rHP, sizeof(rHP), "%s~r~~h~Dead~n~", rHP);
			else
				format(rHP, sizeof(rHP), "%s~r~~h~%d~n~", rHP, array[i][player_HP]);
			format(rAcc, sizeof(rAcc), "%s~r~~h~%d%%~n~", rAcc, array[i][player_Acc]);
			format(rDmg, sizeof(rDmg), "%s~r~~h~%d~n~", rDmg, array[i][player_Score]);
		}
	}

	TextDrawSetString(HPBars_LeftNames, lNames[0] ? lNames : " ");
	TextDrawSetString(HPBars_RoundLeftKills, lKills[0] ? lKills : " ");
	TextDrawSetString(HPBars_RoundLeftHP, lHP[0] ? lHP : " ");
	TextDrawSetString(HPBars_RoundLeftAcc, lAcc[0] ? lAcc : " ");
	TextDrawSetString(HPBars_RoundLeftDmg, lDmg[0] ? lDmg : " ");

	TextDrawSetString(HPBars_RightNames, rNames[0] ? rNames : " ");
	TextDrawSetString(HPBars_RoundRightKills, rKills[0] ? rKills : " ");
	TextDrawSetString(HPBars_RoundRightHP, rHP[0] ? rHP : " ");
	TextDrawSetString(HPBars_RoundRightAcc, rAcc[0] ? rAcc : " ");
	TextDrawSetString(HPBars_RoundRightDmg, rDmg[0] ? rDmg : " ");
	return 1;
}

public UpdateHPBarsMatchStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc)
{
	HPBars_IsMatchEnd = true;
	TextDrawSetString(HPBars_LeftHeader, "Name                  K   D   P   Acc   Dmg");
	TextDrawSetString(HPBars_RightHeader, "Name                  K   D   P   Acc   Dmg");

	new banner[128];
	format(banner, sizeof(banner), "~g~~h~%s ~w~%d  -  %d ~b~~h~%s",
		TeamName[ATTACKER], TeamScore[ATTACKER], TeamScore[DEFENDER], TeamName[DEFENDER]);
	TextDrawSetString(HPBars_TopBanner, banner);

	new mvpStr[96];
	if (topkillID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topkillID][player_Team] == ATTACKER || array[topkillID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		format(mvpStr, sizeof(mvpStr), "~w~Most Kills %s%s ~w~(~r~~h~%d~w~)", teamCol, names[topkillID], topkill);
	}
	else
	{
		format(mvpStr, sizeof(mvpStr), "~w~Most Kills None");
	}
	TextDrawSetString(HPBars_MVPKills, mvpStr);

	if (topDmgID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topDmgID][player_Team] == ATTACKER || array[topDmgID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		format(mvpStr, sizeof(mvpStr), "~w~Most Damage %s%s ~w~(~r~~h~%.0f~w~)", teamCol, names[topDmgID], topDmg);
	}
	else
	{
		format(mvpStr, sizeof(mvpStr), "~w~Most Damage None");
	}
	TextDrawSetString(HPBars_MVPDamage, mvpStr);

	if (topAccID >= 0)
	{
		new teamCol[8];
		teamCol = (array[topAccID][player_Team] == ATTACKER || array[topAccID][player_Team] == ATTACKER_SUB) ? "~g~~h~" : "~b~~h~";
		format(mvpStr, sizeof(mvpStr), "~w~Top Acc %s%s ~w~(~r~~h~%d%%~w~)", teamCol, names[topAccID], topAcc);
	}
	else
	{
		format(mvpStr, sizeof(mvpStr), "~w~Top Acc None");
	}
	TextDrawSetString(HPBars_MVPAcc, mvpStr);

	new lNames[512], lK[256], lD[256], lP[256], lAcc[256], lDmg[256];
	new rNames[512], rK[256], rD[256], rP[256], rAcc[256], rDmg[256];

	for (new i = 0; i != p; ++i)
	{
		if (array[i][player_Team] == ATTACKER || array[i][player_Team] == ATTACKER_SUB)
		{
			format(lNames, sizeof(lNames), "%s~g~~h~%s~n~", lNames, names[i]);
			format(lK, sizeof(lK), "%s~r~~h~%d~n~", lK, array[i][player_Kills]);
			format(lD, sizeof(lD), "%s~r~~h~%d~n~", lD, array[i][player_Deaths]);
			format(lP, sizeof(lP), "%s~r~~h~%d~n~", lP, array[i][player_TPlayed]);
			format(lAcc, sizeof(lAcc), "%s~r~~h~%d%%~n~", lAcc, array[i][player_Acc]);
			format(lDmg, sizeof(lDmg), "%s~r~~h~%d~n~", lDmg, array[i][player_Score]);
		}
		else if (array[i][player_Team] == DEFENDER || array[i][player_Team] == DEFENDER_SUB)
		{
			format(rNames, sizeof(rNames), "%s~b~~h~%s~n~", rNames, names[i]);
			format(rK, sizeof(rK), "%s~r~~h~%d~n~", rK, array[i][player_Kills]);
			format(rD, sizeof(rD), "%s~r~~h~%d~n~", rD, array[i][player_Deaths]);
			format(rP, sizeof(rP), "%s~r~~h~%d~n~", rP, array[i][player_TPlayed]);
			format(rAcc, sizeof(rAcc), "%s~r~~h~%d%%~n~", rAcc, array[i][player_Acc]);
			format(rDmg, sizeof(rDmg), "%s~r~~h~%d~n~", rDmg, array[i][player_Score]);
		}
	}

	TextDrawSetString(HPBars_LeftNames, lNames[0] ? lNames : " ");
	TextDrawSetString(HPBars_MatchLeftK, lK[0] ? lK : " ");
	TextDrawSetString(HPBars_MatchLeftD, lD[0] ? lD : " ");
	TextDrawSetString(HPBars_MatchLeftP, lP[0] ? lP : " ");
	TextDrawSetString(HPBars_MatchLeftAcc, lAcc[0] ? lAcc : " ");
	TextDrawSetString(HPBars_MatchLeftDmg, lDmg[0] ? lDmg : " ");

	TextDrawSetString(HPBars_RightNames, rNames[0] ? rNames : " ");
	TextDrawSetString(HPBars_MatchRightK, rK[0] ? rK : " ");
	TextDrawSetString(HPBars_MatchRightD, rD[0] ? rD : " ");
	TextDrawSetString(HPBars_MatchRightP, rP[0] ? rP : " ");
	TextDrawSetString(HPBars_MatchRightAcc, rAcc[0] ? rAcc : " ");
	TextDrawSetString(HPBars_MatchRightDmg, rDmg[0] ? rDmg : " ");

	// Match rounds history
	new hRound[256], hMap[384], hWin[256];
	new roundsCount = MatchRoundsStarted;
	if (roundsCount > 100)
		roundsCount = 100;

	for (new r = 0; r < roundsCount; r++)
	{
		format(hRound, sizeof(hRound), "%s~w~%d~n~", hRound, r + 1);
		format(hMap, sizeof(hMap), "%s~w~%s %d~n~", hMap, MatchRoundsRecord[r][round__type] == 0 ? "Base" : "Arena", MatchRoundsRecord[r][round__ID]);
		if (MatchRoundsRecord[r][round__winner] == ATTACKER)
			format(hWin, sizeof(hWin), "%s~g~~h~%s~n~", hWin, MatchRoundsRecord[r][round__winnerName]);
		else if (MatchRoundsRecord[r][round__winner] == DEFENDER)
			format(hWin, sizeof(hWin), "%s~b~~h~%s~n~", hWin, MatchRoundsRecord[r][round__winnerName]);
		else
			format(hWin, sizeof(hWin), "%s~w~None~n~", hWin);
	}

	TextDrawSetString(HPBars_HistRound, hRound[0] ? hRound : " ");
	TextDrawSetString(HPBars_HistMap, hMap[0] ? hMap : " ");
	TextDrawSetString(HPBars_HistWinner, hWin[0] ? hWin : " ");
	return 1;
}

public TeamIcons_ResultTextdrawsCreate()
{
	TeamIcons_AttScoreName = TextDrawCreate(220.000, 145.000, "~w~~h~0  ~g~~h~Attackers");
	TextDrawLetterSize(TeamIcons_AttScoreName, 0.3100, 1.3000);
	TextDrawTextSize(TeamIcons_AttScoreName, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_AttScoreName, TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(TeamIcons_AttScoreName, 232664575);
	TextDrawSetShadow(TeamIcons_AttScoreName, 1);
	TextDrawSetOutline(TeamIcons_AttScoreName, 0);
	TextDrawBackgroundColor(TeamIcons_AttScoreName, 0x000000FF);
	TextDrawFont(TeamIcons_AttScoreName, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_AttScoreName, true);

	TeamIcons_HeaderCols = TextDrawCreate(320.000, 149.000, "~w~          Score     Kills   Damage");
	TextDrawLetterSize(TeamIcons_HeaderCols, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_HeaderCols, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_HeaderCols, TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(TeamIcons_HeaderCols, -1);
	TextDrawSetShadow(TeamIcons_HeaderCols, 1);
	TextDrawSetOutline(TeamIcons_HeaderCols, 0);
	TextDrawBackgroundColor(TeamIcons_HeaderCols, 0x000000FF);
	TextDrawFont(TeamIcons_HeaderCols, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_HeaderCols, true);

	TeamIcons_SubTitle = TextDrawCreate(320.000, 105.000, "_");
	TextDrawLetterSize(TeamIcons_SubTitle, 0.3500, 1.5000);
	TextDrawTextSize(TeamIcons_SubTitle, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_SubTitle, TEXT_DRAW_ALIGN_CENTER);
	TextDrawColor(TeamIcons_SubTitle, -1);
	TextDrawSetShadow(TeamIcons_SubTitle, 1);
	TextDrawSetOutline(TeamIcons_SubTitle, 0);
	TextDrawBackgroundColor(TeamIcons_SubTitle, 0x000000FF);
	TextDrawFont(TeamIcons_SubTitle, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_SubTitle, true);

	TeamIcons_AttBoard = TextDrawCreate(220.000, 160.000, "~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~");
	TextDrawLetterSize(TeamIcons_AttBoard, 0.2400, 1.0000);
	TextDrawTextSize(TeamIcons_AttBoard, 432.000, 520.000);
	TextDrawAlignment(TeamIcons_AttBoard, TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(TeamIcons_AttBoard, 0xE1E1E000);
	TextDrawUseBox(TeamIcons_AttBoard, true);
	TextDrawBoxColor(TeamIcons_AttBoard, 0x0DDE2D43);
	TextDrawSetShadow(TeamIcons_AttBoard, 1);
	TextDrawSetOutline(TeamIcons_AttBoard, 0);
	TextDrawBackgroundColor(TeamIcons_AttBoard, 0x000000FF);
	TextDrawFont(TeamIcons_AttBoard, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_AttBoard, true);

	TeamIcons_AttNames = TextDrawCreate(220.000, 159.000, "_");
	TextDrawLetterSize(TeamIcons_AttNames, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_AttNames, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_AttNames, TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(TeamIcons_AttNames, -1);
	TextDrawSetShadow(TeamIcons_AttNames, 1);
	TextDrawSetOutline(TeamIcons_AttNames, 1);
	TextDrawBackgroundColor(TeamIcons_AttNames, 0x00000022);
	TextDrawFont(TeamIcons_AttNames, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_AttNames, true);

	TeamIcons_AttStatus = TextDrawCreate(332.000, 159.000, "_");
	TextDrawLetterSize(TeamIcons_AttStatus, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_AttStatus, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_AttStatus, TEXT_DRAW_ALIGN_RIGHT);
	TextDrawColor(TeamIcons_AttStatus, -1);
	TextDrawSetShadow(TeamIcons_AttStatus, 2);
	TextDrawSetOutline(TeamIcons_AttStatus, 1);
	TextDrawBackgroundColor(TeamIcons_AttStatus, 0x00000022);
	TextDrawFont(TeamIcons_AttStatus, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_AttStatus, true);

	TeamIcons_AttScore = TextDrawCreate(360.000, 159.000, "_");
	TextDrawLetterSize(TeamIcons_AttScore, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_AttScore, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_AttScore, TEXT_DRAW_ALIGN_RIGHT);
	TextDrawColor(TeamIcons_AttScore, -1);
	TextDrawSetShadow(TeamIcons_AttScore, 2);
	TextDrawSetOutline(TeamIcons_AttScore, 1);
	TextDrawBackgroundColor(TeamIcons_AttScore, 0x00000022);
	TextDrawFont(TeamIcons_AttScore, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_AttScore, true);

	TeamIcons_AttKills = TextDrawCreate(389.000, 159.000, "_");
	TextDrawLetterSize(TeamIcons_AttKills, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_AttKills, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_AttKills, TEXT_DRAW_ALIGN_RIGHT);
	TextDrawColor(TeamIcons_AttKills, -1);
	TextDrawSetShadow(TeamIcons_AttKills, 2);
	TextDrawSetOutline(TeamIcons_AttKills, 1);
	TextDrawBackgroundColor(TeamIcons_AttKills, 0x00000036);
	TextDrawFont(TeamIcons_AttKills, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_AttKills, true);

	TeamIcons_AttDamage = TextDrawCreate(424.000, 159.000, "_");
	TextDrawLetterSize(TeamIcons_AttDamage, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_AttDamage, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_AttDamage, TEXT_DRAW_ALIGN_RIGHT);
	TextDrawColor(TeamIcons_AttDamage, -1);
	TextDrawSetShadow(TeamIcons_AttDamage, 2);
	TextDrawSetOutline(TeamIcons_AttDamage, 1);
	TextDrawBackgroundColor(TeamIcons_AttDamage, 0x00000022);
	TextDrawFont(TeamIcons_AttDamage, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_AttDamage, true);

	TeamIcons_DefScoreName = TextDrawCreate(220.000, 265.000, "~w~~h~0  ~b~~h~Defenders");
	TextDrawLetterSize(TeamIcons_DefScoreName, 0.3100, 1.3000);
	TextDrawTextSize(TeamIcons_DefScoreName, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_DefScoreName, TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(TeamIcons_DefScoreName, 0x575CACFF);
	TextDrawSetShadow(TeamIcons_DefScoreName, 1);
	TextDrawSetOutline(TeamIcons_DefScoreName, 0);
	TextDrawBackgroundColor(TeamIcons_DefScoreName, 0x000000FF);
	TextDrawFont(TeamIcons_DefScoreName, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_DefScoreName, true);

	TeamIcons_DefBoard = TextDrawCreate(220.000, 280.000, "~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~");
	TextDrawLetterSize(TeamIcons_DefBoard, 0.2400, 1.0000);
	TextDrawTextSize(TeamIcons_DefBoard, 432.000, 520.000);
	TextDrawAlignment(TeamIcons_DefBoard, TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(TeamIcons_DefBoard, 0xE1E1E000);
	TextDrawUseBox(TeamIcons_DefBoard, true);
	TextDrawBoxColor(TeamIcons_DefBoard, 0x191ED133);
	TextDrawSetShadow(TeamIcons_DefBoard, 1);
	TextDrawSetOutline(TeamIcons_DefBoard, 0);
	TextDrawBackgroundColor(TeamIcons_DefBoard, 0x000000FF);
	TextDrawFont(TeamIcons_DefBoard, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_DefBoard, true);

	TeamIcons_DefNames = TextDrawCreate(220.000, 279.000, "_");
	TextDrawLetterSize(TeamIcons_DefNames, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_DefNames, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_DefNames, TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(TeamIcons_DefNames, -1);
	TextDrawSetShadow(TeamIcons_DefNames, 2);
	TextDrawSetOutline(TeamIcons_DefNames, 1);
	TextDrawBackgroundColor(TeamIcons_DefNames, 0x00000022);
	TextDrawFont(TeamIcons_DefNames, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_DefNames, true);

	TeamIcons_DefStatus = TextDrawCreate(332.000, 279.000, "_");
	TextDrawLetterSize(TeamIcons_DefStatus, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_DefStatus, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_DefStatus, TEXT_DRAW_ALIGN_RIGHT);
	TextDrawColor(TeamIcons_DefStatus, -1);
	TextDrawSetShadow(TeamIcons_DefStatus, 2);
	TextDrawSetOutline(TeamIcons_DefStatus, 1);
	TextDrawBackgroundColor(TeamIcons_DefStatus, 0x00000022);
	TextDrawFont(TeamIcons_DefStatus, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_DefStatus, true);

	TeamIcons_DefScore = TextDrawCreate(360.000, 279.000, "_");
	TextDrawLetterSize(TeamIcons_DefScore, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_DefScore, 640.000, 930.000);
	TextDrawAlignment(TeamIcons_DefScore, TEXT_DRAW_ALIGN_RIGHT);
	TextDrawColor(TeamIcons_DefScore, -1);
	TextDrawSetShadow(TeamIcons_DefScore, 0);
	TextDrawSetOutline(TeamIcons_DefScore, 1);
	TextDrawBackgroundColor(TeamIcons_DefScore, 0x0000003A);
	TextDrawFont(TeamIcons_DefScore, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_DefScore, true);

	TeamIcons_DefKills = TextDrawCreate(389.000, 279.000, "_");
	TextDrawLetterSize(TeamIcons_DefKills, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_DefKills, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_DefKills, TEXT_DRAW_ALIGN_RIGHT);
	TextDrawColor(TeamIcons_DefKills, -1);
	TextDrawSetShadow(TeamIcons_DefKills, 2);
	TextDrawSetOutline(TeamIcons_DefKills, 1);
	TextDrawBackgroundColor(TeamIcons_DefKills, 0x00000022);
	TextDrawFont(TeamIcons_DefKills, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_DefKills, true);

	TeamIcons_DefDamage = TextDrawCreate(424.000, 279.000, "_");
	TextDrawLetterSize(TeamIcons_DefDamage, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_DefDamage, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_DefDamage, TEXT_DRAW_ALIGN_RIGHT);
	TextDrawColor(TeamIcons_DefDamage, -1);
	TextDrawSetShadow(TeamIcons_DefDamage, 2);
	TextDrawSetOutline(TeamIcons_DefDamage, 1);
	TextDrawBackgroundColor(TeamIcons_DefDamage, 0x00000022);
	TextDrawFont(TeamIcons_DefDamage, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_DefDamage, true);

	TeamIcons_GreenLabels = TextDrawCreate(424.000, 400.000, "~w~Most Damage~n~Most Kills~n~Best K/D Ratio~n~Best Accuracy");
	TextDrawLetterSize(TeamIcons_GreenLabels, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_GreenLabels, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_GreenLabels, TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(TeamIcons_GreenLabels, -1);
	TextDrawSetShadow(TeamIcons_GreenLabels, 1);
	TextDrawSetOutline(TeamIcons_GreenLabels, 0);
	TextDrawBackgroundColor(TeamIcons_GreenLabels, 0x000000FF);
	TextDrawFont(TeamIcons_GreenLabels, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_GreenLabels, true);

	TeamIcons_GreenBoard = TextDrawCreate(480.000, 401.000, "~n~~n~~n~~n~");
	TextDrawLetterSize(TeamIcons_GreenBoard, 0.2200, 0.9000);
	TextDrawTextSize(TeamIcons_GreenBoard, 617.000, 420.000);
	TextDrawAlignment(TeamIcons_GreenBoard, TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(TeamIcons_GreenBoard, 0xE1E1E000);
	TextDrawUseBox(TeamIcons_GreenBoard, true);
	TextDrawBoxColor(TeamIcons_GreenBoard, 0x3FA13F22);
	TextDrawSetShadow(TeamIcons_GreenBoard, 1);
	TextDrawSetOutline(TeamIcons_GreenBoard, 0);
	TextDrawBackgroundColor(TeamIcons_GreenBoard, 0x000000FF);
	TextDrawFont(TeamIcons_GreenBoard, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_GreenBoard, true);

	TeamIcons_GreenNames = TextDrawCreate(484.000, 400.000, "_");
	TextDrawLetterSize(TeamIcons_GreenNames, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_GreenNames, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_GreenNames, TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(TeamIcons_GreenNames, -1);
	TextDrawSetShadow(TeamIcons_GreenNames, 2);
	TextDrawSetOutline(TeamIcons_GreenNames, 1);
	TextDrawBackgroundColor(TeamIcons_GreenNames, 0x00000022);
	TextDrawFont(TeamIcons_GreenNames, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_GreenNames, true);

	TeamIcons_GreenValues = TextDrawCreate(587.000, 400.000, "_");
	TextDrawLetterSize(TeamIcons_GreenValues, 0.2000, 0.9000);
	TextDrawTextSize(TeamIcons_GreenValues, 1280.000, 1280.000);
	TextDrawAlignment(TeamIcons_GreenValues, TEXT_DRAW_ALIGN_LEFT);
	TextDrawColor(TeamIcons_GreenValues, -1);
	TextDrawSetShadow(TeamIcons_GreenValues, 2);
	TextDrawSetOutline(TeamIcons_GreenValues, 1);
	TextDrawBackgroundColor(TeamIcons_GreenValues, 0x00000022);
	TextDrawFont(TeamIcons_GreenValues, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(TeamIcons_GreenValues, true);
	return 1;
}


public UpdateTeamIconsPlayerWinLoss(playerid)
{
	new playerTeam = Player[playerid][Team];
	if (MatchEnded)
	{
		if (TeamScore[ATTACKER] == TeamScore[DEFENDER])
		{
			PlayerTextDrawSetString(playerid, TeamIcons_WinLoss[playerid], "~w~MATCH DRAW");
			PlayerTextDrawColor(playerid, TeamIcons_WinLoss[playerid], 0xFFFFFFFF);
		}
		else if ((TeamScore[ATTACKER] > TeamScore[DEFENDER] && (playerTeam == ATTACKER || playerTeam == ATTACKER_SUB)) ||
				 (TeamScore[DEFENDER] > TeamScore[ATTACKER] && (playerTeam == DEFENDER || playerTeam == DEFENDER_SUB)))
		{
			PlayerTextDrawSetString(playerid, TeamIcons_WinLoss[playerid], "~g~~h~MATCH WIN");
			PlayerTextDrawColor(playerid, TeamIcons_WinLoss[playerid], 0x508E42FF);
		}
		else if (playerTeam == ATTACKER || playerTeam == ATTACKER_SUB || playerTeam == DEFENDER || playerTeam == DEFENDER_SUB)
		{
			PlayerTextDrawSetString(playerid, TeamIcons_WinLoss[playerid], "~r~~h~MATCH LOSS");
			PlayerTextDrawColor(playerid, TeamIcons_WinLoss[playerid], 0xFE3130FF);
		}
		else
		{
			if (TeamScore[ATTACKER] > TeamScore[DEFENDER])
			{
				PlayerTextDrawSetString(playerid, TeamIcons_WinLoss[playerid], "~g~~h~MATCH OVER");
				PlayerTextDrawColor(playerid, TeamIcons_WinLoss[playerid], 232664575);
			}
			else
			{
				PlayerTextDrawSetString(playerid, TeamIcons_WinLoss[playerid], "~b~~h~MATCH OVER");
				PlayerTextDrawColor(playerid, TeamIcons_WinLoss[playerid], 0x575CACFF);
			}
		}
	}
	else
	{
		if (TeamIcons_LastWinner == -1 || TeamIcons_LastWinner == 4)
		{
			PlayerTextDrawSetString(playerid, TeamIcons_WinLoss[playerid], "~w~ROUND DRAW");
			PlayerTextDrawColor(playerid, TeamIcons_WinLoss[playerid], 0xFFFFFFFF);
		}
		else if ((TeamIcons_LastWinner == ATTACKER && (playerTeam == ATTACKER || playerTeam == ATTACKER_SUB)) ||
				 (TeamIcons_LastWinner == DEFENDER && (playerTeam == DEFENDER || playerTeam == DEFENDER_SUB)))
		{
			PlayerTextDrawSetString(playerid, TeamIcons_WinLoss[playerid], "~g~~h~ROUND WIN");
			PlayerTextDrawColor(playerid, TeamIcons_WinLoss[playerid], 0x508E42FF);
		}
		else if (playerTeam == ATTACKER || playerTeam == ATTACKER_SUB || playerTeam == DEFENDER || playerTeam == DEFENDER_SUB)
		{
			PlayerTextDrawSetString(playerid, TeamIcons_WinLoss[playerid], "~r~~h~ROUND LOSS");
			PlayerTextDrawColor(playerid, TeamIcons_WinLoss[playerid], 0xFE3130FF);
		}
		else
		{
			if (TeamIcons_LastWinner == ATTACKER)
			{
				PlayerTextDrawSetString(playerid, TeamIcons_WinLoss[playerid], "~g~~h~ROUND OVER");
				PlayerTextDrawColor(playerid, TeamIcons_WinLoss[playerid], 232664575);
			}
			else
			{
				PlayerTextDrawSetString(playerid, TeamIcons_WinLoss[playerid], "~b~~h~ROUND OVER");
				PlayerTextDrawColor(playerid, TeamIcons_WinLoss[playerid], 0x575CACFF);
			}
		}
	}
	return 1;
}

public ShowTeamIconsEndRoundTextDraw(playerid)
{
	UpdateTeamIconsPlayerWinLoss(playerid);

	PlayerTextDrawShow(playerid, TeamIcons_WinLoss[playerid]);
	TextDrawShowForPlayer(playerid, TeamIcons_SubTitle);
	TextDrawShowForPlayer(playerid, TeamIcons_AttScoreName);
	TextDrawShowForPlayer(playerid, TeamIcons_HeaderCols);
	TextDrawShowForPlayer(playerid, TeamIcons_AttBoard);
	TextDrawShowForPlayer(playerid, TeamIcons_AttNames);
	TextDrawShowForPlayer(playerid, TeamIcons_AttStatus);
	TextDrawShowForPlayer(playerid, TeamIcons_AttScore);
	TextDrawShowForPlayer(playerid, TeamIcons_AttKills);
	TextDrawShowForPlayer(playerid, TeamIcons_AttDamage);

	TextDrawShowForPlayer(playerid, TeamIcons_DefScoreName);
	TextDrawShowForPlayer(playerid, TeamIcons_DefBoard);
	TextDrawShowForPlayer(playerid, TeamIcons_DefNames);
	TextDrawShowForPlayer(playerid, TeamIcons_DefStatus);
	TextDrawShowForPlayer(playerid, TeamIcons_DefScore);
	TextDrawShowForPlayer(playerid, TeamIcons_DefKills);
	TextDrawShowForPlayer(playerid, TeamIcons_DefDamage);

	TextDrawShowForPlayer(playerid, TeamIcons_GreenLabels);
	TextDrawShowForPlayer(playerid, TeamIcons_GreenBoard);
	TextDrawShowForPlayer(playerid, TeamIcons_GreenNames);
	TextDrawShowForPlayer(playerid, TeamIcons_GreenValues);
	return 1;
}

public HideTeamIconsEndRoundTextDraw(playerid)
{
	PlayerTextDrawHide(playerid, TeamIcons_WinLoss[playerid]);
	TextDrawHideForPlayer(playerid, TeamIcons_SubTitle);
	TextDrawHideForPlayer(playerid, TeamIcons_AttScoreName);
	TextDrawHideForPlayer(playerid, TeamIcons_HeaderCols);
	TextDrawHideForPlayer(playerid, TeamIcons_AttBoard);
	TextDrawHideForPlayer(playerid, TeamIcons_AttNames);
	TextDrawHideForPlayer(playerid, TeamIcons_AttStatus);
	TextDrawHideForPlayer(playerid, TeamIcons_AttScore);
	TextDrawHideForPlayer(playerid, TeamIcons_AttKills);
	TextDrawHideForPlayer(playerid, TeamIcons_AttDamage);

	TextDrawHideForPlayer(playerid, TeamIcons_DefScoreName);
	TextDrawHideForPlayer(playerid, TeamIcons_DefBoard);
	TextDrawHideForPlayer(playerid, TeamIcons_DefNames);
	TextDrawHideForPlayer(playerid, TeamIcons_DefStatus);
	TextDrawHideForPlayer(playerid, TeamIcons_DefScore);
	TextDrawHideForPlayer(playerid, TeamIcons_DefKills);
	TextDrawHideForPlayer(playerid, TeamIcons_DefDamage);

	TextDrawHideForPlayer(playerid, TeamIcons_GreenLabels);
	TextDrawHideForPlayer(playerid, TeamIcons_GreenBoard);
	TextDrawHideForPlayer(playerid, TeamIcons_GreenNames);
	TextDrawHideForPlayer(playerid, TeamIcons_GreenValues);
	return 1;
}

public UpdateTeamIconsEndRoundStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, winnerid, WinID, Float: ahpleft, Float: dhpleft, aalive, dalive, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc)
{
	#pragma unused ahpleft, dhpleft, aalive, dalive
	TeamIcons_LastWinner = winnerid;

	new sub[128];
	switch (WinID)
	{
		case 3: format(sub, sizeof(sub), "~w~~h~The ~b~~h~%s ~w~~h~were eliminated", TeamName[DEFENDER]);
		case 2: format(sub, sizeof(sub), "~w~~h~The ~g~~h~%s ~w~~h~were eliminated", TeamName[ATTACKER]);
		case 1: format(sub, sizeof(sub), "~w~~h~The ~g~~h~%s ~w~~h~ran out of time", TeamName[ATTACKER]);
		case 0, 5, 6: format(sub, sizeof(sub), "~w~~h~The Checkpoint was captured");
		case 4: format(sub, sizeof(sub), "~w~~h~Round Draw");
		default: format(sub, sizeof(sub), "~w~~h~Round Over");
	}
	TextDrawSetString(TeamIcons_SubTitle, sub);

	new headerStr[64];
	format(headerStr, sizeof(headerStr), "~w~~h~%d  ~g~~h~%s", TeamScore[ATTACKER], TeamName[ATTACKER]);
	TextDrawSetString(TeamIcons_AttScoreName, headerStr);

	format(headerStr, sizeof(headerStr), "~w~~h~%d  ~b~~h~%s", TeamScore[DEFENDER], TeamName[DEFENDER]);
	TextDrawSetString(TeamIcons_DefScoreName, headerStr);

	new aNames[512], aStatus[256], aScore[256], aKills[256], aDamage[256];
	new dNames[512], dStatus[256], dScore[256], dKills[256], dDamage[256];

	new Float: bestKD = -1.0;
	new bestKDID = -1;

	for (new i = 0; i != p; ++i)
	{
		new kills = array[i][player_Kills];
		new deaths = array[i][player_Deaths];
		new dmg = array[i][player_Score];

		if (kills > 0)
		{
			new Float: kd = (deaths == 0) ? float(kills) : (float(kills) / float(deaths));
			if (kd > bestKD)
			{
				bestKD = kd;
				bestKDID = i;
			}
		}

		new sc = (kills * 10) + (dmg / 20) + (deaths == 0 ? 15 : 0);

		if (array[i][player_Team] == ATTACKER || array[i][player_Team] == ATTACKER_SUB)
		{
			format(aNames, sizeof(aNames), "%s%s~n~", aNames, names[i]);
			format(aStatus, sizeof(aStatus), "%s%s~n~", aStatus, (deaths > 0) ? "Died" : " ");
			format(aScore, sizeof(aScore), "%s%d~n~", aScore, sc);
			format(aKills, sizeof(aKills), "%s%d~n~", aKills, kills);
			format(aDamage, sizeof(aDamage), "%s%d~n~", aDamage, dmg);
		}
		else if (array[i][player_Team] == DEFENDER || array[i][player_Team] == DEFENDER_SUB)
		{
			format(dNames, sizeof(dNames), "%s%s~n~", dNames, names[i]);
			format(dStatus, sizeof(dStatus), "%s%s~n~", dStatus, (deaths > 0) ? "Died" : " ");
			format(dScore, sizeof(dScore), "%s%d~n~", dScore, sc);
			format(dKills, sizeof(dKills), "%s%d~n~", dKills, kills);
			format(dDamage, sizeof(dDamage), "%s%d~n~", dDamage, dmg);
		}
	}

	TextDrawSetString(TeamIcons_AttNames, aNames[0] ? aNames : " ");
	TextDrawSetString(TeamIcons_AttStatus, aStatus[0] ? aStatus : " ");
	TextDrawSetString(TeamIcons_AttScore, aScore[0] ? aScore : " ");
	TextDrawSetString(TeamIcons_AttKills, aKills[0] ? aKills : " ");
	TextDrawSetString(TeamIcons_AttDamage, aDamage[0] ? aDamage : " ");

	TextDrawSetString(TeamIcons_DefNames, dNames[0] ? dNames : " ");
	TextDrawSetString(TeamIcons_DefStatus, dStatus[0] ? dStatus : " ");
	TextDrawSetString(TeamIcons_DefScore, dScore[0] ? dScore : " ");
	TextDrawSetString(TeamIcons_DefKills, dKills[0] ? dKills : " ");
	TextDrawSetString(TeamIcons_DefDamage, dDamage[0] ? dDamage : " ");

	new greenNames[256], greenValues[128];
	if (topDmgID >= 0)
	{
		format(greenNames, sizeof(greenNames), "%s~n~", names[topDmgID]);
		format(greenValues, sizeof(greenValues), "%.0f~n~", topDmg);
	}
	else
	{
		format(greenNames, sizeof(greenNames), "None~n~");
		format(greenValues, sizeof(greenValues), "0~n~");
	}

	if (topkillID >= 0)
	{
		format(greenNames, sizeof(greenNames), "%s%s~n~", greenNames, names[topkillID]);
		format(greenValues, sizeof(greenValues), "%s%d~n~", greenValues, topkill);
	}
	else
	{
		format(greenNames, sizeof(greenNames), "%sNone~n~", greenNames);
		format(greenValues, sizeof(greenValues), "%s0~n~", greenValues);
	}

	if (bestKDID >= 0)
	{
		format(greenNames, sizeof(greenNames), "%s%s~n~", greenNames, names[bestKDID]);
		format(greenValues, sizeof(greenValues), "%s%.2f~n~", greenValues, bestKD);
	}
	else
	{
		format(greenNames, sizeof(greenNames), "%sNone~n~", greenNames);
		format(greenValues, sizeof(greenValues), "%s0.00~n~", greenValues);
	}

	if (topAccID >= 0)
	{
		format(greenNames, sizeof(greenNames), "%s%s~n~", greenNames, names[topAccID]);
		format(greenValues, sizeof(greenValues), "%s%d%%", greenValues, topAcc);
	}
	else
	{
		format(greenNames, sizeof(greenNames), "%sNone~n~", greenNames);
		format(greenValues, sizeof(greenValues), "%s0%%", greenValues);
	}

	TextDrawSetString(TeamIcons_GreenNames, greenNames);
	TextDrawSetString(TeamIcons_GreenValues, greenValues);

	foreach (new i : Player)
	{
		UpdateTeamIconsPlayerWinLoss(i);
		if (Player[i][RoundTDStyle] == 3 && Player[i][TextDrawOnScreen])
		{
			PlayerTextDrawShow(i, TeamIcons_WinLoss[i]);
		}
	}
	return 1;
}

public UpdateTeamIconsMatchStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, winnerid, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc)
{
	TeamIcons_LastWinner = winnerid;

	new sub[128];
	if (winnerid == ATTACKER)
		format(sub, sizeof(sub), "~w~~h~The ~g~~h~%s ~w~~h~have won the match!", TeamName[ATTACKER]);
	else if (winnerid == DEFENDER)
		format(sub, sizeof(sub), "~w~~h~The ~b~~h~%s ~w~~h~have won the match!", TeamName[DEFENDER]);
	else
		format(sub, sizeof(sub), "~w~~h~The match ended in a draw!");
	TextDrawSetString(TeamIcons_SubTitle, sub);

	new headerStr[64];
	format(headerStr, sizeof(headerStr), "~w~~h~%d  ~g~~h~%s", TeamScore[ATTACKER], TeamName[ATTACKER]);
	TextDrawSetString(TeamIcons_AttScoreName, headerStr);

	format(headerStr, sizeof(headerStr), "~w~~h~%d  ~b~~h~%s", TeamScore[DEFENDER], TeamName[DEFENDER]);
	TextDrawSetString(TeamIcons_DefScoreName, headerStr);

	new aNames[512], aStatus[256], aScore[256], aKills[256], aDamage[256];
	new dNames[512], dStatus[256], dScore[256], dKills[256], dDamage[256];

	new Float: bestKD = -1.0;
	new bestKDID = -1;

	for (new i = 0; i != p; ++i)
	{
		new kills = array[i][player_Kills];
		new deaths = array[i][player_Deaths];
		new dmg = array[i][player_Score];

		if (kills > 0)
		{
			new Float: kd = (deaths == 0) ? float(kills) : (float(kills) / float(deaths));
			if (kd > bestKD)
			{
				bestKD = kd;
				bestKDID = i;
			}
		}

		new sc = (kills * 10) + (dmg / 20);

		if (array[i][player_Team] == ATTACKER || array[i][player_Team] == ATTACKER_SUB)
		{
			format(aNames, sizeof(aNames), "%s%s~n~", aNames, names[i]);
			format(aStatus, sizeof(aStatus), "%s ~n~", aStatus);
			format(aScore, sizeof(aScore), "%s%d~n~", aScore, sc);
			format(aKills, sizeof(aKills), "%s%d~n~", aKills, kills);
			format(aDamage, sizeof(aDamage), "%s%d~n~", aDamage, dmg);
		}
		else if (array[i][player_Team] == DEFENDER || array[i][player_Team] == DEFENDER_SUB)
		{
			format(dNames, sizeof(dNames), "%s%s~n~", dNames, names[i]);
			format(dStatus, sizeof(dStatus), "%s ~n~", dStatus);
			format(dScore, sizeof(dScore), "%s%d~n~", dScore, sc);
			format(dKills, sizeof(dKills), "%s%d~n~", dKills, kills);
			format(dDamage, sizeof(dDamage), "%s%d~n~", dDamage, dmg);
		}
	}

	TextDrawSetString(TeamIcons_AttNames, aNames[0] ? aNames : " ");
	TextDrawSetString(TeamIcons_AttStatus, aStatus[0] ? aStatus : " ");
	TextDrawSetString(TeamIcons_AttScore, aScore[0] ? aScore : " ");
	TextDrawSetString(TeamIcons_AttKills, aKills[0] ? aKills : " ");
	TextDrawSetString(TeamIcons_AttDamage, aDamage[0] ? aDamage : " ");

	TextDrawSetString(TeamIcons_DefNames, dNames[0] ? dNames : " ");
	TextDrawSetString(TeamIcons_DefStatus, dStatus[0] ? dStatus : " ");
	TextDrawSetString(TeamIcons_DefScore, dScore[0] ? dScore : " ");
	TextDrawSetString(TeamIcons_DefKills, dKills[0] ? dKills : " ");
	TextDrawSetString(TeamIcons_DefDamage, dDamage[0] ? dDamage : " ");

	new greenNames[256], greenValues[128];
	if (topDmgID >= 0)
	{
		format(greenNames, sizeof(greenNames), "%s~n~", names[topDmgID]);
		format(greenValues, sizeof(greenValues), "%.0f~n~", topDmg);
	}
	else
	{
		format(greenNames, sizeof(greenNames), "None~n~");
		format(greenValues, sizeof(greenValues), "0~n~");
	}

	if (topkillID >= 0)
	{
		format(greenNames, sizeof(greenNames), "%s%s~n~", greenNames, names[topkillID]);
		format(greenValues, sizeof(greenValues), "%s%d~n~", greenValues, topkill);
	}
	else
	{
		format(greenNames, sizeof(greenNames), "%sNone~n~", greenNames);
		format(greenValues, sizeof(greenValues), "%s0~n~", greenValues);
	}

	if (bestKDID >= 0)
	{
		format(greenNames, sizeof(greenNames), "%s%s~n~", greenNames, names[bestKDID]);
		format(greenValues, sizeof(greenValues), "%s%.2f~n~", greenValues, bestKD);
	}
	else
	{
		format(greenNames, sizeof(greenNames), "%sNone~n~", greenNames);
		format(greenValues, sizeof(greenValues), "%s0.00~n~", greenValues);
	}

	if (topAccID >= 0)
	{
		format(greenNames, sizeof(greenNames), "%s%s~n~", greenNames, names[topAccID]);
		format(greenValues, sizeof(greenValues), "%s%d%%", greenValues, topAcc);
	}
	else
	{
		format(greenNames, sizeof(greenNames), "%sNone~n~", greenNames);
		format(greenValues, sizeof(greenValues), "%s0%%", greenValues);
	}

	TextDrawSetString(TeamIcons_GreenNames, greenNames);
	TextDrawSetString(TeamIcons_GreenValues, greenValues);

	foreach (new i : Player)
	{
		UpdateTeamIconsPlayerWinLoss(i);
		if (Player[i][RoundTDStyle] == 3 && Player[i][TextDrawOnScreen])
		{
			PlayerTextDrawShow(i, TeamIcons_WinLoss[i]);
		}
	}
	return 1;
}


ShowEndRoundTextDraw(playerid)
{
	if (Player[playerid][Team] == NON)
		return 0;

	if (Player[playerid][RoundTDStyle] == 3)
	{
		ShowTeamIconsEndRoundTextDraw(playerid);
		HidePlayerDeathNotice(playerid);
		Player[playerid][TextDrawOnScreen] = true;
		return 1;
	}
	if (Player[playerid][RoundTDStyle] == 4)
	{
		ShowHPBarsEndRoundTextDraw(playerid);
		HidePlayerDeathNotice(playerid);
		Player[playerid][TextDrawOnScreen] = true;
		return 1;
	}

	TextDrawShowForPlayer(playerid, Modern_AttColorBar);
	TextDrawShowForPlayer(playerid, Modern_DefColorBar);
	TextDrawShowForPlayer(playerid, Modern_AttBoard);
	TextDrawShowForPlayer(playerid, Modern_DefBoard);
	TextDrawShowForPlayer(playerid, Modern_BottomBoard);

	TextDrawShowForPlayer(playerid, Modern_AttTeamName);
	TextDrawShowForPlayer(playerid, Modern_DefTeamName);

	TextDrawShowForPlayer(playerid, Modern_AttHeaderName);
	TextDrawShowForPlayer(playerid, Modern_AttHeaderDmg);
	TextDrawShowForPlayer(playerid, Modern_AttHeaderKills);
	TextDrawShowForPlayer(playerid, Modern_AttHeaderHealth);

	TextDrawShowForPlayer(playerid, Modern_DefHeaderName);
	TextDrawShowForPlayer(playerid, Modern_DefHeaderDmg);
	TextDrawShowForPlayer(playerid, Modern_DefHeaderKills);
	TextDrawShowForPlayer(playerid, Modern_DefHeaderHealth);

	TextDrawShowForPlayer(playerid, Modern_AttNames);
	TextDrawShowForPlayer(playerid, Modern_AttDamage);
	TextDrawShowForPlayer(playerid, Modern_AttKills);
	TextDrawShowForPlayer(playerid, Modern_AttHealth);

	TextDrawShowForPlayer(playerid, Modern_DefNames);
	TextDrawShowForPlayer(playerid, Modern_DefDamage);
	TextDrawShowForPlayer(playerid, Modern_DefKills);
	TextDrawShowForPlayer(playerid, Modern_DefHealth);

	TextDrawShowForPlayer(playerid, Modern_TeamWhoWon);
	TextDrawShowForPlayer(playerid, Modern_MVPDamage);
	TextDrawShowForPlayer(playerid, Modern_MVPLeastDmg);
	TextDrawShowForPlayer(playerid, Modern_MVPKills);

	TextDrawShowForPlayer(playerid, Modern_RoundInfo[0]);
	TextDrawShowForPlayer(playerid, Modern_RoundInfo[1]);
	TextDrawShowForPlayer(playerid, Modern_RoundInfo[2]);

	HidePlayerDeathNotice(playerid);

	Player[playerid][TextDrawOnScreen] = true;
	return 1;
}


HideEndRoundTextDraw(playerid)
{
	HideTeamIconsEndRoundTextDraw(playerid);
	HideHPBarsEndRoundTextDraw(playerid);

	TextDrawHideForPlayer(playerid, Modern_AttColorBar);
	TextDrawHideForPlayer(playerid, Modern_DefColorBar);
	TextDrawHideForPlayer(playerid, Modern_AttBoard);
	TextDrawHideForPlayer(playerid, Modern_DefBoard);
	TextDrawHideForPlayer(playerid, Modern_BottomBoard);

	TextDrawHideForPlayer(playerid, Modern_AttTeamName);
	TextDrawHideForPlayer(playerid, Modern_DefTeamName);

	TextDrawHideForPlayer(playerid, Modern_AttHeaderName);
	TextDrawHideForPlayer(playerid, Modern_AttHeaderDmg);
	TextDrawHideForPlayer(playerid, Modern_AttHeaderKills);
	TextDrawHideForPlayer(playerid, Modern_AttHeaderHealth);

	TextDrawHideForPlayer(playerid, Modern_DefHeaderName);
	TextDrawHideForPlayer(playerid, Modern_DefHeaderDmg);
	TextDrawHideForPlayer(playerid, Modern_DefHeaderKills);
	TextDrawHideForPlayer(playerid, Modern_DefHeaderHealth);

	TextDrawHideForPlayer(playerid, Modern_AttNames);
	TextDrawHideForPlayer(playerid, Modern_AttDamage);
	TextDrawHideForPlayer(playerid, Modern_AttKills);
	TextDrawHideForPlayer(playerid, Modern_AttHealth);

	TextDrawHideForPlayer(playerid, Modern_DefNames);
	TextDrawHideForPlayer(playerid, Modern_DefDamage);
	TextDrawHideForPlayer(playerid, Modern_DefKills);
	TextDrawHideForPlayer(playerid, Modern_DefHealth);

	TextDrawHideForPlayer(playerid, Modern_TeamWhoWon);
	TextDrawHideForPlayer(playerid, Modern_MVPDamage);
	TextDrawHideForPlayer(playerid, Modern_MVPLeastDmg);
	TextDrawHideForPlayer(playerid, Modern_MVPKills);

	TextDrawHideForPlayer(playerid, Modern_RoundInfo[0]);
	TextDrawHideForPlayer(playerid, Modern_RoundInfo[1]);
	TextDrawHideForPlayer(playerid, Modern_RoundInfo[2]);

	for (new i = 0; i < 3; i++)
	{
	}

	Player[playerid][TextDrawOnScreen] = false;
	return 1;
}

