#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_common
	#endinput
#endif
#define _attdef_mod_common

/*
	Common stuff
*/

// Common shortcuts
#define MAX_DEADBODY_LABELS		_: MAX_3DTEXT_GLOBAL

#define PRESSED(%0)				(newkeys & (%0) == (%0) && oldkeys & (%0) != (%0))
#define RELEASED(%0)			(newkeys & (%0) != (%0) && oldkeys & (%0) == (%0))
#define HOLDING(%0)				(newkeys & (%0) == (%0))

#define PLAYER_ROLE_NAME(%0)	(Player[%0][Level] > 0 ? "Admin" : "Player")

#define ShiftRGBAToRGB(%0)		((%0) >>> 8)

#define MAX_LOWFPS_WARNINGS		10
#define MAX_HIGHFPS_WARNINGS	10
#define MAX_PACKETLOSS_WARNINGS 15
#define MAX_HIGHPING_WARNINGS	10

new Iterator: DeadBodyActors<MAX_ACTORS>;
new Iterator: DeadBodyLabels<MAX_DEADBODY_LABELS>;

#define DEATH_BLOOD_EFFECT_MODEL		18668
#define DEATH_BLOOD_EFFECT_DURATION		3500

#define DEAD_BODY_FLARE_MODEL			18728
#define DEAD_BODY_FLARE_DURATION		4000
#define DEAD_BODY_LAND_DELAY			2500

new DeathBloodObject[MAX_PLAYERS] = {INVALID_OBJECT_ID, ...};
new DeadBodyFlareObject[MAX_PLAYERS] = {INVALID_OBJECT_ID, ...};
new Float: DeadBodyFlarePos[MAX_PLAYERS][3];
new DeadBodyFlareGeneration[MAX_PLAYERS] = {0, ...};

forward CreatePlayerBloodEffect(playerid, Float: x, Float: y, Float: z);
forward DestroyPlayerBloodEffect(playerid, objectid);
forward OnDeadBodyLanded(playerid, connectionGeneration, flareGen, actorid);
forward CreateDeadBodySmokeFlare(playerid, Float: x, Float: y, Float: z);
forward DestroyDeadBodyFlare(playerid, objectid);
forward DestroyPlayerDeathEffects(playerid);
forward DeleteAllDeathBloodEffects();
forward GetPlayerTeamID(playerid);

forward CreateMainSpawnText3D();
forward DestroyMainSpawnText3D();
forward AttachHealthTextLabelToVehicle(vehicleid);
forward UpdateVehicleHealthLabel(vehicleid, bool: force = false);
forward UpdateVehicleHealthLabels();
forward UpdatePlayerHealthMoney(playerid);
forward UpdateExistingVehicleHealthLabels();
forward DestroyVehicleHealthLabels();
forward ClearDamageIndicator(playerid);
forward GetTeamHealthBarColor(team);
forward hidew0(playerid);
forward SendAdminMessage(const message[]);

new Float: lastVehicleHealth[MAX_VEHICLES];
new lastVehicleHealthPollTick[MAX_VEHICLES];

forward LoadBases();
forward LoadArenas();
forward ShowBaseSetsDialog(playerid);
forward ShowArenaSetsDialog(playerid);
forward DoLoadBases(playerid, baseset);
forward DoLoadArenas(playerid, arenaset);
forward UpdateBaseIDTextDraw(playerid);
forward GetArenaCount(const arenaSetName[]);
forward GetBaseCount(const baseSetName[]);
forward HandleNetworkIssue(playerid);
forward CheckPlayerNet(playerid);
forward AimbotCheck(playerid);
forward UpdateModDetectionMessage(playerid);
forward ClearSpectatorState(playerid);
forward CheckWinCondition();
forward SwapTeams();
forward BalanceTeams();
forward AssignMatchTeamsByTag();
forward SwitchTeamFix(playerid, bool: message = true, bool: fixvs = true, bool: changeskin = true);
forward GetTeamWithLessPlayers();
forward bool: IsTeamTheSame(team1, team2);
forward GetVehicleModelID(const vehiclename[]);
forward bool: IsAirVehicle(vehicleid);
forward bool: IsVehicleComponentLegal(vehiclemodel, componentid);
forward DestroyDynamicVehicles(whatvehicles);
forward DestroyUnusedVehicles();
forward RespawnInVehicleAfterComeBack(playerid, vehicleid, seatid, connectionGeneration, rejoinGeneration);
forward DeleteAllDeadBodies();
forward CreateDeadBody(playerid, killerid, reason, Float: distance = 0.0, Float: x = 0.0, Float: y = 0.0, Float: z = 0.0);
