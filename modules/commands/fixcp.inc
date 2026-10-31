#if defined _attdef_cmd_fixcp
	#endinput
#endif
#define _attdef_cmd_fixcp

forward SetCPForPlayerDelayed(playerid, connectionGeneration, roundGeneration, roundID, gameType);
public SetCPForPlayerDelayed(playerid, connectionGeneration, roundGeneration, roundID, gameType)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration
		|| RoundEndGeneration != roundGeneration || Current != roundID || GameType != gameType
		|| !Player[playerid][Playing])
		return 0;

	return SetCPForPlayer(playerid);
}

YCMD: fixcp(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "re-load the checkpoint for you.");
	if (GameType == ARENA && !CPInArena)
		return SendErrorMessage(playerid, "Checkpoint in arenas option is disabled in this server");
	if (RCArena)
		return SendErrorMessage(playerid, "There are no checkpoints in RC arenas!");

	if (Player[playerid][Playing])
	{
		if (!SetTimerEx(
			"SetCPForPlayerDelayed", 1000, false, "iiiii", playerid,
			PlayerConnectionGeneration[playerid], RoundEndGeneration, Current, GameType))
			return SendErrorMessage(playerid, "Could not schedule the checkpoint refresh.");
	}
	return 1;
}
