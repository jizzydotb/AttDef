#if defined _attdef_cmd_togglechat
	#endinput
#endif
#define _attdef_cmd_togglechat

YCMD: togglechat(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "enable or disable main chat.");

	LogAdminCommand("togglechat", playerid, INVALID_PLAYER_ID);

	ChatDisabled = !ChatDisabled;



	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s", PLAYER_ROLE_NAME(playerid), Player[playerid][Name],
		ChatDisabled ? "disabled main chat. "COL_PRIM"(Press 'Y' to ask for round pause)" : "enabled main chat.");
}
