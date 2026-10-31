#if defined _attdef_cmd_lobby
	#endinput
#endif
#define _attdef_cmd_lobby

YCMD: lobby(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "teleport you to the lobby.");
	if (Player[playerid][InDuel])
		return SendErrorMessage(playerid, "Can't use this command during duel. Use /rq instead.");
	if (Player[playerid][Playing])
		return SendErrorMessage(playerid, "Cannot go to lobby while you're playing. Use /rem maybe?");

	if (Player[playerid][Spectating])
	{
		StopSpectate(playerid);
		return 1;
	}
	if (Player[playerid][InDM] || Player[playerid][DMReadd] != -1)
		return QuitDM(playerid);

	SpawnInLobby(playerid);
	return 1;
}
