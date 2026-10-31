#if defined _attdef_cmd_skin
	#endinput
#endif
#define _attdef_cmd_skin

#define MAX_SKINS 300

new g_PlayerPreviousSkin[MAX_PLAYERS + 1];

forward SetPlayerSkinWithPrevious(playerid, skinid);
public SetPlayerSkinWithPrevious(playerid, skinid)
{
	new prevSkinid = GetPlayerSkin(playerid);
	g_PlayerPreviousSkin[playerid] = prevSkinid;
	SetPlayerSkin(playerid, skinid);
	ShowSkinChangeMessage(playerid, skinid, prevSkinid);
	return 1;
}

forward ShowSkinChangeMessage(playerid, skinid, prevSkinid);
public ShowSkinChangeMessage(playerid, skinid, prevSkinid)
{
	return va_SendClientMessage(playerid, -1, "\xBB You changed your skin id to: ID %d (Previous skin ID: %d)", skinid, prevSkinid);
}

forward SavePlayerSkinToDB(playerid, skinid);
public SavePlayerSkinToDB(playerid, skinid)
{
	return DatabaseExecuteWrite(
		va_return("UPDATE Players SET pSkin = %d WHERE Name = '%q'", skinid, Player[playerid][Name]),
		"save player skin");
}

YCMD: skin(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "change your skin.");
	if (Player[playerid][Playing])
		return SendErrorMessage(playerid, "Can't use this command while playing.");

	if (isnull(params))
		return SendUsageMessage(playerid, "/skin [id]");

	new skinid = strval(params);
	if (skinid < 0 || skinid >= MAX_SKINS)
		return SendErrorMessage(playerid, "Invalid skin ID.");

	if (skinid == 74)
		return SendErrorMessage(playerid, "Skin ID 74 is invalid. Please choose another skin.");

	if (Player[playerid][pSkin] == skinid)
	{
		if (Player[playerid][InDM] || Player[playerid][InDuel])
			return va_SendClientMessage(playerid, -1, "\xBB You already have the skin ID: %d.", skinid);
		else
			return va_SendClientMessage(playerid, -1, "\xBB You already have the skin ID: %d. Use /dm or /duel.", skinid);
	}

	if (!SavePlayerSkinToDB(playerid, skinid))
		return SendErrorMessage(playerid, "Could not save your skin. No changes were applied.");
	Player[playerid][pSkin] = skinid;

	if (Player[playerid][Spectating] && (Player[playerid][SpectatingType] == BASE || Player[playerid][SpectatingType] == ARENA))
	{
		va_SendClientMessage(playerid, -1, "\xBB Skin ID %d saved. Will be applied when you enter /dm or /duel.", skinid);
	}
	else if (Player[playerid][InDM] || Player[playerid][InDuel])
	{
		SetPlayerSkinWithPrevious(playerid, skinid);
	}
	else
	{
		va_SendClientMessage(playerid, -1, "\xBB Skin ID %d saved. Will be applied when you enter /dm or /duel.", skinid);
	}

	return 1;
}
