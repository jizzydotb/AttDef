#if defined _attdef_cmd_top
	#endinput
#endif
#define _attdef_cmd_top

stock ShowTopMenuDialog(playerid)
{
	return ShowPlayerDialog(
		playerid,
		DIALOG_TOP_MAIN,
		DIALOG_STYLE_LIST,
		"" COL_PRIM "Server Leaderboards (/top)",
		"{FFFFFF}1. Top All-Time Kills\n{FFFFFF}2. Top K/D Ratio (Min. 25 Kills)\n{FFFFFF}3. Top All-Time Damage\n{FFFFFF}4. Top Duels Won\n{FFFFFF}5. Top Level & Experience\n{FFFFFF}6. Top Clutches Won",
		"View",
		"Close"
	);
}

stock ShowTopCategoryDialog(playerid, category)
{
	new query[320], title[64], header[96];
	switch (category)
	{
		case 0:
		{
			format(title, sizeof(title), "" COL_PRIM "Top 15 - All-Time Kills");
			format(header, sizeof(header), "#\tPlayer\tKills\tDeaths\n");
			format(query, sizeof(query),
				"SELECT `Name`, `TKills` AS `Val1`, `TDeaths` AS `Val2` FROM `Players` WHERE `TKills` > 0 ORDER BY `TKills` DESC LIMIT 15");
		}
		case 1:
		{
			format(title, sizeof(title), "" COL_PRIM "Top 15 - K/D Ratio (Min. 25 Kills)");
			format(header, sizeof(header), "#\tPlayer\tK/D Ratio\tKills / Deaths\n");
			format(query, sizeof(query),
				"SELECT `Name`, `TKills` AS `Val1`, `TDeaths` AS `Val2` FROM `Players` WHERE `TKills` >= 25 ORDER BY (CAST(`TKills` AS REAL) / CASE WHEN `TDeaths` <= 0 THEN 1 ELSE `TDeaths` END) DESC, `TKills` DESC LIMIT 15");
		}
		case 2:
		{
			format(title, sizeof(title), "" COL_PRIM "Top 15 - All-Time Damage");
			format(header, sizeof(header), "#\tPlayer\tTotal Damage\tKills\n");
			format(query, sizeof(query),
				"SELECT `Name`, `TTDMG` AS `Val1`, `TKills` AS `Val2` FROM `Players` WHERE `TTDMG` > 0 ORDER BY `TTDMG` DESC LIMIT 15");
		}
		case 3:
		{
			format(title, sizeof(title), "" COL_PRIM "Top 15 - Duels Won");
			format(header, sizeof(header), "#\tPlayer\tDuels Won\tDuels Lost\n");
			format(query, sizeof(query),
				"SELECT `Name`, `DWon` AS `Val1`, `DLost` AS `Val2` FROM `Players` WHERE `DWon` > 0 ORDER BY `DWon` DESC, `DLost` ASC LIMIT 15");
		}
		case 4:
		{
			format(title, sizeof(title), "" COL_PRIM "Top 15 - Level & Experience");
			format(header, sizeof(header), "#\tPlayer\tLevel\tEXP\n");
			format(query, sizeof(query),
				"SELECT `Name`, `Lvl` AS `Val1`, `Exp` AS `Val2` FROM `Players` WHERE `Lvl` > 0 OR `Exp` > 0 ORDER BY `Lvl` DESC, `Exp` DESC LIMIT 15");
		}
		case 5:
		{
			format(title, sizeof(title), "" COL_PRIM "Top 15 - Clutches Won");
			format(header, sizeof(header), "#\tPlayer\tClutches Won\tKills\n");
			format(query, sizeof(query),
				"SELECT `Name`, `ClutchesWon` AS `Val1`, `TKills` AS `Val2` FROM `Players` WHERE `ClutchesWon` > 0 ORDER BY `ClutchesWon` DESC, `TKills` DESC LIMIT 15");
		}
		default:
			return 0;
	}

	new DBResult: res = DB_ExecuteQuery(sqliteconnection, query);
	if (!res)
		return SendErrorMessage(playerid, "Could not query leaderboard statistics from the database.");

	new rows = DB_GetRowCount(res);
	if (rows <= 0)
	{
		DB_FreeResultSet(res);
		return ShowPlayerDialog(
			playerid,
			DIALOG_TOP_VIEW,
			DIALOG_STYLE_MSGBOX,
			title,
			"{FFFFFF}No players have recorded statistics in this leaderboard category yet.",
			"Back",
			"Close"
		);
	}

	new body[1536];
	strcat(body, header, sizeof(body));

	new name[MAX_PLAYER_NAME], val1Str[24], val2Str[24], line[128];
	new rank = 1;
	do
	{
		DB_GetFieldStringByName(res, "Name", name, sizeof(name));
		DB_GetFieldStringByName(res, "Val1", val1Str, sizeof(val1Str));
		DB_GetFieldStringByName(res, "Val2", val2Str, sizeof(val2Str));
		new v1 = strval(val1Str);
		new v2 = strval(val2Str);

		if (category == 1)
		{
			new Float: kd = (v2 <= 0) ? float(v1) : (float(v1) / float(v2));
			format(line, sizeof(line), "{93DCAC}#%d\t{FFFFFF}%s\t{93DCAC}%.2f\t{CCCCCC}%d / %d\n", rank, name, kd, v1, v2);
		}
		else
		{
			format(line, sizeof(line), "{93DCAC}#%d\t{FFFFFF}%s\t{93DCAC}%d\t{CCCCCC}%d\n", rank, name, v1, v2);
		}
		strcat(body, line, sizeof(body));
		rank++;
	}
	while (DB_SelectNextRow(res));

	DB_FreeResultSet(res);
	return ShowPlayerDialog(
		playerid,
		DIALOG_TOP_VIEW,
		DIALOG_STYLE_TABLIST_HEADERS,
		title,
		body,
		"Back",
		"Close"
	);
}

YCMD: top(playerid, params[], help)
{
	#pragma unused params
	if (help)
		return SendCommandHelpMessage(playerid, "view the server top player leaderboards.");

	return ShowTopMenuDialog(playerid);
}

YCMD: posttop(playerid, params[], help)
{
	#pragma unused params
	if (help)
		return SendCommandHelpMessage(playerid, "force-post daily leaderboards to Discord.");
	if (Player[playerid][Level] < 3 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You are not authorized to use this command.");

	Discord_PostDailyTop();
	SendClientMessage(playerid, -1, "{93DCAC}\xBB Leaderboard has been posted to Discord.");
	return 1;
}
