#if defined _attdef_OnPlayerPause
	#endinput
#endif
#define _attdef_OnPlayerPause

forward OnPlayerPause();
public OnPlayerPause()
{
	foreach (new i : Player)
	{
		if (CanPlay(i))
		{
			if (IsPlayerPaused(i))
			{
				if (!Player[i][gWasPaused]) // If the player was not paused before
				{
					Player[i][gPauseWarnings]++;
					if (Player[i][Playing] && Player[i][Team] != REFEREE && (BaseStarted || ArenaStarted || Player[i][gPauseCheck]))
					{
						switch (gPauseMode)
						{
							case PAUSE_MODE_MESSAGE:
							{
								printf("[Round] ID: %d is paused.", i);
								new message[64];
								format(message, sizeof(message), "%s %s is paused.", TextColor[Player[i][Team]], Player[i][Name]);
								SendClientMessageToAllTeams(message);
							}
							case PAUSE_MODE_PAUSE_ROUND:
							{
								if (!RoundPaused)
								{
									new message[120];
									format(message, sizeof(message), COL_PRIM " Round has been paused because " COL_SEC "%s " COL_PRIM "is paused.", Player[i][Name]);
									SendClientMessageToAllTeams(message);
									PauseRound();
								}
							}
							case PAUSE_MODE_NONE:
							{

							}
						}
					}
					if (Player[i][Playing] && Player[i][WasInCP])
					{
						new checkpointPauseMessage[128];
						format(
							checkpointPauseMessage, sizeof(checkpointPauseMessage),
							"%s %s is paused while capturing the checkpoint.",
							TextColor[Player[i][Team]], Player[i][Name]);
						foreach (new recipientid : PlayersInRound)
						{
							if (IsPlayerConnected(recipientid) && Player[recipientid][Playing])
								SendClientMessage(recipientid, -1, checkpointPauseMessage);
						}

						Player[i][gPauseRemovedFromCP] = true;
						CurrentCPTime = ConfigCPTime + 1;
						CallLocalFunction("OnPlayerLeaveCheckpoint", "i", i);
					}
					Player[i][gWasPaused] = 1;
					Player[i][gPauseStart] = GetTickCount();
				}

				new pauseTime = (GetTickCount() - Player[i][gPauseStart]) / 1000;
				new minutes = pauseTime / 60;
				new seconds = pauseTime % 60;
				new pauseLabelText[32];
				format(pauseLabelText, sizeof(pauseLabelText), "ESC: %d:%02d", minutes, seconds);
				if (IsValid3DTextLabel(Player[i][PauseLabel]))
				{
					Update3DTextLabelText(Player[i][PauseLabel], 0xFFFFFFFF, pauseLabelText);
					Player[i][gPauseLabelVisible] = true;
				}
			}
			else
			{
				if (Player[i][gWasPaused])
				{
					if (Player[i][Playing] && Player[i][Team] != REFEREE && (BaseStarted || ArenaStarted || Player[i][gPauseCheck]) && gPauseMode == PAUSE_MODE_MESSAGE)
					{
						printf("[Round] ID: %d is unpaused.", i);
						new message[64];
						format(message, sizeof(message), "%s %s is unpaused.", TextColor[Player[i][Team]], Player[i][Name]);
						SendClientMessageToAllTeams(message);
					}
					Player[i][gPauseWarnings] = 0;
					Player[i][gWasPaused] = 0;
				}
				if (Player[i][gPauseLabelVisible])
				{
					if (IsValid3DTextLabel(Player[i][PauseLabel]))
						Update3DTextLabelText(Player[i][PauseLabel], 0xFFFFFFFF, " ");
					Player[i][gPauseLabelVisible] = false;
				}
				if (Player[i][gPauseRemovedFromCP])
				{
					Player[i][gPauseRemovedFromCP] = false;
					if (Player[i][Playing] && !Player[i][WasInCP] && IsPlayerInCheckpoint(i))
						CallLocalFunction("OnPlayerEnterCheckpoint", "i", i);
				}
			}
		}
	}
	return 1;
}

stock SendClientMessageToAllTeams(const message[])
{
	foreach (new j : Player)
	{
		if (CanPlay(j) && (
			Player[j][Team] == ATTACKER ||
			Player[j][Team] == ATTACKER_SUB ||
			Player[j][Team] == DEFENDER ||
			Player[j][Team] == DEFENDER_SUB ||
			Player[j][Team] == REFEREE))
		{
			SendClientMessage(j, -1, message);
		}
	}
}

stock IsPlayerPaused(playerid)
{
	if (!IsPlayerConnected(playerid) || !WC_IsPlayerSpawned(playerid))
		return false;
	return WC_IsPlayerPaused(playerid);
}
