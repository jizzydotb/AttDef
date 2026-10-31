#if defined _attdef_cmd_ann
	#endinput
#endif
#define _attdef_cmd_ann

YCMD: ann(playerid, params[], help)
{
	//if(Player[playerid][Level] < 2) return SendErrorMessage(playerid,"You must be a higher level admin to use this command.");
	if (help)
		return SendCommandHelpMessage(playerid, "display a specific announcement to all players");
	new str[128];
	if (sscanf(params, "s[128]", str))
		return SendUsageMessage(playerid, "/ann [Text]");
	if (strfind(str, "`") != -1)
		return SendErrorMessage(playerid, "` is not allowed.");


	if (!IsSafeGametext(str))
	{
		SendErrorMessage(playerid, "You're probably missing a '~' which can crash you and/or other clients!");
		return SendClientMessage(playerid, -1, ""COL_PRIM"\xBB Note: "COL_THR"Always leave a space between a "COL_SEC"'~' and the character "COL_SEC"'K'");
	}

	if (AnnTimer)
	{
		KillTimer(AnnTimer);
		AnnTimer = 0;
	}
	AnnTimer = SetTimer("HideAnnForAll", 5000, false);
	if (!AnnTimer)
	{
		printf("[AttDef] Failed to schedule announcement cleanup.");
		TextDrawSetString(AnnTD, " ");
		TextDrawHideForAll(AnnTD);
		return SendErrorMessage(playerid, "Could not schedule announcement cleanup. Please try again.");
	}

	TextDrawSetString(AnnTD, str);
	TextDrawShowForAll(AnnTD);

	LogAdminCommand("ann", playerid, INVALID_PLAYER_ID);

	format(str, sizeof(str), ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "showed an announcement.", PLAYER_ROLE_NAME(playerid), Player[playerid][Name]);
	return SendClientMessageToAll(-1, str);
}
