/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreCatalogItem
{
	protected TajirStoreCatalog m_catalog;

	protected TajirItem m_item;

	protected TajirStoreCatalogItemConfig m_config;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirStoreCatalog            The tajir store catalog
	 * @param[in]  TajirItem                    The tajir item
	 * @param[in]  TajirStoreCatalogItemConfig  The tajir store catalog item configuration
	 */
	void TajirStoreCatalogItem( notnull TajirStoreCatalog catalog, notnull TajirItem item, notnull TajirStoreCatalogItemConfig config )
	{
		m_catalog 	= catalog;
		m_item 		= item;
		m_config 	= config;
	}

	/**
	 * @brief      Gets the configuration.
	 *
	 * @return     The configuration.
	 */
	TajirStoreCatalogItemConfig GetConfig()
	{
		return m_config;
	}
	
	/**
	 * @brief      Gets the catalog.
	 *
	 * @return     The catalog.
	 */
	TajirStoreCatalog GetCatalog()
	{
		return m_catalog;
	}

	/**
	 * @brief Get the underlying base item information.
	 * @see TajirItemComponent	
	 *
	 * @return     The item.
	 */
	TajirItem GetItem()
	{
		return m_item;
	}

	/**
	 * @brief      Gets the item identifier.
	 *
	 * @return     The item identifier.
	 */
	int GetId()
	{
		return GetItemId();
	}

	/**
	 * @brief      Gets the item identifier.
	 *
	 * @return     The item identifier.
	 */
	int GetItemId()
	{
		return m_item.GetId();
	}

	/**
	 * @brief      Gets the object name.
	 *
	 * @return     The object name.
	 */
	string 	GetObjectName()
	{
		return m_item.GetObjectName();
	}

	/**
	 * @brief      Gets the display name.
	 *
	 * @return     The display name.
	 */
	string 	GetDisplayName()
	{
		return m_item.GetDisplayName();
	}

	/**
	 * @brief      Gets the description.
	 *
	 * @return     The description.
	 */
	string GetDescription()
	{
		return m_item.GetDescription();
	}

	/**
	 * @brief      Gets the display object name.
	 *
	 * @return     The display object name.
	 */
	string GetDisplayObjectName()
	{
		return m_item.GetDisplayObjectName();
	}

	/**
	 * @brief      Determines ability to buy.
	 *
	 * @return     True if able to buy, False otherwise.
	 */
	bool CanBuy()
	{
		if ( IsBundle() )
		{
			return false;
		}

		return m_config.can_buy;
	}

	/**
	 * @brief      Determines ability to sell.
	 *
	 * @return     True if able to sell, False otherwise.
	 */
	bool CanSell()
	{
		if ( IsBundle() )
		{
			return true;
		}

		return m_config.can_sell;
	}

	/**
	 * @brief      Determines if bundle.
	 *
	 * @return     True if bundle, False otherwise.
	 */
	bool IsBundle()
	{
		return m_item.IsBundle();
	}

	/**
	 * @brief      Gets the bundle items.
	 *
	 * @return     The bundle items.
	 */
	array<ref TajirItemBundle> GetBundleItems()
	{
		return m_item.GetBundleItems();
	}

	/**
	 * @brief      Gets the sell price.
	 *
	 * @return     The sell price.
	 */
	int GetSellPrice()
	{
		return m_item.GetSellPrice();
	}

	/**
	 * @brief      Gets the buy price.
	 *
	 * @return     The buy price.
	 */
	int GetBuyPrice()
	{
		return m_item.GetBuyPrice();
	}

	/**
	 * @brief      Gets the quantity.
	 *
	 * @return     The quantity.
	 */
	int GetQuantity()
	{
		return m_config.quantity;
	}

	/**
	 * @brief      Adds a quantity.
	 *
	 * @param[in]  amount  The amount
	 */
	void AddQuantity( int amount )
	{
		m_config.quantity += amount;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  amount  The amount
	 */
	void DeductQuantity( int amount )
	{
		m_config.quantity -= amount;
	}

	/**
	 * @brief      Sets the quantity.
	 *
	 * @param[in]  amount  The amount
	 */
	void SetQuantity( int amount )
	{
		m_config.quantity = amount;
	}

	/**
	 * @brief      Gets the restock quantity.
	 *
	 * @return     The restock quantity.
	 */
	int GetRestockQuantity()
	{
		return m_config.restock_quantity;
	}
}