#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_fields
	#endinput
#endif
#define _attdef_mod_fields

/*
	Fields related: bases, arenas... etc
*/

#define MAX_BASES		   300
#define MAX_ARENAS		   300
#define MAX_DMS			   100

#define MAX_MAP_FIELD_NAME 64
#define MAP_MIN_X (-3000.0)
#define MAP_MIN_Y (-3000.0)
#define MAP_MAX_X (3000.0)
#define MAP_MAX_Y (3000.0)

// - Base Variables -
new Iterator: CurrentBases<MAX_BASES>;
new BInterior[MAX_BASES];
new Float: BAttackerSpawn[MAX_BASES][3];
new Float: BDefenderSpawn[MAX_BASES][3];
new Float: BCPSpawn[MAX_BASES][3];
new BName[MAX_BASES][MAX_MAP_FIELD_NAME];
new bool: BExist[MAX_BASES] = false;
new TotalBases;
new CurrentBaseSet[32] = "NewBulletproofBases";
new BAttWins[MAX_BASES];
new BDefWins[MAX_BASES];
new BPlayed[MAX_BASES];

// - Arena Variables -
new Iterator: CurrentArenas<MAX_ARENAS>;
new AInterior[MAX_ARENAS];
new Float: AAttackerSpawn[MAX_ARENAS][3];
new Float: ADefenderSpawn[MAX_ARENAS][3];
new Float: ACPSpawn[MAX_ARENAS][3];
new AName[MAX_ARENAS][MAX_MAP_FIELD_NAME];
new Float: AMax[MAX_ARENAS][2];
new Float: AMin[MAX_ARENAS][2];
new bool: AExist[MAX_ARENAS] = false;
new TotalArenas;
new CurrentArenaSet[32] = "Arenas";
new AAttWins[MAX_ARENAS];
new ADefWins[MAX_ARENAS];
new APlayed[MAX_ARENAS];

// - DM Variables -
new Iterator: CurrentDMs<MAX_DMS>;
new Float: DMSpawn[MAX_DMS][4];
new DMInterior[MAX_DMS];
new DMWeapons[MAX_DMS][5];
new bool: DMExist[MAX_DMS] = false;
