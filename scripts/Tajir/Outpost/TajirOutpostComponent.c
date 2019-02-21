/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      Handles created outposts and safezones.
 */
class TajirOutpostComponent extends TajirComponentBase
{
	protected ref array<ref TajirOutpostConfig> m_config;

	protected ref array<ref TajirOutpost> 		m_outposts;

	protected int m_updateEvery = 1000;

	protected int m_updateLast = 0;
	
	/**
	 * @brief      Constructor
	 */
	void TajirOutpostComponent( notnull TajirManager manager )
	{
		m_name 	 	= "TajirOutposts";
		m_config 	= new array<ref TajirOutpostConfig>;
		m_outposts 	= new ref array<ref TajirOutpost>;

		if ( !manager.HasComponent( "TajirMerchants" ) )
		{
			Error2( ClassName(), "TajirMerchants Component Required" );
			return;
		}
	}

	/**
	 * @brief      Destructor.
	 */
	void ~TajirOutpostComponent()
	{
		foreach ( TajirOutpost outpost : m_outposts )
		{
			if ( outpost )
			{
				delete outpost;
			}			
		}
	}

	/**
	 * @brief     Gets the global instance.
	 *
	 * @return    TajirOutpostComponent
	 * @static
	 */
	static TajirOutpostComponent GetInstance()
	{
		if ( TajirManager.GetInstance() )
		{
			if ( TajirManager.GetInstance().HasComponent( "TajirOutposts" ) )
			{
				return TajirOutpostComponent.Cast( TajirManager.GetInstance().GetComponent( "TajirOutposts" ) );
			}
			
			Error( "No Instance of TajirOutposts" );
		}
		else
		{
			Error( "No Instance of TajirManager" );
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
		if ( !FileExist( TAJIR_OUTPOST_CONFIG_USER ) )
		{
			if ( !CopyFile( TAJIR_OUTPOST_CONFIG_DEFAULT, TAJIR_OUTPOST_CONFIG_USER ) )
			{				
				TajirLogE( string.Format( "Error Copying Default Outposts Configuration from %1 to %2", TAJIR_OUTPOST_CONFIG_DEFAULT, TAJIR_OUTPOST_CONFIG_USER ), ClassName() );
			}
		}
		
		JsonFileLoader<ref array<ref TajirOutpostConfig>>.JsonLoadFile( TAJIR_OUTPOST_CONFIG_USER, m_config );

		for ( int i = 0; i < m_config.Count(); i++ )
		{
			if ( m_config[ i ].id <= 0 )
			{
				TajirLogE( string.Format( "Skipping Entry %1 Invalid ID", i ), ClassName() );
				continue;
			} else if ( m_config[ i ].name == "" )
			{
				TajirLogE( string.Format( "Skipping Entry %1 Outpost Name Missing", i ), ClassName() );
				continue;
			}
			
			m_outposts.Insert( new TajirOutpost( m_config[ i ] ) );
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
	 * @brief      Gets all configured outposts.
	 *
	 * @return     ref array<ref TajirOutpost>
	 */
	array<ref TajirOutpost> GetOutposts()
	{
		return m_outposts;
	}

	/**
	 * @brief      Gets the outpost by identifier.
	 *
	 * @param[in]  id    int
	 *
	 * @return     ref TajirOutpost|NULL
	 */
	TajirOutpost GetOutpost( int id )
	{
		foreach ( TajirOutpost outpost : m_outposts )
		{
			if ( outpost.GetId() == id )
			{
				return outpost;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Add a outpost
	 *
	 * @param[in]  TajirOutpost  The tajir outpost
	 *
	 * @return     int count of outposts after insertion
	 */
	int AddOutpost( notnull ref TajirOutpost outpost )
	{
		m_outposts.Insert( outpost );

		return m_outposts.Count();
	}		
	
	/**
	 * @brief      Executed on client when RPC is recieved.
	 *
	 * @param[in]  sender    PlayerIdentity 
	 * @param[in]  target    Object
	 * @param[in]  rpc_type  int
	 * @param[in]  ctx       ParamsReadContext
	 */
	void OnClientRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
			case TajirOutpostComponentRPC.ServerRPC_OutpostConfiguration:
				ReadConfigurationFromServer( ctx );
				break;
			case TajirOutpostComponentRPC.ServerRPC_NotifyEnteredOutpost:
				ReadNotifyEnteredOutpost( ctx );
				break;
			case TajirOutpostComponentRPC.ServerRPC_NotifyLeftOutpost:
				ReadNotifyLeftOutpost( ctx );
				break;
		}
	}

	/**
	 * @brief      Handles showing user notifications related to entering a outpost area.
	 *
	 * @param[in]  outpost TajirOutpost
	 */
	void ShowNotificationEnteredOutpost( TajirOutpost outpost )
	{
		if ( !outpost )
		{
			TajirLogE( "Outpost Not Specified When Attempting To Notify Player Entered", ClassName() );

			m_manager.ShowNotification( TajirNotificationType.Info, "You Entered The Outpost", 3000 );
		}
		else
		{
			m_manager.ShowNotification( TajirNotificationType.Info, string.Format( "You Entered %1", outpost.GetName() ), 3000 );

			if ( outpost.IsSafeZone() )
			{
				m_manager.ShowNotification( TajirNotificationType.Info, "This Outpost is a Safe Zone", 3000 );
			}
		}

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

		if ( player )
		{
			player.SetOutpost( outpost );
		}
	}
	
	/**
	 * @brief      Handles showing user notifications related to entering a outpost area.
	 *
	 * @param[in]  outpost TajirOutpost
	 */
	void ShowNotificationLeftOutpost( TajirOutpost outpost )
	{
		if ( !outpost )
		{
			TajirLogE( "Outpost Not Specified When Attempting To Notify Player Left", ClassName() );

			m_manager.ShowNotification( TajirNotificationType.Info, "You Left The Outpost", 3000 );
		}
		else
		{
			m_manager.ShowNotification( TajirNotificationType.Info, string.Format( "You Left %1", outpost.GetName() ), 3000 );

			if ( outpost.IsSafeZone() )
			{
				m_manager.ShowNotification( TajirNotificationType.Info, "You Have Left The Safezone", 3000 );
			}
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
		SendClientConfiguration( identity, player );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  identity    The identity
	 * @param[in]  player      The player
	 * @param[in]  logoutTime  The logout time
	 * @param[in]  authFailed  The auth failed
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void OnClientDisconnected( PlayerIdentity identity, PlayerBase player, int logoutTime, bool authFailed )
	{
		foreach ( ref TajirOutpost outpost : m_outposts )
		{
			if ( outpost.HasPlayer( player ) )
			{
				outpost.RemovePlayer( player );

				player.SetOutpost( NULL );
			}
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  timeslice  The timeslice
	 */
	void OnGameplayUpdate( float timeslice )
	{
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			if ( GetGame().GetTime() >= ( m_updateLast + m_updateEvery ) )
			{
				ProcessOutposts();

				m_updateLast = GetGame().GetTime();
			}
		}
	}

	/**
	 * @brief      { function_description }
	 */
	void ProcessOutposts( )
	{
		PlayerBase player;
		array<string> threatTypes;
		bool isThreat;

		ref array<Object> 		objects 	= new array<Object>;
		ref array<CargoBase> 	proxies 	= new array<CargoBase>;
		ref array<PlayerBase> 	players 	= new array<PlayerBase>;
		ref array<PlayerBase> 	remove 	 	= new array<PlayerBase>;
		ref array<Object> 		threats 	= new array<Object>;
		ref array<Transport> 	transports  = new array<Transport>;

		foreach ( TajirOutpost outpost : m_outposts )
		{
			objects.Clear();
			proxies.Clear();
			players.Clear();
			threats.Clear();
			transports.Clear();
			remove.Clear();

			GetGame().GetObjectsAtPosition3D( outpost.GetCenterPosition(), outpost.GetRadius(), objects, proxies );

			foreach ( Object object : objects )
			{
				if ( outpost.IsSafeZone() )
				{
					threatTypes = outpost.GetSafeZoneConfig().GetThreats();
					isThreat 	= false;

					if ( outpost.GetSafeZoneConfig().RemoveThreats() && threatTypes.Count() )
					{
						foreach ( string threatType : threatTypes )
						{
							if ( object.IsKindOf( threatType ) )
							{
								isThreat = true;

								threats.Insert( object );

								break;
							}
						}

						if ( isThreat )
						{
							continue;
						}
					}
				}

				if ( object.IsInherited( PlayerBase ) )
				{
					players.Insert( PlayerBase.Cast( object ) );
				}
				else if ( object.IsInherited( Transport ) )
				{
					transports.Insert( Transport.Cast( object ) );
				}
			}

			ProcessOutpostThreats( outpost, threats );
			ProcessOutpostPlayers( outpost, players );
			ProcessOutpostTransports( outpost, transports );
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  outpost  The outpost
	 * @param[in]  array    The array
	 */
	void ProcessOutpostThreats( notnull TajirOutpost outpost, ref array<Object> threats )
	{
		foreach ( Object object : threats )
		{
			if ( !object.IsEntityAI() )
			{
				continue;
			}

			EntityAI threat = EntityAI.Cast( object );

			if ( threat )
			{
#ifdef TAJIR_DEBUG
				TajirLogD( string.Format( "Removing Threat %1 @ %2", threat.GetType(), threat.GetPosition().ToString() ) );
#endif
				threat.Delete();
			}
		}
	}

	void ProcessOutpostPlayers( notnull TajirOutpost outpost, array<PlayerBase> players )
	{
		foreach ( PlayerBase player : players )
		{		
			if ( !outpost.HasPlayer( player ) )
			{
#ifdef TAJIR_DEBUG
				if ( player.GetIdentity() )
				{
					TajirLogD( string.Format( "Adding Player %1 @ %2 To Outpost ID %3", player.GetIdentity().GetId(), player.GetPosition().ToString(), outpost.GetId() ), ClassName() );
				}
				else
				{
					TajirLogD( string.Format( "Adding Player %1 [Merchant: %2] @ %3 To Outpost ID %4", player.GetType(), player.IsMerchant(), player.GetPosition().ToString(), outpost.GetId() ), ClassName() );
				}
#endif
				outpost.AddPlayer( player );
			}

			if ( player.GetIdentity() && player.GetIdentity().GetId().Length() && player.GetOutpost() == NULL )
			{
				NotifyPlayerEnteredOutpost( player, outpost );
			}

			player.SetOutpost( outpost );

			if ( outpost.IsSafeZone() && ( !player.IsMerchant() || !player.GetMerchant().IsUnkillable() ) )
			{
				//player.SetAllowDamage( false );
				player.SetCanBeDestroyed( false );
			}
		}

		// remove any players no longer in the outpost
		ref array<PlayerBase> playersInOutpost = outpost.GetPlayers();

		foreach( PlayerBase inoutpost : playersInOutpost )
		{
			if ( players.Find( inoutpost ) == -1 ) 
			{
				outpost.RemovePlayer( inoutpost );
				
				if ( inoutpost.GetOutpost() && inoutpost.GetOutpost() == outpost )
				{
					if ( inoutpost.GetIdentity() && inoutpost.GetIdentity().GetId().Length() )
					{
						NotifyPlayerLeftOutpost( inoutpost, outpost );
					}
				}

				inoutpost.SetOutpost( NULL );

				if ( outpost.IsSafeZone() && ( !player.IsMerchant() || !player.GetMerchant().IsUnkillable() ) )
				{
					//inoutpost.SetAllowDamage( true );
					inoutpost.SetCanBeDestroyed( true );
				}
			}
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  outpost     The outpost
	 * @param[in]  transports  The transports
	 */
	void ProcessOutpostTransports( notnull TajirOutpost outpost, array<Transport> transports )
	{
		PlayerBase passenger;
		CarScript car;
		int crewsize;

		foreach ( Transport transport : transports )
		{
			if ( !outpost.HasTransport( transport ) )
			{
				outpost.AddTransport( transport );

				if ( outpost.IsSafeZone() )
				{
					//transport.SetAllowDamage( false );
					transport.SetCanBeDestroyed( false );
				}
			}
		}

		// remove any transports no longer in the outpost
		ref array<Transport> inoutpost = outpost.GetTransports();

		for( int i = 0; i < inoutpost.Count(); i++ )
		{
			if ( transports.Find( inoutpost[ i ] ) == -1 ) 
			{
				outpost.RemoveTransport( inoutpost[ i ] );
				
				if ( outpost.IsSafeZone() )
				{
					//transport.SetAllowDamage( true );
					transport.SetCanBeDestroyed( true );
				}
			}
		}
	}

	/**
	 * @brief      Reads outpost configuration from server.
	 * 
	 * Context: Client
	 *
	 * @param[in]  ctx   ParamsReadContext
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
		m_outposts.Clear();

		ref Param1<ref array<ref TajirOutpostConfig>> params = new Param1<ref array<ref TajirOutpostConfig>>( m_config );

		if ( !ctx.Read( params ) ) 
		{
			SetConfigured( false );

			TajirLogE( "Error Reading Configuration From Server", ClassName() );
			return;
		}	
		
		for ( int i = 0; i < m_config.Count(); i++ )
		{
			AddOutpost( new TajirOutpost( m_config[ i ] ) );
		}	

		SetConfigured( true );
		ApplyConfiguration();
	}

	/**
	 * @brief      Read and process notification for entering outpost from the server.
	 * 
	 * Context: Client
	 *
	 * @param[in]  ctx   ParamsReadContext
	 */
	void ReadNotifyEnteredOutpost( ParamsReadContext ctx )
	{
		ref Param1<int> params = new Param1<int>( 0 );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( "Error Reading Notification From Server", ClassName() );
			return;
		}

		ProcessReadNotifyEnteredOutpost( params.param1 );
	}

	/**
	 * @brief      Process notification for entering outpost from the server.
	 * 
	 * Context: Client
	 *
	 * @param[in]  outpostId  The outpost identifier
	 */
	void ProcessReadNotifyEnteredOutpost( int outpostId )
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

		ref TajirOutpost outpost = GetOutpost( outpostId );

		if ( !outpost )
		{
			TajirLogE( string.Format( "Outpost ID %1 Not Found", outpostId ), ClassName() );

			return;
		}
		
		player.SetOutpost( outpost );

		ShowNotificationEnteredOutpost( outpost );		
	}

	/**
	 * @brief      Read and process notification for leaving outpost from the server.
	 * 
	 * Context: Client
	 *
	 * @param[in]  ctx   ParamsReadContext
	 */
	void ReadNotifyLeftOutpost( ParamsReadContext ctx )
	{
		ref Param1<int> params = new Param1<int>( 0 );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( "Error Reading Notification From Server", ClassName() );
			return;
		}

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

		ref TajirOutpost outpost = GetOutpost( params.param1 );

		if ( !outpost )
		{
			TajirLogE( string.Format( "Outpost ID %1 Not Found", params.param1 ), ClassName() );
			return;
		}
		
		player.SetOutpost( NULL );

		ShowNotificationLeftOutpost( outpost );
	}

	/**
	 * @brief      Sends client configuration.
	 * 
	 * Context: Server
	 *
	 * @param[in]  identity  The identity
	 * @param[in]  player    The player
	 */
	void SendClientConfiguration( PlayerIdentity identity, PlayerBase player )
	{		
		player.RPCSingleParam( TajirOutpostComponentRPC.ServerRPC_OutpostConfiguration, new Param1<ref array<ref TajirOutpostConfig>>( m_config ), true, identity );
	}

	/**
	 * @brief      Notifies a player entered outpost.
	 * 
	 * Context: Server
	 *
	 * @param[in]  player   The player
	 * @param[in]  outpost  The outpost
	 */
	void NotifyPlayerEnteredOutpost( PlayerBase player, TajirOutpost outpost )
	{
		if ( GetGame().IsMultiplayer() )
		{
#ifdef TAJIR_DEBUG
			TajirLogD( string.Format( "Notifying Player ID %1 They Entered Outpost ID %2", player.GetIdentity().GetId(), outpost.GetId() ), ClassName() );
#endif
			player.RPCSingleParam( TajirOutpostComponentRPC.ServerRPC_NotifyEnteredOutpost, new Param1<int>( outpost.GetId() ), true, player.GetIdentity() );
		}
	}

	/**
	 * @brief      Notifies a player left outpost.
	 * 
	 * Context: Server
	 *
	 * @param[in]  player   The player
	 * @param[in]  outpost  The outpost
	 */
	void NotifyPlayerLeftOutpost( PlayerBase player, TajirOutpost outpost )
	{
		if ( GetGame().IsMultiplayer() )
		{
#ifdef TAJIR_DEBUG
			TajirLogD( string.Format( "Notifying Player ID %1 They Left Outpost ID %2", player.GetIdentity().GetId(), outpost.GetId() ), ClassName() );
#endif
			player.RPCSingleParam( TajirOutpostComponentRPC.ServerRPC_NotifyLeftOutpost, new Param1<int>( outpost.GetId() ), true, player.GetIdentity() );
		}
	}
}