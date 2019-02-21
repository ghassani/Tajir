/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreTransactionResponse
{
	protected ref TajirStoreTransactionRequest m_request;

	protected TajirStoreTransactionError m_error;

	protected int m_id;

	protected int m_amount;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  transaction  The transaction
	 * @param[in]  request      The request
	 */
	void TajirStoreTransactionResponse( TajirStoreTransaction transaction = NULL, TajirStoreTransactionRequest request = NULL )
	{
		m_amount 	= 0;
		m_error 	= TajirStoreTransactionError.NoError;
		m_id 	 	= 0;

		if ( request && !transaction )
		{
			m_request = request;
		}
		else if ( !transaction )
		{
			m_request = new TajirStoreTransactionRequest();
		}

		if ( transaction )
		{
			m_request = transaction.GetRequest();
			m_amount  = transaction.GetAmount();
			m_id 	  = transaction.GetId();
			m_error   = transaction.GetError();
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirStoreTransaction  The tajir store transaction
	 */
	void UpdateFromTransaction( notnull TajirStoreTransaction transaction )
	{
		m_request = transaction.GetRequest();
		m_amount  = transaction.GetAmount();
		m_id 	  = transaction.GetId();
		m_error   = transaction.GetError();
	}

	/**
	 * @brief      Sets the request.
	 *
	 * @param[in]  TajirStoreTransactionRequest  The tajir store transaction request
	 */
	void SetRequest( notnull TajirStoreTransactionRequest request )
	{
		m_request = request;
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
	 * @brief      Gets the identifier.
	 *
	 * @return     The identifier.
	 */
	int GetId()
	{
		return m_id;
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
	 * @brief      Sets the error.
	 *
	 * @param[in]  error  The error
	 */
	void SetError( TajirStoreTransactionError error )
	{
		m_error = error;
	}

	/**
	 * @brief      Determines if error.
	 *
	 * @return     True if error, False otherwise.
	 */
	bool IsError()
	{
		if ( m_error == TajirStoreTransactionError.NoError )
		{
			return false;
		}

		return true;
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
}