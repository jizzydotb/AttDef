#if !defined _attdef_header_incl
	#error "Module's header file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if defined _attdef_mod_textdraws
	#endinput
#endif
#define _attdef_mod_textdraws

/*
	All textdraws: Global, per-player and 3D
*/

#include "modules/this_core.inc"
#include "modules/weaponshot.inc"

#define ATTACKER_CHANGES_X			 19.4
#define ATTACKER_CHANGES_Y			 0.0
#define DEFENDER_CHANGES_X			 -79
#define DEFENDER_CHANGES_Y			 0.0

// Player interface

#define MAX_PLAYER_INTERFACE_ASPECTS	3

#define INTERFACE_SPEC		0
#define INTERFACE_NET		1
#define INTERFACE_HP		2

#define MAX_VISUAL_DAMAGES	4
#define MAX_ROUND_TEAM_STATUS_ICONS	10
#define ROUND_TEAM_HP_HUD_BAR_CENTER_X	319.0
#define ROUND_TEAM_HP_HUD_BAR_WIDTH	321.0
#define ROUND_TEAM_HP_HUD_BAR_LEFT_X	-2.0
#define ROUND_TEAM_HP_HUD_BAR_RIGHT_X	640.0
#define ROUND_TEAM_HP_HUD_BAR_Y		435.308
#define ROUND_TEAM_HP_HUD_ATTACKER_TEXT_X	230.113
#define ROUND_TEAM_HP_HUD_ATTACKER_TEXT_Y	437.986
#define ROUND_TEAM_HP_HUD_ATTACKER_TEXT_LETTER_X	0.2435
#define ROUND_TEAM_HP_HUD_ATTACKER_TEXT_LETTER_Y	0.7554
#define ROUND_TEAM_HP_HUD_CENTER_TEXT_X		364.700
#define ROUND_TEAM_HP_HUD_CENTER_TEXT_Y		437.269
#define ROUND_TEAM_HP_HUD_CENTER_TEXT_LETTER_X	0.2252
#define ROUND_TEAM_HP_HUD_CENTER_TEXT_LETTER_Y	0.8890
#define ROUND_TEAM_HP_HUD_DEFENDER_TEXT_X	420.891
#define ROUND_TEAM_HP_HUD_DEFENDER_TEXT_Y	437.596
#define ROUND_TEAM_HP_HUD_DEFENDER_TEXT_LETTER_X	0.2428
#define ROUND_TEAM_HP_HUD_DEFENDER_TEXT_LETTER_Y	0.8237
#define ROUND_TEAM_HP_HUD_ATTACKER_COLOR	0x11591B69
#define ROUND_TEAM_HP_HUD_DEFENDER_COLOR	0x102A6363
#define ROUND_TEAM_HP_HUD_ATTACKER_FLASH_COLOR	0x08651569
#define ROUND_TEAM_HP_HUD_DEFENDER_FLASH_COLOR	0x0F1B6D63
#define ROUND_TEAM_HP_HUD_EDGE_SHADOW_WIDTH	2.5
#define ROUND_TEAM_HP_HUD_EDGE_SHADOW_COLOR	120

#define ROUND_TEAM_HP_HUD_CARD_DEFENDER_X	387.775
#define ROUND_TEAM_HP_HUD_CARD_Y		390.711
#define ROUND_TEAM_HP_HUD_CARD_WIDTH		29.958
#define ROUND_TEAM_HP_HUD_CARD_HEIGHT		30.193
#define ROUND_TEAM_HP_HUD_CARD_ATTACKER_X	222.267
#define ROUND_TEAM_HP_HUD_DMG_TEXT_DEFENDER_X	402.754
#define ROUND_TEAM_HP_HUD_DMG_TEXT_ATTACKER_X	237.246
#define ROUND_TEAM_HP_HUD_DMG_TEXT_Y		397.377
#define ROUND_TEAM_HP_HUD_DMG_TEXT_LETTER_X	0.3093
#define ROUND_TEAM_HP_HUD_DMG_TEXT_LETTER_Y	1.6040
#define ROUND_TEAM_HP_HUD_DMG_TEXT_COLOR	0xD22126FF

