#if defined _attdef_cmd_gamespeed
	#endinput
#endif
#define _attdef_cmd_gamespeed

YCMD: gamespeed(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "change game speed (round timer and cp timer)");
	new speed;
	if (sscanf(params, "i", speed))
	{
		SendUsageMessage(playerid, va_return("/gamespeed [Time in milliseconds, "COL_SEC"current: %d{FFFFFF}]", GAME_SPEED));
		SendClientMessage(playerid, -1, COL_PRIM "\xBB Note: "COL_SEC"this changes the speed (time per lap) of round timer and CP timer.");
		return SendClientMessage(playerid, -1, "\xBB Note: default and recommended value is 1000 milliseconds.");
	}



	if (speed < 500)
		return SendErrorMessage(playerid, "Speed can not be less than 500 milliseconds");
	if (speed > 1500)
		return SendErrorMessage(playerid, "Speed can not be more than 1500 milliseconds");

	new speedStr[12];
	if (speed == 1000)
		strcat(speedStr, "Default");
	else if (speed >= 750 && speed < 1000)
		strcat(speedStr, "Faster");
	else if (speed >= 500 && speed < 750)
		strcat(speedStr, "Very fast");
	else if (speed > 1000 && speed <= 1250)
		strcat(speedStr, "Slower");
	else
		strcat(speedStr, "Very Slow");

	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %d WHERE Option = 'GameSpeed'", speed),
		"save game speed"))
		return SendErrorMessage(playerid, "Could not save the game speed. No changes were applied.");

	GAME_SPEED = speed;
	RestartGameLoop();
	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed game speed to "COL_SEC"%d ms " COL_PRIM "("COL_SEC"%s" COL_PRIM ")", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], speed, speedStr);
}
