#if defined _OnGeoDataReady_included
	#endinput
#endif
#define _OnGeoDataReady_included

new gGeoReadyName[MAX_PLAYERS][MAX_PLAYER_NAME];
new gGeoReadyIP[MAX_PLAYERS][16];

public OnGeoDataReady(playerid)
{
	if (!IsPlayerConnected(playerid))
		return 0;

	new currentName[MAX_PLAYER_NAME], currentIP[16];
	GetPlayerName(playerid, currentName, sizeof(currentName));
	GetPlayerIp(playerid, currentIP, sizeof(currentIP));
	if (strcmp(currentName, gGeoReadyName[playerid]) || strcmp(currentIP, gGeoReadyIP[playerid]))
		return 0;

	new CountryName[MAX_GEOIP_COUNTRY_NAME];
	new IPBlock;
	GetPlayerCountryName(playerid, CountryName);
	GetPlayerIPBlock(playerid, IPBlock);
		
	switch (IPBlock)
	{
		case 0:
		{
			va_SendClientMessageToAll(-1, "{CCCCCC}\xBB Connected: {FFFFFF}%s [ID: %d] {CCCCCC}has joined the server | {FFFFFF}%s{CCCCCC} |", Player[playerid][Name], playerid, CountryName);
		}
		case 1:
		{
			va_SendClientMessageToAll(-1, "{CCCCCC}\xBB Connected: {FFFFFF}%s [ID: %d] {CCCCCC}has joined the server | {FFFFFF}%s{CCCCCC} | {FF4954}Proxy {CCCCCC}|", Player[playerid][Name], playerid, CountryName);
			SendAdminMessage(va_return("{FF4954}\xBB Proxy/hosting IP flagged for %s (ID: %d); connection was not automatically banned.", Player[playerid][Name], playerid));
		}
		case 2: 
		{
			va_SendClientMessageToAll(-1, "{CCCCCC}\xBB Connected: {FFFFFF}%s [ID: %d] {CCCCCC}has joined the server | {FFFFFF}%s{CCCCCC} | {FF4954}Proxy? {CCCCCC}|", Player[playerid][Name], playerid, CountryName);
		}
	}
	

	UpdateModDetectionMessage(playerid);

	new adminMessage[144];
	new AKAString[144];
	if (!GetPlayerAKA(playerid, AKAString, sizeof(AKAString)))
		return 0;

	new pos = 0;
	new commaCount = 0;
	while (AKAString[pos] != EOS && commaCount < 6)
	{
		if (AKAString[pos] == ',')
			commaCount++;
		pos++;
	}

	if (commaCount >= 6)
	{
		pos = 0;
		commaCount = 0;
		while (AKAString[pos] != EOS && commaCount < 5)
		{
			if (AKAString[pos] == ',')
				commaCount++;
			pos++;
		}
		format(adminMessage, sizeof adminMessage, "{FF4954}\xBB %s (ID: %d) Aka's: %.*s and more...", Player[playerid][Name], playerid, pos, AKAString);
	}
	else
		format(adminMessage, sizeof adminMessage, "{FF4954}\xBB %s (ID: %d) Aka's: %s", Player[playerid][Name], playerid, AKAString);

	SendAdminMessage(adminMessage);
	return 1;
}
