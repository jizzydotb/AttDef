#if defined _attdef_td_styles
	#endinput
#endif
#define _attdef_td_styles

/*
	Round HUD Styles (Modern, Simple, Bulletproof, Team Status, HP Bars) Sub-module
*/

RoundTextdrawsCreate()
{
	leftRedBG = TextDrawCreate(335.540924, 401.283630, "-");
	TextDrawLetterSize(leftRedBG, -28.081052, 7.287504);
	TextDrawAlignment(leftRedBG, 1);
	TextDrawColor(leftRedBG, 0x00000011);
	TextDrawSetShadow(leftRedBG, -1);
	TextDrawSetOutline(leftRedBG, 0);
	TextDrawBackgroundColor(leftRedBG, 0xDE000033);
	TextDrawFont(leftRedBG, 1);
	TextDrawSetProportional(leftRedBG, 1);

	rightBlueBG = TextDrawCreate(311.741302, 401.283630, "-");
	TextDrawLetterSize(rightBlueBG, 28.770492, 7.287504);
	TextDrawAlignment(rightBlueBG, 1);
	TextDrawColor(rightBlueBG, 0x00000011);
	TextDrawSetShadow(rightBlueBG, -1);
	TextDrawSetOutline(rightBlueBG, 0);
	TextDrawBackgroundColor(rightBlueBG, 0x3344FF33);
	TextDrawFont(rightBlueBG, 1);
	TextDrawSetProportional(rightBlueBG, 1);

	timerCenterTD = TextDrawCreate(324.140991, 434.872528, "~w~0:00 / ~r~~h~00");
	TextDrawLetterSize(timerCenterTD, 0.24, 1.34);
	TextDrawTextSize(timerCenterTD, 299.293579, 98.583343);
	TextDrawAlignment(timerCenterTD, 2);
	TextDrawColor(timerCenterTD, -1);
	TextDrawSetShadow(timerCenterTD, 0);
	TextDrawSetOutline(timerCenterTD, 1);
	TextDrawBackgroundColor(timerCenterTD, MAIN_BACKGROUND_COLOUR);
	TextDrawFont(timerCenterTD, 2);
	TextDrawSetProportional(timerCenterTD, 1);

	timerCircleTD = TextDrawCreate(281.000000, 509.000000, "ld_pool:ball");
	TextDrawBackgroundColor(timerCircleTD, 255);
	TextDrawFont(timerCircleTD, 4);
	TextDrawLetterSize(timerCircleTD, -0.039999, 18.500000);
	TextDrawColor(timerCircleTD, 0x00000033);
	TextDrawSetOutline(timerCircleTD, 1);
	TextDrawSetProportional(timerCircleTD, 0);
	TextDrawUseBox(timerCircleTD, 1);
	TextDrawBoxColor(timerCircleTD, -1);
	TextDrawTextSize(timerCircleTD, 88.000000, -82.000000);

	leftTeamData = TextDrawCreate(255.400375, 435.060705, "~w~_");
	TextDrawLetterSize(leftTeamData, 0.22, 1.22);
	TextDrawAlignment(leftTeamData, 2);
	TextDrawColor(leftTeamData, -1);
	TextDrawSetShadow(leftTeamData, 0);
	TextDrawSetOutline(leftTeamData, -1);
	TextDrawBackgroundColor(leftTeamData, 0xDE000011);
	TextDrawFont(leftTeamData, 1);
	TextDrawSetProportional(leftTeamData, 1);

	rightTeamData = TextDrawCreate(387.682586, 435.050573, "~w~_");
	TextDrawLetterSize(rightTeamData, 0.22, 1.22);
	TextDrawAlignment(rightTeamData, 2);
	TextDrawColor(rightTeamData, -1);
	TextDrawSetShadow(rightTeamData, 0);
	TextDrawSetOutline(rightTeamData, -1);
	TextDrawBackgroundColor(rightTeamData, 0x3344FF11);
	TextDrawFont(rightTeamData, 1);
	TextDrawSetProportional(rightTeamData, 1);

	//Qwerty _______________________________________________________________ ~b~~h~~h~Asdfg");
	centerTeamNames = TextDrawCreate(321.0, 425.776257, "~r~~h~~h~__- __ -");
	TextDrawLetterSize(centerTeamNames, 0.200000, 2.000000);
	TextDrawAlignment(centerTeamNames, 2);
	TextDrawColor(centerTeamNames, -1);
	TextDrawSetShadow(centerTeamNames, 0);
	TextDrawSetOutline(centerTeamNames, 1);
	TextDrawBackgroundColor(centerTeamNames, 0x00000033);
	TextDrawFont(centerTeamNames, 1);
	TextDrawSetProportional(centerTeamNames, 1);

	attackerNameTD = TextDrawCreate(210.000000, 428.000000, "~r~~h~~h~__- __ -");
	TextDrawLetterSize(attackerNameTD, 0.200000, 2.000000);
	TextDrawAlignment(attackerNameTD, 2);
	TextDrawColor(attackerNameTD, -1);
	TextDrawSetShadow(attackerNameTD, 0);
	TextDrawSetOutline(attackerNameTD, 1);
	TextDrawBackgroundColor(attackerNameTD, MAIN_BACKGROUND_COLOUR);
	TextDrawFont(attackerNameTD, 1);
	TextDrawSetProportional(attackerNameTD, 1);

	defenderNameTD = TextDrawCreate(428.000000, 428.000000, "~r~~h~~h~__- __ -");
	TextDrawLetterSize(defenderNameTD, 0.200000, 2.000000);
	TextDrawAlignment(defenderNameTD, 2);
	TextDrawColor(defenderNameTD, -1);
	TextDrawSetShadow(defenderNameTD, 0);
	TextDrawSetOutline(defenderNameTD, 1);
	TextDrawBackgroundColor(defenderNameTD, MAIN_BACKGROUND_COLOUR);
	TextDrawFont(defenderNameTD, 1);
	TextDrawSetProportional(defenderNameTD, 1);

	PauseTD = TextDrawCreate(152.030, 292.002, "_");
	TextDrawLetterSize(PauseTD, 0.2498, 0.9410);
	TextDrawTextSize(PauseTD, 642.694, 930.000);
	TextDrawAlignment(PauseTD, 1);
	TextDrawColor(PauseTD, -1);
	TextDrawSetShadow(PauseTD, 0);
	TextDrawSetOutline(PauseTD, 1);
	TextDrawBackgroundColor(PauseTD, 0x000000FF);
	TextDrawFont(PauseTD, 1);
	TextDrawSetProportional(PauseTD, 1);

	AttDef_RoundStats = TextDrawCreate(318.0, 431.5, "_");
	TextDrawUseBox(AttDef_RoundStats, 1);
	TextDrawBoxColor(AttDef_RoundStats, 0x0000022);
	TextDrawFont(AttDef_RoundStats, 1);
	TextDrawTextSize(AttDef_RoundStats, 14.0, 640.0);
	TextDrawLetterSize(AttDef_RoundStats, 0.31, 1.55);
	TextDrawBackgroundColor(AttDef_RoundStats, MAIN_BACKGROUND_COLOUR);
	TextDrawColor(AttDef_RoundStats, -65281);
	TextDrawSetOutline(AttDef_RoundStats, 1);
	TextDrawSetShadow(AttDef_RoundStats, 0);
	TextDrawAlignment(AttDef_RoundStats, 2);
	TextDrawSetProportional(AttDef_RoundStats, 1);

	AttDef_RoundTimer = TextDrawCreate(327.000000, 431.500000, "_");
	TextDrawAlignment(AttDef_RoundTimer, 2);
	TextDrawBackgroundColor(AttDef_RoundTimer, MAIN_BACKGROUND_COLOUR);
	TextDrawFont(AttDef_RoundTimer, 1);
	TextDrawLetterSize(AttDef_RoundTimer, 0.31, 1.55);
	TextDrawColor(AttDef_RoundTimer, -1);
	TextDrawSetOutline(AttDef_RoundTimer, 1);
	TextDrawSetProportional(AttDef_RoundTimer, 1);

	ModernRound_BottomBar = TextDrawCreate(-2.000, 432.500, "_");
	TextDrawLetterSize(ModernRound_BottomBar, 0.1599, 2.8000);
	TextDrawTextSize(ModernRound_BottomBar, 642.000, 0.000);
	TextDrawAlignment(ModernRound_BottomBar, 1);
	TextDrawColour(ModernRound_BottomBar, -1);
	TextDrawUseBox(ModernRound_BottomBar, true);
	TextDrawBoxColour(ModernRound_BottomBar, 0x00000092);
	TextDrawSetShadow(ModernRound_BottomBar, 0);
	TextDrawSetOutline(ModernRound_BottomBar, 0);
	TextDrawBackgroundColour(ModernRound_BottomBar, 0x000000FF);
	TextDrawFont(ModernRound_BottomBar, 1);
	TextDrawSetProportional(ModernRound_BottomBar, true);

	ModernRound_Timer = TextDrawCreate(320.000, 433.000, "0:00");
	TextDrawLetterSize(ModernRound_Timer, 0.2251, 1.2883);
	TextDrawAlignment(ModernRound_Timer, 2);
	TextDrawColour(ModernRound_Timer, -1);
	TextDrawSetShadow(ModernRound_Timer, 0);
	TextDrawSetOutline(ModernRound_Timer, 1);
	TextDrawBackgroundColour(ModernRound_Timer, 0x000000FF);
	TextDrawFont(ModernRound_Timer, 1);
	TextDrawSetProportional(ModernRound_Timer, true);

	ModernRound_AttTeamName = TextDrawCreate(210.000, 433.000, "Attacker");
	TextDrawLetterSize(ModernRound_AttTeamName, 0.2251, 1.2883);
	TextDrawAlignment(ModernRound_AttTeamName, 2);
	TextDrawColour(ModernRound_AttTeamName, 0x2ED955FF);
	TextDrawSetShadow(ModernRound_AttTeamName, 0);
	TextDrawSetOutline(ModernRound_AttTeamName, 1);
	TextDrawBackgroundColour(ModernRound_AttTeamName, 0x000000FF);
	TextDrawFont(ModernRound_AttTeamName, 1);
	TextDrawSetProportional(ModernRound_AttTeamName, true);

	ModernRound_DefTeamName = TextDrawCreate(430.000, 433.000, "Defender");
	TextDrawLetterSize(ModernRound_DefTeamName, 0.2251, 1.2883);
	TextDrawAlignment(ModernRound_DefTeamName, 2);
	TextDrawColour(ModernRound_DefTeamName, 0x3D82FFFF);
	TextDrawSetShadow(ModernRound_DefTeamName, 0);
	TextDrawSetOutline(ModernRound_DefTeamName, 1);
	TextDrawBackgroundColour(ModernRound_DefTeamName, 0x000000FF);
	TextDrawFont(ModernRound_DefTeamName, 1);
	TextDrawSetProportional(ModernRound_DefTeamName, true);

	ModernRound_AttHeart = TextDrawCreate(110.000, 435.000, "hud:radar_girlfriend");
	TextDrawTextSize(ModernRound_AttHeart, 10.500, 10.500);
	TextDrawAlignment(ModernRound_AttHeart, 1);
	TextDrawColour(ModernRound_AttHeart, -1);
	TextDrawSetShadow(ModernRound_AttHeart, 1);
	TextDrawSetOutline(ModernRound_AttHeart, 0);
	TextDrawBackgroundColour(ModernRound_AttHeart, 0x000000FF);
	TextDrawFont(ModernRound_AttHeart, 4);
	TextDrawSetProportional(ModernRound_AttHeart, true);

	ModernRound_DefHeart = TextDrawCreate(519.500, 435.000, "hud:radar_girlfriend");
	TextDrawTextSize(ModernRound_DefHeart, 10.500, 10.500);
	TextDrawAlignment(ModernRound_DefHeart, 1);
	TextDrawColour(ModernRound_DefHeart, -1);
	TextDrawSetShadow(ModernRound_DefHeart, 1);
	TextDrawSetOutline(ModernRound_DefHeart, 0);
	TextDrawBackgroundColour(ModernRound_DefHeart, 0x000000FF);
	TextDrawFont(ModernRound_DefHeart, 4);
	TextDrawSetProportional(ModernRound_DefHeart, true);

	ModernRound_AttHP = TextDrawCreate(124.000, 433.000, "200 [1]");
	TextDrawLetterSize(ModernRound_AttHP, 0.2251, 1.2883);
	TextDrawAlignment(ModernRound_AttHP, 1);
	TextDrawColour(ModernRound_AttHP, -1);
	TextDrawSetShadow(ModernRound_AttHP, 0);
	TextDrawSetOutline(ModernRound_AttHP, 1);
	TextDrawBackgroundColour(ModernRound_AttHP, 0x000000FF);
	TextDrawFont(ModernRound_AttHP, 1);
	TextDrawSetProportional(ModernRound_AttHP, true);

	ModernRound_DefHP = TextDrawCreate(516.000, 433.000, "[1] 200");
	TextDrawLetterSize(ModernRound_DefHP, 0.2251, 1.2883);
	TextDrawAlignment(ModernRound_DefHP, 3);
	TextDrawColour(ModernRound_DefHP, -1);
	TextDrawSetShadow(ModernRound_DefHP, 0);
	TextDrawSetOutline(ModernRound_DefHP, 1);
	TextDrawBackgroundColour(ModernRound_DefHP, 0x000000FF);
	TextDrawFont(ModernRound_DefHP, 1);
	TextDrawSetProportional(ModernRound_DefHP, true);

	ModernRound_AttDmgBoard = TextDrawCreate(196.046, 402.176, "LD_POKE:cd1s");
	TextDrawTextSize(ModernRound_AttDmgBoard, 26.108, 26.012);
	TextDrawAlignment(ModernRound_AttDmgBoard, 1);
	TextDrawColour(ModernRound_AttDmgBoard, 0x0000006A);
	TextDrawSetShadow(ModernRound_AttDmgBoard, 1);
	TextDrawSetOutline(ModernRound_AttDmgBoard, 0);
	TextDrawBackgroundColour(ModernRound_AttDmgBoard, 0x000000FF);
	TextDrawFont(ModernRound_AttDmgBoard, 4);
	TextDrawSetProportional(ModernRound_AttDmgBoard, true);

	ModernRound_AttDmgText = TextDrawCreate(209.100, 409.146, " ");
	TextDrawLetterSize(ModernRound_AttDmgText, 0.2359, 1.2537);
	TextDrawTextSize(ModernRound_AttDmgText, 596.683, 930.000);
	TextDrawAlignment(ModernRound_AttDmgText, 2);
	TextDrawColour(ModernRound_AttDmgText, 0x2ED955FF);
	TextDrawSetShadow(ModernRound_AttDmgText, 0);
	TextDrawSetOutline(ModernRound_AttDmgText, 1);
	TextDrawBackgroundColour(ModernRound_AttDmgText, 0x12111193);
	TextDrawFont(ModernRound_AttDmgText, 1);
	TextDrawSetProportional(ModernRound_AttDmgText, true);

	ModernRound_DefDmgBoard = TextDrawCreate(417.846, 402.176, "LD_POKE:cd1s");
	TextDrawTextSize(ModernRound_DefDmgBoard, 26.108, 26.012);
	TextDrawAlignment(ModernRound_DefDmgBoard, 1);
	TextDrawColour(ModernRound_DefDmgBoard, 0x0000006A);
	TextDrawSetShadow(ModernRound_DefDmgBoard, 1);
	TextDrawSetOutline(ModernRound_DefDmgBoard, 0);
	TextDrawBackgroundColour(ModernRound_DefDmgBoard, 0x000000FF);
	TextDrawFont(ModernRound_DefDmgBoard, 4);
	TextDrawSetProportional(ModernRound_DefDmgBoard, true);

	ModernRound_DefDmgText = TextDrawCreate(430.900, 409.146, " ");
	TextDrawLetterSize(ModernRound_DefDmgText, 0.2359, 1.2537);
	TextDrawTextSize(ModernRound_DefDmgText, 596.683, 930.000);
	TextDrawAlignment(ModernRound_DefDmgText, 2);
	TextDrawColour(ModernRound_DefDmgText, 0x3D82FFFF);
	TextDrawSetShadow(ModernRound_DefDmgText, 0);
	TextDrawSetOutline(ModernRound_DefDmgText, 1);
	TextDrawBackgroundColour(ModernRound_DefDmgText, 0x12111193);
	TextDrawFont(ModernRound_DefDmgText, 1);
	TextDrawSetProportional(ModernRound_DefDmgText, true);

	ModernRound_CPBoard = TextDrawCreate(268.500, 374.241, "LD_CARD:cd9s");
	TextDrawTextSize(ModernRound_CPBoard, 103.000, 33.806);
	TextDrawAlignment(ModernRound_CPBoard, 1);
	TextDrawColour(ModernRound_CPBoard, 0x0000004D);
	TextDrawSetShadow(ModernRound_CPBoard, 1);
	TextDrawSetOutline(ModernRound_CPBoard, 0);
	TextDrawBackgroundColour(ModernRound_CPBoard, 0x000000FF);
	TextDrawFont(ModernRound_CPBoard, 4);
	TextDrawSetProportional(ModernRound_CPBoard, true);

	ModernRound_CPText = TextDrawCreate(320.000, 380.608, "TAKING THE CP!");
	TextDrawLetterSize(ModernRound_CPText, 0.2268, 1.0776);
	TextDrawAlignment(ModernRound_CPText, 2);
	TextDrawColour(ModernRound_CPText, -1);
	TextDrawSetShadow(ModernRound_CPText, 0);
	TextDrawSetOutline(ModernRound_CPText, 1);
	TextDrawBackgroundColour(ModernRound_CPText, 0x000000FF);
	TextDrawFont(ModernRound_CPText, 1);
	TextDrawSetProportional(ModernRound_CPText, true);

	ModernRound_CPPercent = TextDrawCreate(320.000, 391.281, "1/20");
	TextDrawLetterSize(ModernRound_CPPercent, 0.2446, 1.1619);
	TextDrawAlignment(ModernRound_CPPercent, 2);
	TextDrawColour(ModernRound_CPPercent, -1);
	TextDrawSetShadow(ModernRound_CPPercent, 0);
	TextDrawSetOutline(ModernRound_CPPercent, 1);
	TextDrawBackgroundColour(ModernRound_CPPercent, 0x000000FF);
	TextDrawFont(ModernRound_CPPercent, 1);
	TextDrawSetProportional(ModernRound_CPPercent, true);

	// Simple Round Textdraws (Good For FPS)
	SimpleRound_Timer = TextDrawCreate(320.000, 433.000, "0:00");
	TextDrawLetterSize(SimpleRound_Timer, 0.2251, 1.2883);
	TextDrawAlignment(SimpleRound_Timer, 2);
	TextDrawColour(SimpleRound_Timer, -1);
	TextDrawSetShadow(SimpleRound_Timer, 1);
	TextDrawSetOutline(SimpleRound_Timer, 0);
	TextDrawBackgroundColour(SimpleRound_Timer, 0x000000FF);
	TextDrawFont(SimpleRound_Timer, 1);
	TextDrawSetProportional(SimpleRound_Timer, true);

	SimpleRound_AttTeamName = TextDrawCreate(210.000, 433.000, "Attacker");
	TextDrawLetterSize(SimpleRound_AttTeamName, 0.2251, 1.2883);
	TextDrawAlignment(SimpleRound_AttTeamName, 2);
	TextDrawColour(SimpleRound_AttTeamName, 0x2ED955FF);
	TextDrawSetShadow(SimpleRound_AttTeamName, 1);
	TextDrawSetOutline(SimpleRound_AttTeamName, 0);
	TextDrawBackgroundColour(SimpleRound_AttTeamName, 0x000000FF);
	TextDrawFont(SimpleRound_AttTeamName, 1);
	TextDrawSetProportional(SimpleRound_AttTeamName, true);

	SimpleRound_DefTeamName = TextDrawCreate(430.000, 433.000, "Defender");
	TextDrawLetterSize(SimpleRound_DefTeamName, 0.2251, 1.2883);
	TextDrawAlignment(SimpleRound_DefTeamName, 2);
	TextDrawColour(SimpleRound_DefTeamName, 0x3D82FFFF);
	TextDrawSetShadow(SimpleRound_DefTeamName, 1);
	TextDrawSetOutline(SimpleRound_DefTeamName, 0);
	TextDrawBackgroundColour(SimpleRound_DefTeamName, 0x000000FF);
	TextDrawFont(SimpleRound_DefTeamName, 1);
	TextDrawSetProportional(SimpleRound_DefTeamName, true);

	SimpleRound_AttHeart = TextDrawCreate(110.000, 435.000, "hud:radar_girlfriend");
	TextDrawTextSize(SimpleRound_AttHeart, 10.500, 10.500);
	TextDrawAlignment(SimpleRound_AttHeart, 1);
	TextDrawColour(SimpleRound_AttHeart, -1);
	TextDrawSetShadow(SimpleRound_AttHeart, 1);
	TextDrawSetOutline(SimpleRound_AttHeart, 0);
	TextDrawBackgroundColour(SimpleRound_AttHeart, 0x000000FF);
	TextDrawFont(SimpleRound_AttHeart, 4);
	TextDrawSetProportional(SimpleRound_AttHeart, true);

	SimpleRound_DefHeart = TextDrawCreate(519.500, 435.000, "hud:radar_girlfriend");
	TextDrawTextSize(SimpleRound_DefHeart, 10.500, 10.500);
	TextDrawAlignment(SimpleRound_DefHeart, 1);
	TextDrawColour(SimpleRound_DefHeart, -1);
	TextDrawSetShadow(SimpleRound_DefHeart, 1);
	TextDrawSetOutline(SimpleRound_DefHeart, 0);
	TextDrawBackgroundColour(SimpleRound_DefHeart, 0x000000FF);
	TextDrawFont(SimpleRound_DefHeart, 4);
	TextDrawSetProportional(SimpleRound_DefHeart, true);

	SimpleRound_AttHP = TextDrawCreate(124.000, 433.000, "200 [1]");
	TextDrawLetterSize(SimpleRound_AttHP, 0.2251, 1.2883);
	TextDrawAlignment(SimpleRound_AttHP, 1);
	TextDrawColour(SimpleRound_AttHP, -1);
	TextDrawSetShadow(SimpleRound_AttHP, 1);
	TextDrawSetOutline(SimpleRound_AttHP, 0);
	TextDrawBackgroundColour(SimpleRound_AttHP, 0x000000FF);
	TextDrawFont(SimpleRound_AttHP, 1);
	TextDrawSetProportional(SimpleRound_AttHP, true);

	SimpleRound_DefHP = TextDrawCreate(516.000, 433.000, "[1] 200");
	TextDrawLetterSize(SimpleRound_DefHP, 0.2251, 1.2883);
	TextDrawAlignment(SimpleRound_DefHP, 3);
	TextDrawColour(SimpleRound_DefHP, -1);
	TextDrawSetShadow(SimpleRound_DefHP, 1);
	TextDrawSetOutline(SimpleRound_DefHP, 0);
	TextDrawBackgroundColour(SimpleRound_DefHP, 0x000000FF);
	TextDrawFont(SimpleRound_DefHP, 1);
	TextDrawSetProportional(SimpleRound_DefHP, true);

	SimpleRound_AttDmgText = TextDrawCreate(209.100, 409.146, " ");
	TextDrawLetterSize(SimpleRound_AttDmgText, 0.2359, 1.2537);
	TextDrawTextSize(SimpleRound_AttDmgText, 596.683, 930.000);
	TextDrawAlignment(SimpleRound_AttDmgText, 2);
	TextDrawColour(SimpleRound_AttDmgText, 0x2ED955FF);
	TextDrawSetShadow(SimpleRound_AttDmgText, 1);
	TextDrawSetOutline(SimpleRound_AttDmgText, 0);
	TextDrawBackgroundColour(SimpleRound_AttDmgText, 0x12111193);
	TextDrawFont(SimpleRound_AttDmgText, 1);
	TextDrawSetProportional(SimpleRound_AttDmgText, true);

	SimpleRound_DefDmgText = TextDrawCreate(430.900, 409.146, " ");
	TextDrawLetterSize(SimpleRound_DefDmgText, 0.2359, 1.2537);
	TextDrawTextSize(SimpleRound_DefDmgText, 596.683, 930.000);
	TextDrawAlignment(SimpleRound_DefDmgText, 2);
	TextDrawColour(SimpleRound_DefDmgText, 0x3D82FFFF);
	TextDrawSetShadow(SimpleRound_DefDmgText, 1);
	TextDrawSetOutline(SimpleRound_DefDmgText, 0);
	TextDrawBackgroundColour(SimpleRound_DefDmgText, 0x12111193);
	TextDrawFont(SimpleRound_DefDmgText, 1);
	TextDrawSetProportional(SimpleRound_DefDmgText, true);

	SimpleRound_CPLine = TextDrawCreate(320.000, 360.043, " ");
	TextDrawLetterSize(SimpleRound_CPLine, 5.6149, 4.5418);
	TextDrawAlignment(SimpleRound_CPLine, 2);
	TextDrawColour(SimpleRound_CPLine, 0x2ED955FF);
	TextDrawSetShadow(SimpleRound_CPLine, 1);
	TextDrawSetOutline(SimpleRound_CPLine, 0);
	TextDrawBackgroundColour(SimpleRound_CPLine, 0x000000FF);
	TextDrawFont(SimpleRound_CPLine, 1);
	TextDrawSetProportional(SimpleRound_CPLine, true);

	SimpleRound_CPText = TextDrawCreate(320.000, 380.608, "TAKING THE CP!");
	TextDrawLetterSize(SimpleRound_CPText, 0.2268, 1.0776);
	TextDrawAlignment(SimpleRound_CPText, 2);
	TextDrawColour(SimpleRound_CPText, -1);
	TextDrawSetShadow(SimpleRound_CPText, 1);
	TextDrawSetOutline(SimpleRound_CPText, 0);
	TextDrawBackgroundColour(SimpleRound_CPText, 0x000000FF);
	TextDrawFont(SimpleRound_CPText, 1);
	TextDrawSetProportional(SimpleRound_CPText, true);

	SimpleRound_CPPercent = TextDrawCreate(320.000, 391.281, "1/20");
	TextDrawLetterSize(SimpleRound_CPPercent, 0.2446, 1.1619);
	TextDrawAlignment(SimpleRound_CPPercent, 2);
	TextDrawColour(SimpleRound_CPPercent, -1);
	TextDrawSetShadow(SimpleRound_CPPercent, 1);
	TextDrawSetOutline(SimpleRound_CPPercent, 0);
	TextDrawBackgroundColour(SimpleRound_CPPercent, 0x000000FF);
	TextDrawFont(SimpleRound_CPPercent, 1);
	TextDrawSetProportional(SimpleRound_CPPercent, true);

	CreateRoundTeamStatusTextDraws();
	CreateRoundTeamStatusHUD();

	AttHpLose = TextDrawCreate(10.000000, 436.000000, "_");
	TextDrawBackgroundColor(AttHpLose, MAIN_BACKGROUND_COLOUR);
	TextDrawFont(AttHpLose, 1);
	TextDrawLetterSize(AttHpLose, 0.160000, 1.060000);
	TextDrawColor(AttHpLose, 0x222222FF);
	TextDrawSetOutline(AttHpLose, 1);
	TextDrawSetProportional(AttHpLose, 1);
	TextDrawAlignment(AttHpLose, 1);
	TextDrawSetShadow(AttHpLose, 0);

	DefHpLose = TextDrawCreate(630.000000, 436.000000, "_");
	TextDrawBackgroundColor(DefHpLose, MAIN_BACKGROUND_COLOUR);
	TextDrawFont(DefHpLose, 1);
	TextDrawLetterSize(DefHpLose, 0.160000, 1.060000);
	TextDrawColor(DefHpLose, 0x222222FF);
	TextDrawSetOutline(DefHpLose, 1);
	TextDrawSetProportional(DefHpLose, 1);
	TextDrawAlignment(DefHpLose, 3);
	TextDrawSetShadow(DefHpLose, 0);

	TeamHpLose[0] = TextDrawCreate(165.000000, 400.000000, "_");
	TextDrawAlignment(TeamHpLose[0], 2);
	TextDrawFont(TeamHpLose[0], 2);
	TextDrawLetterSize(TeamHpLose[0], 0.40000, 1.90000);
	TextDrawBackgroundColor(TeamHpLose[0], MAIN_BACKGROUND_COLOUR);
	TextDrawColor(TeamHpLose[0], -1);
	TextDrawSetOutline(TeamHpLose[0], 1);
	TextDrawSetProportional(TeamHpLose[0], 1);
	TextDrawSetShadow(TeamHpLose[0], 0);

	TeamHpLose[1] = TextDrawCreate(467.000000, 400.000000, "_");
	TextDrawAlignment(TeamHpLose[1], 2);
	TextDrawFont(TeamHpLose[1], 2);
	TextDrawLetterSize(TeamHpLose[1], 0.40000, 1.90000);
	TextDrawBackgroundColor(TeamHpLose[1], MAIN_BACKGROUND_COLOUR);
	TextDrawColor(TeamHpLose[1], -1);
	TextDrawSetOutline(TeamHpLose[1], 1);
	TextDrawSetProportional(TeamHpLose[1], 1);
	TextDrawSetShadow(TeamHpLose[1], 0);

	CreateTeamCornerTextDraws();
}

