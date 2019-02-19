/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirDebugMenuHealthOverview
{

	protected Widget 									m_widget;
	protected TextWidget								m_isAlive;
	protected TextWidget 								m_isDestroyed;
	protected TextWidget 								m_isWet;
	protected TextWidget 								m_bloodType;
	protected TextWidget 								m_objectType;
	protected ScrollWidget								m_scrollarea;
	protected ref array<ref TajirDebugMenuHealthEntry> 	m_hentries;
	protected ref array<ref TajirDebugMenuStatEntry> 	m_sentries;

	void TajirDebugMenuHealthOverview( notnull Widget parent, EntityAI entity )
	{
		m_hentries 			= new array<ref TajirDebugMenuHealthEntry>;
		m_sentries 			= new array<ref TajirDebugMenuStatEntry>;
		
		m_widget 			= GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/debug/debug_health_overview.layout", parent );

		m_isAlive			= TextWidget.Cast( m_widget.FindAnyWidget( "IsAlive" ) );
		m_isDestroyed		= TextWidget.Cast( m_widget.FindAnyWidget( "IsDestroyed" ) );
		m_isWet				= TextWidget.Cast( m_widget.FindAnyWidget( "IsWet" ) );
		m_bloodType			= TextWidget.Cast( m_widget.FindAnyWidget( "BloodType" ) );
		m_objectType 		= TextWidget.Cast( m_widget.FindAnyWidget( "ObjectType" ) );
		m_scrollarea		= ScrollWidget.Cast( m_widget.FindAnyWidget( "DebugHealthEntries" ) );

		if ( entity )
		{
			InitEntity( entity );
		}
	}

	void ~TajirDebugMenuHealthOverview()
	{
		if ( m_widget ) delete m_widget;
	}

	protected void InitEntity( notnull EntityAI entity )
	{
		int i;
		ref array<string> zones = new array<string>;

		entity.GetDamageZones( zones );

		if ( entity.IsInherited( PlayerBase ) )
		{
			PlayerBase player = PlayerBase.Cast( entity );							

			if ( !HasHealthEntry( "GlobalHealth", "Health" ) )
			{
				AddHealthEntry( "GlobalHealth", "Health" );
			}

			if ( !HasHealthEntry( "GlobalHealth", "Blood" ) )
			{
				AddHealthEntry( "GlobalHealth", "Blood" );
			}

			if ( !HasHealthEntry( "GlobalHealth", "Shock" ) )
			{
				AddHealthEntry( "GlobalHealth", "Shock" );
			}


			for ( i = 0; i < zones.Count(); i++ )
			{
				if ( !HasHealthEntry( zones[ i ], "Health" ) )
				{
					AddHealthEntry( zones[ i ], "Health" );
				}


				if ( player.GetMaxHealth( zones[ i ], "Blood" ) > 0 )
				{
					if ( !HasHealthEntry( zones[ i ], "Health" ) )
					{
						AddHealthEntry( zones[ i ], "Blood" );
					}

				}
				if ( player.GetMaxHealth( zones[ i ], "Shock" ) > 0 )
				{
					if ( !HasHealthEntry( zones[ i ], "Health" ) )
					{
						AddHealthEntry( zones[ i ], "Shock" );
					}

				}
			}

			if ( !HasStatEntry( EPlayerStats_v100.HEATCOMFORT ) ) AddStatEntry( EPlayerStats_v100.HEATCOMFORT );
			if ( !HasStatEntry( EPlayerStats_v100.TREMOR ) ) AddStatEntry( EPlayerStats_v100.TREMOR );
			if ( !HasStatEntry( EPlayerStats_v100.ENERGY ) ) AddStatEntry( EPlayerStats_v100.ENERGY );
			if ( !HasStatEntry( EPlayerStats_v100.WATER ) ) AddStatEntry( EPlayerStats_v100.WATER );
			if ( !HasStatEntry( EPlayerStats_v100.STOMACH_SOLID ) ) AddStatEntry( EPlayerStats_v100.STOMACH_SOLID );
			if ( !HasStatEntry( EPlayerStats_v100.STOMACH_ENERGY ) ) AddStatEntry( EPlayerStats_v100.STOMACH_ENERGY );
			if ( !HasStatEntry( EPlayerStats_v100.STOMACH_WATER ) ) AddStatEntry( EPlayerStats_v100.STOMACH_WATER );
			if ( !HasStatEntry( EPlayerStats_v100.DIET ) ) AddStatEntry( EPlayerStats_v100.DIET );
			if ( !HasStatEntry( EPlayerStats_v100.STAMINA ) ) AddStatEntry( EPlayerStats_v100.STAMINA );
			if ( !HasStatEntry( EPlayerStats_v100.SPECIALTY ) ) AddStatEntry( EPlayerStats_v100.SPECIALTY );
		}
		else
		{
			if ( !HasHealthEntry( "GlobalHealth", "Health" ) )
			{
				AddHealthEntry( "GlobalHealth", "Health" );
			}

			for ( i = 0; i < zones.Count(); i++ )
			{
				if ( !HasHealthEntry( zones[ i ], "Health" ) )
				{
					AddHealthEntry( zones[ i ], "Health" );
				}
			}
		}

		ArrangeScrollAreaWidgets();
	}

	Widget GetWidget()
	{
		return m_widget;
	}

	void AddHealthEntry( string zone, string type )
	{
		m_hentries.Insert( new TajirDebugMenuHealthEntry( m_scrollarea, zone, type ) );
	}

	void AddStatEntry( EPlayerStats_v100 stat )
	{
		m_sentries.Insert( new TajirDebugMenuStatEntry( m_scrollarea, stat ) );
	}

	void ArrangeScrollAreaWidgets()
	{
		int i;
		Vector2 pos = new Vector2( 0, 0 );		
		int row 		= 0;
		int col 		= 0;
		int maxcol 		= 3;
		float colwidth 	= 0.3333;
		float rowheight = 0.10;

		for ( i = 0; i < m_hentries.Count(); i++ )
		{
			pos.x = ( col * colwidth );
			pos.y = ( row * rowheight );

			m_hentries[ i ].GetWidget().SetPos( pos.x, pos.y );

			if ( ( col + 1 ) % maxcol == 0 )
			{
				row++;
				col = 0;
			}
			else
			{
				col++;
			}			
		}
		
		for ( i = 0; i < m_sentries.Count(); i++ )
		{
			pos.x = ( col * colwidth );
			pos.y = ( row * rowheight );

			m_sentries[ i ].GetWidget().SetPos( pos.x, pos.y );

			if ( ( col + 1 ) % maxcol == 0 )
			{
				row++;
				col = 0;
			}
			else
			{
				col++;
			}
		}
	}

	void UpdateFromSaveState( notnull TajirEntityHealthSaveState state )
	{
		m_isAlive.SetText( string.Format("Alive: %1", state.IsAlive() ) );
		m_isDestroyed.SetText( string.Format("Damage Destroyed: %1", state.IsDamageDestroyed() ) );
		m_isWet.SetText( string.Format("Wet: %1", state.GetIntStat( EPlayerStats_v100.WET ) ) );
		m_bloodType.SetText( string.Format("Blood Type: %1", state.GetIntStat( EPlayerStats_v100.BLOODTYPE ) ) );

		if ( state.GetEntity() )
		{
			InitEntity( state.GetEntity() );
			
			m_objectType.SetText( state.GetEntity().GetType() );
		}
		else
		{
			m_objectType.SetText( "UNKNOWN TYPE" );
		}

		//ReInitFromState( state );
		
		foreach ( TajirDebugMenuHealthEntry hentry : m_hentries )
		{
			hentry.UpdateFromState( state );
		}

		foreach ( TajirDebugMenuStatEntry sentry : m_sentries )
		{
			sentry.UpdateFromState( state );
		}
	}

	protected void ReInitFromState( notnull TajirEntityHealthSaveState state )
	{
		bool needsUpdate = false;

		ref array<ref TajirEntityDamageZone> stateZones = state.GetZones();

		foreach( TajirEntityDamageZone dz : stateZones )
		{
			if ( !HasHealthEntry( dz.GetZone(), dz.GetType() ) )
			{
				AddHealthEntry( dz.GetZone(), dz.GetType() );
				needsUpdate = true;
			}
		}

		foreach ( TajirDebugMenuHealthEntry hentry : m_hentries )
		{
			if ( !state.HasZone( hentry.GetZone(), hentry.GetType() ) )
			{
				RemoveHealthEntry( hentry.GetZone(), hentry.GetType() );
				needsUpdate = true;
			}
		}

		if ( needsUpdate )
		{
			ArrangeScrollAreaWidgets();
		}
	}

	bool HasHealthEntry( string zone, string type )
	{
		foreach ( TajirDebugMenuHealthEntry entry : m_hentries )
		{
			if ( entry.GetZone() == zone && entry.GetType() == type )
			{
				return true;
			}
		}

		return false;
	}

	bool HasStatEntry( EPlayerStats_v100 stat )
	{
		foreach ( TajirDebugMenuStatEntry entry : m_sentries )
		{
			if ( entry.GetStat() == stat )
			{
				return true;
			}
		}


		return false;
	}

	bool RemoveHealthEntry( string zone, string type )
	{
		for ( int i = 0; i < m_hentries.Count(); i++ )
		{
			if ( m_hentries[ i ].GetZone() == zone && m_hentries[ i ].GetType() == type )
			{
				delete m_hentries[ i ];
				m_hentries.Remove( i );
				return true;
			}
		}

		return false;
	}

	bool RemoveStatEntry( EPlayerStats_v100 stat )
	{
		for ( int i = 0; i < m_sentries.Count(); i++ )
		{
			if ( m_sentries[ i ].GetStat() == stat )
			{
				delete m_sentries[ i ];
				m_sentries.Remove( i );
				return true;
			}
		}

		return false;
	}
}