/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

enum TajirStoreTypeDefaultMenuTabBuyColumns
{
	Data = 0, // padding column used to store reference data
	Name,
	Quantity,
	Price,
	PadRight
}

enum TajirStoreTypeDefaultMenuTabSellColumns
{
	Data = 0, // padding column used to store reference data
	Name,
	Location,
	Quantity,
	Price,
	PadRight
}

/**
 * @brief      Handles the store menu for the default store type.
 */
class TajirStoreTypeDefaultMenu extends TajirStoreTypeMenuBase
{
	protected ButtonWidget 					m_buyToggle;

	protected Widget 						m_buyPanel;

	protected XComboBoxWidget 				m_buyCatalogs;

	protected TextListboxWidget				m_buyInventory;

	protected TextListboxWidget				m_sellInventory;
	
	protected ButtonWidget 					m_sellToggle;

	protected Widget 						m_sellPanel;

	protected ItemPreviewWidget				m_itemPreview;

	protected TextWidget 					m_previewItemTitle;

	protected TextWidget 					m_previewItemDescription;

	protected TextWidget 					m_previewItemConditionText;

	protected Widget 						m_previewItemConditionBg;

	protected ButtonWidget 					m_buyActionButton;

	protected ButtonWidget 					m_sellActionButton;

	protected Widget 						m_processingIndicator;

	protected ImageWidget 					m_processingIndicatorImage;

	protected ButtonWidget 					m_searchButton;
	
	protected ButtonWidget 					m_clearSearchButton;

	protected EditBoxWidget 				m_searchText;

	protected EditBoxWidget 				m_buyQuantity;

	protected int 							m_processingImageRotation = 0;

	protected int 							m_buyAllItemsCatalogIndex = -1;

	protected ref array<ref TajirStoreItemListData> m_buyListData;

	protected ref array<ref TajirStoreItemListData> m_sellListData;

	protected ref array<TajirStoreCatalog> m_catalogs;

	/**
	 * @brief      Constructor
	 */
	void TajirStoreTypeDefaultMenu()
	{
		m_buyListData  = new array<ref TajirStoreItemListData>;
		m_sellListData = new array<ref TajirStoreItemListData>;
	}

	/**
	 * @brief      Destructor
	 */
	void ~TajirStoreTypeDefaultMenu()
	{
		ClearBuyData();
		ClearSellData();

		if ( m_transaction )
		{
			delete m_transaction;
			m_transaction = NULL;
		}
	}

	/**
	 * @brief      Called when widget event is initialized.
	 *
	 * @param[in]  Widget    Root of this widget
	 *
	 * @return     void
	 */
	override void OnWidgetScriptInit( Widget w )
	{
		super.OnWidgetScriptInit( w );

		m_buyPanel 				 	= w.FindAnyWidget( "BuyTabPane" );
		m_sellPanel 			 	= w.FindAnyWidget( "SellTabPane" );
		m_previewItemConditionBg 	= w.FindAnyWidget( "SelectedItemConditionBackground" );
		m_processingIndicator 		= w.FindAnyWidget( "ProcessingIndicatorPanel" );
		m_processingIndicatorImage  = ImageWidget.Cast( w.FindAnyWidget( "ProcessIndicatorImage" ) );
		m_buyToggle 			 	= ButtonWidget.Cast( w.FindAnyWidget( "BuyTabButton" ) );
		m_sellToggle			 	= ButtonWidget.Cast( w.FindAnyWidget( "SellTabButton" ) );
		m_buyActionButton			= ButtonWidget.Cast( w.FindAnyWidget( "BuyActionButton" ) );
		m_sellActionButton			= ButtonWidget.Cast( w.FindAnyWidget( "SellActionButton" ) );
		m_buyCatalogs 	 		 	= XComboBoxWidget.Cast( w.FindAnyWidget( "BuyTabCatalogSelection" ) );
		m_buyInventory 			 	= TextListboxWidget.Cast( w.FindAnyWidget( "BuyTabInventoryList" ) );
		m_sellInventory 		 	= TextListboxWidget.Cast( w.FindAnyWidget( "SellTabInventoryList" ) );
		m_itemPreview			 	= ItemPreviewWidget.Cast( w.FindAnyWidget( "SelectedItemPreview" ) );
		m_previewItemTitle 			= TextWidget.Cast( w.FindAnyWidget( "SelectedItemTextTitle" ) );
		m_previewItemConditionText 	= TextWidget.Cast( w.FindAnyWidget( "SelectedItemCondition" ) );
		m_previewItemDescription 	= TextWidget.Cast( w.FindAnyWidget( "SelectedItemPreviewDescription" ) );
		m_searchButton 				= ButtonWidget.Cast( w.FindAnyWidget( "SearchButton" ) );
		m_clearSearchButton 		= ButtonWidget.Cast( w.FindAnyWidget( "ClearSearchButton" ) );
		m_searchText 				= EditBoxWidget.Cast( w.FindAnyWidget( "SearchText" ) );
		m_buyQuantity 				= EditBoxWidget.Cast( w.FindAnyWidget( "BuyQuantity" ) );

		PopulateBuyCatalogSelection();
		SetProcessingState( false );
		ToggleBuyPanel();
	}