CreateRoundTeamStatusTextDraws()
{
	for (new slot = 0; slot < MAX_ROUND_TEAM_STATUS_ICONS; slot++)
	{
		new Float:attackerX = 240.0 - (slot * 13.5);
		new Float:defenderX = 370.0 + (slot * 13.5);

		RoundTeamStatusIcons[ATTACKER][slot] = TextDrawCreate(attackerX, 416.0, "LD_OTB2:Ric5");
		TextDrawFont(RoundTeamStatusIcons[ATTACKER][slot], 4);
		TextDrawLetterSize(RoundTeamStatusIcons[ATTACKER][slot], 0.48, 1.12);
		TextDrawAlignment(RoundTeamStatusIcons[ATTACKER][slot], 0);
		TextDrawTextSize(RoundTeamStatusIcons[ATTACKER][slot], 24.5, 35.0);
		TextDrawColor(RoundTeamStatusIcons[ATTACKER][slot], 232664575);
		TextDrawSetProportional(RoundTeamStatusIcons[ATTACKER][slot], 1);
		TextDrawSetShadow(RoundTeamStatusIcons[ATTACKER][slot], 2);
		TextDrawSetOutline(RoundTeamStatusIcons[ATTACKER][slot], 0);
		TextDrawBackgroundColor(RoundTeamStatusIcons[ATTACKER][slot], 0x000000FF);
		TextDrawSetSelectable(RoundTeamStatusIcons[ATTACKER][slot], 0);

		RoundTeamStatusIcons[DEFENDER][slot] = TextDrawCreate(defenderX, 416.0, "LD_OTB2:Ric1");
		TextDrawFont(RoundTeamStatusIcons[DEFENDER][slot], 4);
		TextDrawLetterSize(RoundTeamStatusIcons[DEFENDER][slot], 0.48, 1.12);
		TextDrawAlignment(RoundTeamStatusIcons[DEFENDER][slot], 0);
		TextDrawTextSize(RoundTeamStatusIcons[DEFENDER][slot], 24.5, 35.0);
		TextDrawColor(RoundTeamStatusIcons[DEFENDER][slot], 0x375FFFFF);
		TextDrawSetProportional(RoundTeamStatusIcons[DEFENDER][slot], 1);
		TextDrawSetShadow(RoundTeamStatusIcons[DEFENDER][slot], 2);
		TextDrawSetOutline(RoundTeamStatusIcons[DEFENDER][slot], 0);
		TextDrawBackgroundColor(RoundTeamStatusIcons[DEFENDER][slot], 0x000000FF);
		TextDrawSetSelectable(RoundTeamStatusIcons[DEFENDER][slot], 0);
	}
	return 1;
}

