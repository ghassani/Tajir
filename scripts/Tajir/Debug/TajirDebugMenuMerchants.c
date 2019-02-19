/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirDebugMenuMerchantData
{
	ref TajirMerchant merchant;
	ref TajirDebugMenuMerchant overview;
}

class TajirDebugMenuMerchants
{
	protected  ref array<ref TajirMerchant> 				m_merchants;
	protected  ref array<ref TajirDebugMenuMerchantData> 	m_listData;

	protected Widget m_widget;
	protected Widget m_infoView;

	protected TextListboxWidget m_merchantList;

	void TajirDebugMenuMerchants( notnull Widget parent )
	{
		m_widget 		= GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/debug/debug_merchants.layout", parent );
		m_merchantList	= TextListboxWidget.Cast( m_widget.FindAnyWidget( "MerchantList" ) );
		m_infoView		= m_widget.FindAnyWidget( "MerchantInfoView" );
		m_merchants 	= TajirMerchantComponent.GetInstance().GetMerchants();
		m_listData 		= new array<ref TajirDebugMenuMerchantData>;

		int row;

		foreach ( ref TajirMerchant merchant : m_merchants )
		{
			ref TajirDebugMenuMerchantData data = new TajirDebugMenuMerchantData();

			data.merchant  	= merchant;

			m_listData.Insert( data );

			row = m_merchantList.AddItem( merchant.GetId().ToString(), data, 0 );

			m_merchantList.SetItem( row, merchant.GetName(), NULL, 1 );
			m_merchantList.SetItem( row, merchant.GetConfig().position.ToString(), NULL, 2 );
			m_merchantList.SetItem( row, merchant.GetConfig().orientation.ToString(), NULL, 3 );
		}
	}
	
	void ~TajirDebugMenuMerchants()
	{
		if ( m_widget ) delete m_widget;
	}

	Widget GetWidget()
	{
		return m_widget;
	}

	bool OnClick( Widget w, int x, int y, int button )
	{
		foreach( TajirDebugMenuMerchantData d : m_listData )
		{
			if ( d.overview.OnClick( w, x, y, button ) )
			{
				return true;
			}
		}
		return false;
	}

	bool OnItemSelected( Widget w, int x, int y, int row, int  column,	int  oldRow, int  oldColumn )
	{
		if ( w == m_merchantList )
		{
			TajirDebugMenuMerchantData data;

			m_merchantList.GetItemData( row, 0, data );

			if ( data )
			{
				foreach( TajirDebugMenuMerchantData d : m_listData )
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
		}

		return false;
	}

	bool OnChange( Widget w, int x, int y, bool finished )
	{
		return false;
	}
	
	bool OnClientRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
			case TajirDebugComponentRPC.ServerRPC_MerchantInfoResponse:
				ReadMerchantInfoResponse( ctx );
				return true;
		}

		return false;
	}

	void ReadMerchantInfoResponse( ParamsReadContext ctx )
	{
		ref array<ref TajirDebugMerchantInfo> infos = new  array<ref TajirDebugMerchantInfo>;

		ref Param1<ref array<ref TajirDebugMerchantInfo>> params = new Param1<ref array<ref TajirDebugMerchantInfo>>( infos );

		if ( !ctx.Read( params ) ) 
		{
			TajirLogE( string.Format( "Error Reading Update Merchant Info Response" ), ClassName() );
			return;
		}

		foreach ( TajirDebugMerchantInfo info : infos )
		{
			foreach ( TajirDebugMenuMerchantData data : m_listData )
			{
				if ( data.merchant.GetId() == info.id )
				{
					if ( !data.overview )
					{
						data.overview = new TajirDebugMenuMerchant( m_infoView, info );
						data.overview.GetWidget().Show( false );
					}

					data.overview.UpdateFromInfo( info );
				}
			}
		}
	}
}