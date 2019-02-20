/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class CfgPatches
{
	class Tajir
	{
		units[]={
			"CurrencyBase",
			"DynamicCurrencyBase",
			"Dollar",
			"Dollar_5",
			"Dollar_10",
			"Dollar_20",
			"Dollar_50",
			"Dollar_100",
			"DynamicDollar"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={
	  		"DZ_Data"
		};
  	};
};

class CfgMods
{
	class Tajir
	{
		dir         = "Tajir";
		picture     = "";
		action      = "";
		hideName    = 1;
		hidePicture = 1;
		name        = "Tajir";
		credits     = "Gassan Idriss";
		author      = "Gassan Idriss";
		authorID    = "0";
		version     = "1.0";
		extra       = 0;
		type        = "mod";

		dependencies[] = { "World", "Mission" };

		class defs
		{
	 		class worldScriptModule
	  		{
				value = "";
				files[] = { "Tajir/scripts/Defines.c", "Tajir/scripts/Tajir", "Tajir/scripts/4_World" };
	 		 };

			class missionScriptModule
			{
				value = "";
				files[] = { "Tajir/scripts/Defines.c", "Tajir/scripts/Tajir_Mission", "Tajir/scripts/5_Mission" };
			};
		};
	};
};

/**
 * @brief      CfgVehicles
 */
class CfgVehicles
{
	/**
	* @brief      Currency
	*/
	class Inventory_Base;

	class CurrencyBase: Inventory_Base
	{
		scope=0;
		canBeSplit=1;
		rotationFlags=17;
		lootCategory="Materials";
		lootTag[]=
		{
		  "Civilian",
		  "Work"
		};
		itemSize[]={1,1};
		weight=0.05;
		varQuantityInit=1;
		varQuantityMin=0;
		varQuantityMax=1;
		varQuantityDestroyOnMin=1;
		destroyOnEmpty=1;
		isMeleeWeapon=0;
		absorbency=0;

		/** Specific to CurrencyBase */
		currencyValue=0;
		currencyNameSingular="";
		currencyNamePlural="";
		currencySymbol="";

		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLabels[]={1,0.69999999,0.5,0.30000001,0};
					healthLevels[]={ { 1, {} }, { 0.5, {} }, { 0, {} } };
				};
			};
		};
	};

	class DynamicCurrencyBase: CurrencyBase
	{
		minRandomCurrency=1;
		maxRandomCurrency=500;
	};

	class Dollar : CurrencyBase
	{
		scope=2;
		displayName="Dollar";
		descriptionShort="American dollars are accepted everywhere.";
		model="Tajir\assets\5stack.p3d";
		currencyValue=1;
		currencyNameSingular="Dollar";
		currencyNamePlural="Dollars";
		currencySymbol="$";
	};

	class Dollar_5 : Dollar
	{
		scope=2;
		displayName="5 Dollar Bill";
		descriptionShort="American dollars are accepted everywhere.";
		model="Tajir\assets\5stack.p3d";
		currencyValue=5;
	};

	class Dollar_10 : Dollar
	{
		scope=2;
		displayName="10 Dollar Bill";
		descriptionShort="American dollars are accepted everywhere.";
		model="Tajir\assets\5stack.p3d";
		currencyValue=10;
	};

	class Dollar_20 : Dollar
	{
		scope=2;
		displayName="20 Dollar Bill";
		descriptionShort="American dollars are accepted everywhere.";
		model="Tajir\assets\5stack.p3d";
		currencyValue=20;
	};

	class Dollar_50 : Dollar
	{
		scope=2;
		displayName="50 Dollar Bill";
		descriptionShort="American dollars are accepted everywhere.";
		model="Tajir\assets\5stack.p3d";
		currencyValue=50;
	};

	class Dollar_100 : Dollar
	{
		scope=2;
		displayName="100 Dollar Bill";
		descriptionShort="American dollars are accepted everywhere.";
		model="Tajir\assets\5stack.p3d";
		currencyValue=100;
	};

	class DynamicDollar: DynamicCurrencyBase
	{
		scope=2;
		displayName="%1%2 Dollars";
		descriptionShort="American dollars are accepted everywhere.";
		model="Tajir\assets\5stack.p3d";
		currencyValue=0;
	};
};