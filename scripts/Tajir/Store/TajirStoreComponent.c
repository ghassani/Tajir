/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      Handles displaying stores and handles transactions.
 */
class TajirStoreComponent extends TajirComponentBase
{
	protected ref array<ref TajirStoreConfig> 		m_config;

	protected ref array<ref TajirStore> 			m_stores;

	protected ref TajirStoreTransactionManager 		m_transactions

	protected ref array<ref TajirStoreTypeBase> 	m_handlers;

	protected ref TajirStoreMenuBase				m_menu;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirManager  The tajir manager
	 */
	void TajirStoreComponent( notnull TajirManager manager )
	{
		m_name 	 				= "TajirStores";
		m_config 				= new array<ref TajirStoreConfig>;
		m_stores 				= new array<ref TajirStore>;
		m_handlers 				= new array<ref TajirStoreTypeBase>;
		m_transactions			= new TajirStoreTransactionManager( this );
		m_menu 					= NULL;
	}
	
	/**
	 * @brief      Gets the instance.
	 *
	 * @return     The instance.
	 */
	static TajirStoreComponent GetInstance()
	{
		if ( TajirManager.GetInstance() )
		{
			return TajirStoreComponent.Cast( TajirManager.GetInstance().GetComponent( "TajirStores" ) );			
		}

		return NULL;	
	}

	/**
	 * @brief      Loads the component server side configuration.
	 *
	 * @return     void
	 */
	override void LoadServerConfiguration()
	{	
		// Load Store Config
		if ( !FileExist( TAJIR_STORE_CONFIG_USER ) )
		{
			if ( !CopyFile( TAJIR_STORE_CONFIG_DEFAULT, TAJIR_STORE_CONFIG_USER ) )
			{				
				TajirLogE( string.Format( "Error Copying Default Store Configuration from %1 to %2", TAJIR_STORE_CONFIG_DEFAULT, TAJIR_STORE_CONFIG_USER ), ClassName() );
			}
		}

		JsonFileLoader<ref array<ref TajirStoreConfig>>.JsonLoadFile( TAJIR_STORE_CONFIG_USER, m_config );

		for ( int i = 0; i < m_config.Count(); i++ )
		{	
			m_stores.Insert( new TajirStore( m_config[ i ] ) );
		}

		RestoreSavedInventoryLevels();

		SetConfigured( true );
	}

	/**
	 * @brief      Loads the component client side configuration.
	 *
	 * @return     void
	 */
	override void LoadClientConfiguration()
	{
		SetConfigured( false ); // will be sent by server
	}

	/**
	 * @brief      Gets the transaction manager.
	 *
	 * @return     The transaction manager.
	 */
	TajirStoreTransactionManager GetTransactionManager()
	{
		return m_transactions;
	}

	/**
	 * @brief      Gets the configuration.
	 *
	 * @return     array<ref TajirStoreConfig>
	 */
	array<ref TajirStoreConfig> GetConfig()
	{
		return m_config;
	}

	/**
	 * @brief      Gets the stores.
	 *
	 * @return     array<ref TajirStore>
	 */
	array<ref TajirStore> GetStores()
	{
		return m_stores;
	}

