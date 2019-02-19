/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirDebugMenuOutpostObjectData
{
	ref TajirDebugOutpostPlayerInfo 	player;
	ref TajirDebugMenuOutpostPlayer 	playerOverview;
	ref TajirDebugOutpostTransportInfo  transport;
	ref TajirDebugMenuOutpostTransport  transportOverview;
}

class TajirDebugMenuOutpost
{
	protected Widget m_widget;

	protected ref TajirDebugOutpostInfo m_info;

	protected TextListboxWidget m_objectList;
	protected ref array<ref TajirDebugMenuOutpostObjectData> m_listData;

	protected Widget m_objectView;
	protected Widget m_safeZoneSettings;

	protected ButtonWidget m_teleportButton;
	protected ButtonWidget m_safeZoneButton;
	protected ButtonWidget m_safeZoneUpdateButton;

	protected CheckBoxWidget m_sz_toggle;
	protected CheckBoxWidget m_sz_remove_threats;
	protected CheckBoxWidget m_sz_unlock_melee;
	protected CheckBoxWidget m_sz_unlock_weapon_fire;
	protected CheckBoxWidget m_sz_unlock_trap;
	protected CheckBoxWidget m_sz_unlock_restraining;
	protected CheckBoxWidget m_sz_unlock_bandaging;
	protected CheckBoxWidget m_sz_unlock_sewing;
	protected CheckBoxWidget m_sz_unlock_collect_blood;
	protected CheckBoxWidget m_sz_unlock_defibrilate;
	protected CheckBoxWidget m_sz_unlock_give_blood;
	protected CheckBoxWidget m_sz_unlock_give_saline;
	protected CheckBoxWidget m_sz_unlock_splint;
	protected CheckBoxWidget m_sz_unlock_test_blood;
	protected CheckBoxWidget m_sz_unlock_cover_head;
	protected CheckBoxWidget m_sz_unlock_force_feed;
	protected CheckBoxWidget m_sz_unlock_gag;
	protected CheckBoxWidget m_sz_unlock_door_locking;

	void TajirDebugMenuOutpost( notnull Widget parent, TajirDebugOutpostInfo info )
	{
		m_info						= info;
		m_listData 					= new array<ref TajirDebugMenuOutpostObjectData>;
		m_widget 					= GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/debug/debug_outpost.layout", parent );

		m_objectList				= TextListboxWidget.Cast( m_widget.FindAnyWidget( "ObjectList" ) );
		m_teleportButton			= ButtonWidget.Cast( m_widget.FindAnyWidget( "TeleportToButton" ) );
		m_safeZoneButton			= ButtonWidget.Cast( m_widget.FindAnyWidget( "SafeZoneSettingsButton" ) );
		m_safeZoneUpdateButton		= ButtonWidget.Cast( m_widget.FindAnyWidget( "SafeZoneSettingsUpdateButton" ) );
		m_objectView				= m_widget.FindAnyWidget( "ObjectView" );
		m_safeZoneSettings			= m_widget.FindAnyWidget( "SafeZoneSettings" );
		m_sz_toggle 				= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggle" ) );
		m_sz_remove_threats 		= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleAutoRemoveThreats" ) );
		m_sz_unlock_melee 			= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleMelee" ) );
		m_sz_unlock_weapon_fire 	= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleWeaponFire" ) );
		m_sz_unlock_trap 			= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleTrap" ) );
		m_sz_unlock_restraining 	= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleRestraing" ) );
		m_sz_unlock_bandaging 		= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleBandageTarget" ) );
		m_sz_unlock_sewing 			= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleSewTarget" ) );
		m_sz_unlock_collect_blood 	= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleCollectTargetBlood" ) );
		m_sz_unlock_defibrilate 	= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleDefibrillate" ) );
		m_sz_unlock_give_blood 		= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleGiveTargetBlood" ) );
		m_sz_unlock_give_saline 	= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleGiveTargetSaline" ) );
		m_sz_unlock_splint 			= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleSplitTarget" ) );
		m_sz_unlock_test_blood 		= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleTestTargetBlood" ) );
		m_sz_unlock_cover_head 		= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleCoverTargetHead" ) );
		m_sz_unlock_force_feed 		= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleForceFeedTarget" ) );
		m_sz_unlock_gag 			= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleGagTarget" ) );
		m_sz_unlock_door_locking 	= CheckBoxWidget.Cast( m_widget.FindAnyWidget( "SafeZoneToggleDoorLocking" ) );

		m_safeZoneSettings.Show( false );
	}
	
