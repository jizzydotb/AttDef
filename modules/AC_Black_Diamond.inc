// by edgy
#if defined _INC_ac_black_diamond
	#endinput
#endif
#define _INC_ac_black_diamond

#tryinclude <open.mp>

#if !defined _INC_open_mp
	#include <a_samp>
#endif

#include <YSI_Coding\y_hooks>

stock ACBD_ToggleConnectionMessage(bool: toggle)
{
	return SetSVarInt("ACBD_DisableConnectionMessage", !toggle);
}

forward OnOldVersionDetected(playerid);
forward OnImprovedDeagleDetected(playerid);
forward OnExtraWsDetected(playerid);
forward OnS0beitDetected(playerid);
forward OnSampfuncsDetected(playerid);
forward OnSprintHookDetected(playerid);
forward OnModsDetected(playerid);
forward OnBypassDetected(playerid);
forward OnSilentAimDetected(playerid);

hook OnPlayerConnect(playerid)
{
	#pragma unused playerid
	static bool: notified;

	ACBD_ToggleConnectionMessage(false);
	if (!notified)
	{
		new const LOADED = GetSVarInt("AC-Black Diamond");

		if (!LOADED)
		{
			print("AC-Black Diamond is not loaded! Anticheat callbacks will not work");
		}

		notified = true;
	}
	return 1;
}
