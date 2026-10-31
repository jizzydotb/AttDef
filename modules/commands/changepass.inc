#if defined _attdef_cmd_changepass
	#endinput
#endif
#define _attdef_cmd_changepass

forward InlineChangePass(playerid, dialogid, response, listitem, string: inputtext[]);
forward OnChangePassHash(playerid, generation);

YCMD: changepass(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "change your user account password.");
	if (!Player[playerid][Logged])
		return SendErrorMessage(playerid, "You must be logged in.");

	Dialog_ShowCallback(
		playerid, using public InlineChangePass<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Password Change Dialog", "{FFFFFF}Type your new password below:",
		"Modify", "Cancel");
	return 1;
}

public InlineChangePass(playerid, dialogid, response, listitem, string: inputtext[])
{
#pragma unused dialogid, listitem
	if (!response)
		return 1;
	if (isnull(inputtext))
	{
		Dialog_ShowCallback(
			playerid, using public InlineChangePass<iiiis>, DIALOG_STYLE_PASSWORD, "{FFFFFF}Password Change Dialog", "{FFFFFF}Type your new password below:",
			"Modify", "Cancel");
		return 1;
	}

	bcrypt_hash(playerid, "OnChangePassHash", inputtext, BCRYPT_COST, "i", PlayerConnectionGeneration[playerid]);
	return 1;
}

public OnChangePassHash(playerid, generation)
{
	if (!IsPlayerConnected(playerid) || PlayerConnectionGeneration[playerid] != generation || !Player[playerid][Logged])
		return 1;

	new dest[BCRYPT_HASH_LENGTH];
	bcrypt_get_hash(dest);
	if (!DatabaseExecuteWrite(
		va_return("UPDATE Players SET Password = '%q' WHERE Name = '%q'", dest, Player[playerid][Name]),
		"save player password"))
		return SendErrorMessage(playerid, "Could not save your new password.");
	SendClientMessage(playerid, -1, COL_THR "\xBB Your password has been changed.");
	return 1;
}