	void ~TajirDebugMenuOutpost()
	{
		if ( m_widget ) delete m_widget;
	}

	Widget GetWidget()
	{
		return m_widget;
	}

	void UpdateFromInfo( notnull TajirDebugOutpostInfo info )
	{
		m_info = info;

		TajirOutpost outpost = TajirOutpostComponent.GetInstance().GetOutpost( m_info.id );

		if ( outpost )
		{
			outpost.SetSafeZoneConfig( m_info.safezone );
		}

		m_sz_toggle.SetChecked( info.safezone.enabled );
		m_sz_remove_threats.SetChecked( info.safezone.remove_threats );
		m_sz_unlock_melee.SetChecked( info.safezone.unlock_melee );
		m_sz_unlock_weapon_fire.SetChecked( info.safezone.unlock_weapon_fire );
		m_sz_unlock_trap.SetChecked( info.safezone.unlock_trap );
		m_sz_unlock_restraining.SetChecked( info.safezone.unlock_restraining );
		m_sz_unlock_bandaging.SetChecked( info.safezone.unlock_bandaging );
		m_sz_unlock_sewing.SetChecked( info.safezone.unlock_sewing );
		m_sz_unlock_collect_blood.SetChecked( info.safezone.unlock_collect_blood );
		m_sz_unlock_defibrilate.SetChecked( info.safezone.unlock_defibrilate );
		m_sz_unlock_give_blood.SetChecked( info.safezone.unlock_give_blood );
		m_sz_unlock_give_saline.SetChecked( info.safezone.unlock_give_saline );
		m_sz_unlock_splint.SetChecked( info.safezone.unlock_splint );
		m_sz_unlock_test_blood.SetChecked( info.safezone.unlock_test_blood );
		m_sz_unlock_cover_head.SetChecked( info.safezone.unlock_cover_head );
		m_sz_unlock_force_feed.SetChecked( info.safezone.unlock_force_feed );
		m_sz_unlock_gag.SetChecked( info.safezone.unlock_gag );
		m_sz_unlock_door_locking.SetChecked( info.safezone.unlock_door_locking );

		m_objectList.ClearItems();
		m_listData.Clear();

		int row;
		ref TajirDebugMenuOutpostObjectData data;

		foreach ( TajirDebugOutpostPlayerInfo player : m_info.players )
		{
			data = new TajirDebugMenuOutpostObjectData();

			data.player 			= player;
			data.playerOverview 	= new TajirDebugMenuOutpostPlayer( m_objectView, player );
			data.transport 			= NULL;
			data.transportOverview 	= NULL;

			data.playerOverview.GetWidget().Show( false );
			data.playerOverview.UpdateFromInfo( player );

			m_listData.Insert( data );

			row = m_objectList.AddItem( player.id, data, 0 );

			m_objectList.SetItem( row, string.Format("%1 %2", player.network_id_low, player.network_id_high ), NULL, 1 );

			if ( player.merchant )
			{
				m_objectList.SetItem( row, string.Format( "Merchant: %1", player.name ), NULL, 2 );
			}
			else
			{
				m_objectList.SetItem( row, string.Format( "Player: %1", player.name ), NULL, 2 );
			}
			
			m_objectList.SetItem( row, player.position.ToString(), NULL, 3 );
		}

		foreach ( TajirDebugOutpostTransportInfo transport : m_info.transports )
		{
			data = new TajirDebugMenuOutpostObjectData();

			data.player 			= NULL;
			data.playerOverview 	= NULL;
			data.transport 			= transport;
			data.transportOverview 	= new TajirDebugMenuOutpostTransport( m_objectView, transport );

			data.transportOverview.GetWidget().Show( false );
			data.transportOverview.UpdateFromInfo( transport );

			m_listData.Insert( data );

			row = m_objectList.AddItem( "", data, 0 );

			m_objectList.SetItem( row, string.Format("%1 %2", transport.network_id_low, transport.network_id_high ), NULL, 1 );
			m_objectList.SetItem( row, transport.type, NULL, 2 );
			m_objectList.SetItem( row, transport.position.ToString(), NULL, 3 );
		}
	}
	
