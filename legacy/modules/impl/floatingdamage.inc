#if defined _attdef_impl_floatingdamage
	#endinput
#endif
#define _attdef_impl_floatingdamage

public OnFloatingLabelExpire(playerid, slot)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || slot < 0 || slot >= MAX_FLOATING_LABELS_PER_PLAYER)
		return 0;

	if (FloatingLabels[playerid][slot][fl_Active])
	{
		FloatingLabels[playerid][slot][fl_TimerId] = 0;
		if (IsPlayerConnected(playerid) && FloatingLabels[playerid][slot][fl_LabelId] != INVALID_PLAYER_3DTEXT_ID)
		{
			DeletePlayer3DTextLabel(playerid, FloatingLabels[playerid][slot][fl_LabelId]);
		}
		FloatingLabels[playerid][slot][fl_LabelId] = INVALID_PLAYER_3DTEXT_ID;
		FloatingLabels[playerid][slot][fl_Active] = false;
	}
	return 1;
}

stock SpawnFloatingDamageForPlayer(viewerid, Float: amount, Float: x, Float: y, Float: z)
{
	if (viewerid < 0 || viewerid >= MAX_PLAYERS || !IsPlayerConnected(viewerid))
		return 0;

	new slot = FloatingLabelHead[viewerid];
	FloatingLabelHead[viewerid] = (FloatingLabelHead[viewerid] + 1) % MAX_FLOATING_LABELS_PER_PLAYER;

	if (FloatingLabels[viewerid][slot][fl_Active])
	{
		if (FloatingLabels[viewerid][slot][fl_TimerId])
		{
			KillTimer(FloatingLabels[viewerid][slot][fl_TimerId]);
			FloatingLabels[viewerid][slot][fl_TimerId] = 0;
		}
		if (FloatingLabels[viewerid][slot][fl_LabelId] != INVALID_PLAYER_3DTEXT_ID)
		{
			DeletePlayer3DTextLabel(viewerid, FloatingLabels[viewerid][slot][fl_LabelId]);
			FloatingLabels[viewerid][slot][fl_LabelId] = INVALID_PLAYER_3DTEXT_ID;
		}
		FloatingLabels[viewerid][slot][fl_Active] = false;
	}

	new text[16];
	format(text, sizeof(text), "-%d", floatround(amount, floatround_round));

	new PlayerText3D: lbl = CreatePlayer3DTextLabel(
		viewerid, text, 0xFFFFFFCC, x, y, z, (DRAW_DISTANCE + 5.0),
		INVALID_PLAYER_ID, INVALID_VEHICLE_ID, true);

	if (lbl != INVALID_PLAYER_3DTEXT_ID)
	{
		FloatingLabels[viewerid][slot][fl_LabelId] = lbl;
		FloatingLabels[viewerid][slot][fl_Active] = true;
		FloatingLabels[viewerid][slot][fl_TimerId] = SetTimerEx("OnFloatingLabelExpire", FLOATING_DAMAGE_DURATION, false, "ii", viewerid, slot);
	}
	return 1;
}

stock CreateFloatingDamageText(issuerid, playerid, Float: amount)
{
	if (!FloatingDamage || amount <= 0.0)
		return 0;

	if (issuerid < 0 || issuerid >= MAX_PLAYERS || !IsPlayerConnected(issuerid))
		return 0;

	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	new Float: x, Float: y, Float: z;
	GetPlayerPos(playerid, x, y, z);

	// Slight scatter so consecutive shots at the same spot (M4/Deagle) don't overlap into an unreadable blob
	new Float: offsetX = float(random(21) - 10) / 45.0;
	new Float: offsetY = float(random(21) - 10) / 45.0;
	new Float: offsetZ = float(random(11) - 5) / 50.0;

	new Float: spawnX = x + offsetX;
	new Float: spawnY = y + offsetY;
	new Float: spawnZ = z + 0.40 + offsetZ; // Centered on torso/chest

	if (Player[issuerid][ShowFloatingDamage])
	{
		SpawnFloatingDamageForPlayer(issuerid, amount, spawnX, spawnY, spawnZ);
	}

	// Show to spectators watching this shooter
	foreach (new s : Player)
	{
		if (Player[s][Spectating] && Player[s][IsSpectatingID] == issuerid && Player[s][ShowFloatingDamage])
		{
			SpawnFloatingDamageForPlayer(s, amount, spawnX, spawnY, spawnZ);
		}
	}
	return 1;
}

stock DestroyPlayerFloatingLabels(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	for (new slot = 0; slot < MAX_FLOATING_LABELS_PER_PLAYER; slot++)
	{
		if (FloatingLabels[playerid][slot][fl_Active])
		{
			if (FloatingLabels[playerid][slot][fl_TimerId])
			{
				KillTimer(FloatingLabels[playerid][slot][fl_TimerId]);
				FloatingLabels[playerid][slot][fl_TimerId] = 0;
			}
			if (IsPlayerConnected(playerid) && FloatingLabels[playerid][slot][fl_LabelId] != INVALID_PLAYER_3DTEXT_ID)
			{
				DeletePlayer3DTextLabel(playerid, FloatingLabels[playerid][slot][fl_LabelId]);
			}
			FloatingLabels[playerid][slot][fl_LabelId] = INVALID_PLAYER_3DTEXT_ID;
			FloatingLabels[playerid][slot][fl_Active] = false;
		}
	}
	FloatingLabelHead[playerid] = 0;
	return 1;
}

stock ResetAllFloatingDamageLabels()
{
	foreach (new i : Player)
	{
		DestroyPlayerFloatingLabels(i);
	}
	return 1;
}

stock ToggleFloatingDamage(playerid)
{
	FloatingDamage = !FloatingDamage;
	if (sqliteconnection)
	{
		DatabaseExecuteWrite(
			va_return("INSERT OR REPLACE INTO Configs (Option, Value) VALUES ('FloatingDamage', '%d')", FloatingDamage),
			"save floating damage setting");
	}
	va_SendClientMessageToAll(
		-1, ""COL_PRIM"%s "COL_SEC"%s "COL_PRIM"has %s "COL_PRIM"Floating Dmg system.",
		PLAYER_ROLE_NAME(playerid), Player[playerid][Name], FloatingDamage ? "{AADC93}enabled" : "{EB8989}disabled");
	return 1;
}