CreateRoundTeamStatusHUD()
{
	RoundTeamStatusHUD[0] = TextDrawCreate(320.0, 422.0, "07:08");
	RoundTeamStatusHUD[1] = TextDrawCreate(260.0, 410.0, "_");
	RoundTeamStatusHUD[2] = TextDrawCreate(373.0, 410.0, "_");

	for (new i = 0; i < sizeof(RoundTeamStatusHUD); i++)
	{
		TextDrawFont(RoundTeamStatusHUD[i], 2);
		TextDrawLetterSize(RoundTeamStatusHUD[i], 0.26, 1.14);
		TextDrawSetProportional(RoundTeamStatusHUD[i], 1);
		TextDrawSetOutline(RoundTeamStatusHUD[i], 0);
		TextDrawSetShadow(RoundTeamStatusHUD[i], 1);
		TextDrawBackgroundColor(RoundTeamStatusHUD[i], 0x000000FF);
		TextDrawSetSelectable(RoundTeamStatusHUD[i], 0);
	}

	TextDrawAlignment(RoundTeamStatusHUD[0], 2);
	TextDrawColor(RoundTeamStatusHUD[0], -1);
	TextDrawAlignment(RoundTeamStatusHUD[1], 3);
	TextDrawColor(RoundTeamStatusHUD[1], -1);
	TextDrawAlignment(RoundTeamStatusHUD[2], 1);
	TextDrawColor(RoundTeamStatusHUD[2], -1);
	format(RoundTeamStatusTimer, sizeof(RoundTeamStatusTimer), "~w~07:08");
	return 1;
}

