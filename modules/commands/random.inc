#if defined _attdef_cmd_random
	#endinput
#endif
#define _attdef_cmd_random

YCMD: random(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "start a random round.");
	if (Current != -1)
		return SendErrorMessage(playerid, "A round is in progress, please wait for it to end.");
	if (!AllowStartBase)
		return SendErrorMessage(playerid, "Please wait.");
	if (isnull(params) || IsNumeric(params))
		return SendUsageMessage(playerid, "/random [base | arena | allvsall]");

	switch (YHash(params, false))
	{
		case _I<base>: DoRandomBase(playerid);
		case _I<arena>: DoRandomArena(playerid);
		case _I<allvsall>: DoRandomAllVsAll(playerid);
		default: return SendUsageMessage(playerid, "/random [base | arena | allvsall]");
	}
	return 1;
}
