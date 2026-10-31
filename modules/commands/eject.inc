#if defined _attdef_cmd_eject
	#endinput
#endif
#define _attdef_cmd_eject

YCMD: eject(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Use /eject [Player Name/ID] to eject a player from their vehicle.");

	new pID;
	if (sscanf(params, "u", pID))
		return SendUsageMessage(playerid, "/eject [Player Name/ID]");

	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isn't connected.");
	if (Player[pID][InDuel])
		return SendErrorMessage(playerid, "You cannot eject a player during a duel.");

	if (GetPlayerState(pID) != PLAYER_STATE_DRIVER)
		return SendErrorMessage(playerid, "The target player is not in a vehicle.");

	new vehicleid = GetPlayerVehicleID(pID);
	RemovePlayerFromVehicle(pID);

	new Float:vehiclePos[3];
	GetVehiclePos(vehicleid, vehiclePos[0], vehiclePos[1], vehiclePos[2]);
	SetPlayerPos(pID, vehiclePos[0], vehiclePos[1], vehiclePos[2] + 2.0);

	if (pID == playerid)
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has ejected himself from the vehicle.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	else
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has ejected "COL_SEC"%s "COL_PRIM"from the vehicle.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name]);
}
