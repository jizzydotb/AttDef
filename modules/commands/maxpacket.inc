#if defined _attdef_cmd_maxpacket
	#endinput
#endif
#define _attdef_cmd_maxpacket

YCMD: maxpacket(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "set maximum packet-loss limit.");
	new Float: iPacket;
	if (sscanf(params, "f", iPacket))
		return SendUsageMessage(playerid, "/maxpacket [Maximum Packetloss]");

	if (iPacket <= 0.0 || iPacket > 10.0)
		return SendErrorMessage(playerid, "Packetloss value can be between 0 and 10 maximum.");



	if (!DatabaseExecuteWrite(
		va_return("UPDATE Configs SET Value = %.2f WHERE Option = 'Maximum Packetloss'", iPacket),
		"save maximum packet loss"))
		return SendErrorMessage(playerid, "Could not save the maximum packet-loss setting. No changes were applied.");
	Max_Packetloss = iPacket;

	new iString[144];
	LogAdminCommand("maxpacket", playerid, INVALID_PLAYER_ID);

	format(iString, sizeof(iString), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed maximum packet-loss " COL_PRIM "to "COL_SEC"%.2f PacketLoss", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], iPacket);
	return SendClientMessageToAll(-1, iString);
}
