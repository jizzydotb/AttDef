#if defined _attdef_td_scoreboard
	#endinput
#endif
#define _attdef_td_scoreboard

/*
	Scoreboard & Match Status Textdraws Sub-module
*/

LoadMatchScoreTextDraws()
{
	/*
	// Credits: [U]27 aka [U]jay
	ScoreBoardBoxTD[0] = TextDrawCreate(474.000000, 7.000000, "-");
	TextDrawBackgroundColor(ScoreBoardBoxTD[0], 255);
	TextDrawFont(ScoreBoardBoxTD[0], 1);
	TextDrawLetterSize(ScoreBoardBoxTD[0], 11.000005, 14.000000);
	TextDrawColor(ScoreBoardBoxTD[0], 255);
	TextDrawSetOutline(ScoreBoardBoxTD[0], 0);
	TextDrawSetProportional(ScoreBoardBoxTD[0], 1);
	TextDrawSetShadow(ScoreBoardBoxTD[0], 1);

	ScoreBoardBoxTD[1] = TextDrawCreate(524.000000, 43.000000, "-");
	TextDrawBackgroundColor(ScoreBoardBoxTD[1], 255);
	TextDrawFont(ScoreBoardBoxTD[1], 1);
	TextDrawLetterSize(ScoreBoardBoxTD[1], 4.299998, 10.000000);
	TextDrawColor(ScoreBoardBoxTD[1], 255);
	TextDrawSetOutline(ScoreBoardBoxTD[1], 0);
	TextDrawSetProportional(ScoreBoardBoxTD[1], 1);
	TextDrawSetShadow(ScoreBoardBoxTD[1], 1);*/

	RoundsPlayed = TextDrawCreate(524.200, 109.000, "~y~~h~[Rounds: ~w~7~y~~h~/~w~9~y~~h~]");
	TextDrawBackgroundColor(RoundsPlayed, 155);
	TextDrawFont(RoundsPlayed, 1);
	TextDrawLetterSize(RoundsPlayed, 0.2400, 0.8000);
	TextDrawColor(RoundsPlayed, 1600086015);
	TextDrawSetOutline(RoundsPlayed, 1);
	TextDrawSetProportional(RoundsPlayed, 1);
	TextDrawSetSelectable(RoundsPlayed, 0);

	TeamAttackerTD = TextDrawCreate(503.000, 101.000, "~g~~h~[Alpha ~w~- 0~g~~h~]");
	TextDrawAlignment(TeamAttackerTD, 1);
	TextDrawBackgroundColor(TeamAttackerTD, 155);
	TextDrawFont(TeamAttackerTD, 1);
	TextDrawLetterSize(TeamAttackerTD, 0.2400, 0.8000);
	TextDrawColor(TeamAttackerTD, -1);
	TextDrawSetOutline(TeamAttackerTD, 1);
	TextDrawSetProportional(TeamAttackerTD, 1);
	TextDrawSetSelectable(TeamAttackerTD, 0);

	InitTeamAttackerTD = TextDrawCreate(503.000, 101.000, "~g~~h~[Alpha ~w~- 0~g~~h~]");
	TextDrawAlignment(InitTeamAttackerTD, 1);
	TextDrawBackgroundColor(InitTeamAttackerTD, 155);
	TextDrawFont(InitTeamAttackerTD, 1);
	TextDrawLetterSize(InitTeamAttackerTD, 0.2400, 0.8000);
	TextDrawColor(InitTeamAttackerTD, -1);
	TextDrawSetOutline(InitTeamAttackerTD, 1);
	TextDrawSetProportional(InitTeamAttackerTD, 1);
	TextDrawSetSelectable(InitTeamAttackerTD, 0);

	TeamDefenderTD = TextDrawCreate(611.500, 101.000, "~b~~h~[Beta ~w~- 0~b~~h~]");
	TextDrawAlignment(TeamDefenderTD, 3);
	TextDrawBackgroundColor(TeamDefenderTD, 155);
	TextDrawFont(TeamDefenderTD, 1);
	TextDrawLetterSize(TeamDefenderTD, 0.2400, 0.8000);
	TextDrawColor(TeamDefenderTD, -430226433);
	TextDrawSetOutline(TeamDefenderTD, 1);
	TextDrawSetProportional(TeamDefenderTD, 1);
	TextDrawSetSelectable(TeamDefenderTD, 0);

	InitTeamDefenderTD = TextDrawCreate(611.500, 101.000, "~b~~h~[Beta ~w~- 0~b~~h~]");
	TextDrawAlignment(InitTeamDefenderTD, 3);
	TextDrawBackgroundColor(InitTeamDefenderTD, 155);
	TextDrawFont(InitTeamDefenderTD, 1);
	TextDrawLetterSize(InitTeamDefenderTD, 0.2400, 0.8000);
	TextDrawColor(InitTeamDefenderTD, -430226433);
	TextDrawSetOutline(InitTeamDefenderTD, 1);
	TextDrawSetProportional(InitTeamDefenderTD, 1);
	TextDrawSetSelectable(InitTeamDefenderTD, 0);

	TeamScoreText = TextDrawCreate(542.267, 101.000, "0~g~~h~]              ~w~0~b~~h~]");
	TextDrawBackgroundColor(TeamScoreText, 155);
	TextDrawFont(TeamScoreText, 3);
	TextDrawLetterSize(TeamScoreText, 0.2400, 0.8000);
	TextDrawColor(TeamScoreText, -1);
	TextDrawSetOutline(TeamScoreText, 1);
	TextDrawSetProportional(TeamScoreText, 1);
	TextDrawSetSelectable(TeamScoreText, 0);
}


