#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_teamping
	#endinput
#endif
#define _attdef_mod_teamping

#pragma library colandreas
native CA_Init();
native CA_RayCastLine(
	Float: startX, Float: startY, Float: startZ,
	Float: endX, Float: endY, Float: endZ,
	&Float: x, &Float: y, &Float: z
);

#define TEAM_PING_DURATION_MS (5000)
#define TEAM_PING_COOLDOWN_MS (3000)
#define TEAM_PING_DOUBLE_TAP_MS (700)
#define TEAM_PING_ATTACKER_OBJECT_MODEL (19606)
#define TEAM_PING_DEFENDER_OBJECT_MODEL (19607)
#define TEAM_PING_MAP_ICON_TYPE (56)
#define TEAM_PING_DANGER_MAP_ICON_TYPE (19)
#define TEAM_PING_CROSSHAIR_X (0.53)
#define TEAM_PING_CROSSHAIR_Y (0.40)

new bool: TeamPingCollisionReady;
new TeamPingGeneration[MAX_PLAYERS];
new TeamPingOwnerConnectionGeneration[MAX_PLAYERS];
new TeamPingObjects[MAX_PLAYERS][MAX_PLAYERS];
new bool: TeamPingObjectActive[MAX_PLAYERS][MAX_PLAYERS];
new bool: TeamPingMapIconActive[MAX_PLAYERS][MAX_PLAYERS];
new LastTeamPingTick[MAX_PLAYERS];
new bool: TeamPingDanger[MAX_PLAYERS];

forward DestroyTeamPingForViewer(pingerid, viewerid);
forward DestroyTeamPing(pingerid);
forward DestroyTeamPingsForPlayer(playerid);
forward DestroyAllTeamPings();
forward bool: CreateTeamPing(playerid);
