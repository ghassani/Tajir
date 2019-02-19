/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

enum TajirStoreTransactionError
{
	NoError = 0,
	RequestError,
	ConfigurationError,
	InvalidMerchant,
	InvalidStore,
	InvalidItem,
	InvalidQuantity,
	OutOfStock,
	InsufficientStock,
	NotNearMerchant,
	NotEnoughFunds,
	NotEnoughRoom,
	DoesntOwnItem,
	NoTransportationBay,
	TransportationBayNotClear,
	ItemNotEmpty,
	RemoveMagazine,
	UnknownError
}

class TajirStoreTransaction
{
	protected PlayerBase 							m_player;

	protected ref TajirStoreTransactionRequest 		m_request;

	protected int 									m_id;

	protected int 									m_amount;

	protected TajirStoreTransactionError 			m_error;

	protected ref array<EntityAI> 					m_objects;

	protected TajirMerchantTransportBay 			m_transportBay = NULL;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  PlayerBase                    The player base
	 * @param[in]  TajirStoreTransactionRequest  The tajir store transaction request
	 * @param[in]  id                            The identifier
	 */
	void TajirStoreTransaction( notnull PlayerBase player, notnull ref TajirStoreTransactionRequest request, int id )
	{
		m_player 	= player;
		m_request	= request;
		m_id		= id;
		m_amount	= 0;
		m_error 	= TajirStoreTransactionError.NoError;
		m_objects 	= new array<EntityAI>;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  error  The error
	 *
	 * @return     { description_of_the_return_value }
	 */
	static string ErrorToString( TajirStoreTransactionError error )
	{
		switch( error )
		{
			case TajirStoreTransactionError.RequestError:
				return "Request Error";
			case TajirStoreTransactionError.ConfigurationError:
				return "Server Configuration Serror";
			case TajirStoreTransactionError.InvalidMerchant:
				return "Invalid Merchant";
			case TajirStoreTransactionError.InvalidStore:
				return "Invalid Store";
			case TajirStoreTransactionError.InvalidItem:
				return "Invalid Item";
			case TajirStoreTransactionError.InvalidQuantity:
				return "Invalid Quantity";
			case TajirStoreTransactionError.NotEnoughFunds:
				return "Not Enough Funds";
			case TajirStoreTransactionError.NotEnoughRoom:
				return "Not Enough Room";
			case TajirStoreTransactionError.NotNearMerchant:
				return "Not Near Merchant";
			case TajirStoreTransactionError.OutOfStock:
				return "Out of Stock";
			case TajirStoreTransactionError.InsufficientStock:
				return "Insufficient Stock";
			case TajirStoreTransactionError.DoesntOwnItem:
				return "Does Not Own Item";
			case TajirStoreTransactionError.NoTransportationBay:
				return "No Transportation Bay Configured";
			case TajirStoreTransactionError.TransportationBayNotClear:
				return "Transportation Bay Not Clear";
			case TajirStoreTransactionError.ItemNotEmpty:
				return "Item Not Empty";
			case TajirStoreTransactionError.RemoveMagazine:
				return "Remove Magazine";		
			case TajirStoreTransactionError.NoError:
				return "";
		}
		
		return "Unknown Error";
	}

	/**
	 * @brief      Gets the identifier.
	 *
	 * @return     The identifier.
	 */
	int GetId()
	{
		return m_id;
	}

	/**
	 * @brief      Gets the player.
	 *
	 * @return     The player.
	 */
	PlayerBase GetPlayer()
	{
		return m_player;
	}

	/**
	 * @brief      Gets the request.
	 *
	 * @return     The request.
	 */
	TajirStoreTransactionRequest GetRequest()
	{
		return m_request;
	}

	/**
	 * @brief      Sets the error.
	 *
	 * @param[in]  error  The error
	 */
	void SetError( TajirStoreTransactionError error )
	{
		m_error = error;
	}

	/**
	 * @brief      Gets the error.
	 *
	 * @return     The error.
	 */
	TajirStoreTransactionError GetError()
	{
		return m_error;
	}

	/**
	 * @brief      Determines if error.
	 *
	 * @return     True if error, False otherwise.
	 */
	bool IsError()
	{
		return m_error != TajirStoreTransactionError.NoError;
	}

	/**
	 * @brief      Gets the error string.
	 *
	 * @return     The error string.
	 */
	string GetErrorString()
	{
		return TajirStoreTransaction.ErrorToString( m_error );
	}
	
	/**
	 * @brief      Sets the amount.
	 *
	 * @param[in]  amount  The amount
	 */
	void SetAmount( int amount )
	{
		m_amount = amount;
	}

	/**
	 * @brief      Gets the amount.
	 *
	 * @return     The amount.
	 */
	int GetAmount()
	{
		return m_amount;
	}

	/**
	 * @brief      Gets the transport bay.
	 *
	 * @return     The transport bay.
	 */
	TajirMerchantTransportBay GetTransportBay()
	{
		return m_transportBay;
	}

	/**
	 * @brief      Sets the transport bay.
	 *
	 * @param[in]  transportBay  The transport bay
	 */
	void SetTransportBay( TajirMerchantTransportBay transportBay )
	{
		m_transportBay = transportBay;
	}

	/**
	 * @brief      Add an object into the transactions created objects array.
	 *
	 * @param[in]  object  The object
	 *
	 * @return     int Count of objects after insertion.
	 */
	int AddObject( EntityAI object )
	{
		m_objects.Insert( object );

		return m_objects.Count();
	}

	/**
	 * @brief      Get the created objects for the transaction
	 *
	 * @return     The created objects.
	 */
	array<EntityAI> GetObjects()
	{
		return m_objects;
	}

	/**
	 * @brief      { function_description }
	 */
	void DeleteObjects()
	{
		for ( int i = 0; i < m_objects.Count(); i++ )
		{
			m_objects[ i ].Delete();
		}

		m_objects.Clear();
	}
}