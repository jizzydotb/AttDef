#if defined _attdef_cmd_deleteacc
	#endinput
#endif
#define _attdef_cmd_deleteacc

YCMD: deleteacc(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 5)
		return SendErrorMessage(playerid, "You must be level 5 to use this command.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "delete an account from the database");

	new str[MAX_PLAYER_NAME];
	if (isnull(params) || sscanf(params, "s[24]", str))
		return SendUsageMessage(playerid, "/deleteacc [Account Name]");
	if (strlen(str) < 3)
		return SendErrorMessage(playerid, "Minimum name length: 3 characters.");
	if (strlen(str) > 20)
		return SendErrorMessage(playerid, "Maximum name length: 20 characters.");

	new iString[164];
	format(iString, sizeof(iString), "SELECT Level FROM Players WHERE Name = '%q'", str);
	new DBResult: result = DB_ExecuteQuery(sqliteconnection, iString);
	if (!result)
		return SendErrorMessage(playerid, "Could not read the account from the database.");
	new db_rows			 = DB_GetRowCount(result);
	DB_FreeResultSet(result);

	if (db_rows == 0)
		return SendErrorMessage(playerid, va_return("%s" COL_PRIM "'s account wasn't found in the database.", str));

	if (!DatabaseExecuteWrite(
		va_return("DELETE FROM Players WHERE Name = '%q'", str),
		"delete player account"))
		return SendErrorMessage(playerid, "Could not delete the account.");

	return va_SendClientMessage(playerid, -1, ""COL_PRIM"\xBB Success: "COL_SEC"%s" COL_PRIM "'s account was deleted.", str);
}
