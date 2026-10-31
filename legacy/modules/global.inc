#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_global
	#endinput
#endif
#define _attdef_mod_global

/*
	Global variables
*/

#define MAX_GROUP_ACCESS_PASSWORD_LENGTH 10
#define MAX_GROUP_ACCESS_GROUPS			 5

#define MAX_SERVER_PASS_LENGTH			 21

stock bool:IsValidServerPassword(const password[])
{
	new length = strlen(password);
	if (length == 0 || length > MAX_SERVER_PASS_LENGTH)
		return false;

	for (new i = 0; i < length; i++)
	{
		new character = password[i];
		if ((character >= 'a' && character <= 'z')
			|| (character >= 'A' && character <= 'Z')
			|| (character >= '0' && character <= '9')
			|| character == '_' || character == '-' || character == '.'
			|| character == '!' || character == '@' || character == '#'
			|| character == '+')
			continue;
		return false;
	}
	return true;
}

enum TargetInfoData_STRUCT {
	PlayerText: TargetInfo_Name,
	PlayerText: TargetInfo_ArmourBar,
	PlayerText: TargetInfo_ArmourPercent,
	PlayerText: TargetInfo_HealthBar,
	PlayerText: TargetInfo_HealthPercent,
	PlayerText: TargetInfo_Net,
	TargetInfoTimer,
	TargetInfoTimerGeneration,
	TargetInfoLastTick,
	TargetInfoLastTarget
}

new TargetInfoData[MAX_PLAYERS][TargetInfoData_STRUCT];
new TempNetcheckGeneration[MAX_PLAYERS];
new DelayedFreezeGeneration[MAX_PLAYERS];
new PlayerCounterTextDrawGeneration[MAX_PLAYERS];
new PlayerDeathFlowGeneration[MAX_PLAYERS];
new PlayerDeathMessageGeneration[MAX_PLAYERS];
new SpectatorRemovalGeneration[MAX_PLAYERS];
new PlayerSpectatedConnectionGeneration[MAX_PLAYERS];
new PlayerRejoinSpawnGeneration[MAX_PLAYERS];
new PlayerVehicleRejoinGeneration[MAX_PLAYERS];
new FakePacketRenovationGeneration[MAX_PLAYERS];

new ServerPass[MAX_SERVER_PASS_LENGTH + 10]; // contains "password " plus the password itself
new hostname[64];
new GroupAccessPassword[MAX_GROUP_ACCESS_GROUPS][MAX_GROUP_ACCESS_PASSWORD_LENGTH];
new RequestedGroupPass[MAX_PLAYERS][MAX_GROUP_ACCESS_GROUPS][MAX_GROUP_ACCESS_PASSWORD_LENGTH];

new REPLACE_ToAddID[MAX_PLAYERS]; // replace with dialogs
new REPLACE_ToAddGeneration[MAX_PLAYERS];
new REPLACE_RequestGeneration[MAX_PLAYERS];
new REPLACE_RequestRound[MAX_PLAYERS];
new LastClickedPlayer[MAX_PLAYERS];

new ChangingSkinOfTeam[MAX_PLAYERS];

new bool: ChatDisabled = false;
new AnnTimer;

new bool: ArmourToggle = false;
new ArmourPingMin = 0;

new teamRole[36];

new Text3D: MainSpawnText3D = INVALID_3DTEXT_ID;
new Text3D: VehicleHealthText3D[MAX_VEHICLES];
new Iterator:VehicleHealthLabels<MAX_VEHICLES>;
// new PlayerCPZone[MAX_PLAYERS];
// new PlayerAttackersZone[MAX_PLAYERS];

enum PauseMode {
	PAUSE_MODE_NONE,
	PAUSE_MODE_MESSAGE,
	PAUSE_MODE_PAUSE_ROUND
};

new PauseMode: gPauseMode;
new gBadNetAction = 0; // 0: Message, 1: Remove from round, 2: Kick
// new WeaponSelectionType[MAX_PLAYERS];
new AllowedViewingCommands[MAX_PLAYERS];
new BlackListViewingCommands[MAX_PLAYERS];
new NormalListViewingCommands[MAX_PLAYERS];
new w0[MAX_PLAYERS];
new w0Timer[MAX_PLAYERS];

new oldTeamSwitch;

#define OPTION_TEXT_AND_VALUE 0
#define OPTION_VALUE_ONLY 1
#define OPTION_NONE 2

enum _:TextLabelOptions
{
	TEXTLABEL_Ping = 0,
	TEXTLABEL_FPS,
	TEXTLABEL_PL
};

new GlobalTextLabelOption[TextLabelOptions] = {OPTION_TEXT_AND_VALUE, OPTION_TEXT_AND_VALUE, OPTION_TEXT_AND_VALUE};
