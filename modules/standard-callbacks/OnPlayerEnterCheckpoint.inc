#if defined _attdef_OnPEnterCheckpoint
	#endinput
#endif
#define _attdef_OnPEnterCheckpoint

public OnPlayerEnterCheckpoint(playerid)
{
	if (GetPlayerVehicleID(playerid) == 0 && Player[playerid][Playing] && (Player[playerid][Team] == ATTACKER || Player[playerid][Team] == DEFENDER))
	{
		if (Player[playerid][WasInCP])
			return 1;

		switch (GameType)
		{
			case BASE:
			{
				switch (Player[playerid][Team])
				{
					case ATTACKER:
					{
						new Float: attPos[3];
						GetPlayerPos(playerid, attPos[0], attPos[1], attPos[2]);
						if (attPos[2] <= (BCPSpawn[Current][2] - 1.4))
							return 1;

						PlayersInCP++;
						Player[playerid][WasInCP] = true;

						new iString[256];
						format(iString, sizeof iString, "~r~Players In CP");
						foreach (new i : Player)
						{
							if (Player[i][WasInCP])
							{
								new playerLine[64];
								format(playerLine, sizeof(playerLine), "~n~~r~- %s%s", MAIN_TEXT_COLOUR, Player[i][Name]);
								strcat(iString, playerLine, sizeof(iString));
							}
						}
						TextDrawSetString(EN_CheckPoint, iString);
						TextDrawColor(timerCircleTD, 0xFF616133);
						GangZoneFlashForAll(CPZone, GANGZONE_FLASH);
						foreach (new i : Player)
						{
							if (!Player[i][Spawned])
								continue;

							if (Player[i][RoundTDStyle] != 1)
								TextDrawHideForPlayer(i, EN_CheckPoint);
							else
								TextDrawShowForPlayer(i, EN_CheckPoint);

							if (Player[i][RoundTDStyle] != 1)
								continue;

							TextDrawShowForPlayer(i, timerCircleTD);
						}
					}
					case DEFENDER:
					{
						new Float: defPos[3];
						GetPlayerPos(playerid, defPos[0], defPos[1], defPos[2]);
						if (defPos[2] >= BCPSpawn[Current][2] - 1.4)
							CurrentCPTime = ConfigCPTime + 1;
						else if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK)
						{
							if (CurrentCPTime < ConfigCPTime)
								SendClientMessageToAll(-1, va_return(COL_PRIM "\xBB CP touch by "COL_SEC"%s " COL_PRIM "is denied due to abusing jetpack.", Player[playerid][Name]));
						}
						else if (CurrentCPTime < ConfigCPTime)
							SendClientMessageToAll(
								-1,
								va_return(COL_PRIM "\xBB CP touch by "COL_SEC"%s " COL_PRIM "is denied. This might be considered as cheating or bug abusing.", Player[playerid][Name]));
					}
				}
			}
			case ARENA:
			{
				if (!ArenaStarted || !CPInArena)
					return 1;

				switch (TeamCapturingCP)
				{
					case NON: // no one is taking CP
					{
						new Float: attPos[3];
						GetPlayerPos(playerid, attPos[0], attPos[1], attPos[2]);
						if (attPos[2] <= (ACPSpawn[Current][2] - 1.4))
							return 1;

						TeamCapturingCP = Player[playerid][Team];
						PlayersInCP++;
						Player[playerid][WasInCP] = true;

						new iString[256];
						format(iString, sizeof iString, "%sPlayers In CP", TDC[Player[playerid][Team]]);
						foreach (new i : Player)
						{
							if (Player[i][WasInCP] && Player[playerid][Team] == Player[i][Team])
							{
								new playerLine[64];
								format(playerLine, sizeof(playerLine), "~n~%s- %s%s", TDC[Player[i][Team]], MAIN_TEXT_COLOUR, Player[i][Name]);
								strcat(iString, playerLine, sizeof(iString));
							}
						}
						TextDrawSetString(EN_CheckPoint, iString);
						switch (Player[playerid][Team])
						{
							case ATTACKER: TextDrawColor(timerCircleTD, 0xFF616133);
							case DEFENDER: TextDrawColor(timerCircleTD, 0x9698FF33);
						}
						foreach (new i : Player)
						{
							if (!Player[i][Spawned])
								continue;

							if (Player[i][RoundTDStyle] != 1)
								TextDrawHideForPlayer(i, EN_CheckPoint);
							else
								TextDrawShowForPlayer(i, EN_CheckPoint);

							if (Player[i][RoundTDStyle] != 1)
								continue;

							TextDrawShowForPlayer(i, timerCircleTD);
						}
					}
					default: // cp is being taken by some team
					{
						if (TeamCapturingCP == Player[playerid][Team])
						{
							new Float: attPos[3];
							GetPlayerPos(playerid, attPos[0], attPos[1], attPos[2]);
							if (attPos[2] <= (ACPSpawn[Current][2] - 1.4))
								return 1;

							PlayersInCP++;
							Player[playerid][WasInCP] = true;

							new iString[256];
							format(iString, sizeof iString, "%sPlayers In CP", TDC[Player[playerid][Team]]);
							foreach (new i : Player)
							{
								if (Player[i][WasInCP] && Player[playerid][Team] == Player[i][Team])
								{
									new playerLine[64];
									format(playerLine, sizeof(playerLine), "~n~%s- %s%s", TDC[Player[i][Team]], MAIN_TEXT_COLOUR, Player[i][Name]);
									strcat(iString, playerLine, sizeof(iString));
								}
							}
							TextDrawSetString(EN_CheckPoint, iString);
							foreach (new i : Player)
							{
								if (!Player[i][Spawned])
									continue;

								if (Player[i][RoundTDStyle] != 1)
									TextDrawHideForPlayer(i, EN_CheckPoint);
								else
									TextDrawShowForPlayer(i, EN_CheckPoint);
							}
						}
						else
						{
							new Float: defPos[3];
							GetPlayerPos(playerid, defPos[0], defPos[1], defPos[2]);
							if (defPos[2] >= ACPSpawn[Current][2] - 1.4)
								CurrentCPTime = ConfigCPTime + 1;
							else if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK)
							{
								if (CurrentCPTime < ConfigCPTime)
									SendClientMessageToAll(-1, va_return(COL_PRIM "\xBB CP touch by "COL_SEC"%s " COL_PRIM "is denied due to abusing jetpack.", Player[playerid][Name]));
							}
							else if (CurrentCPTime < ConfigCPTime)
								SendClientMessageToAll(
									-1, va_return(
											COL_PRIM "\xBB CP touch by "COL_SEC"%s " COL_PRIM "is denied. This might be considered as cheating or bug abusing.", Player[playerid][Name]));
						}
					}
				}
			}
		}
	}
	UpdateRoundTeamStatusMessages();
	UpdateModernAndSimpleCheckpointHUD();
	return 1;
}
