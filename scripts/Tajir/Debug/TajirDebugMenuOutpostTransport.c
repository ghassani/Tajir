/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirDebugMenuOutpostTransport
{
	protected Widget m_widget;

	protected TextListboxWidget m_infoList;

	protected Widget m_healthOverview;

	protected ref TajirDebugOutpostTransportInfo m_info;

	protected ref TajirDebugMenuHealthOverview m_healthOverviewPanel;

	void TajirDebugMenuOutpostTransport( notnull Widget parent, notnull TajirDebugOutpostTransportInfo info )
	{
		m_info 				= info;
		m_widget 			= GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/debug/debug_outpost_transport.layout", parent );
		m_infoList			= TextListboxWidget.Cast( m_widget.FindAnyWidget( "ObjectInfoList" ) );
		m_healthOverview 	= m_widget.FindAnyWidget( "HealthOverview" );

		m_healthOverviewPanel = new TajirDebugMenuHealthOverview( m_healthOverview, info.state.GetEntity() );
	}
	
	void ~TajirDebugMenuOutpostTransport()
	{
		if ( m_widget ) delete m_widget;
	}

	void UpdateFromInfo( notnull TajirDebugOutpostTransportInfo info )
	{
		m_info = info;

		int row;

		m_infoList.ClearItems();

		row = m_infoList.AddItem( "Network ID", NULL, 0 );
		m_infoList.SetItem( row, string.Format("%1 %2", m_info.network_id_low, m_info.network_id_high ), NULL, 1 );

		row = m_infoList.AddItem( "Type", NULL, 0 );
		m_infoList.SetItem( row, m_info.type, NULL, 1 );

		row = m_infoList.AddItem( "Position", NULL, 0 );
		m_infoList.SetItem( row, m_info.position.ToString(), NULL, 1 );		

		row = m_infoList.AddItem( "Safe Zone", NULL, 0 );
		
		if ( m_info.inSafeZone )
		{
			m_infoList.SetItem( row, "Yes", NULL, 1 );
		}
		else
		{
			m_infoList.SetItem( row, "No", NULL, 1 );
		}

		row = m_infoList.AddItem( "Owner", NULL, 0 );
		m_infoList.SetItem( row, m_info.ownerUUID, NULL, 1 );	

		foreach ( string allowed : m_info.allowedUUIDs )
		{
			row = m_infoList.AddItem( "Allowed UUID", NULL, 0 );
			m_infoList.SetItem( row, allowed, NULL, 1 );
		}

		if ( m_info.state )
		{
			m_healthOverviewPanel.UpdateFromSaveState( m_info.state );
		}
	}

	Widget GetWidget()
	{
		return m_widget;
	}
}