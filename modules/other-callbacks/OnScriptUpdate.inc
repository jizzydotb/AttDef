#if defined _attdef_OnScriptUpdate
	#endinput
#endif
#define _attdef_OnScriptUpdate

#define ARMOUR_PING_OBJECT_SLOT 1

stock UpdateArmourPingObjects()
{
	foreach (new i : Player)
	{
		new bool: shouldShow = ArmourToggle && GetPlayerPing(i) >= ArmourPingMin && CanPlay(i) && Player[i][Spawned] && !Player[i][Spectating];
		if (shouldShow)
		{
			if (!IsPlayerAttachedObjectSlotUsed(i, ARMOUR_PING_OBJECT_SLOT))
			{
				SetPlayerAttachedObject(i, ARMOUR_PING_OBJECT_SLOT, 373, 17,
					-0.032000, -0.013000, -0.166999,
					69.600006, 16.500011, 30.200008,
					0.933000, 0.978000, 1.039999);
			}
		}
		else if (IsPlayerAttachedObjectSlotUsed(i, ARMOUR_PING_OBJECT_SLOT))
		{
			RemovePlayerAttachedObject(i, ARMOUR_PING_OBJECT_SLOT);
		}
	}
	return 1;
}

// OnScriptUpdate - interval: 1000 ms
task OnScriptUpdate[1000]()
{
	UpdateArmourPingObjects();

	return 1;
}
