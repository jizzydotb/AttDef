#if defined _attdef_td_player_hud
	#endinput
#endif
#define _attdef_td_player_hud

/*
	Per-Player HUD, Damage Indicators, Spec & Interface Textdraws Sub-module
*/

public SpectateTextdrawsCreate()
{
	Spec_CardBG = TextDrawCreate(5.911, 334.413, "LD_POKE:cd1s");
	TextDrawTextSize(Spec_CardBG, 98.982, 91.083);
	TextDrawAlignment(Spec_CardBG, 1);
	TextDrawColor(Spec_CardBG, 0x00000096);
	TextDrawSetShadow(Spec_CardBG, 0);
	TextDrawSetOutline(Spec_CardBG, 0);
	TextDrawBackgroundColor(Spec_CardBG, 0x000000FF);
	TextDrawFont(Spec_CardBG, 4);
	TextDrawSetProportional(Spec_CardBG, 1);

	Spec_ArmourLabel = TextDrawCreate(10.118, 341.886, "~w~Armour:");
	TextDrawLetterSize(Spec_ArmourLabel, 0.1439, 0.7559);
	TextDrawTextSize(Spec_ArmourLabel, 637.033, 930.000);
	TextDrawAlignment(Spec_ArmourLabel, 1);
	TextDrawColor(Spec_ArmourLabel, -1);
	TextDrawSetShadow(Spec_ArmourLabel, 0);
	TextDrawSetOutline(Spec_ArmourLabel, 1);
	TextDrawBackgroundColor(Spec_ArmourLabel, 0x000000FF);
	TextDrawFont(Spec_ArmourLabel, 1);
	TextDrawSetProportional(Spec_ArmourLabel, 1);

	Spec_ArmourBarBG = TextDrawCreate(38.889, 342.728, "_");
	TextDrawLetterSize(Spec_ArmourBarBG, 0.1452, 0.6803);
	TextDrawTextSize(Spec_ArmourBarBG, 96.209, 930.000);
	TextDrawAlignment(Spec_ArmourBarBG, 1);
	TextDrawColor(Spec_ArmourBarBG, -1);
	TextDrawUseBox(Spec_ArmourBarBG, 1);
	TextDrawBoxColor(Spec_ArmourBarBG, 0x000000FF);
	TextDrawSetShadow(Spec_ArmourBarBG, 0);
	TextDrawSetOutline(Spec_ArmourBarBG, 0);
	TextDrawBackgroundColor(Spec_ArmourBarBG, 0x000000FF);
	TextDrawFont(Spec_ArmourBarBG, 1);
	TextDrawSetProportional(Spec_ArmourBarBG, 1);

	Spec_HealthLabel = TextDrawCreate(10.687, 354.983, "~w~Health:");
	TextDrawLetterSize(Spec_HealthLabel, 0.1439, 0.7559);
	TextDrawTextSize(Spec_HealthLabel, 637.603, 930.000);
	TextDrawAlignment(Spec_HealthLabel, 1);
	TextDrawColor(Spec_HealthLabel, -1);
	TextDrawSetShadow(Spec_HealthLabel, 0);
	TextDrawSetOutline(Spec_HealthLabel, 1);
	TextDrawBackgroundColor(Spec_HealthLabel, 0x000000FF);
	TextDrawFont(Spec_HealthLabel, 1);
	TextDrawSetProportional(Spec_HealthLabel, 1);

	Spec_HealthBarBG = TextDrawCreate(38.889, 355.919, "_");
	TextDrawLetterSize(Spec_HealthBarBG, 0.1452, 0.6803);
	TextDrawTextSize(Spec_HealthBarBG, 96.209, 930.000);
	TextDrawAlignment(Spec_HealthBarBG, 1);
	TextDrawColor(Spec_HealthBarBG, -1);
	TextDrawUseBox(Spec_HealthBarBG, 1);
	TextDrawBoxColor(Spec_HealthBarBG, 0x000000FF);
	TextDrawSetShadow(Spec_HealthBarBG, 0);
	TextDrawSetOutline(Spec_HealthBarBG, 0);
	TextDrawBackgroundColor(Spec_HealthBarBG, 0x000000FF);
	TextDrawFont(Spec_HealthBarBG, 1);
	TextDrawSetProportional(Spec_HealthBarBG, 1);
	return 1;
}

public TargetInfoTextdrawsCreate()
{
	// Card Background
	TargetInfo_CardBG = TextDrawCreate(41.600, 280.159, "LD_POKE:cd1s");
	TextDrawTextSize(TargetInfo_CardBG, 89.867, 50.521);
	TextDrawAlignment(TargetInfo_CardBG, 1);
	TextDrawColor(TargetInfo_CardBG, 0x0000005F);
	TextDrawSetShadow(TargetInfo_CardBG, 1);
	TextDrawSetOutline(TargetInfo_CardBG, 0);
	TextDrawBackgroundColor(TargetInfo_CardBG, 0x000000FF);
	TextDrawFont(TargetInfo_CardBG, 4);
	TextDrawSetProportional(TargetInfo_CardBG, 1);

	// Armour Slot BG (dark box)
	TargetInfo_ArmourSlotBG = TextDrawCreate(48.859, 289.019, "_");
	TextDrawLetterSize(TargetInfo_ArmourSlotBG, 0.0100, 1.0612);
	TextDrawTextSize(TargetInfo_ArmourSlotBG, 123.474, 930.000);
	TextDrawAlignment(TargetInfo_ArmourSlotBG, 1);
	TextDrawColor(TargetInfo_ArmourSlotBG, 0xC60100FF);
	TextDrawUseBox(TargetInfo_ArmourSlotBG, 1);
	TextDrawBoxColor(TargetInfo_ArmourSlotBG, 0x00000095);
	TextDrawSetShadow(TargetInfo_ArmourSlotBG, 1);
	TextDrawSetOutline(TargetInfo_ArmourSlotBG, 0);
	TextDrawBackgroundColor(TargetInfo_ArmourSlotBG, 0x000000FF);
	TextDrawFont(TargetInfo_ArmourSlotBG, 1);
	TextDrawSetProportional(TargetInfo_ArmourSlotBG, 1);

	// Health Slot BG (dark box)
	TargetInfo_HealthSlotBG = TextDrawCreate(48.859, 303.953, "_");
	TextDrawLetterSize(TargetInfo_HealthSlotBG, 0.0100, 1.0612);
	TextDrawTextSize(TargetInfo_HealthSlotBG, 123.474, 930.000);
	TextDrawAlignment(TargetInfo_HealthSlotBG, 1);
	TextDrawColor(TargetInfo_HealthSlotBG, 0xC60100FF);
	TextDrawUseBox(TargetInfo_HealthSlotBG, 1);
	TextDrawBoxColor(TargetInfo_HealthSlotBG, 0x00000095);
	TextDrawSetShadow(TargetInfo_HealthSlotBG, 1);
	TextDrawSetOutline(TargetInfo_HealthSlotBG, 0);
	TextDrawBackgroundColor(TargetInfo_HealthSlotBG, 0x000000FF);
	TextDrawFont(TargetInfo_HealthSlotBG, 1);
	TextDrawSetProportional(TargetInfo_HealthSlotBG, 1);
	return 1;
}


