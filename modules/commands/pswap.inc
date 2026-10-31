#if defined _attdef_cmd_pswap
	#endinput
#endif
#define _attdef_cmd_pswap

YCMD: pswap(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Swap another player's team.");

	new targetid;
	if (sscanf(params, "r", targetid))
		return SendUsageMessage(playerid, "/pswap [Player Name/ID]");

	if (targetid == INVALID_PLAYER_ID || !IsPlayerConnected(targetid))
		return SendErrorMessage(playerid, "That player isn't connected.");
	if (Player[targetid][Playing])
		return SendErrorMessage(playerid, "Can't swap while the target player is playing.");
	if (Player[targetid][InDuel])
		return SendErrorMessage(playerid, "Can't use this command during duel.");
	if (NoSwitch)
		return SendErrorMessage(playerid, "Team switching is currently disabled.");

	// Define reusable variables
	new newTeam, oldTeam = Player[targetid][Team];

	// Swap the selected player's team based on their current team
	switch (Player[targetid][Team])
	{
		case ATTACKER, ATTACKER_SUB:
		{
			newTeam = DEFENDER;
		}
		case DEFENDER, DEFENDER_SUB:
		{
			newTeam = ATTACKER;
		}
		case REFEREE:
		{
			new countAttacker = 0, countDefender = 0;

			foreach (new i : Player)
			{
				switch (Player[i][Team])
				{
					case ATTACKER: countAttacker++;
					case DEFENDER: countDefender++;
				}
			}

			newTeam = (countAttacker < countDefender) ? ATTACKER : DEFENDER;
		}
	}

	// Assign the new team to the player
	Player[targetid][Team] = newTeam;

	// Check if the player is in DM or Duel
	if (!Player[targetid][InDM] && !Player[targetid][InDuel])
	{
		SetPlayerSkin(targetid, Skin[newTeam]);
	}

	// Broadcast message
	if (targetid == playerid)
	{
		va_SendClientMessageToAll(
			-1,
			""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has swapped himself "COL_PRIM"from "COL_SEC"%s "COL_PRIM"to "COL_SEC"%s"COL_PRIM".",
			PLAYER_ROLE_NAME(playerid),
			Player[playerid][Name],
			TeamName[oldTeam],
			TeamName[newTeam]
		);
	}
	else
	{
		va_SendClientMessageToAll(
			-1,
			""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has swapped "COL_SEC"%s "COL_PRIM"from "COL_SEC"%s "COL_PRIM"to "COL_SEC"%s"COL_PRIM".",
			PLAYER_ROLE_NAME(playerid),
			Player[playerid][Name],
			Player[targetid][Name],
			TeamName[oldTeam],
			TeamName[newTeam]
		);
	}

	// Additional adjustments if needed
	if (HalfDeagle)
		AdjustDeagleLimit();

	// Update the player's interface and team parameters
	UpdatePlayerHealthBar(targetid);
	UpdateRoundKillDmgTDmg(targetid);
	ColorFix(targetid);
	return 1;
}
