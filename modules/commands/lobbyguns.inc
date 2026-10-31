#if defined _attdef_cmd_lobbyguns
	#endinput
#endif
#define _attdef_cmd_lobbyguns

YCMD: lobbyguns(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "toggle guns in lobby");
	return ToggleLobbyGuns(playerid);
}

ToggleLobbyGuns(playerid)
{

	LobbyGuns = !LobbyGuns;
	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has %s "COL_PRIM"guns in lobby.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], LobbyGuns ? "{AADC93}enabled" : "{EB8989}disabled");
}
