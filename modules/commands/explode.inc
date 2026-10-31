#if defined _attdef_cmd_explode
	#endinput
#endif
#define _attdef_cmd_explode

YCMD: explode(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 2 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "explode a specific player.");
	new eid;
	if (sscanf(params, "r", eid))
		return SendUsageMessage(playerid, "/explode [Player Name/ID]");
	if (eid == INVALID_PLAYER_ID || !IsPlayerConnected(eid))
		return SendErrorMessage(playerid, "That player isn't connected.");

	new Float: x,
		Float: y,
		Float: z;
	GetPlayerPos(eid, x, y, z);
	CreateExplosion(x, y, z, 7, 6.0);

	LogAdminCommand("explode", playerid, eid);



	// Adjusted messaging
	if (eid == playerid)
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"detonated themselves.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	}
	else
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has detonated "COL_SEC"%s"COL_PRIM".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[eid][Name]);
	}
	
}
