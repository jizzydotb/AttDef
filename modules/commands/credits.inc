#if defined _attdef_cmd_credits
	#endinput
#endif
#define _attdef_cmd_credits

YCMD: credits(playerid, params[], help)
{
	#pragma unused params
	if (help)
		return SendCommandHelpMessage(playerid, "view gamemode authors and credits.");

	new str[700];
	strcat(str, "{FFFFFF}Attack-Defend Original Creators: " COL_PRIM "062_, Whitetiger, Khalid & Beijind\n");
	strcat(str, "{FFFFFF}UGMP Bases, Arenas & Maps: " COL_PRIM "Volk, Chillax, Excision, cYs & NTL\n");
	strcat(str, "{FFFFFF}open.mp Port & Baseline Modernization (v1.38): " COL_PRIM "ZantetsukenGT\n");
	strcat(str, "{FFFFFF}Full Modular Architecture, Ping System, New HUD Styles, Discord Bot & v1.42: " COL_PRIM "jizzy b\n\n");
	strcat(str, "{CCCCCC}Special thanks to the open.mp and SA-MP competitive communities.");

	return ShowPlayerDialog(playerid, DIALOG_NO_RESPONSE, DIALOG_STYLE_MSGBOX, COL_PRIM "Gamemode Credits", str, "Close", "");
}
