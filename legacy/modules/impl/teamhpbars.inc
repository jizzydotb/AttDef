#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_teamhpbars
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_teamhpbars
	#endinput
#endif
#define _attdef_impl_teamhpbars

/*
	Team HP Bars
*/

#include <YSI_Coding\y_hooks>

hook OnPlayerDisconnect(playerid, reason)
{
	DeletePlayerTeamBar(playerid);
	return 1;
}

// Check if a viewer should see the team HP bars
bool: ShouldPlayerSeeTeamBars(playerid)
{
	if (Current == -1)
		return false;
	if (ViewRoundCountdown > 0)
		return false;
	if (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER)
		return false;
	if (Player[playerid][RoundTDStyle] != 4)
		return false;
	if (Player[playerid][InDM] || Player[playerid][InDuel] || Player[playerid][IsAFK])
		return false;
	if (Player[playerid][Playing] || Player[playerid][Spectating])
		return true;
	if (GetPlayerVirtualWorld(playerid) == 2)
		return true;
	return false;
}

// This finds the Y co-ordinate of the next free place for the team HP bar textdraw
stock FindPlayerTeamBarY(playerid, &Float: y)
{
	y = TEAMBARS_BAR_START_Y;
	for (new slot = 0; slot < TEAMBARS_MAX_PER_TEAM; slot++)
	{
		new bool: used = false;
		foreach (new i : PlayerWithTeamBar)
		{
			if (TeamBarsData[i][TeamBar_Team] == Player[playerid][Team] && TeamBarsData[i][TeamBar_Y] == y)
			{
				used = true;
				break;
			}
		}
		if (!used)
			return 1;
		y += TEAMBARS_DIFF_Y;
	}
	return 1;
}

// Gets how many bars created in a specific team
stock GetTeamBarsCount(team)
{
	new ct = 0;
	foreach (new i : PlayerWithTeamBar)
	{
		if (TeamBarsData[i][TeamBar_Team] == team)
			ct++;
	}
	return ct;
}

