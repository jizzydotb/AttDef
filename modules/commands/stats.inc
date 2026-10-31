#if defined _attdef_cmd_stats
	#endinput
#endif
#define _attdef_cmd_stats

YCMD: stats(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "check stats of a player.");

	new targetid = playerid;
	if (!isnull(params) && sscanf(params, "r", targetid))
		return SendUsageMessage(playerid, "/stats [Player Name/ID]");

	if (targetid == INVALID_PLAYER_ID || !IsPlayerConnected(targetid))
		return SendErrorMessage(playerid, "That player isn't connected.");

	new exp = Player[targetid][Exp];
	new lvl = Player[targetid][Lvl];
	new nextLevelExp = GetNextLevelExp(lvl);

	new Float: ratio = (Player[targetid][ATimeDeaths] == 0)
		? 0.0
		: (float(Player[targetid][ATimeKills]) / float(Player[targetid][ATimeDeaths]));

	new playTimeHours = Player[targetid][PlayTime] / 60;
	new playTimeMinutes = Player[targetid][PlayTime] % 60;

	new skinString[32];
	if (Player[targetid][pSkin] == -1)
		format(skinString, sizeof(skinString), "Default Team Skin");
	else
		format(skinString, sizeof(skinString), "%d", Player[targetid][pSkin]);

	new teamColor[10];
	new targetTeam = Player[targetid][Team];
	if (targetTeam >= 0 && targetTeam < MAX_TEAMS && TextColor[targetTeam][0] != EOS)
		format(teamColor, sizeof(teamColor), "%s", TextColor[targetTeam]);
	else
		format(teamColor, sizeof(teamColor), COL_PRIM);

	va_SendClientMessage(playerid, -1, "%s\xBB {FFFFFF}----------------- Player Statistics -----------------", teamColor);
	va_SendClientMessage(playerid, -1, "%s\xBB Player: {FFFFFF}%s " COL_MUTED "| %sSkin: {FFFFFF}%s", teamColor, Player[targetid][Name], teamColor, skinString);
	va_SendClientMessage(playerid, -1, "%s\xBB Level: {FFFFFF}%d " COL_MUTED "| %sEXP: {FFFFFF}%d/%d", teamColor, lvl, teamColor, exp, nextLevelExp);
	va_SendClientMessage(playerid, -1, "%s\xBB K/D: {FFFFFF}%d/%d (%.2f) " COL_MUTED "| %sDamage: {FFFFFF}%d " COL_MUTED "| %sAssists: {FFFFFF}%d", teamColor, Player[targetid][ATimeKills], Player[targetid][ATimeDeaths], ratio, teamColor, Player[targetid][ATimeDMG], teamColor, Player[targetid][ATimeAssists]);
	va_SendClientMessage(playerid, -1, "%s\xBB Clutches: {FFFFFF}%d " COL_MUTED "| %sDuels: {FFFFFF}%d/%d " COL_MUTED "| %sOnline Time: {FFFFFF}%dh %dm", teamColor, Player[targetid][ClutchesWon], teamColor, Player[targetid][DuelsWon], Player[targetid][DuelsLost], teamColor, playTimeHours, playTimeMinutes);
	va_SendClientMessage(playerid, -1, "%s\xBB {FFFFFF}---------------------------------------------------", teamColor);

	return 1;
}