#define ROUND_TEAM_HP_CP_BAR_START_X		271.000
#define ROUND_TEAM_HP_CP_BAR_MAX_X		366.017
#define ROUND_TEAM_HP_CP_BAR_SPAN_X		95.017
#define ROUND_TEAM_HP_CP_BAR2_OFFSET_X		0.750
#define ROUND_TEAM_HP_CP_BAR2_START_X		271.750
#define ROUND_TEAM_HP_CP_BAR2_MAX_X		365.267

new bool: PlayerInterface[MAX_PLAYERS][MAX_PLAYER_INTERFACE_ASPECTS];

new Text: TeamAttackerTD;
new Text: TeamDefenderTD;

// Scoreboard textdraws
new Text: RoundsPlayed; // Shows how many rounds are played out of for example 9 rounds. (Rounds 3/9)
new Text: TeamScoreText; // Shows team name and score (e.g. TeK 3 CZE 3)

// Default Textdraws to show
new Text: InitTeamAttackerTD;
new Text: InitTeamDefenderTD;

// - Global Textdraws -
new Text: AnnTD;
new Text: PauseTD;
new Text: AttHpLose;
new Text: DefHpLose;
new Text: TeamHpLose[2];
new Text: EN_AttackerBox;
new Text: EN_DefenderBox;
new Text: EN_CheckPoint;
new Text: CheckpointWarningBox;

// - Round Textdraws

new Text: leftRedBG;
new Text: rightBlueBG;
new Text: timerCenterTD, Text:timerCircleTD;
new Text: leftTeamData;
new Text: rightTeamData;
new Text: centerTeamNames;
new Text: attackerNameTD, Text:defenderNameTD;
new Text: AttDef_RoundStats, Text:AttDef_RoundTimer;
new
	Text: ModernRound_BottomBar,
	Text: ModernRound_Timer,
	Text: ModernRound_AttTeamName,
	Text: ModernRound_DefTeamName,
	Text: ModernRound_AttHeart,
	Text: ModernRound_DefHeart,
	Text: ModernRound_AttHP,
	Text: ModernRound_DefHP,
	Text: ModernRound_AttDmgBoard,
	Text: ModernRound_AttDmgText,
	Text: ModernRound_DefDmgBoard,
	Text: ModernRound_DefDmgText,
	Text: ModernRound_CPBoard,
	Text: ModernRound_CPText,
	Text: ModernRound_CPPercent;

new
	Text: SimpleRound_Timer,
	Text: SimpleRound_AttTeamName,
	Text: SimpleRound_DefTeamName,
	Text: SimpleRound_AttHeart,
	Text: SimpleRound_DefHeart,
	Text: SimpleRound_AttHP,
	Text: SimpleRound_DefHP,
	Text: SimpleRound_AttDmgText,
	Text: SimpleRound_DefDmgText,
	Text: SimpleRound_CPLine,
	Text: SimpleRound_CPText,
	Text: SimpleRound_CPPercent;

forward ShowModernRoundHUD(playerid);
forward HideModernRoundHUD(playerid);
forward ShowSimpleRoundHUD(playerid);
forward HideSimpleRoundHUD(playerid);
forward UpdateModernAndSimpleCheckpointHUD();
forward UpdateAllVsAllTextDraws();
new Text: RoundTeamStatusIcons[MAX_TEAMS][MAX_ROUND_TEAM_STATUS_ICONS];
new bool: RoundTeamStatusIconUsed[MAX_TEAMS][MAX_ROUND_TEAM_STATUS_ICONS];
new bool: RoundTeamStatusIconAlive[MAX_TEAMS][MAX_ROUND_TEAM_STATUS_ICONS];
new Text: RoundTeamStatusHUD[3];
new RoundTeamStatusTimer[32];
new PlayerText: RoundTeamStatusCountdown[MAX_PLAYERS];
enum eRoundTeamHPHUDText
{
	PlayerText: RoundTeamHPHUD_Background,
	PlayerText: RoundTeamHPHUD_AttackerBar,
	PlayerText: RoundTeamHPHUD_DefenderBar,
	PlayerText: RoundTeamHPHUD_AttackerDamageBar,
	PlayerText: RoundTeamHPHUD_DefenderDamageBar,
	PlayerText: RoundTeamHPHUD_BothLineOverlay,
	PlayerText: RoundTeamHPHUD_AttackerTeam,
	PlayerText: RoundTeamHPHUD_Center,
	PlayerText: RoundTeamHPHUD_DefenderTeam,
	PlayerText: RoundTeamHPHUD_AttackerCard,
	PlayerText: RoundTeamHPHUD_DefenderCard,
	PlayerText: RoundTeamHPHUD_AttackerDamageText,
	PlayerText: RoundTeamHPHUD_DefenderDamageText
}
new RoundTeamHPHUDText[MAX_PLAYERS][eRoundTeamHPHUDText];

