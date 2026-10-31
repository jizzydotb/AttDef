#if defined _attdef_OnDResponse
	#endinput
#endif
#define _attdef_OnDResponse

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{

	if (dialogid == DIALOG_NO_RESPONSE)
		return 1;
	switch (dialogid)
	{
		case DIALOG_GUNMENU_MELEE:
		{
			if (response)
			{
				new weap = MeleeWeaponsArray_ID[listitem], ammo = MeleeWeaponsArray_AMMO[listitem];

				if (DoesPlayerHaveWeapon(playerid, weap))
				{
					RemovePlayerWeapon(playerid, weap);
				}
				else
				{
					GivePlayerWeapon(playerid, weap, ammo);
				}
				if (!SetTimerEx(
					"ShowPlayerMeleeWeaponsMenuDelayed", GetPlayerPing(playerid) + 200, false,
					"ii", playerid, PlayerConnectionGeneration[playerid]))
				{
					printf("[AttDef] Failed to schedule the melee weapons menu for player %d.", playerid);
					SendErrorMessage(playerid, "Could not reopen the melee weapons menu. Please try again.");
				}
			}
			return 1;
		}
		case DIALOG_WEAPONBIND_MAIN:
		{
			if (response)
			{
				if (listitem == 0) // Toggle
				{
					new bool: enabled = !Player[playerid][WeaponBinding];
					if (!DatabaseExecuteWrite(
						va_return("UPDATE Players SET WeaponBinding = %d WHERE Name = '%q'", enabled, Player[playerid][Name]),
						"save weapon binding"))
						return SendErrorMessage(playerid, "Could not save your weapon binding setting.");

					Player[playerid][WeaponBinding] = enabled;
					va_SendClientMessage(playerid, -1, "" COL_PRIM "Weapon binding %s" COL_PRIM ".", enabled ? "{33CC00}enabled" : "{CC0000}disabled");
					return ShowPlayerWeaponBindDialog(playerid);
				}
				new index = listitem - 1;
				if (index < 0 || index >= MAX_WEAPON_BINDS)
					return SendErrorMessage(playerid, "Invalid weapon binding.");

				WeaponBindData[playerid][ModifyingWeaponBindIndex] = index;
				new str[160];
				format(
					str, sizeof str, "Key: %s+%s\nCurrent weapon: %s\n\nType weapon name or ID below to code this key bind for it:", WEAPON_BIND_PRIMARY_KEY_TEXT_ALT,
					WeaponBindKeyText_ALT[index], ReturnWeaponName(WeaponBindData[playerid][BindKeyWeapon][index]));
				ShowPlayerDialog(playerid, DIALOG_WEAPONBIND_MODIFY, DIALOG_STYLE_INPUT, "Coding weapon key bind", str, "Save", "Back");
			}
			return 1;
		}
		case DIALOG_WEAPONBIND_MODIFY:
		{
			if (response)
			{
				if (isnull(inputtext))
				{
					ShowPlayerWeaponBindDialog(playerid);
					return 1;
				}

				new weaponid;
				if (IsNumeric(inputtext))
					weaponid = strval(inputtext);
				else
					weaponid = GetWeaponID(inputtext);

				if (weaponid == 0 || IsValidWeapon(weaponid))
				{
					new index = WeaponBindData[playerid][ModifyingWeaponBindIndex];
					if (index < 0 || index >= MAX_WEAPON_BINDS)
						return SendErrorMessage(playerid, "Invalid weapon binding.");

					if (!DatabaseExecuteWrite(
						va_return("UPDATE Players SET WeaponBind%d = %d WHERE Name = '%q'", index, weaponid, Player[playerid][Name]),
						"save weapon binding"))
						return SendErrorMessage(playerid, "Could not save your weapon binding.");

					WeaponBindData[playerid][BindKeyWeapon][index] = weaponid;
					va_SendClientMessage(
						playerid, -1, "" COL_PRIM "Weapon bind " COL_SEC "%s+%s " COL_PRIM "set to " COL_SEC "%s" COL_PRIM ".",
						WEAPON_BIND_PRIMARY_KEY_TEXT_ALT, WeaponBindKeyText_ALT[index], ReturnWeaponName(weaponid));
					ShowPlayerWeaponBindDialog(playerid);
				}
				else
				{
					SendErrorMessage(playerid, "Invalid weapon ID or name.");
					ShowPlayerWeaponBindDialog(playerid);
				}
			}
			else
			{
				ShowPlayerWeaponBindDialog(playerid);
			}
			return 1;
		}
		case DIALOG_REPLACE_FIRST:
		{
			if (response)
			{
				new ToAddID = -1;
				foreach (new i : Player)
				{
					if (!strcmp(Player[i][Name], inputtext, false, strlen(inputtext)))
					{
						ToAddID++;
						REPLACE_ToAddID[playerid] = i;
						REPLACE_ToAddGeneration[playerid] = PlayerConnectionGeneration[i];
						REPLACE_RequestRound[playerid] = Current;
						REPLACE_RequestGeneration[playerid]++;
						break;
					}
				}
				if (ToAddID > -1)
				{
					new str[2048];
					foreach (new i : Player)
					{
						if (!Player[i][Playing])
							continue;

						format(str, sizeof str, "%s%s\n", str, Player[i][Name]);
					}
					for (new i = 0; i < SAVE_SLOTS; i++)
					{
						if (strlen(SaveVariables[i][pName]) > 2 && SaveVariables[i][RoundID] == Current && SaveVariables[i][ToBeAdded] == true)
						{
							format(str, sizeof str, "%s%s\n", str, SaveVariables[i][pName]);
						}
					}
					ShowPlayerDialog(playerid, DIALOG_REPLACE_SECOND, DIALOG_STYLE_LIST, "" COL_PRIM "Player to replace", str, "Process", "Cancel");
				}
				else
					SendErrorMessage(playerid, "Player not found.");
			}
			return 1;
		}
		case DIALOG_REPLACE_SECOND:
		{
			if (response)
			{
				new ToReplaceID = -1;
				foreach (new i : Player)
				{
					if (!strcmp(Player[i][Name], inputtext, false, strlen(inputtext)))
					{
						ToReplaceID = i;
						break;
					}
				}
				if (ToReplaceID != -1)
				{
					new ToAddID = REPLACE_ToAddID[playerid];
					if (!IsPlayerConnected(ToAddID)
						|| PlayerConnectionGeneration[ToAddID] != REPLACE_ToAddGeneration[playerid]
						|| Current != REPLACE_RequestRound[playerid])
					{
						return SendErrorMessage(playerid, "The replacement selection is no longer valid.");
					}

					if (Player[ToAddID][InDuel] == true)
						return SendErrorMessage(playerid, "That player is in a duel."); //duel

					if (!SetTimerEx(
						"OnPlayerReplace", 500, false, "iiiiiiii", ToAddID, ToReplaceID, playerid,
						PlayerConnectionGeneration[ToAddID], PlayerConnectionGeneration[ToReplaceID],
						PlayerConnectionGeneration[playerid], REPLACE_RequestGeneration[playerid], REPLACE_RequestRound[playerid]))
						return SendErrorMessage(playerid, "Could not schedule the replacement. Please try again.");
				}
				else
				{
					for (new i = 0; i < SAVE_SLOTS; i++)
					{
						if (strlen(SaveVariables[i][pName]) > 2 && !strcmp(SaveVariables[i][pName], inputtext, false, strlen(inputtext)) && SaveVariables[i][RoundID] == Current)
						{
							ToReplaceID = i;
							break;
						}
					}
					if (ToReplaceID > -1)
					{
						new ToAddID = REPLACE_ToAddID[playerid];
						if (!IsPlayerConnected(ToAddID)
							|| PlayerConnectionGeneration[ToAddID] != REPLACE_ToAddGeneration[playerid]
							|| Current != REPLACE_RequestRound[playerid])
						{
							return SendErrorMessage(playerid, "The replacement selection is no longer valid.");
						}

						if (Player[ToAddID][InDuel] == true)
							return SendErrorMessage(playerid, "That player is in a duel."); //duel
						if (!SetTimerEx(
							"OnPlayerInGameReplace", 500, false, "iiiiiii", ToAddID, ToReplaceID, playerid,
							PlayerConnectionGeneration[ToAddID], PlayerConnectionGeneration[playerid],
							REPLACE_RequestGeneration[playerid], REPLACE_RequestRound[playerid]))
							return SendErrorMessage(playerid, "Could not schedule the replacement. Please try again.");
					}
					else
						SendErrorMessage(playerid, "Player not found.");
				}
			}
			return 1;
		}
		case PLAYERCLICK_DIALOG:
		{
			if (response)
			{
				if (!IsPlayerConnected(LastClickedPlayer[playerid]))
					return SendErrorMessage(playerid, "That player is not connected anymore!");

				switch (listitem)
				{
					case 0:
					{
						new statsSTR[8][400], namee[60], CID, Country[MAX_GEOIP_COUNTRY_NAME], CountryCode[MAX_GEOIP_COUNTRY_CODE], techInfo[200];
						CID = LastClickedPlayer[playerid];

						format(namee, sizeof(namee), "{FF3333}Player {FFFFFF}%s {FF3333}Info", Player[CID][Name]);
						GetPlayerCountryName(CID, Country);
						GetPlayerCountryCode(CID, CountryCode);

						new TD = Player[CID][TotalDeaths];
						new MC = Player[playerid][ChatChannel];
						new YC = Player[CID][ChatChannel];
						new sampversion[24];
						GetPlayerVersion(CID, sampversion, sizeof(sampversion));
						new Float: ratio;
						new Float: ratioglobal;
						new nextLevelExp = GetNextLevelExp(Player[CID][Lvl]);
						new playTimeHours = Player[CID][PlayTime] / 60; // Convert play time to hours
						new playTimeMinutes = Player[CID][PlayTime] % 60;
						new playerSkin = Player[CID][pSkin];
						new skinString[64];
						new dmIdString[20];
						new chatChannelString[20];

						if (Player[CID][DMReadd] == -1)
						{
							format(dmIdString, sizeof(dmIdString), "None");
						}
						else
						{
							format(dmIdString, sizeof(dmIdString), "%d", Player[CID][DMReadd]);
						}

						if (MC == -1)
						{
							format(chatChannelString, sizeof(chatChannelString), "Global");
						}
						else
						{
							format(chatChannelString, sizeof(chatChannelString), "%d", YC);
						}

						if (TD == 0)
						{
							ratio = 0.0;
						}
						else
						{
							ratio = float(Player[CID][TotalKills]) / float(Player[CID][TotalDeaths]);
						}

						if (Player[CID][ATimeDeaths] == 0)
						{
							ratioglobal = 0.0;
						}
						else
						{
							ratioglobal = float(Player[CID][ATimeKills]) / float(Player[CID][ATimeDeaths]);
						}

						if (playerSkin == -1)
						{
							format(skinString, sizeof(skinString), "Default Team Skin");
						}
						else
						{
							format(skinString, sizeof(skinString), "%d", playerSkin);
						}

						if (Player[playerid][Level] > 4 && !IsPlayerAdmin(playerid))
						{
							new str[35];
							GetPlayerIp(CID, str, sizeof(str));
							format(techInfo, sizeof techInfo, "" COL_PRIM "\n{FFFFFF}IP: %s\n", str);
						}

						format(
							statsSTR[1], sizeof(statsSTR[]),
							COL_PRIM "{FFFFFF}Country: %s, %s\nServer Level: %s (%d)\nSA-MP Client: %s\nSkin ID: %s\nOnline Time: %d hours %d minutes\n\n[>Round Stats<]\t[>Match Stats<]\t\t[>AAD Stats<]\nIn Round: %s\t\tRatio:\t%.2f\t\tExp: %d/%d\n"COL_PRIM"{FFFFFF}Kills:\t%d\t\t"COL_PRIM"{FFFFFF}Kills:\t%d\t\tLevel: %d\nAcc:\t%.0f%%\t\tDeaths:\t%d\t\tRatio: %.2f\n",
							Country, CountryCode, PLAYER_ROLE_NAME(playerid), Player[CID][Level], sampversion, skinString, playTimeHours, playTimeMinutes, (Player[CID][Playing] == true ? ("Yes") : ("No")), ratio, Player[CID][Exp], nextLevelExp, Player[CID][RoundKills], Player[CID][TotalKills], Player[CID][Lvl], Player[CID][Accuracy], TD, ratioglobal);
						format(
							statsSTR[2], sizeof(statsSTR[]),
							COL_PRIM "\n{FFFFFF}DMG:\t%d\t\t" COL_PRIM "{FFFFFF}DMG:\t%d\t\tDMG: %d\n\n",
							floatround(Player[CID][RoundDamage]), floatround(Player[CID][TotalDamage]), Player[CID][ATimeDMG]);
						format(
							statsSTR[3], sizeof(statsSTR[]),
							COL_PRIM "\n[>Performance<]\t[>Server Settings<]\t[>Mods<]\nPing:\t%d\t\tTime:\t%d\t\tCleo: %s\nFPS:\t%d\t\tWeather: %d\t\tExtraWS: %s\nPacket:\t%.1f\t\tDM ID:\t%s\t\tSampFuncs: %s\nNetcheck: %s\t\tChat:\t%s\t\tSprinthook: %s\n\n{FFFFFF}",
							GetPlayerPing(CID), Player[CID][Time], Player[CID][UsingCleo] ? "Yes" : "No", GetPlayerFPS(CID), Player[CID][Weather], Player[CID][UsingExtraws] ? "Yes" : "No", NetStats_PacketLossPercent(CID), dmIdString, Player[CID][UsingSampfuncs] ? "Yes" : "No", (Player[CID][NetCheck] ? ("On") : ("Off")), chatChannelString, Player[CID][UsingSprintHook] ? "Yes" : "No");
						format(
							statsSTR[4], sizeof(statsSTR[]), "\t\t\t[>Global Stats<]\n\n{FFFFFF}Duels Won:\t%d\tM4 Kills:\t%d\tSniper Kills: %d\nDuels Lost:\t%d\tSpas Kills:\t%d\tRifle Kills: %d\n{FFFFFF}All-Kills:\t%d\t{FFFFFF}Deagle Kills:\t%d\t\n{FFFFFF}All-Deaths:\t%d\t{FFFFFF}Shotgun Kills:\t%d\t\nAll-DMG:\t%d\tAK-47 Kills:\t%d\nAll-Assists:\t%d\tClutches Won:\t%d\n",
							Player[CID][DuelsWon], Player[CID][AKillM], Player[CID][AKillSN], Player[CID][DuelsLost], Player[CID][AKillSP], Player[CID][AKillPNCH], Player[CID][ATimeKills], Player[CID][AKillDGL], Player[CID][ATimeDeaths], Player[CID][AKillSHT], Player[CID][ATimeDMG], Player[CID][AKillAK], Player[CID][ATimeAssists], Player[CID][ClutchesWon]);
						
						new TotalStr[3000];
						format(TotalStr, sizeof(TotalStr), "%s%s%s%s%s%s%s%s%s", techInfo, statsSTR[0], statsSTR[1], statsSTR[2], statsSTR[3], statsSTR[4], statsSTR[5], statsSTR[6], statsSTR[7]);

						ShowPlayerDialog(playerid, DIALOG_CLICK_STATS, DIALOG_STYLE_MSGBOX, namee, TotalStr, "Close", "");
					}
					case 1:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/aka %d", LastClickedPlayer[playerid]));
					}
					case 2:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/spec %d", LastClickedPlayer[playerid]));
					}
					case 3:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/add %d", LastClickedPlayer[playerid]));
					}
					case 4:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/remove %d", LastClickedPlayer[playerid]));
					}
					case 5:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/readd %d", LastClickedPlayer[playerid]));
					}
					case 6:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/givemenu %d", LastClickedPlayer[playerid]));
					}
					case 7:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/goto %d", LastClickedPlayer[playerid]));
					}
					case 8:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/get %d", LastClickedPlayer[playerid]));
					}
					case 9:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/slap %d", LastClickedPlayer[playerid]));
					}
					case 10:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/eject %d", LastClickedPlayer[playerid]));
					}
					case 11:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/mute %d No Reason Specified", LastClickedPlayer[playerid]));
					}
					case 12:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/unmute %d", LastClickedPlayer[playerid]));
					}
					case 13:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/kick %d No Reason Specified", LastClickedPlayer[playerid]));
					}
					case 14:
					{
						CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/ban %d No Reason Specified", LastClickedPlayer[playerid]));
					}
				}
			}
			return 1;
		}
		case DIALOG_SERVER_PASS:
		{
			if (response)
			{
				if (isnull(inputtext))
					return 1;
				if (strlen(inputtext) > MAX_SERVER_PASS_LENGTH)
				{
					SendErrorMessage(playerid, "Server password is too long.");
					ShowPlayerDialog(playerid, DIALOG_SERVER_PASS, DIALOG_STYLE_INPUT, "" COL_PRIM "Server Password", "" COL_PRIM "Enter server password below:", "Ok", "Close");
					return 1;
				}
				if (!IsValidServerPassword(inputtext))
				{
					SendErrorMessage(playerid, "Password may only contain letters, numbers, _, -, ., !, @, #, and +.");
					ShowPlayerDialog(playerid, DIALOG_SERVER_PASS, DIALOG_STYLE_INPUT, "" COL_PRIM "Server Password", "" COL_PRIM "Enter server password below:", "Ok", "Close");
					return 1;
				}
				format(ServerPass, sizeof(ServerPass), "password %s", inputtext);
				SendRconCommand(ServerPass);

				ServerLocked = true;
				PermLocked	 = false;

				new iString[144];

				format(iString, sizeof(iString), COL_PRIM ""COL_SEC"%s %s "COL_SEC"%s " COL_PRIM "has locked the server. (Pass: "COL_SEC"%s"COL_PRIM")", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], inputtext);
				SendClientMessageToAll(-1, iString);
			}
			return 1;
		}
		case DIALOG_CURRENT_TOTAL:
		{
			if (isnull(inputtext))
				return 1;
			if (!IsNumeric(inputtext))
			{
				SendErrorMessage(playerid, "You can only use numeric input.");
				ShowPlayerDialog(
					playerid, DIALOG_CURRENT_TOTAL, DIALOG_STYLE_INPUT, "" COL_PRIM "Rounds Dialog", "" COL_PRIM "Enter current round or total rounds to be played:", "Current",
					"Total");
				return 1;
			}

			new Value = strval(inputtext);

			if (Value < 0 || Value > 100)
			{
				SendErrorMessage(playerid, "Current or total rounds can only be between 0 and 100.");
				ShowPlayerDialog(
					playerid, DIALOG_CURRENT_TOTAL, DIALOG_STYLE_INPUT, "" COL_PRIM "Rounds Dialog", "" COL_PRIM "Enter current round or total rounds to be played:", "Current",
					"Total");
				return 1;
			}

			new iString[144];

			if (response)
			{
				CurrentRound = Value;
				format(iString, sizeof(iString), COL_PRIM "%s "COL_SEC"%s " COL_PRIM "has changed the current round to "COL_SEC"%d", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], CurrentRound);
				SendClientMessageToAll(-1, iString);
			}
			else
			{
				TotalRounds = Value;
				format(iString, sizeof(iString), COL_PRIM "%s "COL_SEC"%s " COL_PRIM "has changed the total rounds to "COL_SEC"%d rounds.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], TotalRounds);
				SendClientMessageToAll(-1, iString);
			}
			UpdateRoundsPlayedTextDraw();
			return 1;
		}
		case DIALOG_TEAM_SCORE:
		{
			if (response)
			{
				switch (listitem)
				{
					case 0:
					{
						ShowPlayerDialog(
							playerid, DIALOG_ATT_NAME, DIALOG_STYLE_INPUT, "" COL_PRIM "Attacker Team Name", "" COL_PRIM "Enter {FFFFFF}Attacker " COL_PRIM "Team Name Below:", "Next",
							"Close");
					}
					case 1:
					{
						ShowPlayerDialog(playerid, DIALOG_ATT_SCORE, DIALOG_STYLE_INPUT, "" COL_PRIM "Attacker Team Score", va_return(COL_PRIM "Enter {FFFFFF}%s " COL_PRIM "Team Score Below:", TeamName[ATTACKER]), "Next", "Close");
					}
					case 2:
					{
						TeamScore[ATTACKER] = 0;
						TeamScore[DEFENDER] = 0;
						CurrentRound		= 0;

						UpdateTeamScoreTextDraw();
						UpdateRoundsPlayedTextDraw();
						UpdateTeamNameTextDraw();

						UpdateTeamNamesTextdraw();

						ClearPlayerVariables();
						TeamTacPauseUsed[ATTACKER] = false;
						TeamTacPauseUsed[DEFENDER] = false;

						foreach (new i : Player)
						{
							Player[i][TotalKills]		 = 0;
							Player[i][TotalDeaths]		 = 0;
							Player[i][TotalDamage]		 = 0.0;
							Player[i][RoundPlayed]		 = 0;
							Player[i][TotalBulletsFired] = 0;
							Player[i][TotalshotsHit]	 = 0;
							UpdateRoundKillDmgText(i);
						}

						SendClientMessageToAll(-1, va_return(""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has reset the scores.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]));
					}
				}
			}
			return 1;
		}
		case DIALOG_WAR_RESET:
		{
			if (response)
			{
				TeamScore[ATTACKER] = 0;
				TeamScore[DEFENDER] = 0;
				CurrentRound		= 0;

				new DBResult: res	= DB_ExecuteQuery(sqliteconnection, "SELECT * FROM Configs WHERE Option = 'Total Rounds'");

				new iString[144];

				DB_GetFieldStringByName(res, "Value", iString, sizeof(iString));
				TotalRounds = strval(iString);
				DB_FreeResultSet(res);

				TeamName[ATTACKER]	   = "Alpha";
				TeamName[ATTACKER_SUB] = "Alpha Sub";
				TeamName[DEFENDER]	   = "Beta";
				TeamName[DEFENDER_SUB] = "Beta Sub";

				UpdateTeamScoreTextDraw();
				UpdateRoundsPlayedTextDraw();
				UpdateTeamNameTextDraw();


				WarMode = false;
				TeamTacPauseUsed[ATTACKER] = false;
				TeamTacPauseUsed[DEFENDER] = false;

				foreach (new i : Player)
				{
					Player[i][TotalKills]		 = 0;
					Player[i][TotalDeaths]		 = 0;
					Player[i][TotalDamage]		 = 0.0;
					Player[i][RoundPlayed]		 = 0;
					Player[i][TotalBulletsFired] = 0;
					Player[i][TotalshotsHit]	 = 0;
					UpdateRoundKillDmgText(i);
				}

				ClearPlayerVariables();

				HideMatchScoreBoard();

				FixGamemodeText();
				format(iString, sizeof(iString), COL_PRIM "%s "COL_SEC"%s " COL_PRIM "has disabled the Match-Mode.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
				SendClientMessageToAll(-1, iString);
			}
			return 1;
		}
		case DIALOG_ATT_NAME:
		{
			if (response)
			{
				if (isnull(inputtext))
				{
					ShowPlayerDialog(
						playerid, DIALOG_DEF_NAME, DIALOG_STYLE_INPUT, "" COL_PRIM "Defender Team Name", "" COL_PRIM "Enter {FFFFFF}Defender " COL_PRIM "Team Name Below:", "Ok",
						"Close");
					return 1;
				}
				if (strlen(inputtext) > 6)
				{
					SendErrorMessage(playerid, "Team name is too long.");
					ShowPlayerDialog(
						playerid, DIALOG_ATT_NAME, DIALOG_STYLE_INPUT, "" COL_PRIM "Attacker Team Name", "" COL_PRIM "Enter {FFFFFF}Attacker " COL_PRIM "Team Name Below:", "Next",
						"Close");
					return 1;
				}

				if (strfind(inputtext, "~") != -1)
				{
					return SendErrorMessage(playerid, "~ not allowed.");
				}

				format(TeamName[ATTACKER], 24, inputtext);
				format(TeamName[ATTACKER_SUB], 24, "%s Sub", TeamName[ATTACKER]);

				UpdateTeamScoreTextDraw();
				UpdateRoundsPlayedTextDraw();
				UpdateTeamNameTextDraw();

				UpdateTeamNamesTextdraw();

				new iString[144];
				format(iString, sizeof(iString), COL_PRIM "%s "COL_SEC"%s " COL_PRIM "has set attacker team name to "COL_SEC"%s", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], TeamName[ATTACKER]);
				SendClientMessageToAll(-1, iString);

				ShowPlayerDialog(
					playerid, DIALOG_DEF_NAME, DIALOG_STYLE_INPUT, "" COL_PRIM "Defender Team Name", "" COL_PRIM "Enter {FFFFFF}Defender " COL_PRIM "Team Name Below:", "Ok", "Close");
			}
			return 1;
		}
		case DIALOG_DEF_NAME:
		{
			if (response)
			{
				if (isnull(inputtext))
					return 1;
				if (strlen(inputtext) > 6)
				{
					SendErrorMessage(playerid, "Team name is too long.");
					ShowPlayerDialog(
						playerid, DIALOG_DEF_NAME, DIALOG_STYLE_INPUT, "" COL_PRIM "Defender Team Name", "" COL_PRIM "Enter {FFFFFF}Defender " COL_PRIM "Team Name Below:", "Ok",
						"Close");
					return 1;
				}

				if (strfind(inputtext, "~") != -1)
				{
					return SendErrorMessage(playerid, "~ not allowed.");
				}

				format(TeamName[DEFENDER], 24, inputtext);
				format(TeamName[DEFENDER_SUB], 24, "%s Sub", TeamName[DEFENDER]);

				UpdateTeamScoreTextDraw();
				UpdateRoundsPlayedTextDraw();
				UpdateTeamNameTextDraw();

				UpdateTeamNamesTextdraw();

				new iString[144];
				format(iString, sizeof(iString), COL_PRIM "%s "COL_SEC"%s " COL_PRIM "has set defender team name to "COL_SEC"%s", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], TeamName[DEFENDER]);
				SendClientMessageToAll(-1, iString);

				WarMode = true;
				TeamTacPauseUsed[ATTACKER] = false;
				TeamTacPauseUsed[DEFENDER] = false;
				ClearPlayerVariables();
				foreach (new i : Player)
				{
					Player[i][TotalKills]		 = 0;
					Player[i][TotalDeaths]		 = 0;
					Player[i][TotalDamage]		 = 0.0;
					Player[i][RoundPlayed]		 = 0;
					Player[i][TotalBulletsFired] = 0;
					Player[i][TotalshotsHit]	 = 0;
					UpdateRoundKillDmgText(i);
				}


				ShowMatchScoreBoard();
			}
			return 1;
		}
		case DIALOG_ATT_SCORE:
		{
			if (response)
			{
				new iString[128];
				if (isnull(inputtext))
				{
					format(iString, sizeof(iString), "" COL_PRIM "Enter {FFFFFF}%s " COL_PRIM "Team Score Below:", TeamName[DEFENDER]);
					ShowPlayerDialog(playerid, DIALOG_DEF_SCORE, DIALOG_STYLE_INPUT, "" COL_PRIM "Defender Team Score", iString, "Ok", "Close");
					return 1;
				}
				if (!IsNumeric(inputtext))
				{
					SendErrorMessage(playerid, "Score can only be numerical.");
					format(iString, sizeof(iString), "" COL_PRIM "Enter {FF3333}%s " COL_PRIM "Team Score Below:", TeamName[ATTACKER]);
					ShowPlayerDialog(playerid, DIALOG_ATT_SCORE, DIALOG_STYLE_INPUT, "" COL_PRIM "Attacker Team Score", iString, "Next", "Close");
					return 1;
				}
				new Score = strval(inputtext);

				if (Score < 0 || Score > 100)
				{
					SendErrorMessage(playerid, "Score can only be between 0 and 100.");
					format(iString, sizeof(iString), "" COL_PRIM "Enter {FF3333}%s " COL_PRIM "Team Score Below:", TeamName[ATTACKER]);
					ShowPlayerDialog(playerid, DIALOG_ATT_SCORE, DIALOG_STYLE_INPUT, "" COL_PRIM "Attacker Team Score", iString, "Next", "Close");
					return 1;
				}

				if ((Score + TeamScore[DEFENDER]) >= TotalRounds)
				{
					SendErrorMessage(playerid, "Attacker plus defender score is bigger than or equal to total rounds.");
					format(iString, sizeof(iString), "" COL_PRIM "Enter {FFFFFF}%s " COL_PRIM "Team Score Below:", TeamName[ATTACKER]);
					ShowPlayerDialog(playerid, DIALOG_ATT_SCORE, DIALOG_STYLE_INPUT, "" COL_PRIM "Attacker Team Score", iString, "Next", "Close");
					return 1;
				}

				TeamScore[ATTACKER] = Score;
				CurrentRound		= TeamScore[ATTACKER] + TeamScore[DEFENDER];

				format(iString, sizeof(iString), COL_PRIM "%s "COL_SEC"%s " COL_PRIM "has set attacker team score to "COL_SEC"%d", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], TeamScore[ATTACKER]);
				SendClientMessageToAll(-1, iString);

				UpdateTeamScoreTextDraw();
				UpdateRoundsPlayedTextDraw();
				UpdateTeamNameTextDraw();

				format(iString, sizeof(iString), "" COL_PRIM "Enter {FFFFFF}%s " COL_PRIM "Team Score Below:", TeamName[DEFENDER]);
				ShowPlayerDialog(playerid, DIALOG_DEF_SCORE, DIALOG_STYLE_INPUT, "" COL_PRIM "Defender Team Score", iString, "Ok", "Close");
			}
			return 1;
		}
		case DIALOG_DEF_SCORE:
		{
			if (response)
			{
				if (isnull(inputtext))
					return 1;

				new iString[128];
				if (!IsNumeric(inputtext))
				{
					SendErrorMessage(playerid, "Score can only be numerical.");
					format(iString, sizeof(iString), "" COL_PRIM "Enter {FFFFFF}%s " COL_PRIM "Team Score Below:", TeamName[DEFENDER]);
					ShowPlayerDialog(playerid, DIALOG_DEF_SCORE, DIALOG_STYLE_INPUT, "" COL_PRIM "Defender Team Score", iString, "Ok", "Close");
					return 1;
				}

				new Score = strval(inputtext);

				if (Score < 0 || Score > 100)
				{
					SendErrorMessage(playerid, "Score can only be between 0 and 100.");
					format(iString, sizeof(iString), "" COL_PRIM "Enter {FFFFFF}%s " COL_PRIM "Team Score Below:", TeamName[DEFENDER]);
					ShowPlayerDialog(playerid, DIALOG_DEF_SCORE, DIALOG_STYLE_INPUT, "" COL_PRIM "Defender Team Score", iString, "Ok", "Close");
					return 1;
				}

				if ((TeamScore[ATTACKER] + Score) >= TotalRounds)
				{
					SendErrorMessage(playerid, "Attacker plus defender score is bigger than or equal to total rounds.");
					format(iString, sizeof(iString), "" COL_PRIM "Enter {FFFFFF}%s " COL_PRIM "Team Score Below:", TeamName[DEFENDER]);
					ShowPlayerDialog(playerid, DIALOG_DEF_SCORE, DIALOG_STYLE_INPUT, "" COL_PRIM "Defender Team Score", iString, "Ok", "Close");
					return 1;
				}
				TeamScore[DEFENDER] = Score;
				CurrentRound		= TeamScore[ATTACKER] + TeamScore[DEFENDER];

				format(iString, sizeof(iString), COL_PRIM "%s "COL_SEC"%s " COL_PRIM "has set defender team score to "COL_SEC"%d", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], TeamScore[DEFENDER]);
				SendClientMessageToAll(-1, iString);

				UpdateTeamScoreTextDraw();
				UpdateRoundsPlayedTextDraw();
				UpdateTeamNameTextDraw();
			}
			return 1;
		}
		case DIALOG_CONFIG_SET_GA:
		{
			if (!response)
				return ShowConfigDialog(playerid);
			switch (listitem)
			{
				case 0:
				{
					ShowPlayerDialog(
						playerid, DIALOG_CONFIG_SET_GA_ALPHA, DIALOG_STYLE_INPUT, "" COL_PRIM "ALPHA PASSWORD", "" COL_PRIM "Set the password or leave empty to clear:", "OK",
						"Cancel");
				}
				case 1:
				{
					ShowPlayerDialog(
						playerid, DIALOG_CONFIG_SET_GA_BETA, DIALOG_STYLE_INPUT, "" COL_PRIM "BETA PASSWORD", "" COL_PRIM "Set the password or leave empty to clear:", "OK", "Cancel");
				}
				case 2:
				{
					ShowPlayerDialog(
						playerid, DIALOG_CONFIG_SET_GA_REF, DIALOG_STYLE_INPUT, "" COL_PRIM "REFEREE PASSWORD", "" COL_PRIM "Set the password or leave empty to clear:", "OK",
						"Cancel");
				}
				case 3:
				{
					ShowPlayerDialog(
						playerid, DIALOG_CONFIG_SET_GA_ASUB, DIALOG_STYLE_INPUT, "" COL_PRIM "ALPHA SUB PASSWORD", "" COL_PRIM "Set the password or leave empty to clear:", "OK",
						"Cancel");
				}
				case 4:
				{
					ShowPlayerDialog(
						playerid, DIALOG_CONFIG_SET_GA_BSUB, DIALOG_STYLE_INPUT, "" COL_PRIM "BETA SUB PASSWORD", "" COL_PRIM "Set the password or leave empty to clear:", "OK",
						"Cancel");
				}
			}
			return 1;
		}
		case DIALOG_CONFIG_SET_GA_ALPHA:
		{
			if (!response)
				return ShowConfigDialog(playerid);
			if (strlen(inputtext) > MAX_GROUP_ACCESS_PASSWORD_LENGTH)
			{
				SendErrorMessage(playerid, "The password you entered is quite long. Try again with a shorter one!", MSGBOX_TYPE_BOTTOM);
				return ShowConfigDialog(playerid);
			}
			format(GroupAccessPassword[0], MAX_GROUP_ACCESS_PASSWORD_LENGTH, "%s", inputtext);
			new str[128];
			format(str, sizeof(str), "%s " COL_PRIM "has changed the alpha group access", Player[playerid][Name]);
			SendClientMessageToAll(-1, str);
			return ShowConfigDialog(playerid);
		}
		case DIALOG_CONFIG_SET_GA_BETA:
		{
			if (!response)
				return ShowConfigDialog(playerid);
			if (strlen(inputtext) > MAX_GROUP_ACCESS_PASSWORD_LENGTH)
			{
				SendErrorMessage(playerid, "The password you entered is quite long. Try again with a shorter one!", MSGBOX_TYPE_BOTTOM);
				return ShowConfigDialog(playerid);
			}
			format(GroupAccessPassword[1], MAX_GROUP_ACCESS_PASSWORD_LENGTH, "%s", inputtext);
			new str[128];
			format(str, sizeof(str), "%s " COL_PRIM "has changed the beta group access", Player[playerid][Name]);
			SendClientMessageToAll(-1, str);
			return ShowConfigDialog(playerid);
		}
		case DIALOG_CONFIG_SET_GA_REF:
		{
			if (!response)
				return ShowConfigDialog(playerid);
			if (strlen(inputtext) > MAX_GROUP_ACCESS_PASSWORD_LENGTH)
			{
				SendErrorMessage(playerid, "The password you entered is quite long. Try again with a shorter one!", MSGBOX_TYPE_BOTTOM);
				return ShowConfigDialog(playerid);
			}
			format(GroupAccessPassword[2], MAX_GROUP_ACCESS_PASSWORD_LENGTH, "%s", inputtext);
			new str[128];
			format(str, sizeof(str), "%s " COL_PRIM "has changed the referee group access", Player[playerid][Name]);
			SendClientMessageToAll(-1, str);
			return ShowConfigDialog(playerid);
		}
		case DIALOG_CONFIG_SET_GA_ASUB:
		{
			if (!response)
				return ShowConfigDialog(playerid);
			if (strlen(inputtext) > MAX_GROUP_ACCESS_PASSWORD_LENGTH)
			{
				SendErrorMessage(playerid, "The password you entered is quite long. Try again with a shorter one!", MSGBOX_TYPE_BOTTOM);
				return ShowConfigDialog(playerid);
			}
			format(GroupAccessPassword[3], MAX_GROUP_ACCESS_PASSWORD_LENGTH, "%s", inputtext);
			new str[128];
			format(str, sizeof(str), "%s " COL_PRIM "has changed the alpha sub group access", Player[playerid][Name]);
			SendClientMessageToAll(-1, str);
			return ShowConfigDialog(playerid);
		}
		case DIALOG_CONFIG_SET_GA_BSUB:
		{
			if (!response)
				return ShowConfigDialog(playerid);
			if (strlen(inputtext) > MAX_GROUP_ACCESS_PASSWORD_LENGTH)
			{
				SendErrorMessage(playerid, "The password you entered is quite long. Try again with a shorter one!", MSGBOX_TYPE_BOTTOM);
				return ShowConfigDialog(playerid);
			}
			format(GroupAccessPassword[4], MAX_GROUP_ACCESS_PASSWORD_LENGTH, "%s", inputtext);
			new str[128];
			format(str, sizeof(str), "%s " COL_PRIM "has changed the beta sub group access", Player[playerid][Name]);
			SendClientMessageToAll(-1, str);
			return ShowConfigDialog(playerid);
		}
		case DIALOG_GROUPACCESS:
		{
			if (!response)
			{
				if (!Player[playerid][Spawned])
				{
					ShowPlayerClassSelection(playerid);
				}
				return 1;
			}
			new groupID = Player[playerid][RequestedClass];

			if (strcmp(inputtext, GroupAccessPassword[groupID]) != 0 || strlen(inputtext) == 0)
			{
				return ShowPlayerDialog(
					playerid, DIALOG_GROUPACCESS, DIALOG_STYLE_INPUT, "Authorization required", "Wrong password.\n\nPlease enter the group password:", "Submit", "Cancel");
			}
			if (Player[playerid][Spawned])
			{
				switch (Player[playerid][RequestedClass])
				{
					case 0:
					{
						SetPlayerColor(playerid, ATTACKER_NOT_PLAYING);
						Player[playerid][Team] = ATTACKER;
					}
					case 1:
					{
						SetPlayerColor(playerid, DEFENDER_NOT_PLAYING);
						Player[playerid][Team] = DEFENDER;
					}
					case 2:
					{
						SetPlayerColor(playerid, REFEREE_COLOR);
						Player[playerid][Team] = REFEREE;
					}
					case 3:
					{
						SetPlayerColor(playerid, ATTACKER_SUB_COLOR);
						Player[playerid][Team] = ATTACKER_SUB;
					}

					case 4:
					{
						SetPlayerColor(playerid, DEFENDER_SUB_COLOR);
						Player[playerid][Team] = DEFENDER_SUB;
					}
				}
				SwitchTeamFix(playerid);
			}
			else
			{
				RequestedGroupPass[playerid][groupID] = "";
				SpawnConnectedPlayer(playerid, groupID + 1);
			}
			return 1;
		}
		case DIALOG_CONFIG_SET_TEAM_SKIN:
		{
			if (response)
			{
				switch (listitem)
				{
					case 0: CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/teamskin 0"); // Attacker
					case 1: CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/teamskin 1"); // Defender
					case 2: CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/teamskin 2"); // Referee
					case 3: CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/teamskin 3"); // Attacker Sub
					case 4: CallLocalFunction("OnPlayerCommandText", "ds", playerid, "/teamskin 4"); // Defender Sub
				}
			}
			else
			{
				ShowConfigDialog(playerid);
			}
			return 1;
		}
		case DIALOG_CONFIG_SET_AAD:
		{
			if (response)
			{
				switch (listitem)
				{
					case 0:
					{ // set round health
						ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_ROUND_HEALTH, DIALOG_STYLE_INPUT, "" COL_PRIM "Round Health", "" COL_PRIM "Set round health:", "OK", "");
					}
					case 1:
					{ // set round armour
						ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_ROUND_ARMOUR, DIALOG_STYLE_INPUT, "" COL_PRIM "Round Armour", "" COL_PRIM "Set round armour:", "OK", "");
					}
					case 2:
					{ // Round time
						ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_ROUND_TIME, DIALOG_STYLE_INPUT, "" COL_PRIM "Round Time", "" COL_PRIM "Set round time:", "OK", "Cancel");
					}
					case 3:
					{ // CP time
						ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_CP_TIME, DIALOG_STYLE_INPUT, "" COL_PRIM "CP Time", "" COL_PRIM "Set CP time:", "OK", "Cancel");
					}
				}
			}
			else
			{
				ShowConfigDialog(playerid);
			}
			return 1;
		}
		case DIALOG_CONFIG_SET_ROUND_HEALTH:
		{
			new hp = strval(inputtext);
			if (hp <= 0 || hp > 100)
			{
				SendErrorMessage(playerid, "Health value can be between 0 and 100 maximum.");
				ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_ROUND_HEALTH, DIALOG_STYLE_INPUT, "" COL_PRIM "Round Health", "" COL_PRIM "Set round health:", "OK", "");
				return 1;
			}

			new str[128];
			format(str, sizeof(str), "UPDATE `Configs` SET `Value` = '%d,%d' WHERE `Option` = 'RoundHPAR'", hp, RoundAR);
			if (!DatabaseExecuteWrite(str, "save round health"))
				return SendErrorMessage(playerid, "Could not save the round health. No changes were applied.");

			RoundHP = hp;
			format(str, sizeof(str), COL_PRIM "%s "COL_SEC"%s " COL_PRIM "has changed the round health to "COL_SEC"%d HP", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], RoundHP);
			SendClientMessageToAll(-1, str);

			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_CONFIG_SET_ROUND_ARMOUR:
		{
			new hp = strval(inputtext);
			if (hp <= 0 || hp > 100)
			{
				SendErrorMessage(playerid, "Armour value can be between 0 and 100 maximum.");
				ShowPlayerDialog(playerid, DIALOG_CONFIG_SET_ROUND_ARMOUR, DIALOG_STYLE_INPUT, "" COL_PRIM "Round Armour", "" COL_PRIM "Set round armour:", "OK", "");
				return 1;
			}

			new str[128];
			format(str, sizeof(str), "UPDATE `Configs` SET `Value` = '%d,%d' WHERE `Option` = 'RoundHPAR'", RoundHP, hp);
			if (!DatabaseExecuteWrite(str, "save round armour"))
				return SendErrorMessage(playerid, "Could not save the round armour. No changes were applied.");

			RoundAR = hp;
			format(str, sizeof(str), COL_PRIM "%s "COL_SEC"%s " COL_PRIM "has changed the round armour to "COL_SEC"%d AP", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], RoundAR);
			SendClientMessageToAll(-1, str);

			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_CONFIG_SET_ROUND_TIME:
		{
			if (response)
			{
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/roundtime %s", inputtext));
				ShowConfigDialog(playerid);
			}
			else
			{
				ShowConfigDialog(playerid);
			}
			return 1;
		}
		case DIALOG_CONFIG_SET_CP_TIME:
		{
			if (response)
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/cptime %s", inputtext));
			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_CONFIG_SET_MAX_PING:
		{
			if (response)
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/maxping %s", inputtext));
			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_CONFIG_SET_MAX_PACKET:
		{
			if (response)
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/maxpacket %s", inputtext));
			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_CONFIG_SET_MIN_FPS:
		{
			if (response)
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/minfps %s", inputtext));
			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_CONFIG_SET_MAX_FPS:
		{
			if (response)
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/maxfps %s", inputtext));
			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_CONFIG_ADDMETIME:
		{
			if (response)
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/addmetime %s", inputtext));
			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_CONFIG_GUNMENUTIME:
		{
			if (response)
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/gunmenutime %s", inputtext));
			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_CONFIG_SET_ARMOUR_PING:
		{
			if (response)
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/setarmourping %s", inputtext));
			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_CONFIG_DEAGLE_NERF:
		{
			if (response)
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/deaglenerf on %s", inputtext));
			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_CONFIG_SNIPER_NERF:
		{
			if (response)
				CallLocalFunction("OnPlayerCommandText", "ds", playerid, va_return("/snipernerf on %s", inputtext));
			ShowConfigDialog(playerid);
			return 1;
		}
		case DIALOG_SWITCH_TEAM:
		{
			if (response)
			{
				oldTeamSwitch = Player[playerid][Team];
				new groupID = listitem;
				if (strlen(GroupAccessPassword[groupID]) > 0
					&& (strcmp(RequestedGroupPass[playerid][groupID], GroupAccessPassword[groupID]) != 0 || isnull(RequestedGroupPass[playerid][groupID])))
				{
					Player[playerid][RequestedClass] = listitem;
					ShowPlayerDialog(playerid, DIALOG_GROUPACCESS, DIALOG_STYLE_INPUT, "Authorization required", "Please enter the group password:", "Submit", "Cancel");
					return 1;
				}
				switch (listitem)
				{
					case 0:
					{
						SetPlayerColor(playerid, ATTACKER_NOT_PLAYING);
						Player[playerid][Team] = ATTACKER;
					}
					case 1:
					{
						SetPlayerColor(playerid, DEFENDER_NOT_PLAYING);
						Player[playerid][Team] = DEFENDER;
					}
					case 2:
					{
						SetPlayerColor(playerid, REFEREE_COLOR);
						Player[playerid][Team] = REFEREE;
					}
					case 3:
					{
						SetPlayerColor(playerid, ATTACKER_SUB_COLOR);
						Player[playerid][Team] = ATTACKER_SUB;
					}

					case 4:
					{
						SetPlayerColor(playerid, DEFENDER_SUB_COLOR);
						Player[playerid][Team] = DEFENDER_SUB;
					}
				}
				SwitchTeamFix(playerid);
			}
			return 1;
		}
		case DIALOG_TEAM_SELECTION:
		{
			if (response)
			{
				if (listitem > 0) // Not auto-assign
				{
					new groupID = listitem - 1;
					if (strlen(GroupAccessPassword[groupID]) > 0
						&& (strcmp(RequestedGroupPass[playerid][groupID], GroupAccessPassword[groupID]) != 0 || isnull(RequestedGroupPass[playerid][groupID])))
					{
						Player[playerid][RequestedClass] = listitem;
						ShowPlayerDialog(playerid, DIALOG_GROUPACCESS, DIALOG_STYLE_INPUT, "Authorization required", "Please enter the group password:", "Submit", "Cancel");
						return 1;
					}
				}
				SpawnConnectedPlayer(playerid, listitem);
			}
			else
			{
				ShowPlayerClassSelection(playerid);
			}
			return 1;
		}
		case DIALOG_ROUND_TD_STYLE:
		{
			if (!response)
				return 1;

			if (AllVsAllMode)
				return SendErrorMessage(playerid, "You cannot change textdraw style during All-vs-All mode.");

			new selectedStyle;
			switch (listitem)
			{
				case 0: selectedStyle = 0;
				case 1: selectedStyle = 2;
				case 2: selectedStyle = 3;
				case 3: selectedStyle = 4;
				default: return SendErrorMessage(playerid, "Invalid round textdraw style.");
			}

			if (selectedStyle < 0 || selectedStyle > 4)
				return SendErrorMessage(playerid, "Invalid round textdraw style.");

			if (!DatabaseExecuteWrite(
				va_return("UPDATE `Players` SET `RoundTDStyle`=%d WHERE `Name`='%q'", selectedStyle, Player[playerid][Name]),
				"save round textdraw style"))
				return SendErrorMessage(playerid, "Could not save your round textdraw style.");

			HideRoundStats(playerid);
			Player[playerid][RoundTDStyle] = selectedStyle;
			UpdateBaseIDTextDraw(playerid);

			if (selectedStyle != 1)
			{
				TextDrawHideForPlayer(playerid, AttHpLose);
				TextDrawHideForPlayer(playerid, DefHpLose);
				TextDrawHideForPlayer(playerid, TeamHpLose[0]);
				TextDrawHideForPlayer(playerid, TeamHpLose[1]);
			}

			switch (Player[playerid][RoundTDStyle])
			{
				case 0: SendClientMessage(playerid, -1, "\xBB " COL_PRIM "You have switched round textdraws style to: " COL_SEC "Modern");
				case 1: SendClientMessage(playerid, -1, "\xBB " COL_PRIM "You have switched round textdraws style to: " COL_SEC "Bulletproof");
				case 2: SendClientMessage(playerid, -1, "\xBB " COL_PRIM "You have switched round textdraws style to: " COL_SEC "Simple");
				case 3: SendClientMessage(playerid, -1, "\xBB " COL_PRIM "You have switched round textdraws style to: " COL_SEC "Team Status Icons");
				case 4: SendClientMessage(playerid, -1, "\xBB " COL_PRIM "You have switched round textdraws style to: " COL_SEC "Team HP Bars");
			}

			if (Current != -1)
				ShowRoundStats(playerid);
			return 1;
		}
		case DIALOG_CONFIG_PAUSEMODE:
		{
			if (response)
			{
				new PauseMode: mode;
				switch (listitem)
				{
					case 0: mode = PAUSE_MODE_MESSAGE;
					case 1: mode = PAUSE_MODE_PAUSE_ROUND;
					case 2: mode = PAUSE_MODE_NONE;
					default: return SendErrorMessage(playerid, "Invalid pause detection mode.");
				}
				if (!SetPlayerPauseMode(playerid, mode))
					return 1;
				ShowConfigDialog(playerid);
			}
			else
			{
				ShowConfigDialog(playerid);
			}
			return 1;
		}
		case DIALOG_CONFIG_AUTOPAUSE:
		{
			if (response)
			{
				SetAutoPauseMode(playerid, listitem);
				ShowConfigDialog(playerid);
			}
			else
			{
				ShowConfigDialog(playerid);
			}
			return 1;
		}
		case DIALOG_AUTOPAUSE:
		{
			if (response)
			{
				SetAutoPauseMode(playerid, listitem);
			}
			return 1;
		}
		case DIALOG_PAUSEMODE:
		{
			if (response)
			{
				new PauseMode: mode;
				switch (listitem)
				{
					case 0: mode = PAUSE_MODE_MESSAGE;
					case 1: mode = PAUSE_MODE_PAUSE_ROUND;
					case 2: mode = PAUSE_MODE_NONE;
					default: return SendErrorMessage(playerid, "Invalid pause detection mode.");
				}
				SetPlayerPauseMode(playerid, mode);
			}
			return 1;
		}
		case DIALOG_CONFIG_NETMODE:
		{
			if (response)
			{
				if (!SetPlayerNetMode(playerid, listitem))
					return 1;
				ShowConfigDialog(playerid);
			}
			else
			{
				ShowConfigDialog(playerid);
			}
			return 1;
		}
		case DIALOG_NETMODE:
		{
			if (response)
				SetPlayerNetMode(playerid, listitem);
			return 1;
		}
		case DIALOG_SETLABEL:
		{
			if (!response) 
				return 1;

			new message[128];
			new optionType[32];
			new index, option;
	
			if (listitem >= 0 && listitem <= 2) 
			{
				index = listitem;
				option = TEXTLABEL_PL;
				strcat(optionType, "Packet Loss");
			}
			else if (listitem >= 3 && listitem <= 5) 
			{
				index = listitem - 3;
				option = TEXTLABEL_Ping;
				strcat(optionType, "Ping");
			}
			else if (listitem >= 6 && listitem <= 8) 
			{
				index = listitem - 6;
				option = TEXTLABEL_FPS;
				strcat(optionType, "FPS");
			}
			else
			{
				return 1;
			}

			GlobalTextLabelOption[option] = index;
			if (sqliteconnection)
			{
				new optName[24];
				switch (option)
				{
					case TEXTLABEL_PL: format(optName, sizeof(optName), "SkinLabelsPL");
					case TEXTLABEL_Ping: format(optName, sizeof(optName), "SkinLabelsPing");
					case TEXTLABEL_FPS: format(optName, sizeof(optName), "SkinLabelsFPS");
				}
				DatabaseExecuteWrite(
					va_return("INSERT OR REPLACE INTO Configs (Option, Value) VALUES ('%q', %d)", optName, index),
					"save skin labels setting");
			}

			switch (index)
			{
				case 0: format(message, sizeof(message), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed the skin label " COL_SEC "%s " COL_PRIM "option to " COL_SEC "'Text And Value'.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], optionType);
				case 1: format(message, sizeof(message), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed the skin label " COL_SEC "%s " COL_PRIM "option to " COL_SEC "'Value Only'.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], optionType);
				case 2: format(message, sizeof(message), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has disabled the skin label " COL_SEC "%s " COL_PRIM "option.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], optionType);
			}

			SendClientMessageToAll(-1, message);
			return 1;
		}
		case DIALOG_LOAD_BASES:
		{
			if (!response)
				return 1;
			DoLoadBases(playerid, listitem);
			return 1;
		}
		case DIALOG_LOAD_ARENAS:
		{
			if (!response)
				return 1;
			DoLoadArenas(playerid, listitem);
			return 1;
		}
		case DIALOG_TOP_MAIN:
		{
			if (!response)
				return 1;
			return ShowTopCategoryDialog(playerid, listitem);
		}
		case DIALOG_TOP_VIEW:
		{
			if (response)
				return ShowTopMenuDialog(playerid);
			return 1;
		}
		case DIALOG_MATCH_HISTORY:
		{
			if (!response)
				return 1;
			return ShowMatchHistoryEntryDialog(playerid, listitem);
		}
		case DIALOG_MATCH_HISTORY_VIEW:
		{
			if (response)
				return ShowMatchHistoryDialog(playerid);
			return 1;
		}
	}
	return 1;
}
