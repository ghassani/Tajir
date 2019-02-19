/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      Handles currency related functionality.
 */
class TajirCurrencyComponent extends TajirComponentBase
{
	protected ref TajirCurrencyConfig m_config;

	/**
	 * @brief      Constructor
	 */
	void TajirCurrencyComponent( notnull TajirManager manager )
	{
		m_name 	 			= "TajirCurrency";
		m_config 			= new TajirCurrencyConfig();
	}

	/**
	 * @brief      Destructor.
	 */
	void ~TajirCurrencyComponent()
	{

	}

	/**
	 * @brief     Gets the global instance.
	 *
	 * @return    TajirCurrencyComponent
	 * @static
	 */
	static TajirCurrencyComponent GetInstance()
	{
		if ( TajirManager.GetInstance() )
		{
			if ( TajirManager.GetInstance().HasComponent( "TajirCurrency" ) )
			{
				return TajirCurrencyComponent.Cast( TajirManager.GetInstance().GetComponent( "TajirCurrency" ) );
			}
			
			Error( "No Instance of TajirCurrency" );
		}
		else
		{
			Error( "No Instance of TajirManager" );
		}

		return NULL;		
	}

	/**
	 * @brief      Gets the configuration.
	 *
	 * @return     The configuration.
	 */
	TajirCurrencyConfig GetConfig()
	{
		return m_config;
	}

