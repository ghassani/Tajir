/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreCatalogItemPersist
{
	protected int m_storeId;

	protected int m_itemId;

	protected int m_quantity;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  item  The item
	 */
	void TajirStoreCatalogItemPersist( TajirStoreCatalogItem item )
	{
		if ( item )
		{
			m_storeId 		= item.GetCatalog().GetStore().GetId();
			m_itemId 		= item.GetItemId();
			m_quantity 		= item.GetQuantity();
		}
	}

	/**
	 * @brief      Gets the store identifier.
	 *
	 * @return     The store identifier.
	 */
	int GetStoreId()
	{
		return m_storeId;
	}

	/**
	 * @brief      Gets the item identifier.
	 *
	 * @return     The item identifier.
	 */
	int GetItemId()
	{
		return m_itemId;
	}

	/**
	 * @brief      Gets the quantity.
	 *
	 * @return     The quantity.
	 */
	int GetQuantity()
	{
		return m_quantity;
	}
}