LoadPlayerTextDraws(playerid)
{
	PointCounterTD[playerid] = CreatePlayerTextDraw(playerid, 341.533, 145.442, "HEADSHOT");
	PlayerTextDrawLetterSize(playerid, PointCounterTD[playerid], 0.2000, 0.9000);
	PlayerTextDrawTextSize(playerid, PointCounterTD[playerid], 400.000, 17.000);
	PlayerTextDrawAlignment(playerid, PointCounterTD[playerid], 3);
	PlayerTextDrawColor(playerid, PointCounterTD[playerid], -116);
	PlayerTextDrawSetShadow(playerid, PointCounterTD[playerid], 1);
	PlayerTextDrawSetOutline(playerid, PointCounterTD[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, PointCounterTD[playerid], 68);
	PlayerTextDrawFont(playerid, PointCounterTD[playerid], 2);
	PlayerTextDrawSetProportional(playerid, PointCounterTD[playerid], 1);

	BaseID_VS[playerid] = CreatePlayerTextDraw(playerid, 548.000000, 25.000000, "_");
	PlayerTextDrawFont(playerid, BaseID_VS[playerid], 1);
	PlayerTextDrawLetterSize(playerid, BaseID_VS[playerid], 0.31000, 1.500000);
	PlayerTextDrawBackgroundColor(playerid, BaseID_VS[playerid], MAIN_BACKGROUND_COLOUR);
	PlayerTextDrawColor(playerid, BaseID_VS[playerid], -65281);
	PlayerTextDrawSetOutline(playerid, BaseID_VS[playerid], 1);
	PlayerTextDrawSetProportional(playerid, BaseID_VS[playerid], 1);
	PlayerTextDrawSetShadow(playerid, BaseID_VS[playerid], 0);

	RoundTeamStatusMessage[playerid] = CreatePlayerTextDraw(playerid, 320.0, 380.0, "_");
	PlayerTextDrawFont(playerid, RoundTeamStatusMessage[playerid], 2);
	PlayerTextDrawLetterSize(playerid, RoundTeamStatusMessage[playerid], 0.23, 1.0);
	PlayerTextDrawAlignment(playerid, RoundTeamStatusMessage[playerid], 2);
	PlayerTextDrawColor(playerid, RoundTeamStatusMessage[playerid], -1);
	PlayerTextDrawSetProportional(playerid, RoundTeamStatusMessage[playerid], 1);
	PlayerTextDrawTextSize(playerid, RoundTeamStatusMessage[playerid], 1280.0, 1280.0);
	PlayerTextDrawSetShadow(playerid, RoundTeamStatusMessage[playerid], 1);
	PlayerTextDrawSetOutline(playerid, RoundTeamStatusMessage[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamStatusMessage[playerid], 0x000000FF);
	PlayerTextDrawSetSelectable(playerid, RoundTeamStatusMessage[playerid], 0);

	RoundTeamStatusCountdown[playerid] = CreatePlayerTextDraw(playerid, 321.0, 435.5, "_");
	PlayerTextDrawFont(playerid, RoundTeamStatusCountdown[playerid], 2);
	PlayerTextDrawLetterSize(playerid, RoundTeamStatusCountdown[playerid], 0.28, 1.0);
	PlayerTextDrawAlignment(playerid, RoundTeamStatusCountdown[playerid], 2);
	PlayerTextDrawColor(playerid, RoundTeamStatusCountdown[playerid], -1);
	PlayerTextDrawSetProportional(playerid, RoundTeamStatusCountdown[playerid], 1);
	PlayerTextDrawSetShadow(playerid, RoundTeamStatusCountdown[playerid], 1);
	PlayerTextDrawSetOutline(playerid, RoundTeamStatusCountdown[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamStatusCountdown[playerid], 0x000000FF);
	PlayerTextDrawSetSelectable(playerid, RoundTeamStatusCountdown[playerid], 0);
	PlayerTextDrawHide(playerid, BaseID_VS[playerid]);

	// TargetInfo Textdraws
	TargetInfoData[playerid][TargetInfo_Name] = CreatePlayerTextDraw(playerid, 65.969, 275.630, "_");
	PlayerTextDrawLetterSize(playerid, TargetInfoData[playerid][TargetInfo_Name], 0.1600, 0.9402);
	PlayerTextDrawTextSize(playerid, TargetInfoData[playerid][TargetInfo_Name], 636.500, 930.000);
	PlayerTextDrawAlignment(playerid, TargetInfoData[playerid][TargetInfo_Name], 1);
	PlayerTextDrawColor(playerid, TargetInfoData[playerid][TargetInfo_Name], -1);
	PlayerTextDrawSetShadow(playerid, TargetInfoData[playerid][TargetInfo_Name], 0);
	PlayerTextDrawSetOutline(playerid, TargetInfoData[playerid][TargetInfo_Name], 1);
	PlayerTextDrawBackgroundColor(playerid, TargetInfoData[playerid][TargetInfo_Name], 0x000000FF);
	PlayerTextDrawFont(playerid, TargetInfoData[playerid][TargetInfo_Name], 1);
	PlayerTextDrawSetProportional(playerid, TargetInfoData[playerid][TargetInfo_Name], 1);

	TargetInfoData[playerid][TargetInfo_ArmourBar] = CreatePlayerTextDraw(playerid, 49.542, 289.849, "_");
	PlayerTextDrawLetterSize(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], 0.0100, 0.8399);
	PlayerTextDrawTextSize(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], 122.958, 930.000);
	PlayerTextDrawAlignment(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], 1);
	PlayerTextDrawColor(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], -1);
	PlayerTextDrawUseBox(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], 1);
	PlayerTextDrawBoxColor(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], 0xFFFFFF95);
	PlayerTextDrawSetShadow(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], 1);
	PlayerTextDrawSetOutline(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], 0);
	PlayerTextDrawBackgroundColor(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], 0x000000FF);
	PlayerTextDrawFont(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], 1);
	PlayerTextDrawSetProportional(playerid, TargetInfoData[playerid][TargetInfo_ArmourBar], 1);

	TargetInfoData[playerid][TargetInfo_ArmourPercent] = CreatePlayerTextDraw(playerid, 86.284, 289.400, "_");
	PlayerTextDrawLetterSize(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent], 0.1450, 0.7200);
	PlayerTextDrawTextSize(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent], 649.550, 930.000);
	PlayerTextDrawAlignment(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent], 2);
	PlayerTextDrawColor(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent], -1);
	PlayerTextDrawSetShadow(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent], 0);
	PlayerTextDrawSetOutline(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent], 1);
	PlayerTextDrawBackgroundColor(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent], 0x000000FF);
	PlayerTextDrawFont(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent], 1);
	PlayerTextDrawSetProportional(playerid, TargetInfoData[playerid][TargetInfo_ArmourPercent], 1);

	TargetInfoData[playerid][TargetInfo_HealthBar] = CreatePlayerTextDraw(playerid, 49.576, 304.948, "_");
	PlayerTextDrawLetterSize(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], 0.0100, 0.8399);
	PlayerTextDrawTextSize(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], 122.991, 930.000);
	PlayerTextDrawAlignment(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], 1);
	PlayerTextDrawColor(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], -1);
	PlayerTextDrawUseBox(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], 1);
	PlayerTextDrawBoxColor(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], 0x47893B95);
	PlayerTextDrawSetShadow(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], 1);
	PlayerTextDrawSetOutline(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], 0);
	PlayerTextDrawBackgroundColor(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], 0x000000FF);
	PlayerTextDrawFont(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], 1);
	PlayerTextDrawSetProportional(playerid, TargetInfoData[playerid][TargetInfo_HealthBar], 1);

	TargetInfoData[playerid][TargetInfo_HealthPercent] = CreatePlayerTextDraw(playerid, 86.284, 304.500, "_");
	PlayerTextDrawLetterSize(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], 0.1450, 0.7200);
	PlayerTextDrawTextSize(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], 647.483, 930.000);
	PlayerTextDrawAlignment(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], 2);
	PlayerTextDrawColor(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], -1);
	PlayerTextDrawSetShadow(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], 0);
	PlayerTextDrawSetOutline(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], 1);
	PlayerTextDrawBackgroundColor(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], 0x000000FF);
	PlayerTextDrawFont(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], 1);
	PlayerTextDrawSetProportional(playerid, TargetInfoData[playerid][TargetInfo_HealthPercent], 1);

	TargetInfoData[playerid][TargetInfo_Net] = CreatePlayerTextDraw(playerid, 50.240, 317.510, "_");
	PlayerTextDrawLetterSize(playerid, TargetInfoData[playerid][TargetInfo_Net], 0.1450, 0.7463);
	PlayerTextDrawTextSize(playerid, TargetInfoData[playerid][TargetInfo_Net], 555.163, 930.000);
	PlayerTextDrawAlignment(playerid, TargetInfoData[playerid][TargetInfo_Net], 1);
	PlayerTextDrawColor(playerid, TargetInfoData[playerid][TargetInfo_Net], -1);
	PlayerTextDrawSetShadow(playerid, TargetInfoData[playerid][TargetInfo_Net], 0);
	PlayerTextDrawSetOutline(playerid, TargetInfoData[playerid][TargetInfo_Net], 1);
	PlayerTextDrawBackgroundColor(playerid, TargetInfoData[playerid][TargetInfo_Net], 0x000000FF);
	PlayerTextDrawFont(playerid, TargetInfoData[playerid][TargetInfo_Net], 1);
	PlayerTextDrawSetProportional(playerid, TargetInfoData[playerid][TargetInfo_Net], 1);

	TD_RoundSpec[playerid] = CreatePlayerTextDraw(playerid, 330.000000, 350.000000, "_");
	PlayerTextDrawFont(playerid, TD_RoundSpec[playerid], 1);
	PlayerTextDrawLetterSize(playerid, TD_RoundSpec[playerid], 0.40000, 2.00000);
	PlayerTextDrawBackgroundColor(playerid, TD_RoundSpec[playerid], MAIN_BACKGROUND_COLOUR);
	PlayerTextDrawColor(playerid, TD_RoundSpec[playerid], -65281);
	PlayerTextDrawSetOutline(playerid, TD_RoundSpec[playerid], 1);
	PlayerTextDrawAlignment(playerid, TD_RoundSpec[playerid], 2);
	PlayerTextDrawSetShadow(playerid, TD_RoundSpec[playerid], 0);

	FPSPingPacketBox[playerid] = CreatePlayerTextDraw(playerid, 554.000000, -2.000000, "_");
	PlayerTextDrawFont(playerid, FPSPingPacketBox[playerid], 1);
	PlayerTextDrawLetterSize(playerid, FPSPingPacketBox[playerid], 0.500000, 1.500000);
	PlayerTextDrawTextSize(playerid, FPSPingPacketBox[playerid], 0.000000, 105.000000);
	PlayerTextDrawSetOutline(playerid, FPSPingPacketBox[playerid], 1);
	PlayerTextDrawSetShadow(playerid, FPSPingPacketBox[playerid], 0);
	PlayerTextDrawAlignment(playerid, FPSPingPacketBox[playerid], 2);
	PlayerTextDrawColor(playerid, FPSPingPacketBox[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, FPSPingPacketBox[playerid], 0x000000FF);
	PlayerTextDrawBoxColor(playerid, FPSPingPacketBox[playerid], 32);
	PlayerTextDrawUseBox(playerid, FPSPingPacketBox[playerid], 1);
	PlayerTextDrawSetProportional(playerid, FPSPingPacketBox[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, FPSPingPacketBox[playerid], 0);

	FPSPingPacket[playerid] = CreatePlayerTextDraw(playerid, 554.000000, 1.400000, "FPS ~b~~h~0 ~w~Ping ~b~~h~0 ~w~PL ~b~~h~0");
	PlayerTextDrawBackgroundColor(playerid, FPSPingPacket[playerid], 48);
	PlayerTextDrawFont(playerid, FPSPingPacket[playerid], 1);
	PlayerTextDrawLetterSize(playerid, FPSPingPacket[playerid], 0.209999, 0.850000);
	PlayerTextDrawColor(playerid, FPSPingPacket[playerid], -1);
	PlayerTextDrawSetOutline(playerid, FPSPingPacket[playerid], 0);
	PlayerTextDrawSetProportional(playerid, FPSPingPacket[playerid], 1);
	PlayerTextDrawAlignment(playerid, FPSPingPacket[playerid], 2);
	PlayerTextDrawSetShadow(playerid, FPSPingPacket[playerid], 1);

	RoundStyleKillsTD[playerid] = CreatePlayerTextDraw(playerid, 3.000000, 396.000000, "_");
	PlayerTextDrawFont(playerid, RoundStyleKillsTD[playerid], 1);
	PlayerTextDrawLetterSize(playerid, RoundStyleKillsTD[playerid], 0.170000, 0.999000);
	PlayerTextDrawBackgroundColor(playerid, RoundStyleKillsTD[playerid], 150);
	PlayerTextDrawColor(playerid, RoundStyleKillsTD[playerid], -1);
	PlayerTextDrawSetOutline(playerid, RoundStyleKillsTD[playerid], 1);
	PlayerTextDrawSetProportional(playerid, RoundStyleKillsTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, RoundStyleKillsTD[playerid], 1);
	PlayerTextDrawAlignment(playerid, RoundStyleKillsTD[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, RoundStyleKillsTD[playerid], 0);

	RoundStyleDamageTD[playerid] = CreatePlayerTextDraw(playerid, 3.000000, 405.000000, "_");
	PlayerTextDrawFont(playerid, RoundStyleDamageTD[playerid], 1);
	PlayerTextDrawLetterSize(playerid, RoundStyleDamageTD[playerid], 0.170000, 0.999000);
	PlayerTextDrawBackgroundColor(playerid, RoundStyleDamageTD[playerid], 150);
	PlayerTextDrawColor(playerid, RoundStyleDamageTD[playerid], -1);
	PlayerTextDrawSetOutline(playerid, RoundStyleDamageTD[playerid], 1);
	PlayerTextDrawSetProportional(playerid, RoundStyleDamageTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, RoundStyleDamageTD[playerid], 1);
	PlayerTextDrawAlignment(playerid, RoundStyleDamageTD[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, RoundStyleDamageTD[playerid], 0);

	RoundStyleTotalDmgTD[playerid] = CreatePlayerTextDraw(playerid, 3.000000, 414.000000, "_");
	PlayerTextDrawFont(playerid, RoundStyleTotalDmgTD[playerid], 1);
	PlayerTextDrawLetterSize(playerid, RoundStyleTotalDmgTD[playerid], 0.170000, 0.999000);
	PlayerTextDrawBackgroundColor(playerid, RoundStyleTotalDmgTD[playerid], 150);
	PlayerTextDrawColor(playerid, RoundStyleTotalDmgTD[playerid], -1);
	PlayerTextDrawSetOutline(playerid, RoundStyleTotalDmgTD[playerid], 1);
	PlayerTextDrawSetProportional(playerid, RoundStyleTotalDmgTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, RoundStyleTotalDmgTD[playerid], 1);
	PlayerTextDrawAlignment(playerid, RoundStyleTotalDmgTD[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, RoundStyleTotalDmgTD[playerid], 0);

	RoundBulletproofDmgTD[playerid] = CreatePlayerTextDraw(playerid, 3.000000, 385.000000, "_");
	PlayerTextDrawFont(playerid, RoundBulletproofDmgTD[playerid], 1);
	PlayerTextDrawLetterSize(playerid, RoundBulletproofDmgTD[playerid], 0.200000, 0.900000);
	PlayerTextDrawBackgroundColor(playerid, RoundBulletproofDmgTD[playerid], MAIN_BACKGROUND_COLOUR);
	PlayerTextDrawColor(playerid, RoundBulletproofDmgTD[playerid], -1);
	PlayerTextDrawSetOutline(playerid, RoundBulletproofDmgTD[playerid], 1);
	PlayerTextDrawSetProportional(playerid, RoundBulletproofDmgTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, RoundBulletproofDmgTD[playerid], 0);
	PlayerTextDrawAlignment(playerid, RoundBulletproofDmgTD[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, RoundBulletproofDmgTD[playerid], 0);

	RoundTeamStatusDmgTD[playerid] = CreatePlayerTextDraw(playerid, 6.000000, 379.000000, "_");
	PlayerTextDrawFont(playerid, RoundTeamStatusDmgTD[playerid], 1);
	PlayerTextDrawLetterSize(playerid, RoundTeamStatusDmgTD[playerid], 0.200000, 0.900000);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamStatusDmgTD[playerid], 0x00000077);
	PlayerTextDrawColor(playerid, RoundTeamStatusDmgTD[playerid], 0xE1E1E1FF);
	PlayerTextDrawSetOutline(playerid, RoundTeamStatusDmgTD[playerid], 0);
	PlayerTextDrawSetProportional(playerid, RoundTeamStatusDmgTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, RoundTeamStatusDmgTD[playerid], 1);
	PlayerTextDrawTextSize(playerid, RoundTeamStatusDmgTD[playerid], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, RoundTeamStatusDmgTD[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, RoundTeamStatusDmgTD[playerid], 0);

	RoundDamageTD[playerid] = CreatePlayerTextDraw(playerid, 548.000000, 25.000000, "_");
	PlayerTextDrawFont(playerid, RoundDamageTD[playerid], 1);
	PlayerTextDrawLetterSize(playerid, RoundDamageTD[playerid], 0.230000, 0.999000);
	PlayerTextDrawBackgroundColor(playerid, RoundDamageTD[playerid], 150);
	PlayerTextDrawColor(playerid, RoundDamageTD[playerid], -1);
	PlayerTextDrawSetOutline(playerid, RoundDamageTD[playerid], 1);
	PlayerTextDrawSetProportional(playerid, RoundDamageTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, RoundDamageTD[playerid], 1);
	PlayerTextDrawAlignment(playerid, RoundDamageTD[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, RoundDamageTD[playerid], 0);

	RoundTotalDamageTD[playerid] = CreatePlayerTextDraw(playerid, 548.000000, 34.000000, "_");
	PlayerTextDrawFont(playerid, RoundTotalDamageTD[playerid], 1);
	PlayerTextDrawLetterSize(playerid, RoundTotalDamageTD[playerid], 0.230000, 0.999000);
	PlayerTextDrawBackgroundColor(playerid, RoundTotalDamageTD[playerid], 150);
	PlayerTextDrawColor(playerid, RoundTotalDamageTD[playerid], -1);
	PlayerTextDrawSetOutline(playerid, RoundTotalDamageTD[playerid], 1);
	PlayerTextDrawSetProportional(playerid, RoundTotalDamageTD[playerid], 1);
	PlayerTextDrawSetShadow(playerid, RoundTotalDamageTD[playerid], 1);
	PlayerTextDrawAlignment(playerid, RoundTotalDamageTD[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, RoundTotalDamageTD[playerid], 0);



	// Create visual damage textdraws
	new Float: y					= 340.0;

	// Friendly fire textdraws
	FriendlyDamage[playerid][0] = CreatePlayerTextDraw(playerid, 170.0, y, "_");
	PlayerTextDrawFont(playerid, FriendlyDamage[playerid][0], 1);
	PlayerTextDrawLetterSize(playerid, FriendlyDamage[playerid][0], 0.18000, 0.9);
	PlayerTextDrawBackgroundColor(playerid, FriendlyDamage[playerid][0], MAIN_BACKGROUND_COLOUR);
	PlayerTextDrawColor(playerid, FriendlyDamage[playerid][0], 16727295);
	PlayerTextDrawSetProportional(playerid, FriendlyDamage[playerid][0], 1);
	PlayerTextDrawSetOutline(playerid, FriendlyDamage[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, FriendlyDamage[playerid][0], 0);

	FriendlyDamage[playerid][1] = CreatePlayerTextDraw(playerid, 380.0, y, "_");
	PlayerTextDrawFont(playerid, FriendlyDamage[playerid][1], 1);
	PlayerTextDrawLetterSize(playerid, FriendlyDamage[playerid][1], 0.18000, 0.9);
	PlayerTextDrawBackgroundColor(playerid, FriendlyDamage[playerid][1], MAIN_BACKGROUND_COLOUR);
	PlayerTextDrawColor(playerid, FriendlyDamage[playerid][1], 16727295);
	PlayerTextDrawSetProportional(playerid, FriendlyDamage[playerid][1], 1);
	PlayerTextDrawSetOutline(playerid, FriendlyDamage[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, FriendlyDamage[playerid][1], 0);

	PlayerTextDrawShow(playerid, FriendlyDamage[playerid][0]);
	PlayerTextDrawShow(playerid, FriendlyDamage[playerid][1]);

	WhoSpecHeader[playerid] = CreatePlayerTextDraw(playerid, 540.241, 360.524, "_");
	PlayerTextDrawLetterSize(playerid, WhoSpecHeader[playerid], 0.1780, 0.9956);
	PlayerTextDrawTextSize(playerid, WhoSpecHeader[playerid], 644.533, 930.000);
	PlayerTextDrawAlignment(playerid, WhoSpecHeader[playerid], 1);
	PlayerTextDrawColor(playerid, WhoSpecHeader[playerid], 0xD22D3FFF);
	PlayerTextDrawSetShadow(playerid, WhoSpecHeader[playerid], 0);
	PlayerTextDrawSetOutline(playerid, WhoSpecHeader[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, WhoSpecHeader[playerid], 0x000000FF);
	PlayerTextDrawFont(playerid, WhoSpecHeader[playerid], 1);
	PlayerTextDrawSetProportional(playerid, WhoSpecHeader[playerid], 1);

	WhoSpec[playerid] = CreatePlayerTextDraw(playerid, 539.441, 376.121, "_");
	PlayerTextDrawLetterSize(playerid, WhoSpec[playerid], 0.1814, 0.9218);
	PlayerTextDrawTextSize(playerid, WhoSpec[playerid], 644.667, 930.000);
	PlayerTextDrawAlignment(playerid, WhoSpec[playerid], 1);
	PlayerTextDrawColor(playerid, WhoSpec[playerid], 0xD22D3FFF);
	PlayerTextDrawSetShadow(playerid, WhoSpec[playerid], 0);
	PlayerTextDrawSetOutline(playerid, WhoSpec[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, WhoSpec[playerid], 0x000000FF);
	PlayerTextDrawFont(playerid, WhoSpec[playerid], 1);
	PlayerTextDrawSetProportional(playerid, WhoSpec[playerid], 1);

	// Spectator HUD dynamic textdraws
	// 0: Player Name (centered above card)
	SpecText[playerid][0] = CreatePlayerTextDraw(playerid, 55.402, 329.214, "_");
	PlayerTextDrawLetterSize(playerid, SpecText[playerid][0], 0.1771, 0.9218);
	PlayerTextDrawTextSize(playerid, SpecText[playerid][0], 643.538, 930.000);
	PlayerTextDrawAlignment(playerid, SpecText[playerid][0], 2);
	PlayerTextDrawColor(playerid, SpecText[playerid][0], -1);
	PlayerTextDrawSetShadow(playerid, SpecText[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, SpecText[playerid][0], 1);
	PlayerTextDrawBackgroundColor(playerid, SpecText[playerid][0], 0x000000FF);
	PlayerTextDrawFont(playerid, SpecText[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, SpecText[playerid][0], 1);

	// 1: Health Active Bar
	SpecText[playerid][1] = CreatePlayerTextDraw(playerid, 39.140, 356.666, "_");
	PlayerTextDrawLetterSize(playerid, SpecText[playerid][1], 0.1452, 0.5144);
	PlayerTextDrawTextSize(playerid, SpecText[playerid][1], 95.927, 930.000);
	PlayerTextDrawAlignment(playerid, SpecText[playerid][1], 1);
	PlayerTextDrawColor(playerid, SpecText[playerid][1], -1);
	PlayerTextDrawUseBox(playerid, SpecText[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid, SpecText[playerid][1], 0x47893BFF);
	PlayerTextDrawSetShadow(playerid, SpecText[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, SpecText[playerid][1], 0);
	PlayerTextDrawBackgroundColor(playerid, SpecText[playerid][1], 0x000000FF);
	PlayerTextDrawFont(playerid, SpecText[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, SpecText[playerid][1], 1);

	// 2: Health Percentage Text
	SpecText[playerid][2] = CreatePlayerTextDraw(playerid, 67.534, 355.500, "_");
	PlayerTextDrawLetterSize(playerid, SpecText[playerid][2], 0.1350, 0.6500);
	PlayerTextDrawTextSize(playerid, SpecText[playerid][2], 642.800, 930.000);
	PlayerTextDrawAlignment(playerid, SpecText[playerid][2], 2);
	PlayerTextDrawColor(playerid, SpecText[playerid][2], -1);
	PlayerTextDrawSetShadow(playerid, SpecText[playerid][2], 0);
	PlayerTextDrawSetOutline(playerid, SpecText[playerid][2], 1);
	PlayerTextDrawBackgroundColor(playerid, SpecText[playerid][2], 0x000000FF);
	PlayerTextDrawFont(playerid, SpecText[playerid][2], 1);
	PlayerTextDrawSetProportional(playerid, SpecText[playerid][2], 1);

	// 3: Armour Active Bar
	SpecText[playerid][3] = CreatePlayerTextDraw(playerid, 39.140, 343.434, "_");
	PlayerTextDrawLetterSize(playerid, SpecText[playerid][3], 0.1452, 0.5144);
	PlayerTextDrawTextSize(playerid, SpecText[playerid][3], 95.927, 930.000);
	PlayerTextDrawAlignment(playerid, SpecText[playerid][3], 1);
	PlayerTextDrawColor(playerid, SpecText[playerid][3], -1);
	PlayerTextDrawUseBox(playerid, SpecText[playerid][3], 1);
	PlayerTextDrawBoxColor(playerid, SpecText[playerid][3], -1);
	PlayerTextDrawSetShadow(playerid, SpecText[playerid][3], 0);
	PlayerTextDrawSetOutline(playerid, SpecText[playerid][3], 0);
	PlayerTextDrawBackgroundColor(playerid, SpecText[playerid][3], 0x000000FF);
	PlayerTextDrawFont(playerid, SpecText[playerid][3], 1);
	PlayerTextDrawSetProportional(playerid, SpecText[playerid][3], 1);

	// 4: Armour Percentage Text
	SpecText[playerid][4] = CreatePlayerTextDraw(playerid, 67.534, 342.300, "_");
	PlayerTextDrawLetterSize(playerid, SpecText[playerid][4], 0.1350, 0.6500);
	PlayerTextDrawTextSize(playerid, SpecText[playerid][4], 642.667, 930.000);
	PlayerTextDrawAlignment(playerid, SpecText[playerid][4], 2);
	PlayerTextDrawColor(playerid, SpecText[playerid][4], -1);
	PlayerTextDrawSetShadow(playerid, SpecText[playerid][4], 0);
	PlayerTextDrawSetOutline(playerid, SpecText[playerid][4], 1);
	PlayerTextDrawBackgroundColor(playerid, SpecText[playerid][4], 0x000000FF);
	PlayerTextDrawFont(playerid, SpecText[playerid][4], 1);
	PlayerTextDrawSetProportional(playerid, SpecText[playerid][4], 1);

	// 5: FPS / Ping / Packetloss
	SpecText[playerid][5] = CreatePlayerTextDraw(playerid, 10.218, 368.575, "_");
	PlayerTextDrawLetterSize(playerid, SpecText[playerid][5], 0.1439, 0.7559);
	PlayerTextDrawTextSize(playerid, SpecText[playerid][5], 637.133, 930.000);
	PlayerTextDrawAlignment(playerid, SpecText[playerid][5], 1);
	PlayerTextDrawColor(playerid, SpecText[playerid][5], -1);
	PlayerTextDrawSetShadow(playerid, SpecText[playerid][5], 0);
	PlayerTextDrawSetOutline(playerid, SpecText[playerid][5], 1);
	PlayerTextDrawBackgroundColor(playerid, SpecText[playerid][5], 0x000000FF);
	PlayerTextDrawFont(playerid, SpecText[playerid][5], 1);
	PlayerTextDrawSetProportional(playerid, SpecText[playerid][5], 1);

	// 6: Kills / Damage / Total / Weapons
	SpecText[playerid][6] = CreatePlayerTextDraw(playerid, 10.890, 391.269, "_");
	PlayerTextDrawLetterSize(playerid, SpecText[playerid][6], 0.1297, 0.7697);
	PlayerTextDrawTextSize(playerid, SpecText[playerid][6], 642.327, 930.000);
	PlayerTextDrawAlignment(playerid, SpecText[playerid][6], 1);
	PlayerTextDrawColor(playerid, SpecText[playerid][6], -1);
	PlayerTextDrawSetShadow(playerid, SpecText[playerid][6], 0);
	PlayerTextDrawSetOutline(playerid, SpecText[playerid][6], 1);
	PlayerTextDrawBackgroundColor(playerid, SpecText[playerid][6], 0x000000FF);
	PlayerTextDrawFont(playerid, SpecText[playerid][6], 1);
	PlayerTextDrawSetProportional(playerid, SpecText[playerid][6], 1);

	// Spectator HUD Round Header: "=- Round -="
	Spec_RoundHeader[playerid] = CreatePlayerTextDraw(playerid, 10.763, 380.559, "_");
	PlayerTextDrawLetterSize(playerid, Spec_RoundHeader[playerid], 0.1423, 0.7375);
	PlayerTextDrawTextSize(playerid, Spec_RoundHeader[playerid], 642.067, 930.000);
	PlayerTextDrawAlignment(playerid, Spec_RoundHeader[playerid], 1);
	PlayerTextDrawColor(playerid, Spec_RoundHeader[playerid], -1);
	PlayerTextDrawSetShadow(playerid, Spec_RoundHeader[playerid], 0);
	PlayerTextDrawSetOutline(playerid, Spec_RoundHeader[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, Spec_RoundHeader[playerid], 0x000000FF);
	PlayerTextDrawFont(playerid, Spec_RoundHeader[playerid], 1);
	PlayerTextDrawSetProportional(playerid, Spec_RoundHeader[playerid], 1);

	AreaCheckTD[playerid] = CreatePlayerTextDraw(playerid, 322.293, 217.134, "_");
	PlayerTextDrawLetterSize(playerid, AreaCheckTD[playerid], 0.6000, 3.4227);
	PlayerTextDrawAlignment(playerid, AreaCheckTD[playerid], 2);
	PlayerTextDrawColor(playerid, AreaCheckTD[playerid], -1);
	PlayerTextDrawSetShadow(playerid, AreaCheckTD[playerid], 0);
	PlayerTextDrawSetOutline(playerid, AreaCheckTD[playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, AreaCheckTD[playerid], 0x000000FF);
	PlayerTextDrawFont(playerid, AreaCheckTD[playerid], 2);
	PlayerTextDrawSetProportional(playerid, AreaCheckTD[playerid], 1);

	AreaCheckBG[playerid] = CreatePlayerTextDraw(playerid, -1.783, 1.659, "_");
	PlayerTextDrawLetterSize(playerid, AreaCheckBG[playerid], 9.4502, 50.9658);
	PlayerTextDrawTextSize(playerid, AreaCheckBG[playerid], 640.000, 930.000);
	PlayerTextDrawAlignment(playerid, AreaCheckBG[playerid], 1);
	PlayerTextDrawColor(playerid, AreaCheckBG[playerid], -1);
	PlayerTextDrawUseBox(playerid, AreaCheckBG[playerid], 1);
	PlayerTextDrawBoxColor(playerid, AreaCheckBG[playerid], 0x00000044);
	PlayerTextDrawSetShadow(playerid, AreaCheckBG[playerid], 1);
	PlayerTextDrawSetOutline(playerid, AreaCheckBG[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, AreaCheckBG[playerid], 255);
	PlayerTextDrawFont(playerid, AreaCheckBG[playerid], 1);
	PlayerTextDrawSetProportional(playerid, AreaCheckBG[playerid], 1);

	DeathTextBG[playerid] = CreatePlayerTextDraw(playerid, 270.369, 326.487, "LD_POKE:cd1s");
	PlayerTextDrawTextSize(playerid, DeathTextBG[playerid], 95.525, 18.638);
	PlayerTextDrawAlignment(playerid, DeathTextBG[playerid], 1);
	PlayerTextDrawColor(playerid, DeathTextBG[playerid], 0x00000043);
	PlayerTextDrawSetShadow(playerid, DeathTextBG[playerid], 1);
	PlayerTextDrawSetOutline(playerid, DeathTextBG[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, DeathTextBG[playerid], 0x000000FF);
	PlayerTextDrawFont(playerid, DeathTextBG[playerid], 4);
	PlayerTextDrawSetProportional(playerid, DeathTextBG[playerid], 1);

	DeathText[playerid][0] = CreatePlayerTextDraw(playerid, 317.996, 330.018, "You Killed ~r~[NeC]R26B");
	PlayerTextDrawLetterSize(playerid, DeathText[playerid][0], 0.2130, 1.0370);
	PlayerTextDrawTextSize(playerid, DeathText[playerid][0], 122.215, 122.215);
	PlayerTextDrawAlignment(playerid, DeathText[playerid][0], 2);
	PlayerTextDrawColor(playerid, DeathText[playerid][0], -1);
	PlayerTextDrawUseBox(playerid, DeathText[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, DeathText[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, DeathText[playerid][0], 1);
	PlayerTextDrawBackgroundColor(playerid, DeathText[playerid][0], 255);
	PlayerTextDrawFont(playerid, DeathText[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, DeathText[playerid][0], 1);

	DeathText[playerid][1] = CreatePlayerTextDraw(playerid, 317.996, 330.018, "Killed By ~r~[NeC]R26B");
	PlayerTextDrawLetterSize(playerid, DeathText[playerid][1], 0.2130, 1.0370);
	PlayerTextDrawTextSize(playerid, DeathText[playerid][1], 122.215, 122.215);
	PlayerTextDrawAlignment(playerid, DeathText[playerid][1], 2);
	PlayerTextDrawColor(playerid, DeathText[playerid][1], -1);
	PlayerTextDrawUseBox(playerid, DeathText[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, DeathText[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, DeathText[playerid][1], 1);
	PlayerTextDrawBackgroundColor(playerid, DeathText[playerid][1], 255);
	PlayerTextDrawFont(playerid, DeathText[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, DeathText[playerid][1], 1);



	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background] = CreatePlayerTextDraw(playerid, -1.0, 435.0, "_");
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], 1);
	PlayerTextDrawLetterSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], 0.3, 1.5);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], 255);
	PlayerTextDrawUseBox(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], true);
	PlayerTextDrawTextSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], 641.0, 0.0);
	PlayerTextDrawBoxColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], 150);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], 255);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Background], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar] = CreatePlayerTextDraw(
		playerid, ROUND_TEAM_HP_HUD_BAR_LEFT_X, ROUND_TEAM_HP_HUD_BAR_Y, "_");
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar], 1);
	PlayerTextDrawLetterSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar], 0.0, 1.289);
	PlayerTextDrawTextSize(
		playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar],
		ROUND_TEAM_HP_HUD_BAR_CENTER_X, -41.0);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar], -256);
	PlayerTextDrawUseBox(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar], true);
	PlayerTextDrawBoxColor(
		playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar],
		ROUND_TEAM_HP_HUD_ATTACKER_COLOR);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar], 255);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar] = CreatePlayerTextDraw(
		playerid, ROUND_TEAM_HP_HUD_BAR_RIGHT_X - ROUND_TEAM_HP_HUD_BAR_WIDTH, ROUND_TEAM_HP_HUD_BAR_Y, "_");
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar], 1);
	PlayerTextDrawLetterSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar], 0.0, 1.289);
	PlayerTextDrawTextSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar], ROUND_TEAM_HP_HUD_BAR_RIGHT_X, 0.0);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar], -256);
	PlayerTextDrawUseBox(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar], true);
	PlayerTextDrawBoxColor(
		playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar],
		ROUND_TEAM_HP_HUD_DEFENDER_COLOR);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar], 255);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar] = CreatePlayerTextDraw(
		playerid, ROUND_TEAM_HP_HUD_BAR_CENTER_X, ROUND_TEAM_HP_HUD_BAR_Y, "_");
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], 1);
	PlayerTextDrawLetterSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], 0.0, 1.289);
	PlayerTextDrawTextSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], ROUND_TEAM_HP_HUD_BAR_CENTER_X, 0.0);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], -256);
	PlayerTextDrawUseBox(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], true);
	PlayerTextDrawBoxColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], 0);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], 255);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar] = CreatePlayerTextDraw(
		playerid, ROUND_TEAM_HP_HUD_BAR_RIGHT_X - ROUND_TEAM_HP_HUD_BAR_WIDTH, ROUND_TEAM_HP_HUD_BAR_Y, "_");
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], 1);
	PlayerTextDrawLetterSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], 0.0, 1.289);
	PlayerTextDrawTextSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], ROUND_TEAM_HP_HUD_BAR_RIGHT_X - ROUND_TEAM_HP_HUD_BAR_WIDTH, 0.0);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], -256);
	PlayerTextDrawUseBox(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], true);
	PlayerTextDrawBoxColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], 0);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], 255);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay] = CreatePlayerTextDraw(playerid, 646.0, 435.0, "_");
	PlayerTextDrawLetterSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], 0.3, -0.2);
	PlayerTextDrawTextSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], -115.0, 0.0);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], -1);
	PlayerTextDrawUseBox(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], true);
	PlayerTextDrawBoxColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], 150);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], 1);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], 1);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], 255);
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], 0);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_BothLineOverlay], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerTeam] = CreatePlayerTextDraw(
		playerid, ROUND_TEAM_HP_HUD_ATTACKER_TEXT_X, ROUND_TEAM_HP_HUD_ATTACKER_TEXT_Y, "_");
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerTeam], 1);
	PlayerTextDrawLetterSize(
		playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerTeam],
		ROUND_TEAM_HP_HUD_ATTACKER_TEXT_LETTER_X, ROUND_TEAM_HP_HUD_ATTACKER_TEXT_LETTER_Y);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerTeam], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerTeam], -1);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerTeam], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerTeam], 1);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerTeam], 255);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerTeam], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerTeam], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center] = CreatePlayerTextDraw(
		playerid, ROUND_TEAM_HP_HUD_CENTER_TEXT_X, ROUND_TEAM_HP_HUD_CENTER_TEXT_Y, "_");
	PlayerTextDrawLetterSize(
		playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center],
		ROUND_TEAM_HP_HUD_CENTER_TEXT_LETTER_X, ROUND_TEAM_HP_HUD_CENTER_TEXT_LETTER_Y);
	PlayerTextDrawTextSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center], 36.0, 0.0);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center], -1);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center], 1);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center], 255);
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center], 1);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam] = CreatePlayerTextDraw(
		playerid, ROUND_TEAM_HP_HUD_DEFENDER_TEXT_X, ROUND_TEAM_HP_HUD_DEFENDER_TEXT_Y, "_");
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam], 1);
	PlayerTextDrawLetterSize(
		playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam],
		ROUND_TEAM_HP_HUD_DEFENDER_TEXT_LETTER_X, ROUND_TEAM_HP_HUD_DEFENDER_TEXT_LETTER_Y);
	PlayerTextDrawTextSize(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam], 1280.424, 1280.000);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam], -1);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam], 1);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam], 255);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard] = CreatePlayerTextDraw(
		playerid, ROUND_TEAM_HP_HUD_CARD_ATTACKER_X, ROUND_TEAM_HP_HUD_CARD_Y, "LD_POKE:cd1s");
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard], TEXT_DRAW_FONT_SPRITE_DRAW);
	PlayerTextDrawTextSize(
		playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard],
		ROUND_TEAM_HP_HUD_CARD_WIDTH, ROUND_TEAM_HP_HUD_CARD_HEIGHT);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard], 0x00000087);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard], 1);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard], 255);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard] = CreatePlayerTextDraw(
		playerid, ROUND_TEAM_HP_HUD_CARD_DEFENDER_X, ROUND_TEAM_HP_HUD_CARD_Y, "LD_POKE:cd1s");
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard], TEXT_DRAW_FONT_SPRITE_DRAW);
	PlayerTextDrawTextSize(
		playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard],
		ROUND_TEAM_HP_HUD_CARD_WIDTH, ROUND_TEAM_HP_HUD_CARD_HEIGHT);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard], 0x00000087);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard], 1);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard], 255);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText] = CreatePlayerTextDraw(
		playerid, ROUND_TEAM_HP_HUD_DMG_TEXT_ATTACKER_X, ROUND_TEAM_HP_HUD_DMG_TEXT_Y, "_");
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText], TEXT_DRAW_FONT_3);
	PlayerTextDrawLetterSize(
		playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText],
		ROUND_TEAM_HP_HUD_DMG_TEXT_LETTER_X, ROUND_TEAM_HP_HUD_DMG_TEXT_LETTER_Y);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText], TEXT_DRAW_ALIGN_CENTER);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText], ROUND_TEAM_HP_HUD_DMG_TEXT_COLOR);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText], 1);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText], 255);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText], false);

	RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText] = CreatePlayerTextDraw(
		playerid, ROUND_TEAM_HP_HUD_DMG_TEXT_DEFENDER_X, ROUND_TEAM_HP_HUD_DMG_TEXT_Y, "_");
	PlayerTextDrawFont(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText], TEXT_DRAW_FONT_3);
	PlayerTextDrawLetterSize(
		playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText],
		ROUND_TEAM_HP_HUD_DMG_TEXT_LETTER_X, ROUND_TEAM_HP_HUD_DMG_TEXT_LETTER_Y);
	PlayerTextDrawAlignment(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText], TEXT_DRAW_ALIGN_CENTER);
	PlayerTextDrawColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText], ROUND_TEAM_HP_HUD_DMG_TEXT_COLOR);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText], 1);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText], 255);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText], false);

	RoundTeamHPHUDReady[playerid] = true;
	RoundTeamHPHUDShown[playerid] = false;
	RoundTeamHPHUDApplied[playerid][0] = -1.0;
	RoundTeamHPHUDApplied[playerid][1] = -1.0;
	for (new eRoundTeamHPHUDText:i = RoundTeamHPHUD_Background; i <= RoundTeamHPHUD_DefenderDamageText; i++)
	{
		if (RoundTeamHPHUDText[playerid][i] == _:INVALID_PLAYER_TEXT_DRAW)
			RoundTeamHPHUDReady[playerid] = false;
	}
	if (!RoundTeamHPHUDReady[playerid])
		printf("[AttDef] Team HP Bars HUD could not be created for player %d (PlayerTextDraw limit reached).", playerid);

	RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Card] = CreatePlayerTextDraw(playerid, 264.378, 375.698, "LD_POKE:cd1s");
	PlayerTextDrawTextSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Card], 110.222, 48.340);
	PlayerTextDrawAlignment(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Card], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Card], 0x00000099);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Card], 1);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Card], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Card], 0x000000FF);
	PlayerTextDrawFont(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Card], TEXT_DRAW_FONT_SPRITE_DRAW);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Card], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Card], false);

	RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder] = CreatePlayerTextDraw(playerid, 270.250, 407.667, "_");
	PlayerTextDrawLetterSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], 0.4800, 1.0500);
	PlayerTextDrawTextSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], 365.700, 0.000);
	PlayerTextDrawAlignment(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], 0xFFFFFF00);
	PlayerTextDrawUseBox(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], true);
	PlayerTextDrawBoxColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], 0x1A0202EB);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], 0x000000FF);
	PlayerTextDrawFont(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder], false);

	RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty] = CreatePlayerTextDraw(playerid, 271.000, 408.333, "_");
	PlayerTextDrawLetterSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], 0.4800, 0.9184);
	PlayerTextDrawTextSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], 366.017, 0.000);
	PlayerTextDrawAlignment(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], 0xFFFFFF00);
	PlayerTextDrawUseBox(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], true);
	PlayerTextDrawBoxColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], 0x050000FF);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], 0x000000FF);
	PlayerTextDrawFont(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty], false);

	RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder] = CreatePlayerTextDraw(playerid, 271.000, 408.333, "_");
	PlayerTextDrawLetterSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], 0.4800, 0.9000);
	PlayerTextDrawTextSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], 366.017, 0.000);
	PlayerTextDrawAlignment(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], 0xFFFFFF00);
	PlayerTextDrawUseBox(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], true);
	PlayerTextDrawBoxColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], 0x6E0704FF);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], 0x000000FF);
	PlayerTextDrawFont(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], false);

	RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill] = CreatePlayerTextDraw(playerid, 271.750, 409.000, "_");
	PlayerTextDrawLetterSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], 0.4800, 0.7500);
	PlayerTextDrawTextSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], 365.267, 0.000);
	PlayerTextDrawAlignment(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], 0xFFFFFF00);
	PlayerTextDrawUseBox(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], true);
	PlayerTextDrawBoxColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], 0xDC0E05FF);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], 0x000000FF);
	PlayerTextDrawFont(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], false);

	RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title] = CreatePlayerTextDraw(playerid, 280.598, 379.623, "Taking Checkpoint");
	PlayerTextDrawLetterSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title], 0.2435, 0.7972);
	PlayerTextDrawTextSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title], 641.241, 930.000);
	PlayerTextDrawAlignment(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title], 0xE03828FF);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title], 1);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title], 0);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title], 0x000000FF);
	PlayerTextDrawFont(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title], TEXT_DRAW_FONT_3);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title], false);

	RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName] = CreatePlayerTextDraw(playerid, 319.000, 392.646, "_");
	PlayerTextDrawLetterSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName], 0.2272, 0.9725);
	PlayerTextDrawAlignment(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName], TEXT_DRAW_ALIGN_CENTER);
	PlayerTextDrawColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName], -1);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName], 4);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName], 1);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName], 0x000000FF);
	PlayerTextDrawFont(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName], TEXT_DRAW_FONT_3);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName], false);

	RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText] = CreatePlayerTextDraw(playerid, 295.569, 408.233, "_");
	PlayerTextDrawLetterSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText], 0.2630, 0.7810);
	PlayerTextDrawTextSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText], 677.011, 930.000);
	PlayerTextDrawAlignment(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText], -1);
	PlayerTextDrawSetShadow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText], 0);
	PlayerTextDrawSetOutline(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText], 1);
	PlayerTextDrawBackgroundColor(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText], 0x000000FF);
	PlayerTextDrawFont(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText], TEXT_DRAW_FONT_3);
	PlayerTextDrawSetProportional(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText], true);
	PlayerTextDrawSetSelectable(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText], false);

	RoundTeamHPCPReady[playerid] = true;
	RoundTeamHPCPShown[playerid] = false;
	for (new eRoundTeamHPCPHUD:i = RoundTeamHPCP_Card; i <= RoundTeamHPCP_TimeText; i++)
	{
		if (RoundTeamHPCPHUD[playerid][i] == _:INVALID_PLAYER_TEXT_DRAW)
			RoundTeamHPCPReady[playerid] = false;
	}
	if (!RoundTeamHPCPReady[playerid])
		printf("[AttDef] Team HP Bars CP HUD could not be created for player %d (PlayerTextDraw limit reached).", playerid);

	TeamIcons_WinLoss[playerid] = CreatePlayerTextDraw(playerid, 320.000, 85.000, "ROUND WIN");
	PlayerTextDrawLetterSize(playerid, TeamIcons_WinLoss[playerid], 0.5000, 2.2000);
	PlayerTextDrawTextSize(playerid, TeamIcons_WinLoss[playerid], 1280.000, 1280.000);
	PlayerTextDrawAlignment(playerid, TeamIcons_WinLoss[playerid], TEXT_DRAW_ALIGN_CENTER);
	PlayerTextDrawColor(playerid, TeamIcons_WinLoss[playerid], 0x508E42FF);
	PlayerTextDrawSetShadow(playerid, TeamIcons_WinLoss[playerid], 1);
	PlayerTextDrawSetOutline(playerid, TeamIcons_WinLoss[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, TeamIcons_WinLoss[playerid], 0x000000FF);
	PlayerTextDrawFont(playerid, TeamIcons_WinLoss[playerid], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, TeamIcons_WinLoss[playerid], true);
	return 1;
}