// This initializes team bars and creates their textdraws for a specific player at given Y
CreatePlayerTeamBarAtY(playerid, Float: y)
{
	if (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER)
		return 0;

	if (Iter_Contains(PlayerWithTeamBar, playerid))
		DeletePlayerTeamBar(playerid);

	new teamToSet = Player[playerid][Team];
	new Float: startX = (teamToSet == ATTACKER) ? TEAMBARS_ATTACKER_START_X : TEAMBARS_DEFENDER_START_X;
	new healthColor = (teamToSet == ATTACKER) ? TEAMBARS_ATTACKER_COLOR : TEAMBARS_DEFENDER_COLOR;

	new hp = GetHP(playerid) + GetAP(playerid);
	if (hp < 0) hp = 0;
	if (hp > 200) hp = 200;
	new Float: filledWidth = (float(hp) / 200.0) * TEAMBARS_BAR_WIDTH;

	// 1: Blackboard (dark background box)
	TeamBarsData[playerid][TeamBarBlackboard] = TextDrawCreate(startX, y, "_");
	TextDrawLetterSize(TeamBarsData[playerid][TeamBarBlackboard], 0.2561, 0.9587);
	TextDrawTextSize(TeamBarsData[playerid][TeamBarBlackboard], startX + TEAMBARS_BAR_WIDTH, 930.000);
	TextDrawAlignment(TeamBarsData[playerid][TeamBarBlackboard], 1);
	TextDrawColor(TeamBarsData[playerid][TeamBarBlackboard], -1);
	TextDrawUseBox(TeamBarsData[playerid][TeamBarBlackboard], 1);
	TextDrawBoxColor(TeamBarsData[playerid][TeamBarBlackboard], TEAMBARS_BLACKBOARD_COLOR);
	TextDrawSetShadow(TeamBarsData[playerid][TeamBarBlackboard], 1);
	TextDrawSetOutline(TeamBarsData[playerid][TeamBarBlackboard], 0);
	TextDrawBackgroundColor(TeamBarsData[playerid][TeamBarBlackboard], 0x000000FF);
	TextDrawFont(TeamBarsData[playerid][TeamBarBlackboard], 1);
	TextDrawSetProportional(TeamBarsData[playerid][TeamBarBlackboard], 1);

	// 2: Healthbar (colored filled box)
	TeamBarsData[playerid][TeamBarHealth] = TextDrawCreate(startX, y, "_");
	TextDrawLetterSize(TeamBarsData[playerid][TeamBarHealth], 0.2561, 0.9587);
	TextDrawTextSize(TeamBarsData[playerid][TeamBarHealth], startX + filledWidth, 930.000);
	TextDrawAlignment(TeamBarsData[playerid][TeamBarHealth], 1);
	TextDrawColor(TeamBarsData[playerid][TeamBarHealth], -1);
	TextDrawUseBox(TeamBarsData[playerid][TeamBarHealth], 1);
	TextDrawBoxColor(TeamBarsData[playerid][TeamBarHealth], healthColor);
	TextDrawSetShadow(TeamBarsData[playerid][TeamBarHealth], 1);
	TextDrawSetOutline(TeamBarsData[playerid][TeamBarHealth], 0);
	TextDrawBackgroundColor(TeamBarsData[playerid][TeamBarHealth], 0x000000FF);
	TextDrawFont(TeamBarsData[playerid][TeamBarHealth], 1);
	TextDrawSetProportional(TeamBarsData[playerid][TeamBarHealth], 1);

	// 3: Name, HP and Damage text
	new textStr[64], nameStr[MAX_PLAYER_NAME];
	if (strlen(Player[playerid][Name]) <= 13)
		format(nameStr, sizeof(nameStr), "%s", Player[playerid][Name]);
	else
		format(nameStr, sizeof(nameStr), "%s", Player[playerid][NameWithoutTag]);

	new roundDmg = floatround(Player[playerid][RoundDamage]);
	format(textStr, sizeof(textStr), "~w~%s: %d - ~r~%d", nameStr, hp, roundDmg);

	// Center text inside the bar using Alignment 1 (Left alignment) to avoid multi-line wrap bugs
	// In SA-MP Font 1 at LetterSize 0.1289, each glyph is ~2.60 units wide
	new visibleChars = strlen(nameStr) + 5; // name + ": " + " - "
	if (hp >= 100) visibleChars += 3;
	else if (hp >= 10) visibleChars += 2;
	else visibleChars += 1;
	if (roundDmg >= 1000) visibleChars += 4;
	else if (roundDmg >= 100) visibleChars += 3;
	else if (roundDmg >= 10) visibleChars += 2;
	else visibleChars += 1;

	new Float: textWidth = float(visibleChars) * 2.60;
	if (textWidth > (TEAMBARS_BAR_WIDTH - 6.0))
		textWidth = TEAMBARS_BAR_WIDTH - 6.0;

	new Float: padding = (TEAMBARS_BAR_WIDTH - textWidth) / 2.0;
	if (padding < 3.0) padding = 3.0;

	new Float: textX = startX + padding;

	TeamBarsData[playerid][TeamBarText] = TextDrawCreate(textX, y + TEAMBARS_TEXT_Y_OFFSET, textStr);
	TextDrawLetterSize(TeamBarsData[playerid][TeamBarText], 0.1289, 0.7374);
	TextDrawTextSize(TeamBarsData[playerid][TeamBarText], 640.0, 930.000);
	TextDrawAlignment(TeamBarsData[playerid][TeamBarText], 1);
	TextDrawColor(TeamBarsData[playerid][TeamBarText], -1);
	TextDrawSetShadow(TeamBarsData[playerid][TeamBarText], 0);
	TextDrawSetOutline(TeamBarsData[playerid][TeamBarText], 0);
	TextDrawBackgroundColor(TeamBarsData[playerid][TeamBarText], 0x000000FF);
	TextDrawFont(TeamBarsData[playerid][TeamBarText], 1);
	TextDrawSetProportional(TeamBarsData[playerid][TeamBarText], 1);
	TeamBarsData[playerid][TeamBar_Team] = teamToSet;
	TeamBarsData[playerid][TeamBar_Y] = y;
	TeamBarsData[playerid][TeamBar_Flashing] = false;
	TeamBarsData[playerid][TeamBar_HealthWidth] = filledWidth;
	Iter_Add(PlayerWithTeamBar, playerid);

	if (Current != -1 && ViewRoundCountdown == 0)
	{
		foreach (new i : Player)
		{
			if (ShouldPlayerSeeTeamBars(i))
			{
				TextDrawShowForPlayer(i, TeamBarsData[playerid][TeamBarBlackboard]);
				if (hp > 0 && Player[playerid][Playing])
					TextDrawShowForPlayer(i, TeamBarsData[playerid][TeamBarHealth]);
				TextDrawShowForPlayer(i, TeamBarsData[playerid][TeamBarText]);
				TeamBarShownForPlayer[i][playerid] = true;
			}
		}
	}
	return 1;
}

