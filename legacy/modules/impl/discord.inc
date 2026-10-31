#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_discord
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_discord
	#endinput
#endif
#define _attdef_impl_discord

/*
	Discord Connector Implementation
*/

#define DISCORD_MAX_MSG (1900)

new DCC_Channel: g_dccChannelMatches = DCC_INVALID_CHANNEL;
new DCC_Channel: g_dccChannelRounds  = DCC_INVALID_CHANNEL;
new DCC_Channel: g_dccChannelTop     = DCC_INVALID_CHANNEL;
new DCC_Channel: g_dccChannelDuels   = DCC_INVALID_CHANNEL;
new DCC_Channel: g_dccChannelLogs    = DCC_INVALID_CHANNEL;
new DCC_Channel: g_dccChannelChat    = DCC_INVALID_CHANNEL;
new DCC_Channel: g_dccChannelCommands = DCC_INVALID_CHANNEL;
new DCC_Channel: g_dccChannelAntiCheat = DCC_INVALID_CHANNEL;

static g_lastTopPostDay = -1;

stock Discord_StripColors(const input[], output[], maxlen = sizeof(output))
{
	new outIdx = 0, inIdx = 0, len = strlen(input);

	while (inIdx < len && outIdx < (maxlen - 1))
	{
		if (input[inIdx] == '{' && (inIdx + 7) < len && input[inIdx + 7] == '}')
		{
			inIdx += 8;
			continue;
		}

		if (input[inIdx] == '~' && (inIdx + 2) < len && input[inIdx + 2] == '~')
		{
			if (input[inIdx + 1] == 'n' || input[inIdx + 1] == 'N')
				output[outIdx++] = ' ';
			inIdx += 3;
			continue;
		}

		new ch = input[inIdx] & 0xFF;
		if (ch < 32 && ch != '\n' && ch != '\t')
		{
			inIdx++;
			continue;
		}

		if (ch == 0xBB || ch == 0xAB)
		{
			inIdx++;
			if (outIdx == 0 && input[inIdx] == ' ')
				inIdx++;
			continue;
		}

		if (ch == 0x95)
		{
			output[outIdx++] = '-';
			inIdx++;
			continue;
		}

		if (ch >= 128)
		{
			inIdx++;
			continue;
		}

		output[outIdx++] = input[inIdx++];
	}
	output[outIdx] = EOS;
	return outIdx;
}

stock Discord_Utf8ToAnsi(const input[], output[], maxlen = sizeof(output))
{
	new outIdx = 0, inIdx = 0, len = strlen(input);

	while (inIdx < len && outIdx < (maxlen - 1))
	{
		new b1 = input[inIdx] & 0xFF;
		if (b1 < 0x80)
		{
			if (b1 >= 32 || b1 == '\t')
				output[outIdx++] = b1;
			inIdx++;
			continue;
		}

		// 2-byte UTF-8 sequence (U+0080 .. U+07FF) -> Latin-1 / CP1252 (U+00A0 .. U+00FF)
		if ((b1 == 0xC2 || b1 == 0xC3) && (inIdx + 1) < len)
		{
			new b2 = input[inIdx + 1] & 0xFF;
			if ((b2 & 0xC0) == 0x80)
			{
				output[outIdx++] = ((b1 & 0x03) << 6) | (b2 & 0x3F);
				inIdx += 2;
				continue;
			}
		}

		// Skip multi-byte UTF-8 sequences outside Latin-1
		if ((b1 & 0xE0) == 0xC0)
			inIdx += 2;
		else if ((b1 & 0xF0) == 0xE0)
			inIdx += 3;
		else if ((b1 & 0xF8) == 0xF0)
			inIdx += 4;
		else
			inIdx++;
	}
	output[outIdx] = EOS;
	return outIdx;
}

static stock Discord_TrimString(const input[], output[], maxlen = sizeof(output))
{
	new start = 0, end = strlen(input) - 1;
	while (start <= end && (input[start] == ' ' || input[start] == '\t' || input[start] == '\r' || input[start] == '\n' || input[start] == '"'))
		start++;
	while (end >= start && (input[end] == ' ' || input[end] == '\t' || input[end] == '\r' || input[end] == '\n' || input[end] == '"'))
		end--;

	new outIdx = 0;
	for (new i = start; i <= end && outIdx < (maxlen - 1); i++)
		output[outIdx++] = input[i];
	output[outIdx] = EOS;
	return outIdx;
}

stock Discord_LoadConfig()
{
	if (!fexist("discord.cfg"))
		return 0;

	new File: f = fopen("discord.cfg", io_read);
	if (!f)
		return 0;

	new line[192], key[64], val[96];
	while (fread(f, line))
	{
		new trimmed[192];
		Discord_TrimString(line, trimmed);
		if (trimmed[0] == EOS || trimmed[0] == '#' || trimmed[0] == ';' || (trimmed[0] == '/' && trimmed[1] == '/'))
			continue;

		new sep = strfind(trimmed, "=");
		if (sep == -1)
		{
			sep = strfind(trimmed, " ");
			if (sep == -1)
				sep = strfind(trimmed, "\t");
		}
		if (sep <= 0)
			continue;

		new rawKey[64];
		strmid(rawKey, trimmed, 0, sep);
		Discord_TrimString(rawKey, key);
		Discord_TrimString(trimmed[sep + 1], val);

		if (key[0] == EOS || val[0] == EOS || !strcmp(val, "0") || !strcmp(val, "YOUR_", true, 5))
			continue;

		if (!strcmp(key, "channel_matches", true) || !strcmp(key, "DISCORD_CHANNEL_MATCHES", true))
			format(DISCORD_CHANNEL_MATCHES, sizeof(DISCORD_CHANNEL_MATCHES), "%s", val);
		else if (!strcmp(key, "channel_rounds", true) || !strcmp(key, "DISCORD_CHANNEL_ROUNDS", true))
			format(DISCORD_CHANNEL_ROUNDS, sizeof(DISCORD_CHANNEL_ROUNDS), "%s", val);
		else if (!strcmp(key, "channel_top", true) || !strcmp(key, "DISCORD_CHANNEL_TOP", true))
			format(DISCORD_CHANNEL_TOP, sizeof(DISCORD_CHANNEL_TOP), "%s", val);
		else if (!strcmp(key, "channel_duels", true) || !strcmp(key, "DISCORD_CHANNEL_DUELS", true))
			format(DISCORD_CHANNEL_DUELS, sizeof(DISCORD_CHANNEL_DUELS), "%s", val);
		else if (!strcmp(key, "channel_logs", true) || !strcmp(key, "DISCORD_CHANNEL_LOGS", true))
			format(DISCORD_CHANNEL_LOGS, sizeof(DISCORD_CHANNEL_LOGS), "%s", val);
		else if (!strcmp(key, "channel_chat", true) || !strcmp(key, "DISCORD_CHANNEL_CHAT", true))
			format(DISCORD_CHANNEL_CHAT, sizeof(DISCORD_CHANNEL_CHAT), "%s", val);
		else if (!strcmp(key, "channel_commands", true) || !strcmp(key, "DISCORD_CHANNEL_COMMANDS", true))
			format(DISCORD_CHANNEL_COMMANDS, sizeof(DISCORD_CHANNEL_COMMANDS), "%s", val);
		else if (!strcmp(key, "channel_anticheat", true) || !strcmp(key, "DISCORD_CHANNEL_ANTICHEAT", true))
			format(DISCORD_CHANNEL_ANTICHEAT, sizeof(DISCORD_CHANNEL_ANTICHEAT), "%s", val);
		else if (!strcmp(key, "role_admin", true) || !strcmp(key, "DISCORD_ROLE_ADMIN", true))
			format(DISCORD_ROLE_ADMIN, sizeof(DISCORD_ROLE_ADMIN), "%s", val);
		else if (!strcmp(key, "role_member", true) || !strcmp(key, "DISCORD_ROLE_MEMBER", true))
			format(DISCORD_ROLE_MEMBER, sizeof(DISCORD_ROLE_MEMBER), "%s", val);
		else if (!strcmp(key, "server_ip", true) || !strcmp(key, "DISCORD_SERVER_IP", true))
			format(DISCORD_SERVER_IP, sizeof(DISCORD_SERVER_IP), "%s", val);
	}
	fclose(f);
	return 1;
}

