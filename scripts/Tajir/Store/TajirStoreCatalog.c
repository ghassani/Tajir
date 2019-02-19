/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreCatalog
{
	protected TajirStore 							m_store;
	protected TajirStoreCatalogConfig 				m_config;
	protected ref array<ref TajirStoreCatalogItem> 	m_items;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirStore               The tajir store
	 * @param[in]  TajirStoreCatalogConfig  The tajir store catalog configuration
	 */
	void TajirStoreCatalog( notnull TajirStore store, notnull TajirStoreCatalogConfig config )
	{
		m_store 	= store;
		m_config 	= config;
		m_items 	= new array<ref TajirStoreCatalogItem>;
		
		TajirItemComponent itemscomp = TajirItemComponent.GetInstance();

		for ( int i = 0; i < m_config.items.Count(); i++ )
		{
			TajirItem item = itemscomp.GetItem( m_config.items[ i ].item_id );

			if ( !item )
			{
				TajirLogE( string.Format( "Skipping Catalog Item Entry %1 - Item With ID %2 Not Found", i,  m_config.items[ i ].item_id ), ClassName() );
				continue;
			}
			else if ( m_store.HasItem( m_config.items[ i ].item_id ) )
			{
				TajirLogE( string.Format( "Skipping Catalog Item Entry %1 - Item With ID %2 Already Exists", i,  m_config.items[ i ].item_id ), ClassName() );
				continue;
			}

#ifdef TAJIR_DEBUG
			TajirLogD( string.Format( "Adding Catalog Item ID %1 to Catalog %2 in Store ID %3", m_config.items[ i ].item_id, m_config.name, m_store.GetId() ), ClassName() );
#endif

			m_items.Insert( new TajirStoreCatalogItem( this, item, m_config.items[ i ] ) );
		}
	}
	
	/**
	 * @brief      Gets the configuration.
	 *
	 * @return     The configuration.
	 */
	TajirStoreCatalogConfig GetConfig()
	{
		return m_config;
	}

	/**
	 * @brief      Gets the store.
	 *
	 * @return     The store.
	 */
	TajirStore GetStore()
	{
		return m_store;
	}
	
	/**
	 * @brief      Gets the name.
	 *
	 * @return     The name.
	 */
	string GetName()
	{
		return m_config.name;
	}

	/**
	 * @brief      Gets the items.
	 *
	 * @return     The items.
	 */
	array<ref TajirStoreCatalogItem> GetItems()
	{
		return  m_items;
	}

	/**
	 * @brief      Gets the item count.
	 *
	 * @return     The item count.
	 */
	int GetItemCount()
	{
		return m_items.Count();
	}

	/**
	 * @brief      If the catalog is configured to be hidden from the player.
	 *
	 * @return     True if should be hidden, False otherwise.
	 */
	bool IsHidden()
	{
		return m_config.hidden;
	}

	/**
	 * @brief      Determines if it has inventory.
	 *
	 * @param[in]  objectName  The object name
	 *
	 * @return     True if has inventory, False otherwise.
	 */
	bool HasItem( string objectName )
	{
		for ( int i = 0; i < m_items.Count(); i++ )
		{
			if ( m_items[ i ].GetObjectName() == objectName )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Determines if it has inventory.
	 *
	 * @param[in]  id    The identifier
	 *
	 * @return     True if has inventory, False otherwise.
	 */
	bool HasItem( int id )
	{
		for ( int i = 0; i < m_items.Count(); i++ )
		{
			if ( m_items[ i ].GetItemId() == id )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Determines if it has inventory.
	 *
	 * @param[in]  object  The object
	 *
	 * @return     True if has inventory, False otherwise.
	 */
	bool HasItem( Object object )
	{
		return HasItem( object.GetType() );
	}

	/**
	 * @brief      Gets the inventory.
	 *
	 * @param[in]  objectName  The object name
	 *
	 * @return     The inventory.
	 */
	TajirStoreCatalogItem GetItem( string objectName )
	{
		foreach ( ref TajirStoreCatalogItem item : m_items )
		{
			if ( item.GetObjectName() == objectName )
			{
				return item;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Gets the inventory item by its id.
	 *
	 * @param[in]  id    The id
	 *
	 * @return     The inventory.
	 */
	TajirStoreCatalogItem GetItem( int id )
	{
		foreach ( ref TajirStoreCatalogItem item : m_items )
		{
			if ( item.GetItemId() == id )
			{
				return item;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Gets the item.
	 *
	 * @param[in]  object  The object
	 *
	 * @return     The item.
	 */
	TajirStoreCatalogItem GetItem( Object object )
	{
		return GetItem( object.GetType() );
	}
}