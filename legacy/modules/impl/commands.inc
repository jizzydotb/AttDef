#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_commands
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_commands
	#endinput
#endif
#define _attdef_impl_commands

/*
	Commands
*/

// Set alternative names for commands (todo: replace with a custom system)
SetAltCommands()
{
	Command_AddAltNamed("changelog", "updates");
	Command_AddAltNamed("vr", "fix");
	Command_AddAltNamed("serverstats", "sstats");
	Command_AddAltNamed("serverpassword", "sp");
	Command_AddAltNamed("getpara", "gp");
	Command_AddAltNamed("para", "rp");
	Command_AddAltNamed("blockpm", "nopm");
	Command_AddAltNamed("blockpmall", "nopmall");
	Command_AddAltNamed("v", "car");
	Command_AddAltNamed("sync", "s");
	Command_AddAltNamed("weather", "w");
	Command_AddAltNamed("time", "t");
	Command_AddAltNamed("remgun", "rg");
	Command_AddAltNamed("matchhistory", "lastmatches");
	Command_AddAltNamed("tacpause", "timeout");
	Command_AddAltNamed("autopause", "dcmode");
	Command_AddAltNamed("dmg", "damageinformer");
	Command_AddAltNamed("dmg", "floatingdamage");
}

// Create a group for each level
CreateAdminLevelGroups()
{
	new str[8];
	for (new i = 0; i < MAX_ADMIN_LEVELS; i++) // loop through all admin levels
	{
		format(str, sizeof(str), "Level_%d", i); // initialize a good name
		AdminGroups[i] = Group_Create(str);		 // create a group for this level
	}
}

// Update 'CommandsLevel' array with the new level that 'commandid' has got
// This must be used when a command's level gets changed
UpdateCommandsIteratorArray(commandid, level)
{
	Iter_Clear(CommandsLevel[commandid]);	   // empty it because a command array should only have one value ,this command has a valid level now
	Iter_Add(CommandsLevel[commandid], level); // set a new level for this command
}

// Set player to the proper admin group based on his admin level
// This must be used when a player's admin level gets changed
UpdatePlayerAdminGroup(playerid)
{
	// loop through all admin groups and remove player from them (if he's in any)
	for (new i = 0; i < MAX_ADMIN_LEVELS; i++)
	{
		if (Group_GetPlayer(AdminGroups[i], playerid))		  // if he's in this group
			Group_SetPlayer(AdminGroups[i], playerid, false); // remove him, his admin level changed so he shouldn't be in this admin group anymore
	}
	new newlevel = Player[playerid][Level];
	Group_SetPlayer(AdminGroups[newlevel], playerid, true); // add to the proper group based on his new level
}

// Return the level of 'commandid'
// This function returns -2 if the iterator array is empty
// This function returns -1 if the command was defined more than once with different admin level
GetCommandLevel(commandid)
{
	new count = Iter_Count(CommandsLevel[commandid]);

	// array should only have ONE value because we always clear it before adding a new level (see UpdateCommandsIteratorArray)
	if (!count) // empty iterator array
	{
		printf("DEBUG: Error: GetCommandLevel(commandid:%d) ~ CMD:%s: Returning -2, empty iterator array", commandid, Command_GetName(commandid));
		return -2;
	}
	if (count > 1) // found more than one value (level) for a command
	{
		printf("DEBUG: Error: GetCommandLevel(commandid:%d) ~ CMD:%s: Returning -1, found more than a value. Command has different levels?", commandid, Command_GetName(commandid));
		return -1;
	}
	return Iter_First(CommandsLevel[commandid]);
}

