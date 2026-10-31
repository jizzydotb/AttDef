#if defined _attdef_OnPUIUpdate
	#endinput
#endif
#define _attdef_OnPUIUpdate

// OnPlayerUIUpdate - interval: 1000 ms
ptask OnPlayerUIUpdate[1000](playerid)
{
	// AFK Variable Update
	Player[playerid][PauseCount]++;

	UpdatePlayerHealthMoney(playerid);

	// This basically hides damage bubbles that should be hidden
	//UpdateDamageBubbles(playerid);

	// Show target info
	ShowTargetInfo(playerid, GetPlayerTargetPlayer(playerid));

	UpdatePlayerFPS(playerid);
	new pFPS = GetPlayerFPS(playerid);
	Player[playerid][FPS] = pFPS;

	// Update net info textdraws
	if (PlayerInterface[playerid][INTERFACE_NET] && Player[playerid][Team] != NON)
	{
		// Restore both elements if spawning or spectating hid them.
		PlayerTextDrawShow(playerid, FPSPingPacketBox[playerid]);
		PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);
		new Float: pPL = NetStats_PacketLossPercent(playerid);
		new pPing	   = GetPlayerPing(playerid);
		va_PlayerTextDrawSetString(
			playerid, FPSPingPacket[playerid],
			"~w~FPS %s%d ~w~Ping %s%d ~w~PL %s%.2f%%",
			TDC[Player[playerid][Team]], pFPS,
			TDC[Player[playerid][Team]], pPing,
			TDC[Player[playerid][Team]], pPL);
		new labelStr[128];
		labelStr[0] = EOS;

		if (GlobalTextLabelOption[TEXTLABEL_PL] == OPTION_TEXT_AND_VALUE)
		{
			format(labelStr, sizeof(labelStr), "%sPacket: {FFFFFF}%.1f%%", TextColor[Player[playerid][Team]], pPL);
		}
		else if (GlobalTextLabelOption[TEXTLABEL_PL] == OPTION_VALUE_ONLY)
		{
			new plColor[10];
			if (pPL < 1.0)
				plColor = "{66FF66}";
			else if (pPL < 3.0)
				plColor = "{FFA500}";
			else
				plColor = "{FF6666}";
			format(labelStr, sizeof(labelStr), "%s%.1f%%", plColor, pPL);
		}

		if (GlobalTextLabelOption[TEXTLABEL_Ping] == OPTION_TEXT_AND_VALUE)
		{
			if (labelStr[0] != EOS)
				strcat(labelStr, "\n");
			new pingLine[32];
			format(pingLine, sizeof(pingLine), "%sPing: {FFFFFF}%d", TextColor[Player[playerid][Team]], pPing);
			strcat(labelStr, pingLine);
		}
		else if (GlobalTextLabelOption[TEXTLABEL_Ping] == OPTION_VALUE_ONLY)
		{
			if (labelStr[0] != EOS)
				strcat(labelStr, "\n");
			new pingColor[10];
			if (pPing < 100)
				pingColor = "{66FF66}";
			else if (pPing < 180)
				pingColor = "{FFA500}";
			else
				pingColor = "{FF6666}";
			new pingLine[16];
			format(pingLine, sizeof(pingLine), "%s%d", pingColor, pPing);
			strcat(labelStr, pingLine);
		}

		if (GlobalTextLabelOption[TEXTLABEL_FPS] == OPTION_TEXT_AND_VALUE)
		{
			if (labelStr[0] != EOS)
				strcat(labelStr, "\n");
			new fpsLine[32];
			format(fpsLine, sizeof(fpsLine), "%sFPS: {FFFFFF}%d", TextColor[Player[playerid][Team]], pFPS);
			strcat(labelStr, fpsLine);
		}
		else if (GlobalTextLabelOption[TEXTLABEL_FPS] == OPTION_VALUE_ONLY)
		{
			if (labelStr[0] != EOS)
				strcat(labelStr, "\n");
			new fpsColor[10];
			if (pFPS >= 80)
				fpsColor = "{66FF66}";
			else if (pFPS >= 50)
				fpsColor = "{FFA500}";
			else
				fpsColor = "{FF6666}";
			new fpsLine[16];
			format(fpsLine, sizeof(fpsLine), "%s%d", fpsColor, pFPS);
			strcat(labelStr, fpsLine);
		}

		if (labelStr[0] == EOS)
			format(labelStr, sizeof(labelStr), " ");

		Update3DTextLabelText(Player[playerid][InfoLabel], -1, labelStr);
	}
	else
	{
		PlayerTextDrawHide(playerid, FPSPingPacketBox[playerid]);
		PlayerTextDrawHide(playerid, FPSPingPacket[playerid]);
	}
}
