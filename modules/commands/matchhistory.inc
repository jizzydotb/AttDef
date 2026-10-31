#if defined _attdef_cmd_matchhistory
	#endinput
#endif
#define _attdef_cmd_matchhistory

#define MAX_MATCH_HISTORY_LIST (15)
new PlayerMatchHistoryIDs[MAX_PLAYERS][MAX_MATCH_HISTORY_LIST];

stock ShowMatchHistoryDialog(playerid)
{
	new DBResult: res = DB_ExecuteQuery(
		sqliteconnection,
		"SELECT `ID`, `PlayedAt`, `TeamA`, `TeamB`, `ScoreA`, `ScoreB`, `MVP` FROM `MatchHistory` ORDER BY `ID` DESC LIMIT 15"
	);
	if (!res)
		return SendErrorMessage(playerid, "Could not load match history from the database.");

	new rows = DB_GetRowCount(res);
	if (rows <= 0)
	{
		DB_FreeResultSet(res);
		return SendErrorMessage(playerid, "No completed matches have been recorded yet.");
	}

	new body[1536];
	strcat(body, "Match\tScore\tMVP\tDate (UTC)\n", sizeof(body));

	new idStr[16], playedAt[32], teamA[24], teamB[24], scoreAStr[12], scoreBStr[12], mvp[MAX_PLAYER_NAME], line[144];
	new idx = 0;
	for (new i = 0; i < MAX_MATCH_HISTORY_LIST; i++)
		PlayerMatchHistoryIDs[playerid][i] = 0;

	do
	{
		if (idx >= MAX_MATCH_HISTORY_LIST)
			break;

		DB_GetFieldStringByName(res, "ID", idStr, sizeof(idStr));
		DB_GetFieldStringByName(res, "PlayedAt", playedAt, sizeof(playedAt));
		DB_GetFieldStringByName(res, "TeamA", teamA, sizeof(teamA));
		DB_GetFieldStringByName(res, "TeamB", teamB, sizeof(teamB));
		DB_GetFieldStringByName(res, "ScoreA", scoreAStr, sizeof(scoreAStr));
		DB_GetFieldStringByName(res, "ScoreB", scoreBStr, sizeof(scoreBStr));
		DB_GetFieldStringByName(res, "MVP", mvp, sizeof(mvp));

		PlayerMatchHistoryIDs[playerid][idx] = strval(idStr);
		format(
			line, sizeof(line),
			"{FFFFFF}%s vs %s\t{93DCAC}%d - %d\t{FFFFFF}%s\t{CCCCCC}%s\n",
			teamA, teamB, strval(scoreAStr), strval(scoreBStr), mvp, playedAt
		);
		strcat(body, line, sizeof(body));
		idx++;
	}
	while (DB_SelectNextRow(res));

	DB_FreeResultSet(res);
	return ShowPlayerDialog(
		playerid,
		DIALOG_MATCH_HISTORY,
		DIALOG_STYLE_TABLIST_HEADERS,
		"" COL_PRIM "Recent Match History",
		body,
		"Details",
		"Close"
	);
}

stock ShowMatchHistoryEntryDialog(playerid, listitem)
{
	if (listitem < 0 || listitem >= MAX_MATCH_HISTORY_LIST || PlayerMatchHistoryIDs[playerid][listitem] <= 0)
		return ShowMatchHistoryDialog(playerid);

	new matchID = PlayerMatchHistoryIDs[playerid][listitem];
	new DBResult: res = DB_ExecuteQuery(
		sqliteconnection,
		va_return(
			"SELECT `PlayedAt`, `TeamA`, `TeamB`, `ScoreA`, `ScoreB`, `Winner`, `MVP`, `Top1`, `Top2`, `Top3`, `RoundsPlayed` FROM `MatchHistory` WHERE `ID` = %d",
			matchID
		)
	);
	if (!res)
		return SendErrorMessage(playerid, "Could not load the selected match details.");

	if (DB_GetRowCount(res) <= 0)
	{
		DB_FreeResultSet(res);
		return SendErrorMessage(playerid, "That match record was not found.");
	}

	new playedAt[32], teamA[24], teamB[24], scoreAStr[12], scoreBStr[12], winner[32], mvp[MAX_PLAYER_NAME];
	new top1[128], top2[128], top3[128], roundsStr[12];
	DB_GetFieldStringByName(res, "PlayedAt", playedAt, sizeof(playedAt));
	DB_GetFieldStringByName(res, "TeamA", teamA, sizeof(teamA));
	DB_GetFieldStringByName(res, "TeamB", teamB, sizeof(teamB));
	DB_GetFieldStringByName(res, "ScoreA", scoreAStr, sizeof(scoreAStr));
	DB_GetFieldStringByName(res, "ScoreB", scoreBStr, sizeof(scoreBStr));
	DB_GetFieldStringByName(res, "Winner", winner, sizeof(winner));
	DB_GetFieldStringByName(res, "MVP", mvp, sizeof(mvp));
	DB_GetFieldStringByName(res, "Top1", top1, sizeof(top1));
	DB_GetFieldStringByName(res, "Top2", top2, sizeof(top2));
	DB_GetFieldStringByName(res, "Top3", top3, sizeof(top3));
	DB_GetFieldStringByName(res, "RoundsPlayed", roundsStr, sizeof(roundsStr));
	DB_FreeResultSet(res);

	new details[768];
	format(
		details, sizeof(details),
		"{93DCAC}Match #{FFFFFF}%d\n{93DCAC}Date (UTC): {FFFFFF}%s\n{93DCAC}Teams: {FFFFFF}%s (%d) vs %s (%d)\n{93DCAC}Winner: {FFFFFF}%s\n{93DCAC}Rounds Played: {FFFFFF}%d\n{93DCAC}Match MVP: {FFFFFF}%s\n\n{93DCAC}Top Performers:\n{FFFFFF}%s\n{FFFFFF}%s\n{FFFFFF}%s",
		matchID,
		playedAt,
		teamA, strval(scoreAStr), teamB, strval(scoreBStr),
		winner,
		strval(roundsStr),
		mvp,
		strlen(top1) ? top1 : "1st: N/A",
		strlen(top2) ? top2 : "2nd: N/A",
		strlen(top3) ? top3 : "3rd: N/A"
	);

	return ShowPlayerDialog(
		playerid,
		DIALOG_MATCH_HISTORY_VIEW,
		DIALOG_STYLE_MSGBOX,
		"" COL_PRIM "Match Summary Details",
		details,
		"Back",
		"Close"
	);
}

YCMD: matchhistory(playerid, params[], help)
{
	#pragma unused params
	if (help)
		return SendCommandHelpMessage(playerid, "view the history of recently completed matches.");

	return ShowMatchHistoryDialog(playerid);
}
