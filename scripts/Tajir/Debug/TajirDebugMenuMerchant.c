/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirDebugMenuMerchant
{
	protected Widget m_widget;

	protected ref TajirDebugMerchantInfo m_info;

	protected Widget m_healthOverview;

	protected TextWidget m_merchantId;
	protected TextWidget m_merchantName;
	protected TextWidget m_merchantStoreId;

	protected ButtonWidget m_teleportButton;

	protected ref TajirDebugMenuHealthOverview m_healthOverviewPanel;

	void TajirDebugMenuMerchant( notnull Widget parent, TajirDebugMerchantInfo info )
	{
		m_info					= info;
		m_widget 				= GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/debug/debug_merchant.layout", parent );

		m_teleportButton		= ButtonWidget.Cast( m_widget.FindAnyWidget( "TeleportToButton" ) );
		m_merchantId			= TextWidget.Cast( m_widget.FindAnyWidget( "MerchantID" ) );
		m_merchantName			= TextWidget.Cast( m_widget.FindAnyWidget( "MerchantName" ) );
		m_merchantStoreId		= TextWidget.Cast( m_widget.FindAnyWidget( "MerchantStoreId" ) );
		m_healthOverview		= m_widget.FindAnyWidget( "HealthOverview" );

		m_healthOverviewPanel   = new TajirDebugMenuHealthOverview( m_healthOverview, info.state.GetEntity() );
	}

	void ~TajirDebugMenuMerchant()
	{
		if ( m_widget ) delete m_widget;
	}

	Widget GetWidget()
	{
		return m_widget;
	}

	void UpdateFromInfo( notnull TajirDebugMerchantInfo info )
	{
		m_info = info;

		/*TajirMerchant merchant = TajirMerchantComponent.GetInstance().GetMerchant( info.id );

		if ( merchant )
		{
			m_merchantId.SetText( string.Format( "ID: %1", merchant.GetId() ) );
			m_merchantName.SetText( merchant.GetName() );
			m_merchantStoreId.SetText( string.Format( "Store ID: %1", merchant.GetStore().GetId() ) );
		}
		else
		{
			m_merchantId.SetText( "ID: ?" );
			m_merchantName.SetText( "" );
			m_merchantStoreId.SetText( "Store ID: ?" );
		}*/

		m_healthOverviewPanel.UpdateFromSaveState( info.state );
	}

	bool OnChange( Widget w, int x, int y, bool finished )
	{
		return false;
	}

	bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_teleportButton )
		{
			GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_TeleportMerchant, new Param1<int>( m_info.id ), true );

			return true;
		}

		return false;
	}

	bool OnItemSelected( Widget w, int x, int y, int row, int  column,	int  oldRow, int  oldColumn )
	{
		return false;
	}
}