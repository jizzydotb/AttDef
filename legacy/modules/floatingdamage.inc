#if defined _attdef_floatingdamage
	#endinput
#endif
#define _attdef_floatingdamage

#define MAX_FLOATING_LABELS_PER_PLAYER 20
#define FLOATING_DAMAGE_DURATION       1500

enum E_FLOATING_LABEL {
	PlayerText3D: fl_LabelId,
	fl_TimerId,
	bool: fl_Active
};

new bool: FloatingDamage = true;
new FloatingLabels[MAX_PLAYERS][MAX_FLOATING_LABELS_PER_PLAYER][E_FLOATING_LABEL];
new FloatingLabelHead[MAX_PLAYERS];

forward OnFloatingLabelExpire(playerid, slot);
forward CreateFloatingDamageText(issuerid, playerid, Float: amount);
forward SpawnFloatingDamageForPlayer(viewerid, Float: amount, Float: x, Float: y, Float: z);
forward DestroyPlayerFloatingLabels(playerid);
forward ResetAllFloatingDamageLabels();
forward ToggleFloatingDamage(playerid);