	/**
	 * @brief      Populates the buy tab pane's catalog selection list.
	 */
	void PopulateBuyCatalogSelection()
	{
		m_buyCatalogs.ClearAll();

		m_catalogs 	= m_store.GetVisibleCatalogs();

		foreach ( TajirStoreCatalog catalog : m_catalogs )
		{
			m_buyCatalogs.AddItem( catalog.GetName() );
		}

		m_buyAllItemsCatalogIndex = -1;

		if ( m_catalogs.Count() > 1 )
		{
			m_buyAllItemsCatalogIndex = m_buyCatalogs.AddItem( "All Items" );

			m_buyCatalogs.SetCurrentItem( m_buyAllItemsCatalogIndex );
		}
		else if ( m_catalogs.Count() )
		{
			m_buyCatalogs.SetCurrentItem( 0 );
		}
	}

	/**
	 * @brief      Populates the currently selected catalogs inventory in the buy tab pane.
	 *
	 * @param[in]  catalogindex  The catalogindex
	 */
	void PopulateBuyCatalogItems( int catalogindex = 0 )
	{

		TajirStoreCatalog selected;

		if ( m_buyAllItemsCatalogIndex != -1 && catalogindex == m_buyAllItemsCatalogIndex )
		{
			ClearItemPreview();
			
			ClearBuyData();

			m_buyInventory.ClearItems();

			foreach ( TajirStoreCatalog catalog : m_catalogs )
			{
				AddCatalogToBuyInventoryList( catalog );
			}
		}
		else
		{
			selected = m_catalogs.Get( catalogindex );

			if ( !selected )
			{
				TajirLogE( string.Format( "Store ID %1 - Catalog %2 Not Found", m_store.GetId(), catalogindex ) );
			}
			else
			{
				ClearItemPreview();
				
				ClearBuyData();

				m_buyInventory.ClearItems();

				AddCatalogToBuyInventoryList( selected );
			}
		}
	}