// This initializes team bars and creates their textdraws for a specific player
CreatePlayerTeamBar(playerid)
{
	if (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER)
		return 0;

	return ReorganizeTeamBars(Player[playerid][Team]);
}

// Reorganizes bars for a team so dead players are removed and alive players are packed sequentially
ReorganizeTeamBars(team)
{
	if (team != ATTACKER && team != DEFENDER)
		return 0;

	// 1. Remove bars for any player in this team who is not playing or has 0 HP
	foreach (new i : PlayerWithTeamBar)
	{
		if (TeamBarsData[i][TeamBar_Team] == team)
		{
			if (!Player[i][Playing] || (GetHP(i) + GetAP(i) <= 0))
			{
				DeletePlayerTeamBar(i);
			}
		}
	}

	// 2. Collect all alive playing players in this team
	new alivePlayers[MAX_PLAYERS], aliveCount = 0;
	foreach (new i : Player)
	{
		if (Player[i][Playing] && Player[i][Team] == team && (GetHP(i) + GetAP(i) > 0))
		{
			if (aliveCount < TEAMBARS_MAX_PER_TEAM)
			{
				alivePlayers[aliveCount++] = i;
			}
		}
	}

	// 3. For each alive player, ensure their bar is at sequential slot y
	new Float: y = TEAMBARS_BAR_START_Y;
	for (new idx = 0; idx < aliveCount; idx++)
	{
		new pid = alivePlayers[idx];
		if (Iter_Contains(PlayerWithTeamBar, pid))
		{
			if (TeamBarsData[pid][TeamBar_Y] != y)
			{
				new bool: wasFlashing = TeamBarsData[pid][TeamBar_Flashing];
				DeletePlayerTeamBar(pid);
				CreatePlayerTeamBarAtY(pid, y);
				TeamBarsData[pid][TeamBar_Flashing] = wasFlashing;
			}
			else
			{
				UpdatePlayerTeamBar(pid);
			}
		}
		else
		{
			CreatePlayerTeamBarAtY(pid, y);
		}
		y += TEAMBARS_DIFF_Y;
	}
	ShowTeamBarsForAll();
	return 1;
}

// Shows team bars for a specific viewer
ShowTeamBarsForPlayer(playerid)
{
	if (!ShouldPlayerSeeTeamBars(playerid))
	{
		HideAllTeamBarsForPlayer(playerid);
		return 0;
	}

	if (Iter_Count(PlayerWithTeamBar) == 0 && Current != -1)
	{
		foreach (new p : Player)
		{
			if (Player[p][Playing] && (Player[p][Team] == ATTACKER || Player[p][Team] == DEFENDER))
				CreatePlayerTeamBar(p);
		}
	}

	foreach (new i : PlayerWithTeamBar)
	{
		TextDrawShowForPlayer(playerid, TeamBarsData[i][TeamBarBlackboard]);
		new hp = GetHP(i) + GetAP(i);
		if (hp > 0 && Player[i][Playing])
			TextDrawShowForPlayer(playerid, TeamBarsData[i][TeamBarHealth]);
		else
			TextDrawHideForPlayer(playerid, TeamBarsData[i][TeamBarHealth]);
		TextDrawShowForPlayer(playerid, TeamBarsData[i][TeamBarText]);
		TeamBarShownForPlayer[playerid][i] = true;
	}
	return 1;
}

// Shows team bars for all eligible viewers
ShowTeamBarsForAll()
{
	foreach (new i : Player)
	{
		if (ShouldPlayerSeeTeamBars(i))
			ShowTeamBarsForPlayer(i);
		else
			HideAllTeamBarsForPlayer(i);
	}
	return 1;
}

