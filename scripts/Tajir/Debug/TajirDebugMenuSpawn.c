/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

enum TajirDebugMenuSpawnAction
{
	SpawnManWithCurrency = 0,
	SpawnManWithCurrencyAndItems,
	GivePlayer5k,
	GiveAllPlayers5k,
	DepletePlayerWater,
	ReplenishPlayerWater,
	DepletePlayerEnergy,
	ReplenishPlayerEnergy,
	DepletePlayerBlood,
	ReplenishPlayerBlood
}

class TajirDebugMenuSpawn
{
	protected Widget m_widget;

	protected TextListboxWidget m_objectList;
	protected TextListboxWidget m_actionList;

	protected ButtonWidget m_spawnObjectPlayer;
	protected ButtonWidget m_spawnObjecPos;
	protected ButtonWidget m_runAction;
	protected ButtonWidget m_currentPosition;

	protected EditBoxWidget m_spawnPosX;
	protected EditBoxWidget m_spawnPosY;
	protected EditBoxWidget m_spawnPosZ;
	protected EditBoxWidget m_objectSearch;

	void TajirDebugMenuSpawn( notnull Widget parent )
	{
		m_widget = GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/debug/debug_spawn.layout", parent );
		
		m_objectList			= TextListboxWidget.Cast( m_widget.FindAnyWidget( "GameObjectList" ) );
		m_actionList			= TextListboxWidget.Cast( m_widget.FindAnyWidget( "ActionList" ) );
		m_spawnObjectPlayer		= ButtonWidget.Cast( m_widget.FindAnyWidget( "SpawnSelectedAtPlayer" ) );
		m_spawnObjecPos			= ButtonWidget.Cast( m_widget.FindAnyWidget( "SpawnSelectedAtPosition" ) );
		m_currentPosition		= ButtonWidget.Cast( m_widget.FindAnyWidget( "GetCurrenPos" ) );
		m_runAction				= ButtonWidget.Cast( m_widget.FindAnyWidget( "RunSelectedActionButton" ) );
		m_spawnPosX				= EditBoxWidget.Cast( m_widget.FindAnyWidget( "SpawnPosX" ) );
		m_spawnPosY				= EditBoxWidget.Cast( m_widget.FindAnyWidget( "SpawnPosY" ) );
		m_spawnPosZ				= EditBoxWidget.Cast( m_widget.FindAnyWidget( "SpawnPosZ" ) );
		m_objectSearch			= EditBoxWidget.Cast( m_widget.FindAnyWidget( "ObjectSearchText" ) );

		PopulateObjectList();
		PopulateActionList();
	}
	
	void ~TajirDebugMenuSpawn()
	{
		if ( m_widget ) delete m_widget;
	}

	Widget GetWidget()
	{
		return m_widget;
	}

	void PopulateObjectList()
	{
		int scope;
		int classCount;
		int row;
		string name;
		string nameLower;
		ref array<string> configs = new array<string>;
		
		configs.Insert( CFG_VEHICLESPATH );
		configs.Insert( CFG_WEAPONSPATH );
		configs.Insert( CFG_MAGAZINESPATH );

		m_objectList.ClearItems();

		string searchText = m_objectSearch.GetText();

		searchText.ToLower();

		foreach( string config : configs )
		{
			classCount = GetGame().ConfigGetChildrenCount( config );

			for ( int i = 0; i < classCount; ++i )
			{
				if ( GetGame().ConfigGetChildName( config, i, name ) )
				{
					scope = GetGame().ConfigGetInt( config + " " + name + " scope" );

					if ( scope == 0 || scope == 1 )
					{
						continue;
					}

					if ( searchText.Length() )
					{
						nameLower = name;
						nameLower.ToLower();

						if ( !nameLower.Contains( searchText ) )
						{
							continue;
						}
					}

					row = m_objectList.AddItem( name, NULL, 0 );
					m_objectList.SetItem( row, config, NULL, 1 );
				}
			}
		}
	}

	void PopulateActionList()
	{
		m_actionList.AddItem( "Spawn Man w/ Currency", NULL, 0 );
		m_actionList.AddItem( "Spawn Man w/ Currency & Items", NULL, 0 );
		m_actionList.AddItem( "Give Player 5K", NULL, 0 );
		m_actionList.AddItem( "Give All Players 5K", NULL, 0 );	
		m_actionList.AddItem( "Deplete Player Water Level", NULL, 0 );	
		m_actionList.AddItem( "Replenish Player Water Level", NULL, 0 );
		m_actionList.AddItem( "Deplete Player Energy Level", NULL, 0 );	
		m_actionList.AddItem( "Replenish Player Energy Level", NULL, 0 );
		m_actionList.AddItem( "Deplete Player Blood Level", NULL, 0 );	
		m_actionList.AddItem( "Replenish Player Blood Level", NULL, 0 );
	}

	bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_runAction )
		{
			if ( m_actionList.GetSelectedRow() > -1 )
			{
				switch( m_actionList.GetSelectedRow() )
				{
					case TajirDebugMenuSpawnAction.SpawnManWithCurrency:
						GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_SpawnManWithCurrency, new Param1<vector>( PlayerBase.Cast( GetGame().GetPlayer() ).GetPosition() ), true );
						break;
					case TajirDebugMenuSpawnAction.SpawnManWithCurrencyAndItems:
						GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_SpawnManWithCurrencyAndItems, new Param1<vector>( PlayerBase.Cast( GetGame().GetPlayer() ).GetPosition() ), true );
						break;
					case TajirDebugMenuSpawnAction.GivePlayer5k:
						GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_GiveCurrency, new Param1<vector>( PlayerBase.Cast( GetGame().GetPlayer() ).GetPosition() ), true );
						break;
					case TajirDebugMenuSpawnAction.GiveAllPlayers5k:
						GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_GiveAllCurrency, new Param1<vector>( PlayerBase.Cast( GetGame().GetPlayer() ).GetPosition() ), true );
						break;
					case TajirDebugMenuSpawnAction.DepletePlayerWater:
						GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_UpdatePlayerStat, new Param2<int, int>( 1, 0 ), true );
						break;
					case TajirDebugMenuSpawnAction.ReplenishPlayerWater:
						GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_UpdatePlayerStat, new Param2<int, int>( 1, 1 ), true );
						break;
					case TajirDebugMenuSpawnAction.DepletePlayerEnergy:
						GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_UpdatePlayerStat, new Param2<int, int>( 2, 0 ), true );
						break;
					case TajirDebugMenuSpawnAction.ReplenishPlayerEnergy:
						GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_UpdatePlayerStat, new Param2<int, int>( 2, 1 ), true );
						break;
					case TajirDebugMenuSpawnAction.DepletePlayerBlood:
						GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_UpdatePlayerStat, new Param2<int, int>( 3, 0 ), true );
						break;
					case TajirDebugMenuSpawnAction.ReplenishPlayerBlood:
						GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_UpdatePlayerStat, new Param2<int, int>( 3, 1 ), true );
						break;	
				}
			}

			return true;
		}
		else if ( w == m_spawnObjectPlayer || w == m_spawnObjecPos )
		{
			if ( m_objectList.GetSelectedRow() == -1 )
			{
				TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Select an Object", 1000 );
				return true;
			}
			
			string object = "";
			vector pos = PlayerBase.Cast( GetGame().GetPlayer() ).GetPosition();

			m_objectList.GetItemText( m_objectList.GetSelectedRow(), 0, object );

			if ( !object.Length() )
			{
				TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Invalid Selection", 1000 );
				return true;
			}

			if ( w == m_spawnObjecPos )
			{
				pos[ 0 ] = m_spawnPosX.GetText().ToFloat();
				pos[ 1 ] = m_spawnPosY.GetText().ToFloat();
				pos[ 2 ] = m_spawnPosZ.GetText().ToFloat();

				if ( pos[ 0 ] == 0 || pos[ 2 ] == 0 )
				{
					TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Invalid Position", 1000 );
					return true;
				}				
			}

			TajirLogD( string.Format( "SPAWN AT: %1", pos.ToString() ) );

			GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirDebugComponentRPC.ClientRPC_SpawnObjectAtPosition, new Param2<string, vector>( object, pos ), true );

			return true;
		}
		else if ( w == m_currentPosition )
		{
			vector playerPos = PlayerBase.Cast( GetGame().GetPlayer() ).GetPosition();

			m_spawnPosX.SetText( playerPos[ 0 ].ToString() );
			m_spawnPosY.SetText( playerPos[ 1 ].ToString() );
			m_spawnPosZ.SetText( playerPos[ 2 ].ToString() );

			return true;
		}
 
		return false;
	}
	
	bool OnChange( Widget w, int x, int y, bool finished )
	{
		if ( w == m_objectSearch && finished )
		{
			PopulateObjectList();

			return true;
		}

		return false;
	}
	
	bool OnItemSelected( Widget w, int x, int y, int row, int  column,	int  oldRow, int  oldColumn )
	{
		return false;
	}
}