#if defined _attdef_cmd_sync
	#endinput
#endif
#define _attdef_cmd_sync

YCMD: sync(playerid, params[], help)
{
	if (IsPlayerInAnyVehicle(playerid))
	{
		SendErrorMessage(playerid, "You cannot sync while inside a vehicle!");
		return 1;
	}
	if (help)
		return SendCommandHelpMessage(playerid, "re-sync you (restore stamina and other stuff).");
	return SyncPlayer(playerid);
}
