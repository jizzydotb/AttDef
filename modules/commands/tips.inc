#if defined _attdef_cmd_tips
	#endinput
#endif
#define _attdef_cmd_tips

#define TIPS_INTERVAL1 120000 // 2 minutes in milliseconds
#define TIPS_INTERVAL2 240000 // 4 minutes in milliseconds

new TipsTimer[MAX_PLAYERS];
new NextTipToSend[MAX_PLAYERS];
new TipsScheduleGeneration[MAX_PLAYERS];

forward SendTips(playerid, connectionGeneration, scheduleGeneration);
public SendTips(playerid, connectionGeneration, scheduleGeneration)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid)
		|| PlayerConnectionGeneration[playerid] != connectionGeneration
		|| TipsScheduleGeneration[playerid] != scheduleGeneration || !Player[playerid][TipsEnabled])
		return 0;

	TipsTimer[playerid] = 0;
	// Send a tip message to the player
	switch (random(17))
	{
		case 0: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Use {BD93DC}/gunmenu {FFFFFF}to choose your round weapons.");
		case 1: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Want to differentiate your team-mates on radar by color? Use {BD93DC}/playermarkers");
		case 2: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Use {BD93DC}/weaponbinds {FFFFFF}to code your own weapon key binds.");
		case 3: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}You can drop a gun by holding it and typing {BD93DC}/remgun");
		case 4: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}If you're lagging, you can request netcheck with {BD93DC}/netcheckme");
		case 5: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Disrespect noobs, use {BD93DC}/deathdiss {FFFFFF}command.");
		case 6: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Getting distracted by some textdraws? Try {BD93DC}/hud");
		case 7: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Spectate other players with {BD93DC}/spec {FFFFFF}command.");
		case 8: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Play in different duel arenas by means of {BD93DC}'custom' {FFFFFF}parameter in {BD93DC}/duel {FFFFFF}command.");
		case 9: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Use {BD93DC}/cmdhelp <Command name> {FFFFFF}to get info about any command.");
		case 10: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Change your hit sounds with {BD93DC}/sound {FFFFFF}command.");
		case 11: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Change your time and weather with {BD93DC}/time {FFFFFF}and {BD93DC}/weather commands.");
		case 12: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Press {BD93DC}'Tab' {FFFFFF}and click on a player for quicker operations/commands.");
		case 13: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}You can change your skin with {BD93DC}/skin {FFFFFF}while in /dm or /duel.");
		case 14: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Switch round textdraws style with {BD93DC}/style");
		case 15: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Use {BD93DC}/stats {FFFFFF}to check your own stats and of the others players.");
		case 16: SendClientMessage(playerid, -1, "{BD93DC}\xBB Tip: {FFFFFF}Use {BD93DC}/matchtips {FFFFFF}to display some guidelines you may need while playing.");
	}

	new nextInterval = NextTipToSend[playerid] == 1 ? TIPS_INTERVAL1 : TIPS_INTERVAL2;
	NextTipToSend[playerid] = NextTipToSend[playerid] == 1 ? 2 : 1;
	TipsTimer[playerid] = SetTimerEx(
		"SendTips", nextInterval, false, "iii", playerid, connectionGeneration, scheduleGeneration);
	if (!TipsTimer[playerid])
	{
		printf("[AttDef] Failed to schedule the next tip for player %d.", playerid);
		return 0;
	}
	return 1;
}

forward StartTipsTimers(playerid);
public StartTipsTimers(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	StopTipsTimers(playerid);
	NextTipToSend[playerid] = 1;
	TipsTimer[playerid] = SetTimerEx(
		"SendTips", TIPS_INTERVAL1, false, "iii", playerid, PlayerConnectionGeneration[playerid], TipsScheduleGeneration[playerid]);
	if (!TipsTimer[playerid])
	{
		printf("[AttDef] Failed to start tips for player %d.", playerid);
		return 0;
	}
	return 1;
}

forward StopTipsTimers(playerid);
public StopTipsTimers(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (TipsTimer[playerid]) {
		KillTimer(TipsTimer[playerid]);
		TipsTimer[playerid] = 0;
	}
	TipsScheduleGeneration[playerid]++;
	return 1;
}

YCMD: tips(playerid, params[], help)
{
	if (!IsPlayerConnected(playerid))
		return 0;

	if (help)
		return SendCommandHelpMessage(playerid, "toggle tip messages on/off.");

	new bool: enabled;
	if (isnull(params) || !strcmp(params, "toggle", true))
		enabled = !Player[playerid][TipsEnabled];
	else if (!strcmp(params, "on", true) || !strcmp(params, "1", true) || !strcmp(params, "enable", true))
		enabled = true;
	else if (!strcmp(params, "off", true) || !strcmp(params, "0", true) || !strcmp(params, "disable", true))
		enabled = false;
	else
		return SendUsageMessage(playerid, "/tips [optional: on / off / toggle]");

	if (!DatabaseExecuteWrite(
		va_return("UPDATE Players SET TipsEnabled = %d WHERE Name = '%q'", enabled, Player[playerid][Name]),
		"save player tips setting"))
		return SendErrorMessage(playerid, "Could not save your tips setting. No changes were applied.");

	Player[playerid][TipsEnabled] = enabled;
	if (enabled)
	{
		if (!StartTipsTimers(playerid))
		{
			Player[playerid][TipsEnabled] = false;
			if (!DatabaseExecuteWrite(
				va_return("UPDATE Players SET TipsEnabled = 0 WHERE Name = '%q'", Player[playerid][Name]),
				"restore player tips setting after timer failure"))
				printf("[AttDef] Could not restore TipsEnabled for player %d after the timer failed.", playerid);
			return SendErrorMessage(playerid, "Could not start tips. The setting was reverted.");
		}
		SendClientMessage(playerid, -1, "{BD93DC}\xBB (Tips) {FFFFFF}Tips Messages {AADC93}Enabled.");
	}
	else
	{
		SendClientMessage(playerid, -1, "{BD93DC}\xBB (Tips) {FFFFFF}Tips Messages {EB8989}Disabled.");
		StopTipsTimers(playerid);
	}
	return 1;
}