GetRoundTeamHPHUDBarWidths(&Float: attackerWidth, &Float: defenderWidth)
{
	attackerWidth = ROUND_TEAM_HP_HUD_BAR_WIDTH;
	defenderWidth = ROUND_TEAM_HP_HUD_BAR_WIDTH;
	if (Current == -1)
		return 1;
	if (!ArenaStarted && !BaseStarted)
	{
		attackerWidth = 0.0;
		defenderWidth = 0.0;
		return 1;
	}

	new attackerMax = TeamHPMax[ATTACKER] > 0 ? TeamHPMax[ATTACKER] : 1;
	new defenderMax = TeamHPMax[DEFENDER] > 0 ? TeamHPMax[DEFENDER] : 1;
	attackerWidth = float(TeamHP[ATTACKER]) * ROUND_TEAM_HP_HUD_BAR_WIDTH / float(attackerMax);
	defenderWidth = float(TeamHP[DEFENDER]) * ROUND_TEAM_HP_HUD_BAR_WIDTH / float(defenderMax);
	if (attackerWidth < 0.0)
		attackerWidth = 0.0;
	else if (attackerWidth > ROUND_TEAM_HP_HUD_BAR_WIDTH)
		attackerWidth = ROUND_TEAM_HP_HUD_BAR_WIDTH;
	if (defenderWidth < 0.0)
		defenderWidth = 0.0;
	else if (defenderWidth > ROUND_TEAM_HP_HUD_BAR_WIDTH)
		defenderWidth = ROUND_TEAM_HP_HUD_BAR_WIDTH;
	return 1;
}

ApplyRoundTeamHPHUDBars(playerid, Float: attackerWidth, Float: defenderWidth, bool: force)
{
	if (!RoundTeamHPHUDReady[playerid])
		return 0;
	if (!force
		&& RoundTeamHPHUDApplied[playerid][0] == attackerWidth
		&& RoundTeamHPHUDApplied[playerid][1] == defenderWidth)
		return 0;

	new PlayerText: attackerBar = PlayerText:RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerBar];
	new PlayerText: defenderBar = PlayerText:RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderBar];
	new PlayerText: attackerDmgBar = PlayerText:RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageBar];
	new PlayerText: defenderDmgBar = PlayerText:RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageBar];

	new Float: attEdgeX = ROUND_TEAM_HP_HUD_BAR_LEFT_X + attackerWidth;
	PlayerTextDrawSetPos(playerid, attackerBar, ROUND_TEAM_HP_HUD_BAR_LEFT_X, ROUND_TEAM_HP_HUD_BAR_Y);
	PlayerTextDrawTextSize(
		playerid, attackerBar,
		attEdgeX, -41.0);
	PlayerTextDrawUseBox(playerid, attackerBar, true);
	PlayerTextDrawBoxColor(
		playerid, attackerBar,
		attackerWidth > 0.0
			? (RoundTeamHPHUDFlashing[ATTACKER]
				? ROUND_TEAM_HP_HUD_ATTACKER_FLASH_COLOR
				: ROUND_TEAM_HP_HUD_ATTACKER_COLOR)
			: 0);

	new bool: showAttackerShadow = (attackerWidth > 0.0 && attackerWidth < ROUND_TEAM_HP_HUD_BAR_WIDTH);
	new Float: attShadowStartX = attEdgeX + 4.0 - ROUND_TEAM_HP_HUD_EDGE_SHADOW_WIDTH;
	if (attShadowStartX < ROUND_TEAM_HP_HUD_BAR_LEFT_X)
		attShadowStartX = ROUND_TEAM_HP_HUD_BAR_LEFT_X;
	PlayerTextDrawSetPos(playerid, attackerDmgBar, attShadowStartX, ROUND_TEAM_HP_HUD_BAR_Y);
	PlayerTextDrawTextSize(playerid, attackerDmgBar, attEdgeX, 0.0);
	PlayerTextDrawUseBox(playerid, attackerDmgBar, true);
	PlayerTextDrawBoxColor(
		playerid, attackerDmgBar,
		showAttackerShadow ? ROUND_TEAM_HP_HUD_EDGE_SHADOW_COLOR : 0);

	new Float: defEdgeX = ROUND_TEAM_HP_HUD_BAR_RIGHT_X - defenderWidth;
	PlayerTextDrawSetPos(
		playerid, defenderBar,
		defEdgeX, ROUND_TEAM_HP_HUD_BAR_Y);
	PlayerTextDrawTextSize(
		playerid, defenderBar,
		ROUND_TEAM_HP_HUD_BAR_RIGHT_X, 0.0);
	PlayerTextDrawUseBox(playerid, defenderBar, true);
	PlayerTextDrawBoxColor(
		playerid, defenderBar,
		defenderWidth > 0.0
			? (RoundTeamHPHUDFlashing[DEFENDER]
				? ROUND_TEAM_HP_HUD_DEFENDER_FLASH_COLOR
				: ROUND_TEAM_HP_HUD_DEFENDER_COLOR)
			: 0);

	new bool: showDefenderShadow = (defenderWidth > 0.0 && defenderWidth < ROUND_TEAM_HP_HUD_BAR_WIDTH);
	new Float: defShadowEndX = defEdgeX - 4.0 + ROUND_TEAM_HP_HUD_EDGE_SHADOW_WIDTH;
	if (defShadowEndX > ROUND_TEAM_HP_HUD_BAR_RIGHT_X)
		defShadowEndX = ROUND_TEAM_HP_HUD_BAR_RIGHT_X;
	PlayerTextDrawSetPos(playerid, defenderDmgBar, defEdgeX, ROUND_TEAM_HP_HUD_BAR_Y);
	PlayerTextDrawTextSize(playerid, defenderDmgBar, defShadowEndX, 0.0);
	PlayerTextDrawUseBox(playerid, defenderDmgBar, true);
	PlayerTextDrawBoxColor(
		playerid, defenderDmgBar,
		showDefenderShadow ? ROUND_TEAM_HP_HUD_EDGE_SHADOW_COLOR : 0);

	RoundTeamHPHUDApplied[playerid][0] = attackerWidth;
	RoundTeamHPHUDApplied[playerid][1] = defenderWidth;

	if (RoundTeamHPHUDShown[playerid])
	{
		PlayerTextDrawShow(playerid, attackerBar);
		PlayerTextDrawShow(playerid, defenderBar);
		PlayerTextDrawShow(playerid, attackerDmgBar);
		PlayerTextDrawShow(playerid, defenderDmgBar);
	}
	return 1;
}

UpdateRoundTeamStatusHUD()
{
	new string[128];
	format(string, sizeof(string), "%s~n~~g~~h~%d  ~b~~h~%d~n~",
		RoundTeamStatusTimer, TeamHP[ATTACKER], TeamHP[DEFENDER]);
	TextDrawSetString(RoundTeamStatusHUD[0], string);

	if (TempDamage[ATTACKER] > 0.0)
	{
		format(string, sizeof(string), "~g~~h~-%d", floatround(TempDamage[ATTACKER]));
		TextDrawSetString(RoundTeamStatusHUD[1], string);
	}
	else
		TextDrawSetString(RoundTeamStatusHUD[1], "_");

	if (TempDamage[DEFENDER] > 0.0)
	{
		format(string, sizeof(string), "~b~-%d", floatround(TempDamage[DEFENDER]));
		TextDrawSetString(RoundTeamStatusHUD[2], string);
	}
	else
		TextDrawSetString(RoundTeamStatusHUD[2], "_");

	if (Current == -1 || AllVsAllMode)
		return 1;

	new centerText[64];
	if (!ArenaStarted && !BaseStarted && ViewRoundCountdown > 0)
	{
		format(
			centerText, sizeof(centerText), "~w~%s: %d - Starting: %d",
			GameType == BASE ? "Base" : "Arena", Current, ViewRoundCountdown);
	}
	else if (GameType == BASE)
		format(centerText, sizeof(centerText), "~w~Base: %d - Time: %s", Current, RoundTeamStatusTimer);
	else
		format(centerText, sizeof(centerText), "~w~Arena: %d - Time: %s", Current, RoundTeamStatusTimer);

	new bool: roundStarting = !ArenaStarted && !BaseStarted;
	new attackerHP = roundStarting ? 0 : TeamHP[ATTACKER];
	new defenderHP = roundStarting ? 0 : TeamHP[DEFENDER];
	new Float: attackerBarWidth,
		Float: defenderBarWidth;
	GetRoundTeamHPHUDBarWidths(attackerBarWidth, defenderBarWidth);

	new attDmgStr[16];
	new defDmgStr[16];
	if (TempDamage[ATTACKER] > 0.0)
		format(attDmgStr, sizeof(attDmgStr), "-%d", floatround(TempDamage[ATTACKER]));
	if (TempDamage[DEFENDER] > 0.0)
		format(defDmgStr, sizeof(defDmgStr), "-%d", floatround(TempDamage[DEFENDER]));

	foreach (new playerid : Player)
	{
		if (Player[playerid][RoundTDStyle] != 4 || !RoundTeamHPHUDReady[playerid] || (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER))
		{
			if (RoundTeamHPHUDShown[playerid])
			{
				RoundTeamHPHUDShown[playerid] = false;
				for (new eRoundTeamHPHUDText:i = RoundTeamHPHUD_Background; i <= RoundTeamHPHUD_DefenderDamageText; i++)
					PlayerTextDrawHide(playerid, PlayerText:RoundTeamHPHUDText[playerid][i]);
			}
			continue;
		}

		ApplyRoundTeamHPHUDBars(playerid, attackerBarWidth, defenderBarWidth, false);

		format(
			string, sizeof(string), "~g~~h~[%s]    %d/%d     (~g~~h~%d~g~~h~)",
			TeamName[ATTACKER], PlayersAlive[ATTACKER],
			PlayersAlive[ATTACKER] + PlayersDead[ATTACKER], attackerHP);
		PlayerTextDrawSetString(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerTeam], string);
		PlayerTextDrawSetString(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center], centerText);
		format(
			string, sizeof(string), "~b~~h~(~b~~h~%d~b~~h~)    %d/%d     [%s]",
			defenderHP, PlayersAlive[DEFENDER],
			PlayersAlive[DEFENDER] + PlayersDead[DEFENDER], TeamName[DEFENDER]);
		PlayerTextDrawSetString(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderTeam], string);

		if (!RoundTeamHPHUDShown[playerid] && (Player[playerid][Playing] || Player[playerid][ToAddInRound] || Player[playerid][Spectating]))
		{
			for (new eRoundTeamHPHUDText:i = RoundTeamHPHUD_Background; i <= RoundTeamHPHUD_DefenderTeam; i++)
				PlayerTextDrawShow(playerid, PlayerText:RoundTeamHPHUDText[playerid][i]);
			RoundTeamHPHUDShown[playerid] = true;
		}
		else if (RoundTeamHPHUDShown[playerid])
		{
			PlayerTextDrawShow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_Center]);
		}

		if (RoundTeamHPHUDShown[playerid])
		{
			if (TempDamage[ATTACKER] > 0.0)
			{
				PlayerTextDrawSetString(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText], attDmgStr);
				PlayerTextDrawShow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard]);
				PlayerTextDrawShow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText]);
			}
			else
			{
				PlayerTextDrawHide(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerCard]);
				PlayerTextDrawHide(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_AttackerDamageText]);
			}

			if (TempDamage[DEFENDER] > 0.0)
			{
				PlayerTextDrawSetString(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText], defDmgStr);
				PlayerTextDrawShow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard]);
				PlayerTextDrawShow(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText]);
			}
			else
			{
				PlayerTextDrawHide(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderCard]);
				PlayerTextDrawHide(playerid, RoundTeamHPHUDText[playerid][RoundTeamHPHUD_DefenderDamageText]);
			}
		}
	}
	return 1;
}

