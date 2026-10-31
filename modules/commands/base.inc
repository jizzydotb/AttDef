#if defined _attdef_cmd_base
	#endinput
#endif
#define _attdef_cmd_base

YCMD: base(playerid, params[], help)
{
	//if(Player[playerid][Level] < 5 && !IsPlayerAdmin(playerid)) return SendErrorMessage(playerid,"You need to be level 5 or rcon admin.");
	if (help)
		return SendCommandHelpMessage(playerid, "create a new base.");
	if (Current != -1)
		return SendErrorMessage(playerid, "Can't use this command while round is active.");

	new baseID, CommandID, cmdType[7], BaseName[96 + 1];
	if (sscanf(params, "s[*]I(-1)S( )[*]", sizeof(cmdType), cmdType, baseID, sizeof(BaseName), BaseName))
		return SendUsageMessage(playerid, "/base [create | att | def | cp | name | delete]");

	new usageMessage[28], updateQuery[144], finishedMessage[21];
	switch (YHash(cmdType, false))
	{
		case _I<create>: CommandID = 1;
		case _I<att>:
		{
			strcat(usageMessage, "/base att [Base ID]");
			strcat(updateQuery, "AttSpawn = '%q'");
			strcat(finishedMessage, "Attacker position");
			CommandID = 2;
		}
		case _I<def>:
		{
			strcat(usageMessage, "/base def [Base ID]");
			strcat(updateQuery, "DefSpawn = '%q'");
			strcat(finishedMessage, "Defender position");
			CommandID = 3;
		}
		case _I<cp>:
		{
			strcat(usageMessage, "/base cp [Base ID]");
			strcat(updateQuery, "CPSpawn = '%q', Interior = %d");
			strcat(finishedMessage, "CP/Interior position");
			CommandID = 4;
		}
		case _I<name>:
		{
			strcat(usageMessage, "/base name [Base ID] [Name]");
			strcat(updateQuery, "Name = '%q'");
			strcat(finishedMessage, "base Name");
			CommandID = 5;
		}
		case _I<delete>:
		{
			strcat(usageMessage, "/base delete [Base ID]");
			CommandID = 6;
		}
		default: return SendUsageMessage(playerid, "/base [create | att | def | cp | name | delete]");
	}

	switch (CommandID)
	{
		case 1:
		{
			if (Iter_Count(CurrentBases) == MAX_BASES)
				return SendErrorMessage(playerid, "Too many bases already created. You can use /loadbases to create this base in another set.");



			new BaseID = Iter_Alloc(CurrentBases);
			if (BaseID < 0 || BaseID >= MAX_BASES)
				return SendErrorMessage(playerid, "Could not find a free base ID.");

			if (!DatabaseExecuteWrite(
				va_return(
					"INSERT INTO `%q` (ID, AttSpawn, DefSpawn, CPSpawn, Interior, Name) VALUES (%d, '0,0,0', '0,0,0', '0,0,0', 0, 'No Name')",
					CurrentBaseSet, BaseID),
				"create base"))
			{
				Iter_Remove(CurrentBases, BaseID);
				return SendErrorMessage(playerid, "Could not create the base. No changes were applied.");
			}

			if (!LoadBases())
			{
				Iter_Remove(CurrentBases, BaseID);
				if (!DatabaseExecuteWrite(
					va_return("DELETE FROM `%q` WHERE ID = %d", CurrentBaseSet, BaseID),
					"remove base after failed reload"))
				{
					printf("[AttDef] Could not remove newly created base %d after its pack failed to reload.", BaseID);
				}
				else if (!LoadBases())
				{
					printf("[AttDef] Base pack '%s' still failed to reload after removing base %d.", CurrentBaseSet, BaseID);
				}
				return SendErrorMessage(playerid, "The base pack could not be reloaded; the new base was rolled back.");
			}

			return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has created base ID: "COL_SEC"%d " COL_PRIM "| in base set: "COL_SEC"%s", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], BaseID,
				CurrentBaseSet);
		}
	}

	if (baseID < 0 || CommandID == 5 && isnull(BaseName))
		return SendUsageMessage(playerid, usageMessage);
	if (!Iter_Contains(CurrentBases, baseID))
		return SendErrorMessage(playerid, "That base doesn't exist.");



	// the queries here are already escaped with %q, don't panic if you see a %s
	switch (CommandID)
	{
		case 5:
		{
			if (!DatabaseExecuteWrite(
				va_return("UPDATE `%q` SET %s WHERE ID = %d", CurrentBaseSet, va_return(updateQuery, BaseName), baseID),
				"update base name"))
				return SendErrorMessage(playerid, "Could not update the base name. No changes were applied.");
		}
		case 6:
		{
			if (Iter_Count(CurrentBases) <= 1)
				return SendErrorMessage(playerid, "Cannot delete the last base in the active base pack.");

			if (!DatabaseExecuteWrite(
				va_return("DELETE FROM `%q` WHERE ID = %d", CurrentBaseSet, baseID),
				"delete base"))
				return SendErrorMessage(playerid, "Could not delete the base. No changes were applied.");

			if (!LoadBases())
				return SendErrorMessage(playerid, "The base was deleted, but the active pack could not be reloaded. Check the server log.");

			return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has deleted Base ID: "COL_SEC"%d from base set: "COL_SEC"%s", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], baseID, CurrentBaseSet);
		}
		default:
		{
			new Float: x,
				Float: y,
				Float: z,
				CoordenatesXYZ[64];
			GetPlayerPos(playerid, x, y, z);
			format(CoordenatesXYZ, sizeof(CoordenatesXYZ), "%f,%f,%f", x, y, z);

			if (CommandID == 4)
			{
				if (!DatabaseExecuteWrite(
					va_return("UPDATE `%q` SET %s WHERE ID = %d", CurrentBaseSet, va_return(updateQuery, CoordenatesXYZ, GetPlayerInterior(playerid)), baseID),
					"update base checkpoint"))
					return SendErrorMessage(playerid, "Could not update the base checkpoint. No changes were applied.");
			}
			else
			{
				if (!DatabaseExecuteWrite(
					va_return("UPDATE `%q` SET %s WHERE ID = %d", CurrentBaseSet, va_return(updateQuery, CoordenatesXYZ), baseID),
					"update base spawn"))
					return SendErrorMessage(playerid, "Could not update the base spawn. No changes were applied.");
			}
		}
	}

	if (!LoadBases())
		return SendErrorMessage(playerid, "The base was updated, but the active pack could not be reloaded. Check the server log.");

	return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has configured the "COL_SEC"%s "COL_PRIM"for Base ID: "COL_SEC"%d", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], finishedMessage, baseID);
}
