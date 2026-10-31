#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_textdraws
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_textdraws
	#endinput
#endif
#define _attdef_impl_textdraws

/*
	All textdraws: Global, per-player and 3D
	Modularized into sub-modules under legacy/modules/textdraws/
*/

#include "modules/textdraws/scoreboard.inc"
#include "modules/textdraws/results.inc"
#include "modules/textdraws/player_hud.inc"
#include "modules/textdraws/styles.inc"

LoadTextDraws()
{
	TextDrawCreate(-1000.0, -1000.0, "_");

	AnnTD = TextDrawCreate(315.796, 124.280, "_");
	TextDrawLetterSize(AnnTD, 0.3738, 1.6829);
	TextDrawAlignment(AnnTD, 2);
	TextDrawColor(AnnTD, -1);
	TextDrawSetShadow(AnnTD, 0);
	TextDrawSetOutline(AnnTD, 1);
	TextDrawBackgroundColor(AnnTD, 0x000000FF);
	TextDrawFont(AnnTD, 1);
	TextDrawSetProportional(AnnTD, 1);

	LoadMatchScoreTextDraws();


	//  - End Round TextDraw -

	EN_AttackerBox = TextDrawCreate(178.500000 + ATTACKER_CHANGES_X, 176.00000 + ATTACKER_CHANGES_Y, "~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~");
	TextDrawAlignment(EN_AttackerBox, 2);
	TextDrawFont(EN_AttackerBox, 1);
	TextDrawLetterSize(EN_AttackerBox, 0.31, 1.45);
	TextDrawBackgroundColor(EN_AttackerBox, 0xFF444444);
	TextDrawColor(EN_AttackerBox, 16711935);
	TextDrawSetOutline(EN_AttackerBox, 1);
	TextDrawSetProportional(EN_AttackerBox, 1);
	TextDrawSetShadow(EN_AttackerBox, 0);
	TextDrawTextSize(EN_AttackerBox, 20, 180);
	TextDrawUseBox(EN_AttackerBox, 1);
	TextDrawBoxColor(EN_AttackerBox, 0xFF444444);

	EN_DefenderBox = TextDrawCreate(460.000000 + DEFENDER_CHANGES_X, 176.00000 + DEFENDER_CHANGES_Y, "~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~");
	TextDrawAlignment(EN_DefenderBox, 2);
	TextDrawFont(EN_DefenderBox, 1);
	TextDrawLetterSize(EN_DefenderBox, 0.31, 1.45);
	TextDrawBackgroundColor(EN_DefenderBox, 0x3388FF44);
	TextDrawColor(EN_DefenderBox, 16711935);
	TextDrawSetOutline(EN_DefenderBox, 1);
	TextDrawSetProportional(EN_DefenderBox, 1);
	TextDrawSetShadow(EN_DefenderBox, 0);
	TextDrawTextSize(EN_DefenderBox, 20, 180); //20, 220
	TextDrawUseBox(EN_DefenderBox, 1);
	TextDrawBoxColor(EN_DefenderBox, 0x3388FF44);

	RoundTextdrawsCreate();

	ResultTextdrawsCreate();
	HPBars_ResultTextdrawsCreate();
	TeamIcons_ResultTextdrawsCreate();
	SpectateTextdrawsCreate();
	TargetInfoTextdrawsCreate();

	EN_CheckPoint = TextDrawCreate(182.000000, 280.000000, "_");
	TextDrawAlignment(EN_CheckPoint, 1);
	TextDrawBackgroundColor(EN_CheckPoint, MAIN_BACKGROUND_COLOUR);
	TextDrawFont(EN_CheckPoint, 1);
	TextDrawLetterSize(EN_CheckPoint, 0.18000, 0.9);
	TextDrawColor(EN_CheckPoint, -1);
	TextDrawSetOutline(EN_CheckPoint, 1);
	TextDrawSetProportional(EN_CheckPoint, 1);

	CheckpointWarningBox = TextDrawCreate(320.000000, 0.000000, "_");
	TextDrawAlignment(CheckpointWarningBox, 2);
	TextDrawBackgroundColor(CheckpointWarningBox, 255);
	TextDrawFont(CheckpointWarningBox, 1);
	TextDrawLetterSize(CheckpointWarningBox, 0.500000, 55.000000);
	TextDrawColor(CheckpointWarningBox, -1);
	TextDrawSetOutline(CheckpointWarningBox, 0);
	TextDrawSetProportional(CheckpointWarningBox, 1);
	TextDrawSetShadow(CheckpointWarningBox, 1);
	TextDrawUseBox(CheckpointWarningBox, 1);
	TextDrawBoxColor(CheckpointWarningBox, 0xFF222210);
	TextDrawTextSize(CheckpointWarningBox, 0.000000, 640.000000);

	//  - End of Round Textdraw -


	return 1;
}

