#if defined _attdef_cmd_dm
	#endinput
#endif
#define _attdef_cmd_dm

YCMD: dm(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "teleport you to a deathmatch arena.");
	if (Player[playerid][InDuel])
		return SendErrorMessage(playerid, "Leave the duel with /rq before joining a deathmatch.");
	new DMID;
	if (sscanf(params, "I(-1)", DMID) || DMID < 0)
		return SendUsageMessage(playerid, "/dm [DM ID]");
	if (Player[playerid][Playing])
		return 1;

	if (!Iter_Contains(CurrentDMs, DMID))
		return SendErrorMessage(playerid, "This DM does not exist.");

	if (Player[playerid][Spectating])
		StopSpectate(playerid);

	new bool: alreadyInDM = Player[playerid][InDM] && Player[playerid][DMReadd] == DMID;

	ResetPlayerWeapons(playerid);
	new dmVirtualWorld = DMID + 6;
	SetPlayerVirtualWorld(playerid, dmVirtualWorld);
	if (CPZone != INVALID_GANG_ZONE)
		GangZoneHideForPlayer(playerid, CPZone);
	SetHPAP(playerid, 100.0, 100.0);

	SetPlayerDMState(playerid, DMID);
	Player[playerid][VWorld]  = dmVirtualWorld;

	new skin;
	if (Player[playerid][pSkin] == -1)
	{
		skin = Skin[Player[playerid][Team]];
	}
	else
	{
		skin = Player[playerid][pSkin];
	}

	SetSpawnInfo(
		playerid, NO_TEAM, skin, DMSpawn[DMID][0] + RandomMinMax(-1, 1), DMSpawn[DMID][1] + RandomMinMax(-1, 1), DMSpawn[DMID][2], DMSpawn[DMID][3],
		DMWeapons[DMID][0], 9999, DMWeapons[DMID][1], 9999, DMWeapons[DMID][2], 9999);
	Player[playerid][IgnoreSpawn] = true;
	SpawnPlayer(playerid);
	SetPlayerInterior(playerid, DMInterior[DMID]);
	SetPlayerTeam(playerid, NO_TEAM);

	GivePlayerWeapon(playerid, DMWeapons[DMID][3], 9999);
	GivePlayerWeapon(playerid, DMWeapons[DMID][4], 9999);

	if (!alreadyInDM)
	{
		foreach (new i : Player)
		{
			if (!Player[i][Playing] && !Player[i][InDuel])
			{
				if (DMWeapons[DMID][1] == 0 && DMWeapons[DMID][2] == 0 && DMWeapons[DMID][3] == 0 && DMWeapons[DMID][4] == 0)
					va_SendClientMessage(i, -1, "{9593DC}%s entered the {BDBFE4}\"%s\" {9593DC}DM. {BDBFE4}/DM %d {9593DC}to join!", Player[playerid][Name], ReturnWeaponName(DMWeapons[DMID][0]), DMID);
				else if (DMWeapons[DMID][2] == 0 && DMWeapons[DMID][3] == 0 && DMWeapons[DMID][4] == 0)
					va_SendClientMessage(i, -1, "{9593DC}%s entered the {BDBFE4}\"%s & %s\" {9593DC}DM. {BDBFE4}/DM %d {9593DC}to join!", Player[playerid][Name],
					ReturnWeaponName(DMWeapons[DMID][0]), ReturnWeaponName(DMWeapons[DMID][1]), DMID);
				else if (DMWeapons[DMID][3] == 0 && DMWeapons[DMID][4] == 0)
					va_SendClientMessage(i, -1, "{9593DC}%s entered the {BDBFE4}\"%s & %s & %s\" {9593DC}DM. {BDBFE4}/DM %d {9593DC}to join!", Player[playerid][Name],
					ReturnWeaponName(DMWeapons[DMID][0]), ReturnWeaponName(DMWeapons[DMID][1]), ReturnWeaponName(DMWeapons[DMID][2]), DMID);
				else if (DMWeapons[DMID][4] == 0)
					va_SendClientMessage(i, -1, "{9593DC}%s entered the {BDBFE4}\"%s & %s & %s & %s\" {9593DC}DM. {BDBFE4}/DM %d {9593DC}to join!", Player[playerid][Name],
					ReturnWeaponName(DMWeapons[DMID][0]), ReturnWeaponName(DMWeapons[DMID][1]), ReturnWeaponName(DMWeapons[DMID][2]), ReturnWeaponName(DMWeapons[DMID][3]), DMID);
				else
					va_SendClientMessage(i, -1, "{9593DC}%s entered the {BDBFE4}\"%s & %s & %s & %s & %s\" {9593DC}DM. {BDBFE4}/DM %d {9593DC}to join!", Player[playerid][Name],
					ReturnWeaponName(DMWeapons[DMID][0]), ReturnWeaponName(DMWeapons[DMID][1]), ReturnWeaponName(DMWeapons[DMID][2]), ReturnWeaponName(DMWeapons[DMID][3]), ReturnWeaponName(DMWeapons[DMID][4]), DMID);
			}
		}
	}

	if (Player[playerid][BeingSpeced])
	{
		foreach (new i : Player)
		{
			if (Player[i][Spectating] && Player[i][IsSpectatingID] == playerid)
			{
				StopSpectate(i);
			}
		}
	}
	return 1;
}
