/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class MissionServer
{
	protected ref TajirManager m_tajirManager;

	void MissionServer()
	{		
		m_tajirManager = new TajirManager();

		m_tajirManager.RegisterComponent( new TajirCurrencyComponent( m_tajirManager ) );
		m_tajirManager.RegisterComponent( new TajirItemComponent( m_tajirManager ) );
		m_tajirManager.RegisterComponent( new TajirStoreComponent( m_tajirManager ) );
		m_tajirManager.RegisterComponent( new TajirMerchantComponent( m_tajirManager ) );
		m_tajirManager.RegisterComponent( new TajirOutpostComponent( m_tajirManager ) );
#ifdef TAJIR_DEBUG_MENU
		m_tajirManager.RegisterComponent( new TajirDebugComponent( m_tajirManager ) );
#endif
	}
	
	void ~MissionServer()
	{
		if ( m_tajirManager )
		{
			delete m_tajirManager;
		}
	}

	override void OnInit()
	{		
		super.OnInit();
		
		m_tajirManager.Init();
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		m_tajirManager.OnMissionStart();
	}

	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		m_tajirManager.OnMissionFinish();
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		m_tajirManager.OnClientConnected( player, identity );
	}

	override void OnClientReadyEvent(PlayerIdentity identity, PlayerBase player)
	{
		super.OnClientReadyEvent(identity, player);

		m_tajirManager.OnClientReady( identity, player );
	}

	override void OnClientRespawnEvent(PlayerIdentity identity, PlayerBase player)
	{
		super.OnClientRespawnEvent(identity, player);
		
		m_tajirManager.OnClientRespawn( identity, player );
	}

	override void OnClientReconnectEvent(PlayerIdentity identity, PlayerBase player)
	{
		super.OnClientReconnectEvent(identity, player);
		
		m_tajirManager.OnClientReconnect( identity, player );
	}

	override void OnClientDisconnectedEvent(PlayerIdentity identity, PlayerBase player, int logoutTime, bool authFailed)
	{
		super.OnClientDisconnectedEvent(identity, player, logoutTime, authFailed);

		m_tajirManager.OnClientDisconnected( identity, player, logoutTime, authFailed );
	}

#ifdef TAJIR_DEBUG_START_SAFEZONE

	override PlayerBase CreateCharacter( PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName )
	{	
		PlayerBase player = super.CreateCharacter( identity, pos, ctx, characterName );

		if ( TajirOutpostComponent.GetInstance() )
		{		
			array<ref TajirOutpost> outposts = TajirOutpostComponent.GetInstance().GetOutposts();

			for ( int i = 0; i < outposts.Count(); i++ )
			{
				if ( outposts[ i ].IsSafeZone() )
				{
					player.SetPosition( outposts[ i ].GetCenterPosition() );
					break;
				}
			}
			
		}

		return player;
	}

#endif

#ifdef TAJIR_WITH_PLAYER_START_CURRENCY	
	override void StartingEquipSetup( PlayerBase player, bool clothesChosen )
	{
		super.StartingEquipSetup( player, clothesChosen );

		player.SetCurrency( TAJIR_DEFAULT_PLAYER_START_CURRENCY );

		if ( TajirCurrencyComponent.GetInstance() )
		{
			int startconfig = TajirCurrencyComponent.GetInstance().GetConfig().player_start_currency;

			if ( startconfig > 0 )
			{
				player.SetCurrency( startconfig );
			}
		}
	}
#endif
}
