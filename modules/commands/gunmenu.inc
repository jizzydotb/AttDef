#if defined _attdef_cmd_gunmenu
	#endinput
#endif
#define _attdef_cmd_gunmenu

YCMD: gunmenu(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "display the weapon menu.");
	if (Current == -1)
		return SendErrorMessage(playerid, "Round is not active.");
	if (!Player[playerid][Playing])
		return SendErrorMessage(playerid, "You are not playing.");
	if (RCArena)
		return SendErrorMessage(playerid, "You cannot get gunmenu in RC arenas");
	if (ElapsedTime <= gGunmenuTime && Player[playerid][Team] != REFEREE)
	{
		ShowPlayerGunmenu(playerid, 0);

		return va_SendClientMessageToAll(-1, "\xBB " COL_PRIM "Gunmenu: " COL_SEC "%s " COL_PRIM "is choosing weapons from the gunmenu.", Player[playerid][Name]);
	}
	return SendErrorMessage(playerid, "Too late to show yourself weapon menu.");
}