	/**
	 * @brief      Gets a store by its id.
	 *
	 * @param[in]  id 
	 *
	 * @return     TajirStore|NULL
	 */
	TajirStore GetStore( int id )
	{
		foreach ( ref TajirStore store : m_stores )
		{
			if ( store.GetId() == id )
			{
				return store;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Determines if it has a store by id.
	 *
	 * @param[in]  id
	 *
	 * @return     bool
	 */
	bool HasStore( int id )
	{
		foreach ( ref TajirStore store : m_stores )
		{
			if ( store.GetId() == id )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Add a store.
	 *
	 * @param[in]  TajirStore  The tajir store
	 *
	 * @return     int Total stores after insertion.
	 */
	int AddStore( notnull ref TajirStore store )
	{
		m_stores.Insert( store );

		return m_stores.Count();
	}

	/**
	 * @brief      Register a store type handler.
	 *
	 * @param[in]  TajirStoreTypeBase  The tajir store type base
	 *
	 * @return     bool
	 */
	bool AddStoreTypeHandler( notnull ref TajirStoreTypeBase handler )
	{
		foreach ( TajirStoreTypeBase existing : m_handlers )
		{
			if ( existing.GetName() == handler.GetName() )
			{
				TajirLogE( string.Format( "Store Type Handler %1 is Already Regsiterd", handler.GetName() ), ClassName() );

				return false;
			}
		}

		m_handlers.Insert( handler );

		return true;
	}

	/**
	 * @brief      Gets the store type handler.
	 *
	 * @param[in]  name  The name
	 *
	 * @return     The store type handler.
	 */
	TajirStoreTypeBase GetStoreTypeHandler( string name )
	{
		foreach ( TajirStoreTypeBase handler : m_handlers )
		{
			if ( handler.GetName() == name )
			{
				return handler;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Determines if it has store type handler.
	 *
	 * @param[in]  name  The name
	 *
	 * @return     True if has store type handler, False otherwise.
	 */
	bool HasStoreTypeHandler( string name )
	{
		foreach ( TajirStoreTypeBase handler : m_handlers )
		{
			if ( handler.GetName() == name )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  player    The player
	 * @param[in]  identity  The identity
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void OnClientConnected( PlayerBase player, PlayerIdentity identity )
	{
		SendClientConfiguration( identity, player );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  sender    The sender
	 * @param[in]  target    The target
	 * @param[in]  rpc_type  The rpc type
	 * @param[in]  ctx       The context
	 */
	void OnClientRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
			case TajirStoreComponentRPC.ServerRPC_StoreConfiguration:
				ReadConfigurationFromServer( ctx );
				break;
			case TajirStoreComponentRPC.ServerRPC_TransactionResponse:
				ReadTransactionResponseFromServer( ctx );
				break;
			case TajirStoreComponentRPC.ServerRPC_InventoryUpdate:
				ReadInventoryUpdatesFromServer( ctx );
				break;
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  sender    The sender
	 * @param[in]  target    The target
	 * @param[in]  rpc_type  The rpc type
	 * @param[in]  ctx       The context
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void OnServerRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
			case TajirStoreComponentRPC.ClientRPC_TransactionRequest:
				ReadTransactionRequestFromClient( sender, target, ctx );
				break;
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  timeslice  The timeslice
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void OnSystemUpdate( float timeslice )
	{
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			m_transactions.ProcessQueue();

			ProcessRestocking();

			SaveCurrentInventoryLevels();
		}
	}

	/**
	 * @brief      Shows the store menu.
	 *
	 * @param[in]  store     The store
	 * @param[in]  merchant  The merchant
	 */
	void ShowStoreMenu( TajirStore store, TajirMerchant merchant = NULL )
	{
		if ( !GetGame().GetUIManager().GetMenu() )
		{
			if ( store.GetTypeHandler() )
			{
				m_menu = store.GetTypeHandler().CreateMenu( this, store, merchant );

				if ( m_menu )
				{
					GetGame().GetUIManager().ShowScriptedMenu( m_menu, NULL );
				}
			}
		}
	}

	/**
	 * @brief      { function_description }
	 */
	void OnMenuClose()
	{
		m_menu = NULL;
	}

	/**
	 * @brief      Reads a configuration from server.
	 *
	 * @param[in]  ctx   The context
	 */
	void ReadConfigurationFromServer( ParamsReadContext ctx )
	{
		if ( !GetGame().IsMultiplayer() && !GetGame().IsClient() )
		{
			TajirLogE( "Expected Call From Client", ClassName() );
			return;
		}
		else if ( IsConfigured() )
		{
			TajirLogE( "Already Configured", ClassName() );
			return;
		}

		m_config.Clear();

		ref Param1<ref array<ref TajirStoreConfig>> params = new Param1<ref array<ref TajirStoreConfig>>( m_config );

		if ( !ctx.Read( params ) ) 
		{
			SetConfigured( false );
			TajirLogE( "Error Reading Configuration From Server", ClassName() );
			return;
		}
		
		for ( int i = 0; i < m_config.Count(); i++ )
		{
			AddStore( new TajirStore( m_config[ i ] ) );
		}
		
		SetConfigured( true );
		ApplyConfiguration();
	}

	/**
	 * @brief      Reads transaction response from server.
	 * 
	 * Context: Client
	 *
	 * @param[in]  ctx   The context
	 */
	void ReadTransactionResponseFromServer( ParamsReadContext ctx )
	{
		TajirStoreTransactionRequest request;
		
		if ( m_menu )
		{
			request = m_menu.GetTransaction();
		}

		ref TajirStoreTransactionResponse response 	= new TajirStoreTransactionResponse( NULL, request );
		
		ref Param1<ref TajirStoreTransactionResponse> params = new Param1<ref TajirStoreTransactionResponse>( response );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( "Error Reading Transaction Response From Server", ClassName() );

			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Unexpected Response From Server", 2000 );
			
			if ( m_menu )
			{
				m_menu.OnTransactionResponseReceived( NULL );
			}

			return;
		}

#ifdef TAJIR_DEBUG
		TajirLogD( string.Format( "Transaction Response: ID: %1 Amount: %2 Code: %3 IsError: %4 ", response.GetId(), response.GetAmount(), response.GetError(), response.IsError() ), ClassName() );
		TajirLogD( string.Format( "Original Transaction Request: Type: %1 Store ID: %2 Item ID %3 Quantity: %4", response.GetRequest().GetType(), response.GetRequest().GetStoreId(), response.GetRequest().GetItemId(), response.GetRequest().GetQuantity() ), ClassName() );
#endif

		if ( response.IsError() )
		{
			if ( response.GetId() )
			{
				TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, string.Format( "Transaction ID %1 Failed: %2", response.GetId(), response.GetErrorString() ), 2000 );
			}
			else
			{
				TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, string.Format( "Transaction Failed: %1", response.GetErrorString() ), 2000 );
			}	
		}
		else
		{
			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Success, string.Format( "Transaction ID %1 Complete", response.GetId() ), 2000 );

			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

			player.AddCurrency( response.GetAmount() );
		}

		if ( m_menu )
		{
			m_menu.OnTransactionResponseReceived( response );
		}
	}

	/**
	 * @brief      Reads inventory updates from server.
	 * 
	 * Contect: Client
	 *
	 * @param[in]  ctx   The context
	 */
	void ReadInventoryUpdatesFromServer( ParamsReadContext ctx )
	{
		ref TajirStoreInventoryUpdates updates = new TajirStoreInventoryUpdates();

		ref Param1<ref TajirStoreInventoryUpdates> params = new Param1<ref TajirStoreInventoryUpdates>( updates );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( "Error Reading Inventory Updates From Server", ClassName() );
			return;
		}

		TajirStore store;
		TajirStoreCatalogItem item;
		ref array<TajirStore> updatedStores = new array<TajirStore>;

		for ( int i = 0; i < updates.items.Count(); i++ )
		{
			store = GetStore( updates.items[ i ].storeId );

			if ( !store )
			{
				TajirLogE( string.Format( "Store ID %1 Not Found", updates.items[ i ].storeId ), ClassName() );
				continue;
			}

			item = store.GetItem( updates.items[ i ].itemId );

			if ( !item )
			{
				TajirLogE( string.Format( "Item ID %1 Not Found in Store ID %1", updates.items[ i ].itemId, updates.items[ i ].storeId ), ClassName() );
				continue;
			}

			item.SetQuantity( updates.items[ i ].quantity );

			if ( !updatedStores.Find( store ) )
			{
				updatedStores.Insert( store );
			}
#ifdef TAJIR_DEBUG
			TajirLogD( string.Format( "Updated Item ID %1 in Store ID %2 to Quantity %3", updates.items[ i ].itemId, updates.items[ i ].storeId, updates.items[ i ].quantity ), ClassName() );
#endif
		}

		if ( m_menu )
		{
			m_menu.OnInventoryUpdateReceived( updatedStores );
		}
	}

	/**
	 * @brief      Sends a client configuration.
	 * 
	 * Context: Server
	 *
	 * @param[in]  identity  The identity
	 * @param[in]  player    The player
	 */
	void SendClientConfiguration( PlayerIdentity identity, PlayerBase player )
	{
		GetGame().RPCSingleParam( player, TajirStoreComponentRPC.ServerRPC_StoreConfiguration, new Param1<ref array<ref TajirStoreConfig>>( m_config ), true, identity );
	}

	/**
	 * @brief      Reads a transaction request from client.
	 * 
	 * Context: Server
	 *
	 * @param[in]  sender  The sender
	 * @param[in]  target  The target
	 * @param[in]  ctx     The context
	 */
	void ReadTransactionRequestFromClient( PlayerIdentity sender, Object target, ParamsReadContext ctx )
	{
		PlayerBase player = PlayerBase.Cast( target );

		if ( !player )
		{
			TajirLogE( "Invalid Player", ClassName() );
			return;
		}

		ref TajirStoreTransactionRequest request = new TajirStoreTransactionRequest();

		ref Param1<ref TajirStoreTransactionRequest> params = new Param1<ref TajirStoreTransactionRequest>( request );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading Transaction Request From Player ID %1", sender.GetId() ), ClassName() );
			SendTransactionErrorResponse( player, NULL, TajirStoreTransactionError.RequestError );
			return;
		}

#ifdef TAJIR_DEBUG
		TajirLogD( string.Format( "New Transaction Request From %1 - Type: %2 Store ID: %3 Item ID: %4 Quantity: %5 Time: %6",  sender.GetId(), request.GetType(), request.GetStoreId(), request.GetItemId(), request.GetQuantity(), request.GetTime() ), ClassName() );
#endif

		if ( request.GetMerchantId() <= 0 )
		{
			TajirLogE( string.Format( "Invalid Merchant ID From Player ID %1", sender.GetId() ), ClassName() );
			SendTransactionErrorResponse( player, request, TajirStoreTransactionError.InvalidMerchant );
			return;
		}
		else if ( request.GetStoreId() <= 0 )
		{
			TajirLogE( string.Format( "Invalid Store ID From Player ID %1", sender.GetId() ), ClassName() );
			SendTransactionErrorResponse( player, request, TajirStoreTransactionError.InvalidStore );
			return;
		} 
		else if ( request.GetItemId() <= 0 && request.GetType() != TajirStoreTransactionRequestType.Sell )
		{
			TajirLogE( string.Format( "Invalid Item ID From Player ID %1", sender.GetId() ), ClassName() );
			SendTransactionErrorResponse( player, request, TajirStoreTransactionError.InvalidItem );
			return;
		}
		else if ( request.GetQuantity() <= 0 )
		{
			TajirLogE( string.Format( "Invalid Quantity From Player ID %1", sender.GetId() ), ClassName() );
			SendTransactionErrorResponse( player, request, TajirStoreTransactionError.InvalidQuantity );
			return;
		}
		
#ifdef TAJIR_SIMULATE_SERVER_TRANSACTION_TIMEOUT
		TajirLogD( "Timeout Simulation Start", ClassName() );

		int start = GetGame().GetTime();
		int now = start;
		int end = start + ( TAJIR_SIMULATE_SERVER_TRANSACTION_TIMEOUT_SECONDS * 1000 );

		while ( now < end )
		{
			now = GetGame().GetTime();
		}

		TajirLogD( "Timeout Simulation Complete", ClassName() );
#endif

		GetTransactionManager().QueueRequestForProcessing( player, request );
	}

	/**
	 * @brief      Sends a transaction error response.
	 *
	 * @param[in]  target   The target
	 * @param[in]  request  The request
	 * @param[in]  error    The error
	 */
	protected void SendTransactionErrorResponse( PlayerBase target, TajirStoreTransactionRequest request, TajirStoreTransactionError error )
	{
		ref TajirStoreTransactionResponse response = new TajirStoreTransactionResponse( NULL, request );

		response.SetError( error );

		GetGame().RPCSingleParam( target, TajirStoreComponentRPC.ServerRPC_TransactionResponse, new Param1<ref TajirStoreTransactionResponse>( response ), true, target.GetIdentity() );
	}

	/**
	 * @brief      Notifies a players inventory update. Shortcut to NotifyPlayersInventoryUpdates
	 * 
	 * Contect: Server
	 *
	 * @param[in]  TajirStoreCatalogItem  The tajir store catalog item
	 */
	void NotifyPlayersInventoryUpdate( notnull TajirStoreCatalogItem item )
	{
		ref TajirStoreInventoryUpdates updates = new TajirStoreInventoryUpdates();

		updates.AddItem( item );

		NotifyPlayersInventoryUpdates( updates );
	}

	/**
	 * @brief      Notifies players inventory updates.
	 *
	 * @param[in]  TajirStoreInventoryUpdates  The tajir store inventory updates
	 */
	void NotifyPlayersInventoryUpdates( notnull TajirStoreInventoryUpdates updates )
	{
		ref array<Man> players = new array<Man>;

		GetGame().GetPlayers( players );
		
		foreach ( Man player : players )
		{
#ifdef TAJIR_DEBUG
			TajirLogD( string.Format( "Syncing Inventory Update With Player ID %1", player.GetIdentity().GetId() ), ClassName() );
#endif
			player.RPCSingleParam( TajirStoreComponentRPC.ServerRPC_InventoryUpdate, new Param1<ref TajirStoreInventoryUpdates>( updates ), true, player.GetIdentity() );
		}
	}

	/**
	 * @brief      { function_description }
	 */
	void ProcessRestocking()
	{
		ref TajirStoreInventoryUpdates updates = new TajirStoreInventoryUpdates();

		foreach ( ref TajirStore store : m_stores )
		{
			store.ProcessRestocking( updates );
		}

		if ( updates.HasUpdates() )
		{
			NotifyPlayersInventoryUpdates( updates );
		}
	}

	/**
	 * @brief      { function_description }
	 */
	void SaveCurrentInventoryLevels( bool force = false )
	{		
		foreach ( ref TajirStore store : m_stores )
		{
			store.SaveCurrentInventoryLevels( force );
		}
	}

	/**
	 * @brief      { function_description }
	 */
	void RestoreSavedInventoryLevels()
	{
	
		foreach ( ref TajirStore store : m_stores )
		{
			store.RestoreSavedInventoryLevels();
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void OnServerMissionFinish()
	{
		SaveCurrentInventoryLevels( true );
	}
}