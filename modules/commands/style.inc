#if defined _attdef_cmd_style
	#endinput
#endif
#define _attdef_cmd_style

YCMD: style(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "an option to switch the style of round textdraws on your screen");
	if (AllVsAllMode)
		return SendErrorMessage(playerid, "You cannot change textdraw style during All-vs-All mode.");

	new string[300];
	format(
		string, sizeof(string), "%s\n%s\n%s\n%s",
		(Player[playerid][RoundTDStyle] == 0) ? ("{AADC93}Modern (Active)") : ("{FFFFFF}Modern"),
		(Player[playerid][RoundTDStyle] == 2) ? ("{AADC93}Simple (Good For FPS) (Active)") : ("{FFFFFF}Simple (Good For FPS)"),
		(Player[playerid][RoundTDStyle] == 3) ? ("{AADC93}Team Status Icons (Active)") : ("{FFFFFF}Team Status Icons"),
		(Player[playerid][RoundTDStyle] == 4) ? ("{AADC93}Team HP Bars (Active)") : ("{FFFFFF}Team HP Bars"));
	return ShowPlayerDialog(playerid, DIALOG_ROUND_TD_STYLE, DIALOG_STYLE_LIST, "Select round textdraws style", string, "Select", "Back");
}
