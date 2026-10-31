#if defined _attdef_common_teams
	#endinput
#endif
#define _attdef_common_teams

/*
	Team Management, Auto-Balancing & Tag Assignment Sub-module
*/

SwapTeams()
{
	new MyVehicle = -1;
	new Seat;
	new Iterator: ToFixSwitch<MAX_PLAYERS>;
	foreach (new i : Player)
	{
		if (Player[i][Team] == ATTACKER)
			Player[i][Team] = DEFENDER;
		else if (Player[i][Team] == ATTACKER_SUB)
			Player[i][Team] = DEFENDER_SUB;
		else if (Player[i][Team] == DEFENDER)
			Player[i][Team] = ATTACKER;
		else if (Player[i][Team] == DEFENDER_SUB)
			Player[i][Team] = ATTACKER_SUB;

		MyVehicle = GetPlayerVehicleID(i);
		if (MyVehicle)
		{
			Seat = GetPlayerVehicleSeat(i);
			PutPlayerInVehicle(i, MyVehicle, Seat);
		}
		Iter_Add(ToFixSwitch, i);
	}

	for (new i = 0; i < SAVE_SLOTS; ++i)
	{
		if (SaveVariables[i][pName][0])
		{
			if (SaveVariables[i][pTeam] == ATTACKER)
				SaveVariables[i][pTeam] = DEFENDER;
			else if (SaveVariables[i][pTeam] == DEFENDER)
				SaveVariables[i][pTeam] = ATTACKER;
		}
	}

	new TempScore		= TeamScore[ATTACKER];
	TeamScore[ATTACKER] = TeamScore[DEFENDER];
	TeamScore[DEFENDER] = TempScore;

	new bool: TempTacPause		  = TeamTacPauseUsed[ATTACKER];
	TeamTacPauseUsed[ATTACKER]	  = TeamTacPauseUsed[DEFENDER];
	TeamTacPauseUsed[DEFENDER]	  = TempTacPause;

	new TempName[24];
	TempName			   = TeamName[ATTACKER];
	TeamName[ATTACKER]	   = TeamName[DEFENDER];
	TeamName[DEFENDER]	   = TempName;
	TempName			   = TeamName[ATTACKER_SUB];
	TeamName[ATTACKER_SUB] = TeamName[DEFENDER_SUB];
	TeamName[DEFENDER_SUB] = TempName;

	UpdateTeamScoreTextDraw();
	UpdateTeamNameTextDraw();

	// Fix player teams after switching
	foreach (new i : ToFixSwitch)
	{
		SwitchTeamFix(i, false, false);
	}
	Iter_Clear(ToFixSwitch);

	va_SendClientMessageToAll(-1, "\xBB Teams are swapped! - %sAttackers {FFFFFF}%s %sDefenders: {FFFFFF}%s", TextColor[ATTACKER], TeamName[ATTACKER], TextColor[DEFENDER], TeamName[DEFENDER]);
	FixVsTextDraw();
	return 1;
}

forward DoAutoBalance(generation);
public DoAutoBalance(generation)
{
	if (generation != RoundEndGeneration || Current != -1 || WarModeEnding || WarMode || !AutoBal)
		return 0;

	BalanceTeams();
	if (!PreMatchResultsShowing)
		AllowStartBase = true;
	SendClientMessageToAll(-1, "\xBB Teams have been auto-balanced.");
	return 1;
}

forward DontAutoBalance(generation);
public DontAutoBalance(generation)
{
	if (generation != RoundEndGeneration || Current != -1 || WarModeEnding || WarMode || AutoBal)
		return 0;

	if (!PreMatchResultsShowing)
		AllowStartBase = true;
	return 1;
}

BalanceTeams()
{
	// Collect eligible players, shuffle them and deal them alternately so the
	// teams always end up even (difference of at most one player).
	new players[MAX_PLAYERS], count = 0;
	foreach (new i : Player)
	{
		if (Player[i][Spawned] && !Player[i][InDuel] && (Player[i][Team] == ATTACKER || Player[i][Team] == DEFENDER))
			players[count++] = i;
	}

	for (new i = count - 1; i > 0; i--)
	{
		new j = random(i + 1), tmp = players[i];
		players[i] = players[j];
		players[j] = tmp;
	}

	new firstTeam = random(2) == 0 ? ATTACKER : DEFENDER;
	new secondTeam = firstTeam == ATTACKER ? DEFENDER : ATTACKER;
	for (new i = 0; i < count; i++)
	{
		new playerid = players[i];
		Player[playerid][Team] = (i % 2 == 0) ? firstTeam : secondTeam;
		SwitchTeamFix(playerid, false, false);
	}
	FixVsTextDraw();
	return 1;
}