stock DCC_Channel: Discord_ResolveChannel(&DCC_Channel: cache, const id[])
{
	if (id[0] == EOS || !strcmp(id, "0") || !strcmp(id, "YOUR_", true, 5))
		return DCC_INVALID_CHANNEL;
	if (!cache)
		cache = DCC_FindChannelById(id);
	return cache;
}

stock DCC_Role: Discord_ResolveRole(const id[])
{
	if (id[0] == EOS || !strcmp(id, "0") || !strcmp(id, "YOUR_", true, 5))
		return DCC_INVALID_ROLE;
	return DCC_FindRoleById(id);
}

stock Discord_SendToChannel(&DCC_Channel: cache, const id[], const message[])
{
	new DCC_Channel: channel = Discord_ResolveChannel(cache, id);
	if (!channel)
		return 0;

	return DCC_SendChannelMessage(channel, message);
}

stock Discord_SendEmbed(DCC_Channel: channel, const title[], const description[], color = 0x5865F2, const footer[] = "")
{
	if (!channel)
		return 0;

	new footerText[64];
	if (footer[0] != EOS)
		format(footerText, sizeof(footerText), "%s", footer);
	else
		format(footerText, sizeof(footerText), "%s", (ServerName[0] != EOS ? ServerName : "SA-MP Server"));

	new DCC_Embed: embed = DCC_CreateEmbed(title, description, "", "", color, footerText);
	new ret = DCC_SendChannelEmbedMessage(channel, embed);
	DCC_DeleteEmbed(embed);
	return ret;
}

stock Discord_SendSuccessEmbed(DCC_Channel: channel, const title[], const description[])
{
	return Discord_SendEmbed(channel, title, description, 0x2ECC71);
}

stock Discord_SendErrorEmbed(DCC_Channel: channel, const description[])
{
	return Discord_SendEmbed(channel, "Error", description, 0xED4245);
}

stock Discord_GetServerIP(dest[], maxlen = sizeof(dest))
{
	if (strlen(DISCORD_SERVER_IP) > 0 && strcmp(DISCORD_SERVER_IP, "127.0.0.1:7777") != 0 && strcmp(DISCORD_SERVER_IP, "0") != 0)
	{
		format(dest, maxlen, "%s", DISCORD_SERVER_IP);
		return 1;
	}

#if defined _INC_open_mp
	GetConsoleVarAsString("network.public_addr", dest, maxlen);
	if (!strlen(dest) || !strcmp(dest, "0.0.0.0"))
		GetConsoleVarAsString("network.bind", dest, maxlen);
	new port = GetConsoleVarAsInt("network.port");
#else
	GetConsoleVarAsString("bind", dest, maxlen);
	new port = GetConsoleVarAsInt("port");
#endif

	if (port <= 0)
		port = 7777;

	if (strlen(dest) > 0 && strcmp(dest, "0.0.0.0") != 0 && strcmp(dest, "noip") != 0)
	{
		format(dest, maxlen, "%s:%d", dest, port);
		return 1;
	}

	if (strlen(DISCORD_SERVER_IP) > 0 && strcmp(DISCORD_SERVER_IP, "0") != 0)
	{
		format(dest, maxlen, "%s", DISCORD_SERVER_IP);
		return 1;
	}

	format(dest, maxlen, "Port %d", port);
	return 1;
}

public Discord_UpdateStatus()
{
	if (!Discord_ResolveChannel(g_dccChannelMatches, DISCORD_CHANNEL_MATCHES) && !Discord_ResolveChannel(g_dccChannelChat, DISCORD_CHANNEL_CHAT))
		return 0;

	new count = Iter_Count(Player);
	new srvAddr[48];
	Discord_GetServerIP(srvAddr, sizeof(srvAddr));

	new status[96];
	format(status, sizeof(status), "%s | %d/%d players", srvAddr, count, GetMaxPlayers());
	DCC_SetBotActivity(status);
	return 1;
}

public Discord_Init()
{
	Discord_LoadConfig();

	g_dccChannelMatches   = DCC_INVALID_CHANNEL;
	g_dccChannelRounds    = DCC_INVALID_CHANNEL;
	g_dccChannelTop       = DCC_INVALID_CHANNEL;
	g_dccChannelDuels     = DCC_INVALID_CHANNEL;
	g_dccChannelLogs      = DCC_INVALID_CHANNEL;
	g_dccChannelChat      = DCC_INVALID_CHANNEL;
	g_dccChannelCommands  = DCC_INVALID_CHANNEL;
	g_dccChannelAntiCheat = DCC_INVALID_CHANNEL;

	Discord_ResolveChannel(g_dccChannelMatches, DISCORD_CHANNEL_MATCHES);
	Discord_ResolveChannel(g_dccChannelRounds, DISCORD_CHANNEL_ROUNDS);
	Discord_ResolveChannel(g_dccChannelTop, DISCORD_CHANNEL_TOP);
	Discord_ResolveChannel(g_dccChannelDuels, DISCORD_CHANNEL_DUELS);
	Discord_ResolveChannel(g_dccChannelLogs, DISCORD_CHANNEL_LOGS);
	Discord_ResolveChannel(g_dccChannelChat, DISCORD_CHANNEL_CHAT);
	Discord_ResolveChannel(g_dccChannelCommands, DISCORD_CHANNEL_COMMANDS);
	Discord_ResolveChannel(g_dccChannelAntiCheat, DISCORD_CHANNEL_ANTICHEAT);

	Discord_UpdateStatus();
	printf("[AttDef-Discord] Discord integration initialized.");
	return 1;
}

public Discord_SendRoundResult(const message[])
{
	new DCC_Channel: channel = Discord_ResolveChannel(g_dccChannelRounds, DISCORD_CHANNEL_ROUNDS);
	if (!channel)
		return 0;

	new clean[256];
	Discord_StripColors(message, clean, sizeof(clean));
	return Discord_SendEmbed(channel, "Round Result", clean, 0x5865F2, va_return("%s | Round Updates", (ServerName[0] != EOS ? ServerName : "SA-MP Server")));
}

public Discord_SendRoundResultWithStats(const summary[], const names[][MAX_PLAYER_NAME], const scores[][], count, topOverallID)
{
	new DCC_Channel: channel = Discord_ResolveChannel(g_dccChannelRounds, DISCORD_CHANNEL_ROUNDS);
	if (!channel)
		return 0;

	new cleanSummary[256];
	Discord_StripColors(summary, cleanSummary, sizeof(cleanSummary));

	new DCC_Embed: embed = DCC_CreateEmbed("Round Finished", cleanSummary, "", "", 0x5865F2, va_return("%s | Round Results", (ServerName[0] != EOS ? ServerName : "SA-MP Server")));

	if (topOverallID != -1 && topOverallID < count)
	{
		DCC_AddEmbedField(embed, "Round MVP", names[topOverallID], false);
	}

	new table[1024];
	format(table, sizeof(table), "```\n%-20s %-5s %-7s %-6s %-6s\n------------------------------------------------\n", "Player", "Team", "Damage", "Kills", "Deaths");

	new maxDisplay = (count > 20) ? 20 : count;
	for (new i = 0; i < maxDisplay; i++)
	{
		new teamTag[8];
		switch (scores[i][player_Team])
		{
			case ATTACKER, ATTACKER_SUB: format(teamTag, sizeof(teamTag), "Att");
			case DEFENDER, DEFENDER_SUB: format(teamTag, sizeof(teamTag), "Def");
			default: format(teamTag, sizeof(teamTag), "Spec");
		}

		new line[96];
		format(line, sizeof(line), "%-20s %-5s %-7d %-6d %-6d\n",
			names[i], teamTag, scores[i][player_Score], scores[i][player_Kills], scores[i][player_Deaths]);
		strcat(table, line, sizeof(table));
	}
	strcat(table, "```", sizeof(table));
	DCC_AddEmbedField(embed, "Scoreboard", table, false);

	new ret = DCC_SendChannelEmbedMessage(channel, embed);
	DCC_DeleteEmbed(embed);
	return ret;
}

