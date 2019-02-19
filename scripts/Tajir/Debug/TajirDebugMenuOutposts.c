/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirDebugMenuOutpostData
{
	ref TajirOutpost outpost;
	ref TajirDebugMenuOutpost overview;
}

class TajirDebugMenuOutposts
{
	protected  ref array<ref TajirOutpost> 					m_outposts;
	protected  ref array<ref TajirDebugMenuOutpostData> 	m_listData;

	protected Widget m_widget;
	protected Widget m_outpostView;

	protected TextListboxWidget m_outpostList;

	void TajirDebugMenuOutposts( notnull Widget parent )
	{
		m_widget 		= GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/debug/debug_outposts.layout", parent );
		m_outpostList	= TextListboxWidget.Cast( m_widget.FindAnyWidget( "OutpostList" ) );
		m_outpostView	= m_widget.FindAnyWidget( "OutpostView" );
		m_outposts 		= TajirOutpostComponent.GetInstance().GetOutposts();
		m_listData 		= new array<ref TajirDebugMenuOutpostData>;

		int row;

		foreach ( ref TajirOutpost outpost : m_outposts )
		{
			ref TajirDebugMenuOutpostData data = new TajirDebugMenuOutpostData();

			data.outpost  	= outpost;

			m_listData.Insert( data );

			row = m_outpostList.AddItem( outpost.GetId().ToString(), data, 0 );

			m_outpostList.SetItem( row, outpost.GetName(), NULL, 1 );
			m_outpostList.SetItem( row, outpost.GetConfig().position.ToString(), NULL, 2 );
			m_outpostList.SetItem( row, outpost.GetConfig().radius.ToString(), NULL, 3 );
			m_outpostList.SetItem( row, outpost.IsSafeZone().ToString(), NULL, 4 );
		}
	}
	
	void ~TajirDebugMenuOutposts()
	{
		if ( m_widget ) delete m_widget;
	}

	Widget GetWidget()
	{
		return m_widget;
	}

	bool OnClick( Widget w, int x, int y, int button )
	{
		foreach( TajirDebugMenuOutpostData d : m_listData )
		{
			if ( d.overview )
			{
				if ( d.overview.OnClick( w, x, y, button ) )
				{
					return true;
				}
			}
		}

		return false;
	}

	bool OnChange( Widget w, int x, int y, bool finished )
	{
		foreach( TajirDebugMenuOutpostData d : m_listData )
		{
			if ( d.overview )
			{
				if ( d.overview.OnChange( w, x, y, finished ) )
				{
					return true;
				}
			}
		}

		return false;
	}

	bool OnItemSelected( Widget w, int x, int y, int row, int  column,	int  oldRow, int  oldColumn )
	{
		if ( w == m_outpostList )
		{
			TajirDebugMenuOutpostData data;

			m_outpostList.GetItemData( row, 0, data );

			if ( data )
			{
				foreach( TajirDebugMenuOutpostData d : m_listData )
				{
					if ( d.overview )
					{
						if ( data == d )
						{
							d.overview.GetWidget().Show( true );
						}
						else
						{
							d.overview.GetWidget().Show( false );
						}
					}
				}
			}

			return true;
		}

		foreach( TajirDebugMenuOutpostData da : m_listData )
		{
			if ( da.overview )
			{
				if ( da.overview.OnItemSelected( w, x, y, row, column, oldRow, oldColumn ) )
				{
					return true;
				}
			}
		}

		return false;
	}

	bool OnClientRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
			case TajirDebugComponentRPC.ServerRPC_OutpostInfoResponse:
				ReadOutpostInfoResponse( ctx );
				return true;
		}

		return false;
	}

	void ReadOutpostInfoResponse( ParamsReadContext ctx )
	{
		ref array<ref TajirDebugOutpostInfo> infos = new  array<ref TajirDebugOutpostInfo>;

		ref Param1<ref array<ref TajirDebugOutpostInfo>> params = new Param1<ref array<ref TajirDebugOutpostInfo>>( infos );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading Update Outpost Info Response" ), ClassName() );
			return;
		}

		foreach ( ref TajirDebugOutpostInfo info : infos )
		{
			foreach ( ref TajirDebugMenuOutpostData data : m_listData )
			{
				if ( data.outpost.GetId() == info.id )
				{
					if ( !data.overview )
					{
						data.overview = new TajirDebugMenuOutpost( m_outpostView, info );
						data.overview.GetWidget().Show( false );
					}
					
					data.overview.UpdateFromInfo( info );
				}
			}
		}
	}
}