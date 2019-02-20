/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreTypeBase
{
	protected string m_name;
	
	/**
	 * @brief      Gets the name.
	 *
	 * @return     The name.
	 */
	string GetName()
	{
		return m_name;
	}
	
	/**
	 * @brief      Creates a catalog root widget.
	 *
	 * @return     { description_of_the_return_value }
	 */
	Widget CreateCatalogRootWidget()
	{
		Error( "Method " + ClassName() + ":CreateCatalogRootWidget must be overloaded" );
		return NULL;
	}

	/**
	 * @brief      Creates a catalog widget.
	 *
	 * @param[in]  root  The root
	 *
	 * @return     { description_of_the_return_value }
	 */
	Widget CreateCatalogWidget( Widget root )
	{
		Error( "Method " + ClassName() + ":CreateCatalogWidget must be overloaded" );
		return NULL;
	}

	/**
	 * @brief      Creates a menu.
	 *
	 * @param[in]  TajirStoreComponent  The tajir store component
	 * @param[in]  TajirStore           The tajir store
	 * @param[in]  merchant             The merchant
	 *
	 * @return     { description_of_the_return_value }
	 */
	TajirStoreMenuBase CreateMenu( notnull TajirStoreComponent component, notnull TajirStore store, TajirMerchant merchant = NULL )
	{
		Error( "Method " + ClassName() + ":CreateMenu must be overloaded" );
		return NULL;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirMerchant          The tajir merchant
	 * @param[in]  TajirStore             The tajir store
	 * @param[in]  TajirStoreTransaction  The tajir store transaction
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool ProcessTransaction( notnull TajirMerchant merchant, notnull TajirStore store, notnull TajirStoreTransaction transaction )
	{
		Error( "Method " + ClassName() + ":ProcessTransaction must be overloaded" );

		transaction.SetError( TajirStoreTransactionError.ConfigurationError );

		return false;
	}
}