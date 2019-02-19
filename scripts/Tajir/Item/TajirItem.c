/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirItem
{
	protected TajirItemConfig m_config;

	protected ref array<ref TajirItemBundle> m_bundleItems; 

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirItemConfig  The tajir item configuration
	 */
	void TajirItem( notnull ref TajirItemConfig config )
	{
		m_config 		= config;
		m_bundleItems 	= new array<ref TajirItemBundle>;

		for ( int i = 0; i <  m_config.items.Count(); i++ )
		{
			AddBundleItem( new TajirItemBundle( this, m_config.items[ i ] ) );
		}
	}

	/**
	 * @brief      Gets the configuration.
	 *
	 * @return     The configuration.
	 */
	TajirItemConfig GetConfig()
	{
		return m_config;
	}
	
	/**
	 * @brief      Gets the identifier.
	 *
	 * @return     The identifier.
	 */
	int GetId()
	{
		return m_config.id;
	}

	/**
	 * @brief      Gets the object name.
	 *
	 * @return     The object name.
	 */
	string 	GetObjectName()
	{
		return m_config.object_name;
	}

	/**
	 * @brief      Gets the display name.
	 *
	 * @return     The display name.
	 */
	string 	GetDisplayName()
	{
		if ( m_config.display_name.Length() )
		{
			return m_config.display_name;
		}

		return GetObjectConfig( m_config.object_name, "displayName" );
	}

	/**
	 * @brief      Gets the description.
	 *
	 * @return     The description.
	 */
	string GetDescription()
	{
		if ( m_config.description.Length() )
		{
			return m_config.description;
		}

		return GetObjectConfig( m_config.object_name, "descriptionShort" );
	}

	/**
	 * @brief      Gets the display object name.
	 *
	 * @return     The display object name.
	 */
	string GetDisplayObjectName()
	{
		return m_config.display_object_name;
	}

	/**
	 * @brief      Adds a bundle item.
	 *
	 * @param[in]  TajirItemBundle  The tajir item bundle
	 *
	 * @return     { description_of_the_return_value }
	 */
	int AddBundleItem( notnull ref TajirItemBundle item )
	{
		m_bundleItems.Insert( item );

		return m_bundleItems.Count();
	}

	/**
	 * @brief      Gets the bundle items.
	 *
	 * @return     The bundle items.
	 */
	array<ref TajirItemBundle> GetBundleItems()
	{
		return m_bundleItems;
	}

	/**
	 * @brief      Determines if bundle.
	 *
	 * @return     True if bundle, False otherwise.
	 */
	bool IsBundle()
	{
		if ( m_bundleItems.Count() )
		{
			return true;
		}

		return false;
	}

	/**
	 * @brief      Gets the sell price.
	 *
	 * @return     The sell price.
	 */
	int GetSellPrice()
	{
		return m_config.sell_price;
	}

	/**
	 * @brief      Gets the buy price.
	 *
	 * @return     The buy price.
	 */
	int GetBuyPrice()
	{
		return m_config.buy_price;
	}
	
	/**
	 * @brief      Get a root level string configuration for a given object by name.
	 *
	 * @param[in]  objectName  The object name
	 * @param[in]  config      The configuration
	 *
	 * @return     string
	 */
	private string GetObjectConfig( string objectName, string config )
	{
		string resolved;

		if ( !GetGame().ConfigGetText( "cfgVehicles " + objectName + " " + config, resolved ) )
		{
			if ( !GetGame().ConfigGetText( "cfgWeapons " + objectName + " " + config, resolved ) )
			{
				if ( !GetGame().ConfigGetText( "CfgMagazines " + objectName + " " + config, resolved ) )
				{
					return objectName;
				}
			}
		}

		return resolved;
	}
}