public Discord_SendMatchResult(const message[])
{
	new DCC_Channel: channel = Discord_ResolveChannel(g_dccChannelMatches, DISCORD_CHANNEL_MATCHES);
	if (!channel)
		return 0;

	new clean[256];
	Discord_StripColors(message, clean, sizeof(clean));
	return Discord_SendEmbed(channel, "Match Result", clean, 0xF1C40F, va_return("%s | Match Results", (ServerName[0] != EOS ? ServerName : "SA-MP Server")));
}

public Discord_SendMatchResultWithStats(const summary[], const names[][MAX_PLAYER_NAME], const scores[][], count, topOverallID)
{
	new DCC_Channel: channel = Discord_ResolveChannel(g_dccChannelMatches, DISCORD_CHANNEL_MATCHES);
	if (!channel)
		return 0;

	new cleanSummary[256];
	Discord_StripColors(summary, cleanSummary, sizeof(cleanSummary));

	new DCC_Embed: embed = DCC_CreateEmbed("Match Finished", cleanSummary, "", "", 0xF1C40F, va_return("%s | Match Results", (ServerName[0] != EOS ? ServerName : "SA-MP Server")));

	if (topOverallID != -1 && topOverallID < count)
	{
		DCC_AddEmbedField(embed, "Match MVP", names[topOverallID], false);
	}

	new table[1100];
	format(table, sizeof(table), "```\n%-20s %-5s %-7s %-6s %-7s %-6s\n--------------------------------------------------------\n", "Player", "Team", "Damage", "Kills", "Deaths", "Played");

	new maxDisplay = (count > 20) ? 20 : count;
	for (new i = 0; i < maxDisplay; i++)
	{
		new teamTag[8];
		switch (scores[i][player_Team])
		{
			case ATTACKER, ATTACKER_SUB: format(teamTag, sizeof(teamTag), "Att");
			case DEFENDER, DEFENDER_SUB: format(teamTag, sizeof(teamTag), "Def");
			default: format(teamTag, sizeof(teamTag), "Spec");
		}

		new line[96];
		format(line, sizeof(line), "%-20s %-5s %-7d %-6d %-7d %-6d\n",
			names[i], teamTag, scores[i][player_Score], scores[i][player_Kills], scores[i][player_Deaths], scores[i][player_TPlayed]);
		strcat(table, line, sizeof(table));
	}
	strcat(table, "```", sizeof(table));
	DCC_AddEmbedField(embed, "Final Scoreboard", table, false);

	new ret = DCC_SendChannelEmbedMessage(channel, embed);
	DCC_DeleteEmbed(embed);
	return ret;
}

public Discord_SendDuelResult(const message[])
{
	new DCC_Channel: channel = Discord_ResolveChannel(g_dccChannelDuels, DISCORD_CHANNEL_DUELS);
	if (!channel)
		return 0;

	new clean[256];
	Discord_StripColors(message, clean, sizeof(clean));
	return Discord_SendEmbed(channel, "Duel Finished", clean, 0x9B59B6, va_return("%s | Duel Arena", (ServerName[0] != EOS ? ServerName : "SA-MP Server")));
}

public Discord_SendPlayerLog(const message[])
{
	new DCC_Channel: channel = Discord_ResolveChannel(g_dccChannelLogs, DISCORD_CHANNEL_LOGS);
	if (!channel)
		return 0;

	new clean[256];
	Discord_StripColors(message, clean, sizeof(clean));

	new dccMsg[300];
	if (clean[0] == '`' && clean[1] == '`' && clean[2] == '`')
	{
		format(dccMsg, sizeof(dccMsg), "%s", clean);
	}
	else if (strfind(clean, "disconnected", true) != -1 || strfind(clean, "left the server", true) != -1)
	{
		format(dccMsg, sizeof(dccMsg), "```diff\n- %s\n```", clean);
	}
	else if (strfind(clean, "connected", true) != -1 || strfind(clean, "joined the server", true) != -1)
	{
		format(dccMsg, sizeof(dccMsg), "```diff\n+ %s\n```", clean);
	}
	else
	{
		format(dccMsg, sizeof(dccMsg), "```diff\n! %s\n```", clean);
	}

	return DCC_SendChannelMessage(channel, dccMsg);
}

public Discord_SendAntiCheatAlert(playerid, const detection[], const details[])
{
	new DCC_Channel: channel = Discord_ResolveChannel(g_dccChannelAntiCheat, DISCORD_CHANNEL_ANTICHEAT);
	if (!channel)
		return 0;

	new playerName[MAX_PLAYER_NAME];
	if (playerid >= 0 && playerid < MAX_PLAYERS && Player[playerid][Name][0] != EOS)
		format(playerName, sizeof(playerName), "%s", Player[playerid][Name]);
	else
		GetPlayerName(playerid, playerName, sizeof(playerName));

	if (playerName[0] == EOS)
		format(playerName, sizeof(playerName), "Unknown");

	new desc[384];
	format(
		desc,
		sizeof(desc),
		"**Player:** `%s` (ID: `%d`)\n**Detection:** `%s`\n**Action:** Kicked from the server\n**Details:** %s",
		playerName,
		playerid,
		detection,
		details
	);

	return Discord_SendEmbed(
		channel,
		"Anti-Cheat Detection",
		desc,
		0xED4245,
		va_return("%s | AC Black Diamond", (ServerName[0] != EOS ? ServerName : "SA-MP Server"))
	);
}

public Discord_SendChatMessage(const player_name[], const text[])
{
	new DCC_Channel: channel = Discord_ResolveChannel(g_dccChannelChat, DISCORD_CHANNEL_CHAT);
	if (!channel)
		return 0;

	new cleanText[144];
	Discord_StripColors(text, cleanText, sizeof(cleanText));

	new dccMsg[256];
	format(dccMsg, sizeof(dccMsg), "```\n%s: %s\n```", player_name, cleanText);
	return DCC_SendChannelMessage(channel, dccMsg);
}

static Discord_BuildTopSection(dest[], maxlen, const query[], bool: isRatio = false)
{
	new DBResult: res = DB_ExecuteQuery(sqliteconnection, query);
	if (!res || DB_GetRowCount(res) <= 0)
	{
		if (res) DB_FreeResultSet(res);
		return 0;
	}

	dest[0] = EOS;
	strcat(dest, "```\n", maxlen);
	new name[MAX_PLAYER_NAME], v1Str[24], v2Str[24], line[96], rank = 1;
	do
	{
		DB_GetFieldStringByName(res, "Name", name, sizeof(name));
		DB_GetFieldStringByName(res, "Val1", v1Str, sizeof(v1Str));
		DB_GetFieldStringByName(res, "Val2", v2Str, sizeof(v2Str));
		new v1 = strval(v1Str), v2 = strval(v2Str);
		if (isRatio)
		{
			new Float: kd = (v2 <= 0) ? float(v1) : (float(v1) / float(v2));
			format(line, sizeof(line), "%2d. %-20s %6.2f (%d/%d)\n", rank, name, kd, v1, v2);
		}
		else
			format(line, sizeof(line), "%2d. %-20s %8d\n", rank, name, v1);
		strcat(dest, line, maxlen);
		rank++;
	}
	while (DB_SelectNextRow(res));
	DB_FreeResultSet(res);
	strcat(dest, "```", maxlen);
	return 1;
}