	/**
	 * { item_description }
	 */
	protected void AddCatalogToBuyInventoryList( notnull TajirStoreCatalog catalog )
	{
		ref TajirStoreItemListData data;
		int row = 0;

		if ( m_buyInventory.GetNumItems() > 0 )
		{
			row = m_buyInventory.GetNumItems() -  1;
		}
		
		array<ref TajirStoreCatalogItem> items = catalog.GetItems();

		string searchText = m_searchText.GetText();

		if ( searchText.Length() )
		{
			searchText.ToLower();
		}

		foreach ( ref TajirStoreCatalogItem item : items )
		{
			if ( searchText.Length() )
			{
				string itemText = item.GetDisplayName();

				itemText.ToLower();

				if ( !itemText.Contains( searchText ) )
				{
					continue;
				}
			}

			if ( item.CanSell() && ( item.GetQuantity() > 0 || m_store.ShouldShowOutOfStockItems() ) )
			{
				data = new TajirStoreItemListData( item );

				m_buyListData.Insert( data );

				m_buyInventory.AddItem( "", data, TajirStoreTypeDefaultMenuTabBuyColumns.Data, row );

				m_buyInventory.SetItem( row, item.GetDisplayName(), NULL, TajirStoreTypeDefaultMenuTabBuyColumns.Name );
				m_buyInventory.SetItem( row, item.GetQuantity().ToString(), NULL, TajirStoreTypeDefaultMenuTabBuyColumns.Quantity );
				m_buyInventory.SetItem( row, string.Format("%1%2", TajirCurrencyComponent.GetInstance().GetCurrencySymbol(), item.GetSellPrice() ), NULL, TajirStoreTypeDefaultMenuTabBuyColumns.Price );

				row++;
			}
		}
	}

	/**
	 * @brief      Populates the sell tab inventory list with the players inventory.
	 */
	void PopulateSellItems()
	{
		string location;
		ref TajirStoreItemListData data;
		TajirStoreCatalogItem catalogItem;
		ItemBase itemBase;

		ref array<EntityAI> items 		= new array<EntityAI>;	
		InventoryLocation itemLocation 	= new InventoryLocation();
		int row 						= 0;

		GetGame().GetPlayer().GetHumanInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );

		m_sellInventory.ClearItems();
		
		ClearSellData();

		string searchText = m_searchText.GetText();

		if ( searchText.Length() )
		{
			searchText.ToLower();
		}

