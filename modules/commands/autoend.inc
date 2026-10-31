#if defined _attdef_cmd_autoend
	#endinput
#endif
#define _attdef_cmd_autoend

YCMD: autoend(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Toggle automatic match ending based on first team to reach a score.");

	AutoEnd = !AutoEnd;

	if (sqliteconnection)
	{
		DatabaseExecuteWrite(
			va_return("INSERT OR REPLACE INTO Configs (Option, Value) VALUES ('AutoEnd', '%d')", AutoEnd),
			"save autoend setting");
	}

	if (AutoEnd)
	{
		WinningScore = (TotalRounds / 2) + 1;
		UpdateRoundsPlayedTextDraw();
		va_SendClientMessageToAll(
			-1,
			""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has {AADC93}enabled " COL_PRIM "Auto-End. First team to reach "COL_SEC"%d " COL_PRIM "rounds wins.",
			PLAYER_ROLE_NAME(playerid), Player[playerid][Name], WinningScore
		);
	}
	else
	{
		va_SendClientMessageToAll(
			-1,
			""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has {EB8989}disabled " COL_PRIM "Auto-End. Total match rounds are played.",
			PLAYER_ROLE_NAME(playerid), Player[playerid][Name]
		);
		UpdateRoundsPlayedTextDraw();
	}
	return 1;
}
