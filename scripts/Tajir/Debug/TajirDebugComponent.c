/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

enum RPCUpdatePlayerStatType
{
	WATER = 1,
	ENERGY,
	BLOOD
}

/**
 * @brief      Used in development only. Do not enable this component for release.
 */
class TajirDebugComponent extends TajirComponentBase
{
	protected ref TajirDebugMenu m_menu;

	protected ref TajirDebugOutpostInfo m_lastOutpostInfo;

	/**
	 * @brief      Constructor
	 *
	 * @param[in]  TajirManager
	 */
	void TajirDebugComponent( notnull TajirManager manager )
	{
		m_name 		      = "TajirDebug";

		m_lastOutpostInfo = new TajirDebugOutpostInfo();
	}

	/**
	 * @brief      Destructor
	 */
	void ~TajirDebugComponent()
	{
		
	}

	/**
	 * @brief      Convinence static function to get the instance from TajirManager
	 */
	static TajirDebugComponent GetInstance()
	{
		return TajirDebugComponent.Cast( TajirManager.GetInstance().GetComponent( "TajirDebug" ) );
	}


	TajirDebugOutpostInfo GetLastOutpostInfo()
	{
		return m_lastOutpostInfo;
	}

	/**
	 * @brief      Loads the component server side configuration.
	 *
	 * @return     void
	 */
	override void LoadServerConfiguration()
	{	
		SetConfigured( true );
	}

	/**
	 * @brief      Loads the component client side configuration.
	 *
	 * @return     void
	 */
	override void LoadClientConfiguration()
	{
		SetConfigured( true );
	}

	/**
	 * @brief      Applys the component client side configuration.
	 *
	 * @return     void
	 */
	override void ApplyClientConfiguration()
	{
		if ( m_manager.GetKeybindManager().HasBinding( "safe_zone_debug_menu_toggle" ) )
		{
			TajirLogE( "safe_zone_debug_menu_toggle Already Bound", ClassName() );
			return;
		}
		
		TajirConfigKeyBinding config = m_manager.GetConfig().GetKeybindConfig( "safe_zone_debug_menu_toggle" );
		
		ref TajirKeyBinding binding = new TajirKeyBinding( "safe_zone_debug_menu_toggle", "Toggles the safe zone debug menu", this, "OnToggleDebugMenu" );

		if ( config )
		{
			for ( int i = 0; i < config.keybinds.Count(); i++ )
			{
				binding.BindCombo( config.keybinds[ i ] );
			}
		}
		else
		{
			binding.BindCombo( "kL" );
		}
		
		m_manager.GetKeybindManager().AddBinding( binding );
	}

	/**
	 * @brief      Keypress handler for opening the safe zone debug menu.
	 *
	 * Context: Client
	 *
	 * @param[in]  UAInput
	 */
	void OnToggleDebugMenu( UAInput input )
	{
		if ( input.LocalRelease() )
		{
			if ( !GetGame().GetUIManager().GetMenu() )
			{
				//if ( m_menu ) delete m_menu;

				m_menu = new TajirDebugMenu( this );

				GetGame().GetUIManager().ShowScriptedMenu( m_menu, NULL );
			}
		}
	}

	void OnDebugMenuClose()
	{
		//delete m_menu;
		m_menu = NULL;
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
		if ( m_menu )
		{
			m_menu.OnClientRPC( sender, target, rpc_type, ctx );
		}
	}

	/**
	 * @brief      RPC hook on server side
	 *
	 * @param[in]  PlayerIdentity sender
	 * @param[in]  Object target
	 * @param[in]  int rpc_type
	 * @param[in]  ParamsReadContext ctx
	 * @return     void
	 */
	void OnServerRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		PlayerBase player = PlayerBase.Cast( target );

