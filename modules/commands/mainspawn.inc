#if defined _attdef_cmd_mainspawn
	#endinput
#endif
#define _attdef_cmd_mainspawn

YCMD: mainspawn(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 4 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "change server main spawn position.");

	new Float: spawn[4], interior = GetPlayerInterior(playerid);
	GetPlayerPos(playerid, spawn[0], spawn[1], spawn[2]);
	GetPlayerFacingAngle(playerid, spawn[3]);

	new iString[128];
	format(iString, sizeof(iString), "%.0f,%.0f,%.0f,%.0f,%d", spawn[0], spawn[1], spawn[2], spawn[3], interior);
	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = '%q' WHERE Option = 'Main Spawn'", iString),
		"save main spawn"))
		return SendErrorMessage(playerid, "Could not save the main spawn. No changes were applied.");

	DestroyMainSpawnText3D();
	MainSpawn[0] = spawn[0];
	MainSpawn[1] = spawn[1];
	MainSpawn[2] = spawn[2];
	MainSpawn[3] = spawn[3];
	MainInterior = interior;

	LogAdminCommand("mainspawn", playerid, INVALID_PLAYER_ID);

	CreateMainSpawnText3D();



	format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has updated the lobby spawn position.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	return SendClientMessageToAll(-1, iString);
}
