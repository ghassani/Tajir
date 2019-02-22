/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      Handles the core item list configuration. Includes item pricing and bundle items.
 */
class TajirItemComponent extends TajirComponentBase
{
	protected ref array<ref TajirItemConfig> 	m_config;

	protected ref array<ref TajirItem> 			m_items;
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirManager  The tajir manager
	 */
	void TajirItemComponent( notnull TajirManager manager )
	{
		m_name 	 = "TajirItems";
		m_config = new array<ref TajirItemConfig>;
		m_items  = new array<ref TajirItem>;
	}

	/**
	 * @brief      Gets the instance.
	 *
	 * @return     The instance.
	 */
	static TajirItemComponent GetInstance()
	{
		if ( TajirManager.GetInstance() )
		{
			return TajirItemComponent.Cast( TajirManager.GetInstance().GetComponent( "TajirItems" ) );			
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
		// Load Item Base Config
		if ( !FileExist( TAJIR_ITEM_CONFIG_USER ) )
		{
			if ( !CopyFile( TAJIR_ITEM_CONFIG_DEFAULT, TAJIR_ITEM_CONFIG_USER ) )
			{				
				TajirLogE( string.Format( "Error Copying Default Store Item Configuration from %1 to %2", TAJIR_ITEM_CONFIG_DEFAULT, TAJIR_ITEM_CONFIG_USER ), ClassName() );
			}
		}

		JsonFileLoader<ref array<ref TajirItemConfig>>.JsonLoadFile( TAJIR_ITEM_CONFIG_USER, m_config );

		for ( int i = 0; i < m_config.Count(); i++ )
		{	
			if ( m_config[ i ].id <= 0 )
			{
				TajirLogE( string.Format( "Item Config %1 - Invalid ID %2", i, m_config[ i ].id ), ClassName() );
				continue;
			}
			else if ( HasItem( m_config[ i ].id ) )
			{
				TajirLogE( string.Format( "Item Config %1 - Item ID %2 Already Exists", i, m_config[ i ].id ), ClassName() );
				continue;
			}

			m_items.Insert( new TajirItem( m_config[ i ] ) );
		}

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
	 * @brief      Gets the item list.
	 *
	 * @return     array<ref TajirItem>
	 */
	array<ref TajirItem> GetItems()
	{
		return m_items;
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
			case TajirItemComponentRPC.ServerRPC_ItemConfiguration:
				ReadConfigurationFromServer( ctx );
				break;
		}
	}

	/**
	 * @brief      Reads a configuration from server.
	 *
	 * @param[in]  ctx   The context
	 */
	void ReadConfigurationFromServer( ParamsReadContext ctx )
	{
		if ( !m_manager.IsClient() )
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
		m_items.Clear();

		ref Param1<ref array<ref TajirItemConfig>> params = new Param1<ref array<ref TajirItemConfig>>( m_config );

		if ( !ctx.Read( params ) )
		{
			SetConfigured( false );

			TajirLogE( "Error Reading Configuration From Server", ClassName() );

			return;
		}
		
		for ( int i = 0; i < m_config.Count(); i++ )
		{	
			m_items.Insert( new TajirItem( m_config[ i ] ) );
		}

		SetConfigured( true );
		ApplyConfiguration();
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
		GetGame().RPCSingleParam( player, TajirItemComponentRPC.ServerRPC_ItemConfiguration, new Param1<ref array<ref TajirItemConfig>>(  m_config ), true, identity );
	}

	TajirItem GetItem( int id )
	{
		foreach ( ref TajirItem item : m_items )
		{
			if ( item.GetId() == id )
			{
				return item;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Gets the item.
	 *
	 * @param[in]  itemType  The item type
	 *
	 * @return     The item.
	 */
	TajirItem GetItem( string itemType )
	{
		foreach ( ref TajirItem item : m_items )
		{
			if ( item.GetObjectName() == itemType )
			{
				return item;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Determines if it has item.
	 *
	 * @param[in]  id    The identifier
	 *
	 * @return     True if has item, False otherwise.
	 */
	bool HasItem( int id )
	{
		foreach ( ref TajirItem item : m_items )
		{
			if ( item.GetId() == id )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Determines if it has item.
	 *
	 * @param[in]  itemType  The item type
	 *
	 * @return     True if has item, False otherwise.
	 */
	bool HasItem( string itemType )
	{
		foreach ( ref TajirItem item : m_items )
		{
			if ( item.GetObjectName() == itemType )
			{
				return true;
			}
		}

		return false;
	}
}