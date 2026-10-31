#if defined _attdef_OnRconLoginAttempt
	#endinput
#endif
#define _attdef_OnRconLoginAttempt

public OnRconLoginAttempt(ip[], password[], success)
{
	#pragma unused password
	new Str[144], iName[MAX_PLAYER_NAME], playerid = INVALID_PLAYER_ID, IP[16], matches;
	foreach (new i : Player)
	{
		GetPlayerIp(i, IP, sizeof(IP));
		if (!strcmp(IP, ip))
		{
			playerid = i;
			matches++;
		}
	}

	if (matches > 1 && success)
	{
		new newRconMatches = 0, newRconId = INVALID_PLAYER_ID;
		foreach (new i : Player)
		{
			GetPlayerIp(i, IP, sizeof(IP));
			if (!strcmp(IP, ip) && IsPlayerAdmin(i) && Player[i][Level] < 5)
			{
				newRconMatches++;
				newRconId = i;
			}
		}
		if (newRconMatches == 1)
		{
			playerid = newRconId;
			matches = 1;
		}
		else
		{
			new adminMatches = 0, lastAdminId = INVALID_PLAYER_ID;
			foreach (new i : Player)
			{
				GetPlayerIp(i, IP, sizeof(IP));
				if (!strcmp(IP, ip) && IsPlayerAdmin(i))
				{
					adminMatches++;
					lastAdminId = i;
				}
			}
			if (adminMatches == 1)
			{
				playerid = lastAdminId;
				matches = 1;
			}
			else
			{
				new nonAdminMatches = 0, candidateId = INVALID_PLAYER_ID;
				foreach (new i : Player)
				{
					GetPlayerIp(i, IP, sizeof(IP));
					if (!strcmp(IP, ip) && Player[i][Level] < 5)
					{
						nonAdminMatches++;
						candidateId = i;
					}
				}
				if (nonAdminMatches == 1)
				{
					playerid = candidateId;
					matches = 1;
				}
			}
		}
	}

	if (matches != 1)
	{
		if (success)
		{
			foreach (new i : Player)
			{
				GetPlayerIp(i, IP, sizeof(IP));
				if (!strcmp(IP, ip) && IsPlayerAdmin(i) && Player[i][Level] < 5)
				{
					Player[i][Level] = 5;
					UpdatePlayerAdminGroup(i);
					if (Player[i][Logged])
					{
						DatabaseExecuteWrite(
							va_return("UPDATE Players SET Level = 5 WHERE Name = '%q'", Player[i][Name]),
							"save RCON admin level");
					}
				}
			}
		}
		printf("(RCON) Login attempt from %s could not be matched to one connected player.", ip);
		return 1;
	}

	GetPlayerName(playerid, iName, sizeof(iName));
	if (!success)
	{
		format(Str, sizeof(Str), "" COL_PRIM "\xBB (RCON) " COL_SEC "%s " COL_PRIM "failed RCON login attempt.", iName);
		SendClientMessageToAll(-1, Str);

		Player[playerid][RconTry]++;

		if (Player[playerid][RconTry] >= 2)
		{
			format(Str, sizeof(Str), "" COL_PRIM "\xBB (RCON) " COL_SEC "%s " COL_PRIM "was kicked for repeated failed RCON attempts.", iName);
			SendClientMessageToAll(-1, Str);
			Kick(playerid);
			return 1;
		}
		else
			SendClientMessage(playerid, -1, "\xBB Warning: One more incorrect password attempt will result in a kick.");
	}
	else
	{
		Player[playerid][RconTry] = 0;
		new previousLevel = Player[playerid][Level];
		if (Player[playerid][Level] != 5)
		{
			Player[playerid][Level] = 5;
			UpdatePlayerAdminGroup(playerid);

			format(Str, sizeof(Str), "" COL_PRIM "\xBB (RCON) " COL_SEC "%s " COL_PRIM "authenticated as RCON Administrator (Level 5).", iName);
		}
		else
			format(Str, sizeof(Str), "" COL_PRIM "\xBB (RCON) " COL_SEC "%s " COL_PRIM "authenticated as RCON.", iName);

		if (Player[playerid][Logged])
		{
			DatabaseExecuteWrite(
				va_return("UPDATE Players SET Level = 5 WHERE Name = '%q'", Player[playerid][Name]),
				"save RCON admin level");
		}

		foreach (new j : Player)
		{
			if (Player[j][Level] > 4)
				SendClientMessage(j, -1, Str);
		}
		if (previousLevel == 0)
		{
			SendClientMessage(playerid, -1, COL_PRIM "\xBB Administrator access granted. Type " COL_SEC "/acmds " COL_PRIM "to view admin commands.");
		}
	}
	return 1;
}
