#if defined _attdef_cmd_loadarenas
	#endinput
#endif
#define _attdef_cmd_loadarenas

YCMD: loadarenas(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Load another set of arenas from the database.");
	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is active.");

	new arenaset = 0;
	if (sscanf(params, "d", arenaset))
	{
		ShowArenaSetsDialog(playerid);
		SendClientMessage(playerid, -1, "{BD93DC}\xBB Note: {FFFFFF}Optionally you can use {BD93DC}/loadarenas {FFFFFF}<ID>");
		return 1;
	}

	return DoLoadArenas(playerid, arenaset);
}
