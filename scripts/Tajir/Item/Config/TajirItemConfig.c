/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirItemConfig - Data read from JSON configuration
 */
class TajirItemConfig
{
	int 	id;
	string 	object_name;
	string 	display_name;
	string 	description;
	string 	display_object_name;
	int 	sell_price;
	int 	buy_price;
	ref array<ref TajirItemBundleConfig> 	items;

	/**
	 * @brief      { function_description }
	 */
	void TajirStoreCatalogItemConfig()
	{
		items = new array<ref TajirItemBundleConfig>;
	}
}