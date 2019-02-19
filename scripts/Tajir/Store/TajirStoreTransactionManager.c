/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreTransactionManager
{
	protected TajirStoreComponent m_stores;

	protected ref TajirStoreTransactionLogger 	m_logger;

	protected ref array<ref TajirStoreTransaction> m_queue;

	protected int m_last_transaction_id = 0;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  stores  The stores
	 */
	void TajirStoreTransactionManager( TajirStoreComponent stores )
	{
		m_stores = stores;

		if ( GetGame().IsServer() )
		{
			m_queue  = new array<ref TajirStoreTransaction>;
			m_logger = new TajirStoreTransactionLogger( TAJIR_TRANSACTION_LOG_FILE );
		}
	}

	/**
	 * @brief      Destroys the object.
	 */
	void ~TajirStoreTransactionManager()
	{
		
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  PlayerBase                    The player base
	 * @param[in]  TajirStoreTransactionRequest  The tajir store transaction request
	 *
	 * @return     { description_of_the_return_value }
	 */
	int QueueRequestForProcessing( notnull PlayerBase player, notnull ref TajirStoreTransactionRequest request )
	{
		m_queue.Insert( new TajirStoreTransaction( player, request, GetNextTransactionId() ) );

		return m_queue.Count();
	}

	/**
	 * @brief      { function_description }
	 */
	void ProcessQueue()
	{
		foreach ( ref TajirStoreTransaction transaction : m_queue )
		{
			ProcessTransaction( transaction );
		}

		m_queue.Clear();
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirStoreTransaction  The tajir store transaction
	 */
	void ProcessTransaction( notnull TajirStoreTransaction transaction )
	{
		TajirMerchant merchant;
		TajirStore store;

		TajirLogD( string.Format( "Processing Transaction ID %1 Player ID %2", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );

		merchant = TajirMerchantComponent.GetInstance().GetMerchant( transaction.GetRequest().GetMerchantId() );

		if ( !merchant )
		{
			TajirLogE( string.Format( "Invalid Merchant for Transaction ID %1 Player ID %2", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );

			transaction.SetError( TajirStoreTransactionError.InvalidMerchant );
		}
		else
		{
			store = m_stores.GetStore( transaction.GetRequest().GetStoreId() );

			if ( !store || merchant.GetStore() != store ) // doesnt exist or trying to send a store not assigned to merchant
			{
				TajirLogE( string.Format( "Invalid Store for Transaction ID %1 Player ID %2", transaction.GetId(), transaction.GetPlayer().GetIdentity().GetId() ), ClassName() );
				
				transaction.SetError( TajirStoreTransactionError.InvalidStore );
			}
			else
			{
				if ( !store.IsValidStoreType() || store.GetTypeHandler() == NULL )
				{
					TajirLogE( "Invalid Store Type Configuration", ClassName() );
					
					transaction.SetError( TajirStoreTransactionError.ConfigurationError );
				}
				else
				{
					// actually process the transaction from the type handler
					if ( !store.GetTypeHandler().ProcessTransaction( merchant, store, transaction ) )
					{
						// the transaction processing reported error, but if no error was set, set an unknown error
						if ( !transaction.IsError() )
						{
							transaction.SetError( TajirStoreTransactionError.UnknownError );
						}
					}
				}
			}
		}

		if ( store && store.ShouldLogTransactions() )
		{
			m_logger.LogTransaction( transaction );
		}

		if ( transaction.IsError() && transaction.GetObjects().Count() )
		{
			TajirLogD( "Removing Objects Created Before Error", ClassName() );
			
			transaction.DeleteObjects();
		}

		GetGame().RPCSingleParam( transaction.GetPlayer(), TajirStoreComponentRPC.ServerRPC_TransactionResponse, new Param1<ref TajirStoreTransactionResponse>( new TajirStoreTransactionResponse( transaction ) ), true, transaction.GetPlayer().GetIdentity() );
	}

	/**
	 * @brief      Gets the last transaction identifier.
	 *
	 * @return     The last transaction identifier.
	 */
	int GetLastTransactionId()
	{
		return m_last_transaction_id;
	}

	/**
	 * { item_description }
	 */
	protected int GetNextTransactionId()
	{
		m_last_transaction_id++;
		return m_last_transaction_id;
	}
}