UpdateRoundTeamStatusTimer(const text[])
{
	format(RoundTeamStatusTimer, sizeof(RoundTeamStatusTimer), "%s", text);
	UpdateRoundTeamStatusHUD();
	return 1;
}

HideRoundTeamHPCPHUDForPlayer(playerid)
{
	RoundTeamHPCPShown[playerid] = false;
	if (!RoundTeamHPCPReady[playerid])
		return 0;

	for (new eRoundTeamHPCPHUD:i = RoundTeamHPCP_Card; i <= RoundTeamHPCP_TimeText; i++)
		PlayerTextDrawHide(playerid, PlayerText:RoundTeamHPCPHUD[playerid][i]);
	return 1;
}

HideRoundTeamHPCPHUDForAll()
{
	foreach (new i : Player)
		HideRoundTeamHPCPHUDForPlayer(i);
	return 1;
}

UpdateRoundTeamHPCPHUD()
{
	if (Current == -1 || AllVsAllMode || (!ArenaStarted && !BaseStarted) || RoundPaused || PlayersInCP <= 0)
	{
		HideRoundTeamHPCPHUDForAll();
		return 1;
	}

	new Float: ratio = 0.0;
	if (ConfigCPTime > 0)
	{
		ratio = float(CurrentCPTime) / float(ConfigCPTime);
		if (ratio < 0.0)
			ratio = 0.0;
		else if (ratio > 1.0)
			ratio = 1.0;
	}

	new Float: redBarX = ROUND_TEAM_HP_CP_BAR_START_X + (ratio * ROUND_TEAM_HP_CP_BAR_SPAN_X);
	new Float: redBar2X = redBarX - ROUND_TEAM_HP_CP_BAR2_OFFSET_X;
	if (redBar2X < ROUND_TEAM_HP_CP_BAR2_START_X)
		redBar2X = ROUND_TEAM_HP_CP_BAR2_START_X;

	new cpPlayerName[MAX_PLAYER_NAME + 16];
	new capturingTeam = GameType == BASE ? ATTACKER : TeamCapturingCP;
	new countInCP = 0;
	foreach (new i : PlayersInRound)
	{
		if (Player[i][WasInCP] && (GameType == BASE ? (Player[i][Team] == ATTACKER) : (Player[i][Team] == capturingTeam)))
		{
			countInCP++;
			if (countInCP == 1)
				format(cpPlayerName, sizeof(cpPlayerName), "%s", Player[i][Name]);
		}
	}
	if (countInCP > 1)
		format(cpPlayerName, sizeof(cpPlayerName), "%s (+%d)", cpPlayerName, countInCP - 1);
	else if (countInCP == 0)
		format(cpPlayerName, sizeof(cpPlayerName), "Capturing");

	new cpTimeStr[32];
	if (CurrentCPTime == 1)
		format(cpTimeStr, sizeof(cpTimeStr), "1 Second");
	else
		format(cpTimeStr, sizeof(cpTimeStr), "%d Seconds", CurrentCPTime);

	foreach (new playerid : Player)
	{
		if (Player[playerid][RoundTDStyle] != 4 || !RoundTeamHPCPReady[playerid] || !RoundTeamHPHUDShown[playerid] || Player[playerid][Team] == REFEREE)
		{
			if (RoundTeamHPCPShown[playerid])
				HideRoundTeamHPCPHUDForPlayer(playerid);
			continue;
		}

		PlayerTextDrawTextSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder], redBarX, 0.0);
		PlayerTextDrawTextSize(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill], redBar2X, 0.0);
		PlayerTextDrawSetString(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName], cpPlayerName);
		PlayerTextDrawSetString(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText], cpTimeStr);

		PlayerTextDrawShow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Card]);
		PlayerTextDrawShow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarBorder]);
		PlayerTextDrawShow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarEmpty]);
		PlayerTextDrawShow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveBorder]);
		PlayerTextDrawShow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_BarActiveFill]);
		PlayerTextDrawShow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_Title]);
		PlayerTextDrawShow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_PlayerName]);
		PlayerTextDrawShow(playerid, RoundTeamHPCPHUD[playerid][RoundTeamHPCP_TimeText]);
		RoundTeamHPCPShown[playerid] = true;
	}
	return 1;
}


public UpdateAllVsAllTextDraws()
{
	if (!AllVsAllMode || Current == -1)
		return 0;

	new top1ID = INVALID_PLAYER_ID, top1Kills = -1, Float: top1Dmg = -1.0;
	new top2ID = INVALID_PLAYER_ID, top2Kills = -1, Float: top2Dmg = -1.0;

	foreach (new i : PlayersInRound)
	{
		if (!IsPlayerConnected(i) || !Player[i][Playing])
			continue;

		new kills = Player[i][RoundKills];
		new Float: dmg = Player[i][RoundDamage];

		if (kills > top1Kills || (kills == top1Kills && dmg > top1Dmg))
		{
			top2ID = top1ID;
			top2Kills = top1Kills;
			top2Dmg = top1Dmg;

			top1ID = i;
			top1Kills = kills;
			top1Dmg = dmg;
		}
		else if (kills > top2Kills || (kills == top2Kills && dmg > top2Dmg))
		{
			top2ID = i;
			top2Kills = kills;
			top2Dmg = dmg;
		}
	}

	new str[64];

	// Top 1 (Attacker slot)
	if (top1ID != INVALID_PLAYER_ID && top1Kills > 0)
	{
		new team1 = Player[top1ID][Team];
		new col1[16];
		if (team1 >= 0 && team1 < MAX_TEAMS && TDC[team1][0] != EOS)
			format(col1, sizeof(col1), "%s", TDC[team1]);
		else
			format(col1, sizeof(col1), "~g~~h~");

		format(str, sizeof(str), "%s%s", col1, Player[top1ID][Name]);
		TextDrawSetString(SimpleRound_AttTeamName, str);

		new hp1 = Player[top1ID][Playing] ? (GetHP(top1ID) + GetAP(top1ID)) : 0;
		if (hp1 < 0) hp1 = 0;
		format(str, sizeof(str), "%s%d ~w~[%d]", col1, hp1, top1Kills);
		TextDrawSetString(SimpleRound_AttHP, str);
	}
	else
	{
		TextDrawSetString(SimpleRound_AttTeamName, "~g~~h~None");
		TextDrawSetString(SimpleRound_AttHP, "~g~~h~0 ~w~[0]");
	}

	// Top 2 (Defender slot)
	if (top2ID != INVALID_PLAYER_ID && top2Kills > 0)
	{
		new team2 = Player[top2ID][Team];
		new col2[16];
		if (team2 >= 0 && team2 < MAX_TEAMS && TDC[team2][0] != EOS)
			format(col2, sizeof(col2), "%s", TDC[team2]);
		else
			format(col2, sizeof(col2), "~b~~h~");

		format(str, sizeof(str), "%s%s", col2, Player[top2ID][Name]);
		TextDrawSetString(SimpleRound_DefTeamName, str);

		new hp2 = Player[top2ID][Playing] ? (GetHP(top2ID) + GetAP(top2ID)) : 0;
		if (hp2 < 0) hp2 = 0;
		format(str, sizeof(str), "~w~[%d] %s%d", top2Kills, col2, hp2);
		TextDrawSetString(SimpleRound_DefHP, str);
	}
	else
	{
		TextDrawSetString(SimpleRound_DefTeamName, "~b~~h~None");
		TextDrawSetString(SimpleRound_DefHP, "~w~[0] ~b~~h~0");
	}

	return 1;
}

UpdateRoundTeamHPHUDFlash(team, bool: flashing)
{
	if (team != ATTACKER && team != DEFENDER)
		return 0;

	RoundTeamHPHUDFlashing[team] = flashing;
	new eRoundTeamHPHUDText: bar;
	new color;
	switch (team)
	{
		case ATTACKER:
		{
			bar = RoundTeamHPHUD_AttackerBar;
			color = flashing ? ROUND_TEAM_HP_HUD_ATTACKER_FLASH_COLOR : ROUND_TEAM_HP_HUD_ATTACKER_COLOR;
		}
		case DEFENDER:
		{
			bar = RoundTeamHPHUD_DefenderBar;
			color = flashing ? ROUND_TEAM_HP_HUD_DEFENDER_FLASH_COLOR : ROUND_TEAM_HP_HUD_DEFENDER_COLOR;
		}
	}

	foreach (new playerid : Player)
	{
		if (Player[playerid][RoundTDStyle] != 4 || !RoundTeamHPHUDReady[playerid] || (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER))
			continue;

		new PlayerText: teamBar = PlayerText:RoundTeamHPHUDText[playerid][bar];
		PlayerTextDrawBoxColor(playerid, teamBar, color);
		if (RoundTeamHPHUDShown[playerid])
			PlayerTextDrawShow(playerid, teamBar);
	}
	return 1;
}

