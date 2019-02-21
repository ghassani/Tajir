/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirStoreConfig - Data read from JSON configuration
 */
class TajirStoreConfig
{
	int 									id;
	string 									name;
	string 									type;
	bool 									log_transactions;
	int 									save_every_minutes;
	int 									restock_every_minutes;
	bool 									show_out_of_stock;
	ref array<int> 							allowed_conditions;
	ref array<ref TajirStoreCatalogConfig>  catalogs;

	/**
	 * @brief      { function_description }
	 */
	void TajirStoreConfig()
	{
		type 					= "default";
		log_transactions 		= false;
		save_every_minutes 		= 0;
		restock_every_minutes 	= 0;
		show_out_of_stock 		= false;
		catalogs 			= new array<ref TajirStoreCatalogConfig>;
		allowed_conditions	= new array<int>;
	}
}