		foreach ( EntityAI item : items )
		{
			if ( item.GetType() == "" )
			{
				//TajirLogD( string.Format( "Skipping Empty Type - Name: %1", item.GetDisplayName() ), ClassName() );
				continue;
			}

			catalogItem = m_store.GetItem( item.GetType() );
			
			if ( catalogItem == NULL )
			{
				//TajirLogD( string.Format( "Player Inventory %1 [ %2 ] Not Sellable In this Store - No Inventory Found", item.GetType(),  item.GetDisplayName() ), ClassName() );
				continue;
			}
			else if ( !catalogItem.CanBuy() )
			{
				//TajirLogD( string.Format( "Player Inventory %1 [ %2 ] Not Sellable In this Store - Marked Cannot Buy", item.GetType(),  item.GetDisplayName() ), ClassName() );
				continue;
			} else if ( !m_store.AcceptsItemCondition( item.GetHealthLevel() ) )
			{
				//TajirLogD( string.Format( "Player Inventory %1 [ %2 ] Not Sellable In this Store - Invalid Condition %3", item.GetType(),  item.GetDisplayName(), item.GetHealthLevel() ), ClassName() );
				continue;
			}

			if ( searchText.Length() )
			{
				string itemText = catalogItem.GetDisplayName();

				itemText.ToLower();

				if ( !itemText.Contains( searchText ) )
				{
					continue;
				}
			}
			
			location = "";

			if ( item.GetInventory().GetCurrentInventoryLocation( itemLocation ) )
			{
				location = BuildItemLocationString( item, itemLocation );
			}

			data = new TajirStoreItemListData( catalogItem, item );

			m_sellListData.Insert( data );

			m_sellInventory.AddItem( "", data, TajirStoreTypeDefaultMenuTabSellColumns.Data, row );

			m_sellInventory.SetItem( row, catalogItem.GetDisplayName(), NULL, TajirStoreTypeDefaultMenuTabSellColumns.Name );
			m_sellInventory.SetItem( row, location, NULL, TajirStoreTypeDefaultMenuTabSellColumns.Location );
			
			itemBase = ItemBase.Cast( item );

			int quantity = 1;
			
			if ( itemBase.HasQuantity() )
			{
				if ( itemBase.CanBeSplit() && ( !itemBase.IsKindOf( "Edible_Base" ) || !itemBase.IsLiquidContainer() ) )
				{
					quantity = itemBase.GetQuantity();
				}
			}
			
			m_sellInventory.SetItem( row, quantity.ToString(), NULL, TajirStoreTypeDefaultMenuTabSellColumns.Quantity );						
			m_sellInventory.SetItem( row, string.Format("%1%2", TajirCurrencyComponent.GetInstance().GetCurrencySymbol(), catalogItem.GetBuyPrice() ), NULL, TajirStoreTypeDefaultMenuTabSellColumns.Price );
			
			row++;
		}
	}

	/**
	 * @brief      Updates the item preview when a user selects something from the buy inventory list.
	 *
	 * @param[in]  row     The row
	 * @param[in]  oldRow  The old row
	 */
	void UpdateItemPreviewFromBuySelection( int row, int oldRow )
	{
		Object object;
		TajirStoreItemListData data;

		ClearItemPreview();

		m_buyInventory.GetItemData( row, TajirStoreTypeDefaultMenuTabBuyColumns.Data, data );

		if ( data )
		{
			if ( !data.GetItem() )
			{
				TajirLogE( string.Format( "No Item for Data at Row %1 (Prev: %2)", row, oldRow ), ClassName() );
			}
			else if ( data.GetObject() != NULL )
			{
				UpdateItemPreview( data );
			}
			else
			{
				if ( !data.GetItem().IsBundle() )
				{
					object = GetGame().CreateObject( data.GetItem().GetObjectName(), Vector( 0, 0, 0 ), true, false, false );
				}
				else
				{
					
					if ( data.GetItem().GetDisplayObjectName().Length() )
					{
						object = GetGame().CreateObject( data.GetItem().GetObjectName(), Vector( 0, 0, 0 ), true, false, false );
					}
					else if ( data.GetItem().GetObjectName().Length() )
					{
						object = GetGame().CreateObject( data.GetItem().GetObjectName(), Vector( 0, 0, 0 ), true, false, false );
					}
				}

				data.SetObject( EntityAI.Cast( object ) );
				
				UpdateItemPreview( data );
			}
		}
	}

	/**
	 * @brief     Updates the item preview when a user selects something from the sell inventory list.
	 *
	 * @param[in]  row     The row
	 * @param[in]  oldRow  The old row
	 */
	void UpdateItemPreviewFromSellSelection( int row, int oldRow )
	{
		TajirStoreItemListData data;

		ClearItemPreview();

		m_sellInventory.GetItemData( row, TajirStoreTypeDefaultMenuTabSellColumns.Data, data );

		if ( data )
		{
			UpdateItemPreview( data );
		}
	}

	/**
	 * @brief      Updates the item preview widget.
	 *
	 * @param[in]  TajirStoreItemListData  The row data
	 */
	void UpdateItemPreview( notnull TajirStoreItemListData data )
	{
		Object object = data.GetObject();

		if ( !data.GetItem() )
		{
			TajirLogE( string.Format( "No Item in Data" ), ClassName() );
			return;
		}

		m_itemPreview.SetItem( EntityAI.Cast( object ) );

		m_previewItemTitle.SetText( data.GetItem().GetDisplayName() );
		m_previewItemDescription.SetText( data.GetItem().GetDescription() );
			
		if ( object )
		{
			string conditionText = "";
			int conditionColor 	 = Colors.BLACK;
			int textColor 	 	 = Colors.WHITE;

			switch ( object.GetHealthLevel() )
			{
				case STATE_PRISTINE:

					conditionText = "PRESTINE";
					conditionColor = Colors.COLOR_PRISTINE;
					break;
				case STATE_WORN:
					conditionText = "WORN";
					conditionColor = Colors.COLOR_WORN;
					break;
				case STATE_DAMAGED:
					conditionText = "DAMAGED";
					conditionColor = Colors.COLOR_DAMAGED;
					break;
				case STATE_BADLY_DAMAGED:
					conditionText = "BADLY DAMAGED";
					conditionColor = Colors.COLOR_BADLY_DAMAGED;
					break;
				case STATE_RUINED:
					conditionText = "RUINED";
					conditionColor = Colors.COLOR_RUINED;
					break;
			}
			
			m_previewItemConditionBg.Show( true );
			m_previewItemConditionText.SetText( conditionText );
			m_previewItemConditionText.SetColor( textColor  | 0xFF000000 );			
			m_previewItemConditionBg.SetColor( conditionColor  | 0x7F000000 );
		}
		else
		{
			m_previewItemConditionBg.Show( false );
		}
	}

	/**
	 * @brief      Clears the item preview widgets.
	 */
	void ClearItemPreview()
	{
		m_itemPreview.SetItem( NULL );
		m_previewItemTitle.SetText( "" );
		m_previewItemDescription.SetText( "" );
		m_previewItemConditionText.SetText( "" );
		m_previewItemConditionBg.SetColor( Colors.BLACK | 0xFF000000 );
	}

	/**
	 * @brief      Toggles the Buy Tab
	 */
	protected void ToggleBuyPanel()
	{
		m_sellInventory.ClearItems();
		
		ClearSellData();

		if ( m_buyAllItemsCatalogIndex != -1 )
		{
			PopulateBuyCatalogItems( m_buyAllItemsCatalogIndex );
		} 
		else
		{
			PopulateBuyCatalogItems( 0 );
		}
		

		m_buyPanel.Show( true );
		m_sellPanel.Show( false );

		m_buyToggle.SetColor( 0xFF535353  );
		m_sellToggle.SetColor( 0xFF0C0E0E );

		ClearItemPreview();
	}

	/**
	 * @brief      Toggles the Sell Tab
	 */
	protected void ToggleSellPanel()
	{
		m_buyInventory.ClearItems();
		ClearBuyData();

		PopulateSellItems();

		m_buyPanel.Show( false );
		m_sellPanel.Show( true );
		m_buyToggle.SetColor( 0xFF0C0E0E );
		m_sellToggle.SetColor( 0xFF535353 );

		ClearItemPreview();
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  timeslice  The timeslice
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void OnUpdate( float timeslice )
	{
		if ( m_transaction && m_processingIndicator.IsVisible() )
		{
			if ( ( m_transaction.GetTime() + ( TAJIR_CLIENT_TRANSACTION_TIMEOUT_SECONDS * 1000 ) ) < timeslice )
			{
				delete m_transaction;

				m_transaction = NULL;
				
				TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Transaction Request Timed Out", 2000 );

				TajirLogD( "Transaction Timed Out", ClassName() );

				return;
			}

			m_processingImageRotation += 5;
			m_processingIndicatorImage.SetRotation( 0, 0, m_processingImageRotation );
		}
	}

	/**
	 * @brief      Sets the state of the loading indication. Used when processing transactions with server.
	 */
	protected void SetProcessingState( bool state )
	{
		m_processingIndicator.Show( state );		
	}

	/**
	 * @brief      Clears all buy item list data.
	 */
	protected void ClearBuyData()
	{
		foreach ( ref TajirStoreItemListData data : m_buyListData )
		{
			delete data;
		}
		
		m_buyListData.Clear();
	}

	/**
	 * @brief      Clears all sell item list data.
	 */
	protected void ClearSellData()
	{
		foreach ( ref TajirStoreItemListData data : m_sellListData )
		{
			delete data;
		}
		
		m_sellListData.Clear();
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  w       { parameter_description }
	 * @param[in]  x       { parameter_description }
	 * @param[in]  y       { parameter_description }
	 * @param[in]  button  The button
	 *
	 * @return     { description_of_the_return_value }
	 */
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_buyToggle && !m_buyPanel.IsVisible() )
		{
			ToggleBuyPanel();
			return true;
		}
		else if ( w == m_sellToggle && !m_sellPanel.IsVisible() )
		{

			ToggleSellPanel();
			return true;
		} 
		else if ( w == m_buyActionButton )
		{

			ProcessBuyRequest();
			return true;
		}
		else if ( w == m_sellActionButton )
		{

			ProcessSellRequest();
			return true;
		}
		else if ( w == m_searchButton )
		{

			UpdateCurrentInventoryView();

			return true;
		}
		else if ( w == m_clearSearchButton )
		{
			m_searchText.SetText( "" );
			UpdateCurrentInventoryView();
		}

		return false;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  w         { parameter_description }
	 * @param[in]  x         { parameter_description }
	 * @param[in]  y         { parameter_description }
	 * @param[in]  finished  The finished
	 *
	 * @return     { description_of_the_return_value }
	 */
	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		if ( finished && w == m_buyCatalogs )
		{
			PopulateBuyCatalogItems( m_buyCatalogs.GetCurrentItem() );

			return true;
		}
		else if( finished && w == m_searchText )
		{
			UpdateCurrentInventoryView();
			return true;
		}
		else if( finished && w == m_buyQuantity )
		{
			if ( !m_buyQuantity.GetText().Length() || m_buyQuantity.GetText().ToInt() <= 0 )
			{
				m_buyQuantity.SetText( "1" );
			}
		}

		return false;
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  w          { parameter_description }
	 * @param[in]  x          { parameter_description }
	 * @param[in]  y          { parameter_description }
	 * @param[in]  row        The row
	 * @param[in]  column     The column
	 * @param[in]  oldRow     The old row
	 * @param[in]  oldColumn  The old column
	 *
	 * @return     { description_of_the_return_value }
	 */
	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		if ( w == m_buyInventory )
		{
			UpdateItemPreviewFromBuySelection( row, oldRow );
			return true;
		}
		else if ( w == m_sellInventory )
		{
			UpdateItemPreviewFromSellSelection( row, oldRow );
			return true;
		}

		return false;
	}

	/**
	 * @brief      Process a buy transaction request.
	 */
	void ProcessBuyRequest()
	{
		if ( m_buyInventory.GetSelectedRow() == -1 )
		{
			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Make a selection", 1000 );
			return;
		}
		else if ( m_transaction != NULL )
		{
			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Transaction in progress", 1000 );
			return;
		}
		
		TajirStoreItemListData data;

		m_buyInventory.GetItemData( m_buyInventory.GetSelectedRow(), TajirStoreTypeDefaultMenuTabSellColumns.Data, data );

		if ( data )
		{
			int quantity 	= m_buyQuantity.GetText().ToInt();

			if ( quantity <= 0 )
			{
				quantity = 1;
				m_buyQuantity.SetText( quantity.ToString() );
			}

			if ( data.GetItem().GetQuantity() < quantity )
			{
				TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Not enough stock", 1000 );
				return;
			}

			m_transaction 	= new TajirStoreTransactionRequest();

			m_transaction.Init( TajirStoreTransactionRequestType.Buy, m_merchant, m_store, data.GetItem(), quantity );

#ifdef TAJIR_DEBUG
			TajirLogD( string.Format( "Requesting Buy Transaction. Type: %1 Store ID: %2 Item ID: %3 Quantity: %4 Time: %5", m_transaction.GetType(), m_transaction.GetStoreId(), m_transaction.GetItemId(), m_transaction.GetQuantity(), m_transaction.GetTime() ), ClassName() );
#endif

			GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirStoreComponentRPC.ClientRPC_TransactionRequest, new Param1<ref TajirStoreTransactionRequest>( m_transaction ), true );
			
			SetProcessingState( true );
		}
	}

	/**
	 * @brief      Process a sell transaction request.
	 */
	void ProcessSellRequest()
	{
		if ( m_sellInventory.GetSelectedRow() == -1 )
		{
			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Make a selection", 1000 );
			return;
		}
		else if ( m_transaction != NULL )
		{
			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Transaction in progress", 1000 );
			return;
		}
		
		TajirStoreItemListData data;

		m_sellInventory.GetItemData( m_sellInventory.GetSelectedRow(), TajirStoreTypeDefaultMenuTabSellColumns.Data, data );

		if ( !data )
		{
			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Internal error", 1000 );
			return;
		}

		if ( !data.GetObject() )
		{
			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Invalid object", 1000 );
			return;
		} 
		else if ( GetGame().IsMultiplayer() && !data.GetObject().HasNetworkID() )
		{
			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Not a networked object", 1000 );
			return;
		}
		else if ( !data.GetObject().IsEmpty() )
		{
			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Remove cargo/attachments first", 1000 );
			return;
		}
		else if ( !m_store.AcceptsItemCondition( data.GetObject().GetHealthLevel() ) )
		{
			TajirManager.GetInstance().ShowNotification( TajirNotificationType.Error, "Can\t sell item in that condition", 1000 );
			return;
		}

		int quantity 	= 1; // TODO: let user input this?

		m_transaction 	= new TajirStoreTransactionRequest();

		m_transaction.Init( TajirStoreTransactionRequestType.Sell, m_merchant, m_store, data.GetObject(), quantity );

#ifdef TAJIR_DEBUG
		TajirLogD( string.Format( "Requesting Sell Transaction. Type: %1 Merchant ID: %2 Store ID: %3 Item ID: %4 Quantity: %5 Time: %6", m_transaction.GetType(), m_transaction.GetMerchantId(), m_transaction.GetStoreId(), m_transaction.GetItemId(), m_transaction.GetQuantity(), m_transaction.GetTime() ), ClassName() );
#endif
		
		GetGame().RPCSingleParam( GetGame().GetPlayer(), TajirStoreComponentRPC.ClientRPC_TransactionRequest, new Param1<ref TajirStoreTransactionRequest>( m_transaction ), true );
		
		SetProcessingState( true );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void OnInventoryUpdateReceived()
	{
		UpdateCurrentInventoryView();
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  response  The response
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void OnTransactionResponseReceived( TajirStoreTransactionResponse response )
	{
		delete m_transaction;
		
		m_transaction = NULL;

		UpdateCurrentInventoryView();

		SetProcessingState( false );
			
		// TODO: fix this ugliness
		//GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( this.UpdateCurrentInventoryView, 500, false );
		//GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( this.SetProcessingState, 600, false, false );
	}

	/**
	 * { item_description }
	 */
	protected void UpdateCurrentInventoryView()
	{
		int selectedCatalog = -1;
		int selectedItem 	= -1;
		int totalItems 		= 0;

		if ( m_buyPanel.IsVisible() )
		{
			selectedCatalog = m_buyCatalogs.GetCurrentItem();
			selectedItem 	= m_buyInventory.GetSelectedRow();

			if ( selectedCatalog >= 0 )
			{
				PopulateBuyCatalogItems( selectedCatalog );

				totalItems = m_buyInventory.GetNumItems();

				if ( selectedItem >= 0 && totalItems > 0 )
				{
					if ( ( selectedItem + 1 ) <= totalItems )
					{
						m_buyInventory.EnsureVisible( selectedItem );
						m_buyInventory.SelectRow( selectedItem );
					}
				}
			}
		}

		if ( m_sellPanel.IsVisible() )
		{
			selectedItem = m_sellInventory.GetSelectedRow();

			PopulateSellItems();

			totalItems = m_sellInventory.GetNumItems();

			if ( selectedItem >= 0 && totalItems > 0 )
			{
				if ( ( selectedItem + 1 ) <= totalItems )
				{
					m_sellInventory.EnsureVisible( selectedItem );
					m_sellInventory.SelectRow( selectedItem );
				}
			}
		}
	}
}