// Hide a specific team bar for a player
TeamBarHideForPlayer(playerid, barofplayerid)
{
	if (!Iter_Contains(PlayerWithTeamBar, barofplayerid))
		return 0;

	TeamBarShownForPlayer[playerid][barofplayerid] = false;
	TextDrawHideForPlayer(playerid, TeamBarsData[barofplayerid][TeamBarBlackboard]);
	TextDrawHideForPlayer(playerid, TeamBarsData[barofplayerid][TeamBarHealth]);
	TextDrawHideForPlayer(playerid, TeamBarsData[barofplayerid][TeamBarText]);
	return 1;
}

// Hides all team bars for a specific player
HideTeamBarsForPlayer(playerid)
{
	foreach (new i : PlayerWithTeamBar)
	{
		TeamBarShownForPlayer[playerid][i] = false;
		TextDrawHideForPlayer(playerid, TeamBarsData[i][TeamBarBlackboard]);
		TextDrawHideForPlayer(playerid, TeamBarsData[i][TeamBarHealth]);
		TextDrawHideForPlayer(playerid, TeamBarsData[i][TeamBarText]);
	}
	return 1;
}

// Hide all team bars for a player with force
HideAllTeamBarsForPlayer(playerid)
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		TeamBarShownForPlayer[playerid][i] = false;
		if (TeamBarsData[i][TeamBarBlackboard] != Text: INVALID_TEXT_DRAW)
			TextDrawHideForPlayer(playerid, TeamBarsData[i][TeamBarBlackboard]);
		if (TeamBarsData[i][TeamBarHealth] != Text: INVALID_TEXT_DRAW)
			TextDrawHideForPlayer(playerid, TeamBarsData[i][TeamBarHealth]);
		if (TeamBarsData[i][TeamBarText] != Text: INVALID_TEXT_DRAW)
			TextDrawHideForPlayer(playerid, TeamBarsData[i][TeamBarText]);
	}
	return 1;
}

// This deletes a player team bar
DeletePlayerTeamBar(playerid)
{
	if (!Iter_Contains(PlayerWithTeamBar, playerid))
		return 0;

	foreach (new i : Player)
	{
		TeamBarShownForPlayer[i][playerid] = false;
	}

	TextDrawDestroy(TeamBarsData[playerid][TeamBarBlackboard]);
	TextDrawDestroy(TeamBarsData[playerid][TeamBarHealth]);
	TextDrawDestroy(TeamBarsData[playerid][TeamBarText]);

	TeamBarsData[playerid][TeamBarBlackboard] = Text: INVALID_TEXT_DRAW;
	TeamBarsData[playerid][TeamBarHealth] = Text: INVALID_TEXT_DRAW;
	TeamBarsData[playerid][TeamBarText] = Text: INVALID_TEXT_DRAW;
	TeamBarsData[playerid][TeamBar_Y] = Float: INVALID_PLAYER_ID;
	TeamBarsData[playerid][TeamBar_Team] = REFEREE;
	TeamBarsData[playerid][TeamBar_Flashing] = false;
	TeamBarsData[playerid][TeamBar_HealthWidth] = 0.0;
	Iter_Remove(PlayerWithTeamBar, playerid);
	return 1;
}

// This deletes all team bars
DeleteAllTeamBars()
{
	foreach (new i : PlayerWithTeamBar)
	{
		TextDrawDestroy(TeamBarsData[i][TeamBarBlackboard]);
		TextDrawDestroy(TeamBarsData[i][TeamBarHealth]);
		TextDrawDestroy(TeamBarsData[i][TeamBarText]);

		TeamBarsData[i][TeamBarBlackboard] = Text: INVALID_TEXT_DRAW;
		TeamBarsData[i][TeamBarHealth] = Text: INVALID_TEXT_DRAW;
		TeamBarsData[i][TeamBarText] = Text: INVALID_TEXT_DRAW;
		TeamBarsData[i][TeamBar_Y] = Float: INVALID_PLAYER_ID;
		TeamBarsData[i][TeamBar_Team] = REFEREE;
		TeamBarsData[i][TeamBar_Flashing] = false;
		TeamBarsData[i][TeamBar_HealthWidth] = 0.0;
	}
	Iter_Clear(PlayerWithTeamBar);

	for (new p = 0; p < MAX_PLAYERS; p++)
	{
		for (new b = 0; b < MAX_PLAYERS; b++)
		{
			TeamBarShownForPlayer[p][b] = false;
		}
	}
	return 1;
}