ConfigureRoundKillDmgTDmg(playerid)
{
	PlayerTextDrawSetPos(playerid, RoundStyleKillsTD[playerid], 3.000, 396.000);
	PlayerTextDrawSetPos(playerid, RoundStyleDamageTD[playerid], 3.000, 405.000);
	PlayerTextDrawSetPos(playerid, RoundStyleTotalDmgTD[playerid], 3.000, 414.000);
	return 1;
}


ShowRoundKillDmgTextDraws(playerid)
{
	HideRoundKillDmgTextDraws(playerid);
	if (Current == -1 || (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER))
		return 0;
	switch (Player[playerid][RoundTDStyle])
	{
		case 0, 2:
		{
			PlayerTextDrawShow(playerid, RoundStyleKillsTD[playerid]);
			PlayerTextDrawShow(playerid, RoundStyleDamageTD[playerid]);
			PlayerTextDrawShow(playerid, RoundStyleTotalDmgTD[playerid]);
		}
		case 1:
			PlayerTextDrawShow(playerid, RoundBulletproofDmgTD[playerid]);
		case 3:
			PlayerTextDrawShow(playerid, RoundTeamStatusDmgTD[playerid]);
		case 4:
		{
			PlayerTextDrawShow(playerid, RoundDamageTD[playerid]);
			PlayerTextDrawShow(playerid, RoundTotalDamageTD[playerid]);
		}
		default:
		{
			PlayerTextDrawShow(playerid, RoundStyleKillsTD[playerid]);
			PlayerTextDrawShow(playerid, RoundStyleDamageTD[playerid]);
			PlayerTextDrawShow(playerid, RoundStyleTotalDmgTD[playerid]);
		}
	}
	return 1;
}

