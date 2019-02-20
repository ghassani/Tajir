/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirDebugMenuStatEntry
{
	protected PlayerBase 			m_player;
	protected Widget 				m_widget;
	protected TextWidget 			m_statName;
	protected EPlayerStats_v100 	m_stat;

	protected SimpleProgressBarWidget 	m_progress;
	protected TextWidget 				m_value;


	void TajirDebugMenuStatEntry( notnull Widget parent, EPlayerStats_v100 stat )
	{
		m_stat 	 		= stat;
		m_player 		= PlayerBase.Cast( GetGame().GetPlayer() );
		m_widget 		= GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/debug/debug_stat_entry.layout", parent );

		m_statName		= TextWidget.Cast( m_widget.FindAnyWidget( "StatName" ) );
		m_value 		= TextWidget.Cast( m_widget.FindAnyWidget( "ValueText" ) );
		m_progress 		= SimpleProgressBarWidget.Cast( m_widget.FindAnyWidget( "ValueProgress" ) );

		m_statName.SetText( typename.EnumToString( EPlayerStats_v100, m_stat ) );
		m_progress.SetCurrent( 0 );
		m_value.SetText( string.Format( "%1/%2", 0, 100 ) );
	}
	
	void ~TajirDebugMenuStatEntry()
	{
		if ( m_widget ) delete m_widget;
	}

	EPlayerStats_v100 GetStat()
	{
		return m_stat;
	}

	Widget GetWidget()
	{
		return m_widget;
	}

	void UpdateFromState( notnull TajirEntityHealthSaveState state )
	{
		if ( m_stat == EPlayerStats_v100.WET || m_stat == EPlayerStats_v100.BLOODTYPE )
		{
			return;
		}
		else
		{
			if ( !state.HasFloatStat( m_stat ) )
			{
				//TajirLogD( string.Format( "Stat %1 does not exist", m_stat ), ClassName() );
				return;
			}
			else if ( !state.HasFloatStatMax( m_stat ) )
			{
				//TajirLogD( string.Format( "Stat %1 max does not exist", m_stat ), ClassName() );
				return;
			}


			float fmin = 0;
			float fmax = state.GetFloatStatMax( m_stat );
			float fcur = state.GetFloatStat( m_stat );

			if ( fmax <= 0 )
			{
				m_progress.SetCurrent( 0 );
			}
			else
			{
				m_progress.SetCurrent( ( ( fcur / fmax ) * 100 ) );
			}

			m_value.SetText( string.Format( "%1/%2", fcur, fmax ) );
		}
	}
}
