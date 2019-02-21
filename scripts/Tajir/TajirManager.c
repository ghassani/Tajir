/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirManager
{
	private ref static TajirManager 				s_instance;

	protected ref TajirConfig 						m_config;
	
	protected ref array<ref TajirComponentBase>		m_components;

	protected ref TajirNotificationsManager			m_notifications;

	protected ref TajirKeyBindManager 				m_keybinds;

	protected ref TajirFileLogger 					m_logger;

	protected bool m_configured 				 	= false;

	protected bool m_initialized 				 	= false;

	protected bool m_isServer 						= false;

	protected bool m_isClient 						= false;

	protected bool m_isOffline 						= false;

	/**
	 * @brief      { function_description }
	 */
	void TajirManager()
	{
		if ( s_instance != NULL )
		{
			Error( "Tajir Manager Already Initialized" );
			return;
		}

		m_config 		= new TajirConfig();
		m_components	= new array<ref TajirComponentBase>;
		m_isServer 		= GetGame().IsServer() && GetGame().IsMultiplayer();
		m_isClient 		= GetGame().IsClient() && GetGame().IsMultiplayer();
		m_isOffline 	= GetGame().IsServer() && !GetGame().IsMultiplayer();

		s_instance = this;

		GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Insert( this.OnSystemUpdate );
		GetGame().GetUpdateQueue( CALL_CATEGORY_GUI ).Insert( this.OnGuiUpdate );
		GetGame().GetUpdateQueue( CALL_CATEGORY_GAMEPLAY ).Insert( this.OnGameplayUpdate );

		if ( IsServer() || IsClient() )
		{
			GetDayZGame().Event_OnRPC.Insert( this.OnRPC );
		}

		if ( IsClient() || IsOffline() )
		{
			m_notifications = new TajirNotificationsManager();
			m_keybinds 		= new TajirKeyBindManager();
		}

		m_logger = new TajirFileLogger( TAJIR_LOG_FILE );

		if ( IsClient() || IsOffline() )
		{
			LoadWidgetImageSet( "Tajir/gui/imagesets/icons.imageset" );
			LoadWidgetStyles( "Tajir/gui/lookandfeel/tajir.styles" );
		}
	}

	/**
	 * @brief      Destroys the object.
	 */
	void ~TajirManager()
	{
		GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Remove( this.OnSystemUpdate );
		GetGame().GetUpdateQueue( CALL_CATEGORY_GUI ).Remove( this.OnGuiUpdate );
		GetGame().GetUpdateQueue( CALL_CATEGORY_GAMEPLAY ).Remove( this.OnGameplayUpdate );
		
		if ( IsServer() || IsClient() )
		{
			GetDayZGame().Event_OnRPC.Remove( this.OnRPC );
		}

		for ( int i = 0; i < m_components.Count(); ++i )
		{			
			if ( m_components[ i ] != NULL )
			{
				delete m_components[ i ];
			}
		}

		m_components.Clear();

		if (  m_notifications != NULL )
		{
			delete m_notifications;
		}

		if (  m_keybinds != NULL )
		{
			delete m_keybinds;
		}

		s_instance = NULL;
	}

	/**
	 * @brief      Gets the instance.
	 *
	 * @return     The instance.
	 */
	static TajirManager GetInstance()
	{
		return s_instance;
	}

	/**
	 * @brief      { function_description }
	 */
	void Init()
	{
		LoadConfiguration();
		ApplyConfiguration();

		for ( int i = 0; i < m_components.Count(); ++i )
		{
			m_components[ i ].Init();
		}

		m_initialized = true;
	}

	/**
	 * @brief      Determines if server.
	 *
	 * @return     True if server, False otherwise.
	 */
	bool IsServer()
	{
		return m_isServer;
	}

	/**
	 * @brief      Determines if client.
	 *
	 * @return     True if client, False otherwise.
	 */
	bool IsClient()
	{
		return m_isClient;
	}

	/**
	 * @brief      Determines if offline.
	 *
	 * @return     True if offline, False otherwise.
	 */
	bool IsOffline()
	{
		return m_isOffline;
	}

	/**
	 * @brief      Loads a configuration.
	 */
	void LoadConfiguration()
	{
		if ( TAJIR_CONFIG_DIR.Length() && !FileExist( TAJIR_CONFIG_DIR ) )
		{
			TajirLogD( string.Format( "Creating Directory %1", TAJIR_CONFIG_DIR ), ClassName() );

			if ( !MakeDirectory( TAJIR_CONFIG_DIR ) )
			{
				TajirLogE( string.Format( "Unable to Create Directory %1", TAJIR_CONFIG_DIR ) );
			}
		}

		if ( IsClient() || IsOffline() )
		{
			if ( !FileExist( TAJIR_CLIENT_CONFIG_USER ) )
			{
				if ( !CopyFile( TAJIR_CLIENT_CONFIG_DEFAULT, TAJIR_CLIENT_CONFIG_USER ) )
				{				
					TajirLogE( string.Format( "Error Copying Default Client Configuration from %1 to %2", TAJIR_CLIENT_CONFIG_DEFAULT, TAJIR_CLIENT_CONFIG_USER ), ClassName() );
				}
			}

			TajirLogD( string.Format( "Loading Client Config: %1", TAJIR_CLIENT_CONFIG_USER ), ClassName() );

			JsonFileLoader<ref TajirConfig>.JsonLoadFile( TAJIR_CLIENT_CONFIG_USER, m_config );
		} 
		
		m_configured = true;
	}

	/**
	 * @brief      { function_description }
	 */
	void ApplyConfiguration()
	{
		
	}

	/**
	 * @brief      Determines if configured.
	 *
	 * @return     True if configured, False otherwise.
	 */
	bool IsConfigured()
	{
		return m_configured;
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool AllComponentsConfigured()
	{
		for ( int i = 0; i < m_components.Count(); ++i )
		{
			if ( !m_components[ i ].IsConfigured() )
			{
				return false;
			}	
		}

		return true;
	}

	/**
	 * @brief      Determines if initialized.
	 *
	 * @return     True if initialized, False otherwise.
	 */
	bool IsInitialized()
	{
		return m_initialized;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool AllComponentsInitialized()
	{
		for ( int i = 0; i < m_components.Count(); ++i )
		{
			if ( !m_components[ i ].IsInitialized() )
			{
				return false;
			}	
		}

		return true;
	}

	/**
	 * @brief      Gets the logger.
	 *
	 * @return     TajirFileLogger
	 */
	TajirFileLogger GetLogger()
	{
		return m_logger;
	}

	/**
	 * @brief      Gets the configuration.
	 *
	 * @return     The configuration.
	 */
	TajirConfig GetConfig()
	{
		return m_config;
	}

	/**
	 * @brief      Gets the keybinding configuration.
	 *
	 * @return     The keybinding configuration.
	 */
	array<ref TajirConfigKeyBinding> GetKeybindingConfig()
	{
		return m_config.keybindings;
	}

	TajirKeyBindManager GetKeybindManager()
	{
		return m_keybinds;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirComponentBase  The tajir component base
	 * @param[in]  replace             The replace
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool RegisterComponent( ref TajirComponentBase component, bool replace = false )
	{
		if ( HasComponent( component.GetName() ) )
		{
			if ( !replace )
			{
				Error2( ClassName(), string.Format( "Component Already Has %1", component.GetName() ) );
				return false;
			}

			RemoveComponent( component.GetName() );
		}

		m_components.Insert( component );

		// runtime register, call init of component if manager has been initialized already.
		if ( m_initialized && !component.IsInitialized() )
		{
			component.Init();
		}

		TajirLogD( string.Format( "Registered Component %1", component.GetName() ), ClassName() );

		return true;
	}

	/**
	 * @brief      Removes a component.
	 *
	 * @param[in]  name  The name
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool RemoveComponent( string name )
	{
		for ( int i = 0; i < m_components.Count(); ++i )
		{
			if ( m_components[ i ].GetName() == name )
			{
				delete m_components[ i ];

				m_components.Remove( i );

				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Determines if it has component.
	 *
	 * @param[in]  name  The name
	 *
	 * @return     True if has component, False otherwise.
	 */
	bool HasComponent( string name )
	{
		for ( int i = 0; i < m_components.Count(); ++i )
		{
			if ( m_components[ i ].GetName() == name )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Determines if it has component.
	 *
	 * @param[in]  component  The component
	 *
	 * @return     True if has component, False otherwise.
	 */
	bool HasComponent( TajirComponentBase component )
	{
		for ( int i = 0; i < m_components.Count(); ++i )
		{
			if ( m_components[ i ]  == component )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Gets the component.
	 *
	 * @param[in]  name  The name
	 *
	 * @return     The component.
	 */
	TajirComponentBase GetComponent( string name )
	{
		for ( int i = 0; i < m_components.Count(); ++i )
		{
			if ( m_components[ i ].GetName() == name )
			{
				return m_components[ i ];
			}
		}
		
		return NULL;
	}

	/**
	 * @brief      Shows the notification.
	 *
	 * @param[in]  type          The type
	 * @param[in]  message       The message
	 * @param[in]  closeAfterMs  The close after milliseconds
	 */
	void ShowNotification( TajirNotificationType type, string message, int closeAfterMs )
	{
		if ( m_notifications != NULL )
		{
			m_notifications.ShowNotification( type, message, closeAfterMs );
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  type          The type
	 * @param[in]  message       The message
	 * @param[in]  showAfterMs   The show after milliseconds
	 * @param[in]  closeAfterMs  The close after milliseconds
	 */
	void QueueNotification( TajirNotificationType type, string message, int showAfterMs, int closeAfterMs )
	{
		if ( m_notifications != NULL )
		{
			m_notifications.QueueNotification( type, message, showAfterMs, closeAfterMs );
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  timeslice  The timeslice
	 */
	void OnSystemUpdate( float timeslice )
	{
		for ( int i = 0; i < m_components.Count(); ++i )
		{
			m_components[ i ].OnSystemUpdate( timeslice );
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  timeslice  The timeslice
	 */
	void OnGuiUpdate( float timeslice )
	{
		for ( int i = 0; i < m_components.Count(); ++i )
		{
			m_components[ i ].OnGuiUpdate( timeslice );
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  timeslice  The timeslice
	 */
	void OnGameplayUpdate( float timeslice )
	{
		if ( IsClient() || IsOffline() )
		{
			m_keybinds.OnClientUpdate( timeslice );
		}

		for ( int i = 0; i < m_components.Count(); ++i )
		{
			m_components[ i ].OnGameplayUpdate( timeslice );
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  sender    The sender
	 * @param[in]  target    The target
	 * @param[in]  rpc_type  The rpc type
	 * @param[in]  ctx       The context
	 */
	void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		if ( IsClient() )
		{
			switch ( rpc_type )
			{
				case TajirManagerRPC.ServerRPC_SendNotification:
					m_notifications.ReadNotificationFromServer( ctx );
					break;
				case TajirManagerRPC.ServerRPC_QueueNotification:
					m_notifications.ReadQueueNotificationFromServer( ctx );
					break;
			}
		}
		
		for ( int i = 0; i < m_components.Count(); ++i )
		{
			if ( IsServer() )
			{
				m_components[ i ].OnServerRPC( sender, target, rpc_type, ctx );
			}
			else if ( IsClient() )
			{
				m_components[ i ].OnClientRPC( sender, target, rpc_type, ctx );
			}
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  player    The player
	 * @param[in]  identity  The identity
	 */
	void OnClientConnected( PlayerBase player, PlayerIdentity identity )
	{
		if ( !IsServer() )
		{
			return;
		}

		for ( int i = 0; i < m_components.Count(); ++i )
		{
			m_components[ i ].OnClientConnected( player,  identity );			
		}
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  identity  The identity
	 * @param[in]  player    The player
	 */
	void OnClientReady( PlayerIdentity identity, PlayerBase player )
	{
		if ( !IsServer() )
		{
			return;
		}

		for ( int i = 0; i < m_components.Count(); ++i )
		{
			m_components[ i ].OnClientReady( identity,  player );			
		}
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  identity  The identity
	 * @param[in]  player    The player
	 */
	void OnClientRespawn( PlayerIdentity identity, PlayerBase player )
	{
		if ( !IsServer() )
		{
			return;
		}

		for ( int i = 0; i < m_components.Count(); ++i )
		{
			m_components[ i ].OnClientRespawn( identity,  player );			
		}
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  identity  The identity
	 * @param[in]  player    The player
	 */
	void OnClientReconnect( PlayerIdentity identity, PlayerBase player )
	{
		if ( !IsServer() )
		{
			return;
		}

		for ( int i = 0; i < m_components.Count(); ++i )
		{
			m_components[ i ].OnClientReconnect( identity,  player );			
		}
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  identity    The identity
	 * @param[in]  player      The player
	 * @param[in]  logoutTime  The logout time
	 * @param[in]  authFailed  The auth failed
	 */
	void OnClientDisconnected( PlayerIdentity identity, PlayerBase player, int logoutTime, bool authFailed )
	{
		if ( !IsServer() )
		{
			return;
		}

		for ( int i = 0; i < m_components.Count(); ++i )
		{
			m_components[ i ].OnClientDisconnected( identity, player, logoutTime, authFailed );			
		}
	}

	/**
	 * @brief      { function_description }
	 */
	void OnMissionStart()
	{
		for ( int i = 0; i < m_components.Count(); ++i )
		{
			if ( IsServer() )
			{
				m_components[ i ].OnServerMissionStart();	
			}
			else
			{
				m_components[ i ].OnClientMissionStart();	
			}
		}
	}

	/**
	 * @brief      { function_description }
	 */
	void OnMissionFinish()
	{		
		for ( int i = 0; i < m_components.Count(); ++i )
		{
			if ( IsServer() )
			{
				m_components[ i ].OnServerMissionFinish();	
			}
			else
			{
				m_components[ i ].OnClientMissionFinish();	
			}		
		}
	}
}