	bool OnChange( Widget w, int x, int y, bool finished )
	{
		return false;
	}

	bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_teleportButton )
		{
			GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_TeleportOutpost, new Param1<int>( m_info.id ), true );

			return true;
		}
		else if ( w == m_safeZoneButton )
		{
			m_safeZoneSettings.Show( !m_safeZoneSettings.IsVisible() );

			return true;
		}
		else if ( w == m_safeZoneUpdateButton )
		{
			ref TajirOutpostSafeZoneConfig settings = new TajirOutpostSafeZoneConfig();

			settings.enabled				= m_sz_toggle.IsChecked();
			settings.remove_threats 		= m_sz_remove_threats.IsChecked();
			settings.unlock_melee			= m_sz_unlock_melee.IsChecked();
			settings.unlock_weapon_fire		= m_sz_unlock_weapon_fire.IsChecked();
			settings.unlock_trap			= m_sz_unlock_trap.IsChecked();
			settings.unlock_restraining		= m_sz_unlock_restraining.IsChecked();
			settings.unlock_bandaging		= m_sz_unlock_bandaging.IsChecked();
			settings.unlock_sewing			= m_sz_unlock_sewing.IsChecked();
			settings.unlock_collect_blood	= m_sz_unlock_collect_blood.IsChecked();
			settings.unlock_defibrilate		= m_sz_unlock_defibrilate.IsChecked();
			settings.unlock_give_blood		= m_sz_unlock_give_blood.IsChecked();
			settings.unlock_give_saline		= m_sz_unlock_give_saline.IsChecked();
			settings.unlock_splint			= m_sz_unlock_splint.IsChecked();
			settings.unlock_test_blood		= m_sz_unlock_test_blood.IsChecked();
			settings.unlock_cover_head		= m_sz_unlock_cover_head.IsChecked();
			settings.unlock_force_feed		= m_sz_unlock_force_feed.IsChecked();
			settings.unlock_gag				= m_sz_unlock_gag.IsChecked();
			settings.unlock_door_locking	= m_sz_unlock_door_locking.IsChecked();

			GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_UpdateSafeZoneSettings, new Param2<int, ref TajirOutpostSafeZoneConfig >( m_info.id, settings ), true );

			return true;
		}

		return false;
	}

	bool OnItemSelected( Widget w, int x, int y, int row, int  column,	int  oldRow, int  oldColumn )
	{
		if ( w == m_objectList )
		{
			TajirDebugMenuOutpostObjectData data;

			m_objectList.GetItemData( row, 0, data );

			if ( data )
			{
				m_safeZoneSettings.Show( false );

				foreach( TajirDebugMenuOutpostObjectData d : m_listData )
				{
					if ( d.player && d.playerOverview )
					{
						d.playerOverview.GetWidget().Show( data == d );
					}
					else if ( d.transport && d.transportOverview )
					{
						d.transportOverview.GetWidget().Show( data == d );
					}					
				}
			}

			return true;
		}


		return false;
	}
}