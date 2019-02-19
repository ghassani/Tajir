/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

enum TajirDebugMenuTabs
{
	PlayerHealth,
	Spawn,
	Stores,
	Merchants,
	Outposts
}

class TajirDebugMenu extends UIScriptedMenu
{
	protected TajirDebugComponent 		m_component;
	protected PlayerBase 				m_player;

	protected ButtonWidget m_tabToggleHealth;
	protected ButtonWidget m_tabToggleSpawn;
	protected ButtonWidget m_tabToggleStores;
	protected ButtonWidget m_tabToggleMerchants;
	protected ButtonWidget m_tabToggleOutposts;

	protected Widget m_tabHealth;
	protected Widget m_tabSpawn;
	protected Widget m_tabStores;
	protected Widget m_tabMerchants;
	protected Widget m_tabOutposts;

	protected  ref TajirEntityHealthSaveState	m_playerState;
	protected  ref array<ref TajirOutpost>		m_outposts;
	protected  ref array<ref TajirStore> 		m_stores;

	protected ref TajirDebugMenuHealthOverview m_playerHealthOverview;
	protected ref TajirDebugMenuMerchants 		m_merchantOverview;
	protected ref TajirDebugMenuOutposts		m_outpostOverview;
	protected ref TajirDebugMenuSpawn 			m_spawnMenu;

	protected float m_lastHealthUpdate 		= 0;
	protected float m_lastMerchantsUpdate 	= 0;
	protected float m_lastOutpostsUpdate 	= 0;

	protected int m_updateHealthEvery 		= 2000;
	protected int m_updateOutpostsEvery 	= 5000;
	protected int m_updateMerchantsEvery 	= 2000;

	void TajirDebugMenu( notnull TajirDebugComponent component )
	{
		m_component 	= component;
		m_player 		= PlayerBase.Cast( GetGame().GetPlayer() );

		m_playerState 	= new TajirEntityHealthSaveState( m_player );
		m_outposts 		= TajirOutpostComponent.GetInstance().GetOutposts();
		m_stores 		= TajirStoreComponent.GetInstance().GetStores();
	}

	override int GetID()
	{
		return TAJIR_MENU_DEBUG_ID;
	}

	/**
	 * @brief      Initialize the root of the menu
	 *
	 * @return     Widget
	 */
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/debug/debug.layout" );
 
