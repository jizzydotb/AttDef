#if defined _attdef_cmd_goto
	#endinput
#endif
#define _attdef_cmd_goto

YCMD: goto(playerid, params[], help)
{
	if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You Need To Be An Admin.");
	if (help)
		return SendCommandHelpMessage(playerid, "teleport you to a player.");
	if (Player[playerid][InDuel])
		return SendErrorMessage(playerid, "Leave the duel with /rq before teleporting.");
	new gid;
	if (sscanf(params, "r", gid))
		return SendUsageMessage(playerid, "/goto [Player Name/ID]");

	if (gid == INVALID_PLAYER_ID || !IsPlayerConnected(gid))
		return SendErrorMessage(playerid, "Player isn't connected.");
	if (gid == playerid)
		return SendErrorMessage(playerid, "Can't go to yourself.");
	new Float: x,
		Float: y,
		Float: z;
	GetPlayerPos(gid, x, y, z);
	SetPlayerInterior(playerid, GetPlayerInterior(gid));
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(gid));

	if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		SetVehiclePos(GetPlayerVehicleID(playerid), x + 2, y, z);
		LinkVehicleToInterior(GetPlayerVehicleID(playerid), GetPlayerInterior(gid));
		SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), GetPlayerVirtualWorld(gid));
	}
	else
		SetPlayerPos(playerid, x + 1, y, z);

	LogAdminCommand("goto", playerid, gid);



	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has teleported to "COL_SEC"%s"COL_PRIM".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[gid][Name]);
}
