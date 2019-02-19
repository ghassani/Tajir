/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      All Tajir components must inherit this class.
 */
class TajirComponentBase
{
	protected TajirManager m_manager;

	protected string m_name;

	protected bool m_initialized;

	protected bool m_configured;

	/**
	 * @brief      Constructor.
	 *
	 * @param[in]  TajirManager
	 */
	void TajirComponentBase( notnull TajirManager manager )
	{
		m_manager = manager;
		m_initialized = false;
		m_configured  = false;
	}

	/**
	 * @brief      Destructor.
	 */
	void ~TajirComponentBase()
	{

	}
	
	/**
	 * @brief      Gets the name of the component.
	 *
	 * @return     string
	 */
	string GetName()
	{		
		return m_name;
	}

	/**
	 * @brief      Called on component initialixation. Called by TajirManager if m_initialized is false.
	 * @see 	   TajirManager:RegisterComponent
	 * @return 	   void
	 */
	void Init()
	{
		LoadConfiguration();
		ApplyConfiguration();
		
		SetInitialized( true );
	}

	/**
	 * @brief      Check if component has initialized.
	 *
	 * @return     bool
	 */
	bool IsInitialized()
	{
		return m_initialized;
	}

	protected void SetInitialized( bool state )
	{
		m_initialized = state;
	}

	/**
	 * @brief      Check if component has fully configured.
	 *
	 * @return     bool
	 */
	bool IsConfigured()
	{
		return m_configured;
	}

	/**
	 * @brief      Mark the component as configured.
	 *
	 * @param[in]  state  The state to set it to
	 */
	protected void SetConfigured( bool state )
	{
		m_configured = state;
	}

	/**
	 * @brief      Get the TajirManager instance.
	 *
	 * @return     TajirManager
	 */
	TajirManager GetManager()
	{
		return m_manager;
	}

	/**
	 * @brief      Loads the component configuration.
	 *
	 * @return     void
	 */
	void LoadConfiguration()
	{
		if ( GetGame().IsServer() )
		{
			LoadServerConfiguration();
		}
		else
		{
			LoadClientConfiguration();
		}
	}

	/**
	 * @brief      Applys the component configuration.
	 *
	 * @return     void
	 */
	void ApplyConfiguration()
	{
		if ( GetGame().IsServer() )
		{
			ApplyServerConfiguration();
		}
		else
		{
			ApplyClientConfiguration();
		}
	}

	/**
	 * @brief      Loads the component server side configuration.
	 *
	 * @return     void
	 */
	void LoadServerConfiguration() {}

	/**
	 * @brief      Applys the component server side configuration.
	 *
	 * @return     void
	 */
	void ApplyServerConfiguration() {}

	/**
	 * @brief      Loads the component client side configuration.
	 *
	 * @return     void
	 */
	void LoadClientConfiguration() {}

	/**
	 * @brief      Applys the component client side configuration.
	 *
	 * @return     void
	 */
	void ApplyClientConfiguration() {}

	/**
	 * @brief      Called every system update.
	 *
	 * @param[in]  float timesclice
	 * @return     void
	 */
	void OnSystemUpdate( float timeslice ) {}

	/**
	 * @brief      Called every gui update.
	 *
	 * @param[in]  float timesclice
	 * @return     void
	 */
	void OnGuiUpdate( float timeslice )	{}

	/**
	 * @brief      Called every gameplay update.
	 *
	 * @param[in]  float timesclice
	 * @return     void
	 */
	void OnGameplayUpdate( float timeslice ) {}
	
	/**
	 * @brief      Called when server mission starts.
	 */
	void OnServerMissionStart() {}

	/**
	 * @brief      Called when client mission starts.
	 */
	void OnClientMissionStart() {}
	
	/**
	 * @brief      Called when server mission finishes.
	 */
	void OnServerMissionFinish() {}

	/**
	 * @brief      Called when client mission finishes.
	 */
	void OnClientMissionFinish() {}

	/**
	 * @brief      RPC hook on client side
	 *
	 * @param[in]  PlayerIdentity sender
	 * @param[in]  Object target
	 * @param[in]  int rpc_type
	 * @param[in]  ParamsReadContext ctx
	 * @return     void
	 */
	void OnClientRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx ) {}

	/**
	 * @brief      RPC hook on server side
	 *
	 * @param[in]  PlayerIdentity sender
	 * @param[in]  Object target
	 * @param[in]  int rpc_type
	 * @param[in]  ParamsReadContext ctx
	 * @return     void
	 */
	void OnServerRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx ) {}

	/**
	 * @brief      Called when the client first connects.
	 *
	 * @param[in]  player    The player
	 * @param[in]  identity  The identity
	 */
	void OnClientConnected( PlayerBase player, PlayerIdentity identity ) {}

	/**
	 * @brief      Called on server when new client is ready.
	 * 
	 * @see 	   MissionServer:OnClientReadyEvent
	 *
	 * @param[in]  PlayerIdentity sender
	 * @param[in]  PlayerBase player
	 * @return     void
	 */
	void OnClientReady( PlayerIdentity identity, PlayerBase player ) {}

	/**
	 * @brief      Called on server when new client has respawned.
	 * 
	 * @see 	   MissionServer:OnClientRespawnEvent
	 *
	 * @param[in]  PlayerIdentity sender
	 * @param[in]  PlayerBase player
	 * @return     void
	 */
	void OnClientRespawn( PlayerIdentity identity, PlayerBase player ) {}

	/**
	 * @brief      Called on server when a client has reconnected to the server.
	 * 
	 * @see 	   MissionServer:OnClientReconnectEvent
	 *
	 * @param[in]  PlayerIdentity sender
	 * @param[in]  PlayerBase player
	 * @return     void
	 */
	void OnClientReconnect( PlayerIdentity identity, PlayerBase player ) {}

	/**
	 * @brief      Called on server when a client has disconnected from the server.
	 * 
	 * @see 	   MissionServer:OnClientDisconnectedEvent
	 *
	 * @param[in]  PlayerIdentity sender
	 * @param[in]  PlayerBase player
	 * @param[in]  int logoutTime
	 * @param[in]  bool authFailed
	 * @return     void
	 */
	void OnClientDisconnected( PlayerIdentity identity, PlayerBase player, int logoutTime, bool authFailed ) {}

}