		m_tabToggleHealth 			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "TabToggleHealth" ) );
		m_tabToggleSpawn 			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "TabToggleSpawn" ) );
		m_tabToggleStores 			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "TabToggleStores" ) );
		m_tabToggleMerchants  		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "TabToggleMerchants" ) );
		m_tabToggleOutposts  		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "TabToggleOutposts" ) );

		m_tabHealth 				= layoutRoot.FindAnyWidget( "DebugTabHealth" );
		m_tabSpawn 					= layoutRoot.FindAnyWidget( "DebugTabSpawn" );
		m_tabStores 				= layoutRoot.FindAnyWidget( "DebugTabStores" );
		m_tabMerchants 				= layoutRoot.FindAnyWidget( "DebugTabMerchants" );
		m_tabOutposts  	 			= layoutRoot.FindAnyWidget( "DebugTabOutposts" );

		m_playerHealthOverview 		= new TajirDebugMenuHealthOverview( m_tabHealth, EntityAI.Cast( m_player ) );
		m_merchantOverview 			= new TajirDebugMenuMerchants( m_tabMerchants );
		m_outpostOverview 			= new TajirDebugMenuOutposts( m_tabOutposts );
		m_spawnMenu 				= new TajirDebugMenuSpawn( m_tabSpawn );

		SetTabView( TajirDebugMenuTabs.PlayerHealth );

		return layoutRoot;
	}

	/**
	 * @brief      Locks the controls.
	 *
	 * @return     void
	 */
	override void LockControls()
	{
		super.LockControls();
        GetGame().GetMission().PlayerControlDisable();
	}

	/**
	 * @brief      Unlocks the controls.
	 *
	 * @return     void
	 */
	override void UnlockControls()
	{
		super.UnlockControls();
		GetGame().GetMission().PlayerControlEnable();
	}

	/**
	 * @brief      Called when the menu is hidden.
	 *
	 * @return     void
	 */
	override void OnHide()
	{
		super.OnHide();

		m_component.OnDebugMenuClose();

		Close();
	}

	void SetTabView( TajirDebugMenuTabs tab )
	{
		m_tabHealth.Show( tab == TajirDebugMenuTabs.PlayerHealth );
		m_tabSpawn.Show( tab == TajirDebugMenuTabs.Spawn );
		m_tabStores.Show( tab == TajirDebugMenuTabs.Stores );
		m_tabMerchants.Show( tab == TajirDebugMenuTabs.Merchants );
		m_tabOutposts.Show( tab == TajirDebugMenuTabs.Outposts );
	}

	void OnClientRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
			case TajirDebugComponentRPC.ServerRPC_UpdatePlayerResponse:
				ReadUpdatePlayerResponse( ctx );
				return;
		}

		if ( m_merchantOverview.OnClientRPC( sender, target, rpc_type, ctx ) )
		{
			return;
		}
		
		if ( m_outpostOverview.OnClientRPC( sender, target, rpc_type, ctx ) )
		{
			return;
		}
	}

	override void Update( float timeslice )
	{
		int time = GetGame().GetTime();

		if ( !m_lastHealthUpdate || (m_lastHealthUpdate + m_updateHealthEvery) <= time )
		{
			m_lastHealthUpdate = time;

			UpdatePlayerRequest();
		}

		if ( !m_lastMerchantsUpdate || (m_lastMerchantsUpdate + m_updateMerchantsEvery) <= time )
		{
			m_lastMerchantsUpdate = time;

			UpdateMerchantsRequest();
		}

		if ( !m_lastOutpostsUpdate || (m_lastOutpostsUpdate + m_updateOutpostsEvery) <= time )
		{
			m_lastOutpostsUpdate = time;

			UpdateOutpostsRequest();
		}

		if ( m_playerState )
		{
			m_playerHealthOverview.UpdateFromSaveState( m_playerState );
		}
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_tabToggleHealth )
		{
			SetTabView( TajirDebugMenuTabs.PlayerHealth );
			return true;
		}
		else if ( w == m_tabToggleSpawn )
		{
			SetTabView( TajirDebugMenuTabs.Spawn );
			return true;
		}
		else if ( w == m_tabToggleStores )
		{
			SetTabView( TajirDebugMenuTabs.Stores );
			return true;
		}
		else if ( w == m_tabToggleMerchants )
		{
			SetTabView( TajirDebugMenuTabs.Merchants );
			return true;
		}
		else if ( w == m_tabToggleOutposts )
		{
			SetTabView( TajirDebugMenuTabs.Outposts );
			return true;
		}

		if ( m_merchantOverview.OnClick( w, x, y, button ) )
		{
			return true;
		}

		if ( m_outpostOverview.OnClick( w, x, y, button ) )
		{
			return true;
		}

		if ( m_spawnMenu.OnClick( w, x, y, button ) )
		{
			return true;
		}		

		return false;
	}
	
	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		if ( m_merchantOverview.OnChange( w, x, y, finished ) )
		{
			return true;
		}

		if ( m_outpostOverview.OnChange( w, x, y, finished ) )
		{
			return true;
		}

		if ( m_spawnMenu.OnChange( w, x, y, finished ) )
		{
			return true;
		}

		return false;
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int  column,	int  oldRow, int  oldColumn )
	{
		if ( m_merchantOverview.OnItemSelected( w, x, y, row, column, oldRow, oldColumn ) )
		{
			return true;
		}

		if ( m_outpostOverview.OnItemSelected( w, x, y, row, column, oldRow, oldColumn ) )
		{
			return true;
		}

		if ( m_spawnMenu.OnItemSelected( w, x, y, row, column, oldRow, oldColumn ) )
		{
			return true;
		}

		return false;
	}

	void UpdatePlayerRequest()
	{
		//TajirLogD( "UpdatePlayerRequest", ClassName() );

		GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_UpdatePlayerRequest, new Param1<bool>( true ), true );
	}

	void UpdateStoresRequest()
	{
		//TajirLogD( "UpdateStoresRequest", ClassName() );

		GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_GetStoreInfo, new Param1<bool>( true ), true );
	}

	void UpdateMerchantsRequest()
	{
		//TajirLogD( "UpdateMerchantsRequest", ClassName() );

		GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_GetMerchantInfo, new Param1<bool>( true ), true );
	}

	void UpdateOutpostsRequest()
	{
		//TajirLogD( "UpdateOutpostsRequest", ClassName() );

		GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_GetOutpostInfo, new Param1<bool>( true ), true );
	}

	void ReadUpdatePlayerResponse( ParamsReadContext ctx )
	{
		ref Param1<TajirEntityHealthSaveState> params = new Param1<TajirEntityHealthSaveState>( m_playerState );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading Update Player Request" ), ClassName() );
			return;
		}
	}
}