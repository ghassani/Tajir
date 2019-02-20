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
	protected TajirStoreComponent 			m_component;

	protected TajirStore 					m_store;

	protected TajirMerchant 				m_merchant;

	protected ref TajirStoreTypeMenuBase 	m_catalogMenuScript;

	/**
	 * @brief      Constructor
	 *
	 * @param[in]  TajirStore     The store
	 * @param[in]  TajirMerchant|NULL  The merchant
	 */
	void TajirStoreMenuBase( notnull TajirStoreComponent component, notnull TajirStore store, TajirMerchant merchant = NULL )
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
	 * @brief      Get the menu id.
	 *
	 * @return     int
	 */
	override int GetID()	{
		return TAJIR_MENU_STORE_ID;
	}
	
	/**
	 * @brief      Gets the script.
	 *
	 * @return     TajirStoreTypeMenuBase.
	 */
	TajirStoreTypeMenuBase GetScript()
	{
		return m_catalogMenuScript;
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