		switch ( rpc_type )
		{
			case TajirDebugComponentRPC.ClientRPC_UpdatePlayerRequest:
				ReadUpdatePlayerRequest( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_SpawnManWithCurrency:
				SpawnManWithCurrency( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_SpawnManWithCurrencyAndItems:
				SpawnManWithCurrencyAndItems( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_GiveCurrency:
				GiveCurrency( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_GiveAllCurrency:
				GiveAllCurrency( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_GetStoreInfo:
				GetStoreInfo( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_GetMerchantInfo:
				GetMerchantInfo( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_GetOutpostInfo:
				GetOutpostInfo( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_TeleportOutpost:
				TeleportPlayerToOutpost( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_TeleportMerchant:
				TeleportPlayerToMerchant( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_UpdateSafeZoneSettings:
				UpdateOutpostSafeZoneSettings( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_SpawnObjectAtPosition:
				SpawnObject( player, ctx );
				break;
			case TajirDebugComponentRPC.ClientRPC_UpdatePlayerStat:
				UpdatePlayerStat( player, ctx );
				break;
		}
	}

	void UpdatePlayerStat( PlayerBase player, ParamsReadContext ctx )
	{
		ref Param2<int, int> params = new Param2<int, int>( 0, 0 );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading Update Player Stat Request" ), ClassName() );
			return;
		}

		switch( params.param1 )
		{
			case RPCUpdatePlayerStatType.WATER:
			{
				PlayerStat<float> waterStat = PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.WATER ) );
				if ( params.param2 )
				{					
					waterStat.Set( waterStat.GetMax() );
					TajirLogD( "Replenished Player Water Level", ClassName() );
				}
				else
				{
					waterStat.Set( 0.00 );
					TajirLogD( "Depleted Player Water Level", ClassName() );
				}
				break;
			}
			case RPCUpdatePlayerStatType.ENERGY:
			{
				PlayerStat<float> energyStat = PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.ENERGY ) );
				if ( params.param2 )
				{					
					energyStat.Set( energyStat.GetMax() );
					TajirLogD( "Replenished Player Energy Level", ClassName() );

				}
				else
				{
					energyStat.Set( 0.00 );
					TajirLogD( "Depleted Player Energy Level", ClassName() );
				}
				break;
			}
			case RPCUpdatePlayerStatType.BLOOD:	
			{			
				if ( params.param2 )
				{					
					TajirLogD( "Replenished Player Blood Level", ClassName() );
					player.SetHealth( "GlobalHealth", "Blood", player.GetMaxHealth( "GlobalHealth", "Blood" ) );
				}
				else
				{
					TajirLogD( "Depleted Player Blood Level", ClassName() );
					player.SetHealth( "GlobalHealth", "Blood", 0.00 );
				}
				break;
			}
		}
	}

	void ReadUpdatePlayerRequest( PlayerBase player, ParamsReadContext ctx )
	{
		ref Param1<bool> params = new Param1<bool>( false );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading Update Safe Zone Request" ), ClassName() );
			return;
		}

		TajirLogD( string.Format( "Sending Player %1 Their Health State", player.GetIdentity().GetId() ), ClassName() );

		ref TajirEntityHealthSaveState state = new TajirEntityHealthSaveState( player );

		player.RPCSingleParam( TajirDebugComponentRPC.ServerRPC_UpdatePlayerResponse, new Param1<TajirEntityHealthSaveState>( state ), true, player.GetIdentity() );
	}

	void ReadOutpostInfoResponse( ParamsReadContext ctx )
	{
		ref Param1<ref TajirDebugOutpostInfo> params = new Param1<ref TajirDebugOutpostInfo>( m_lastOutpostInfo );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading Update Player Request" ), ClassName() );
			return;
		}

		if ( GetGame().GetUIManager().GetMenu() && GetGame().GetUIManager().GetMenu().GetID() == TAJIR_MENU_DEBUG_ID )
		{
			TajirDebugMenu menu = TajirDebugMenu.Cast(  GetGame().GetUIManager().GetMenu() );

			if ( menu )
			{
				//menu.UpdateOutpostInfo( m_lastOutpostInfo );
			}
		}
	}

	protected void SpawnManWithCurrency( PlayerBase player, ParamsReadContext ctx )
	{
		vector pos = Vector( 0, 0, 0 );

		ref Param1<vector> params = new Param1<vector>( pos );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading SpawnManWithCurrency" ), ClassName() );
			return;
		}

		ManBase man = ManBase.Cast( GetGame().CreateObject( "SurvivorM_Hassan", player.GetPosition(), false, true, true ) );

		if ( man )
		{
			TajirSpawnHelper.SetUpNewEntity( EntityAI.Cast( man ) );

			man.SetCurrency( 20000 );

			TajirLogE( string.Format( "Created Man With Currency At %1", player.GetPosition().ToString() ), ClassName() );
		}
	}

	protected void SpawnManWithCurrencyAndItems( PlayerBase player, ParamsReadContext ctx )
	{
		vector pos = Vector( 0, 0, 0 );

		ref Param1<vector> params = new Param1<vector>( pos );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading SpawnManWithCurrencyAndItems" ), ClassName() );
			return;
		}

		ManBase man = ManBase.Cast( GetGame().CreateObject( "SurvivorM_Hassan", player.GetPosition(), false, true, true ) );

		if ( man )
		{
			TajirSpawnHelper.SetUpNewEntity( EntityAI.Cast( man ) );

			man.SetCurrency( 20000 );

			EntityAI bp = man.GetInventory().CreateInInventory( "AliceBag_Green" );
			
			bp.GetInventory().CreateInInventory( "CZ75" );
			bp.GetInventory().CreateInInventory( "Mag_CZ75_15Rnd" );
			bp.GetInventory().CreateInInventory( "Mag_CZ75_15Rnd" );
			bp.GetInventory().CreateInInventory( "Mag_CZ75_15Rnd" );
			bp.GetInventory().CreateInInventory( "Mag_CZ75_15Rnd" );

			TajirLogE( string.Format( "Created Man With Currency and Items At %1", player.GetPosition().ToString() ), ClassName() );

		}
	}

	protected void GiveCurrency( PlayerBase player, ParamsReadContext ctx )
	{
		vector pos = Vector( 0 , 0, 0 );

		ref Param1<vector> params = new Param1<vector>( pos );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading GiveCurrency" ), ClassName() );
			return;
		}

		player.AddCurrency( 5000 );
	}

