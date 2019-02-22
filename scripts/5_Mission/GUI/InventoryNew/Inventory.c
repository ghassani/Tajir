modded class Inventory
{
	protected Widget 			m_playerCurrency;
	protected TextWidget	 	m_playerCurrencyText;
	protected EditBoxWidget 	m_removeCurrencyAmount;
	protected ButtonWidget 		m_removeFundsButton;
	protected ButtonWidget 		m_canceButton;
	protected ButtonWidget 		m_moveButton;

	protected Widget m_removeFundsWindow;

	void Inventory( LayoutHolder parent )
	{
		m_playerCurrency 		 = GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/currency_hud.layout", GetMainWidget() );
		m_removeFundsWindow  	 = GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/remove_funds_dialog.layout", GetMainWidget() );
		m_playerCurrencyText 	 = TextWidget.Cast( m_playerCurrency.FindAnyWidget( "PlayerCurrencyValue" ) );
		m_removeCurrencyAmount 	 = EditBoxWidget.Cast( m_removeFundsWindow.FindAnyWidget( "Amount" ) );
		m_removeFundsButton 	 = ButtonWidget.Cast( m_playerCurrency.FindAnyWidget( "RemoveFundsButton" ) );
		m_canceButton 	 		 = ButtonWidget.Cast( m_removeFundsWindow.FindAnyWidget( "CancelButton" ) );
		m_moveButton 	 		 = ButtonWidget.Cast( m_removeFundsWindow.FindAnyWidget( "MoveButton" ) );
		
		WidgetEventHandler.GetInstance().RegisterOnClick( m_removeFundsButton,  this, "OnRemoveFundsToggle" );
		WidgetEventHandler.GetInstance().RegisterOnClick( m_canceButton,  this, "OnRemoveFundsToggle" );
		WidgetEventHandler.GetInstance().RegisterOnClick( m_moveButton,  this, "DoRemoveFunds" );

		m_removeCurrencyAmount.SetText( "0" );
		m_removeFundsWindow.Show( false );

		UpdatePlayerCurrency();
	}

	void ~Inventory()
	{
		if ( m_removeFundsButton )	WidgetEventHandler.GetInstance().UnregisterWidget( m_removeFundsButton );
		if ( m_canceButton )		WidgetEventHandler.GetInstance().UnregisterWidget( m_canceButton );
		if ( m_moveButton )			WidgetEventHandler.GetInstance().UnregisterWidget( m_moveButton );
	}

	void OnRemoveFundsToggle()
	{
		m_removeFundsWindow.Show( !m_removeFundsWindow.IsVisible() );
	}

	void DoRemoveFunds()
	{
		if ( !m_removeFundsWindow.IsVisible() )
		{
			return;
		}

		int amount = m_removeCurrencyAmount.GetText().ToInt();

		if ( amount <= 0 )
		{
			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Invalid Value", 1000 );
			return;
		}

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

		if ( player )
		{
			if ( player.GetCurrency() < amount )
			{
				TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Not Enough Funds", 1000 );
				return;
			}

			if ( player.GetHumanInventory().GetEntityInHands() )
			{
				TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Remove Item From Hands", 1000 );
				return;
			}

			if ( GetGame().IsServer() )
			{
				if ( !player.MoveFundsToHands( amount ) )
				{
					TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Error Moving To Hands", 1000 );
					return;
				}
			}
			else
			{
				GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirCurrencyComponentRPC.ClientRPC_MoveFundsToHands, new Param1<int>( amount ), true );
			}
		}
	}

	override void OnShow()
	{
		super.OnShow();

		m_playerCurrency.Show( true );
	}

	override void OnHide()
	{
		m_playerCurrency.Show( false );
		m_removeFundsWindow.Show( false );

		super.OnHide();
	}

	override void UpdateInterval()
	{
		super.UpdateInterval();

		UpdatePlayerCurrency();
	}

	void UpdatePlayerCurrency()
	{
		if ( !TajirCurrencyComponent.GetInstance().IsConfigured() || m_playerCurrencyText == NULL )
		{
			return;
		}

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

		m_playerCurrencyText.SetText( string.Format( "%1%2", TajirCurrencyComponent.GetInstance().GetCurrencySymbol(), player.GetCurrency() ) );
	}
}