#if defined _attdef_cmd_weather
	#endinput
#endif
#define _attdef_cmd_weather

YCMD: weather(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "change your weather.");
	new myweather, previousWeather;
	if (sscanf(params, "i", myweather))
		return SendUsageMessage(playerid, "/weather [ID]");

	if (myweather < 0 || myweather > WeatherLimit)
		return SendErrorMessage(playerid, "Invalid weather ID.");

	previousWeather = Player[playerid][Weather];
	if (!DatabaseExecuteWrite(
		va_return("UPDATE Players SET Weather = %d WHERE Name = '%q'", myweather, Player[playerid][Name]),
		"save player weather"))
		return SendErrorMessage(playerid, "Could not save your weather setting. No changes were applied.");

	SetPlayerWeather(playerid, myweather);
	Player[playerid][Weather] = myweather;
	return va_SendClientMessage(playerid, -1, "\xBB Your Weather changed to: %d (Previous Weather: %d)", myweather, previousWeather);
}
