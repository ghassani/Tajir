/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreTypeDefault extends TajirStoreTypeBase
{
	protected Widget m_root 	= NULL;
	protected Widget m_catalog 	= NULL;

	/**
	 * @brief      Constructor
	 */
	void TajirStoreTypeDefault()
	{
		m_name = "default";
	}

	/**
	 * @brief      Creates a catalog root widget.
	 *
	 * @return     { description_of_the_return_value }
	 */
	override Widget CreateCatalogRootWidget()
	{
		m_root = GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/store.layout" );
		return m_root;
	}

	/**
	 * @brief      Creates a catalog widget.
	 *
	 * @param[in]  root  The root
	 *
	 * @return     Widget
	 */
	override Widget CreateCatalogWidget( Widget root )
	{
		m_catalog = GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/store_default.layout", root );	
		
		if ( root != m_root )
		{
			m_root = root;
		}

		return m_catalog;
	}

	/**
	 * @brief      Processes a transaction.
	 *
	 * @param[in]  TajirMerchant          The tajir merchant
	 * @param[in]  TajirStore             The tajir store
	 * @param[in]  TajirStoreTransaction  The tajir store transaction
	 *
	 * @return     bool
	 */
	override bool ProcessTransaction( notnull TajirMerchant merchant, notnull TajirStore store, notnull TajirStoreTransaction transaction )
	{
		if ( transaction.GetRequest().GetType() == TajirStoreTransactionRequestType.Buy )
		{
			return ProcessBuyTransaction( merchant, store, transaction );			
		}
		else if ( transaction.GetRequest().GetType() == TajirStoreTransactionRequestType.Sell )
		{
			return ProcessSellTransaction( merchant, store, transaction );
		}

		TajirLogE( string.Format( "Process Transaction Unable to handle transaction type %1", transaction.GetRequest().GetType() ),  ClassName() );
		
		transaction.SetError( TajirStoreTransactionError.ConfigurationError );

		return false;
	}

	/**
	 * @brief      Processes a buy transaction.
	 *
	 * @param[in]  TajirMerchant          The tajir merchant
	 * @param[in]  TajirStore             The tajir store
	 * @param[in]  TajirStoreTransaction  The tajir store transaction
	 *
	 * @return     bool
	 */
	protected bool ProcessBuyTransaction( TajirMerchant merchant, TajirStore store, TajirStoreTransaction transaction )
	{
		TajirStoreCatalogItem item;
		int requiredFunds;
		int muzzleIndex;

		if ( !merchant.PlayerNearMerchant( transaction.GetPlayer() ) )
		{
			//TajirLogD( string.Format( "Player Not Near Merchant for Transaction ID %1 Player ID %2", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );

			transaction.SetError( TajirStoreTransactionError.NotNearMerchant );

			return false;
		}
		
		item = store.GetItem( transaction.GetRequest().GetItemId() );

		if ( !item )
		{
			//TajirLogD( string.Format( "Invalid Inventory for Transaction ID %1 Player ID %2", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );

			transaction.SetError( TajirStoreTransactionError.InvalidItem );

			return false;
		}

		if ( item.GetQuantity() <= 0 )
		{
			//TajirLogD( string.Format( "Out of Stock for Transaction ID %1 Player ID %2", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );

			transaction.SetError( TajirStoreTransactionError.OutOfStock );

			return false;
		}

		if ( item.GetQuantity() < transaction.GetRequest().GetQuantity() )
		{
			//TajirLogD( string.Format( "Insufficient Stock for Transaction ID %1 Player ID %2", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );

			transaction.SetError( TajirStoreTransactionError.OutOfStock );

			return false;
		}

		if ( GetGame().IsKindOf( item.GetObjectName(), "Transport" ) )
		{
			if ( !merchant.HasTransportBay() )
			{
				TajirLogE( string.Format( "No Merchant Transportation Bay For Item %1", item.GetObjectName() ) );
				transaction.SetError( TajirStoreTransactionError.NoTransportationBay );
				return false;
			}

			TajirMerchantTransportBay tb = merchant.FindFreeTransportationBayFor( item.GetObjectName() );

			if ( !tb )
			{
				TajirLogE( string.Format( "No Merchant Transportation Bay Found Able to Fit Item %1", item.GetObjectName() ) );
				transaction.SetError( TajirStoreTransactionError.TransportationBayNotClear );
				return false;
			}

			transaction.SetTransportBay( tb );
		}

		requiredFunds = item.GetSellPrice() * transaction.GetRequest().GetQuantity();

		if ( transaction.GetPlayer().GetCurrency() < requiredFunds )
		{
			//TajirLogD( string.Format( "Insufficient Funds for Transaction ID %1 Player ID %2", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );

			transaction.SetError( TajirStoreTransactionError.NotEnoughFunds );

			return false;
		}

		for ( int i = 0; i < transaction.GetRequest().GetQuantity(); i++ )
		{
			if ( item.IsBundle() )
			{
				if ( !CreateTransactionBundleItems( merchant, store, transaction, item ) )
				{
					return false;
				}
			}
			else if ( !CreateTransactionItem( merchant, store, transaction, item ) )
			{
				return false;
			}
		}

		transaction.GetPlayer().DeductCurrency( requiredFunds );

		transaction.SetAmount( -requiredFunds );

		item.DeductQuantity( transaction.GetRequest().GetQuantity() );

		TajirStoreComponent.GetInstance().NotifyPlayersInventoryUpdate( item );

		transaction.SetError( TajirStoreTransactionError.NoError );

		return true;
	}

	/**
	 * @brief      Creates a transaction entity item. Created items are added into the transaction objects array.
	 *
	 * @param[in]  TajirMerchant     		The merchant
	 * @param[in]  TajirStore        		The store
	 * @param[in]  TajirStoreTransaction  	The transaction
	 * @param[in]  TajirStoreCatalogItem    The item
	 *
	 * @return     bool 
	 */
	protected bool CreateTransactionItem( TajirMerchant merchant, TajirStore store, TajirStoreTransaction transaction, TajirStoreCatalogItem item )
	{
		HumanInventory inventory = transaction.GetPlayer().GetHumanInventory();
		vector pos;

		pos = transaction.GetPlayer().GetPosition();

		if ( GetGame().IsKindOf( item.GetObjectName(), "Transport" ) )
		{
			pos = transaction.GetTransportBay().GetPosition();
		}
		
		EntityAI object = EntityAI.Cast( GetGame().CreateObject( item.GetObjectName(), pos, false, TajirSpawnHelper.EntityRequiresAI( item.GetObjectName() ), true ) );

		if ( !object )
		{
			TajirLogE( string.Format( "Invalid Object Type %1", item.GetObjectName() ), ClassName() );
			transaction.SetError( TajirStoreTransactionError.InvalidItem );
			return false;
		}

		TajirSpawnHelper.SetUpNewEntity( object );

		if ( object.IsTransport() )
		{
			if ( object.IsKindOf( "CarScript" ) )
			{
				CarScript car = CarScript.Cast( object );

				if ( car && !car.HasOwner() )
				{
					car.SetOwner( transaction.GetPlayer() );
				}
			}
			else
			{
				TajirLogE( string.Format( "[CreateTransactionItem] Unhandled Transport Type %1", object.GetType() ), ClassName() );
			}
		}
		else
		{
			if ( !inventory.CanAddEntityIntoInventory( object ) && !inventory.CanAddEntityIntoHands( object ) )
			{
				TajirLogE( string.Format( "[CreateTransactionItem] No Room For Item %1", object.GetType() ) );
				transaction.SetError( TajirStoreTransactionError.NotEnoughRoom );
				object.Delete();
				return false;
			}

			if ( !inventory.TakeEntityToInventory( InventoryMode.SERVER, FindInventoryLocationType.ANY, object ) )
			{
				TajirLogE( string.Format( "[CreateTransactionItem] Could Not Take %1 To Cargo or Hands", object.GetType() ) );

				transaction.SetError( TajirStoreTransactionError.NotEnoughRoom );

				object.Delete();

				return false;
			}
		}

		transaction.AddObject( object );
			
		return true;		
	}

	/**
	 * @brief      Creates a transaction bundle entity items. Created top level items are added into the transaction objects array.
	 *
	 * @param[in]  TajirMerchant     		The merchant
	 * @param[in]  TajirStore        		The store
	 * @param[in]  TajirStoreTransaction  	The transaction
	 * @param[in]  TajirStoreCatalogItem    The item
	 *
	 * @return     bool 
	 */
	protected bool CreateTransactionBundleItems( TajirMerchant merchant, TajirStore store, TajirStoreTransaction transaction, TajirStoreCatalogItem item )
	{
		HumanInventory inventory = transaction.GetPlayer().GetHumanInventory();
		array<ref TajirItemBundle> bundleItems = item.GetBundleItems();
		EntityAI root = NULL;
		  
		// if a parent object is specified, it acts as source for item attachments defined in the bundle items
		// otherwise each item defined in the bundle items will be created as seperate items.

		if ( item.GetObjectName().Length() )
		{
			vector pos = transaction.GetPlayer().GetPosition();

			if ( GetGame().IsKindOf( item.GetObjectName(), "Transport" ) )
			{				
				pos = transaction.GetTransportBay().GetPosition();
			}

			root = EntityAI.Cast( GetGame().CreateObject( item.GetObjectName(), pos, false, TajirSpawnHelper.EntityRequiresAI( item.GetObjectName() ), true ) );
			
			if ( !root )
			{
				TajirLogE( string.Format( "[CreateTransactionBundleItems] Invalid Item Object Type %1", item.GetObjectName() ) );

				transaction.SetError( TajirStoreTransactionError.ConfigurationError );
				return false;
			}
			
			TajirSpawnHelper.SetUpNewEntity( root );

			transaction.AddObject( root );

			TajirLogD( string.Format( "[CreateTransactionBundleItems] Created Root Bundle Item: %1", root.GetType() ), ClassName() );
		}

		foreach ( ref TajirItemBundle bundleItem : bundleItems )
		{			
			if ( !CreateBundleItemObject( merchant, store, transaction, bundleItem, root ) )
			{
				return false;
			}
		}

		array<EntityAI> objects = transaction.GetObjects();

		foreach ( EntityAI object : objects  )
		{
			if ( object.IsTransport() )
			{
				if ( object.IsKindOf( "CarScript" ) )
				{
					CarScript car = CarScript.Cast( object );

					if ( car && !car.HasOwner() )
					{
						car.SetOwner( transaction.GetPlayer() );
					}
				}
				else
				{
					TajirLogE( string.Format( "[CreateTransactionBundleItems] Unhandled Transport Type %1", object.GetType() ), ClassName() );
				}

				continue;
			}

			if ( !inventory.CanAddEntityIntoInventory( object ) && !inventory.CanAddEntityIntoHands( object ) )
			{
				TajirLogE( string.Format( "[CreateTransactionBundleItems] No Room For Item %1", object.GetType() ) );
				transaction.SetError( TajirStoreTransactionError.NotEnoughRoom );
				return false;
			} 
			else if ( !inventory.TakeEntityToInventory( InventoryMode.SERVER, FindInventoryLocationType.ANY, object ) )
			{
				TajirLogE( string.Format( "[CreateTransactionBundleItems] Error when taking entity to inventory. No Room For Item %1", object.GetType() ) );
				transaction.SetError( TajirStoreTransactionError.NotEnoughRoom );
				return false;
			}
		}

		transaction.SetError( TajirStoreTransactionError.NoError );

		return true;
	}

	/**
	 * @brief      Create a bundles item object
	 *
	 * @param[in]  TajirMerchant          	The tajir merchant
	 * @param[in]  TajirStore            	The tajir store
	 * @param[in]  TajirStoreTransaction  	The tajir store transaction
	 * @param[in]  TajirItemBundle The bundle item
	 * @param[in]  EntityAI|NULL  			The items parent object, to be attached to or inserted into its cargo
	 *
	 * @return     bool
	 */
	protected bool CreateBundleItemObject( TajirMerchant merchant, TajirStore store, TajirStoreTransaction transaction, notnull TajirItemBundle item,  EntityAI parent = NULL )
	{
		EntityAI object;
		Weapon weapon;
		Magazine magazine;
		InventoryLocation location;
		vector pos;

		bool isTransport 		= GetGame().IsKindOf( item.GetObjectName(), "Transport" );
		int quantity 			= item.GetQuantity();
		
		array<ref TajirItemBundle> children = item.GetChildren();

		if ( quantity <= 0 || ( isTransport && quantity > 1 ) )
		{
			quantity = 1;
		}

		pos = transaction.GetPlayer().GetPosition();

		if ( parent )
		{
			pos = parent.GetPosition();
		}

		// check if it is transportation, verify the merchant has a vehicle bay position and radius configured
		if ( isTransport )
		{
			if ( parent && parent.IsTransport() )
			{
				TajirLogE( string.Format( "[CreateBundleItemObject] Cannot attach Transport Bundle Item %1 to Parent Transport %2", item.GetObjectName(), parent.GetType() ), ClassName() );
				transaction.SetError( TajirStoreTransactionError.ConfigurationError );
				return false;
			}
			else if ( !merchant.HasTransportBay() )
			{
				TajirLogE( string.Format( "[CreateBundleItemObject] No Merchant Transportation Bay For Bundle Item %1", item.GetObjectName() ), ClassName() );
				transaction.SetError( TajirStoreTransactionError.NoTransportationBay );
				return false;
			}

			if ( !transaction.GetTransportBay() )
			{
				TajirMerchantTransportBay tb = merchant.FindFreeTransportationBayFor( item.GetObjectName() );

				if ( !tb )
				{
					TajirLogE( string.Format( "[CreateBundleItemObject] No Merchant Transportation Bay Found Able to Fit Item %1", item.GetObjectName() ) );
					transaction.SetError( TajirStoreTransactionError.TransportationBayNotClear );
					return false;
				}

				transaction.SetTransportBay( tb );
			}

			pos = transaction.GetTransportBay().GetPosition();
		}
		
		for ( int i = 0; i < quantity; i++ )
		{	
			object = NULL;

			if ( !parent )
			{
				object = EntityAI.Cast( GetGame().CreateObject( item.GetObjectName(), pos, false, TajirSpawnHelper.EntityRequiresAI( item.GetObjectName() ), true ) );
				
				if ( object )
				{
					transaction.AddObject( object );
				}
			}
			else
			{
				location = new InventoryLocation();

				if ( !parent.GetInventory().FindFirstFreeLocationForNewEntity( item.GetObjectName(), FindInventoryLocationType.ANY, location ) )
				{
					TajirLogE( string.Format( "[CreateBundleItemObject] No Available Placement for %1 as Attachment or Cargo of %2", item.GetObjectName(), parent.GetType() ), ClassName() );
					transaction.SetError( TajirStoreTransactionError.ConfigurationError );
					return false;
				}

				TajirLogD( string.Format( "[CreateBundleItemObject] Found Slot For %1 in %2 of %3", item.GetObjectName(), typename.EnumToString( InventoryLocationType, location.GetType() ), parent.GetType() ), ClassName() );

				object = parent.GetInventory().LocationCreateEntity( location, item.GetObjectName(), ECE_IN_INVENTORY, RF_DEFAULT );
			}

			if ( !object )
			{
				transaction.SetError( TajirStoreTransactionError.InvalidItem );
				return false;
			}

			TajirSpawnHelper.SetUpNewEntity( object );

			if ( parent )
			{
				TajirLogD( string.Format( "[CreateBundleItemObject] Created Bundle Item: %1 in Parent %2", object.GetType(), parent.GetType() ), ClassName() );
			}
			else
			{
				TajirLogD( string.Format( "[CreateBundleItemObject] Created Bundle Item: %1", object.GetType() ), ClassName() );
			}
			
			// recursive call for children
			// for each quantity needed of this item
			foreach ( ref TajirItemBundle child : children )
			{
				if ( !CreateBundleItemObject( merchant, store, transaction, child, object ) )
				{
					return false;
				}
			}
		}
		
		return true;
	}
	
	/**
	 * @brief      Processes a sell transaction.
	 *
	 * @param[in]  TajirMerchant          The tajir merchant
	 * @param[in]  TajirStore             The tajir store
	 * @param[in]  TajirStoreTransaction  The tajir store transaction
	 *
	 * @return     bool
	 */
	protected bool ProcessSellTransaction( TajirMerchant merchant, TajirStore store, TajirStoreTransaction transaction )
	{
		TajirStoreCatalogItem item;

		if ( transaction.GetRequest().GetQuantity() <= 0 )
		{
			TajirLogD( string.Format( "Invalid Quantity %1 Transaction ID %2 Player ID %3", transaction.GetRequest().GetQuantity(), transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );
			transaction.SetError( TajirStoreTransactionError.InvalidQuantity );
			return false;
		}
		else if ( !transaction.GetRequest().GetPlayerItem() )
		{
			TajirLogD( string.Format( "Missing Player Inventory for Transaction ID %1 Player ID %2", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );
			transaction.SetError( TajirStoreTransactionError.InvalidItem );
			return false;
		}

		item = store.GetItem( transaction.GetRequest().GetPlayerItem().GetType() );

		if ( !item )
		{
			TajirLogD( string.Format( "Invalid Inventory for Transaction ID %1 Player ID %2", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );

			transaction.SetError( TajirStoreTransactionError.InvalidItem );
			return false;
		}
		else if ( !item.CanBuy() )
		{
			TajirLogD( string.Format( "Invalid Inventory for Transaction ID %1 Player ID %2. Store unable to purchase.", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );
			transaction.SetError( TajirStoreTransactionError.InvalidItem );
			return false;
		}
		else if ( item.GetBuyPrice() <= 0 )
		{
			TajirLogD( string.Format( "Inventory item purchase price is misconfigured. Transaction ID %1 Player ID %2.", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );
			transaction.SetError( TajirStoreTransactionError.ConfigurationError );
			return false;
		} 
		else if ( !transaction.GetPlayer().GetHumanInventory().HasEntityInInventory( transaction.GetRequest().GetPlayerItem() ) )
		{
			TajirLogD( string.Format( "Player does not own item. Transaction ID %1 Player ID %2.", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );
			transaction.SetError( TajirStoreTransactionError.DoesntOwnItem );
			return false;
		} 
		else if ( !transaction.GetRequest().GetPlayerItem().IsEmpty() )
		{
			TajirLogD( string.Format( "Item contains attachments Transaction ID %1 Player ID %2.", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );
			transaction.SetError( TajirStoreTransactionError.ItemNotEmpty );
			return false;
		}

		int quantity = transaction.GetRequest().GetQuantity();

		transaction.GetPlayer().AddCurrency( item.GetBuyPrice() * quantity );

		transaction.SetAmount( item.GetBuyPrice() );

		if ( transaction.GetRequest().GetPlayerItem().IsItemBase() )
		{
			ItemBase itemBase = ItemBase.Cast( transaction.GetRequest().GetPlayerItem() );

			if ( itemBase.HasQuantity() )
			{
				if ( !itemBase.CanBeSplit() || itemBase.IsKindOf( "Edible_Base" ) || itemBase.IsLiquidContainer() )
				{
					itemBase.Delete();
				}
				else
				{
					itemBase.AddQuantity( -quantity );

					if ( itemBase.GetQuantity() <= 0 )
					{
						itemBase.Delete();
					}
				}
			}
			else
			{
				itemBase.Delete();
			}
		}
		else
		{
			transaction.GetRequest().GetPlayerItem().Delete();
		}		

		if ( item.CanSell() )
		{
			item.AddQuantity( quantity );

			TajirStoreComponent.GetInstance().NotifyPlayersInventoryUpdate( item );
		}

		transaction.SetError( TajirStoreTransactionError.NoError );
		
		return true;
	}
}