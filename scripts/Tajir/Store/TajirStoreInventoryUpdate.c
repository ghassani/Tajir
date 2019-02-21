/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreInventoryUpdates
{
	ref array<ref TajirStoreInventoryUpdate> items;
	ref map<int, int> storeLastRestockTimes;

	/**
	 * @brief      { function_description }
	 */
	void TajirStoreInventoryUpdates()
	{
		items = new array<ref TajirStoreInventoryUpdate>;
		storeLastRestockTimes = new map<int, int>;
	}

	/**
	 * @brief      Destroys the object.
	 */
	void ~TajirStoreInventoryUpdates()
	{

	}

	/**
	 * @brief      Gets the items.
	 *
	 * @return     The items.
	 */
	array<ref TajirStoreInventoryUpdate> GetItems()
	{
		return items;
	}

	/**
	 * @brief      Gets the store restock times.
	 *
	 * @return     The store restock times.
	 */
	map<int, int> GetStoreRestockTimes()
	{
		return storeLastRestockTimes;
	}

	/**
	 * @brief      Determines if it has updates.
	 *
	 * @return     True if has updates, False otherwise.
	 */
	bool HasUpdates()
	{
		if ( items.Count() || storeLastRestockTimes.Count() )
		{
			return true;
		}

		return false;
	}

	/**
	 * @brief      Sets the store last restock time.
	 *
	 * @param[in]  TajirStore  The tajir store
	 */
	void SetStoreLastRestockTime( notnull TajirStore store )
	{
		SetStoreLastRestockTime( store.GetId(), store.GetLastRestockTime() );
	}

	/**
	 * @brief      Sets the store last restock time.
	 *
	 * @param[in]  storeId  The store identifier
	 * @param[in]  time     The time
	 */
	void SetStoreLastRestockTime( int storeId, int time )
	{
		storeLastRestockTimes.Set( storeId, time );
	}

	/**
	 * @brief      Gets the store last restock time.
	 *
	 * @param[in]  storeId  The store identifier
	 *
	 * @return     The store last restock time.
	 */
	int GetStoreLastRestockTime( int storeId )
	{
		return storeLastRestockTimes.Get( storeId );
	}

	/**
	 * @brief      Adds an item.
	 *
	 * @param[in]  TajirStoreCatalogItem  The tajir store catalog item
	 */
	void AddItem( notnull  TajirStoreCatalogItem item )
	{
		items.Insert( TajirStoreInventoryUpdate( item ) );
	}
}

/**
 * @brief      { class_description }
 */
class TajirStoreInventoryUpdate
{
	int storeId;

	int itemId;

	int quantity;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirStoreCatalogItem  The tajir store catalog item
	 */
	void TajirStoreInventoryUpdate( notnull TajirStoreCatalogItem item )
	{
		storeId  = item.GetCatalog().GetStore().GetId();
		itemId   = item.GetId();
		quantity = item.GetQuantity();
	}
}