HideRoundKillDmgTextDraws(playerid)
{
	PlayerTextDrawHide(playerid, RoundStyleKillsTD[playerid]);
	PlayerTextDrawHide(playerid, RoundStyleDamageTD[playerid]);
	PlayerTextDrawHide(playerid, RoundStyleTotalDmgTD[playerid]);
	PlayerTextDrawHide(playerid, RoundBulletproofDmgTD[playerid]);
	PlayerTextDrawHide(playerid, RoundTeamStatusDmgTD[playerid]);
	PlayerTextDrawHide(playerid, RoundDamageTD[playerid]);
	PlayerTextDrawHide(playerid, RoundTotalDamageTD[playerid]);
	return 1;
}

UpdateRoundKillDmgText(playerid)
{
	if (Current == -1 || (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER))
		return 0;
	new bulletproofString[80], teamStatusString[160], killsString[48], damageString[48], totalString[48];
	bulletproofString[0] = EOS;
	teamStatusString[0] = EOS;
	killsString[0] = EOS;
	damageString[0] = EOS;
	totalString[0] = EOS;
	PlayerTextDrawSetString(playerid, RoundStyleKillsTD[playerid], " ");
	PlayerTextDrawSetString(playerid, RoundStyleDamageTD[playerid], " ");
	PlayerTextDrawSetString(playerid, RoundStyleTotalDmgTD[playerid], " ");
	PlayerTextDrawSetString(playerid, RoundBulletproofDmgTD[playerid], " ");
	PlayerTextDrawSetString(playerid, RoundTeamStatusDmgTD[playerid], " ");
	PlayerTextDrawSetString(playerid, RoundDamageTD[playerid], " ");
	PlayerTextDrawSetString(playerid, RoundTotalDamageTD[playerid], " ");

	if (Player[playerid][RoundTDStyle] != 1
		&& Player[playerid][RoundTDStyle] != 3
		&& Player[playerid][RoundTDStyle] != 4)
	{
		new teamCol[16];
		if (Player[playerid][Team] >= 0 && Player[playerid][Team] < MAX_TEAMS && TDC[Player[playerid][Team]][0] != EOS)
			format(teamCol, sizeof(teamCol), "%s", TDC[Player[playerid][Team]]);
		else
			format(teamCol, sizeof(teamCol), "~w~");

		format(killsString, sizeof(killsString), "~w~Kills %s%d", teamCol, Player[playerid][RoundKills]);
		format(damageString, sizeof(damageString), "~w~Damage %s%d", teamCol, floatround(Player[playerid][RoundDamage]));
		format(totalString, sizeof(totalString), "~w~Total Dmg %s%d", teamCol, floatround(Player[playerid][TotalDamage]));
		PlayerTextDrawSetString(playerid, RoundStyleKillsTD[playerid], killsString);
		PlayerTextDrawSetString(playerid, RoundStyleDamageTD[playerid], damageString);
		PlayerTextDrawSetString(playerid, RoundStyleTotalDmgTD[playerid], totalString);
	}
	else if (Player[playerid][RoundTDStyle] == 1)
	{
		format(bulletproofString, sizeof(bulletproofString), "~r~Damage: ~r~~h~%d~n~~r~Total: ~r~~h~%d",
			floatround(Player[playerid][RoundDamage]), floatround(Player[playerid][TotalDamage]));
		PlayerTextDrawSetString(playerid, RoundBulletproofDmgTD[playerid], bulletproofString);
	}
	else if (Player[playerid][RoundTDStyle] == 3)
	{
		new Float: ratio;
		if (AllVsAllMode && GameType == ARENA)
		{
			if (Player[playerid][RoundDeaths] > 0)
				ratio = float(Player[playerid][RoundKills]) / float(Player[playerid][RoundDeaths]);
		}
		else if (WarMode)
		{
			if (Player[playerid][TotalDeaths] > 0)
				ratio = float(Player[playerid][TotalKills]) / float(Player[playerid][TotalDeaths]);
		}
		else
		{
			if (Player[playerid][ATimeDeaths] > 0)
				ratio = float(Player[playerid][ATimeKills]) / float(Player[playerid][ATimeDeaths]);
		}

		format(
			teamStatusString, sizeof(teamStatusString),
			"~p~Kills ~w~%d~n~~p~Dmg ~w~%d~n~~p~Total ~w~%d~n~~p~Ratio ~w~%.2f",
			Player[playerid][RoundKills], floatround(Player[playerid][RoundDamage]),
			floatround(Player[playerid][TotalDamage]), ratio);
		PlayerTextDrawSetString(playerid, RoundTeamStatusDmgTD[playerid], teamStatusString);
	}
	else if (Player[playerid][RoundTDStyle] == 4)
	{
		format(damageString, sizeof(damageString), "~r~Damage: ~r~~h~%d",
			floatround(Player[playerid][RoundDamage]));
		format(totalString, sizeof(totalString), "~r~Total: ~r~~h~%d",
			floatround(Player[playerid][TotalDamage]));
		PlayerTextDrawSetString(playerid, RoundDamageTD[playerid], damageString);
		PlayerTextDrawSetString(playerid, RoundTotalDamageTD[playerid], totalString);
	}

	return 1;
}

