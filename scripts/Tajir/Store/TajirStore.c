/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStore
{
	protected TajirStoreConfig 					m_config;

	protected ref array<ref TajirStoreCatalog> 	m_catalogs;
	
	protected int 								m_lastRestockTime = 0;

	protected int 								m_lastSaveTime 	  = 0;

	/**
	 * @brief      Constructor
	 *
	 * @param[in]  TajirStoreConfig  The tajir store configuration
	 */
	void TajirStore( notnull TajirStoreConfig config )
	{
		m_config 			= config;
		m_catalogs 			= new array<ref TajirStoreCatalog>;
		m_lastRestockTime	= GetGame().GetTime();
		m_lastSaveTime		= GetGame().GetTime();

		for ( int i = 0; i < m_config.catalogs.Count(); i++ )
		{
#ifdef TAJIR_DEBUG
			TajirLogD( string.Format( "Adding Catalog %1 to Store ID %2",  m_config.catalogs[ i ].name,  m_config.id ), ClassName() );
#endif

			m_catalogs.Insert( new TajirStoreCatalog( this, m_config.catalogs[ i ] ) );
		}
	}

	/**
	 * @brief      Gets the configuration.
	 *
	 * @return     The configuration.
	 */
	TajirStoreConfig GetConfig()
	{
		return m_config;
	}

	/**
	 * @brief      Get the store id.
	 *
	 * @return     int
	 */
	int GetId()
	{
		return m_config.id;
	}

	/**
	 * @brief      Gets the name of the store.
	 *
	 * @return     The name.
	 */
	string GetName()
	{
		return m_config.name;
	}

	/**
	 * @brief      Get all catalogs.
	 *
	 * @return     array<ref TajirStoreCatalog
	 */
	array<ref TajirStoreCatalog> GetCatalogs()
	{
		return m_catalogs;
	}

	/**
	 * @brief      Gets the catalog count.
	 *
	 * @return     The catalog count.
	 */
	int GetCatalogCount()
	{
		return m_catalogs.Count();
	}

	/**
	 * @brief      { function_description }
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool ShouldLogTransactions()
	{
		return m_config.log_transactions;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool ShouldShowOutOfStockItems()
	{
		return m_config.show_out_of_stock;
	}

	/**
	 * @brief      Gets the last restock time.
	 *
	 * @return     The last restock time.
	 */
	int GetLastRestockTime()
	{
		return m_lastRestockTime;
	}

	/**
	 * @brief      Sets the last restock time.
	 *
	 * @param[in]  time  The time
	 */
	void SetLastRestockTime( int time )
	{
		m_lastRestockTime = time;
	}

	/**
	 * @brief      Get all visible catalogs for player display.
	 *
	 * @return     array<ref TajirStoreCatalog
	 */
	array<TajirStoreCatalog> GetVisibleCatalogs()
	{
		array<TajirStoreCatalog> ret = new array<TajirStoreCatalog>;

		foreach ( ref TajirStoreCatalog catalog : m_catalogs )
		{
			if ( !catalog.IsHidden() )
			{
				ret.Insert( catalog );
			}
		}

		return ret;
	}

	/**
	 * @brief      Get all hidden catalogs.
	 *
	 * @return     array<ref TajirStoreCatalog
	 */
	array<TajirStoreCatalog> GetHiddenCatalogs()
	{
		array<TajirStoreCatalog> ret = new array<TajirStoreCatalog>;

		foreach ( ref TajirStoreCatalog catalog : m_catalogs )
		{
			if ( catalog.IsHidden() )
			{
				ret.Insert( catalog );
			}
		}

		return ret;
	}

	/**
	 * @brief      Get a catalog by its index in the catalogs array.
	 *
	 * @param[in]  index  The index
	 *
	 * @return     TajirStoreCatalog|NULL
	 */
	TajirStoreCatalog GetCatalog( int index )
	{
		for ( int i = 0; i < m_catalogs.Count(); i++ )
		{
			if ( index == i )
			{
				return  m_catalogs[ i ];
			}
		}

		return NULL;
	}

	/**
	 * @brief      Get the store type handler
	 *
	 * @return     TajirStoreTypeBase|NULL
	 */
	TajirStoreTypeBase GetTypeHandler()
	{
		return TajirStoreComponent.GetInstance().GetStoreTypeHandler( m_config.type );
	}

	/**
	 * @brief      Determines if valid store type assigned is valid.
	 *
	 * @return     bool
	 */
	bool IsValidStoreType()
	{
		return TajirStoreComponent.GetInstance().HasStoreTypeHandler( m_config.type );
	}

	/**
	 * @brief      Check if any catalog has an item record matching the object name. 
	 * 			   Searches all catalogs, returning true when finding the first match.
	 *
	 * @param[in]  objectName  The object name
	 *
	 * @return     bool
	 */
	bool HasItem( string objectName )
	{
		foreach ( ref TajirStoreCatalog catalog : m_catalogs )
		{
			if ( catalog.HasItem( objectName ) )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Check if any catalog has an item record matching the id. 
	 * 			   Searches all catalogs, returning true when finding the first match.
	 *
	 * @param[in]  objectName  The object name
	 *
	 * @return     bool
	 */
	bool HasItem( int id )
	{
		foreach ( ref TajirStoreCatalog catalog : m_catalogs )
		{
			if ( catalog.HasItem( id ) )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Get the catalog that has the item record for the matching object name. 
	 * 			   Searches all catalogs, returning the first match.
	 *
	 * @param[in]  objectName  The object name
	 *
	 * @return     TajirStoreCatalog|NULL
	 */
	TajirStoreCatalog GetCatalogByItem( string objectName )
	{
		foreach ( ref TajirStoreCatalog catalog : m_catalogs )
		{
			if ( catalog.HasItem( objectName ) )
			{
				return catalog;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Get the catalog that has the item record for the matching id. 
	 * 			   Searches all catalogs, returning the first match.
	 *
	 * @param[in]  objectName  The object name
	 *
	 * @return     TajirStoreCatalog|NULL
	 */
	TajirStoreCatalog GetCatalogByItem( int id )
	{
		foreach ( ref TajirStoreCatalog catalog : m_catalogs )
		{
			if ( catalog.HasItem( id ) )
			{
				return catalog;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Get the item for the given object name. 
	 * 			   Searches all catalogs, returning the first match.
	 *
	 * @param[in]  objectName  The object name
	 *
	 * @return     TajirStoreCatalogItem|NULL
	 */
	TajirStoreCatalogItem GetItem( string objectName )
	{
		TajirStoreCatalogItem item;

		foreach ( ref TajirStoreCatalog catalog : m_catalogs )
		{
			item = catalog.GetItem( objectName );

			if ( item )
			{
				return item;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Get the item for the given item id. 
	 * 			   Searches all catalogs, returning the first match.
	 *
	 * @param[in]  objectName  The object name
	 *
	 * @return     TajirStoreCatalogItem|NULL
	 */
	TajirStoreCatalogItem GetItem( int id )
	{
		TajirStoreCatalogItem item;

		foreach ( ref TajirStoreCatalog catalog : m_catalogs )
		{
			item = catalog.GetItem( id );

			if ( item )
			{
				return item;
			}
		}

		return NULL;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  itemCondition  The item condition
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool AcceptsItemCondition( int itemCondition )
	{
		if ( m_config.allowed_conditions.Count() )
		{
			if ( m_config.allowed_conditions.Find( itemCondition ) != -1 )
			{
				return true;
			}
		}

		return false;
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirStoreInventoryUpdates  The tajir store inventory updates
	 */
	void ProcessRestocking( out notnull ref TajirStoreInventoryUpdates updates )
	{
		if ( !GetGame().IsServer() )
		{
			return;
		}
		else if ( m_config.restock_every_minutes <= 0 )
		{
			return;
		}

		if ( ( m_lastRestockTime + ( ( m_config.restock_every_minutes * 60 ) * 1000 ) ) < GetGame().GetTime() )
		{	

			foreach ( ref TajirStoreCatalog catalog : m_catalogs )
			{
				ref array<ref TajirStoreCatalogItem> items = catalog.GetItems();

				foreach ( TajirStoreCatalogItem item : items )
				{
					if ( item.GetRestockQuantity() <= 0 )
					{
						continue;
					}
					else if ( item.GetRestockQuantity() <= item.GetQuantity() )
					{
						continue;
					}

					item.SetQuantity( item.GetRestockQuantity() );

					updates.AddItem( item );
				}
			}

			m_lastRestockTime = GetGame().GetTime();

			updates.SetStoreLastRestockTime( this );

#ifdef TAJIR_DEBUG
			TajirLogD( string.Format( "Processed Inventory Restock For Store ID %1", GetId() ), ClassName() );
#endif
		}
	}

	/**
	 * @brief      Saves current inventory levels.
	 *
	 * @param[in]  force  The force
	 */
	void SaveCurrentInventoryLevels( bool force = false )
	{
		if ( !GetGame().IsServer() )
		{
			return;
		}
		else if ( m_config.save_every_minutes <= 0 )
		{
			return;
		}

		if ( force || ( m_lastSaveTime + ( ( m_config.save_every_minutes * 60 ) * 1000 ) ) < GetGame().GetTime() )
		{			
			string result;

			string outfile = string.Format( TAJIR_STORE_INVENTORY_SAVE_FILE_FORMAT, GetId() );
			ref array<ref TajirStoreCatalogItemPersist> save = new array<ref TajirStoreCatalogItemPersist>;

			foreach ( ref TajirStoreCatalog catalog : m_catalogs )
			{
				array<ref TajirStoreCatalogItem> items = catalog.GetItems();

				foreach ( ref TajirStoreCatalogItem item : items )
				{
					save.Insert( new TajirStoreCatalogItemPersist( item ) );
				}
			}

			result = JsonFileLoader<ref array<ref TajirStoreCatalogItemPersist>>.JsonMakeData( save );

			if ( !result.Length() )
			{
				TajirLogE( string.Format( "Store ID %1 Resulting serialization was empty", GetId() ), ClassName() );
				return;
			}

			if ( FileExist( outfile ) )
			{
				if ( !DeleteFile( outfile ) )
				{
					TajirLogE( string.Format( "Store ID %1 Unable to remove existing data file at %2",  GetId(), outfile ), ClassName() );
					return;
				}
			}

			FileHandle fh = OpenFile( outfile, FileMode.APPEND );

			if ( fh <= 0 )
			{
				TajirLogE( string.Format( "Store ID %1 Unable to open %2 for writing", GetId(), outfile ), ClassName() );
				return;
			}
						
			// seems we can only write so much at a time due to some security restriction
			// write up to 4096 bytes at a time
			int len = result.Length();
			int cur = 0;
			int per = 4096;

			while ( cur < len )
			{
				if ( ( cur + per ) > len )
				{
					per = len - cur;
				}

				string sub = result.Substring( cur, per );

				sub.Replace(" ", "");
				sub.Replace("\n", "");
				sub.Replace("\t", "");

				if ( sub && sub.Length() )
				{
					FPrint( fh, sub );
				}

				cur += per;
			}
			
			CloseFile( fh );

			m_lastSaveTime =  GetGame().GetTime();
		}
	}

	/**
	 * @brief      { function_description }
	 */
	void RestoreSavedInventoryLevels()
	{
		TajirStore store;
		TajirStoreCatalogItem item;

		string infile = string.Format( TAJIR_STORE_INVENTORY_SAVE_FILE_FORMAT, GetId() );

		ref array<ref TajirStoreCatalogItemPersist> saved = new array<ref TajirStoreCatalogItemPersist>;

		if ( !FileExist( infile ) )
		{
			TajirLogI( string.Format( "Store ID %1 No Saved Store Inventory To Load At %2", GetId(), infile ), ClassName() );
			return;
		}

		JsonFileLoader<ref array<ref TajirStoreCatalogItemPersist>>.JsonLoadFile( infile, saved );

#ifdef TAJIR_DEBUG
		TajirLogD( string.Format( "Loaded %1 Inventory Items For All Stores", saved.Count() ), ClassName() );
#endif

		foreach ( TajirStoreCatalogItemPersist restore : saved )
		{
			item = GetItem( restore.GetItemId() );

			if ( item )
			{
#ifdef TAJIR_DEBUG
				TajirLogD( string.Format( "Store %1 Item %2 Current Quantity %3 Updating To: %4", GetId(), item.GetItemId(), item.GetQuantity(), restore.GetQuantity() ), ClassName() );
#endif
				
				item.SetQuantity( restore.GetQuantity() );
			}
		}
	}
}