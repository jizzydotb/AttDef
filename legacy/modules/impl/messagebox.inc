#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_messagebox
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_messagebox
	#endinput
#endif
#define _attdef_impl_messagebox

/*
	MessageBox Script (Migrated to Chat Messages)
*/

stock CreateMessageBoxTextDraws(playerid)
{
	#pragma unused playerid
	return 1;
}

stock InitPlayerMessageBox(playerid)
{
	#pragma unused playerid
	return 1;
}

forward HideMessageBox(playerid, MessageBoxType: type);
public HideMessageBox(playerid, MessageBoxType: type)
{
	#pragma unused playerid, type
	return 1;
}

forward HideMessageBoxTimed(playerid, MessageBoxType: type, generation, timerGeneration);
public HideMessageBoxTimed(playerid, MessageBoxType: type, generation, timerGeneration)
{
	#pragma unused playerid, type, generation, timerGeneration
	return 1;
}

stock CleanMessageBoxText(const input[], output[], maxlen = sizeof(output))
{
	new outIdx = 0, inIdx = 0, len = strlen(input);
	while (inIdx < len && outIdx < maxlen - 1)
	{
		if (input[inIdx] == '~')
		{
			if (inIdx + 2 < len && input[inIdx + 1] == 'n' && input[inIdx + 2] == '~')
			{
				output[outIdx++] = ' ';
				inIdx += 3;
				continue;
			}
			new endTilde = -1;
			for (new j = inIdx + 1; j < len && j <= inIdx + 6; j++)
			{
				if (input[j] == '~')
				{
					endTilde = j;
					break;
				}
			}
			if (endTilde != -1)
			{
				inIdx = endTilde + 1;
				continue;
			}
		}
		output[outIdx++] = input[inIdx++];
	}
	output[outIdx] = EOS;
}

MessageBox(playerid, MessageBoxType: type, const caption[], const info[], interval = 6000, bool: sound = true)
{
	#pragma unused type, interval
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;
	if (sound)
		PlayerPlaySound(playerid, 1053, 0.0, 0.0, 0.0);

	new cleanCaption[128], cleanInfo[256];
	CleanMessageBoxText(caption, cleanCaption);
	CleanMessageBoxText(info, cleanInfo);

	if (cleanInfo[0] == EOS || !strcmp(cleanInfo, "_"))
	{
		va_SendClientMessage(playerid, -1, "" COL_PRIM "[%s]", cleanCaption);
	}
	else
	{
		va_SendClientMessage(playerid, -1, "" COL_PRIM "[%s] " COL_SEC "%s", cleanCaption, cleanInfo);
	}
	return 1;
}