	/**
	 * @brief      Loads the component server side configuration.
	 *
	 * @return     void
	 */
	override void LoadServerConfiguration()
	{	
		string objectName;
		int currencyValue;

		if ( !FileExist( TAJIR_CURRENCY_CONFIG_USER ) )
		{
			if ( !CopyFile( TAJIR_CURRENCY_CONFIG_DEFAULT, TAJIR_CURRENCY_CONFIG_USER ) )
			{				
				TajirLogE( string.Format( "Error Copying Default Currency Configuration from %1 to %2", TAJIR_CURRENCY_CONFIG_DEFAULT, TAJIR_CURRENCY_CONFIG_USER ), ClassName() );
			}
		}

		JsonFileLoader<ref TajirCurrencyConfig>.JsonLoadFile( TAJIR_CURRENCY_CONFIG_USER, m_config );

		if ( !GetGame().IsKindOf( m_config.base_object, "CurrencyBase" ) )
		{
			TajirLogE( string.Format( "base_currency object type %1 not of CurrencyBase", m_config.base_object  ), ClassName() );
			return;
		}

		if ( !GetGame().IsKindOf( m_config.dynamic_object, "DynamicCurrencyBase" ) )
		{
			TajirLogE( string.Format( "dynamic_object object type %1 not of DynamicCurrencyBase", m_config.dynamic_object  ), ClassName() );
			return;
		}

		for ( int i = 0; i < m_config.objects.Count(); i++ )
		{	
			if ( !GetGame().IsKindOf( m_config.objects[ i ], "CurrencyBase" ) )
			{
				TajirLogE( string.Format( "Currency object entry %1 type %2 not of CurrencyBase", i, m_config.objects[ i ]  ), ClassName() );

				continue;
			}
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
		SetConfigured( false );
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
		if ( m_manager.IsServer() )
		{
#ifdef TAJIR_DEBUG
			TajirLogD( string.Format( "Sending Player ID %1 Server Currency Configuration", identity.GetId() ), ClassName() );
#endif
			player.RPCSingleParam( TajirCurrencyComponentRPC.ServerRPC_CurrencyConfiguration, new Param1<ref TajirCurrencyConfig>( m_config ), true, identity );
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
	override void OnClientRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
			case TajirCurrencyComponentRPC.ServerRPC_CurrencyConfiguration:
				ReadConfigurationFromServer( ctx );
				break;
			case TajirCurrencyComponentRPC.ServerRPC_SearchPlayerCurrencyResult:
				ReadSearchPlayerCurrencyResult( ctx );
				break;				
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
		switch ( rpc_type )
		{
			case TajirCurrencyComponentRPC.ClientRPC_MoveFundsToHands:
				ReadMoveFundsToHands( sender, target, ctx );
				break;
		}
	}
	
	void ReadMoveFundsToHands(  PlayerIdentity sender, Object target, ParamsReadContext ctx )
	{
		if ( target.IsMan() )
		{
			PlayerBase player = PlayerBase.Cast( target );

			if ( !player.GetIdentity() || player.GetIdentity().GetId() != sender.GetId() )
			{
				TajirLogE( string.Format( "Invalid Identity %1 != %2", player.GetIdentity().GetId(), sender.GetId() ), ClassName() );
				return;
			}

			ref Param1<int> params = new Param1<int>( 0 );

			if ( !ctx.Read( params ) )
			{
				TajirLogE( "Error reading move funds request from client", ClassName() );
				return;
			}

			if ( params.param1 <= 0 )
			{
				TajirLogE( "Invalid Amount", ClassName() );
				return;
			}
			else if ( params.param1 > player.GetCurrency() )
			{
				TajirLogE( "Not Enough Funds", ClassName() );
				return;
			} 
			else if ( player.GetHumanInventory().GetEntityInHands() )
			{
				TajirLogE( "Item In Hands", ClassName() );
				return;
			}

			DynamicCurrencyBase currency = CreateDynamicCurrency( player, params.param1 );

			if ( !currency )
			{
				TajirLogE( "Error Creating Currency", ClassName() );
				return;
			}

			if ( !player.GetHumanInventory().TakeEntityToInventory( InventoryMode.SERVER, FindInventoryLocationType.HANDS, EntityAI.Cast( currency ) ) )
			{
				TajirLogE( string.Format( "Could Not Take %1 To Hands", currency.GetType() ) );

				currency.Delete();

				return;
			}

			player.DeductCurrency( params.param1 );

			currency.SetSynchDirty();
		}
	}

	/**
	 * @brief      Reads a configuration from server.
	 *
	 * @param[in]  ctx   The context
	 */
	void ReadConfigurationFromServer( ParamsReadContext ctx )
	{
		ref Param1<ref TajirCurrencyConfig> config = new Param1<ref TajirCurrencyConfig>( m_config );

		if ( !ctx.Read( config ) )
		{
			TajirLogE( "Error reading currency config from server", ClassName() );

			SetConfigured( false );

			return;
		}

		SetConfigured( true );
		ApplyConfiguration();
	}

	/**
	 * @brief      Reads a search player currency result.
	 *
	 * @param[in]  ctx   The context
	 */
	void ReadSearchPlayerCurrencyResult( ParamsReadContext ctx )
	{
		ref Param1<int> params = new Param1<int>( 0 );

		if ( !ctx.Read( params ) )
		{
			TajirLogE( "Error reading result from server", ClassName() );
			return;
		}

		if ( params.param1 <= 0 )
		{
			m_manager.ShowNotification( TajirNotificationType.Info, "Found Nothing", 2000 );
		}
		else
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			
			m_manager.ShowNotification( TajirNotificationType.Success, string.Format( "Found %1%2", GetCurrencySymbol(), params.param1 ), 2000 );
		}
	}

	/**
	 * @brief      Determines if valid currency object.
	 *
	 * @param[in]  item  The item
	 *
	 * @return     True if valid currency object, False otherwise.
	 */
	bool IsValidCurrencyObject( notnull EntityAI item )
	{
		return IsValidCurrencyObject( item.GetType() );
	}

	/**
	 * @brief      Determines if valid currency object.
	 *
	 * @param[in]  type  The item type string
	 *
	 * @return     True if valid currency object, False otherwise.
	 */
	bool IsValidCurrencyObject( string item )
	{
		return GetGame().IsKindOf( item, "CurrencyBase" );
	}

	/**
	 * @brief      Gets the currency object value.
	 *
	 * @param[in]  objectName  The object name
	 *
	 * @return     The currency object value.
	 */
	int GetCurrencyObjectValue( string objectName )
	{
		return GetGame().ConfigGetInt( "CfgVehicles " + objectName + " currencyValue" );
	}

	/**
	 * @brief      Gets the currency name singular.
	 *
	 * @return     The currency name singular.
	 */
	string GetCurrencyNameSingular()
	{
		string ret;

		if ( !GetGame().ConfigGetText( "CfgVehicles " + m_config.base_object + " currencyNameSingular", ret ) )
		{
			TajirLogW( string.Format( "Invalid Currency Base Configuration `CfgVehicles %1 currencyNameSingular`", m_config.base_object ), ClassName() );
		}

		return ret;
	}

	/**
	 * @brief      Gets the currency name plural.
	 *
	 * @return     The currency name plural.
	 */
	string GetCurrencyNamePlural()
	{
		string ret;

		if ( !GetGame().ConfigGetText( "CfgVehicles " + m_config.base_object + " currencyNamePlural", ret ) )
		{
			TajirLogW( string.Format( "Invalid Currency Base Configuration `CfgVehicles %1 currencyNamePlural`", m_config.base_object ), ClassName() );
		}

		return ret;
	}

	/**
	 * @brief      Gets the currency symbol.
	 *
	 * @return     The currency symbol.
	 */
	string GetCurrencySymbol()
	{
		string ret;

		if ( !GetGame().ConfigGetText( "CfgVehicles " + m_config.base_object + " currencySymbol", ret ) )
		{
			TajirLogW( string.Format( "Invalid Currency Base Configuration `CfgVehicles %1 currencySymbol`", m_config.base_object ), ClassName() );
		}

		return ret;
	}

	DynamicCurrencyBase CreateDynamicCurrency( notnull PlayerBase player, int amount )
	{
		EntityAI object = EntityAI.Cast( GetGame().CreateObject( m_config.dynamic_object, player.GetPosition(), false, TajirSpawnHelper.EntityRequiresAI( m_config.dynamic_object ), true ) );

		if ( object )
		{
			if ( object.IsInherited( DynamicCurrencyBase ) )
			{
				DynamicCurrencyBase currency = DynamicCurrencyBase.Cast( object );

				currency.SetCurrencyValue( amount );

				return currency;
			}
		}

		return NULL;
	}
}