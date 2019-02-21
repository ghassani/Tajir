/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirStoreCatalogItemConfig - Data read from JSON configuration
 */
class TajirStoreCatalogItemConfig
{
	int 	item_id;
	bool 	can_sell;
	bool 	can_buy;
	int 	quantity;
	int 	restock_quantity;

	/**
	 * @brief      { function_description }
	 */
	void TajirStoreCatalogItemConfig()
	{
		item_id				= -1;
		can_sell			= false;
		can_buy 			= false;
		quantity			= -1;
		restock_quantity	= -1;
	}
}