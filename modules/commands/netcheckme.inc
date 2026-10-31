#if defined _attdef_cmd_netcheckme
	#endinput
#endif
#define _attdef_cmd_netcheckme

forward RemoveTempNetcheck(playerid, generation, requestGeneration);
public RemoveTempNetcheck(playerid, generation, requestGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != generation
		|| TempNetcheckGeneration[playerid] != requestGeneration)
		return 0;

	Player[playerid][TempNetcheck] = false;
	TempNetcheckGeneration[playerid]++;
	return 1;
}

YCMD: netcheckme(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "temporarily netcheck yourself until an admin does it for you");
	if (Player[playerid][TempNetcheck])
		return SendErrorMessage(playerid, "Netcheck is already temporarily disabled on you");
	if (!Player[playerid][Playing])
		return SendErrorMessage(playerid, "You are not playing to request a permanent netcheck.");
	if (!Player[playerid][NetCheck])
		return SendErrorMessage(playerid, "You've already got netcheck disabled on you");

	new dummyHour, dummyMinute, dummySecond, timeStamp = gettime(dummyHour, dummyMinute, dummySecond);
	if (Player[playerid][NetCheckTimeoutTimestamp] != -1 && (timeStamp - Player[playerid][NetCheckTimeoutTimestamp]) <= 5 * 60)
		return SendErrorMessage(playerid, "You must wait 5 minutes before you can netcheck yourself again");

	new requestGeneration = ++TempNetcheckGeneration[playerid];
	if (!SetTimerEx(
		"RemoveTempNetcheck", 60000, false, "iii", playerid,
		PlayerConnectionGeneration[playerid], requestGeneration))
	{
		TempNetcheckGeneration[playerid]++;
		return SendErrorMessage(playerid, "Could not schedule temporary netcheck reset.");
	}
	Player[playerid][NetCheckTimeoutTimestamp] = timeStamp;
	Player[playerid][TempNetcheck] = true;
	SendClientMessage(playerid, -1, COL_PRIM "\xBB You've been temporarily netchecked for 60 seconds only.");
	return SendClientMessageToAll(-1, va_return(""COL_PRIM"\xBB (netcheck) "COL_SEC"%s [%d] " COL_PRIM "is requesting permanent "COL_SEC"net-check" COL_PRIM ".", Player[playerid][Name], playerid));
}
