#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_config
	#endinput
#endif
#define _attdef_mod_config

/*
	Gamemode configuration
*/

#include "modules/this_core.inc"

new MainWeather;		 // Server start up weather
new MainTime;			 // Server start up time
new ConfigCPTime;		 // CP time
new ConfigRoundTime;	 // Round time
new Float: MainSpawn[4]; // Main spawn position and facing angle
new MainInterior;		 // Main spawn interior
new WeatherLimit		   = 50;
new TimeLimit			   = 50;
new GAME_SPEED			   = 1000;
new Float: DeagleNerfValue = 35.000;
new Float: SniperNerfValue = 25.000;

new Float: Max_Packetloss;
new Max_Ping;
new Min_FPS;
new Max_FPS;

new WinningScore;
new gAddmeTime;
new gGunmenuTime;

new bool: DeadBodies				  = false;
new bool: Aircraft					  = false;
new bool: AutoEnd					  = false;
new bool: ShowHPBars				  = true;
new bool: AntiSpam					  = true;
new bool: LobbyGuns					  = true;
new bool: NoSwitch					  = false;
new bool: LobbyVehicles				  = true;
new bool: ServerLocked				  = false;
new bool: PermLocked				  = false;
new bool: CPInArena					  = false;
new bool: HalfDeagle				  = true;
new bool: AntiMacros				  = true;
new bool: AntiCBug					  = false;
new bool: AntiSlideBug				  = false;
new bool: MeleeAllowed				  = false;
new bool: AutoRoundStarter			  = false;
new bool: RandomDeathMessages		  = false;
new bool: SightseeingInClassSelection = false;
new bool: VehicleHealthTextdraw		  = false;
new bool: SyncAbuse					  = true;
new bool: ScreenFlashing			  = true;
new bool: DefendersWithVehicles		  = false;
new bool: ShouldNerfDeagle			  = false;
new bool: ShouldNerfSniper			  = false;
new bool: UseCleo					  = false;
new bool: UseSampfuncs				  = false;
new bool: UseExtraws				  = false;
new bool: UseSprinthook				  = false;
#define ANTICBUG_CHECK_WINDOW_MS (3000)
#define ANTISLIDEBUG_INPUT_WINDOW_MS (2000)
#define ANTIBUG_FREEZE_DURATION_MS (2000)
new bool: AntiCBugCheckActive[MAX_PLAYERS];
new AntiCBugCheckStarted[MAX_PLAYERS];
new AntiCBugCheckWeapon[MAX_PLAYERS];
new AntiCBugCheckAmmo[MAX_PLAYERS];
new AntiBugTrackedGeneration[MAX_PLAYERS];
new AntiSlideBugCrouchTick[MAX_PLAYERS];
new bool: AntiSlideBugTriggered[MAX_PLAYERS];
new AntiBugFreezeGeneration[MAX_PLAYERS];
new bool: AntiBugFrozen[MAX_PLAYERS];

new RoundHP							  = 100;
new RoundAR							  = 100;

new Skin[MAX_TEAMS];