ModernTextDrawFlash(team)
{
	if (Current == -1 || AllVsAllMode)
		return 1;

	TeamDamageFlashing[team] = true;
	new str[64];

	switch (team)
	{
		case ATTACKER:
		{
			format(str, sizeof(str), "~g~~h~~h~%s", TeamName[ATTACKER]);
			TextDrawSetString(ModernRound_AttTeamName, str);
			TextDrawSetString(SimpleRound_AttTeamName, str);

			format(str, sizeof(str), "~g~~h~~h~%d ~w~[%d]", TeamHP[ATTACKER], PlayersAlive[ATTACKER]);
			TextDrawSetString(ModernRound_AttHP, str);
			TextDrawSetString(SimpleRound_AttHP, str);

			foreach (new i : Player)
			{
				if (Player[i][Team] == REFEREE)
					continue;
				if (Player[i][RoundTDStyle] == 0)
				{
					TextDrawShowForPlayer(i, ModernRound_AttDmgBoard);
					TextDrawShowForPlayer(i, ModernRound_AttDmgText);
				}
				else if (Player[i][RoundTDStyle] == 2)
				{
					TextDrawShowForPlayer(i, SimpleRound_AttDmgText);
				}
			}
			UpdateRoundTeamHPHUDFlash(ATTACKER, true);
		}
		case DEFENDER:
		{
			format(str, sizeof(str), "~b~~h~~h~~h~%s", TeamName[DEFENDER]);
			TextDrawSetString(ModernRound_DefTeamName, str);
			TextDrawSetString(SimpleRound_DefTeamName, str);

			format(str, sizeof(str), "~w~[%d] ~b~~h~~h~~h~%d", PlayersAlive[DEFENDER], TeamHP[DEFENDER]);
			TextDrawSetString(ModernRound_DefHP, str);
			TextDrawSetString(SimpleRound_DefHP, str);

			foreach (new i : Player)
			{
				if (Player[i][Team] == REFEREE)
					continue;
				if (Player[i][RoundTDStyle] == 0)
				{
					TextDrawShowForPlayer(i, ModernRound_DefDmgBoard);
					TextDrawShowForPlayer(i, ModernRound_DefDmgText);
				}
				else if (Player[i][RoundTDStyle] == 2)
				{
					TextDrawShowForPlayer(i, SimpleRound_DefDmgText);
				}
			}
			UpdateRoundTeamHPHUDFlash(DEFENDER, true);
		}
	}
	return 1;
}

StopModernTextDrawFlash(team)
{
	TeamDamageFlashing[team] = false;
	UpdateRoundTeamHPHUDFlash(team, false);

	if (Current == -1 || AllVsAllMode)
		return 1;

	new str[64];
	switch (team)
	{
		case ATTACKER:
		{
			format(str, sizeof(str), "%s%s", TDC[ATTACKER], TeamName[ATTACKER]);
			TextDrawSetString(ModernRound_AttTeamName, str);
			TextDrawSetString(SimpleRound_AttTeamName, str);

			format(str, sizeof(str), "%s%d ~w~[%d]", TDC[ATTACKER], TeamHP[ATTACKER], PlayersAlive[ATTACKER]);
			TextDrawSetString(ModernRound_AttHP, str);
			TextDrawSetString(SimpleRound_AttHP, str);

			foreach (new i : Player)
			{
				if (Player[i][Team] == REFEREE)
					continue;
				if (Player[i][RoundTDStyle] == 0)
				{
					if (TempDamage[ATTACKER] > 0.0)
					{
						TextDrawShowForPlayer(i, ModernRound_AttDmgBoard);
						TextDrawShowForPlayer(i, ModernRound_AttDmgText);
					}
					else
					{
						TextDrawHideForPlayer(i, ModernRound_AttDmgBoard);
						TextDrawHideForPlayer(i, ModernRound_AttDmgText);
					}
				}
				else if (Player[i][RoundTDStyle] == 2)
				{
					if (TempDamage[ATTACKER] <= 0.0)
						TextDrawHideForPlayer(i, SimpleRound_AttDmgText);
				}
			}
		}
		case DEFENDER:
		{
			format(str, sizeof(str), "%s%s", TDC[DEFENDER], TeamName[DEFENDER]);
			TextDrawSetString(ModernRound_DefTeamName, str);
			TextDrawSetString(SimpleRound_DefTeamName, str);

			format(str, sizeof(str), "~w~[%d] %s%d", PlayersAlive[DEFENDER], TDC[DEFENDER], TeamHP[DEFENDER]);
			TextDrawSetString(ModernRound_DefHP, str);
			TextDrawSetString(SimpleRound_DefHP, str);

			foreach (new i : Player)
			{
				if (Player[i][Team] == REFEREE)
					continue;
				if (Player[i][RoundTDStyle] == 0)
				{
					if (TempDamage[DEFENDER] > 0.0)
					{
						TextDrawShowForPlayer(i, ModernRound_DefDmgBoard);
						TextDrawShowForPlayer(i, ModernRound_DefDmgText);
					}
					else
					{
						TextDrawHideForPlayer(i, ModernRound_DefDmgBoard);
						TextDrawHideForPlayer(i, ModernRound_DefDmgText);
					}
				}
				else if (Player[i][RoundTDStyle] == 2)
				{
					if (TempDamage[DEFENDER] <= 0.0)
						TextDrawHideForPlayer(i, SimpleRound_DefDmgText);
				}
			}
		}
	}
	return 1;
}

public ShowModernRoundHUD(playerid)
{
	if (Player[playerid][Team] == REFEREE)
		return 0;
	TextDrawShowForPlayer(playerid, ModernRound_BottomBar);
	TextDrawShowForPlayer(playerid, ModernRound_Timer);
	TextDrawShowForPlayer(playerid, ModernRound_AttTeamName);
	TextDrawShowForPlayer(playerid, ModernRound_DefTeamName);
	TextDrawShowForPlayer(playerid, ModernRound_AttHeart);
	TextDrawShowForPlayer(playerid, ModernRound_DefHeart);
	TextDrawShowForPlayer(playerid, ModernRound_AttHP);
	TextDrawShowForPlayer(playerid, ModernRound_DefHP);
	if (TempDamage[ATTACKER] > 0.0)
	{
		TextDrawShowForPlayer(playerid, ModernRound_AttDmgBoard);
		TextDrawShowForPlayer(playerid, ModernRound_AttDmgText);
	}
	if (TempDamage[DEFENDER] > 0.0)
	{
		TextDrawShowForPlayer(playerid, ModernRound_DefDmgBoard);
		TextDrawShowForPlayer(playerid, ModernRound_DefDmgText);
	}
	if (PlayersInCP > 0)
	{
		TextDrawShowForPlayer(playerid, ModernRound_CPBoard);
		TextDrawShowForPlayer(playerid, ModernRound_CPText);
		TextDrawShowForPlayer(playerid, ModernRound_CPPercent);
	}
	ShowTeamCornerInfo(playerid);
	return 1;
}

public HideModernRoundHUD(playerid)
{
	TextDrawHideForPlayer(playerid, ModernRound_BottomBar);
	TextDrawHideForPlayer(playerid, ModernRound_Timer);
	TextDrawHideForPlayer(playerid, ModernRound_AttTeamName);
	TextDrawHideForPlayer(playerid, ModernRound_DefTeamName);
	TextDrawHideForPlayer(playerid, ModernRound_AttHeart);
	TextDrawHideForPlayer(playerid, ModernRound_DefHeart);
	TextDrawHideForPlayer(playerid, ModernRound_AttHP);
	TextDrawHideForPlayer(playerid, ModernRound_DefHP);
	TextDrawHideForPlayer(playerid, ModernRound_AttDmgBoard);
	TextDrawHideForPlayer(playerid, ModernRound_AttDmgText);
	TextDrawHideForPlayer(playerid, ModernRound_DefDmgBoard);
	TextDrawHideForPlayer(playerid, ModernRound_DefDmgText);
	TextDrawHideForPlayer(playerid, ModernRound_CPBoard);
	TextDrawHideForPlayer(playerid, ModernRound_CPText);
	TextDrawHideForPlayer(playerid, ModernRound_CPPercent);
	HideTeamCornerInfo(playerid);
	return 1;
}

public ShowSimpleRoundHUD(playerid)
{
	if (Player[playerid][Team] == REFEREE)
		return 0;
	TextDrawShowForPlayer(playerid, SimpleRound_Timer);
	TextDrawShowForPlayer(playerid, SimpleRound_AttTeamName);
	TextDrawShowForPlayer(playerid, SimpleRound_DefTeamName);
	TextDrawShowForPlayer(playerid, SimpleRound_AttHeart);
	TextDrawShowForPlayer(playerid, SimpleRound_DefHeart);
	TextDrawShowForPlayer(playerid, SimpleRound_AttHP);
	TextDrawShowForPlayer(playerid, SimpleRound_DefHP);
	if (!AllVsAllMode && TempDamage[ATTACKER] > 0.0)
	{
		TextDrawShowForPlayer(playerid, SimpleRound_AttDmgText);
	}
	if (!AllVsAllMode && TempDamage[DEFENDER] > 0.0)
	{
		TextDrawShowForPlayer(playerid, SimpleRound_DefDmgText);
	}
	if (!AllVsAllMode && PlayersInCP > 0)
	{
		TextDrawShowForPlayer(playerid, SimpleRound_CPText);
		TextDrawShowForPlayer(playerid, SimpleRound_CPPercent);
	}
	ShowTeamCornerInfo(playerid);
	return 1;
}

public HideSimpleRoundHUD(playerid)
{
	TextDrawHideForPlayer(playerid, SimpleRound_Timer);
	TextDrawHideForPlayer(playerid, SimpleRound_AttTeamName);
	TextDrawHideForPlayer(playerid, SimpleRound_DefTeamName);
	TextDrawHideForPlayer(playerid, SimpleRound_AttHeart);
	TextDrawHideForPlayer(playerid, SimpleRound_DefHeart);
	TextDrawHideForPlayer(playerid, SimpleRound_AttHP);
	TextDrawHideForPlayer(playerid, SimpleRound_DefHP);
	TextDrawHideForPlayer(playerid, SimpleRound_AttDmgText);
	TextDrawHideForPlayer(playerid, SimpleRound_DefDmgText);
	TextDrawHideForPlayer(playerid, SimpleRound_CPLine);
	TextDrawHideForPlayer(playerid, SimpleRound_CPText);
	TextDrawHideForPlayer(playerid, SimpleRound_CPPercent);
	HideTeamCornerInfo(playerid);
	return 1;
}

