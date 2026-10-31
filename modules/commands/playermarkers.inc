#if defined _attdef_cmd_playermarkers
	#endinput
#endif
#define _attdef_cmd_playermarkers

YCMD: playermarkers(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "an option to switch the color of the player markers on your radar");
	new bool: playerMarkers = !Player[playerid][PlayerMarkers];
	if (!DatabaseExecuteWrite(
		va_return("UPDATE `Players` SET `PlayerMarkers` = '%d' WHERE `Name` = '%q'", playerMarkers, Player[playerid][Name]),
		"save player marker setting"))
		return SendErrorMessage(playerid, "Could not save your player marker setting; it was not changed.");
	Player[playerid][PlayerMarkers] = playerMarkers;
	foreach (new i : Player)
	{
		CallLocalFunction("OnPlayerStreamIn", "ii", i, playerid);
	}
	if (Player[playerid][PlayerMarkers])
		return SendClientMessage(playerid, -1, "\xBB Changed player markers setting: each player has a unique marker color on radar now.");
	return SendClientMessage(playerid, -1, "\xBB Changed player markers setting: you will now see marker color based on team color.");
}
