#if defined _attdef_cmd_fakepacket
	#endinput
#endif
#define _attdef_cmd_fakepacket

forward ScheduleFakePacketRenovation(playerid, duration, bool: message, Float: fakepacket);
ScheduleFakePacketRenovation(playerid, duration, bool: message, Float: fakepacket)
{
	new renovationGeneration = ++FakePacketRenovationGeneration[playerid];
	new timer = SetTimerEx(
		"FakePacketRenovationEnd", duration, false, "ifbii",
		playerid, fakepacket, message, PlayerConnectionGeneration[playerid], renovationGeneration);
	if (!timer)
	{
		FakePacketRenovationGeneration[playerid]++;
		printf("[AttDef] Failed to schedule fake packet-loss renovation for player %d.", playerid);
		return 0;
	}

	Player[playerid][FakePacketRenovation] = true;
	return 1;
}

forward FakePacketRenovationEnd(playerid, Float: fakepacket, bool: message, connectionGeneration, renovationGeneration);
public FakePacketRenovationEnd(playerid, Float: fakepacket, bool: message, connectionGeneration, renovationGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration
		|| FakePacketRenovationGeneration[playerid] != renovationGeneration
		|| !Player[playerid][FakePacketRenovation])
		return 0;

	Player[playerid][FakePacketRenovation] = false;
	FakePacketRenovationGeneration[playerid]++;
	if (message)
	{
		new Float: currPacket = NetStats_PacketLossPercent(playerid);
		if (currPacket >= fakepacket)
		{
			new str[160];
			format(
				str, sizeof str, COL_PRIM "\xBB Fake PL renovation on "COL_SEC"%s " COL_PRIM "has failed - Old: "COL_SEC"%.2f " COL_PRIM " | Current: "COL_SEC"%.2f",
				Player[playerid][Name], fakepacket, currPacket);
			return SendClientMessageToAll(-1, str);
		}
		new str[160];
		format(
			str, sizeof str, COL_PRIM "\xBB Fake PL renovation on "COL_SEC"%s " COL_PRIM "has ended - Old: "COL_SEC"%.2f " COL_PRIM " | Current: "COL_SEC"%.2f", Player[playerid][Name],
			fakepacket, currPacket);
		return SendClientMessageToAll(-1, str);
	}
	return 1;
}

YCMD: fakepacket(playerid, params[], help)
{
	//if(Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be a higher admin level.");
	if (help)
		return SendCommandHelpMessage(playerid, "disable packet-loss status check on a player for a specific time.");

	new pID, interv;
	if (sscanf(params, "rd", pID, interv))
		return SendUsageMessage(playerid, "/fakepacket [Player Name/ID] [Time in minutes]");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isn't connected.");
	if (interv <= 0 || interv > 5)
		return SendErrorMessage(playerid, "Invalid (Min: 1 | Max: 5).");
	if (Player[pID][FakePacketRenovation])
		return SendErrorMessage(playerid, "Player is already on fake packetloss renovation.");
	if (NetStats_PacketLossPercent(pID) == 0.0)
		return SendErrorMessage(playerid, "That player has 0.0% packet-loss");

	if (!ScheduleFakePacketRenovation(pID, interv * 60 * 1000, true, NetStats_PacketLossPercent(pID)))
		return SendErrorMessage(playerid, "Could not schedule packet-loss renovation.");

	LogAdminCommand("fakepacket", playerid, pID);



	// Adjusted messaging
	if (pID == playerid) // The command is executed by the player changing their own level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has started PL renovation " COL_PRIM "on "COL_SEC"himself " COL_PRIM "- Interval: "COL_SEC"%d "COL_PRIM"min(s).", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], interv);
	}
	else // The command is executed by an admin changing another player's level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has started PL renovation " COL_PRIM "on "COL_SEC"%s " COL_PRIM "- Interval: "COL_SEC"%d "COL_PRIM"min(s).", PLAYER_ROLE_NAME(playerid), Player[playerid][Name],
		Player[pID][Name], interv);
	}	

}
