#if defined _attdef_cmd_setteam
	#endinput
#endif
#define _attdef_cmd_setteam

YCMD: setteam(playerid, params[], help)
{
	/*if (Player[playerid][Level] < 1 && !IsPlayerAdmin(playerid))
		return SendErrorMessage(playerid, "You need to be a higher admin level.");*/
	if (help)
		return SendCommandHelpMessage(playerid, "set the team of a player.");

	new pID, teamID;
	if (sscanf(params, "rd", pID, teamID))
		return SendUsageMessage(playerid, "/setteam [Player Name/ID] [Team ID | 0 Att | 1 Def | 2 Ref | 3 Att_Sub | 4 Def_Sub]");

	if (teamID < 0 || teamID > 4)
		return SendErrorMessage(playerid, "Invalid team ID.");
	if (pID == INVALID_PLAYER_ID || !IsPlayerConnected(pID))
		return SendErrorMessage(playerid, "That player isn't connected.");
	if (Player[pID][Playing])
		return SendErrorMessage(playerid, "That player is playing.");
	if (Player[pID][Spectating])
		StopSpectate(pID);

	new MyVehicle = -1;
	new Seat;

	if (IsPlayerInAnyVehicle(pID))
	{
		MyVehicle = GetPlayerVehicleID(pID);
		Seat	  = GetPlayerVehicleSeat(pID);
	}

	Player[pID][Team] = teamID + 1;
	SetPlayerSkin(pID, Skin[teamID + 1]);
	
	if (Player[pID][Team] == ATTACKER)
	{
		format(teamRole, sizeof(teamRole), "(Attackers)");
	}	
	if (Player[pID][Team] == ATTACKER_SUB)
	{
		format(teamRole, sizeof(teamRole), "(Attackers Subs)");
	}	
	if (Player[pID][Team] == DEFENDER)
	{
		format(teamRole, sizeof(teamRole), "(Defenders)");
	}
	if (Player[pID][Team] == DEFENDER_SUB)
	{
		format(teamRole, sizeof(teamRole), "(Defenders Subs)");
	}
	if (Player[pID][Team] == REFEREE)
	{
		format(teamRole, sizeof(teamRole), "(Spectators)");
	}
	
	ColorFix(pID);

	if (Player[pID][Team] == REFEREE)
	{
		HideRoundStats(pID);
		HideEndRoundTextDraw(pID);
		DestroyPlayerBlackoutZone(pID);
		DisablePlayerCheckpoint(pID);
	}
	else
	{
		if (Current != -1)
		{
			ShowTeamBarsForPlayer(pID);
			ShowTeamCornerInfo(pID);
		}
		UpdateRoundKillDmgTDmg(pID);
	}
	UpdateBaseIDTextDraw(pID);

	if (MyVehicle != -1)
		PutPlayerInVehicle(pID, MyVehicle, Seat);

	SwitchTeamFix(pID, false, true);
	UpdatePlayerHealthBar(pID);
	

	if (HalfDeagle && Current == -1)
	{
		AdjustDeagleLimit();
	}

	// Adjusted messaging
	if (pID == playerid) // The command is executed by the player changing their own level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"transferred to "COL_SEC"%s %s"COL_PRIM".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], TeamName[teamID + 1], teamRole);
	}
	else // The command is executed by an admin changing another player's level
	{
		return va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"transferred "COL_SEC"%s "COL_PRIM"to "COL_SEC"%s %s"COL_PRIM".", PLAYER_ROLE_NAME(playerid), Player[playerid][Name], Player[pID][Name], TeamName[teamID + 1], teamRole);
	}
	
}
