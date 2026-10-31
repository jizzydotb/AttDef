#if defined _attdef_OnPModelSelection
	#endinput
#endif
#define _attdef_OnPModelSelection

public OnPlayerModelSelection(playerid, response, listid, modelid)
{
	if (listid == teamskinlist)
	{
		if (response && ChangingSkinOfTeam[playerid] != -1)
		{
			new team = ChangingSkinOfTeam[playerid];
			if (!PersistTeamSkin(team, modelid))
			{
				ChangingSkinOfTeam[playerid] = -1;
				return SendErrorMessage(playerid, "Could not save the team skin. No changes were applied.");
			}

			foreach (new i : Player)
			{
				if (Player[i][Team] == team)
				{
					SetPlayerSkin(i, modelid);
				}
			}

			new iString[128];
			format(iString, sizeof(iString), "{FFFFFF}%s " COL_PRIM "has changed {FFFFFF}%s " COL_PRIM "skin to: {FFFFFF}%d", Player[playerid][Name], TeamName[team], modelid);
			SendClientMessageToAll(-1, iString);
			ChangingSkinOfTeam[playerid] = -1;
		}
		else
		{
			ChangingSkinOfTeam[playerid] = -1;
			SendClientMessage(playerid, -1, "\xBB Canceled team skin selection");
		}
	}
	else if (listid == playerskinlist)
	{
		if (response && !Player[playerid][Playing] && !Player[playerid][Spectating])
		{
			SetPlayerSkin(playerid, modelid);
		}
	}
	return 1;
}
