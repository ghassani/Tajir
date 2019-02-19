/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirDebugMenuHealthEntry
{
	protected PlayerBase 	m_player;
	protected Widget 		m_widget;
	protected TextWidget 	m_zoneName;
	protected TextWidget 	m_zoneType;
	protected string 		m_zone;
	protected string 		m_type;

	protected SimpleProgressBarWidget 	m_progress;
	protected TextWidget 				m_value;

	void TajirDebugMenuHealthEntry( notnull Widget parent, string zone, string type )
	{
		m_zone 		= zone;
		m_type 		= type;
		m_player 	= PlayerBase.Cast( GetGame().GetPlayer() );
		m_widget 	= GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/debug/debug_health_entry.layout", parent );
		m_zoneName	= TextWidget.Cast( m_widget.FindAnyWidget( "ZoneText" ) );
		m_zoneType 	= TextWidget.Cast( m_widget.FindAnyWidget( "TypeText" ) );
		m_progress 	= SimpleProgressBarWidget.Cast( m_widget.FindAnyWidget( "ValueProgress" ) );
		m_value 	= TextWidget.Cast( m_widget.FindAnyWidget( "ValueText" ) );

		m_zoneName.SetText( m_zone );
		m_zoneType.SetText( m_type );

		m_progress.SetCurrent( 0 );
		m_value.SetText( string.Format( "%1/%2", 0, 100 ) );
	}
	
	void ~TajirDebugMenuHealthEntry()
	{
		if ( m_widget ) delete m_widget;
	}

	string GetZone()
	{
		return m_zone;
	}

	string GetType()
	{
		return m_type;
	}

	Widget GetWidget()
	{
		return m_widget;
	}

	void UpdateFromState( notnull TajirEntityHealthSaveState state )
	{
		if ( !state.HasZone( m_zone, m_type ) )
		{
			TajirLogE( string.Format( "Zone %1 Type %2 does not exist", m_zone, m_type ), ClassName() );
			return;
		}

		float min = 0;
		float max = m_player.GetMaxHealth( m_zone, m_type );
		float cur = state.GetHealth( m_zone, m_type );

		if ( max <= 0 )
		{
			m_progress.SetCurrent( 0 );
		}
		else
		{
			m_progress.SetCurrent( ( ( cur / max ) * 100 ) );
		}

		m_value.SetText( string.Format( "%1/%2", cur, max ) );
	}
}