// Used when match mode is enabled: players whose name contains a team tag are
// moved to that team; everybody else keeps the team they are already in.
AssignMatchTeamsByTag()
{
	new vehicleid, seat, newTeam;
	foreach (new i : Player)
	{
		if (Player[i][InDuel] || Player[i][IsAFK] || !Player[i][Spawned])
			continue;

		new team = Player[i][Team];
		if (team != ATTACKER && team != DEFENDER && team != ATTACKER_SUB && team != DEFENDER_SUB)
			continue; // referees / players without team are left untouched

		new bool: tagA = strfind(Player[i][Name], TeamName[ATTACKER], true) != -1;
		new bool: tagB = strfind(Player[i][Name], TeamName[DEFENDER], true) != -1;
		if (tagA == tagB)
			continue; // no tag (or ambiguous): keep current team

		new bool: isSub = team == ATTACKER_SUB || team == DEFENDER_SUB;
		if (tagA)
			newTeam = isSub ? ATTACKER_SUB : ATTACKER;
		else
			newTeam = isSub ? DEFENDER_SUB : DEFENDER;

		if (newTeam == team)
			continue;

		vehicleid = GetPlayerVehicleID(i);
		if (vehicleid)
			seat = GetPlayerVehicleSeat(i);

		Player[i][Team] = newTeam;
		SwitchTeamFix(i, false, false);

		if (vehicleid)
			PutPlayerInVehicle(i, vehicleid, seat);
	}
	FixVsTextDraw();
	return 1;
}

SwitchTeamFix(playerid, bool: message = true, bool: fixvs = true, bool: changeskin = true)
{	
	if (Player[playerid][Playing])
		Player[playerid][WasInTeam] = Player[playerid][Team];

	if (Player[playerid][Team] == ATTACKER)
	{
		format(teamRole, sizeof(teamRole), "(Attacker)");
	}
	if (Player[playerid][Team] == ATTACKER_SUB)
	{
		format(teamRole, sizeof(teamRole), "(Attacker Sub)");
	}		
	if (Player[playerid][Team] == DEFENDER)
	{
		format(teamRole, sizeof(teamRole), "(Defender)");
	}
	if (Player[playerid][Team] == DEFENDER_SUB)
	{
		format(teamRole, sizeof(teamRole), "(Defender Sub)");
	}
	if (Player[playerid][Team] == REFEREE)
	{
		format(teamRole, sizeof(teamRole), "(Spectator)");
	}

	if (message)
	{
		va_SendClientMessageToAll(-1, "%s\xBB %s has switched from %s to %s %s", TextColor[Player[playerid][Team]], Player[playerid][Name], TeamName[oldTeamSwitch], TeamName[Player[playerid][Team]], teamRole);
	}

	if (HalfDeagle && Current == -1)
	{
		AdjustDeagleLimit();
	}

	UpdatePlayerHealthBar(playerid);
	UpdateRoundKillDmgTDmg(playerid);
	ColorFix(playerid);

	if (fixvs && Current == -1)
		FixVsTextDraw();

	if (Current != -1)
	{
		UpdateAttackersZoneVisibility(playerid);
		if (GameType == BASE)
			UpdateCPZoneVisibility(playerid);
	}

	if (changeskin)
	{
		if (Player[playerid][InDM] || Player[playerid][InDuel] || Player[playerid][Spectating])
		{
			// Player is in DM, Duel, or spectating
			if (Player[playerid][pSkin] != -1)
			{
				SetPlayerSkin(playerid, Player[playerid][pSkin]); // Apply pSkin if it's not -1
			}
			else
			{
				SetPlayerSkin(playerid, Skin[Player[playerid][Team]]); // Apply team skin
			}
		}
		else
		{
			// Player is not in DM, Duel, or spectating
			new teamSkin;

			// Determine the skin to use based on the player's current team
			switch (Player[playerid][Team])
			{
				case ATTACKER:
				{
					teamSkin = Skin[ATTACKER];
				}
				case DEFENDER:
				{
					teamSkin = Skin[DEFENDER];
				}
				case REFEREE:
				{
					teamSkin = Skin[REFEREE];
				}
				case ATTACKER_SUB:
				{
					teamSkin = Skin[ATTACKER_SUB];
				}
				case DEFENDER_SUB:
				{
					teamSkin = Skin[DEFENDER_SUB];
				}
			}

			// Apply the determined skin
			if (teamSkin != -1)
			{
				SetPlayerSkin(playerid, teamSkin);
			}
		}
	}

	SetCameraBehindPlayer(playerid);
	HideAllTeamBarsForPlayer(playerid);
	if (Current != -1 && Player[playerid][Playing])
		UpdateRoundTeamStatusTextDraws();
}

GetTeamWithLessPlayers()
{
	new attacker, defender;
	switch (Current)
	{
		case -1:
		{
			foreach (new i : Player)
			{
				switch (Player[i][Team])
				{
					case DEFENDER: defender++;
					case ATTACKER: attacker++;
				}
			}
		}
		default:
		{
			foreach (new i : PlayersInRound)
			{
				switch (Player[i][Team])
				{
					case DEFENDER: defender++;
					case ATTACKER: attacker++;
				}
			}
		}
	}
	return defender <= attacker ? DEFENDER : ATTACKER;
}