new bool: RoundTeamHPHUDReady[MAX_PLAYERS];
new bool: RoundTeamHPHUDShown[MAX_PLAYERS];
new bool: RoundTeamHPHUDFlashing[MAX_TEAMS];
new bool: TeamDamageFlashing[MAX_TEAMS];
new Float: RoundTeamHPHUDApplied[MAX_PLAYERS][2];

enum eRoundTeamHPCPHUD
{
	PlayerText: RoundTeamHPCP_Card,
	PlayerText: RoundTeamHPCP_BarBorder,
	PlayerText: RoundTeamHPCP_BarEmpty,
	PlayerText: RoundTeamHPCP_BarActiveBorder,
	PlayerText: RoundTeamHPCP_BarActiveFill,
	PlayerText: RoundTeamHPCP_Title,
	PlayerText: RoundTeamHPCP_PlayerName,
	PlayerText: RoundTeamHPCP_TimeText
}
new RoundTeamHPCPHUD[MAX_PLAYERS][eRoundTeamHPCPHUD];
new bool: RoundTeamHPCPReady[MAX_PLAYERS];
new bool: RoundTeamHPCPShown[MAX_PLAYERS];

forward UpdateRoundTeamHPCPHUD();
forward HideRoundTeamHPCPHUDForPlayer(playerid);
forward HideRoundTeamHPCPHUDForAll();

// - Result Textdraws - added by Niko_boy // -

new
	Text: Modern_RoundInfo[3],
	Text: Modern_AttColorBar,
	Text: Modern_DefColorBar,
	Text: Modern_AttBoard,
	Text: Modern_DefBoard,
	Text: Modern_BottomBoard,
	Text: Modern_AttHeaderName,
	Text: Modern_AttHeaderDmg,
	Text: Modern_AttHeaderKills,
	Text: Modern_AttHeaderHealth,
	Text: Modern_DefHeaderName,
	Text: Modern_DefHeaderDmg,
	Text: Modern_DefHeaderKills,
	Text: Modern_DefHeaderHealth,
	Text: Modern_AttNames,
	Text: Modern_AttDamage,
	Text: Modern_AttKills,
	Text: Modern_AttHealth,
	Text: Modern_DefNames,
	Text: Modern_DefDamage,
	Text: Modern_DefKills,
	Text: Modern_DefHealth,
	Text: Modern_AttTeamName,
	Text: Modern_DefTeamName,
	Text: Modern_TeamWhoWon,
	Text: Modern_MVPDamage,
	Text: Modern_MVPLeastDmg,
	Text: Modern_MVPKills;

forward UpdateModernEndRoundStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, winnerid, WinID, Float: ahpleft, Float: dhpleft, aalive, dalive, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc, const attList[], const attDmg[], const attKills[], const attHP[], const defList[], const defDmg[], const defKills[], const defHP[]);
forward UpdateModernMatchStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, matchWinnerID, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc, const attList[], const attDmg[], const attKills[], const attDeaths[], const defList[], const defDmg[], const defKills[], const defDeaths[]);

