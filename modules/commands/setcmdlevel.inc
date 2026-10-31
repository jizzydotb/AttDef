#if defined _attdef_cmd_setcmdlevel
	#endinput
#endif
#define _attdef_cmd_setcmdlevel

YCMD: setcmdlevel(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "set the level of a command");



	new cmd[MAX_COMMAND_LENGTH], level;
	if (sscanf(params, "s[*]d", sizeof(cmd), cmd, level))
		return SendUsageMessage(playerid, "/setcmdlevel [Command name without /] [New level]");
	if (level < 0 || level >= MAX_ADMIN_LEVELS)
		return SendErrorMessage(playerid, "Invalid level!");
	if (Command_GetID(cmd) < 0)
		return SendErrorMessage(playerid, "Invalid command!");
	if (!SetCommandLevel(cmd, level, true))
		return SendErrorMessage(playerid, "Could not save the command level. No permission changes were applied.");

	if (level == 0)
		va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has permitted command "COL_SEC"/%s " COL_PRIM "for everyone's use.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], cmd);
	else
		va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has permitted command "COL_SEC"/%s " COL_PRIM "for admin level "COL_SEC"%d" COL_PRIM " and higher.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], cmd, level);
	return 1;
}