	protected void GiveAllCurrency( PlayerBase player, ParamsReadContext ctx )
	{
		vector pos = Vector( 0 , 0, 0 );

		ref Param1<vector> params = new Param1<vector>( pos );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading GiveAllCurrency" ), ClassName() );
			return;
		}
		
		ref array<Man> playerlist  = new array<Man>;

		GetGame().GetPlayers( playerlist );

		foreach( Man man : playerlist )
		{
			ManBase manbase = ManBase.Cast( man );

			if ( manbase )
			{
				manbase.AddCurrency( 5000 );
			}
		}
	}
	
	protected void GetStoreInfo( PlayerBase player, ParamsReadContext ctx )
	{
		//TajirLogD( "Reading Store Info Request", ClassName() );

	}
	
	protected void GetMerchantInfo( PlayerBase player, ParamsReadContext ctx )
	{
		//TajirLogD( "Reading Merchant Info Request", ClassName() );

		ref Param1<int> params = new Param1<int>( 0 );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading GetMerchantInfo" ), ClassName() );
			return;
		}

		ref array<ref TajirDebugMerchantInfo> infos = new  array<ref TajirDebugMerchantInfo>;

		ref array<ref TajirMerchant> merchants = TajirMerchantComponent.GetInstance().GetMerchants();

		foreach( TajirMerchant merchant : merchants )
		{
			infos.Insert( new TajirDebugMerchantInfo( merchant ) );
		}

		player.RPCSingleParam( TajirDebugComponentRPC.ServerRPC_MerchantInfoResponse, new Param1<ref array<ref TajirDebugMerchantInfo>>( infos ), true, player.GetIdentity() );
	}
	
	protected void GetOutpostInfo( PlayerBase player, ParamsReadContext ctx )
	{
		//TajirLogD( "Reading Outpost Info Request", ClassName() );

		ref Param1<int> params = new Param1<int>( 0 );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading GetOutpostInfo" ), ClassName() );
			return;
		}


		ref array<ref TajirDebugOutpostInfo> infos = new  array<ref TajirDebugOutpostInfo>;

		ref array<ref TajirOutpost> outposts = TajirOutpostComponent.GetInstance().GetOutposts();

		foreach( TajirOutpost outpost : outposts )
		{
			infos.Insert( new TajirDebugOutpostInfo( outpost ) );
		}

		player.RPCSingleParam( TajirDebugComponentRPC.ServerRPC_OutpostInfoResponse, new Param1<ref array<ref TajirDebugOutpostInfo>>( infos ), true, player.GetIdentity() );
	}	

	protected void TeleportPlayerToOutpost( PlayerBase player, ParamsReadContext ctx )
	{
		ref Param1<int> params = new Param1<int>( 0 );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading TeleportPlayerToOutpost" ), ClassName() );
			return;
		}

		TajirOutpost outpost = TajirOutpostComponent.GetInstance().GetOutpost( params.param1 );

		if ( outpost )
		{
			TajirLogD( string.Format( "Teleporting Player %1 To Outpost %2 At %3", player.GetIdentity().GetId(), outpost.GetId(), outpost.GetCenterPosition().ToString() ), ClassName() );

			player.SetPosition( outpost.GetCenterPosition() );
		}
	}	

	protected void TeleportPlayerToMerchant( PlayerBase player, ParamsReadContext ctx )
	{
		ref Param1<int> params = new Param1<int>( 0 );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading TeleportPlayerToMerchant" ), ClassName() );
			return;
		}

		TajirMerchant merchant = TajirMerchantComponent.GetInstance().GetMerchant( params.param1 );

		if ( merchant && merchant.GetObject() )
		{
			TajirLogD( string.Format( "Teleporting Player %1 To Merchant %2 At %3", player.GetIdentity().GetId(), merchant.GetId(), merchant.GetObject().GetPosition().ToString() ), ClassName() );

			player.SetPosition( merchant.GetObject().GetPosition() );
		}
	}	

	protected void UpdateOutpostSafeZoneSettings( PlayerBase player, ParamsReadContext ctx )
	{
		ref TajirOutpostSafeZoneConfig config = new TajirOutpostSafeZoneConfig();

		ref Param2<int, ref TajirOutpostSafeZoneConfig> params = new Param2<int, ref TajirOutpostSafeZoneConfig>( 0, config );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading UpdateOutpostSafeZoneSettings" ), ClassName() );
			return;
		}

		TajirOutpost outpost = TajirOutpostComponent.GetInstance().GetOutpost( params.param1 );

		if ( outpost )
		{
			TajirLogD( string.Format( "Updating Outpost %1 Safe Zone Config", outpost.GetId() ), ClassName() );

			outpost.GetSafeZoneConfig().enabled 				= config.enabled;
			outpost.GetSafeZoneConfig().remove_threats 			= config.remove_threats;
			outpost.GetSafeZoneConfig().unlock_melee 			= config.unlock_melee;
			outpost.GetSafeZoneConfig().unlock_weapon_fire 		= config.unlock_weapon_fire;
			outpost.GetSafeZoneConfig().unlock_trap 			= config.unlock_trap;
			outpost.GetSafeZoneConfig().unlock_restraining 		= config.unlock_restraining;
			outpost.GetSafeZoneConfig().unlock_bandaging 		= config.unlock_bandaging;
			outpost.GetSafeZoneConfig().unlock_sewing 			= config.unlock_sewing;
			outpost.GetSafeZoneConfig().unlock_collect_blood	= config.unlock_collect_blood;
			outpost.GetSafeZoneConfig().unlock_defibrilate		= config.unlock_defibrilate;
			outpost.GetSafeZoneConfig().unlock_give_blood		= config.unlock_give_blood;
			outpost.GetSafeZoneConfig().unlock_give_saline 		= config.unlock_give_saline;
			outpost.GetSafeZoneConfig().unlock_splint			= config.unlock_splint;
			outpost.GetSafeZoneConfig().unlock_test_blood 		= config.unlock_test_blood;
			outpost.GetSafeZoneConfig().unlock_cover_head 		= config.unlock_cover_head
			outpost.GetSafeZoneConfig().unlock_force_feed	 	= config.unlock_force_feed;
			outpost.GetSafeZoneConfig().unlock_gag 				= config.unlock_gag;
			outpost.GetSafeZoneConfig().unlock_door_locking 	= config.unlock_door_locking;
		}
	}

	protected void SpawnObject( PlayerBase player, ParamsReadContext ctx )
	{

		ref Param2<string, vector> params = new Param2<string, vector>( "", Vector( 0, 0, 0 ) );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading SpawnObject" ), ClassName() );
			return;
		}

		if ( params.param2[ 0 ] == 0 || params.param2[ 2 ] == 0 )
		{
			TajirLogE( string.Format( "Invalid Position %1", params.param2.ToString() ), ClassName() );
			return;
		}
		else if ( !params.param1.Length() )
		{
			TajirLogE( "Invalid Object", ClassName() );
			return;
		}

		vector pos = TajirSpawnHelper.SnapToGround( params.param2[ 0 ],  params.param2[ 2 ] );

		EntityAI object = EntityAI.Cast( GetGame().CreateObject( params.param1, pos, false, TajirSpawnHelper.EntityRequiresAI( params.param1 ), true ) );

		if ( object )
		{

			TajirSpawnHelper.SetUpNewEntity( object );

			TajirLogD( string.Format( "Spawned %1 At %2", object.GetType(), object.GetPosition().ToString() ), ClassName() );
		}
	}
}