// This triggers damage flash on a player's bar
TriggerPlayerTeamBarFlash(playerid)
{
	if (!Iter_Contains(PlayerWithTeamBar, playerid))
		return 0;

	TeamBarsData[playerid][TeamBar_Flashing] = true;
	new gen = ++TeamBarFlashGen[playerid];
	SetTimerEx("StopPlayerTeamBarFlash", 3000, false, "ii", playerid, gen);
	UpdatePlayerTeamBar(playerid);
	return 1;
}

// Timer callback to stop damage flash
public StopPlayerTeamBarFlash(playerid, gen)
{
	if (!Iter_Contains(PlayerWithTeamBar, playerid))
		return 0;
	if (TeamBarFlashGen[playerid] != gen)
		return 0;

	TeamBarsData[playerid][TeamBar_Flashing] = false;
	UpdatePlayerTeamBar(playerid);
	return 1;
}

// This updates a player team bar with the correct damage done and HP
UpdatePlayerTeamBar(playerid)
{
	if (!Iter_Contains(PlayerWithTeamBar, playerid))
		return 0;

	new hp = GetHP(playerid) + GetAP(playerid);
	if (!Player[playerid][Playing] || hp <= 0)
	{
		new team = TeamBarsData[playerid][TeamBar_Team];
		DeletePlayerTeamBar(playerid);
		ReorganizeTeamBars(team);
		return 1;
	}

	if (hp > 200) hp = 200;

	new Float: startX = (TeamBarsData[playerid][TeamBar_Team] == ATTACKER) ? TEAMBARS_ATTACKER_START_X : TEAMBARS_DEFENDER_START_X;

	new textStr[64], nameStr[MAX_PLAYER_NAME];
	if (strlen(Player[playerid][Name]) <= 13)
		format(nameStr, sizeof(nameStr), "%s", Player[playerid][Name]);
	else
		format(nameStr, sizeof(nameStr), "%s", Player[playerid][NameWithoutTag]);

	format(textStr, sizeof(textStr), "~w~%s: %d - ~r~%d", nameStr, hp, floatround(Player[playerid][RoundDamage]));
	TextDrawSetString(TeamBarsData[playerid][TeamBarText], textStr);

	new Float: filledWidth = (float(hp) / 200.0) * TEAMBARS_BAR_WIDTH;
	TeamBarsData[playerid][TeamBar_HealthWidth] = filledWidth;
	TextDrawTextSize(TeamBarsData[playerid][TeamBarHealth], startX + filledWidth, 930.000);

	new boxColor;
	if (TeamBarsData[playerid][TeamBar_Team] == ATTACKER)
		boxColor = TeamBarsData[playerid][TeamBar_Flashing] ? TEAMBARS_ATTACKER_FLASH_COLOR : TEAMBARS_ATTACKER_COLOR;
	else
		boxColor = TeamBarsData[playerid][TeamBar_Flashing] ? TEAMBARS_DEFENDER_FLASH_COLOR : TEAMBARS_DEFENDER_COLOR;

	TextDrawBoxColor(TeamBarsData[playerid][TeamBarHealth], boxColor);

	foreach (new i : Player)
	{
		if (ShouldPlayerSeeTeamBars(i))
		{
			if (TeamBarShownForPlayer[i][playerid])
			{
				TextDrawHideForPlayer(i, TeamBarsData[playerid][TeamBarHealth]);
				TextDrawShowForPlayer(i, TeamBarsData[playerid][TeamBarHealth]);
			}
			else
			{
				TextDrawShowForPlayer(i, TeamBarsData[playerid][TeamBarBlackboard]);
				TextDrawShowForPlayer(i, TeamBarsData[playerid][TeamBarHealth]);
				TextDrawShowForPlayer(i, TeamBarsData[playerid][TeamBarText]);
				TeamBarShownForPlayer[i][playerid] = true;
			}
		}
		else
		{
			TeamBarHideForPlayer(i, playerid);
		}
	}
	return 1;
}
