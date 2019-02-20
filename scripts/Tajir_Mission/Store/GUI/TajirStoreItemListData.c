/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreItemListData
{
	protected TajirStoreCatalogItem   		m_item;

	protected EntityAI 					 	m_object;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirStoreCatalogItem  The tajir store catalog item
	 * @param[in]  object                 The object
	 */
	void TajirStoreItemListData( notnull TajirStoreCatalogItem item, EntityAI object = NULL )
	{
		m_item 		= item;
		m_object    = object;
	}

	/**
	 * @brief      Destroys the object.
	 */
	void ~TajirStoreItemListData()
	{
		if ( m_object != NULL && !m_object.HasNetworkID() )
		{
			m_object.Delete();
		}
	}

	/**
	 * @brief      Gets the item.
	 *
	 * @return     The item.
	 */
	TajirStoreCatalogItem GetItem()
	{
		return m_item;
	}

	/**
	 * @brief      Gets the object.
	 *
	 * @return     The object.
	 */
	EntityAI GetObject()
	{
		return m_object;
	}

	/**
	 * @brief      Sets the object.
	 *
	 * @param[in]  object  The object
	 */
	void SetObject( EntityAI object )
	{
		m_object = object;
	}
}