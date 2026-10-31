#if defined _attdef_cmd_limit
	#endinput
#endif
#define _attdef_cmd_limit

YCMD: limit(playerid, params[], help)
{
	//if(Player[playerid][Level] < 3 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be level 5 or rcon admin.");
	if (help)
		return SendCommandHelpMessage(playerid, "set a limit for weather and time.");



	new Command[8], aLimit, CommandID, iString[144];
	if (sscanf(params, "s[8]d", Command, aLimit))
		return SendUsageMessage(playerid, "/limit [weather | time] [Limit]");

	switch (YHash(Command, false))
	{
		case _I<weather>: CommandID = 1;
		case _I<time>: CommandID = 2;
		default: return SendUsageMessage(playerid, "/limit [weather | time] [Limit]");
	}

	if (aLimit < 10 || aLimit > 9999)
		return SendErrorMessage(playerid, "Invalid limit.");

	switch (CommandID)
	{
		case 1:
		{
			if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin weather limit update"))
				return SendErrorMessage(playerid, "Could not begin saving the weather limit.");
			foreach (new i : Player)
			{
				if (Player[i][Weather] > aLimit && !DatabaseExecuteWrite(
					va_return("UPDATE Players SET Weather = 0 WHERE Name = '%q'", Player[i][Name]),
					"save reset player weather"))
				{
					DatabaseExecuteWrite("ROLLBACK", "rollback weather limit update");
					return SendErrorMessage(playerid, "Could not save affected player weather settings. No changes were applied.");
				}
			}
			if (!DatabaseExecuteWrite("COMMIT", "commit weather limit update"))
			{
				DatabaseExecuteWrite("ROLLBACK", "rollback weather limit update");
				return SendErrorMessage(playerid, "Could not save the weather limit. No changes were applied.");
			}
			WeatherLimit = aLimit;
			foreach (new i : Player)
			{
				if (Player[i][Weather] > WeatherLimit)
				{
					Player[i][Weather] = 0;
					SetPlayerWeather(i, 0);
				}
			}
			format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed "COL_SEC"Weather limit " COL_PRIM "to "COL_SEC"%d", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], WeatherLimit);
		}
		case 2:
		{
			new resetTime = min(12, aLimit);
			if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin time limit update"))
				return SendErrorMessage(playerid, "Could not begin saving the time limit.");
			foreach (new i : Player)
			{
				if (Player[i][Time] > aLimit && !DatabaseExecuteWrite(
					va_return("UPDATE Players SET Time = %d WHERE Name = '%q'", resetTime, Player[i][Name]),
					"save reset player time"))
				{
					DatabaseExecuteWrite("ROLLBACK", "rollback time limit update");
					return SendErrorMessage(playerid, "Could not save affected player time settings. No changes were applied.");
				}
			}
			if (!DatabaseExecuteWrite("COMMIT", "commit time limit update"))
			{
				DatabaseExecuteWrite("ROLLBACK", "rollback time limit update");
				return SendErrorMessage(playerid, "Could not save the time limit. No changes were applied.");
			}
			TimeLimit = aLimit;
			foreach (new i : Player)
			{
				if (Player[i][Time] > TimeLimit)
				{
					Player[i][Time] = resetTime;
					SetPlayerTime(i, resetTime, 12);
				}
			}
			format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed "COL_SEC"Time limit " COL_PRIM "to "COL_SEC"%d", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], TimeLimit);
		}
	}
	LogAdminCommand("limit", playerid, INVALID_PLAYER_ID);
	return SendClientMessageToAll(-1, iString);
}