// Set the level of a command
// 'updatedb' should be disabled when loading commands from database (on startup?)
// otherwise it's good to enable it to update the newly set level in the database too
SetCommandLevel(const command[], level, bool: updatedb)
{
	new id = Command_GetID(command); // get the id of our command
	if (id < 0)
		return 0;	// invalid command id, they start from 0.
	if (level < 0 || level >= MAX_ADMIN_LEVELS)
		return 0;
	if (updatedb && !DatabaseExecuteWrite(
		va_return("UPDATE Commands SET Level = %d WHERE Command = '%q'", level, command),
		"save command access level"))
		return 0;

	if (level == 0) // if this command is to be set for normal users (level 0)
	{
		// then no need for much work just make it globally available
		Group_SetGlobalCommand(id, ALLOW);
	}
	else
	{
		// we initialize everything by disabling this command for everyone
		Group_SetGlobalCommand(id, UNDEF);
		for (new i = 0; i < MAX_ADMIN_LEVELS; i++) // loop through all admin levels
		{
			// Enable this command for the given level or any level that is higher than it and disable it for lower levels.
			Group_SetCommand(AdminGroups[i], id, i >= level ? ALLOW : UNDEF);
		}
	}
	UpdateCommandsIteratorArray(id, level); // Update the iterator array that contains all commands ID and level
	return 1;
}

// This is a core function that configures a command
// It loads a command level from the database if it exists.
// If a command doesn't exist, it gets inserted into the database
CreateCommandConfig(const cmd[], level)
{
	new DBResult: result = DB_ExecuteQuery(sqliteconnection, va_return("SELECT Level FROM Commands WHERE Command='%q'", cmd));
	if (!result)
	{
		printf("[AttDef] Failed to load the access level for command /%s.", cmd);
		return 0;
	}

	if (DB_GetRowCount(result))
	{
		new commandLevel = DB_GetFieldIntByName(result, "Level");
		if (!DB_FreeResultSet(result))
		{
			printf("[AttDef] Failed to release the command access-level query result for /%s.", cmd);
			return 0;
		}
		if (!SetCommandLevel(cmd, commandLevel, false))
		{
			printf("[AttDef] Invalid stored access level %d for command /%s.", commandLevel, cmd);
			return 0;
		}
		return 1;
	}

	if (!DB_FreeResultSet(result))
	{
		printf("[AttDef] Failed to release the command access-level query result for /%s.", cmd);
		return 0;
	}

	if (!DatabaseExecuteWrite(
		va_return("INSERT INTO Commands (Command, Level) VALUES ('%q', %d)", cmd, level),
		"insert command access level"))
		return 0;

	if (!SetCommandLevel(cmd, level, false))
	{
		printf("[AttDef] Could not apply the default access level for command /%s.", cmd);
		return 0;
	}
	return 1;
}

// Initialize and load commands
LoadCommands()
{
	new cmdCount, failedCount; // number of registered commands and failed configuration loads
	CreateAdminLevelGroups();
	Iter_Init(CommandsLevel);
	foreach (new i : Command())
	{
		cmdCount++; // increase count
		// Level parameter is ignored if the command already exists in the database
		if (!CreateCommandConfig(Command_GetName(i), 0))
		{
			failedCount++;
			printf("[AttDef] Could not configure command /%s.", Command_GetName(i));
		}
	}

	// Ensure command aliases inherit their parent command's access level if not explicitly raised
	new const altPairs[][2][24] = {
		{ "changelog", "updates" },
		{ "vr", "fix" },
		{ "serverstats", "sstats" },
		{ "serverpassword", "sp" },
		{ "getpara", "gp" },
		{ "para", "rp" },
		{ "blockpm", "nopm" },
		{ "blockpmall", "nopmall" },
		{ "v", "car" },
		{ "sync", "s" },
		{ "weather", "w" },
		{ "time", "t" },
		{ "remgun", "rg" },
		{ "matchhistory", "lastmatches" },
		{ "tacpause", "timeout" },
		{ "autopause", "dcmode" },
		{ "dmg", "damageinformer" },
		{ "dmg", "floatingdamage" },
		{ "skinlabels", "skinslabels" }
	};
	for (new idx = 0; idx < sizeof(altPairs); idx++)
	{
		new parentId = Command_GetID(altPairs[idx][0]);
		new altId = Command_GetID(altPairs[idx][1]);
		if (parentId >= 0 && altId >= 0)
		{
			new parentLvl = GetCommandLevel(parentId);
			new altLvl = GetCommandLevel(altId);
			if (parentLvl >= 0 && altLvl != parentLvl)
			{
				SetCommandLevel(altPairs[idx][1], parentLvl, true);
			}
		}
	}

	printf("Commands Loaded: %d (%d failed)", cmdCount, failedCount);
	return failedCount == 0;
}
