#if defined _attdef_cmd_vlobby
	#endinput
#endif
#define _attdef_cmd_vlobby

YCMD: vlobby(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "toggle vehicles in lobby.");
	return ToggleLobbyVehicles(playerid);
}

ToggleLobbyVehicles(playerid)
{
	LobbyVehicles = !LobbyVehicles;
	return va_SendClientMessageToAll(
		-1,
		""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"vehicles in lobby.",
		PLAYER_ROLE_NAME(playerid),
		Player[playerid][Name],
		LobbyVehicles ? "{AADC93}enabled" : "{EB8989}disabled"
	);
}