public UpdateModernAndSimpleCheckpointHUD()
{
	if (Current == -1 || AllVsAllMode || (!ArenaStarted && !BaseStarted) || RoundPaused || PlayersInCP <= 0)
	{
		TextDrawHideForAll(ModernRound_CPBoard);
		TextDrawHideForAll(ModernRound_CPText);
		TextDrawHideForAll(ModernRound_CPPercent);
		TextDrawHideForAll(SimpleRound_CPLine);
		TextDrawHideForAll(SimpleRound_CPText);
		TextDrawHideForAll(SimpleRound_CPPercent);
		return 1;
	}

	new capturingTeam = (GameType == BASE) ? ATTACKER : TeamCapturingCP;
	if (capturingTeam == NON)
	{
		TextDrawHideForAll(ModernRound_CPBoard);
		TextDrawHideForAll(ModernRound_CPText);
		TextDrawHideForAll(ModernRound_CPPercent);
		TextDrawHideForAll(SimpleRound_CPLine);
		TextDrawHideForAll(SimpleRound_CPText);
		TextDrawHideForAll(SimpleRound_CPPercent);
		return 1;
	}

	new countInCP = 0;
	new firstPlayer[MAX_PLAYER_NAME + 1];
	new secondPlayer[MAX_PLAYER_NAME + 1];
	firstPlayer[0] = EOS;
	secondPlayer[0] = EOS;
	foreach (new i : PlayersInRound)
	{
		if (Player[i][WasInCP] && (GameType == BASE ? (Player[i][Team] == ATTACKER) : (Player[i][Team] == capturingTeam)))
		{
			countInCP++;
			if (countInCP == 1)
				format(firstPlayer, sizeof(firstPlayer), "%s", Player[i][Name]);
			else if (countInCP == 2)
				format(secondPlayer, sizeof(secondPlayer), "%s", Player[i][Name]);
		}
	}

	new cpText[128];
	if (countInCP == 0)
		format(cpText, sizeof(cpText), "%sTAKING THE CP!", TDC[capturingTeam]);
	else if (countInCP == 1)
		format(cpText, sizeof(cpText), "%s%s ~w~is taking the cp!", TDC[capturingTeam], firstPlayer);
	else if (countInCP == 2)
		format(cpText, sizeof(cpText), "%s%s, %s ~w~are taking the cp!", TDC[capturingTeam], firstPlayer, secondPlayer);
	else
		format(cpText, sizeof(cpText), "%s%s (+%d) ~w~are taking the cp!", TDC[capturingTeam], firstPlayer, countInCP - 1);

	new visibleLen = 0, idx = 0;
	while (cpText[idx] != EOS)
	{
		if (cpText[idx] == '~')
		{
			new j = idx + 1;
			while (cpText[j] != EOS && cpText[j] != '~' && (j - idx) <= 4)
				j++;
			if (cpText[j] == '~')
			{
				idx = j + 1;
				continue;
			}
		}
		visibleLen++;
		idx++;
	}

	new Float:boardWidth = float(visibleLen) * 3.6 + 16.0;
	if (boardWidth < 88.0)
		boardWidth = 88.0;
	else if (boardWidth > 140.0)
		boardWidth = 140.0;

	new Float:boardX = 320.0 - (boardWidth / 2.0);

	static Float:s_CurrentCPBoardWidth = 0.0;
	if (floatabs(s_CurrentCPBoardWidth - boardWidth) > 1.0)
	{
		s_CurrentCPBoardWidth = boardWidth;
		if (ModernRound_CPBoard != Text:INVALID_TEXT_DRAW)
			TextDrawDestroy(ModernRound_CPBoard);

		ModernRound_CPBoard = TextDrawCreate(boardX, 374.241, "LD_CARD:cd9s");
		TextDrawTextSize(ModernRound_CPBoard, boardWidth, 33.806);
		TextDrawAlignment(ModernRound_CPBoard, 1);
		TextDrawColour(ModernRound_CPBoard, 0x0000004D);
		TextDrawSetShadow(ModernRound_CPBoard, 1);
		TextDrawSetOutline(ModernRound_CPBoard, 0);
		TextDrawBackgroundColour(ModernRound_CPBoard, 0x000000FF);
		TextDrawFont(ModernRound_CPBoard, 4);
		TextDrawSetProportional(ModernRound_CPBoard, true);
	}

	new remainingCP = CurrentCPTime;
	if (remainingCP < 0)
		remainingCP = 0;
	else if (remainingCP > ConfigCPTime)
		remainingCP = ConfigCPTime;

	TextDrawSetString(ModernRound_CPText, cpText);
	va_TextDrawSetString(ModernRound_CPPercent, "%s%d~w~/%d", TDC[capturingTeam], remainingCP, ConfigCPTime);

	TextDrawSetString(SimpleRound_CPText, cpText);
	va_TextDrawSetString(SimpleRound_CPPercent, "%s%d~w~/%d", TDC[capturingTeam], remainingCP, ConfigCPTime);

	foreach (new p : Player)
	{
		if (!Player[p][Spawned] || Player[p][Spectating] || (Player[p][Team] != ATTACKER && Player[p][Team] != DEFENDER))
			continue;

		if (Player[p][RoundTDStyle] == 0)
		{
			TextDrawShowForPlayer(p, ModernRound_CPBoard);
			TextDrawShowForPlayer(p, ModernRound_CPText);
			TextDrawShowForPlayer(p, ModernRound_CPPercent);
		}
		else if (Player[p][RoundTDStyle] == 2)
		{
			TextDrawShowForPlayer(p, SimpleRound_CPText);
			TextDrawShowForPlayer(p, SimpleRound_CPPercent);
		}
	}
	return 1;
}

ShowRoundStats(playerid, bool: updateKillDamage = true)
{
	if (Current == -1 
		|| (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER)
		|| Player[playerid][InDM] || Player[playerid][InDuel] || Player[playerid][IsAFK]
		|| (!Player[playerid][Playing] && !Player[playerid][ToAddInRound] && !Player[playerid][Spectating]))
	{
		HideRoundStats(playerid);
		return 1;
	}

	HideRoundStats(playerid);

	if (ViewRoundCountdown == 0)
	{
		ConfigureRoundKillDmgTDmg(playerid);
		if (updateKillDamage)
			UpdateRoundKillDmgTDmg(playerid);
		else
			UpdateRoundKillDmgText(playerid);
		ShowRoundKillDmgTextDraws(playerid);
	}

	if (RoundPaused)
		TextDrawShowForPlayer(playerid, PauseTD);

	UpdateBaseIDTextDraw(playerid);

	switch (Player[playerid][RoundTDStyle])
	{
		/**** Modern Round Textdraws ****/
		case 0:
		{
			ShowModernRoundHUD(playerid);
		}
		/**** Bulletproof Gamemode Textdraws ****/
		case 1:
		{
			TextDrawShowForPlayer(playerid, leftRedBG);
			TextDrawShowForPlayer(playerid, rightBlueBG);
			TextDrawShowForPlayer(playerid, timerCenterTD);
			TextDrawShowForPlayer(playerid, timerCircleTD);
			TextDrawShowForPlayer(playerid, leftTeamData);
			TextDrawShowForPlayer(playerid, rightTeamData);
			TextDrawShowForPlayer(playerid, centerTeamNames);
			TextDrawShowForPlayer(playerid, attackerNameTD);
			TextDrawShowForPlayer(playerid, defenderNameTD);
			if (!Player[playerid][Spectating])
			{
				TextDrawShowForPlayer(playerid, AttHpLose);
				TextDrawShowForPlayer(playerid, DefHpLose);
				TextDrawShowForPlayer(playerid, TeamHpLose[0]);
				TextDrawShowForPlayer(playerid, TeamHpLose[1]);
				if (PlayersInCP > 0)
					TextDrawShowForPlayer(playerid, EN_CheckPoint);
			}
		}
		/**** Simple Attack-Defend Textdraws ****/
		case 2:
		{
			ShowSimpleRoundHUD(playerid);
		}
		/**** Team Status Icons Textdraws ****/
		case 3:
		{
			UpdateRoundTeamStatusHUD();
			for (new i = 0; i < sizeof(RoundTeamStatusHUD); i++)
				TextDrawShowForPlayer(playerid, RoundTeamStatusHUD[i]);
			UpdateRoundTeamStatusMessage(playerid);
			UpdateRoundTeamStatusTextDraws();
		}
		/**** Team HP Bars Textdraws ****/
		case 4:
		{
			if (AllVsAllMode || !RoundTeamHPHUDReady[playerid])
			{
				ShowModernRoundHUD(playerid);
			}
			else
			{
				new Float: attackerWidth,
					Float: defenderWidth;
				GetRoundTeamHPHUDBarWidths(attackerWidth, defenderWidth);
				RoundTeamHPHUDShown[playerid] = false;
				ApplyRoundTeamHPHUDBars(playerid, attackerWidth, defenderWidth, true);
				UpdateRoundTeamStatusHUD();
				for (new eRoundTeamHPHUDText:i = RoundTeamHPHUD_Background; i <= RoundTeamHPHUD_DefenderTeam; i++)
					PlayerTextDrawShow(playerid, PlayerText:RoundTeamHPHUDText[playerid][i]);
				RoundTeamHPHUDShown[playerid] = true;
				if (PlayersInCP > 0)
					UpdateRoundTeamHPCPHUD();
			}
			ShowTeamBarsForPlayer(playerid);
		}
	}
	return 1;
}