public Discord_PostDailyTop()
{
	new DCC_Channel: channel = Discord_ResolveChannel(g_dccChannelTop, DISCORD_CHANNEL_TOP);
	if (!channel)
		return 0;

	new year, month, day;
	getdate(year, month, day);

	new title[64], desc[96];
	format(title, sizeof(title), "Daily Leaderboard");
	format(desc, sizeof(desc), "Rankings for %02d/%02d/%d", day, month, year);

	new DCC_Embed: embed = DCC_CreateEmbed(title, desc, "", "", 0xF1C40F, va_return("%s | Daily Top", (ServerName[0] != EOS ? ServerName : "SA-MP Server")));

	new fText[512];
	if (Discord_BuildTopSection(fText, sizeof(fText), "SELECT `Name`, `TKills` AS `Val1`, `TDeaths` AS `Val2` FROM `Players` WHERE `TKills` > 0 ORDER BY `TKills` DESC LIMIT 5"))
		DCC_AddEmbedField(embed, "Top 5 All-Time Kills", fText, false);

	if (Discord_BuildTopSection(fText, sizeof(fText), "SELECT `Name`, `TKills` AS `Val1`, `TDeaths` AS `Val2` FROM `Players` WHERE `TKills` >= 25 ORDER BY (CAST(`TKills` AS REAL) / CASE WHEN `TDeaths` <= 0 THEN 1 ELSE `TDeaths` END) DESC LIMIT 5", true))
		DCC_AddEmbedField(embed, "Top 5 K/D Ratio (Min. 25 Kills)", fText, false);

	if (Discord_BuildTopSection(fText, sizeof(fText), "SELECT `Name`, `TTDMG` AS `Val1`, 0 AS `Val2` FROM `Players` WHERE `TTDMG` > 0 ORDER BY `TTDMG` DESC LIMIT 5"))
		DCC_AddEmbedField(embed, "Top 5 All-Time Damage", fText, false);

	if (Discord_BuildTopSection(fText, sizeof(fText), "SELECT `Name`, `DWon` AS `Val1`, `DLost` AS `Val2` FROM `Players` WHERE `DWon` > 0 ORDER BY `DWon` DESC, `DLost` ASC LIMIT 5"))
		DCC_AddEmbedField(embed, "Top 5 Duels Won", fText, false);

	if (Discord_BuildTopSection(fText, sizeof(fText), "SELECT `Name`, `ClutchesWon` AS `Val1`, 0 AS `Val2` FROM `Players` WHERE `ClutchesWon` > 0 ORDER BY `ClutchesWon` DESC LIMIT 5"))
		DCC_AddEmbedField(embed, "Top 5 Clutches Won", fText, false);

	new ret = DCC_SendChannelEmbedMessage(channel, embed);
	DCC_DeleteEmbed(embed);
	return ret;
}

public Discord_TopTimerCheck()
{
	new hour, minute, second, year, month, day;
	gettime(hour, minute, second);
	getdate(year, month, day);

	if (hour == 0 && minute <= 1 && g_lastTopPostDay != day)
	{
		g_lastTopPostDay = day;
		Discord_PostDailyTop();
	}
	return 1;
}

// Auto-assign member role to anyone who joins the Discord guild
public DCC_OnGuildMemberAdd(DCC_Guild:guild, DCC_User:user)
{
	new DCC_Role: role = Discord_ResolveRole(DISCORD_ROLE_MEMBER);
	if (role)
	{
		DCC_AddGuildMemberRole(guild, user, role);
	}
	return 1;
}

stock DCC_Guild: Discord_GetMainGuild(DCC_Guild: currentGuild = DCC_INVALID_GUILD)
{
	if (currentGuild)
		return currentGuild;

	new DCC_Guild: guilds[4];
	new count = DCC_GetAllGuilds(guilds, sizeof(guilds));
	if (count > 0 && guilds[0])
		return guilds[0];

	return DCC_INVALID_GUILD;
}

stock bool: Discord_IsUserAdmin(DCC_Guild:guild, DCC_User:user)
{
	if (!user)
		return false;

	guild = Discord_GetMainGuild(guild);
	if (!guild)
		return false;

	new DCC_Role: adminRole = Discord_ResolveRole(DISCORD_ROLE_ADMIN);
	if (!adminRole)
		return false;

	new bool: hasRole = false;
	DCC_HasGuildMemberRole(guild, user, adminRole, hasRole);
	return hasRole;
}

#define MAX_FAILED_VERIFY_TRACK (64)
new g_VerifyFailUser[MAX_FAILED_VERIFY_TRACK][DCC_ID_SIZE];
new g_VerifyFailCount[MAX_FAILED_VERIFY_TRACK];
new g_VerifyFailLockUntil[MAX_FAILED_VERIFY_TRACK];

stock GetVerifyFailSlot(const userId[])
{
	new oldestSlot = 0;
	new oldestTime = 0x7FFFFFFF;

	for (new i = 0; i < MAX_FAILED_VERIFY_TRACK; i++)
	{
		if (g_VerifyFailUser[i][0] != EOS && !strcmp(g_VerifyFailUser[i], userId))
			return i;

		if (g_VerifyFailUser[i][0] == EOS)
			return i;

		if (g_VerifyFailLockUntil[i] < oldestTime)
		{
			oldestTime = g_VerifyFailLockUntil[i];
			oldestSlot = i;
		}
	}
	return oldestSlot;
}