// - HP Bars Result Textdraws -
new
	Text: HPBars_TopBox,
	Text: HPBars_TopBanner,
	Text: HPBars_Blackboard,
	Text: HPBars_AttBox,
	Text: HPBars_DefBox,
	Text: HPBars_HeaderShadow,
	Text: HPBars_LeftHeader,
	Text: HPBars_RightHeader,
	Text: HPBars_BottomBox,
	Text: HPBars_MVPKills,
	Text: HPBars_MVPDamage,
	Text: HPBars_MVPAcc,

	Text: HPBars_LeftNames,
	Text: HPBars_RoundLeftKills,
	Text: HPBars_RoundLeftHP,
	Text: HPBars_RoundLeftAcc,
	Text: HPBars_RoundLeftDmg,
	Text: HPBars_MatchLeftK,
	Text: HPBars_MatchLeftD,
	Text: HPBars_MatchLeftP,
	Text: HPBars_MatchLeftAcc,
	Text: HPBars_MatchLeftDmg,

	Text: HPBars_RightNames,
	Text: HPBars_RoundRightKills,
	Text: HPBars_RoundRightHP,
	Text: HPBars_RoundRightAcc,
	Text: HPBars_RoundRightDmg,
	Text: HPBars_MatchRightK,
	Text: HPBars_MatchRightD,
	Text: HPBars_MatchRightP,
	Text: HPBars_MatchRightAcc,
	Text: HPBars_MatchRightDmg,

	Text: HPBars_HistBox,
	Text: HPBars_HistHeaderShadow,
	Text: HPBars_HistHeaderBox,
	Text: HPBars_HistHeader,
	Text: HPBars_HistRound,
	Text: HPBars_HistMap,
	Text: HPBars_HistWinner;

new bool: HPBars_IsMatchEnd;

forward HPBars_ResultTextdrawsCreate();
forward ShowHPBarsEndRoundTextDraw(playerid);
forward HideHPBarsEndRoundTextDraw(playerid);
forward UpdateHPBarsEndRoundStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, winnerid, Float: ahpleft, Float: dhpleft, aalive, dalive, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc);
forward UpdateHPBarsMatchStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc);

// - Team Icons Result Textdraws -
new
	Text: TeamIcons_AttScoreName,
	Text: TeamIcons_HeaderCols,
	Text: TeamIcons_SubTitle,
	Text: TeamIcons_AttBoard,
	Text: TeamIcons_AttNames,
	Text: TeamIcons_AttStatus,
	Text: TeamIcons_AttScore,
	Text: TeamIcons_AttKills,
	Text: TeamIcons_AttDamage,
	Text: TeamIcons_DefScoreName,
	Text: TeamIcons_DefBoard,
	Text: TeamIcons_DefNames,
	Text: TeamIcons_DefStatus,
	Text: TeamIcons_DefScore,
	Text: TeamIcons_DefKills,
	Text: TeamIcons_DefDamage,
	Text: TeamIcons_GreenBoard,
	Text: TeamIcons_GreenLabels,
	Text: TeamIcons_GreenNames,
	Text: TeamIcons_GreenValues;

new TeamIcons_LastWinner = -1;
new PlayerText: TeamIcons_WinLoss[MAX_PLAYERS];

forward TeamIcons_ResultTextdrawsCreate();
forward ShowTeamIconsEndRoundTextDraw(playerid);
forward HideTeamIconsEndRoundTextDraw(playerid);
forward UpdateTeamIconsPlayerWinLoss(playerid);
forward UpdateTeamIconsEndRoundStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, winnerid, WinID, Float: ahpleft, Float: dhpleft, aalive, dalive, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc);
forward UpdateTeamIconsMatchStats(array[][rankingEnum], names[][MAX_PLAYER_NAME], p, winnerid, topkillID, topkill, topDmgID, Float: topDmg, topAccID, topAcc);

// - Spectator HUD Textdraws -
new
	Text: Spec_CardBG = Text:INVALID_TEXT_DRAW,
	Text: Spec_HealthLabel = Text:INVALID_TEXT_DRAW,
	Text: Spec_HealthBarBG = Text:INVALID_TEXT_DRAW,
	Text: Spec_ArmourLabel = Text:INVALID_TEXT_DRAW,
	Text: Spec_ArmourBarBG = Text:INVALID_TEXT_DRAW;
new PlayerText: Spec_RoundHeader[MAX_PLAYERS] = {PlayerText:INVALID_PLAYER_TEXT_DRAW, ...};

forward SpectateTextdrawsCreate();

// - TargetInfo Textdraws -
new
	Text: TargetInfo_CardBG = Text:INVALID_TEXT_DRAW,
	Text: TargetInfo_ArmourSlotBG = Text:INVALID_TEXT_DRAW,
	Text: TargetInfo_HealthSlotBG = Text:INVALID_TEXT_DRAW;

forward TargetInfoTextdrawsCreate();

// - Player Textdraws -

