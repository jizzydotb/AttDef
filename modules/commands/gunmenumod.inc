#if defined _attdef_cmd_gunmenumod
	#endinput
#endif
#define _attdef_cmd_gunmenumod

YCMD: gunmenumod(playerid, params[], help)
{
	if (help)
		return SendCommandHelpMessage(playerid, "put you in gunmenu modification mode.");
	return ShowPlayerGunmenuModification(playerid);
}

ShowPlayerGunmenuModification(playerid1)
{

	inline InlineGunmenuModifySet(playerid, dialogid, response, listitem, string: inputtext[])
	{
		#pragma unused dialogid, listitem
		if (!response)
			return 1;
		new idx = Player[playerid][GunmenuModdingIndex];
		switch (Player[playerid][GunmenuModdingType])
		{
			case GUNMENU_MOD_TYPE_WEAPON:
			{
				new weaponID = GetWeaponID(inputtext);
				if (!IsValidWeapon(weaponID))
				{
					SendErrorMessage(playerid, "Invalid weapon", MSGBOX_TYPE_BOTTOM);
					Dialog_ShowCallback(
						playerid, using inline InlineGunmenuModifySet, DIALOG_STYLE_INPUT, "Changing weapon...",
						va_return("Current weapon: %s\n\nType new weapon name below to change!", ReturnWeaponName(GunmenuData[idx][GunID])), "Set", "Cancel");
					return 1;
				}

				if (GunmenuData[idx][GunID] == weaponID)
				{
					SendErrorMessage(playerid, "Cannot change gunmenu to the same weapon.", MSGBOX_TYPE_BOTTOM);
					return 1;
				}
				for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
				{
					if (GunmenuData[i][GunID] == weaponID)
					{
						SendErrorMessage(playerid, "This weapon is already present in the gunmenu.", MSGBOX_TYPE_BOTTOM);
						return 1;
					}
				}

				new oldWeapon = GunmenuData[idx][GunID];
				new bool: replacingDeagle = oldWeapon == WEAPON_DEAGLE && HalfDeagle;
				if (replacingDeagle)
				{
					if (!SetHalfDeagleMode(false, weaponID))
						return SendErrorMessage(playerid, "Could not save Half Deagle mode. No changes were applied.");
					GunmenuData[idx][GunModel] = GetWeaponModelFromID(weaponID);
					va_SendClientMessageToAll(-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has disabled Half Deagle Mode.", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name]);
				}

				if (!replacingDeagle)
				{
					if (!DatabaseExecuteWrite(
						va_return("UPDATE `Gunmenu` SET `Weapon`=%d WHERE `Weapon`=%d", weaponID, oldWeapon),
						"save Gunmenu weapon"))
						return SendErrorMessage(playerid, "Could not save the Gunmenu weapon. No changes were applied.");
					GunmenuData[idx][GunID] = weaponID;
					GunmenuData[idx][GunModel] = GetWeaponModelFromID(weaponID);
				}

				va_SendClientMessageToAll(
					-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed gunmenu index: "COL_SEC"%d " COL_PRIM "from "COL_SEC"\"%s\" "COL_PRIM"to "COL_SEC"\"%s\"", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], idx,
					ReturnWeaponName(oldWeapon), ReturnWeaponName(weaponID));
			}
			case GUNMENU_MOD_TYPE_LIMIT:
			{
				if (HalfDeagle && GunmenuData[idx][GunID] == WEAPON_DEAGLE)
				{
					SendErrorMessage(playerid, "Cannot change Deagle limit while Half Deagle Mode is active.", MSGBOX_TYPE_BOTTOM);
					return 1;
				}

				new limit = strval(inputtext);
				if (limit < 0)
				{
					SendErrorMessage(playerid, "Selection limit cannot be less than 0", MSGBOX_TYPE_BOTTOM);
					Dialog_ShowCallback(
						playerid, using inline InlineGunmenuModifySet, DIALOG_STYLE_INPUT, "Changing limit...",
						va_return("Current limit of %s: %d\n\nType new limit value below to change!", ReturnWeaponName(GunmenuData[idx][GunID]), GunmenuData[idx][GunLimit]), "Set",
						"Cancel");
					return 1;
				}
				if (!DatabaseExecuteWrite(
					va_return("UPDATE `Gunmenu` SET `Limit`=%d WHERE `Weapon`=%d", limit, GunmenuData[idx][GunID]),
					"save Gunmenu weapon limit"))
					return SendErrorMessage(playerid, "Could not save the Gunmenu weapon limit. No changes were applied.");
				GunmenuData[idx][GunLimit] = limit;
				va_SendClientMessageToAll(
					-1, ""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed "COL_SEC"\"%s\" " COL_PRIM "limit to "COL_SEC"%d", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], ReturnWeaponName(GunmenuData[idx][GunID]), limit);
			}
			case GUNMENU_MOD_TYPE_AMMO:
			{
				new ammo = strval(inputtext);
				if (ammo < 1 || ammo > 9999)
				{
					SendErrorMessage(playerid, "Weapon ammo must be equal to or between 1 and 9999", MSGBOX_TYPE_BOTTOM);
					Dialog_ShowCallback(
						playerid, using inline InlineGunmenuModifySet, DIALOG_STYLE_INPUT, "Changing ammo...",
						va_return("Current ammo of %s: %d\n\nType new ammo value below to change!", ReturnWeaponName(GunmenuData[idx][GunID]), GunmenuData[idx][GunAmmo]), "Set",
						"Cancel");
					return 1;
				}
				if (!DatabaseExecuteWrite(
					va_return("UPDATE `Gunmenu` SET `Ammo`=%d WHERE `Weapon`=%d", ammo, GunmenuData[idx][GunID]),
					"save Gunmenu weapon ammo"))
					return SendErrorMessage(playerid, "Could not save the Gunmenu weapon ammo. No changes were applied.");
				GunmenuData[idx][GunAmmo] = ammo;
				va_SendClientMessageToAll(-1,""COL_PRIM"%s "COL_SEC"%s " COL_PRIM "has changed "COL_SEC"\"%s\" " COL_PRIM "ammo to "COL_SEC"%d", PLAYER_ROLE_NAME(playerid1), Player[playerid][Name], ReturnWeaponName(GunmenuData[idx][GunID]), ammo);
			}
		}
		return 1;
	}

	inline InlineGunmenuModifySub(playerid, dialogid, response, listitem, string: inputtext[])
	{
		#pragma unused dialogid, inputtext
		if (!response)
		{
			ShowPlayerGunmenuModification(playerid);
			return 1;
		}
		new idx = Player[playerid][GunmenuModdingIndex];
		switch (listitem)
		{
			case 0:
			{
				Player[playerid][GunmenuModdingType] = GUNMENU_MOD_TYPE_WEAPON;
				Dialog_ShowCallback(
					playerid, using inline InlineGunmenuModifySet, DIALOG_STYLE_INPUT, "Changing weapon...",
					va_return("Current weapon: %s\n\nType new weapon name below to change!", ReturnWeaponName(GunmenuData[idx][GunID])), "Set", "Cancel");
			}
			case 1:
			{
				Player[playerid][GunmenuModdingType] = GUNMENU_MOD_TYPE_LIMIT;
				Dialog_ShowCallback(
					playerid, using inline InlineGunmenuModifySet, DIALOG_STYLE_INPUT, "Changing limit...",
					va_return("Current limit of %s: %d\n\nType new limit value below to change!", ReturnWeaponName(GunmenuData[idx][GunID]), GunmenuData[idx][GunLimit]), "Set",
					"Cancel");
			}
			case 2:
			{
				Player[playerid][GunmenuModdingType] = GUNMENU_MOD_TYPE_AMMO;
				Dialog_ShowCallback(
					playerid, using inline InlineGunmenuModifySet, DIALOG_STYLE_INPUT, "Changing ammo...",
					va_return("Current ammo of %s: %d\n\nType new ammo value below to change!", ReturnWeaponName(GunmenuData[idx][GunID]), GunmenuData[idx][GunAmmo]), "Set", "Cancel");
			}
		}
		return 1;
	}
	inline InlineGunmenuModifyMain(playerid, dialogid, response, listitem, string: inputtext[])
	{
		#pragma unused dialogid, inputtext
		if (!response)
			return 1;
		Player[playerid][GunmenuModdingIndex] = listitem;
		Dialog_ShowCallback(
			playerid, using inline InlineGunmenuModifySub, DIALOG_STYLE_LIST, va_return("Modifying: %s", ReturnWeaponName(GunmenuData[listitem][GunID])), "Weapon\nLimit\nAmmo",
			"Select", "Back");
		return 1;
	}

	new str[GUNMENU_LIMIT_DIALOG_SIZE];
	strcat(str, "Weapon\tAmmo\tLimit");
	for (new i = 0; i < MAX_GUNMENU_GUNS; i++)
	{
		if (HalfDeagle && GunmenuData[i][GunID] == WEAPON_DEAGLE)
		{
			strcat(str, va_return("\n{50EB50}%s\t%d\t%s", ReturnWeaponName(GunmenuData[i][GunID]), GunmenuData[i][GunAmmo], "Half Deagle Mode"));
		}
		else
		{
			strcat(str, va_return("\n{50EB50}%s\t%d\t%d", ReturnWeaponName(GunmenuData[i][GunID]), GunmenuData[i][GunAmmo], GunmenuData[i][GunLimit]));
		}
	}
	Dialog_ShowCallback(playerid1, using inline InlineGunmenuModifyMain, DIALOG_STYLE_TABLIST_HEADERS, "Gunmenu Modification Mode", str, "Modify", "Cancel");
	return 1;
}
