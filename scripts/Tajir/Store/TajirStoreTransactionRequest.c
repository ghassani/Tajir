/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

enum TajirStoreTransactionRequestType
{
	Unknown = 0,
	Buy,
	Sell
}

class TajirStoreTransactionRequest
{
	protected TajirStoreTransactionRequestType m_type;

	protected int m_merchantId;

	protected int m_storeId;

	protected int m_itemId;

	protected EntityAI m_item;

	protected int m_quantity;

	protected int m_time;

	/**
	 * @brief      { function_description }
	 */
	void TajirStoreTransactionRequest()
	{

	}

	/**
	 * @brief      Destroys the object.
	 */
	void ~TajirStoreTransactionRequest()
	{

	}
	
	/**
	 * @brief      Initialize a transaction request from TajirStoreCatalogItem.
	 *
	 * @param[in]  TajirStoreTransactionRequestType       The type
	 * @param[in]  TajirMerchant   The merchant
	 * @param[in]  TajirStore      The store
	 * @param[in]  TajirStoreCatalogItem  The item
	 * @param[in]  int   The quantity
	 */
	void Init( TajirStoreTransactionRequestType type, notnull TajirMerchant merchant, notnull TajirStore store, notnull TajirStoreCatalogItem item, int quantity )
	{
		m_type 			= type;
		m_merchantId 	= merchant.GetId();
		m_storeId 		= store.GetId();
		m_itemId 		= item.GetId();
		m_item 			= NULL;
		m_quantity    	= quantity;
		m_time 			= GetGame().GetTime();
	}

	/**
	 * @brief      Initialize a transaction request from a players inventory item.
	 *
	 * @param[in]  TajirStoreTransactionRequestType  The type
	 * @param[in]  TajirMerchant   The merchant
	 * @param[in]  TajirStore      The store
	 * @param[in]  EntityAI  The player inventory item
	 * @param[in]  int   The quantity
	 */
	void Init( TajirStoreTransactionRequestType type, notnull TajirMerchant merchant, notnull TajirStore store, notnull EntityAI item, int quantity  )
	{
		m_type 		 	= type;
		m_merchantId 	= merchant.GetId();
		m_storeId 		= store.GetId();
		m_itemId 		= 0;
		m_item 			= item;
		m_quantity    	= quantity;
		m_time 			= GetGame().GetTime();
	}

	/**
	 * @brief      Gets the type.
	 *
	 * @return     The type.
	 */
	TajirStoreTransactionRequestType GetType()
	{
		return m_type;
	}

	/**
	 * @brief      Sets the type.
	 *
	 * @param[in]  type  The type
	 */
	void SetType( TajirStoreTransactionRequestType type )
	{
		m_type = type;
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
	 * @brief      Sets the store identifier.
	 *
	 * @param[in]  id    The identifier
	 */
	void SetStoreId( int id )
	{
		m_storeId = id;
	}

	/**
	 * @brief      Gets the merchant identifier.
	 *
	 * @return     The merchant identifier.
	 */
	int GetMerchantId()
	{
		return m_merchantId;
	}

	/**
	 * @brief      Sets the merchant identifier.
	 *
	 * @param[in]  id    The identifier
	 */
	void SetMerchantId( int id )
	{
		m_merchantId = id;
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
	 * @brief      Sets the item identifier.
	 *
	 * @param[in]  id    The identifier
	 */
	void SetItemId( int id )
	{
		m_itemId = id;
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

	/**
	 * @brief      Sets the quantity.
	 *
	 * @param[in]  quantity  The quantity
	 */
	void SetQuantity( int quantity )
	{
		m_quantity = quantity;
	}

	/**
	 * @brief      Gets the time.
	 *
	 * @return     The time.
	 */
	int GetTime()
	{
		return m_time;
	}

	/**
	 * @brief      Gets the player item.
	 *
	 * @return     The player item.
	 */
	EntityAI GetPlayerItem()
	{
		return m_item;
	}
}