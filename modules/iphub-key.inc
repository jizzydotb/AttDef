#if defined _iphub_key_included
	#endinput
#endif
#define _iphub_key_included

#tryinclude "iphub-key.local.inc"

#if !defined IPHUB_KEY
	#define IPHUB_KEY "REPLACE_WITH_YOUR_IPHUB_KEY"
#endif
