/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreMenu extends TajirStoreMenuBase
{
	protected TextWidget 					m_merchantName;

	protected TextWidget 					m_playerCurrency;

	protected PlayerPreviewWidget 			m_merchantPreview;
	
	protected Widget 						m_catalogRootWidget;

	protected Widget 						m_catalogWidget;
	
	/**
	 * @brief      Gets the player preview.
	 *
	 * @return     The player preview.
	 */
	PlayerPreviewWidget GetPlayerPreview()
	{
		return m_merchantPreview;
	}

	/**
	 * @brief      Initialize the root of the menu
	 *
	 * @return     Widget
	 */
	override Widget Init()
	{
		layoutRoot = m_store.GetTypeHandler().CreateCatalogRootWidget();
		
		if ( !layoutRoot )
		{
			TajirLogE( string.Format( "Store ID %1 Type Handler Did Not Create Root Widget", m_store.GetId() ), ClassName() );
			return NULL;
		}

		m_merchantName   	= TextWidget.Cast( layoutRoot.FindAnyWidget( "MerchantName" ) );
		m_merchantPreview  	= PlayerPreviewWidget.Cast( layoutRoot.FindAnyWidget( "MerchantPreview" ) );
		m_catalogRootWidget = layoutRoot.FindAnyWidget( "MerchantShopInventoryContainer" );
		m_playerCurrency    = TextWidget.Cast( layoutRoot.FindAnyWidget( "CurrencyText" ) );

		m_merchantName.SetText( "" );
		m_playerCurrency.SetText( "" );

		string merchName;

		if ( m_merchant != NULL && m_merchant.GetObject().IsInherited( ManBase ) )
		{
			float w;
			float h;

			m_merchantPreview.GetSize( w, h );

			m_merchantPreview.SetPlayer( ManBase.Cast(  m_merchant.GetObject() ) );
			m_merchantPreview.SetModelPosition( Vector( -0.5, -0.5, 3 ) );
			m_merchantPreview.SetModelOrientation( Vector( 0, 0, 0 ) );

			merchName =  m_merchant.GetName();
		}

		if ( m_store.GetName() != "" )
		{
			if ( merchName.Length() )
			{
				m_merchantName.SetText( merchName + " - " + m_store.GetName() );
			}
			else
			{
				m_merchantName.SetText( merchName );
			}
		}

		m_catalogWidget = m_store.GetTypeHandler().CreateCatalogWidget( m_catalogRootWidget );

		m_catalogWidget.GetScript( m_catalogMenuScript );
		
		if ( m_catalogMenuScript == NULL )
		{
			TajirLogE( string.Format( "Store ID %1 Type Handler Widget Has No Script Assigned", m_store.GetId() ), ClassName() );
			return NULL;
		}

		UpdatePlayerCurrency();

		return layoutRoot;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  timeslice  The timeslice
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void Update(float timeslice)
	{
		if ( m_catalogMenuScript )
		{
			m_catalogMenuScript.OnUpdate(timeslice );
		}
	}

	/**
	 * @brief      GUI Input event handler
	 */
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( m_catalogMenuScript )
		{
			return m_catalogMenuScript.OnClick( w, x, y, button );
		}

		return false;
	}

	/**
	 * @brief      GUI event handler
	 */
	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		if ( m_catalogMenuScript )
		{
			return m_catalogMenuScript.OnChange( w, x, y, finished );
		}

		return false;
	}

	/**
	 * @brief      GUI event handler
	 */
	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		if ( m_catalogMenuScript )
		{
			return m_catalogMenuScript.OnItemSelected( w, x, y, row, column, oldRow, oldColumn );
		}

		return false;
	}

	/**
	 * @brief      Update the players currency display
	 */
	override void UpdatePlayerCurrency()
	{
		TajirCurrencyComponent currency = TajirCurrencyComponent.GetInstance();
		
		string currencyName = currency.GetCurrencyNamePlural();

		int playerCurrency = PlayerBase.Cast( GetGame().GetPlayer() ).GetCurrency();

		if ( playerCurrency == 1 )
		{
			currencyName = currency.GetCurrencyNameSingular();
		}
		
		m_playerCurrency.SetText( string.Format( "%1: %2%3", currencyName, currency.GetCurrencySymbol(), playerCurrency ) );
	}

	/**
	 * @brief      Called when an inventory update RPC is received while the menu is open.
	 *
	 * @param[in]  ref array<TajirStore>  Array of stores that received updates.
	 */
	override void OnInventoryUpdateReceived( ref array<TajirStore> updatedStores )
	{
		if ( updatedStores.Find( m_store ) && m_catalogMenuScript )
		{
			m_catalogMenuScript.OnInventoryUpdateReceived();
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  response  The response
	 */
	override void OnTransactionResponseReceived( TajirStoreTransactionResponse response )
	{
		UpdatePlayerCurrency();

		if ( m_catalogMenuScript )
		{
			m_catalogMenuScript.OnTransactionResponseReceived( response );
		}
	}
}