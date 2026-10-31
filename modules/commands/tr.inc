#if defined _attdef_cmd_tr
	#endinput
#endif
#define _attdef_cmd_tr

YCMD: tr(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level to do that.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "set total rounds of the current match.");
	if(!WarMode)
		return SendErrorMessage(playerid, "Match-Mode is not enabled.");

	new Value;
	if (sscanf(params, "i", Value))
		return SendUsageMessage(playerid, "/tr [Total Rounds]");

	if (Value < CurrentRound || Value < 1 || Value > 100)
		return SendErrorMessage(playerid, "Invalid total rounds.");



	TotalRounds = Value;

	WinningScore = (TotalRounds / 2) + 1; 

	UpdateRoundsPlayedTextDraw();

	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed the total rounds " COL_PRIM "to "COL_SEC"%d Rounds", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], TotalRounds);
}