public DCC_OnMessageCreate(DCC_Message:message)
{
	new DCC_User: author;
	DCC_GetMessageAuthor(message, author);

	new bool: isBot;
	DCC_IsUserBot(author, isBot);
	if (isBot)
		return 1;

	new DCC_Channel: channel;
	DCC_GetMessageChannel(message, channel);

	new DCC_Guild: guild;
	DCC_GetChannelGuild(channel, guild);

	new rawContent[1024];
	DCC_GetMessageContent(message, rawContent, sizeof(rawContent));
	if (rawContent[0] == EOS)
		return 1;

	// Check if message is a bot command (prefix ! or .)
	if (rawContent[0] == DISCORD_PREFIX_CHAR || rawContent[0] == '.')
	{
		new DCC_ChannelType: chType;
		DCC_GetChannelType(channel, chType);
		new bool: isDM = (chType == DM || chType == GROUP_DM || !guild);

		new cmd[32], params[960];
		new spacePos = strfind(rawContent, " ");
		if (spacePos != -1)
		{
			strmid(cmd, rawContent, 1, spacePos, sizeof(cmd));
			strmid(params, rawContent, spacePos + 1, strlen(rawContent), sizeof(params));
			strtrim(params);
		}
		else
		{
			strmid(cmd, rawContent, 1, strlen(rawContent), sizeof(cmd));
			params[0] = EOS;
		}

		new DCC_Channel: cmdChannel = Discord_ResolveChannel(g_dccChannelCommands, DISCORD_CHANNEL_COMMANDS);
		new bool: isCmdChannel = (channel == cmdChannel);
		if (!isCmdChannel && cmdChannel)
		{
			new chId[DCC_ID_SIZE];
			DCC_GetChannelId(channel, chId, sizeof(chId));
			if (!strcmp(chId, DISCORD_CHANNEL_COMMANDS))
				isCmdChannel = true;
		}

		// Allow commands in DISCORD_CHANNEL_COMMANDS, or if user is Admin, or if DM and running !verify, !stats, !server, or !help
		new bool: isAllowedDMCommand = (isDM && (!strcmp(cmd, "verify", true) || !strcmp(cmd, "help", true) || !strcmp(cmd, "commands", true) || !strcmp(cmd, "stats", true) || !strcmp(cmd, "server", true)));
		if (!isCmdChannel && !isAllowedDMCommand && !Discord_IsUserAdmin(guild, author))
			return 1;

		// Typing just the prefix (! or .) or !help / !commands
		if (cmd[0] == EOS || !strcmp(cmd, "help", true) || !strcmp(cmd, "commands", true))
		{
			new bool: isAdmin = Discord_IsUserAdmin(guild, author);

			new DCC_Embed: embed = DCC_CreateEmbed(
				isAdmin ? va_return("%s Bot - Admin Commands", (ServerName[0] != EOS ? ServerName : "SA-MP Server")) : va_return("%s Bot - Server Commands", (ServerName[0] != EOS ? ServerName : "SA-MP Server")),
				"Commands use the prefix `" DISCORD_PREFIX_STR "` before each action:",
				"",
				"",
				isAdmin ? 0xED4245 : 0x5865F2,
				(ServerName[0] != EOS ? ServerName : "SA-MP Server"),
				"",
				"https://assets.open.mp/assets/images/skins/0.png"
			);

			DCC_AddEmbedField(embed, "Player & Statistics",
				"`" DISCORD_PREFIX_STR "stats` - View your profile, K/D, duels and skin\n" \
				"`" DISCORD_PREFIX_STR "stats <name>` - View another player's statistics\n" \
				"`" DISCORD_PREFIX_STR "top` - All-time leaderboards (Kills, Damage, K/D)\n" \
				"`" DISCORD_PREFIX_STR "matchhistory` - Recent match results and MVPs",
				false
			);

			DCC_AddEmbedField(embed, "Account & Server",
				"`" DISCORD_PREFIX_STR "server` - Server status, round and online players\n" \
				"`" DISCORD_PREFIX_STR "verify <code>` - Link your SA-MP account (/linkdiscord in-game)\n" \
				"`" DISCORD_PREFIX_STR "changename <new>` - Change your linked account name",
				false
			);

			// Admin commands only visible to Admin role (1565912106736296097)
			if (isAdmin)
			{
				DCC_AddEmbedField(embed, "Administration",
					"`" DISCORD_PREFIX_STR "kick <id/name> [reason]` - Kick a player from server\n" \
					"`" DISCORD_PREFIX_STR "ban <id/name> [reason]` - Ban a player (or offline IP)\n" \
					"`" DISCORD_PREFIX_STR "unban <ip/name>` - Unban an IP address or player name",
					false
				);
			}

			DCC_SendChannelEmbedMessage(channel, embed);
			DCC_DeleteEmbed(embed);
			return 1;
		}

		// Command: !top
		if (!strcmp(cmd, "top", true))
		{
			new DCC_Embed: embed = DCC_CreateEmbed("Server Leaderboards", "All-time top ranking players", "", "", 0xF1C40F, (ServerName[0] != EOS ? ServerName : "SA-MP Server"));

			new fText[600];
			if (Discord_BuildTopSection(fText, sizeof(fText), "SELECT `Name`, `TKills` AS `Val1`, `TDeaths` AS `Val2` FROM `Players` WHERE `TKills` > 0 ORDER BY `TKills` DESC LIMIT 10"))
				DCC_AddEmbedField(embed, "Top 10 All-Time Kills", fText, false);

			if (Discord_BuildTopSection(fText, sizeof(fText), "SELECT `Name`, `TTDMG` AS `Val1`, 0 AS `Val2` FROM `Players` WHERE `TTDMG` > 0 ORDER BY `TTDMG` DESC LIMIT 5"))
				DCC_AddEmbedField(embed, "Top 5 All-Time Damage", fText, false);

			DCC_SendChannelEmbedMessage(channel, embed);
			DCC_DeleteEmbed(embed);
			return 1;
		}

		// Command: !server
		if (!strcmp(cmd, "server", true))
		{
			new currentRoundName[32];
			if (Current != -1)
				format(currentRoundName, sizeof(currentRoundName), "%s %d", (GameType == BASE ? "Base" : "Arena"), Current);
			else
				format(currentRoundName, sizeof(currentRoundName), "Lobby");

			new desc[160];
			format(desc, sizeof(desc), "Gamemode: **%s (v%s)**\nLocation: **%s** | War Mode: **%s**\nOnline: **%d / %d players**",
				GM_NAME, GM_VERSION, currentRoundName, (WarMode ? "Active" : "Inactive"), Iter_Count(Player), GetMaxPlayers());

			new DCC_Embed: embed = DCC_CreateEmbed("Server Status", desc, "", "", 0x2ECC71, (ServerName[0] != EOS ? ServerName : "SA-MP Server"));

			new roster[1024];
			format(roster, sizeof(roster), "```\n%-4s %-20s %-10s %-6s %-6s\n-------------------------------------------------\n", "ID", "Player", "Team", "Ping", "Score");

			new count = 0;
			foreach (new i : Player)
			{
				new teamName[16];
				switch (Player[i][Team])
				{
					case ATTACKER, ATTACKER_SUB: format(teamName, sizeof(teamName), "Attacker");
					case DEFENDER, DEFENDER_SUB: format(teamName, sizeof(teamName), "Defender");
					case REFEREE: format(teamName, sizeof(teamName), "Referee");
					default: format(teamName, sizeof(teamName), "Lobby");
				}

				new line[96];
				format(line, sizeof(line), "%-4d %-20s %-10s %-6d %-6d\n",
					i, Player[i][Name], teamName, GetPlayerPing(i), GetPlayerScore(i));
				strcat(roster, line, sizeof(roster));
				count++;
			}

			if (count == 0)
				strcat(roster, "No players currently online.\n", sizeof(roster));

			strcat(roster, "```", sizeof(roster));
			DCC_AddEmbedField(embed, "Player Roster", roster, false);

			DCC_SendChannelEmbedMessage(channel, embed);
			DCC_DeleteEmbed(embed);
			return 1;
		}

		// Command: !matchhistory
		if (!strcmp(cmd, "matchhistory", true))
		{
			new DBResult: res = DB_ExecuteQuery(sqliteconnection, "SELECT `PlayedAt`, `TeamA`, `TeamB`, `ScoreA`, `ScoreB`, `Winner`, `MVP` FROM `MatchHistory` ORDER BY `ID` DESC LIMIT 5");
			if (!res || DB_GetRowCount(res) <= 0)
			{
				if (res) DB_FreeResultSet(res);
				return Discord_SendEmbed(channel, "Match History", "No recorded matches found.", 0x5865F2);
			}

			new DCC_Embed: embed = DCC_CreateEmbed("Recent Match History", "Latest 5 recorded matches:", "", "", 0x3498DB, (ServerName[0] != EOS ? ServerName : "SA-MP Server"));

			new playedAt[32], teamA[24], teamB[24], scoreA, scoreB, winner[32], mvp[MAX_PLAYER_NAME], valStr[24];
			new matchNum = 1, fieldTitle[64], fieldVal[160];
			do
			{
				DB_GetFieldStringByName(res, "PlayedAt", playedAt, sizeof(playedAt));
				DB_GetFieldStringByName(res, "TeamA", teamA, sizeof(teamA));
				DB_GetFieldStringByName(res, "TeamB", teamB, sizeof(teamB));
				DB_GetFieldStringByName(res, "ScoreA", valStr, sizeof(valStr)); scoreA = strval(valStr);
				DB_GetFieldStringByName(res, "ScoreB", valStr, sizeof(valStr)); scoreB = strval(valStr);
				DB_GetFieldStringByName(res, "Winner", winner, sizeof(winner));
				DB_GetFieldStringByName(res, "MVP", mvp, sizeof(mvp));

				format(fieldTitle, sizeof(fieldTitle), "#%d :: %s (%d) vs %s (%d)", matchNum, teamA, scoreA, teamB, scoreB);
				format(fieldVal, sizeof(fieldVal), "**Winner:** %s | **MVP:** %s\n*Date:* %s", winner, mvp, playedAt);
				DCC_AddEmbedField(embed, fieldTitle, fieldVal, false);
				matchNum++;
			}
			while (DB_SelectNextRow(res));
			DB_FreeResultSet(res);

			DCC_SendChannelEmbedMessage(channel, embed);
			DCC_DeleteEmbed(embed);
			return 1;
		}

		// Command: !stats
		if (!strcmp(cmd, "stats", true))
		{
			new targetAccount[MAX_PLAYER_NAME];
			new discordId[DCC_ID_SIZE];
			DCC_GetUserId(author, discordId, sizeof(discordId));

			if (!isnull(params))
			{
				format(targetAccount, sizeof(targetAccount), "%s", params);
			}
			else
			{
				new query[192];
				format(query, sizeof(query), "SELECT `AccountName` FROM `DiscordLinks` WHERE `DiscordID` = '%q'", discordId);
				new DBResult: res = DB_ExecuteQuery(sqliteconnection, query);
				if (!res || DB_GetRowCount(res) <= 0)
				{
					if (res) DB_FreeResultSet(res);
					return Discord_SendErrorEmbed(channel, "You haven't linked your account yet! Type /linkdiscord in-game and " DISCORD_PREFIX_STR "verify <code> here.\nOr use " DISCORD_PREFIX_STR "stats <player> to view another player's stats.");
				}
				DB_GetFieldStringByName(res, "AccountName", targetAccount, sizeof(targetAccount));
				DB_FreeResultSet(res);
			}

			new query[256];
			format(query, sizeof(query), "SELECT `Name`, `TKills`, `TDeaths`, `TAssists`, `TTDMG`, `DWon`, `DLost`, `ClutchesWon`, `Lvl`, `Exp`, `pSkin` FROM `Players` WHERE `Name` = '%q'", targetAccount);
			new DBResult: sRes = DB_ExecuteQuery(sqliteconnection, query);
			if (!sRes || DB_GetRowCount(sRes) <= 0)
			{
				if (sRes) DB_FreeResultSet(sRes);
				return Discord_SendErrorEmbed(channel, va_return("Player '%s' was not found in the database.", targetAccount));
			}

			new val[32];
			new name[MAX_PLAYER_NAME];
			DB_GetFieldStringByName(sRes, "Name", name, sizeof(name));

			DB_GetFieldStringByName(sRes, "TKills", val, sizeof(val)); new kills = strval(val);
			DB_GetFieldStringByName(sRes, "TDeaths", val, sizeof(val)); new deaths = strval(val);
			DB_GetFieldStringByName(sRes, "TAssists", val, sizeof(val)); new assists = strval(val);
			DB_GetFieldStringByName(sRes, "TTDMG", val, sizeof(val)); new dmg = strval(val);
			DB_GetFieldStringByName(sRes, "DWon", val, sizeof(val)); new dWon = strval(val);
			DB_GetFieldStringByName(sRes, "DLost", val, sizeof(val)); new dLost = strval(val);
			DB_GetFieldStringByName(sRes, "ClutchesWon", val, sizeof(val)); new clutches = strval(val);
			DB_GetFieldStringByName(sRes, "Lvl", val, sizeof(val)); new lvl = strval(val);
			DB_GetFieldStringByName(sRes, "Exp", val, sizeof(val)); new exp = strval(val);
			DB_GetFieldStringByName(sRes, "pSkin", val, sizeof(val)); new skin = strval(val);
			DB_FreeResultSet(sRes);

			new bool: isOnline = false;
			new onlineId = INVALID_PLAYER_ID;
			foreach (new i : Player)
			{
				if (!strcmp(Player[i][Name], name, true))
				{
					isOnline = true;
					onlineId = i;
					if (Player[i][pSkin] != -1)
						skin = Player[i][pSkin];
					break;
				}
			}

			new skinId = skin;
			if (skinId < 0 || skinId > 311 || skinId == 74)
				skinId = 0;

			new skinUrl[128];
			format(skinUrl, sizeof(skinUrl), "https://assets.open.mp/assets/images/skins/%d.png", skinId);

			new Float: kd = float(kills) / float(deaths > 0 ? deaths : 1);
			new Float: duelWr = (dWon + dLost > 0) ? (float(dWon) * 100.0 / float(dWon + dLost)) : 0.0;

			new title[64], desc[128];
			format(title, sizeof(title), "Player Profile: %s", name);
			format(desc, sizeof(desc), "Status: **%s** %s | Level: **%d** (Exp: %d)",
				(isOnline ? "ONLINE" : "OFFLINE"),
				(isOnline ? va_return("(ID: %d)", onlineId) : ""),
				lvl, exp
			);

			new embedColor = isOnline ? 0x2ECC71 : 0x3498DB;

			new DCC_Embed: embed = DCC_CreateEmbed(
				title,
				desc,
				"",
				"",
				embedColor,
				va_return("%s | Player Statistics", (ServerName[0] != EOS ? ServerName : "SA-MP Server")),
				"",
				skinUrl
			);
			DCC_SetEmbedThumbnail(embed, skinUrl);

			new fCombat[128];
			format(fCombat, sizeof(fCombat), "Kills: **%d**\nDeaths: **%d**\nK/D: **%.2f**\nAssists: **%d**", kills, deaths, kd, assists);
			DCC_AddEmbedField(embed, "Combat", fCombat, true);

			new fDmg[128];
			format(fDmg, sizeof(fDmg), "Total Damage: **%d**\nClutches Won: **%d**\nSkin ID: **%d**", dmg, clutches, skinId);
			DCC_AddEmbedField(embed, "Performance", fDmg, true);

			new fDuels[128];
			format(fDuels, sizeof(fDuels), "Won: **%d**\nLost: **%d**\nWinrate: **%.1f%%**", dWon, dLost, duelWr);
			DCC_AddEmbedField(embed, "Duels", fDuels, false);

			DCC_SendChannelEmbedMessage(channel, embed);
			DCC_DeleteEmbed(embed);
			return 1;
		}

		// Command: !verify <code>
		if (!strcmp(cmd, "verify", true))
		{
			// If sent in a public channel, delete the user message immediately to keep the code confidential
			if (!isDM)
			{
				DCC_DeleteMessage(message);
			}

			new discordId[DCC_ID_SIZE];
			DCC_GetUserId(author, discordId, sizeof(discordId));

			new failSlot = GetVerifyFailSlot(discordId);
			new now = gettime();

			if (g_VerifyFailUser[failSlot][0] != EOS && !strcmp(g_VerifyFailUser[failSlot], discordId))
			{
				if (now < g_VerifyFailLockUntil[failSlot])
				{
					new remaining = g_VerifyFailLockUntil[failSlot] - now;
					return Discord_SendErrorEmbed(channel, va_return("Anti-BruteForce: Verification locked for %d seconds. Please wait.", remaining));
				}
			}

			new code = strval(params);
			if (code <= 0)
				return Discord_SendErrorEmbed(channel, "Usage: " DISCORD_PREFIX_STR "verify <code> (Type /linkdiscord in-game to get your code).");

			new foundPlayer = INVALID_PLAYER_ID;
			foreach (new i : Player)
			{
				if (Player[i][Logged] && g_discordVerifyCode[i] == code && now <= g_discordVerifyExpire[i] && !strcmp(Player[i][Name], g_discordVerifyAccount[i]))
				{
					foundPlayer = i;
					break;
				}
			}

			if (foundPlayer == INVALID_PLAYER_ID)
			{
				format(g_VerifyFailUser[failSlot], sizeof(g_VerifyFailUser[]), "%s", discordId);
				g_VerifyFailCount[failSlot]++;
				if (g_VerifyFailCount[failSlot] >= 3)
				{
					g_VerifyFailLockUntil[failSlot] = now + 180;
					return Discord_SendErrorEmbed(channel, "Invalid or expired code! Too many failed attempts. Verification locked for 3 minutes.");
				}

				new attemptsLeft = 3 - g_VerifyFailCount[failSlot];
				return Discord_SendErrorEmbed(channel, va_return("Invalid or expired code! (%d attempts remaining before temporary lockout).", attemptsLeft));
			}

			// Security: Check if this SAMP account is already linked to another Discord ID
			new checkQ[192];
			format(checkQ, sizeof(checkQ), "SELECT `DiscordID` FROM `DiscordLinks` WHERE `AccountName` = '%q'", Player[foundPlayer][Name]);
			new DBResult: cRes = DB_ExecuteQuery(sqliteconnection, checkQ);
			if (cRes && DB_GetRowCount(cRes) > 0)
			{
				new existingDiscId[DCC_ID_SIZE];
				DB_GetFieldStringByName(cRes, "DiscordID", existingDiscId, sizeof(existingDiscId));
				DB_FreeResultSet(cRes);
				if (strcmp(existingDiscId, discordId) != 0)
				{
					return Discord_SendErrorEmbed(channel, "Security Protection: That SA-MP account is already linked to another Discord user!");
				}
			}
			if (cRes) DB_FreeResultSet(cRes);

			// Link in database
			new query[256];
			format(query, sizeof(query), "INSERT OR REPLACE INTO `DiscordLinks` (`DiscordID`, `AccountName`) VALUES ('%q', '%q')", discordId, Player[foundPlayer][Name]);
			if (!DatabaseExecuteWrite(query, "link discord account"))
				return Discord_SendErrorEmbed(channel, "Database error while saving verification.");

			// Clear code and reset failure tracking
			g_discordVerifyCode[foundPlayer] = 0;
			g_discordVerifyExpire[foundPlayer] = 0;
			g_discordVerifyAccount[foundPlayer][0] = EOS;

			g_VerifyFailCount[failSlot] = 0;
			g_VerifyFailLockUntil[failSlot] = 0;
			g_VerifyFailUser[failSlot][0] = EOS;

			va_SendClientMessage(foundPlayer, -1, "{93DCAC}\xBB Your SAMP account has been verified and linked to Discord!");

			guild = Discord_GetMainGuild(guild);
			if (guild)
			{
				new DCC_Role: memberRole = Discord_ResolveRole(DISCORD_ROLE_MEMBER);
				if (memberRole)
					DCC_AddGuildMemberRole(guild, author, memberRole);
			}

			return Discord_SendSuccessEmbed(channel, "Verification Successful", va_return("Discord linked to SA-MP account: **%s**", Player[foundPlayer][Name]));
		}

		// Command: !changename <new_name>
		if (!strcmp(cmd, "changename", true))
		{
			if (isnull(params))
				return Discord_SendErrorEmbed(channel, "Usage: " DISCORD_PREFIX_STR "changename <New_Name>");

			new discordId[DCC_ID_SIZE];
			DCC_GetUserId(author, discordId, sizeof(discordId));

			new query[256];
			format(query, sizeof(query), "SELECT `AccountName` FROM `DiscordLinks` WHERE `DiscordID` = '%q'", discordId);
			new DBResult: res = DB_ExecuteQuery(sqliteconnection, query);
			if (!res || DB_GetRowCount(res) <= 0)
			{
				if (res) DB_FreeResultSet(res);
				return Discord_SendErrorEmbed(channel, "You must link your account first! Type /linkdiscord in-game.");
			}

			new currentAccountName[MAX_PLAYER_NAME];
			DB_GetFieldStringByName(res, "AccountName", currentAccountName, sizeof(currentAccountName));
			DB_FreeResultSet(res);

			if (strlen(params) < 3 || strlen(params) >= MAX_PLAYER_NAME)
				return Discord_SendErrorEmbed(channel, "Invalid name length (3 to 20 characters required).");

			if (!IsValidNickName(params))
				return Discord_SendErrorEmbed(channel, "Invalid nickname format.");

			// Check if new name is already taken in Players table
			format(query, sizeof(query), "SELECT `Name` FROM `Players` WHERE `Name` = '%q'", params);
			new DBResult: checkRes = DB_ExecuteQuery(sqliteconnection, query);
			if (checkRes && DB_GetRowCount(checkRes) > 0)
			{
				DB_FreeResultSet(checkRes);
				return Discord_SendErrorEmbed(channel, "That name is already registered by another player.");
			}
			if (checkRes) DB_FreeResultSet(checkRes);

			// Update in database
			format(query, sizeof(query), "UPDATE `Players` SET `Name` = '%q' WHERE `Name` = '%q'", params, currentAccountName);
			if (!DatabaseExecuteWrite(query, "discord changename players"))
				return Discord_SendErrorEmbed(channel, "Could not update name in database.");

			format(query, sizeof(query), "UPDATE `DiscordLinks` SET `AccountName` = '%q' WHERE `DiscordID` = '%q'", params, discordId);
			DatabaseExecuteWrite(query, "discord changename links");

			// If player is currently online, update their in-game name
			foreach (new i : Player)
			{
				if (!strcmp(Player[i][Name], currentAccountName, false))
				{
					SetPlayerName(i, params);
					format(Player[i][Name], MAX_PLAYER_NAME, "%s", params);
					new NewName[MAX_PLAYER_NAME];
					NewName = RemoveClanTagFromName(i);
					if (strlen(NewName) != 0)
						Player[i][NameWithoutTag] = NewName;
					else
						Player[i][NameWithoutTag] = Player[i][Name];
					va_SendClientMessage(i, -1, "{93DCAC}\xBB Your account name was changed to {FFFFFF}%s {93DCAC}via Discord!", params);
					break;
				}
			}

			return Discord_SendSuccessEmbed(channel, "Name Changed", va_return("Account name successfully updated: **%s** -> **%s**", currentAccountName, params));
		}

		// Admin Command: !kick <id/name> [reason]
		if (!strcmp(cmd, "kick", true))
		{
			if (!Discord_IsUserAdmin(guild, author))
				return Discord_SendErrorEmbed(channel, "Access Denied: You do not have the Admin role.");

			new targetParam[MAX_PLAYER_NAME], reason[128];
			new sp = strfind(params, " ");
			if (sp != -1)
			{
				strmid(targetParam, params, 0, sp, sizeof(targetParam));
				strmid(reason, params, sp + 1, strlen(params), sizeof(reason));
			}
			else
			{
				format(targetParam, sizeof(targetParam), "%s", params);
				format(reason, sizeof(reason), "Kicked from Discord");
			}

			if (isnull(targetParam))
				return Discord_SendErrorEmbed(channel, "Usage: " DISCORD_PREFIX_STR "kick <player_id_or_name> [reason]");

			new targetId = INVALID_PLAYER_ID;
			if (IsNumeric(targetParam))
			{
				targetId = strval(targetParam);
				if (!IsPlayerConnected(targetId))
					targetId = INVALID_PLAYER_ID;
			}
			else
			{
				foreach (new i : Player)
				{
					if (!strcmp(Player[i][Name], targetParam, true))
					{
						targetId = i;
						break;
					}
				}
			}

			if (targetId == INVALID_PLAYER_ID)
				return Discord_SendErrorEmbed(channel, "Player is not online.");

			new ansiReason[128];
			Discord_Utf8ToAnsi(reason, ansiReason, sizeof(ansiReason));
			va_SendClientMessageToAll(-1, "" COL_PRIM "[Discord Admin] kicked " COL_SEC "%s " COL_PRIM "[Reason: " COL_SEC "%s" COL_PRIM "]", Player[targetId][Name], ansiReason);
			SchedulePlayerKick(targetId, 350);

			return Discord_SendSuccessEmbed(channel, "Player Kicked", va_return("Player **%s** (ID: %d) has been kicked.\nReason: %s", Player[targetId][Name], targetId, reason));
		}

		// Admin Command: !ban <id/name> [reason]
		if (!strcmp(cmd, "ban", true))
		{
			if (!Discord_IsUserAdmin(guild, author))
				return Discord_SendErrorEmbed(channel, "Access Denied: You do not have the Admin role.");

			new targetParam[MAX_PLAYER_NAME], reason[128];
			new sp = strfind(params, " ");
			if (sp != -1)
			{
				strmid(targetParam, params, 0, sp, sizeof(targetParam));
				strmid(reason, params, sp + 1, strlen(params), sizeof(reason));
			}
			else
			{
				format(targetParam, sizeof(targetParam), "%s", params);
				format(reason, sizeof(reason), "Banned from Discord");
			}

			if (isnull(targetParam))
				return Discord_SendErrorEmbed(channel, "Usage: " DISCORD_PREFIX_STR "ban <player_id_or_name> [reason]");

			new targetId = INVALID_PLAYER_ID;
			if (IsNumeric(targetParam))
			{
				targetId = strval(targetParam);
				if (!IsPlayerConnected(targetId))
					targetId = INVALID_PLAYER_ID;
			}
			else
			{
				foreach (new i : Player)
				{
					if (!strcmp(Player[i][Name], targetParam, true))
					{
						targetId = i;
						break;
					}
				}
			}

			if (targetId != INVALID_PLAYER_ID)
			{
				new ansiReason[128];
				Discord_Utf8ToAnsi(reason, ansiReason, sizeof(ansiReason));
				va_SendClientMessageToAll(-1, "" COL_PRIM "[Discord Admin] banned " COL_SEC "%s " COL_PRIM "[Reason: " COL_SEC "%s" COL_PRIM "]", Player[targetId][Name], ansiReason);
				SetTimerEx("BanExDelayed", 350, false, "dis", targetId, PlayerConnectionGeneration[targetId], va_return("[Discord] %s", ansiReason));

				return Discord_SendSuccessEmbed(channel, "Player Banned", va_return("Player **%s** (ID: %d) has been banned.\nReason: %s", Player[targetId][Name], targetId, reason));
			}

			// If offline, look up IP in database
			new q[256];
			format(q, sizeof(q), "SELECT `IP` FROM `Players` WHERE `Name` = '%q'", targetParam);
			new DBResult: ipRes = DB_ExecuteQuery(sqliteconnection, q);
			if (ipRes && DB_GetRowCount(ipRes) > 0)
			{
				new ipStr[16];
				DB_GetFieldStringByName(ipRes, "IP", ipStr, sizeof(ipStr));
				DB_FreeResultSet(ipRes);

				va_SendRconCommand("banip %s", ipStr);
				SendRconCommand("reloadbans");

				return Discord_SendSuccessEmbed(channel, "IP Banned", va_return("Player **%s** was offline; banned IP **%s**.\nReason: %s", targetParam, ipStr, reason));
			}
			if (ipRes) DB_FreeResultSet(ipRes);

			return Discord_SendErrorEmbed(channel, "Player not online and no IP found in database.");
		}

		// Admin Command: !unban <ip_or_name>
		if (!strcmp(cmd, "unban", true))
		{
			if (!Discord_IsUserAdmin(guild, author))
				return Discord_SendErrorEmbed(channel, "Access Denied: You do not have the Admin role.");

			if (isnull(params))
				return Discord_SendErrorEmbed(channel, "Usage: " DISCORD_PREFIX_STR "unban <IP_or_PlayerName>");

			// If contains dot, assume it's an IP
			if (strfind(params, ".") != -1)
			{
				va_SendRconCommand("unbanip %s", params);
				SendRconCommand("reloadbans");

				return Discord_SendSuccessEmbed(channel, "Unban Successful", va_return("Unbanned IP **%s** successfully.", params));
			}

			// Look up IP in database for player name
			new q[256];
			format(q, sizeof(q), "SELECT `IP` FROM `Players` WHERE `Name` = '%q'", params);
			new DBResult: ipRes = DB_ExecuteQuery(sqliteconnection, q);
			if (ipRes && DB_GetRowCount(ipRes) > 0)
			{
				new ipStr[16];
				DB_GetFieldStringByName(ipRes, "IP", ipStr, sizeof(ipStr));
				DB_FreeResultSet(ipRes);

				va_SendRconCommand("unbanip %s", ipStr);
				SendRconCommand("reloadbans");

				return Discord_SendSuccessEmbed(channel, "Unban Successful", va_return("Unbanned **%s** (IP: %s) successfully.", params, ipStr));
			}
			if (ipRes) DB_FreeResultSet(ipRes);

			return Discord_SendErrorEmbed(channel, "No IP found for that player in database.");
		}

		// Unknown command handler
		return Discord_SendErrorEmbed(channel, va_return("Unknown command: '%s'. Type %s or %shelp to view all available commands.", cmd, DISCORD_PREFIX_STR, DISCORD_PREFIX_STR));
	}

	// Normal chat bridge: only forward to server if typed in DISCORD_CHANNEL_CHAT
	Discord_ResolveChannel(g_dccChannelChat, DISCORD_CHANNEL_CHAT);

	if (g_dccChannelChat && channel == g_dccChannelChat)
	{
		new authorName[DCC_USERNAME_SIZE], ansiAuthor[DCC_USERNAME_SIZE], ansiContent[144];
		DCC_GetUserName(author, authorName, sizeof(authorName));
		Discord_Utf8ToAnsi(authorName, ansiAuthor, sizeof(ansiAuthor));
		Discord_Utf8ToAnsi(rawContent, ansiContent, sizeof(ansiContent));

		if (ansiContent[0] != EOS)
			va_SendClientMessageToAll(-1, "{7289DA}[Discord] {FFFFFF}%s: {CCCCCC}%s", ansiAuthor, ansiContent);
	}
	return 1;
}