UpdateRoundKillDmgTDmg(playerid)
{
	if (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER)
		return 0;
	new str[400];

	UpdateRoundKillDmgText(playerid);
	format(str, sizeof(str), "UPDATE Players SET TKills = %d, TDeaths = %d, TAssists = %d, TTDMG = %d, AKWD = %d, AKWP = %d, AKWM = %d, AKWSN = %d, AKWS = %d, AKWSHT = %d, AKWAK = %d, Exp = %d, ClutchesWon = %d WHERE Name = '%q'",
		Player[playerid][ATimeKills], Player[playerid][ATimeDeaths], Player[playerid][ATimeAssists], Player[playerid][ATimeDMG],
		Player[playerid][AKillDGL], Player[playerid][AKillPNCH], Player[playerid][AKillM],
		Player[playerid][AKillSN], Player[playerid][AKillSP], Player[playerid][AKillSHT],
		Player[playerid][AKillAK], Player[playerid][Exp], Player[playerid][ClutchesWon], Player[playerid][Name]);
	new saved = DatabaseExecuteWrite(str, "save player round statistics");
	return saved;
}



stock ShowPlayerDeathNotice(playerid, type, const text[])
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	PlayerTextDrawSetString(playerid, DeathText[playerid][type], text);

	new len = 0, i = 0, l = strlen(text);
	while (i < l)
	{
		if (text[i] == '~')
		{
			new nextTilde = -1;
			for (new j = i + 1; j < l && j <= i + 6; j++)
			{
				if (text[j] == '~') { nextTilde = j; break; }
			}
			if (nextTilde != -1) { i = nextTilde + 1; continue; }
		}
		len++;
		i++;
	}

	new Float: width = 95.525;
	if (len > 18)
		width = 95.525 + float(len - 18) * 5.6;

	new Float: startX = 318.0 - (width / 2.0);

	PlayerTextDrawDestroy(playerid, DeathTextBG[playerid]);
	DeathTextBG[playerid] = CreatePlayerTextDraw(playerid, startX, 326.487, "LD_POKE:cd1s");
	PlayerTextDrawTextSize(playerid, DeathTextBG[playerid], width, 18.638);
	PlayerTextDrawAlignment(playerid, DeathTextBG[playerid], 1);
	PlayerTextDrawColor(playerid, DeathTextBG[playerid], 0x00000043);
	PlayerTextDrawSetShadow(playerid, DeathTextBG[playerid], 1);
	PlayerTextDrawSetOutline(playerid, DeathTextBG[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, DeathTextBG[playerid], 0x000000FF);
	PlayerTextDrawFont(playerid, DeathTextBG[playerid], 4);
	PlayerTextDrawSetProportional(playerid, DeathTextBG[playerid], 1);

	PlayerTextDrawShow(playerid, DeathTextBG[playerid]);
	PlayerTextDrawShow(playerid, DeathText[playerid][type]);
	return 1;
}

