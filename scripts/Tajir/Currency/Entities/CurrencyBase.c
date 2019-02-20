/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class CurrencyBase extends Inventory_Base
{

	/**
	 * @brief      Gets the currency value.
	 *
	 * @return     The currency value.
	 */
	int GetCurrencyValue()
	{
		return GetGame().ConfigGetInt( "CfgVehicles " + ClassName() + " currencyValue" );
	}

	/**
	 * @brief      Gets the currency name singular.
	 *
	 * @return     The currency name singular.
	 */
	string GetCurrencyNameSingular()
	{
		string ret;
		GetGame().ConfigGetText( "CfgVehicles " + ClassName() + " currencyNameSingular", ret );
		return ret;
	}

	/**
	 * @brief      Gets the currency name plural.
	 *
	 * @return     The currency name plural.
	 */
	string GetCurrencyNamePlural()
	{
		string ret;
		GetGame().ConfigGetText( "CfgVehicles " + ClassName() + " currencyNamePlural", ret );
		return ret;
	}

	/**
	 * @brief      Gets the currency symbol.
	 *
	 * @return     The currency symbol.
	 */
	string GetCurrencySymbol()
	{
		string ret;
		GetGame().ConfigGetText( "CfgVehicles " + ClassName() + " currencySymbol", ret );
		return ret;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  player  The player
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void OnInventoryEnter( Man player )
	{
		super.OnInventoryEnter( player );

		ManBase man = ManBase.Cast( player );

		if ( man.GetHumanInventory().GetEntityInHands() == this )
		{
			return;
		}

		RedeemCurrency( man );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  InventoryLocation  The inventory location
	 * @param[in]  InventoryLocation  The inventory location
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void EEItemLocationChanged( notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc )
	{
		super.EEItemLocationChanged( oldLoc, newLoc );

		if ( newLoc.GetType() == InventoryLocationType.CARGO )
		{
			ManBase owner = ManBase.Cast( GetHierarchyRootPlayer() );

			if ( owner )
			{
				RedeemCurrency( owner );
			}			
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  old_owner  The old owner
	 * @param[in]  new_owner  The new owner
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void OnItemLocationChanged( EntityAI old_owner, EntityAI new_owner )
	{
		super.OnItemLocationChanged( old_owner, new_owner );
	}

	protected void RedeemCurrency( notnull ManBase redeemer )
	{
		if ( GetGame().IsServer() )
		{
			redeemer.AddCurrency( GetCurrencyValue() );
			Delete();
			SetSynchDirty();
		}
	}
}