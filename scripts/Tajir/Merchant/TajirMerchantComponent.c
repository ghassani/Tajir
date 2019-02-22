/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      Handles merchant objects placed in the world.
 */
class TajirMerchantComponent extends TajirComponentBase
{

	protected ref array<ref TajirMerchantConfig> m_config;

	protected ref array<ref TajirMerchant> m_merchants;

	/**
	 * @brief      Constructor
	 *
	 * @param[in]  TajirManager
	 */
	void TajirMerchantComponent( notnull TajirManager manager )
	{
		m_name 	 	= "TajirMerchants";
		m_config 	= new array<ref TajirMerchantConfig>;
		m_merchants = new array<ref TajirMerchant>;
	}

	/**
	 * @brief      Destructor
	 */
	void ~TajirMerchantComponent()
	{
		foreach ( TajirMerchant merchant : m_merchants )
		{
			if ( merchant )
			{
				delete merchant;
			}			
		}
	}

	/**
	 * @brief      Convinence static function to get the instance from TajirManager
	 */
	static TajirMerchantComponent GetInstance()
	{
		if ( TajirManager.GetInstance() )
		{
			return TajirMerchantComponent.Cast( TajirManager.GetInstance().GetComponent( "TajirMerchants" ) );
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
		if ( !FileExist( TAJIR_MERCHANT_CONFIG_USER ) )
		{
			if ( !CopyFile( TAJIR_MERCHANT_CONFIG_DEFAULT, TAJIR_MERCHANT_CONFIG_USER ) )
			{				
				TajirLogE( string.Format( "Error Copying Default Merchant Configuration from %1 to %2", TAJIR_MERCHANT_CONFIG_DEFAULT, TAJIR_MERCHANT_CONFIG_USER ), ClassName() );
			}
		}

		JsonFileLoader<ref array<ref TajirMerchantConfig>>.JsonLoadFile( TAJIR_MERCHANT_CONFIG_USER, m_config );

		TajirLogI( string.Format( "Loaded %1 Merchants From Config %2", m_config.Count(), TAJIR_MERCHANT_CONFIG_USER ), ClassName() );

		Object object;

		for ( int i = 0; i < m_config.Count(); i++ )
		{				
			if ( m_config[ i ].id <= 0 )
			{
				TajirLogE( string.Format( "Skipping Entry %1 Invalid ID", i ), ClassName() );
				continue;
			}
			else if ( m_config[ i ].name == "" )
			{
				TajirLogE( string.Format( "Skipping Entry %1 Merchant Name Missing", i ), ClassName() );
				continue;
			}
			else if ( m_config[ i ].object_name == "" )
			{
				TajirLogE( string.Format( "Skipping Entry %1 Object Name Missing", i ), ClassName() );
				continue;
			}

			object = GetGame().CreateObject( m_config[ i ].object_name, m_config[ i ].position, false, TajirSpawnHelper.EntityRequiresAI( m_config[ i ].object_name ), true );

			if ( object == NULL )
			{
				TajirLogE( string.Format( "Skipping Entry %1 Object Invalid", i ), ClassName() );
				continue;
			}

			if ( GetGame().IsMultiplayer() )
			{
				 object.GetNetworkID( m_config[ i ].network_id_low, m_config[ i ].network_id_high );
			}

#ifdef TAJIR_DEBUG
			TajirLogD( string.Format( "Adding Merchant ID %1 - %2 Object: %3", m_config[ i ].id, m_config[ i ].name, m_config[ i ].object_name ), ClassName() );
#endif
			AddMerchant( new TajirMerchant( m_config[ i ], object ) );
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
	 * @brief      Applys the component client side configuration.
	 *
	 * @return     void
	 */
	override void ApplyClientConfiguration()
	{
		if ( m_manager.GetKeybindManager().HasBinding( "toggle_merchant_shop" ) )
		{
			TajirLogE( "toggle_merchant_shop Already Bound", ClassName() );
			return;
		}
		
		TajirConfigKeyBinding config = m_manager.GetConfig().GetKeybindConfig( "toggle_merchant_shop" );

		if ( config == NULL )
		{
			TajirLogE( "Keybind Configuration toggle_merchant_shop Not Found", ClassName() );
			return;
		}

		ref TajirKeyBinding binding = new TajirKeyBinding( "toggle_merchant_shop", "Toggles the merchant shop when near a merchant", this, "OnMerchantShopKeyPress" );

		for ( int i = 0; i < config.keybinds.Count(); i++ )
		{
			binding.BindCombo( config.keybinds[ i ] );
		}

		m_manager.GetKeybindManager().AddBinding( binding );
	}

	/**
	 * @brief      Get all merchants.
	 *
	 * @return     array<ref TajirMerchant>
	 */
	array<ref TajirMerchant> GetMerchants()
	{
		return m_merchants;
	}

	/**
	 * @brief      Adds a merchant.
	 *
	 * @param[in]  TajirMerchant  The tajir merchant
	 *
	 * @return     int Total merchants after insertion
	 */
	int AddMerchant( notnull ref TajirMerchant merchant )
	{
		m_merchants.Insert( merchant );

		return m_merchants.Count();
	}

	/**
	 * @brief      Get a merchant by its ID.
	 *
	 * @param[in]  id
	 *
	 * @return     TajirMerchant
	 */
	TajirMerchant GetMerchant( int id )
	{
		foreach ( TajirMerchant merchant : m_merchants )
		{
			if ( merchant.GetId() == id )
			{
				return merchant;
			}
		}
		
		return NULL;
	}

	/**
	 * @brief      Check if a merchant exists for a given id.
	 *
	 * @param[in]  id    The identifier
	 *
	 * @return     True if has merchant, False otherwise.
	 */
	bool HasMerchant( int id )
	{
		foreach ( TajirMerchant merchant : m_merchants )
		{
			if ( merchant.GetId() == id )
			{
				return true;
			}
		}

		return false;
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  sender    The sender
	 * @param[in]  target    The target
	 * @param[in]  rpc_type  The rpc type
	 * @param[in]  ctx       The context
	 */
	override void OnClientRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
			case TajirMerchantComponentRPC.ServerRPC_MerchantConfiguration:
				ReadConfigurationFromServer( ctx );
				break;
		}
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
		if ( !GetGame().IsServer() )
		{
			return;
		}

		SendClientConfiguration( identity, player );
	}

	/**
	 * @brief      Check if a given object is a merchant object.
	 *
	 * @param[in]  object  The object
	 *
	 * @return     True if merchant object, False otherwise.
	 */
	bool IsMerchantObject( Object object )
	{
		foreach ( TajirMerchant merchant : m_merchants )
		{
			if ( merchant.GetObject() == object )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Get a merchant by object.
	 *
	 * @param[in]  object  The object
	 *
	 * @return     True if merchant object, False otherwise.
	 */
	TajirMerchant GetMerchantByObject( Object object )
	{
		foreach ( TajirMerchant merchant : m_merchants )
		{
			if ( merchant.GetObject() == object )
			{
				return merchant;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Keypress handler for opening a store menu from a merchant.
	 *
	 * Context: Client
	 *
	 * @param[in]  UAInput
	 */
	void OnMerchantShopKeyPress( UAInput input )
	{
		if ( GetGame().GetUIManager().GetMenu() != NULL ) 
		{
			return;
		}

		if ( input.LocalRelease() )
		{
			TajirStore store;
			TajirMerchant merchant;
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

			if ( !player )
			{
				return;
			}

			ref array<Object> objects 	 = new array<Object>;
			ref array<CargoBase> proxies = new array<CargoBase>;
			
			GetGame().GetObjectsAtPosition3D( player.GetPosition(), TAJIR_MERCHANT_STOP_TRIGGER_RADIUS, objects, proxies );

			foreach ( Object object : objects )
			{
				merchant = GetMerchantByObject( object );				

				if ( merchant )
				{
					store = merchant.GetStore();
					
					if ( !store )
					{
						TajirLogE( string.Format( "Merchant %1 Not Assigned Store" ), ClassName() );
						return;
					}
					else
					{
						TajirLogD( string.Format( "Merchant: %1 ID %2 @ %3 Store ID %4", merchant.GetName(), merchant.GetId(), merchant.GetObject().GetPosition().ToString(), store.GetId() ), ClassName() );
						
						TajirStoreComponent.GetInstance().ShowStoreMenu( store, merchant );
						
						return;
					}
				}
			}

#ifdef TAJIR_DEBUG
			m_manager.ShowNotification( TajirNotificationType.Info, "No Merchants Nearby", 1000 );
#endif	
		}
	}

	/**
	 * @brief      Send the client the components configuration.
	 * 
	 * Context: Server
	 *
	 * @param[in]  identity  The identity
	 * @param[in]  player    The player
	 */
	void SendClientConfiguration( PlayerIdentity identity, PlayerBase player )
	{		
		GetGame().RPCSingleParam( player, TajirMerchantComponentRPC.ServerRPC_MerchantConfiguration, new Param1<ref array<ref TajirMerchantConfig>>( m_config ), true, identity );
	}

	/**
	 * @brief      Reads configuration from server.
	 * 
	 * Context: Client
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
		
		ref Param1<ref array<ref TajirMerchantConfig>> params = new Param1<ref array<ref TajirMerchantConfig>>( m_config );

		if ( !ctx.Read( params ) ) 
		{
			SetConfigured( false );

			TajirLogE( "Error Reading Configuration From Server", ClassName() );

			return;
		}
		
		for ( int i = 0; i < m_config.Count(); i++ )
		{

			Object object = GetGame().GetObjectByNetworkId( m_config[ i ].network_id_low, m_config[ i ].network_id_high );

			if ( !object )
			{
				TajirLogE( string.Format( "No Object Found For ID %1 %2 in Config Entry %3", m_config[ i ].network_id_low, m_config[ i ].network_id_high, i ), ClassName() );
				continue;
			}

			AddMerchant( new TajirMerchant( m_config[ i ], object ) );

		}
	
		SetConfigured( true );
		ApplyConfiguration();
	}
}