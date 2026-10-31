#if !defined _attdef_impl_incl
	#error "Module's implementation file include is not allowed here, include it in the correct section of the 'legacy/modules.inc' file."
#endif

#if !defined _attdef_mod_colors
	#error "Module's header file must be included before its implementation file, it should be located in the 'legacy/modules/' folder."
#endif

#if defined _attdef_impl_colors
	#endinput
#endif
#define _attdef_impl_colors

/*
	Gamemode colors: teams, messages... etc
*/

SetTeamDefaultColors()
{
	TextColor[ATTACKER]		= "{2ED955}"; // Vibrant emerald green
	TextColor[ATTACKER_SUB] = "{A4F2B4}"; // Soft light sage green
	TextColor[DEFENDER]     = "{3D82FF}"; // Vibrant royal blue
	TextColor[DEFENDER_SUB] = "{8CE0FC}"; // Soft light icy sky cyan
	TextColor[REFEREE]		= "{FFFF88}";

	TDC[NON]				= "~p~";
	TDC[ATTACKER]			= "~g~";
	TDC[ATTACKER_SUB]		= "~g~~h~";
	TDC[DEFENDER]			= "~b~~h~";
	TDC[DEFENDER_SUB]		= "~b~~h~~h~";
	TDC[REFEREE]			= "~y~~h~";
}
