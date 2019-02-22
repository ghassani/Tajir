/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreMenuBase extends UIScriptedMenu
{
	protected TajirStoreComponent 				m_component;

	protected TajirStore 						m_store;

	protected TajirMerchant 					m_merchant;

	protected ref TajirStoreTransactionRequest 	m_transaction;
	
	/**
	 * @brief      Constructor
	 *
	 * @param[in]  TajirStore     The store
	 * @param[in]  TajirMerchant|NULL  The merchant
	 */
	void TajirStoreMenuBase( notnull TajirStoreComponent component, notnull TajirStore store, notnull TajirMerchant merchant )
	{		
		m_component = component;
		m_store 	= store;
		m_merchant 	= merchant;
	}
	
	/**
	 * @brief      Destructor
	 */
	void ~TajirStoreMenuBase()
	{
	}


	/**
	 * @brief      Initialize the root of the menu
	 *
	 * @return     Widget
	 */
	override Widget Init()
	{
		if ( !m_store.GetTypeHandler() )
		{
			TajirLogE( string.Format( "Store ID %1 Type Handler Invalid", m_store.GetId() ), ClassName() );
			return NULL;
		}

		layoutRoot = m_store.GetTypeHandler().CreateMenuWidget();
		
		if ( !layoutRoot )
		{
			TajirLogE( string.Format( "Store ID %1 Type Handler Did Not Create Widget", m_store.GetId() ), ClassName() );
			return NULL;
		}

		return layoutRoot;
	}
	
	/**
	 * @brief      Get the menu id.
	 *
	 * @return     int
	 */
	override int GetID()	{
		return TAJIR_MENU_STORE_ID;
	}
	
	/**
	 * @brief      Get the store.
	 *
	 * @return     TajirStore
	 */
	TajirStore GetStore()
	{
		return m_store;
	}

	/**
	 * @brief      Gets the merchant.
	 *
	 * @return     TajirMerchant
	 */
	TajirMerchant GetMerchant()
	{
		return m_merchant;
	}
	
	/**
	 * @brief      Locks the controls.
	 *
	 * @return     void
	 */
	override void LockControls()
	{
		super.LockControls();

		if ( GetGame().GetMission() )
		{
			GetGame().GetMission().PlayerControlDisable();
		}
	}

	/**
	 * @brief      Unlocks the controls.
	 *
	 * @return     void
	 */
	override void UnlockControls()
	{
		super.UnlockControls();
		
		if ( GetGame().GetMission() )
		{
			GetGame().GetMission().PlayerControlEnable();
		}
	}

	/**
	 * @brief      Called when the menu is hidden.
	 *
	 * @return     void
	 */
	override void OnHide()
	{
		super.OnHide();

		if ( m_component )
		{
			m_component.OnMenuClose();
		}
	}

	/**
	 * @brief      Gets the transaction.
	 *
	 * @return     The transaction.
	 */
	TajirStoreTransactionRequest GetTransaction()
	{
		return m_transaction;
	}

	/**
	 * @brief      Determines if it has transaction.
	 *
	 * @return     True if has transaction, False otherwise.
	 */
	bool HasTransaction()
	{
		return m_transaction != NULL;
	}

	/**
	 * @brief      Builds a human readable string for a players inventory item location.
	 */
	protected string BuildItemLocationString( notnull EntityAI item, InventoryLocation location )
	{
		switch ( location.GetType() )
		{
			case InventoryLocationType.ATTACHMENT:
				if ( location.GetParent().IsMan() )
				{
					return "Equipped";
				}
				else
				{
					return string.Format( "Attached to %1", location.GetParent().GetDisplayName() );
				}

				break;
			case InventoryLocationType.CARGO:
				return string.Format( "In %1", location.GetParent().GetDisplayName() );
				break;
			case InventoryLocationType.GROUND:
				return "Equipped";
				break;
			case InventoryLocationType.HANDS:
				return "Your Hands";
				break;
		}

		return "";
	}

	/**
	 * @brief      Update the players currency display
	 */
	void UpdatePlayerCurrency() {}

	/**
	 * @brief      Called when an inventory update RPC is received while the menu is open.
	 *
	 * @param[in]  ref array<TajirStore>  Array of stores that received updates.
	 */
	void OnInventoryUpdateReceived( ref array<TajirStore> updatedStores ) {}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  response  The response
	 */
	void OnTransactionResponseReceived( TajirStoreTransactionResponse response ) {}
}