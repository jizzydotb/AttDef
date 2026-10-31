#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_discord
	#endinput
#endif
#define _attdef_mod_discord

/*
	Discord Connector Integration
*/

new DISCORD_CHANNEL_MATCHES[DCC_ID_SIZE]   = "0";
new DISCORD_CHANNEL_ROUNDS[DCC_ID_SIZE]    = "0";
new DISCORD_CHANNEL_TOP[DCC_ID_SIZE]       = "0";
new DISCORD_CHANNEL_DUELS[DCC_ID_SIZE]     = "0";
new DISCORD_CHANNEL_LOGS[DCC_ID_SIZE]      = "0";
new DISCORD_CHANNEL_CHAT[DCC_ID_SIZE]      = "0";
new DISCORD_CHANNEL_COMMANDS[DCC_ID_SIZE]  = "0";
new DISCORD_CHANNEL_ANTICHEAT[DCC_ID_SIZE] = "0";

new DISCORD_ROLE_ADMIN[DCC_ID_SIZE]        = "0";
new DISCORD_ROLE_MEMBER[DCC_ID_SIZE]       = "0";

#if !defined DISCORD_PREFIX_CHAR
	#define DISCORD_PREFIX_CHAR     '!'
#endif

#if !defined DISCORD_PREFIX_STR
	#define DISCORD_PREFIX_STR      "!"
#endif

new DISCORD_SERVER_IP[48]                  = "127.0.0.1:7777";

new g_discordVerifyCode[MAX_PLAYERS];
new g_discordVerifyExpire[MAX_PLAYERS];
new g_discordVerifyAccount[MAX_PLAYERS][MAX_PLAYER_NAME];

forward Discord_Init();
forward Discord_UpdateStatus();
forward Discord_SendRoundResult(const message[]);
forward Discord_SendRoundResultWithStats(const summary[], const names[][MAX_PLAYER_NAME], const scores[][], count, topOverallID);
forward Discord_SendMatchResult(const message[]);
forward Discord_SendMatchResultWithStats(const summary[], const names[][MAX_PLAYER_NAME], const scores[][], count, topOverallID);
forward Discord_SendDuelResult(const message[]);
forward Discord_SendPlayerLog(const message[]);
forward Discord_SendAntiCheatAlert(playerid, const detection[], const details[]);
forward Discord_SendChatMessage(const player_name[], const text[]);
forward Discord_PostDailyTop();
forward Discord_TopTimerCheck();
forward OnPlayerChatMessage(playerid, const text[]);