HideRoundStats(playerid)
{
	HideRoundKillDmgTextDraws(playerid);

	TextDrawHideForPlayer(playerid, PauseTD);
	PlayerTextDrawHide(playerid, BaseID_VS[playerid]);

	// Modern Round HUD
	HideModernRoundHUD(playerid);

	// Simple Round HUD
	HideSimpleRoundHUD(playerid);

	// Bulletproof Round HUD
	TextDrawHideForPlayer(playerid, leftRedBG);
	TextDrawHideForPlayer(playerid, rightBlueBG);
	TextDrawHideForPlayer(playerid, timerCenterTD);
	TextDrawHideForPlayer(playerid, timerCircleTD);
	TextDrawHideForPlayer(playerid, leftTeamData);
	TextDrawHideForPlayer(playerid, rightTeamData);
	TextDrawHideForPlayer(playerid, centerTeamNames);
	TextDrawHideForPlayer(playerid, attackerNameTD);
	TextDrawHideForPlayer(playerid, defenderNameTD);
	TextDrawHideForPlayer(playerid, AttHpLose);
	TextDrawHideForPlayer(playerid, DefHpLose);
	TextDrawHideForPlayer(playerid, TeamHpLose[0]);
	TextDrawHideForPlayer(playerid, TeamHpLose[1]);
	TextDrawHideForPlayer(playerid, EN_CheckPoint);

	// Team Status Icons HUD
	for (new team = ATTACKER; team <= DEFENDER; team++)
	{
		for (new slot = 0; slot < MAX_ROUND_TEAM_STATUS_ICONS; slot++)
			TextDrawHideForPlayer(playerid, RoundTeamStatusIcons[team][slot]);
	}
	for (new i = 0; i < sizeof(RoundTeamStatusHUD); i++)
		TextDrawHideForPlayer(playerid, RoundTeamStatusHUD[i]);
	PlayerTextDrawHide(playerid, RoundTeamStatusMessage[playerid]);
	PlayerTextDrawHide(playerid, RoundTeamStatusCountdown[playerid]);

	// Team HP Bars HUD
	RoundTeamHPHUDShown[playerid] = false;
	for (new eRoundTeamHPHUDText:i = RoundTeamHPHUD_Background; i <= RoundTeamHPHUD_DefenderDamageText; i++)
		PlayerTextDrawHide(playerid, PlayerText:RoundTeamHPHUDText[playerid][i]);
	HideRoundTeamHPCPHUDForPlayer(playerid);
	HideAllTeamBarsForPlayer(playerid);
	return 1;
}

ResetRoundTextDraws()
{
	HideRoundTeamHPCPHUDForAll();
	for (new team = ATTACKER; team <= DEFENDER; team++)
	{
		for (new slot = 0; slot < MAX_ROUND_TEAM_STATUS_ICONS; slot++)
			TextDrawHideForAll(RoundTeamStatusIcons[team][slot]);
	}
	for (new i = 0; i < sizeof(RoundTeamStatusHUD); i++)
		TextDrawHideForAll(RoundTeamStatusHUD[i]);
	format(RoundTeamStatusTimer, sizeof(RoundTeamStatusTimer), "_");
	TextDrawSetString(RoundTeamStatusHUD[0], "_");
	TextDrawSetString(RoundTeamStatusHUD[1], "_");
	TextDrawSetString(RoundTeamStatusHUD[2], "_");

	/**** Modern Round Textdraws ****/
	TextDrawSetString(ModernRound_AttTeamName, "_");
	TextDrawSetString(ModernRound_DefTeamName, "_");
	TextDrawSetString(ModernRound_Timer, "_");
	TextDrawSetString(ModernRound_AttHP, "_");
	TextDrawSetString(ModernRound_DefHP, "_");
	TextDrawSetString(ModernRound_AttDmgText, " ");
	TextDrawSetString(ModernRound_DefDmgText, " ");
	TextDrawSetString(ModernRound_CPText, "_");
	TextDrawSetString(ModernRound_CPPercent, "_");
	TextDrawColour(ModernRound_AttDmgBoard, 0x0000006A);
	TextDrawColour(ModernRound_DefDmgBoard, 0x0000006A);
	TextDrawHideForAll(ModernRound_AttDmgBoard);
	TextDrawHideForAll(ModernRound_AttDmgText);
	TextDrawHideForAll(ModernRound_DefDmgBoard);
	TextDrawHideForAll(ModernRound_DefDmgText);
	TextDrawHideForAll(ModernRound_CPBoard);
	TextDrawHideForAll(ModernRound_CPText);
	TextDrawHideForAll(ModernRound_CPPercent);

	/**** Simple Round Textdraws ****/
	TextDrawSetString(SimpleRound_AttTeamName, "_");
	TextDrawSetString(SimpleRound_DefTeamName, "_");
	TextDrawSetString(SimpleRound_Timer, "_");
	TextDrawSetString(SimpleRound_AttHP, "_");
	TextDrawSetString(SimpleRound_DefHP, "_");
	TextDrawSetString(SimpleRound_AttDmgText, " ");
	TextDrawSetString(SimpleRound_DefDmgText, " ");
	TextDrawSetString(SimpleRound_CPText, "_");
	TextDrawSetString(SimpleRound_CPPercent, "_");
	TextDrawHideForAll(SimpleRound_AttDmgText);
	TextDrawHideForAll(SimpleRound_DefDmgText);
	TextDrawHideForAll(SimpleRound_CPLine);
	TextDrawHideForAll(SimpleRound_CPText);
	TextDrawHideForAll(SimpleRound_CPPercent);

	/**** Bulletproof Gamemode Textdraws ****/
	TextDrawColor(timerCircleTD, 0x00000033);
	TextDrawSetString(leftTeamData, "_");
	TextDrawSetString(rightTeamData, "_");
	TextDrawSetString(centerTeamNames, "_");
	TextDrawSetString(timerCenterTD, "_");

	/**** Simple Attack-Defend Textdraws ****/
	TextDrawSetString(AttDef_RoundStats, "_");
	TextDrawSetString(AttDef_RoundTimer, "_");
}

UpdateRoundTeamStatusTextDraws()
{
	for (new team = ATTACKER; team <= DEFENDER; team++)
	{
		for (new slot = 0; slot < MAX_ROUND_TEAM_STATUS_ICONS; slot++)
		{
			RoundTeamStatusIconUsed[team][slot] = false;
			RoundTeamStatusIconAlive[team][slot] = false;
		}

		new slot;
		foreach (new playerid : Player)
		{
			if (!Player[playerid][WasInBase] || Player[playerid][Team] != team)
				continue;
			if (slot >= MAX_ROUND_TEAM_STATUS_ICONS)
				break;

			RoundTeamStatusIconUsed[team][slot] = true;
			RoundTeamStatusIconAlive[team][slot] = Player[playerid][Playing];
			slot++;
		}

		for (new icon = 0; icon < MAX_ROUND_TEAM_STATUS_ICONS; icon++)
		{
			if (!RoundTeamStatusIconUsed[team][icon])
				continue;

			switch (team)
			{
				case ATTACKER:
					TextDrawColor(RoundTeamStatusIcons[team][icon], RoundTeamStatusIconAlive[team][icon] ? 232664575 : 0x0DDE2D77);
				case DEFENDER:
					TextDrawColor(RoundTeamStatusIcons[team][icon], RoundTeamStatusIconAlive[team][icon] ? 0x375FFFFF : 0x375FFF77);
			}
		}
	}

	foreach (new viewerid : Player)
	{
		if (Player[viewerid][RoundTDStyle] != 3 || Current == -1 || (Player[viewerid][Team] != ATTACKER && Player[viewerid][Team] != DEFENDER) || Player[viewerid][InDM] || Player[viewerid][InDuel] || Player[viewerid][IsAFK] || ViewRoundCountdown > 0)
		{
			for (new team = ATTACKER; team <= DEFENDER; team++)
				for (new slot = 0; slot < MAX_ROUND_TEAM_STATUS_ICONS; slot++)
					TextDrawHideForPlayer(viewerid, RoundTeamStatusIcons[team][slot]);
			continue;
		}

		for (new team = ATTACKER; team <= DEFENDER; team++)
		{
			for (new slot = 0; slot < MAX_ROUND_TEAM_STATUS_ICONS; slot++)
			{
				if (RoundTeamStatusIconUsed[team][slot])
					TextDrawShowForPlayer(viewerid, RoundTeamStatusIcons[team][slot]);
				else
					TextDrawHideForPlayer(viewerid, RoundTeamStatusIcons[team][slot]);
			}
		}
	}
	UpdateRoundTeamStatusMessages();
	UpdateRoundTeamStatusHUD();
	return 1;
}

UpdateRoundTeamStatusMessage(playerid)
{
	if (Player[playerid][RoundTDStyle] != 3 || Current == -1 || Player[playerid][Team] == REFEREE)
	{
		PlayerTextDrawHide(playerid, RoundTeamStatusMessage[playerid]);
		PlayerTextDrawHide(playerid, RoundTeamStatusCountdown[playerid]);
		return 1;
	}

	new message[96];
	message[0] = EOS;
	if (ViewRoundCountdown > 0)
	{
		format(message, sizeof(message), "~r~~h~ROUND STARTS IN");
		PlayerTextDrawSetPos(playerid, RoundTeamStatusMessage[playerid], 321.0, 425.0);
		PlayerTextDrawSetString(playerid, RoundTeamStatusCountdown[playerid], va_return("~w~~h~%d", ViewRoundCountdown));
		PlayerTextDrawShow(playerid, RoundTeamStatusCountdown[playerid]);
		TextDrawHideForPlayer(playerid, RoundTeamStatusHUD[0]);
	}
	else
	{
		PlayerTextDrawSetPos(playerid, RoundTeamStatusMessage[playerid], 320.0, 380.0);
		PlayerTextDrawHide(playerid, RoundTeamStatusCountdown[playerid]);
		if (PlayersInCP > 0)
		{
			new capturingTeam = GameType == BASE ? ATTACKER : TeamCapturingCP;
			if (Player[playerid][Team] != ATTACKER && Player[playerid][Team] != DEFENDER)
			{
				message[0] = EOS;
			}
			else if (Player[playerid][Team] == capturingTeam)
			{
				new progress = ConfigCPTime - CurrentCPTime + 1;
				if (progress < 0)
					progress = 0;
				else if (progress > ConfigCPTime)
					progress = ConfigCPTime;

				if (GameType == BASE)
					format(message, sizeof(message), "~w~~h~CAPTURING THE BASE~n~~g~~h~%d/%d", progress, ConfigCPTime);
				else
					format(message, sizeof(message), "~w~~h~CAPTURING THE CP~n~%s%d/%d", TDC[capturingTeam], progress, ConfigCPTime);
			}
			else if (GameType == BASE)
				format(message, sizeof(message), "~b~~h~ALERT! OUR CP IS BEING CAPTURED~n~~w~~h~%d", CurrentCPTime);
			else
				format(message, sizeof(message), "~b~~h~ALERT! ENEMY IS CAPTURING THE CP~n~~w~~h~%d", CurrentCPTime);
		}
	}

	if (message[0] == EOS)
	{
		TextDrawShowForPlayer(playerid, RoundTeamStatusHUD[0]);
		PlayerTextDrawHide(playerid, RoundTeamStatusMessage[playerid]);
		return 1;
	}

	PlayerTextDrawSetString(playerid, RoundTeamStatusMessage[playerid], message);
	PlayerTextDrawShow(playerid, RoundTeamStatusMessage[playerid]);
	if (ViewRoundCountdown == 0)
		TextDrawShowForPlayer(playerid, RoundTeamStatusHUD[0]);
	return 1;
}

UpdateRoundTeamStatusMessages()
{
	foreach (new playerid : Player)
		UpdateRoundTeamStatusMessage(playerid);
	UpdateRoundTeamHPCPHUD();
	return 1;
}

