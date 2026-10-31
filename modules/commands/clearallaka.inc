#if defined _attdef_cmd_clearallaka
	#endinput
#endif
#define _attdef_cmd_clearallaka

YCMD: clearallaka(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "clear all players aka logs from database");

	if (!DatabaseExecuteWrite("DELETE FROM `AKAs`", "clear AKA logs"))
		return SendErrorMessage(playerid, "Could not clear the AKA logs.");
	return SendClientMessage(playerid, -1, ""COL_PRIM"\xBB AKA logs have been successfully cleared!");
}