stock HidePlayerDeathNotice(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || !IsPlayerConnected(playerid))
		return 0;

	PlayerTextDrawHide(playerid, DeathText[playerid][0]);
	PlayerTextDrawHide(playerid, DeathText[playerid][1]);
	if (DeathTextBG[playerid] != PlayerText: INVALID_TEXT_DRAW)
		PlayerTextDrawHide(playerid, DeathTextBG[playerid]);
	return 1;
}


InitPlayerInterface(playerid)
{
	for (new i = 0; i < MAX_PLAYER_INTERFACE_ASPECTS; i++) PlayerInterface[playerid][i] = true;
}

// Toggle gamemode HUD
TogglePlayerInterface(playerid, bool: toggle, interface = -1)
{
	switch (interface)
	{
		case -1: // all
		{
			if (!DatabaseExecuteWrite("BEGIN IMMEDIATE TRANSACTION", "begin player HUD update"))
				return SendErrorMessage(playerid, "Could not save the HUD setting. No changes were applied.");
			if (!DatabaseExecuteWrite(
				va_return("UPDATE Players SET InterfaceSpec = %d, InterfaceNet = %d, InterfaceHP = %d WHERE Name = '%q'", toggle, toggle, toggle, Player[playerid][Name]),
				"save player HUD settings")
				|| !DatabaseExecuteWrite("COMMIT", "commit player HUD update"))
			{
				DatabaseExecuteWrite("ROLLBACK", "rollback player HUD update");
				return SendErrorMessage(playerid, "Could not save the HUD setting. No changes were applied.");
			}

			PlayerInterface[playerid][INTERFACE_SPEC] = toggle;
			PlayerInterface[playerid][INTERFACE_NET]  = toggle;
			PlayerInterface[playerid][INTERFACE_HP]	  = toggle;
			if (toggle)
			{
				PlayerTextDrawShow(playerid, WhoSpecHeader[playerid]);
				PlayerTextDrawShow(playerid, WhoSpec[playerid]);
				if (Player[playerid][Team] != NON)
				{
					PlayerTextDrawShow(playerid, FPSPingPacketBox[playerid]);
					PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);
				}
				else
				{
					PlayerTextDrawHide(playerid, FPSPingPacketBox[playerid]);
					PlayerTextDrawHide(playerid, FPSPingPacket[playerid]);
				}
				UpdatePlayerHealthBar(playerid);
			}
			else
			{
				PlayerTextDrawHide(playerid, WhoSpecHeader[playerid]);
				PlayerTextDrawHide(playerid, WhoSpec[playerid]);
				PlayerTextDrawHide(playerid, FPSPingPacketBox[playerid]);
				PlayerTextDrawHide(playerid, FPSPingPacket[playerid]);
				UpdatePlayerHealthBar(playerid);
			}
		}
		case INTERFACE_SPEC:
		{
			if (!DatabaseExecuteWrite(
				va_return("UPDATE Players SET InterfaceSpec = %d WHERE Name = '%q'", toggle, Player[playerid][Name]),
				"save player spectator HUD setting"))
				return SendErrorMessage(playerid, "Could not save the HUD setting. No changes were applied.");
			PlayerInterface[playerid][INTERFACE_SPEC] = toggle;
			if (toggle)
			{
				PlayerTextDrawShow(playerid, WhoSpecHeader[playerid]);
				PlayerTextDrawShow(playerid, WhoSpec[playerid]);
			}
			else
			{
				PlayerTextDrawHide(playerid, WhoSpecHeader[playerid]);
				PlayerTextDrawHide(playerid, WhoSpec[playerid]);
			}
		}
		case INTERFACE_NET:
		{
			if (!DatabaseExecuteWrite(
				va_return("UPDATE Players SET InterfaceNet = %d WHERE Name = '%q'", toggle, Player[playerid][Name]),
				"save player network HUD setting"))
				return SendErrorMessage(playerid, "Could not save the HUD setting. No changes were applied.");
			PlayerInterface[playerid][INTERFACE_NET] = toggle;
			if (toggle && Player[playerid][Team] != NON)
			{
				PlayerTextDrawShow(playerid, FPSPingPacketBox[playerid]);
				PlayerTextDrawShow(playerid, FPSPingPacket[playerid]);
			}
			else
			{
				PlayerTextDrawHide(playerid, FPSPingPacketBox[playerid]);
				PlayerTextDrawHide(playerid, FPSPingPacket[playerid]);
			}
		}
		case INTERFACE_HP:
		{
			if (!DatabaseExecuteWrite(
				va_return("UPDATE Players SET InterfaceHP = %d WHERE Name = '%q'", toggle, Player[playerid][Name]),
				"save player health HUD setting"))
				return SendErrorMessage(playerid, "Could not save the HUD setting. No changes were applied.");
			PlayerInterface[playerid][INTERFACE_HP] = toggle;
			UpdatePlayerHealthBar(playerid);
		}
		default: return SendErrorMessage(playerid, "Invalid HUD aspect.");
	}
	UpdateRoundTeamStatusHUD();
	return 1;
}
