#if defined _attdef_match_cameras
	#endinput
#endif
#define _attdef_match_cameras

/*
	Match/Round Cinematic Cameras & View Sequences Sub-module
*/

new bool:BaseCameraInitialized[MAX_PLAYERS];
new Float:BaseCameraHeight[MAX_PLAYERS];
new Float:BaseCameraRadius[MAX_PLAYERS];
new Float:BaseCameraAngle[MAX_PLAYERS];
new BaseCameraMovementTimer;
new BaseCameraMovementGeneration;

forward BaseCameraMovementTick(generation);
public BaseCameraMovementTick(generation)
{
	if (generation != BaseCameraMovementGeneration)
		return 0;

	if (Current < 0 || Current >= MAX_BASES || GameType != BASE)
	{
		if (BaseCameraMovementTimer)
		{
			KillTimer(BaseCameraMovementTimer);
			BaseCameraMovementTimer = 0;
		}
		return 0;
	}

	foreach (new i : Player)
	{
		if (!Player[i][ToAddInRound])
			continue;

		if (!BaseCameraInitialized[i])
		{
			BaseCameraRadius[i] = 30.0;
			BaseCameraHeight[i] = BCPSpawn[Current][2] + 55.5;
			BaseCameraAngle[i] = 0.0;
			BaseCameraInitialized[i] = true;
			new Float:cameraX = BCPSpawn[Current][0] + BaseCameraRadius[i] * floatsin(BaseCameraAngle[i], degrees);
			new Float:cameraY = BCPSpawn[Current][1] + BaseCameraRadius[i] * floatcos(BaseCameraAngle[i], degrees);
			SetPlayerCameraPos(i, cameraX, cameraY, BaseCameraHeight[i]);
			SetPlayerCameraLookAt(i, BCPSpawn[Current][0], BCPSpawn[Current][1], BCPSpawn[Current][2]);
		}
		else
		{
			new Float:cameraX = BCPSpawn[Current][0] + BaseCameraRadius[i] * floatsin(BaseCameraAngle[i], degrees);
			new Float:cameraY = BCPSpawn[Current][1] + BaseCameraRadius[i] * floatcos(BaseCameraAngle[i], degrees);
			SetPlayerCameraPos(i, cameraX, cameraY, BaseCameraHeight[i]);
			SetPlayerCameraLookAt(i, BCPSpawn[Current][0], BCPSpawn[Current][1], BCPSpawn[Current][2]);

			if (BaseCameraRadius[i] > 2.0)
			{
				BaseCameraRadius[i] -= 0.1;
				if (BaseCameraRadius[i] < 2.0)
					BaseCameraRadius[i] = 2.0;
			}

			new Float:minHeight = BCPSpawn[Current][2] + 3.0;
			if (BaseCameraHeight[i] > minHeight)
			{
				BaseCameraHeight[i] -= 0.2;
				if (BaseCameraHeight[i] < minHeight)
					BaseCameraHeight[i] = minHeight;
			}

			BaseCameraAngle[i] += 1.07;
			if (BaseCameraAngle[i] >= 360.0)
				BaseCameraAngle[i] -= 360.0;
		}
	}
	return 1;
}

StartBaseCameraMovement()
{
	if (BaseCameraMovementTimer)
		KillTimer(BaseCameraMovementTimer);

	BaseCameraMovementGeneration++;
	foreach (new i : Player)
		BaseCameraInitialized[i] = false;

	BaseCameraMovementTick(BaseCameraMovementGeneration);
	BaseCameraMovementTimer = SetTimerEx(
		"BaseCameraMovementTick", 33, true, "i", BaseCameraMovementGeneration);
	if (!BaseCameraMovementTimer)
		printf("[AttDef] Failed to schedule base camera movement.");
	return BaseCameraMovementTimer != 0;
}

StartRoundViewSequence()
{
	RoundViewGeneration++;
	if (RoundViewGeneration == 0)
		RoundViewGeneration++;

	if (ViewRoundTimer)
		KillTimer(ViewRoundTimer);
	ViewRoundTimer = 0;

	if (ArenaCameraMovementTimer)
		KillTimer(ArenaCameraMovementTimer);
	ArenaCameraMovementTimer = 0;
	ArenaCameraGeneration++;
	return RoundViewGeneration;
}

CancelRoundViewSequence()
{
	RoundViewGeneration++;
	if (RoundViewGeneration == 0)
		RoundViewGeneration++;

	if (ViewRoundTimer)
		KillTimer(ViewRoundTimer);
	ViewRoundTimer = 0;

	if (ArenaCameraMovementTimer)
		KillTimer(ArenaCameraMovementTimer);
	ArenaCameraMovementTimer = 0;
	ArenaCameraGeneration++;
	return 1;
}

StopArenaCameraMovement()
{
	if (ArenaCameraMovementTimer)
	{
		KillTimer(ArenaCameraMovementTimer);
		ArenaCameraMovementTimer = 0;
	}
	ArenaCameraGeneration++;
	foreach (new i : Player)
	{
		if (Player[i][ToAddInRound] || Player[i][Playing])
			SetCameraBehindPlayer(i);
	}
	return 1;
}

public StopBaseCameraMovement()
{
	if (BaseCameraMovementTimer)
	{
		KillTimer(BaseCameraMovementTimer);
		BaseCameraMovementTimer = 0;
	}
	BaseCameraMovementGeneration++;
	foreach (new i : Player)
	{
		BaseCameraInitialized[i] = false;
		if (Player[i][ToAddInRound])
			SetCameraBehindPlayer(i);
	}
	return 1;
}
