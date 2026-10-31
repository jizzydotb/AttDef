#if defined _attdef_cmd_tacpause
	#endinput
#endif
#define _attdef_cmd_tacpause

YCMD: tacpause(playerid, params[], help)
{
	#pragma unused params
	if (help)
		return SendCommandHelpMessage(playerid, "call a 30-second tactical team timeout during a match (Team Leaders only, 1 per match).");

	if (!WarMode)
		return SendErrorMessage(playerid, "Tactical timeouts can only be used while Match-Mode (/match or /war) is active.");
	if (Current == -1 || (!BaseStarted && !ArenaStarted))
		return SendErrorMessage(playerid, "There is no active round in progress.");
	if (RoundPaused || RoundUnpausing)
		return SendErrorMessage(playerid, "The round is already paused or unpausing.");

	new team = Player[playerid][Team];
	if (!Player[playerid][Playing] || (team != ATTACKER && team != DEFENDER))
		return SendErrorMessage(playerid, "You must be actively playing in a team to call a tactical timeout.");

	new bool: isLeader = TeamHasLeader[team] && TeamLeader[team] == playerid;
	if (!isLeader && Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "Only the Team Leader (/lead) can call a tactical timeout.");

	if (TeamTacPauseUsed[team])
		return SendErrorMessage(playerid, "Your team has already used its tactical timeout in this match.");

	TeamTacPauseUsed[team] = true;
	TacticalPauseRemaining = TACTICAL_PAUSE_DURATION_SEC;
	PausePressed = GetTickCount();
	PauseRound();

	va_SendClientMessageToAll(
		-1,
		"%s\xBB [TACTICAL TIMEOUT] {FFFFFF}Team Leader %s%s {FFFFFF}called a %d-second tactical pause for Team %s%s{FFFFFF}!",
		TextColor[team], TextColor[team], Player[playerid][Name], TACTICAL_PAUSE_DURATION_SEC, TextColor[team], TeamName[team]
	);
	return 1;
}
