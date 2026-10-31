#if defined _attdef_cmd_loadbases
	#endinput
#endif
#define _attdef_cmd_loadbases

YCMD: loadbases(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Load another set of bases from the database.");
	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is active.");

	new baseset = 0;
	if (sscanf(params, "d", baseset))
	{
		ShowBaseSetsDialog(playerid);
		SendClientMessage(playerid, -1, "{BD93DC}\xBB Note: {FFFFFF}Optionally you can use {BD93DC}/loadbases {FFFFFF}<ID>");
		return 1;
	}

	return DoLoadBases(playerid, baseset);
}
