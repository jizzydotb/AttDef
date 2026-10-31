#if defined _attdef_cmd_aircraft
	#endinput
#endif
#define _attdef_cmd_aircraft

YCMD: aircraft(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle the aircraft spawning during rounds.");
	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is active.");

	if (!ToggleStoredConfigValue(playerid, "Aircraft", Aircraft))
		return 1;

	return va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s "COL_PRIM" has "COL_SEC"%s "COL_PRIM"aircraft spawning during rounds.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		Aircraft ? "{AADC93}enabled" : "{EB8989}disabled"
	);
}