new PlayerText:FPSPingPacket[MAX_PLAYERS]; // Ping, FPS and Packetloss textdraw on top right corner.
new PlayerText:FPSPingPacketBox[MAX_PLAYERS]; // Ping, FPS and Packetloss Textdraw box
new PlayerText:RoundStyleKillsTD[MAX_PLAYERS];
new PlayerText:RoundStyleDamageTD[MAX_PLAYERS];
new PlayerText:RoundStyleTotalDmgTD[MAX_PLAYERS];
new PlayerText:RoundBulletproofDmgTD[MAX_PLAYERS]; // Dedicated panel for the legacy Bulletproof style.
new PlayerText:RoundTeamStatusDmgTD[MAX_PLAYERS]; // Separate panel for the Team Status Icons style.
new PlayerText:RoundDamageTD[MAX_PLAYERS]; // Separate Damage line for the Team HP Bars style.
new PlayerText:RoundTotalDamageTD[MAX_PLAYERS]; // Separate Total line for the Team HP Bars style.
// new PlayerText:DoingDamage[MAX_PLAYERS][MAX_VISUAL_DAMAGES]; // Shows when player hits someone.
// new PlayerText:GettingDamaged[MAX_PLAYERS][MAX_VISUAL_DAMAGES]; // Shows when someone hits you.
new PlayerText:FriendlyDamage[MAX_PLAYERS][2]; // Shows when your teammate hits you or you hit them
new PlayerText:WhoSpecHeader[MAX_PLAYERS]; // Header "Spectators:" or "Spectating you:"
new PlayerText:WhoSpec[MAX_PLAYERS]; // Shows who is spectating you.
new PlayerText:SpecText[MAX_PLAYERS][7]; // Shows spectated player info.
new PlayerText:AreaCheckTD[MAX_PLAYERS]; // Show countdown textdraw when the player is out of arena.
new PlayerText:AreaCheckBG[MAX_PLAYERS];
new PlayerText:DeathText[MAX_PLAYERS][2];
new PlayerText:DeathTextBG[MAX_PLAYERS];
forward ShowPlayerDeathNotice(playerid, type, const text[]);
forward HidePlayerDeathNotice(playerid);
new PlayerText:TD_RoundSpec[MAX_PLAYERS];
new PlayerText:PointCounterTD[MAX_PLAYERS];
new PlayerText:BaseID_VS[MAX_PLAYERS];
new PlayerText:RoundTeamStatusMessage[MAX_PLAYERS];
new DeathMessageStr[MAX_PLAYERS][32];

forward LoadMatchScoreTextDraws();
forward ShowMatchScoreBoard(playerid = -1);
forward HideMatchScoreBoard(playerid = -1);
forward UpdateTeamScoreTextDraw();
forward UpdateTeamNameTextDraw();
forward UpdateRoundsPlayedTextDraw();
forward UpdateTeamNamesTextdraw();
forward ShowPauseTDForAll();
forward ShowEndRoundTextDraw(playerid);
forward HideEndRoundTextDraw(playerid);
forward ResultTextdrawsCreate();
forward RoundTextdrawsCreate();
forward CreateRoundTeamStatusTextDraws();
forward CreateRoundTeamStatusHUD();
forward UpdateRoundTeamStatusHUD();
forward UpdateRoundTeamStatusTimer(const text[]);
forward UpdateRoundTeamStatusTextDraws();
forward UpdateRoundTeamStatusMessage(playerid);
forward UpdateRoundTeamStatusMessages();
forward ShowRoundStats(playerid, bool: updateKillDamage = true);
forward HideRoundStats(playerid);
forward ResetRoundTextDraws();
forward LoadPlayerTextDraws(playerid);
forward ConfigureRoundKillDmgTDmg(playerid);
forward ShowRoundKillDmgTextDraws(playerid);
forward HideRoundKillDmgTextDraws(playerid);
forward UpdateRoundKillDmgText(playerid);
forward UpdateRoundKillDmgTDmg(playerid);
forward InitPlayerInterface(playerid);
forward TogglePlayerInterface(playerid, bool: toggle, interface = -1);
forward UpdateRoundTeamHPHUDFlash(team, bool: flashing);
forward ModernTextDrawFlash(team);
forward StopModernTextDrawFlash(team);
