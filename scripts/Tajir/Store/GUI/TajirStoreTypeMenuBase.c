/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreTypeMenuBase extends ScriptedWidgetEventHandler
{
	protected TajirStoreMenu 	m_menu;

	protected TajirStore 		m_store;

	protected TajirMerchant 	m_merchant;

	protected ref TajirStoreTransactionRequest m_transaction;

	/**
	 * @brief      { function_description }
	 */
	void TajirStoreTypeMenuBase()
	{

	}

	/**
	 * @brief      Destroys the object.
	 */
	void ~TajirStoreTypeMenuBase()
	{

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
	 * @brief      { function_description }
	 *
	 * @param[in]  w     { parameter_description }
	 */
	void OnWidgetScriptInit( Widget w )
	{
		m_menu  	= TajirStoreMenu.Cast( GetGame().GetUIManager().GetMenu() );

		if ( m_menu != NULL )
		{
			m_store 	= m_menu.GetStore();
			m_merchant 	= m_menu.GetMerchant();
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  timeslice  The timeslice
	 */
	void OnUpdate( float timeslice )
	{
		
	}

	/**
	 * @brief      { function_description }
	 */
	void OnInventoryUpdateReceived()
	{
		
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  response  The response
	 */
	void OnTransactionResponseReceived( TajirStoreTransactionResponse response )
	{
		
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
}