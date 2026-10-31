#if defined _attdef_cmd_swapme
	#endinput
#endif
#define _attdef_cmd_swapme

static const teamRoleNames[][] = {
	"",
	"(Attacker)",
	"(Defender)",
	"(Attacker Sub)",
	"(Defender Sub)",
	"(Spectator)"
};

YCMD: swapme(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "Swap your team.");
	if (Player[playerid][Playing])
		return SendErrorMessage(playerid, "Can't swap while playing.");
	if (Player[playerid][InDuel])
		return SendErrorMessage(playerid, "Can't use this command during duel.");
	if (NoSwitch)
		return SendErrorMessage(playerid, "Team switching is currently disabled.");
	if (Player[playerid][Spectating])
		StopSpectate(playerid);

	new newTeam, oldTeam = Player[playerid][Team];

	switch (oldTeam)
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
		default:
		{
			newTeam = ATTACKER;
		}
	}

	Player[playerid][Team] = newTeam;

	if (!Player[playerid][InDM] && !Player[playerid][InDuel])
	{
		SetPlayerSkin(playerid, Skin[newTeam]);
	}

	new roleIndex = (newTeam >= 0 && newTeam < sizeof(teamRoleNames)) ? newTeam : 0;

	if (WarMode)
	{
		va_SendClientMessageToAll(
			-1,
			"%s\xBB %s has swapped from %s to %s %s.",
			TextColor[newTeam],
			Player[playerid][Name],
			TeamName[oldTeam],
			TeamName[newTeam],
			teamRoleNames[roleIndex]
		);
	}
	else
	{
		va_SendClientMessage(
			playerid,
			-1,
			"%s\xBB You have swapped yourself from %s to %s.",
			TextColor[newTeam],
			TeamName[oldTeam],
			TeamName[newTeam]
		);
	}

	if (HalfDeagle)
		AdjustDeagleLimit();

	UpdatePlayerHealthBar(playerid);
	UpdateRoundKillDmgTDmg(playerid);
	ColorFix(playerid);

	return 1;
}
