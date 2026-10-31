#if defined _attdef_OnPText
	#endinput
#endif
#define _attdef_OnPText

static CopySafeChatText(const source[], destination[], size)
{
	new index;
	while (index < size - 1 && source[index] != EOS)
	{
		switch (source[index])
		{
			case '{': destination[index] = '(';
			case '}': destination[index] = ')';
			default: destination[index] = source[index];
		}
		index++;
	}
	destination[index] = EOS;
}

public OnPlayerText(playerid, text[])
{
	new safeText[144];
	CopySafeChatText(text, safeText, sizeof(safeText));

	new currentTick = GetTickCount();
	if (Player[playerid][Mute])
	{
		SendErrorMessage(playerid, "You are muted.");
		return 0;
	}
	if (ChatDisabled && Player[playerid][Level] == 0)
	{
		SendErrorMessage(playerid, "Main chat is currently disabled.");
		return 0;
	}

	// Anti spam
	if (AntiSpam && currentTick - Player[playerid][lastChat] < 500)
	{
		SendErrorMessage(playerid, "Please wait.");
		return 0;
	}
	Player[playerid][lastChat] = currentTick;

	// Team Chat
	if (safeText[0] == '!')
	{
		new ChatColor;
		switch (Player[playerid][Team])
		{
			case REFEREE: ChatColor = 0xFFFF90FF;
			case DEFENDER: ChatColor = 0x3D82FFFF;
			case ATTACKER: ChatColor = 0x2ED955FF;
			case ATTACKER_SUB: ChatColor = 0xA4F2B4FF;
			case DEFENDER_SUB: ChatColor = 0x8CE0FCFF;
			default:
			{
				SendErrorMessage(playerid, "You must be part of a team.");
				return 0;
			}
		}
		new ChatString[144];
		format(ChatString, sizeof(ChatString), "\xBB (Team %s Chat) %s: [%d] %s", TeamName[Player[playerid][Team]], Player[playerid][Name], playerid, safeText[1]);

		foreach (new i : Player)
		{
			if (Player[i][Team] != NON)
			{
				if ((Player[playerid][Team] == ATTACKER || Player[playerid][Team] == ATTACKER_SUB) && (Player[i][Team] == ATTACKER || Player[i][Team] == ATTACKER_SUB))
				{
					SendClientMessage(i, ChatColor, ChatString);
					PlayerPlaySound(i, 1137, 0.0, 0.0, 0.0);
				}
				else if ((Player[playerid][Team] == DEFENDER || Player[playerid][Team] == DEFENDER_SUB) && (Player[i][Team] == DEFENDER || Player[i][Team] == DEFENDER_SUB))
				{
					SendClientMessage(i, ChatColor, ChatString);
					PlayerPlaySound(i, 1137, 0.0, 0.0, 0.0);
				}
				else if (Player[playerid][Team] == REFEREE && Player[i][Team] == REFEREE && !Player[i][InDuel])
				{
					SendClientMessage(i, ChatColor, ChatString);
					PlayerPlaySound(i, 1137, 0.0, 0.0, 0.0);
				}
			}
		}
		return 0;
	}

	// Admin chat
	if (safeText[0] == '@' && (Player[playerid][Level] > 0 || IsPlayerAdmin(playerid)))
	{
		new ChatString[144];
		format(ChatString, sizeof(ChatString), "\xBB (Admin Chat) %s: [%d] %s", Player[playerid][Name], playerid, safeText[1]);
		foreach (new i : Player)
		{
			if (Player[i][Level] > 0 || IsPlayerAdmin(i))
			{
				SendClientMessage(i, 0x66CC66FF, ChatString);
				PlayerPlaySound(i, 1137, 0.0, 0.0, 0.0);
			}
		}
		return 0;
	}

	// Channel chat
	if (safeText[0] == '#' && Player[playerid][ChatChannel] != -1)
	{
		new ChatString[144];
		format(ChatString, sizeof(ChatString), "\xBB (Channel Chat) %s: {FFFFFF}[%d]{FFCC99} %s", Player[playerid][Name], Player[playerid][ChatChannel], safeText[1]);

		foreach (new i : Player)
		{
			if (Player[i][ChatChannel] == Player[playerid][ChatChannel])
			{
				SendClientMessage(i, 0xFFCC99FF, ChatString);
				PlayerPlaySound(i, 1137, 0.0, 0.0, 0.0);
			}
		}
		return 0;
	}

	// Main chat
	new pColor[12];
	if (Player[playerid][Team] >= 0 && Player[playerid][Team] < MAX_TEAMS && strlen(TextColor[Player[playerid][Team]]) > 0)
		format(pColor, sizeof(pColor), "%s", TextColor[Player[playerid][Team]]);
	else
		format(pColor, sizeof(pColor), "{FFFFFF}");

	new ChatString[144];
	format(ChatString, sizeof(ChatString), "%s%s: {FFFFFF}[%d] %s", pColor, Player[playerid][Name], playerid, safeText);
	SendClientMessageToAll(-1, ChatString);
	CallLocalFunction("OnPlayerChatMessage", "ds", playerid, safeText);
	return 0;
}
