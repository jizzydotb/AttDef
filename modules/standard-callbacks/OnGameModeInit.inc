#if defined _attdef_OnGMInit
	#endinput
#endif
#define _attdef_OnGMInit

static PeriodicPlayTimeTimer;
static PeriodicPauseDetectionTimer;
static PeriodicVehicleHealthTimer;
static PeriodicTimerRetry;

static StartPeriodicTimers()
{
	if (!PeriodicPlayTimeTimer)
	{
		PeriodicPlayTimeTimer = SetTimer("UpdatePlayTime", 60000, true);
		if (!PeriodicPlayTimeTimer)
			printf("[AttDef] Failed to start the play-time update timer.");
	}

	if (!PeriodicPauseDetectionTimer)
	{
		PeriodicPauseDetectionTimer = SetTimer("OnPlayerPause", 1000, true);
		if (!PeriodicPauseDetectionTimer)
			printf("[AttDef] Failed to start the pause-detection timer.");
	}

	if (!PeriodicVehicleHealthTimer)
	{
		PeriodicVehicleHealthTimer = SetTimer("UpdateVehicleHealthLabels", 250, true);
		if (!PeriodicVehicleHealthTimer)
			printf("[AttDef] Failed to start vehicle-health label updates.");
	}

	if ((!PeriodicPlayTimeTimer || !PeriodicPauseDetectionTimer || !PeriodicVehicleHealthTimer)
		&& !PeriodicTimerRetry)
	{
		PeriodicTimerRetry = SetTimer("RetryPeriodicTimers", 5000, false);
		if (!PeriodicTimerRetry)
			printf("[AttDef] Failed to schedule retry for missing periodic timers.");
	}
	return 1;
}

forward RetryPeriodicTimers();
public RetryPeriodicTimers()
{
	PeriodicTimerRetry = 0;
	return StartPeriodicTimers();
}

public OnGameModeInit()
{
	Server_DisableLongCall();

	if (!InitScriptCoreSettings())
	{
		printf("[AttDef] Fatal: core settings could not be initialized. Shutting down.");
		SendRconCommand("exit");
		return 0;
	}
	InitScriptSecondarySettings();
	AddToServersDatabase();

	if (!GetSVarInt("AC-Black Diamond"))
	{
		SendRconCommand("loadfs AC_Black_Diamond-omp");
	}
	ACBD_ToggleConnectionMessage(false);

	StartPeriodicTimers();

	for (new i = 0; i < MAX_VEHICLES; i++)
	{
		VehicleHealthText3D[i] = INVALID_3DTEXT_ID;
	}

	TimeLimit = 23;

	CreateMainSpawnText3D();

	Server_RestartLongCall();
	Server_EnableLongCall();
	return 1;
}