#include <YSI_Coding\y_hooks>

hook OnRoundEndEvent(winnerid, reason, topOverallID)
{
	Discord_SendRoundResultWithStats(LastRoundSummary, RoundResultNames, playerScores, LastRoundScoreCount, topOverallID);
	return 1;
}

hook OnMatchEndEvent(winnerid, scoreAtt, scoreDef, topOverallID)
{
	Discord_SendMatchResultWithStats(LastMatchSummary, RoundResultNames, playerScores, LastMatchScoreCount, topOverallID);
	return 1;
}

hook OnDuelEndEvent(winner, loser, winnerScore, loserScore)
{
	new dDuelMsg[200];
	format(dDuelMsg, sizeof(dDuelMsg), "```asciidoc\n[DUEL RESULT]\nWinner : %s\nLoser  : %s\nScore  : %d - %d\n```", Player[winner][Name], Player[loser][Name], winnerScore, loserScore);
	Discord_SendDuelResult(dDuelMsg);
	return 1;
}

hook OnPlayerConnect(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	new dJoinMsg[144];
	format(dJoinMsg, sizeof(dJoinMsg), "[Connected] %s (ID: %d) has joined the server.", name, playerid);
	Discord_SendPlayerLog(dJoinMsg);
	SetTimer("Discord_UpdateStatus", 1000, false);
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	static const disconnectReasons[][] = {
		"(Timeout/Crash)",
		"(Quit)",
		"(Kicked/Banned)",
		"(Custom)"
	};
	new dDiscLog[144];
	format(dDiscLog, sizeof(dDiscLog), "[Disconnected] %s (ID: %d) has left the server %s.",
		Player[playerid][Name], playerid,
		(reason >= 0 && reason < sizeof(disconnectReasons) ? disconnectReasons[reason] : "(Unknown)"));
	Discord_SendPlayerLog(dDiscLog);
	SetTimer("Discord_UpdateStatus", 1000, false);
	return 1;
}

hook OnPlayerChatMessage(playerid, const text[])
{
	Discord_SendChatMessage(Player[playerid][Name], text);
	return 1;
}

hook OnGameModeInit()
{
	Discord_Init();
	SetTimer("Discord_Init", 5000, false);
	SetTimer("Discord_TopTimerCheck", 30000, true);
	SetTimer("Discord_UpdateStatus", 60000, true);
	return 1;
}
