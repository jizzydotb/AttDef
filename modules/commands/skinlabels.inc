#if defined _attdef_cmd_skinlabels
	#endinput
#endif
#define _attdef_cmd_skinlabels

YCMD: skinlabels(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Skin labels display options.");

	new dialogText[384];
	format(dialogText, sizeof(dialogText),
		"Type\tOption\tSample\n{FFFFFF}Packet\tText And Value\t%sPacket: {FFFFFF}0.0%%\nPacket\tValue Only\t\t{66FF66}0.0%%\nPacket\tNone\t\t\t\nPing\tText And Value\t%sPing: {FFFFFF}60\nPing\tValue Only\t\t{66FF66}60\nPing\tNone\t\t\t\nFPS\tText And Value\t%sFPS: {FFFFFF}101\nFPS\tValue Only\t\t{66FF66}101\nFPS\tNone\t\t\t\n",
		TextColor[Player[playerid][Team]], TextColor[Player[playerid][Team]], TextColor[Player[playerid][Team]]);

	return ShowPlayerDialog(playerid, DIALOG_SETLABEL, DIALOG_STYLE_TABLIST_HEADERS, "Skin Labels", dialogText, "Select", "Cancel");
}

YCMD: skinslabels(playerid, params[], help) = skinlabels;
