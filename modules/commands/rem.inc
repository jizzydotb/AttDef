#if defined _attdef_cmd_rem
	#endinput
#endif
#define _attdef_cmd_rem

YCMD: rem(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "remove yourself from the round.");
	if (!Player[playerid][Playing])
		return SendErrorMessage(playerid, "You are not playing.");
	if (ElapsedTime > 60)
		return SendErrorMessage(playerid, "Too late to remove yourself.");

	new HP[2];
	HP[0] = GetHP(playerid);
	HP[1] = GetAP(playerid);

	RemovePlayerFromRound(playerid);

	return va_SendClientMessageToAll(-1, "" COL_PRIM "\xAB (/rem) "COL_SEC"%s (%d) " COL_PRIM "removed himself from the current round. {FFFFFF}(HP: %d - %d)", Player[playerid][Name], playerid, HP[0], HP[1]);
}