UpdateTeamNamesTextdraw()
{
	if (AllVsAllMode)
		return UpdateAllVsAllTextDraws();

	new str[64];
	format(str, sizeof(str), "%s%s", TeamDamageFlashing[ATTACKER] ? "~g~~h~~h~" : TDC[ATTACKER], TeamName[ATTACKER]);
	TextDrawSetString(attackerNameTD, str);
	TextDrawSetString(ModernRound_AttTeamName, str);
	TextDrawSetString(SimpleRound_AttTeamName, str);
	format(str, sizeof(str), "%s%s", TeamDamageFlashing[DEFENDER] ? "~b~~h~~h~~h~" : TDC[DEFENDER], TeamName[DEFENDER]);
	TextDrawSetString(defenderNameTD, str);
	TextDrawSetString(ModernRound_DefTeamName, str);
	TextDrawSetString(SimpleRound_DefTeamName, str);
	UpdateRoundTeamStatusHUD();
	return 1;
}


ShowPauseTDForAll()
{
	foreach (new i : Player)
	{
		if (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER)
			TextDrawShowForPlayer(i, PauseTD);
		else
			TextDrawHideForPlayer(i, PauseTD);
	}
	return 1;
}


ShowMatchScoreBoard(playerid = -1)
{
	switch (playerid)
	{
		case -1:
		{
			TextDrawShowForAll(RoundsPlayed);
			TextDrawHideForAll(InitTeamAttackerTD);
			TextDrawHideForAll(InitTeamDefenderTD);
			TextDrawShowForAll(TeamAttackerTD);
			TextDrawShowForAll(TeamDefenderTD);
		}
		default:
		{
			TextDrawShowForPlayer(playerid, RoundsPlayed);
			TextDrawShowForPlayer(playerid, TeamAttackerTD);
			TextDrawShowForPlayer(playerid, TeamDefenderTD);
			TextDrawHideForPlayer(playerid, InitTeamAttackerTD);
			TextDrawHideForPlayer(playerid, InitTeamDefenderTD);
		}
	}
}

HideMatchScoreBoard(playerid = -1)
{
	switch (playerid)
	{
		case -1:
		{
			TextDrawHideForAll(RoundsPlayed);
			TextDrawShowForAll(InitTeamAttackerTD);
			TextDrawShowForAll(InitTeamDefenderTD);
			TextDrawHideForAll(TeamAttackerTD);
			TextDrawHideForAll(TeamDefenderTD);
		}
		default:
		{
			TextDrawHideForPlayer(playerid, RoundsPlayed);
			TextDrawShowForPlayer(playerid, InitTeamAttackerTD);
			TextDrawShowForPlayer(playerid, InitTeamDefenderTD);
			TextDrawHideForPlayer(playerid, TeamAttackerTD);
			TextDrawHideForPlayer(playerid, TeamDefenderTD);
		}
	}
}

UpdateTeamScoreTextDraw()
{	
	if (!WarMode)
	{	
		TextDrawSetString(InitTeamAttackerTD, va_return("~g~~h~[%s ~w~- %d~g~~h~]", TeamName[ATTACKER], TeamRoundWins[ATTACKER]));
		TextDrawSetString(InitTeamDefenderTD, va_return("~b~~h~~h~[%s ~w~- %d~b~~h~]", TeamName[DEFENDER], TeamRoundWins[DEFENDER]));
		UpdateRoundsPlayedTextDraw();
	}
}

UpdateTeamNameTextDraw()
{
	TextDrawSetString(TeamAttackerTD, va_return("~g~~h~[%s ~w~- %d~g~~h~]", TeamName[ATTACKER], TeamScore[ATTACKER]));
	TextDrawSetString(TeamDefenderTD, va_return("~b~~h~~h~[%s ~w~- %d~b~~h~]", TeamName[DEFENDER], TeamScore[DEFENDER]));
}

UpdateRoundsPlayedTextDraw()
{
	if (AutoEnd)
	{
		TextDrawSetString(RoundsPlayed, va_return("~y~~h~[First to ~w~%d~y~~h~]", WinningScore));
	}
	else
	{
		TextDrawSetString(RoundsPlayed, va_return("~y~~h~[Rounds: ~w~%d~y~~h~/~w~%d~y~~h~]", CurrentRound, TotalRounds));
	}
}

